/* GCC MELT GENERATED C++ FILE warmelt-macro+06.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #6 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f6[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-macro+06.cc declarations ****/
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



class Melt_InitialClassyFrame_WARMELTmiMACRO_h412462272; // forward declaration fromline 6518
typedef Melt_InitialClassyFrame_WARMELTmiMACRO_h412462272 Melt_InitialFrame;
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

typedef Melt_InitialClassyFrame_WARMELTmiMACRO_h412462272 /*opaqueinitialclassy*/ meltinitial_frame_t;


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


/**** warmelt-macro+06.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX fromline 1716 */

    /** start of frame for meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX// fromline 1531
        : public Melt_CallFrameWithValues<30>
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
        MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<30> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX), clos) {};
        MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX() //the constructor fromline 1606
            : Melt_CallFrameWithValues<30> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<30> (fil,lin, sizeof(MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX)) {};
        MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<30> (fil,lin, sizeof(MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX


    /** end of frame for meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX fromline 1661**/

    /* end of frame for routine meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX fromline 1720 */

    /* classy proc frame meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX */
    MeltFrame_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_CONSTANT_BOX", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7107:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7108:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:7108:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7108:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7108;
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7108:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7108:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7109:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:7109:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7109:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7109;
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7109:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7109:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7110:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L3*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7110:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7110:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7110;
                            /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                            meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V13*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7110:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                        meltfptr[13] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V12*/
            meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7110:/ clear");
            /*clear*/ /*_#IS_OBJECT__L3*/
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
        MELT_LOCATION("warmelt-macro.melt:7111:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7111:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7111:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7111;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_constant_box start sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V16*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V15*/
                        meltfptr[13] = /*_.MELT_DEBUG_FUN__V16*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7111:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/
                        meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V16*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V15*/ meltfptr[13] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7111:/ locexp");
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
            /*clear*/ /*_._IF___V15*/
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
        MELT_LOCATION("warmelt-macro.melt:7112:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[13] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7113:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V18*/ meltfptr[13])));;
        /*^compute*/
        /*_.CURPAIR__V21*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.BOXEDEXP__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:7117:/ cond");
        /*cond*/
        if (/*_.PAIR_TAIL__V23*/ meltfptr[22]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7118:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "CONSTANT_BOX should have only one argument"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7119:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7120:/ apply");
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
            /*_.BOXED__V25*/
            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.BOXEDEXP__V22*/ meltfptr[21]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7121:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_CONSTANT_BOX*/ meltfrout->tabval[4])), (3), "CLASS_SOURCE_CONSTANT_BOX");
            /*_.INST__V27*/
            meltfptr[26] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V27*/ meltfptr[26])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V27*/ meltfptr[26]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SBOXED", melt_magic_discr((melt_ptr_t)(/*_.INST__V27*/ meltfptr[26])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V27*/ meltfptr[26]), (2), (/*_.BOXED__V25*/ meltfptr[24]), "SBOXED");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V27*/ meltfptr[26], "newly made instance");
        ;
        /*_.SRC__V26*/
        meltfptr[25] = /*_.INST__V27*/ meltfptr[26];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7125:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            meltfnum[4] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:7125:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L6*/ meltfnum[4]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:7125:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7125;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_constant_box src=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SRC__V26*/ meltfptr[25];
                            /*_.MELT_DEBUG_FUN__V29*/
                            meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V28*/
                        meltfptr[27] = /*_.MELT_DEBUG_FUN__V29*/ meltfptr[28];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7125:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L7*/
                        meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V29*/
                        meltfptr[28] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V28*/ meltfptr[27] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7125:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L6*/
            meltfnum[4] = 0 ;
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7126:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SRC__V26*/ meltfptr[25];;

        {
            MELT_LOCATION("warmelt-macro.melt:7126:/ locexp");
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
        meltfptr[23] = /*_.RETURN___V30*/ meltfptr[28];;

        MELT_LOCATION("warmelt-macro.melt:7119:/ clear");
        /*clear*/ /*_.BOXED__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SRC__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V30*/
        meltfptr[28] = 0 ;
        /*_.LET___V17*/
        meltfptr[6] = /*_.LET___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-macro.melt:7112:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[13] = 0 ;
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
        /*clear*/ /*_.BOXEDEXP__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V24*/
        meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7107:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-macro.melt:7107:/ locexp");
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
        /*clear*/ /*_.IFCPP___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_CONSTANT_BOX", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX fromline 1716 */

    /** start of frame for meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX// fromline 1531
        : public Melt_CallFrameWithValues<50>
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
        MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<50> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX), clos) {};
        MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX() //the constructor fromline 1606
            : Melt_CallFrameWithValues<50> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<50> (fil,lin, sizeof(MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX)) {};
        MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<50> (fil,lin, sizeof(MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX


    /** end of frame for meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX fromline 1661**/

    /* end of frame for routine meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX fromline 1720 */

    /* classy proc frame meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX */
    MeltFrame_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_UNBOX", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7139:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7140:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:7140:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7140:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7140;
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7140:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7140:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7141:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:7141:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7141:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7141;
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7141:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7141:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7142:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L3*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7142:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7142:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7142;
                            /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                            meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V13*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7142:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                        meltfptr[13] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V12*/
            meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7142:/ clear");
            /*clear*/ /*_#IS_OBJECT__L3*/
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
        MELT_LOCATION("warmelt-macro.melt:7143:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7143:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7143:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7143;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_unbox start sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V16*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V15*/
                        meltfptr[13] = /*_.MELT_DEBUG_FUN__V16*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7143:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/
                        meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V16*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V15*/ meltfptr[13] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7143:/ locexp");
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
            /*clear*/ /*_._IF___V15*/
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
        MELT_LOCATION("warmelt-macro.melt:7144:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[13] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7145:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V18*/ meltfptr[13])));;
        /*^compute*/
        /*_.CURPAIR__V21*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.UBCTYPK__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:7149:/ compute");
        /*_.CURPAIR__V21*/
        meltfptr[20] = /*_.SETQ___V24*/ meltfptr[23] = /*_.PAIR_TAIL__V23*/ meltfptr[22];;
        /*_.PAIR_HEAD__V25*/
        meltfptr[24] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:7148:/ quasiblock");


        /*_.UBEXPR__V26*/
        meltfptr[25] = /*_.PAIR_HEAD__V25*/ meltfptr[24];;
        /*^compute*/
        /*_.PAIR_TAIL__V27*/
        meltfptr[26] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:7152:/ compute");
        /*_.CURPAIR__V21*/
        meltfptr[20] = /*_.SETQ___V28*/ meltfptr[27] = /*_.PAIR_TAIL__V27*/ meltfptr[26];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7153:/ cond");
        /*cond*/
        if (/*_.CURPAIR__V21*/ meltfptr[20]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7154:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "UNBOX expects two arguments: (UNBOX <ctype> <expr>)"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7155:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7155:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7153:/ quasiblock");


                    /*_.PROGN___V31*/
                    meltfptr[30] = /*_.RETURN___V30*/ meltfptr[29];;
                    /*^compute*/
                    /*_._IF___V29*/
                    meltfptr[28] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7153:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7156:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            meltfnum[4] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:7156:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L6*/ meltfnum[4]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:7156:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7156;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_unbox ubctypk=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.UBCTYPK__V22*/ meltfptr[21];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " ubexpr=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.UBEXPR__V26*/ meltfptr[25];
                            /*_.MELT_DEBUG_FUN__V33*/
                            meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V32*/
                        meltfptr[29] = /*_.MELT_DEBUG_FUN__V33*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7156:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L7*/
                        meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V33*/
                        meltfptr[30] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V32*/ meltfptr[29] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7156:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L6*/
            meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V32*/
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
        /*_#IS_NOT_A__L8*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.UBCTYPK__V22*/ meltfptr[21]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:7157:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L8*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7158:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "first argument to UNBOX should be a ctype keyword like :tree ...."), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7159:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7159:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7157:/ quasiblock");


                    /*_.PROGN___V36*/
                    meltfptr[35] = /*_.RETURN___V35*/ meltfptr[29];;
                    /*^compute*/
                    /*_._IF___V34*/
                    meltfptr[30] = /*_.PROGN___V36*/ meltfptr[35];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7157:/ clear");
                    /*clear*/ /*_.RETURN___V35*/
                    meltfptr[29] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V36*/
                    meltfptr[35] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V34*/ meltfptr[30] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7160:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.UBCTYPK__V22*/ meltfptr[21]),
                                          (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[5])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.UBCTYPK__V22*/ meltfptr[21]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "SYMB_DATA");
                    /*_.CTYP__V38*/
                    meltfptr[35] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.CTYP__V38*/ meltfptr[35] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L9*/
        meltfnum[4] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CTYP__V38*/ meltfptr[35]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[6])));;
        MELT_LOCATION("warmelt-macro.melt:7162:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L9*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7163:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "first argument to UNBOX should be a ctype"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7164:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7164:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7162:/ quasiblock");


                    /*_.PROGN___V41*/
                    meltfptr[40] = /*_.RETURN___V40*/ meltfptr[39];;
                    /*^compute*/
                    /*_._IF___V39*/
                    meltfptr[38] = /*_.PROGN___V41*/ meltfptr[40];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7162:/ clear");
                    /*clear*/ /*_.RETURN___V40*/
                    meltfptr[39] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V41*/
                    meltfptr[40] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V39*/ meltfptr[38] =  /*fromline 1341*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7165:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7165:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7165:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7165;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_unbox ctyp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CTYP__V38*/ meltfptr[35];
                            /*_.MELT_DEBUG_FUN__V43*/
                            meltfptr[40] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V42*/
                        meltfptr[39] = /*_.MELT_DEBUG_FUN__V43*/ meltfptr[40];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7165:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L11*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V43*/
                        meltfptr[40] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V42*/ meltfptr[39] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7165:/ locexp");
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
            /*clear*/ /*_._IF___V42*/
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
        MELT_LOCATION("warmelt-macro.melt:7166:/ quasiblock");



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
            /*_.MEXP__V45*/
            meltfptr[39] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.UBEXPR__V26*/ meltfptr[25]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7167:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_UNBOX*/ meltfrout->tabval[7])), (4), "CLASS_SOURCE_UNBOX");
            /*_.INST__V47*/
            meltfptr[46] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V47*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V47*/ meltfptr[46]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUNBOX_CTYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V47*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V47*/ meltfptr[46]), (2), (/*_.CTYP__V38*/ meltfptr[35]), "SUNBOX_CTYPE");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUNBOX_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V47*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V47*/ meltfptr[46]), (3), (/*_.MEXP__V45*/ meltfptr[39]), "SUNBOX_EXPR");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V47*/ meltfptr[46], "newly made instance");
        ;
        /*_.SUNBOX__V46*/
        meltfptr[45] = /*_.INST__V47*/ meltfptr[46];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7173:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7173:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7173:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[9];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7173;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_unbox mexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXP__V45*/ meltfptr[39];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " sunbox=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.SUNBOX__V46*/ meltfptr[45];
                            /*_.MELT_DEBUG_FUN__V49*/
                            meltfptr[48] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V48*/
                        meltfptr[47] = /*_.MELT_DEBUG_FUN__V49*/ meltfptr[48];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7173:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L13*/
                        meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V49*/
                        meltfptr[48] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V48*/ meltfptr[47] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7173:/ locexp");
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
            /*clear*/ /*_._IF___V48*/
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
        MELT_LOCATION("warmelt-macro.melt:7174:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SUNBOX__V46*/ meltfptr[45];;

        {
            MELT_LOCATION("warmelt-macro.melt:7174:/ locexp");
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
        meltfptr[40] = /*_.RETURN___V50*/ meltfptr[48];;

        MELT_LOCATION("warmelt-macro.melt:7166:/ clear");
        /*clear*/ /*_.MEXP__V45*/
        meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SUNBOX__V46*/
        meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V50*/
        meltfptr[48] = 0 ;
        /*_.LET___V37*/
        meltfptr[29] = /*_.LET___V44*/ meltfptr[40];;

        MELT_LOCATION("warmelt-macro.melt:7160:/ clear");
        /*clear*/ /*_.CTYP__V38*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L9*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V39*/
        meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V44*/
        meltfptr[40] = 0 ;
        /*_.LET___V17*/
        meltfptr[6] = /*_.LET___V37*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:7144:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[13] = 0 ;
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
        /*clear*/ /*_.UBCTYPK__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.UBEXPR__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L8*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V34*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V37*/
        meltfptr[29] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7139:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-macro.melt:7139:/ locexp");
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
        /*clear*/ /*_.IFCPP___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_UNBOX", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT fromline 1716 */

    /** start of frame for meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT// fromline 1531
        : public Melt_CallFrameWithValues<28>
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
        MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT), clos) {};
        MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT)) {};
        MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT


    /** end of frame for meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT fromline 1661**/

    /* end of frame for routine meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT fromline 1720 */

    /* classy proc frame meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT */
    MeltFrame_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_COMMENT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7185:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7186:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:7186:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7186:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7186;
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7186:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7186:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7187:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:7187:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7187:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7187;
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7187:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7187:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7188:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L3*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7188:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7188:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7188;
                            /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                            meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V13*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7188:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                        meltfptr[13] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V12*/
            meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7188:/ clear");
            /*clear*/ /*_#IS_OBJECT__L3*/
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
        MELT_LOCATION("warmelt-macro.melt:7189:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V16*/
            meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7190:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V17*/
            meltfptr[16] = slot;
        };
        ;
        /*_.LIST_FIRST__V18*/
        meltfptr[17] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V16*/ meltfptr[6])));;
        /*^compute*/
        /*_.CURPAIR__V19*/
        meltfptr[18] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V18*/ meltfptr[17])));;
        /*^compute*/
        /*_.COMSTR__V20*/
        meltfptr[19] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V21*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
        MELT_LOCATION("warmelt-macro.melt:7194:/ cond");
        /*cond*/
        if (/*_.PAIR_TAIL__V21*/ meltfptr[20]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7195:/ locexp");
                        melt_warning_str(0, (melt_ptr_t)(/*_.LOC__V17*/ meltfptr[16]), ( "COMMENT should have only one string argument"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L4*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.COMSTR__V20*/ meltfptr[19])) == MELTOBMAG_STRING);;
        /*^compute*/
        /*_#NOT__L5*/
        meltfnum[4] =
            (!(/*_#IS_STRING__L4*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:7196:/ cond");
        /*cond*/
        if (/*_#NOT__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7198:/ locexp");
                        melt_warning_str(0, (melt_ptr_t)(/*_.LOC__V17*/ meltfptr[16]), ( "COMMENT without string is ignored"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7199:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7199:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7197:/ quasiblock");


                    /*_.PROGN___V24*/
                    meltfptr[23] = /*_.RETURN___V23*/ meltfptr[22];;
                    /*^compute*/
                    /*_._IF___V22*/
                    meltfptr[21] = /*_.PROGN___V24*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7196:/ clear");
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

                /*_._IF___V22*/ meltfptr[21] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7202:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_COMMENT*/ meltfrout->tabval[3])), (3), "CLASS_SOURCE_COMMENT");
            /*_.INST__V27*/
            meltfptr[26] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V27*/ meltfptr[26])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V27*/ meltfptr[26]), (1), (/*_.LOC__V17*/ meltfptr[16]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SCOMM_STR", melt_magic_discr((melt_ptr_t)(/*_.INST__V27*/ meltfptr[26])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V27*/ meltfptr[26]), (2), (/*_.COMSTR__V20*/ meltfptr[19]), "SCOMM_STR");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V27*/ meltfptr[26], "newly made instance");
        ;
        /*_.SCOM__V26*/
        meltfptr[23] = /*_.INST__V27*/ meltfptr[26];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7205:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SCOM__V26*/ meltfptr[23];;

        {
            MELT_LOCATION("warmelt-macro.melt:7205:/ locexp");
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
        /*_.LET___V25*/
        meltfptr[22] = /*_.RETURN___V28*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:7202:/ clear");
        /*clear*/ /*_.SCOM__V26*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V28*/
        meltfptr[27] = 0 ;
        /*_.LET___V15*/
        meltfptr[13] = /*_.LET___V25*/ meltfptr[22];;

        MELT_LOCATION("warmelt-macro.melt:7189:/ clear");
        /*clear*/ /*_.CONT__V16*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.COMSTR__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L4*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L5*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V25*/
        meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7185:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V15*/ meltfptr[13];;

        {
            MELT_LOCATION("warmelt-macro.melt:7185:/ locexp");
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
        /*clear*/ /*_.IFCPP___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V15*/
        meltfptr[13] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_COMMENT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER fromline 1716 */

    /** start of frame for meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER// fromline 1531
        : public Melt_CallFrameWithValues<46>
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
        MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<46> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER), clos) {};
        MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER() //the constructor fromline 1606
            : Melt_CallFrameWithValues<46> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER)) {};
        MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER


    /** end of frame for meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER fromline 1661**/

    /* end of frame for routine meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER fromline 1720 */

    /* classy proc frame meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER */
    MeltFrame_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_CHEADER", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7215:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7216:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7216:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7216:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7216;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cheader sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7216:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7216:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7217:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7217:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7217:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7217;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7217:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7217:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7218:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:7218:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7218:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7218;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7218:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7218:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7219:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7219:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7219:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7219;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7219:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7219:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7220:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7221:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V18*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V21*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.CHEAD__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:7225:/ cond");
        /*cond*/
        if (/*_.PAIR_TAIL__V23*/ meltfptr[22]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7226:/ locexp");
                        melt_warning_str(0, (melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "CHEADER should have only one argument"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7227:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7227:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7227:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7227;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cheader chead=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CHEAD__V22*/ meltfptr[21];
                            /*_.MELT_DEBUG_FUN__V25*/
                            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V24*/
                        meltfptr[23] = /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7227:/ clear");
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

                    /*_._IF___V24*/ meltfptr[23] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7227:/ locexp");
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
        /*_#IS_STRING__L8*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-macro.melt:7229:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L8*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-macro.melt:7230:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V26*/
                    meltfptr[24] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:7229:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L9*/
                    meltfnum[0] =
                        melt_is_instance_of((melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]), (melt_ptr_t)((/*!CLASS_SEXPR_MACROSTRING*/ meltfrout->tabval[4])));;
                    MELT_LOCATION("warmelt-macro.melt:7232:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L9*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:7233:/ quasiblock");


                                /*_.SBUF__V29*/
                                meltfptr[28] =
                                    (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[5])), (const char*)0);;
                                MELT_LOCATION("warmelt-macro.melt:7234:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]),
                                                                  (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                            /*_.SCONT__V30*/
                                            meltfptr[29] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.SCONT__V30*/ meltfptr[29] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7235:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]),
                                                                  (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[6])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                                            /*_.SLOC__V31*/
                                            meltfptr[30] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.SLOC__V31*/ meltfptr[30] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7237:/ cond");
                                /*cond*/
                                if (/*_.SLOC__V31*/ meltfptr[30]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*^compute*/
                                            /*_.LOC__V19*/ meltfptr[18] = /*_.SETQ___V33*/ meltfptr[32] = /*_.SLOC__V31*/ meltfptr[30];;
                                            /*_._IF___V32*/
                                            meltfptr[31] = /*_.SETQ___V33*/ meltfptr[32];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:7237:/ clear");
                                            /*clear*/ /*_.SETQ___V33*/
                                            meltfptr[32] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V32*/ meltfptr[31] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                /*citerblock FOREACH_IN_LIST*/
                                {
                                    /* start foreach_in_list meltcit1__EACHLIST */
                                    for (/*_.CURPAIR__V34*/ meltfptr[32] = melt_list_first( (melt_ptr_t)/*_.SCONT__V30*/ meltfptr[29]);
                                                            melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V34*/ meltfptr[32]) == MELTOBMAG_PAIR;
                                                            /*_.CURPAIR__V34*/ meltfptr[32] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V34*/ meltfptr[32]))
                                        {
                                            /*_.CURARG__V35*/ meltfptr[34] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V34*/ meltfptr[32]);



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:7241:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURARG__V35*/ meltfptr[34];
                                                /*_.ADD2OUT__V36*/
                                                meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.SBUF__V29*/ meltfptr[28]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                        } /* end foreach_in_list meltcit1__EACHLIST */
                                    /*_.CURPAIR__V34*/ meltfptr[32] = NULL;
                                    /*_.CURARG__V35*/
                                    meltfptr[34] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-macro.melt:7238:/ clear");
                                    /*clear*/ /*_.CURPAIR__V34*/
                                    meltfptr[32] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURARG__V35*/
                                    meltfptr[34] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.ADD2OUT__V36*/
                                    meltfptr[35] = 0 ;
                                } /*endciterblock FOREACH_IN_LIST*/
                                ;
                                /*_.STRBUF2STRING__V37*/
                                meltfptr[36] =
                                    (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[8])), melt_strbuf_str((melt_ptr_t)(/*_.SBUF__V29*/ meltfptr[28]))));;
                                MELT_LOCATION("warmelt-macro.melt:7243:/ compute");
                                /*_.CHEAD__V22*/
                                meltfptr[21] = /*_.SETQ___V38*/ meltfptr[37] = /*_.STRBUF2STRING__V37*/ meltfptr[36];;
                                /*_.LET___V28*/
                                meltfptr[27] = /*_.SETQ___V38*/ meltfptr[37];;

                                MELT_LOCATION("warmelt-macro.melt:7233:/ clear");
                                /*clear*/ /*_.SBUF__V29*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SCONT__V30*/
                                meltfptr[29] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SLOC__V31*/
                                meltfptr[30] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V32*/
                                meltfptr[31] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.STRBUF2STRING__V37*/
                                meltfptr[36] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V38*/
                                meltfptr[37] = 0 ;
                                /*_._IFELSE___V27*/
                                meltfptr[23] = /*_.LET___V28*/ meltfptr[27];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7232:/ clear");
                                /*clear*/ /*_.LET___V28*/
                                meltfptr[27] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:7246:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "CHEADER without string or macrostring"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7247:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:7247:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:7245:/ quasiblock");


                                /*_.PROGN___V40*/
                                meltfptr[29] = /*_.RETURN___V39*/ meltfptr[28];;
                                /*^compute*/
                                /*_._IFELSE___V27*/
                                meltfptr[23] = /*_.PROGN___V40*/ meltfptr[29];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7232:/ clear");
                                /*clear*/ /*_.RETURN___V39*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V40*/
                                meltfptr[29] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V26*/
                    meltfptr[24] = /*_._IFELSE___V27*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7229:/ clear");
                    /*clear*/ /*_#IS_A__L9*/
                    meltfnum[0] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V27*/
                    meltfptr[23] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7250:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_CHEADER*/ meltfrout->tabval[9])), (3), "CLASS_SOURCE_CHEADER");
            /*_.INST__V43*/
            meltfptr[36] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V43*/ meltfptr[36])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V43*/ meltfptr[36]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SC_CODESTRING", melt_magic_discr((melt_ptr_t)(/*_.INST__V43*/ meltfptr[36])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V43*/ meltfptr[36]), (2), (/*_.CHEAD__V22*/ meltfptr[21]), "SC_CODESTRING");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V43*/ meltfptr[36], "newly made instance");
        ;
        /*_.SCH__V42*/
        meltfptr[31] = /*_.INST__V43*/ meltfptr[36];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7253:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:7253:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L10*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:7253:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7253;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cheader gives sch=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SCH__V42*/ meltfptr[31];
                            /*_.MELT_DEBUG_FUN__V45*/
                            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V44*/
                        meltfptr[37] = /*_.MELT_DEBUG_FUN__V45*/ meltfptr[27];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7253:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L11*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V45*/
                        meltfptr[27] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V44*/ meltfptr[37] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7253:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L10*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V44*/
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
        MELT_LOCATION("warmelt-macro.melt:7254:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SCH__V42*/ meltfptr[31];;

        {
            MELT_LOCATION("warmelt-macro.melt:7254:/ locexp");
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
        /*_.LET___V41*/
        meltfptr[30] = /*_.RETURN___V46*/ meltfptr[28];;

        MELT_LOCATION("warmelt-macro.melt:7250:/ clear");
        /*clear*/ /*_.SCH__V42*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V46*/
        meltfptr[28] = 0 ;
        /*_.LET___V17*/
        meltfptr[15] = /*_.LET___V41*/ meltfptr[30];;

        MELT_LOCATION("warmelt-macro.melt:7220:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[5] = 0 ;
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
        /*clear*/ /*_.CHEAD__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L8*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V26*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V41*/
        meltfptr[30] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7215:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7215:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_CHEADER", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT fromline 1716 */

    /** start of frame for meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT// fromline 1531
        : public Melt_CallFrameWithValues<46>
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
        MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<46> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT), clos) {};
        MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<46> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT)) {};
        MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT


    /** end of frame for meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT fromline 1661**/

    /* end of frame for routine meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT fromline 1720 */

    /* classy proc frame meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT */
    MeltFrame_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_CIMPLEMENT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7263:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7264:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7264:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7264:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7264;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cimplement sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7264:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7264:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7265:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7265:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7265:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7265;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7265:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7265:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7266:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:7266:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7266:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7266;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7266:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7266:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7267:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7267:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7267:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7267;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7267:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7267:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7268:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7269:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V18*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V21*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.CHEAD__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:7273:/ cond");
        /*cond*/
        if (/*_.PAIR_TAIL__V23*/ meltfptr[22]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7274:/ locexp");
                        melt_warning_str(0, (melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "CIMPLEMENT should have only one argument"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7275:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7275:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7275:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7275;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cimplement chead=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CHEAD__V22*/ meltfptr[21];
                            /*_.MELT_DEBUG_FUN__V25*/
                            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V24*/
                        meltfptr[23] = /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7275:/ clear");
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

                    /*_._IF___V24*/ meltfptr[23] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7275:/ locexp");
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
        /*_#IS_STRING__L8*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-macro.melt:7277:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L8*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-macro.melt:7278:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V26*/
                    meltfptr[24] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:7277:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L9*/
                    meltfnum[0] =
                        melt_is_instance_of((melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]), (melt_ptr_t)((/*!CLASS_SEXPR_MACROSTRING*/ meltfrout->tabval[4])));;
                    MELT_LOCATION("warmelt-macro.melt:7280:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L9*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:7281:/ quasiblock");


                                /*_.SBUF__V29*/
                                meltfptr[28] =
                                    (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[5])), (const char*)0);;
                                MELT_LOCATION("warmelt-macro.melt:7282:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]),
                                                                  (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                            /*_.SCONT__V30*/
                                            meltfptr[29] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.SCONT__V30*/ meltfptr[29] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7283:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]),
                                                                  (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[6])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CHEAD__V22*/ meltfptr[21]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                                            /*_.SLOC__V31*/
                                            meltfptr[30] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.SLOC__V31*/ meltfptr[30] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7285:/ cond");
                                /*cond*/
                                if (/*_.SLOC__V31*/ meltfptr[30]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*^compute*/
                                            /*_.LOC__V19*/ meltfptr[18] = /*_.SETQ___V33*/ meltfptr[32] = /*_.SLOC__V31*/ meltfptr[30];;
                                            /*_._IF___V32*/
                                            meltfptr[31] = /*_.SETQ___V33*/ meltfptr[32];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:7285:/ clear");
                                            /*clear*/ /*_.SETQ___V33*/
                                            meltfptr[32] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V32*/ meltfptr[31] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                /*citerblock FOREACH_IN_LIST*/
                                {
                                    /* start foreach_in_list meltcit1__EACHLIST */
                                    for (/*_.CURPAIR__V34*/ meltfptr[32] = melt_list_first( (melt_ptr_t)/*_.SCONT__V30*/ meltfptr[29]);
                                                            melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V34*/ meltfptr[32]) == MELTOBMAG_PAIR;
                                                            /*_.CURPAIR__V34*/ meltfptr[32] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V34*/ meltfptr[32]))
                                        {
                                            /*_.CURARG__V35*/ meltfptr[34] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V34*/ meltfptr[32]);



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:7289:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURARG__V35*/ meltfptr[34];
                                                /*_.ADD2OUT__V36*/
                                                meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.SBUF__V29*/ meltfptr[28]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                        } /* end foreach_in_list meltcit1__EACHLIST */
                                    /*_.CURPAIR__V34*/ meltfptr[32] = NULL;
                                    /*_.CURARG__V35*/
                                    meltfptr[34] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-macro.melt:7286:/ clear");
                                    /*clear*/ /*_.CURPAIR__V34*/
                                    meltfptr[32] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURARG__V35*/
                                    meltfptr[34] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.ADD2OUT__V36*/
                                    meltfptr[35] = 0 ;
                                } /*endciterblock FOREACH_IN_LIST*/
                                ;
                                /*_.STRBUF2STRING__V37*/
                                meltfptr[36] =
                                    (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[8])), melt_strbuf_str((melt_ptr_t)(/*_.SBUF__V29*/ meltfptr[28]))));;
                                MELT_LOCATION("warmelt-macro.melt:7291:/ compute");
                                /*_.CHEAD__V22*/
                                meltfptr[21] = /*_.SETQ___V38*/ meltfptr[37] = /*_.STRBUF2STRING__V37*/ meltfptr[36];;
                                /*_.LET___V28*/
                                meltfptr[27] = /*_.SETQ___V38*/ meltfptr[37];;

                                MELT_LOCATION("warmelt-macro.melt:7281:/ clear");
                                /*clear*/ /*_.SBUF__V29*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SCONT__V30*/
                                meltfptr[29] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SLOC__V31*/
                                meltfptr[30] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V32*/
                                meltfptr[31] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.STRBUF2STRING__V37*/
                                meltfptr[36] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V38*/
                                meltfptr[37] = 0 ;
                                /*_._IFELSE___V27*/
                                meltfptr[23] = /*_.LET___V28*/ meltfptr[27];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7280:/ clear");
                                /*clear*/ /*_.LET___V28*/
                                meltfptr[27] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:7294:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "CIMPLEMENT without string or macrostring"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7295:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:7295:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:7293:/ quasiblock");


                                /*_.PROGN___V40*/
                                meltfptr[29] = /*_.RETURN___V39*/ meltfptr[28];;
                                /*^compute*/
                                /*_._IFELSE___V27*/
                                meltfptr[23] = /*_.PROGN___V40*/ meltfptr[29];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7280:/ clear");
                                /*clear*/ /*_.RETURN___V39*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V40*/
                                meltfptr[29] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V26*/
                    meltfptr[24] = /*_._IFELSE___V27*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7277:/ clear");
                    /*clear*/ /*_#IS_A__L9*/
                    meltfnum[0] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V27*/
                    meltfptr[23] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7298:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_CIMPLEMENT*/ meltfrout->tabval[9])), (3), "CLASS_SOURCE_CIMPLEMENT");
            /*_.INST__V43*/
            meltfptr[36] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V43*/ meltfptr[36])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V43*/ meltfptr[36]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SC_CODESTRING", melt_magic_discr((melt_ptr_t)(/*_.INST__V43*/ meltfptr[36])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V43*/ meltfptr[36]), (2), (/*_.CHEAD__V22*/ meltfptr[21]), "SC_CODESTRING");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V43*/ meltfptr[36], "newly made instance");
        ;
        /*_.SCH__V42*/
        meltfptr[31] = /*_.INST__V43*/ meltfptr[36];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7301:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:7301:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L10*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:7301:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7301;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cimplement gives sch=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SCH__V42*/ meltfptr[31];
                            /*_.MELT_DEBUG_FUN__V45*/
                            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V44*/
                        meltfptr[37] = /*_.MELT_DEBUG_FUN__V45*/ meltfptr[27];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7301:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L11*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V45*/
                        meltfptr[27] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V44*/ meltfptr[37] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7301:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L10*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V44*/
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
        MELT_LOCATION("warmelt-macro.melt:7302:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SCH__V42*/ meltfptr[31];;

        {
            MELT_LOCATION("warmelt-macro.melt:7302:/ locexp");
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
        /*_.LET___V41*/
        meltfptr[30] = /*_.RETURN___V46*/ meltfptr[28];;

        MELT_LOCATION("warmelt-macro.melt:7298:/ clear");
        /*clear*/ /*_.SCH__V42*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V46*/
        meltfptr[28] = 0 ;
        /*_.LET___V17*/
        meltfptr[15] = /*_.LET___V41*/ meltfptr[30];;

        MELT_LOCATION("warmelt-macro.melt:7268:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[5] = 0 ;
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
        /*clear*/ /*_.CHEAD__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L8*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V26*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V41*/
        meltfptr[30] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7263:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7263:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_CIMPLEMENT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG fromline 1716 */

    /** start of frame for meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG// fromline 1531
        : public Melt_CallFrameWithValues<48>
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
        MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<48> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG), clos) {};
        MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG() //the constructor fromline 1606
            : Melt_CallFrameWithValues<48> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<48> (fil,lin, sizeof(MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG)) {};
        MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<48> (fil,lin, sizeof(MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG


    /** end of frame for meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG fromline 1661**/

    /* end of frame for routine meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG fromline 1720 */

    /* classy proc frame meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG */
    MeltFrame_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7312:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7313:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7313:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7313:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7313;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_use_package_from_pkg_config sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7313:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7313:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7314:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7314:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7314:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7314;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7314:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7314:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7315:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:7315:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7315:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7315;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7315:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7315:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7316:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7316:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7316:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7316;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7316:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7316:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7317:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7318:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V19*/
            meltfptr[18] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7319:/ apply");
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
            /*_.XARGTUP__V20*/
            meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_RESTLIST_AS_TUPLE*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.CONT__V18*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.CMDBUF__V21*/
        meltfptr[20] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[5])), (const char*)0);;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7322:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7322:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7322:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7322;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_use_package_from_pkg_config xargtup=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XARGTUP__V20*/ meltfptr[19];
                            /*_.MELT_DEBUG_FUN__V23*/
                            meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V22*/
                        meltfptr[21] = /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7322:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L7*/
                        meltfnum[1] = 0 ;
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
                MELT_LOCATION("warmelt-macro.melt:7322:/ locexp");
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7323:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "pkg-config --exists";
            /*_.ADD2OUT__V24*/
            meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.CMDBUF__V21*/ meltfptr[20]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit1__EACHTUP */
            long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.XARGTUP__V20*/ meltfptr[19]);
            for (/*_#PKGIX__L8*/ meltfnum[1] = 0;
                                 (/*_#PKGIX__L8*/ meltfnum[1] >= 0) && (/*_#PKGIX__L8*/ meltfnum[1] <  meltcit1__EACHTUP_ln);
                                 /*_#PKGIX__L8*/ meltfnum[1]++)
                {
                    /*_.CURPKGNAME__V25*/ meltfptr[21] = melt_multiple_nth((melt_ptr_t)(/*_.XARGTUP__V20*/ meltfptr[19]),  /*_#PKGIX__L8*/ meltfnum[1]);




                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_STRING__L9*/
                    meltfnum[0] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CURPKGNAME__V25*/ meltfptr[21])) == MELTOBMAG_STRING);;
                    MELT_LOCATION("warmelt-macro.melt:7327:/ cond");
                    /*cond*/
                    if (/*_#IS_STRING__L9*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*_._IFELSE___V26*/ meltfptr[25] = (/*nil*/NULL);;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-macro.melt:7327:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:7329:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "invalid argument to (USE_PACKAGE_FROM_PKG_CONFIG <pkg-name>...), expecting\
 string"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7330:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:7330:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:7327:/ quasiblock");


                                /*_.PROGN___V28*/
                                meltfptr[27] = /*_.RETURN___V27*/ meltfptr[26];;
                                /*^compute*/
                                /*_._IFELSE___V26*/
                                meltfptr[25] = /*_.PROGN___V28*/ meltfptr[27];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7327:/ clear");
                                /*clear*/ /*_.RETURN___V27*/
                                meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V28*/
                                meltfptr[27] = 0 ;
                            }
                            ;
                        }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7332:/ quasiblock");


                    /*_#GOODPKGNAME__L10*/
                    meltfnum[9] = 1;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7334:/ locexp");
                        /* mexpand_use_package_from_pkg_config CHECKPKGNAME_CHK__1 */
                        {
                            const char* pkgs_CHECKPKGNAME_CHK__1
                                = melt_string_str ((melt_ptr_t) /*_.CURPKGNAME__V25*/ meltfptr[21]) ;
                            if (!pkgs_CHECKPKGNAME_CHK__1)
                                /*_#GOODPKGNAME__L10*/ meltfnum[9] = 0 ;
                            else
                                {
                                    const char* pc = NULL;
                                    for (pc = pkgs_CHECKPKGNAME_CHK__1   ;
                                            *pc && /*_#GOODPKGNAME__L10*/ meltfnum[9]   ;
                                            pc++)
                                        {
                                            if (ISALNUM(*pc) || *pc == '+' || *pc == '-' || *pc == '_' || *pc == '.' || *pc == '@')
                                                continue ;
                                            else /*_#GOODPKGNAME__L10*/ meltfnum[9] = 0L ;
                                        }
                                } ;
                        } /* end mexpand_use_package_from_pkg_config CHECKPKGNAME_CHK__1 */ ;
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7351:/ cond");
                    /*cond*/
                    if (/*_#GOODPKGNAME__L10*/ meltfnum[9]) /*then*/
                        {
                            /*^cond.then*/
                            /*_._IFELSE___V30*/ meltfptr[27] = (/*nil*/NULL);;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-macro.melt:7351:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:7352:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "invalid package name for (USE_PACKAGE_FROM_PKG_CONFIG ...)"), (melt_ptr_t)(/*_.CURPKGNAME__V25*/ meltfptr[21]));
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7353:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:7353:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:7351:/ quasiblock");


                                /*_.PROGN___V32*/
                                meltfptr[31] = /*_.RETURN___V31*/ meltfptr[30];;
                                /*^compute*/
                                /*_._IFELSE___V30*/
                                meltfptr[27] = /*_.PROGN___V32*/ meltfptr[31];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7351:/ clear");
                                /*clear*/ /*_.RETURN___V31*/
                                meltfptr[30] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V32*/
                                meltfptr[31] = 0 ;
                            }
                            ;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7354:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  " ";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURPKGNAME__V25*/ meltfptr[21];
                        /*_.ADD2OUT__V33*/
                        meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.CMDBUF__V21*/ meltfptr[20]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.LET___V29*/
                    meltfptr[26] = /*_.ADD2OUT__V33*/ meltfptr[30];;

                    MELT_LOCATION("warmelt-macro.melt:7332:/ clear");
                    /*clear*/ /*_#GOODPKGNAME__L10*/
                    meltfnum[9] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V30*/
                    meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V33*/
                    meltfptr[30] = 0 ;
                    if (/*_#PKGIX__L8*/ meltfnum[1]<0) break;
                } /* end  foreach_in_multiple meltcit1__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-macro.melt:7324:/ clear");
            /*clear*/ /*_.CURPKGNAME__V25*/
            meltfptr[21] = 0 ;
            /*^clear*/
            /*clear*/ /*_#PKGIX__L8*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_STRING__L9*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V26*/
            meltfptr[25] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LET___V29*/
            meltfptr[26] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7358:/ quasiblock");


        /*_.CMDSTR__V35*/
        meltfptr[27] =
            (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[7])), melt_strbuf_str((melt_ptr_t)(/*_.CMDBUF__V21*/ meltfptr[20]))));;
        /*^compute*/
        /*_#FAILCMD__L11*/
        meltfnum[9] = 0;;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7361:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7361:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7361:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7361;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_use_package_from_pkg_config cmdstr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CMDSTR__V35*/ meltfptr[27];
                            /*_.MELT_DEBUG_FUN__V37*/
                            meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V36*/
                        meltfptr[30] = /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7361:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L13*/
                        meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V37*/
                        meltfptr[36] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V36*/ meltfptr[30] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7361:/ locexp");
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
            /*clear*/ /*_._IF___V36*/
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

        {
            MELT_LOCATION("warmelt-macro.melt:7362:/ locexp");
            /* mexpand_use_package_from_pkg_config TESTPKGCONFIG_CHK__1 */
            {
                const char* cmd_TESTPKGCONFIG_CHK__1 =
                    melt_string_str ((melt_ptr_t) /*_.CMDSTR__V35*/ meltfptr[27]) ;
                fflush (NULL)	;
                /*_#FAILCMD__L11*/
                meltfnum[9] = (cmd_TESTPKGCONFIG_CHK__1
                               ? system (cmd_TESTPKGCONFIG_CHK__1)
                               : 1000) ;
            } /* end mexpand_use_package_from_pkg_config TESTPKGCONFIG_CHK__1 */
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7371:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7371:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7371:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7371;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_use_package_from_pkg_config failcmd=";
                            /*^apply.arg*/
                            argtab[4].meltbp_long = /*_#FAILCMD__L11*/ meltfnum[9];
                            /*_.MELT_DEBUG_FUN__V39*/
                            meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V38*/
                        meltfptr[36] = /*_.MELT_DEBUG_FUN__V39*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7371:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L15*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V39*/
                        meltfptr[30] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V38*/ meltfptr[36] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7371:/ locexp");
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
            /*clear*/ /*_._IF___V38*/
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
        MELT_LOCATION("warmelt-macro.melt:7372:/ cond");
        /*cond*/
        if (/*_#FAILCMD__L11*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7373:/ locexp");
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "unexistent package names for USE_PACKAGE_FROM_PKG_CONFIG "), (melt_ptr_t)(/*_.CMDSTR__V35*/ meltfptr[27]));
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7374:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7374:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7372:/ quasiblock");


                    /*_.PROGN___V42*/
                    meltfptr[41] = /*_.RETURN___V41*/ meltfptr[36];;
                    /*^compute*/
                    /*_._IF___V40*/
                    meltfptr[30] = /*_.PROGN___V42*/ meltfptr[41];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7372:/ clear");
                    /*clear*/ /*_.RETURN___V41*/
                    meltfptr[36] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V42*/
                    meltfptr[41] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V40*/ meltfptr[30] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.LET___V34*/
        meltfptr[31] = /*_._IF___V40*/ meltfptr[30];;

        MELT_LOCATION("warmelt-macro.melt:7358:/ clear");
        /*clear*/ /*_.CMDSTR__V35*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#FAILCMD__L11*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V40*/
        meltfptr[30] = 0 ;
        MELT_LOCATION("warmelt-macro.melt:7377:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_USE_PACKAGE_FROM_PKG_CONFIG*/ meltfrout->tabval[8])), (3), "CLASS_SOURCE_USE_PACKAGE_FROM_PKG_CONFIG");
            /*_.INST__V45*/
            meltfptr[27] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V45*/ meltfptr[27])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V45*/ meltfptr[27]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUSEPACKAGE_PKGTUPLE", melt_magic_discr((melt_ptr_t)(/*_.INST__V45*/ meltfptr[27])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V45*/ meltfptr[27]), (2), (/*_.XARGTUP__V20*/ meltfptr[19]), "SUSEPACKAGE_PKGTUPLE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V45*/ meltfptr[27], "newly made instance");
        ;
        /*_.SUP__V44*/
        meltfptr[41] = /*_.INST__V45*/ meltfptr[27];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7380:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7380:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L16*/ meltfnum[11]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L17*/ meltfnum[12] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:7380:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7380;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_use_package_from_pkg_config gives sup=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SUP__V44*/ meltfptr[41];
                            /*_.MELT_DEBUG_FUN__V47*/
                            meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V46*/
                        meltfptr[30] = /*_.MELT_DEBUG_FUN__V47*/ meltfptr[46];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7380:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L17*/
                        meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V47*/
                        meltfptr[46] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V46*/ meltfptr[30] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7380:/ locexp");
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
            /*clear*/ /*_._IF___V46*/
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7381:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SUP__V44*/ meltfptr[41];;

        {
            MELT_LOCATION("warmelt-macro.melt:7381:/ locexp");
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
        /*_.LET___V43*/
        meltfptr[36] = /*_.RETURN___V48*/ meltfptr[46];;

        MELT_LOCATION("warmelt-macro.melt:7377:/ clear");
        /*clear*/ /*_.SUP__V44*/
        meltfptr[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V48*/
        meltfptr[46] = 0 ;
        /*_.LET___V17*/
        meltfptr[15] = /*_.LET___V43*/ meltfptr[36];;

        MELT_LOCATION("warmelt-macro.melt:7317:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XARGTUP__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CMDBUF__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V24*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V34*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V43*/
        meltfptr[36] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7312:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7312:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN fromline 1716 */

    /** start of frame for meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN// fromline 1531
        : public Melt_CallFrameWithValues<21>
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
        MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<21> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN), clos) {};
        MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<21> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<21> (fil,lin, sizeof(MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN)) {};
        MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<21> (fil,lin, sizeof(MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN


    /** end of frame for meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN fromline 1661**/

    /* end of frame for routine meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN fromline 1720 */

    /* classy proc frame meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN */
    MeltFrame_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PAIRLIST_TO_RETURN", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7402:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7403:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:7403:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7403:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7403;
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/
                            meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/
                        meltfptr[7] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7403:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/
                        meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/
            meltfptr[6] = /*_._IFELSE___V8*/ meltfptr[7];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7403:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7404:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L2*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V5*/ meltfptr[4])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:7404:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7404:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check mexpander";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7404;
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/
                            meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/
                        meltfptr[7] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7404:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/
                        meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/
            meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[7];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7404:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L2*/
            meltfnum[0] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:7405:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L3*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V6*/ meltfptr[5])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7405:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7405:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7405;
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                            meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/
                        meltfptr[7] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7405:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                        meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/
            meltfptr[11] = /*_._IFELSE___V14*/ meltfptr[7];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7405:/ clear");
            /*clear*/ /*_#IS_OBJECT__L3*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/
            meltfptr[7] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[11] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7406:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7410:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V18*/
        meltfptr[17] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_4*/ meltfrout->tabval[4])), (3));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V18*/ meltfptr[17])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V18*/ meltfptr[17])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V18*/ meltfptr[17])->tabval[0] = (melt_ptr_t)(/*_.MEXPANDER__V5*/ meltfptr[4]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V18*/ meltfptr[17])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V18*/ meltfptr[17])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V18*/ meltfptr[17])->tabval[1] = (melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V18*/ meltfptr[17])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V18*/ meltfptr[17])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V18*/ meltfptr[17])->tabval[2] = (melt_ptr_t)(/*_.MODCTX__V6*/ meltfptr[5]);
        ;
        /*_.LAMBDA___V17*/
        meltfptr[7] = /*_.LAMBDA___V18*/ meltfptr[17];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7407:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[3]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V17*/ meltfptr[7];
            /*_.BODYTUP__V19*/
            meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_MULTIPLE*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.PAIR__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7411:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_RETURN*/ meltfrout->tabval[5])), (3), "CLASS_SOURCE_RETURN");
            /*_.INST__V21*/
            meltfptr[20] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V21*/ meltfptr[20])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V21*/ meltfptr[20]), (1), (/*_.LOC__V3*/ meltfptr[2]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V21*/ meltfptr[20])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V21*/ meltfptr[20]), (2), (/*_.BODYTUP__V19*/ meltfptr[18]), "SARGOP_ARGS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V21*/ meltfptr[20], "newly made instance");
        ;
        /*_.INST___V20*/
        meltfptr[19] = /*_.INST__V21*/ meltfptr[20];;
        /*^compute*/
        /*_.LET___V16*/
        meltfptr[14] = /*_.INST___V20*/ meltfptr[19];;

        MELT_LOCATION("warmelt-macro.melt:7406:/ clear");
        /*clear*/ /*_.LAMBDA___V17*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYTUP__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V20*/
        meltfptr[19] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7402:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:7402:/ locexp");
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
    melt_trace_end("PAIRLIST_TO_RETURN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_151_WARMELTmiMACRO_LAMBDA_cl36(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_151_WARMELTmiMACRO_LAMBDA_cl36 fromline 1716 */

    /** start of frame for meltrout_151_WARMELTmiMACRO_LAMBDA_cl36 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36// fromline 1531
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
        MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36), clos) {};
        MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36)) {};
        MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36


    /** end of frame for meltrout_151_WARMELTmiMACRO_LAMBDA_cl36 fromline 1661**/

    /* end of frame for routine meltrout_151_WARMELTmiMACRO_LAMBDA_cl36 fromline 1720 */

    /* classy proc frame meltrout_151_WARMELTmiMACRO_LAMBDA_cl36 */
    MeltFrame_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_151_WARMELTmiMACRO_LAMBDA_cl36 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl36", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7410:/ getarg");
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
            MELT_LOCATION("warmelt-macro.melt:7410:/ locexp");
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
    melt_trace_end("LAMBDA_cl36", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_151_WARMELTmiMACRO_LAMBDA_cl36_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_151_WARMELTmiMACRO_LAMBDA_cl36*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN fromline 1716 */

    /** start of frame for meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN// fromline 1531
        : public Melt_CallFrameWithValues<19>
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
        MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN), clos) {};
        MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN)) {};
        MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN


    /** end of frame for meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN fromline 1661**/

    /* end of frame for routine meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN fromline 1720 */

    /* classy proc frame meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN */
    MeltFrame_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_PROGN", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7418:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7419:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:7419:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7419:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7419;
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7419:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7419:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7420:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.SLOC__V10*/
            meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7422:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.SEXP_CONTENTS__V11*/
            meltfptr[10] = slot;
        };
        ;
        /*_.LIST_FIRST__V12*/
        meltfptr[11] =
            (melt_list_first((melt_ptr_t)(/*_.SEXP_CONTENTS__V11*/ meltfptr[10])));;
        /*^compute*/
        /*_.PAIRS__V13*/
        meltfptr[12] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V12*/ meltfptr[11])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_PAIR__L2*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.PAIRS__V13*/ meltfptr[12])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L3*/
        meltfnum[2] =
            (!(/*_#IS_PAIR__L2*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:7424:/ cond");
        /*cond*/
        if (/*_#NOT__L3*/ meltfnum[2]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7426:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V10*/ meltfptr[6]), ( "empty PROGN"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7427:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7427:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7425:/ quasiblock");


                    /*_.PROGN___V16*/
                    meltfptr[15] = /*_.RETURN___V15*/ meltfptr[14];;
                    /*^compute*/
                    /*_._IF___V14*/
                    meltfptr[13] = /*_.PROGN___V16*/ meltfptr[15];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7424:/ clear");
                    /*clear*/ /*_.RETURN___V15*/
                    meltfptr[14] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V16*/
                    meltfptr[15] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[13] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7428:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7429:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V10*/ meltfptr[6];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.PROGR__V18*/
            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_PROGN*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.PAIRS__V13*/ meltfptr[12]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7434:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.PROGR__V18*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7434:/ locexp");
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
        /*_.LET___V17*/
        meltfptr[14] = /*_.RETURN___V19*/ meltfptr[18];;

        MELT_LOCATION("warmelt-macro.melt:7428:/ clear");
        /*clear*/ /*_.PROGR__V18*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V19*/
        meltfptr[18] = 0 ;
        /*_.LET___V9*/
        meltfptr[7] = /*_.LET___V17*/ meltfptr[14];;

        MELT_LOCATION("warmelt-macro.melt:7420:/ clear");
        /*clear*/ /*_.SLOC__V10*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SEXP_CONTENTS__V11*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V12*/
        meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIRS__V13*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L2*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L3*/
        meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7418:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V9*/ meltfptr[7];;

        {
            MELT_LOCATION("warmelt-macro.melt:7418:/ locexp");
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
        /*clear*/ /*_.LET___V9*/
        meltfptr[7] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_PROGN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN fromline 1716 */

    /** start of frame for meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN// fromline 1531
        : public Melt_CallFrameWithValues<23>
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
        MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<23> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN), clos) {};
        MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<23> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<23> (fil,lin, sizeof(MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN)) {};
        MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<23> (fil,lin, sizeof(MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN


    /** end of frame for meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN fromline 1661**/

    /* end of frame for routine meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN fromline 1720 */

    /* classy proc frame meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN */
    MeltFrame_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_RETURN", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7443:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7444:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7444:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7444:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7444;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_return sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7444:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7444:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7445:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7445:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7445:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7445;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7445:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7445:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7446:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L4*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:7446:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7446:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check mexpander";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7446;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7446:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7446:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L4*/
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
        MELT_LOCATION("warmelt-macro.melt:7447:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7447:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7447:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7447;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7447:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7447:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7448:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7449:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.SEXP_CONTENTS__V18*/
            meltfptr[5] = slot;
        };
        ;
        /*_.LIST_FIRST__V19*/
        meltfptr[18] =
            (melt_list_first((melt_ptr_t)(/*_.SEXP_CONTENTS__V18*/ meltfptr[5])));;
        /*^compute*/
        /*_.PAIR_TAIL__V20*/
        meltfptr[19] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V19*/ meltfptr[18])));;
        MELT_LOCATION("warmelt-macro.melt:7450:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOCA_LOCATION__V21*/
            meltfptr[20] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7449:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOCA_LOCATION__V21*/ meltfptr[20];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.RETR__V22*/
            meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_RETURN*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.PAIR_TAIL__V20*/ meltfptr[19]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7455:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RETR__V22*/ meltfptr[21];;

        {
            MELT_LOCATION("warmelt-macro.melt:7455:/ locexp");
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
        /*_.LET___V17*/
        meltfptr[15] = /*_.RETURN___V23*/ meltfptr[22];;

        MELT_LOCATION("warmelt-macro.melt:7448:/ clear");
        /*clear*/ /*_.SEXP_CONTENTS__V18*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOCA_LOCATION__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETR__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V23*/
        meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7443:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7443:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_RETURN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER fromline 1716 */

    /** start of frame for meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER// fromline 1531
        : public Melt_CallFrameWithValues<40>
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
        MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<40> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER), clos) {};
        MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER() //the constructor fromline 1606
            : Melt_CallFrameWithValues<40> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<40> (fil,lin, sizeof(MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER)) {};
        MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<40> (fil,lin, sizeof(MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER


    /** end of frame for meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER fromline 1661**/

    /* end of frame for routine meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER fromline 1720 */

    /* classy proc frame meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER */
    MeltFrame_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_FOREVER", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7466:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7467:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7467:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7467:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7467;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_forever sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7467:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7467:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7468:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7468:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7468:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7468;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7468:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7468:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7469:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L4*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:7469:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7469:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check mexpander";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7469;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7469:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7469:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L4*/
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
        MELT_LOCATION("warmelt-macro.melt:7470:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7470:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7470:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7470;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7470:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7470:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7471:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7472:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V18*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V21*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.SLABNAM__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7475:/ apply");
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
            /*_.XLABNAM__V23*/
            meltfptr[22] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SLABNAM__V22*/ meltfptr[21]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7476:/ apply");
        /*apply*/
        {
            /*_.NEWENV__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!FRESH_ENV*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L6*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.XLABNAM__V23*/ meltfptr[22]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:7478:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7480:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "missing label in FOREVER"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7481:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7481:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7479:/ quasiblock");


                    /*_.PROGN___V27*/
                    meltfptr[26] = /*_.RETURN___V26*/ meltfptr[25];;
                    /*^compute*/
                    /*_._IF___V25*/
                    meltfptr[24] = /*_.PROGN___V27*/ meltfptr[26];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7478:/ clear");
                    /*clear*/ /*_.RETURN___V26*/
                    meltfptr[25] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V27*/
                    meltfptr[26] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V25*/ meltfptr[24] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V28*/
        meltfptr[25] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:7482:/ compute");
        /*_.CURPAIR__V21*/
        meltfptr[20] = /*_.SETQ___V29*/ meltfptr[26] = /*_.PAIR_TAIL__V28*/ meltfptr[25];;
        MELT_LOCATION("warmelt-macro.melt:7483:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_LABEL_BINDING*/ meltfrout->tabval[5])), (4), "CLASS_LABEL_BINDING");
            /*_.INST__V32*/
            meltfptr[31] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @BINDER", melt_magic_discr((melt_ptr_t)(/*_.INST__V32*/ meltfptr[31])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V32*/ meltfptr[31]), (0), (/*_.XLABNAM__V23*/ meltfptr[22]), "BINDER");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LABIND_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V32*/ meltfptr[31])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V32*/ meltfptr[31]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LABIND_LOC");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V32*/ meltfptr[31], "newly made instance");
        ;
        /*_.LABIND__V31*/
        meltfptr[30] = /*_.INST__V32*/ meltfptr[31];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7486:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LABIND__V31*/ meltfptr[30];
            /*_.PUT_ENV__V33*/
            meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!PUT_ENV*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.NEWENV__V24*/ meltfptr[23]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:7487:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7490:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V36*/
        meltfptr[35] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_9*/ meltfrout->tabval[9])), (3));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V36*/ meltfptr[35])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V36*/ meltfptr[35])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V36*/ meltfptr[35])->tabval[0] = (melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V36*/ meltfptr[35])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V36*/ meltfptr[35])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V36*/ meltfptr[35])->tabval[1] = (melt_ptr_t)(/*_.NEWENV__V24*/ meltfptr[23]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V36*/ meltfptr[35])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V36*/ meltfptr[35])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V36*/ meltfptr[35])->tabval[2] = (melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4]);
        ;
        /*_.LAMBDA___V35*/
        meltfptr[34] = /*_.LAMBDA___V36*/ meltfptr[35];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7487:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[8]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V35*/ meltfptr[34];
            /*_.BODYTUP__V37*/
            meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_MULTIPLE*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7492:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_FOREVER*/ meltfrout->tabval[10])), (4), "CLASS_SOURCE_FOREVER");
            /*_.INST__V39*/
            meltfptr[38] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V39*/ meltfptr[38])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V39*/ meltfptr[38]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SLABEL_BIND", melt_magic_discr((melt_ptr_t)(/*_.INST__V39*/ meltfptr[38])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V39*/ meltfptr[38]), (2), (/*_.LABIND__V31*/ meltfptr[30]), "SLABEL_BIND");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SFRV_BODY", melt_magic_discr((melt_ptr_t)(/*_.INST__V39*/ meltfptr[38])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V39*/ meltfptr[38]), (3), (/*_.BODYTUP__V37*/ meltfptr[36]), "SFRV_BODY");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V39*/ meltfptr[38], "newly made instance");
        ;
        /*_.FORR__V38*/
        meltfptr[37] = /*_.INST__V39*/ meltfptr[38];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7497:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.FORR__V38*/ meltfptr[37];;

        {
            MELT_LOCATION("warmelt-macro.melt:7497:/ locexp");
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
        meltfptr[33] = /*_.RETURN___V40*/ meltfptr[39];;

        MELT_LOCATION("warmelt-macro.melt:7487:/ clear");
        /*clear*/ /*_.LAMBDA___V35*/
        meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYTUP__V37*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FORR__V38*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V40*/
        meltfptr[39] = 0 ;
        /*_.LET___V30*/
        meltfptr[29] = /*_.LET___V34*/ meltfptr[33];;

        MELT_LOCATION("warmelt-macro.melt:7483:/ clear");
        /*clear*/ /*_.LABIND__V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PUT_ENV__V33*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V34*/
        meltfptr[33] = 0 ;
        /*_.LET___V17*/
        meltfptr[15] = /*_.LET___V30*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:7471:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[5] = 0 ;
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
        /*clear*/ /*_.SLABNAM__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XLABNAM__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NEWENV__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V28*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V29*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V30*/
        meltfptr[29] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7466:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7466:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_FOREVER", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_155_WARMELTmiMACRO_LAMBDA_cl37(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_155_WARMELTmiMACRO_LAMBDA_cl37 fromline 1716 */

    /** start of frame for meltrout_155_WARMELTmiMACRO_LAMBDA_cl37 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37// fromline 1531
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
        MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37), clos) {};
        MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37)) {};
        MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37


    /** end of frame for meltrout_155_WARMELTmiMACRO_LAMBDA_cl37 fromline 1661**/

    /* end of frame for routine meltrout_155_WARMELTmiMACRO_LAMBDA_cl37 fromline 1720 */

    /* classy proc frame meltrout_155_WARMELTmiMACRO_LAMBDA_cl37 */
    MeltFrame_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_155_WARMELTmiMACRO_LAMBDA_cl37 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl37", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7490:/ getarg");
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*~NEWENV*/ meltfclos->tabval[1]);
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
            MELT_LOCATION("warmelt-macro.melt:7490:/ locexp");
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
    melt_trace_end("LAMBDA_cl37", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_155_WARMELTmiMACRO_LAMBDA_cl37_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_155_WARMELTmiMACRO_LAMBDA_cl37*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT fromline 1716 */

    /** start of frame for meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT// fromline 1531
        : public Melt_CallFrameWithValues<42>
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
        MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT), clos) {};
        MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT)) {};
        MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT


    /** end of frame for meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT fromline 1661**/

    /* end of frame for routine meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT fromline 1720 */

    /* classy proc frame meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT */
    MeltFrame_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_EXIT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7509:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7510:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7510:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7510:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7510;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_exit sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7510:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7510:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7511:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7511:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7511:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7511;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7511:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7511:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7512:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L4*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7512:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7512:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7512;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7512:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7512:/ clear");
            /*clear*/ /*_#IS_OBJECT__L4*/
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
        MELT_LOCATION("warmelt-macro.melt:7513:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:7513:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7513:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check mexpander";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7513;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7513:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7513:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7514:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7515:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V18*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V21*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.SLABNAM__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7518:/ apply");
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
            /*_.XLABNAM__V23*/
            meltfptr[22] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SLABNAM__V22*/ meltfptr[21]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7519:/ apply");
        /*apply*/
        {
            /*_.NEWENV__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!FRESH_ENV*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L6*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.XLABNAM__V23*/ meltfptr[22]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:7521:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7523:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "missing label in EXIT"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7524:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7524:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7522:/ quasiblock");


                    /*_.PROGN___V27*/
                    meltfptr[26] = /*_.RETURN___V26*/ meltfptr[25];;
                    /*^compute*/
                    /*_._IF___V25*/
                    meltfptr[24] = /*_.PROGN___V27*/ meltfptr[26];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7521:/ clear");
                    /*clear*/ /*_.RETURN___V26*/
                    meltfptr[25] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V27*/
                    meltfptr[26] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V25*/ meltfptr[24] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V28*/
        meltfptr[25] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:7525:/ compute");
        /*_.CURPAIR__V21*/
        meltfptr[20] = /*_.SETQ___V29*/ meltfptr[26] = /*_.PAIR_TAIL__V28*/ meltfptr[25];;
        MELT_LOCATION("warmelt-macro.melt:7526:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.XLABNAM__V23*/ meltfptr[22];
            /*_.LABIND__V31*/
            meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L7*/
        meltfnum[1] =
            !melt_is_instance_of((melt_ptr_t)(/*_.LABIND__V31*/ meltfptr[30]), (melt_ptr_t)((/*!CLASS_LABEL_BINDING*/ meltfrout->tabval[6])));;
        MELT_LOCATION("warmelt-macro.melt:7527:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L7*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:7530:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.XLABNAM__V23*/ meltfptr[22]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V33*/
                        meltfptr[32] = slot;
                    };
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7529:/ locexp");
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "bad label in EXIT"), (melt_ptr_t)(/*_.NAMED_NAME__V33*/ meltfptr[32]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7531:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7531:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7528:/ quasiblock");


                    /*_.PROGN___V35*/
                    meltfptr[34] = /*_.RETURN___V34*/ meltfptr[33];;
                    /*^compute*/
                    /*_._IF___V32*/
                    meltfptr[31] = /*_.PROGN___V35*/ meltfptr[34];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7527:/ clear");
                    /*clear*/ /*_.NAMED_NAME__V33*/
                    meltfptr[32] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V34*/
                    meltfptr[33] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V35*/
                    meltfptr[34] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V32*/ meltfptr[31] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7532:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7535:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V38*/
        meltfptr[34] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_9*/ meltfrout->tabval[9])), (3));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V38*/ meltfptr[34])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V38*/ meltfptr[34])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V38*/ meltfptr[34])->tabval[0] = (melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V38*/ meltfptr[34])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V38*/ meltfptr[34])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V38*/ meltfptr[34])->tabval[1] = (melt_ptr_t)(/*_.NEWENV__V24*/ meltfptr[23]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V38*/ meltfptr[34])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V38*/ meltfptr[34])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V38*/ meltfptr[34])->tabval[2] = (melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4]);
        ;
        /*_.LAMBDA___V37*/
        meltfptr[33] = /*_.LAMBDA___V38*/ meltfptr[34];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7532:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[8]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V37*/ meltfptr[33];
            /*_.BODYTUP__V39*/
            meltfptr[38] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_MULTIPLE*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7537:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_EXIT*/ meltfrout->tabval[10])), (4), "CLASS_SOURCE_EXIT");
            /*_.INST__V41*/
            meltfptr[40] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V41*/ meltfptr[40])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V41*/ meltfptr[40]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SLABEL_BIND", melt_magic_discr((melt_ptr_t)(/*_.INST__V41*/ meltfptr[40])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V41*/ meltfptr[40]), (2), (/*_.LABIND__V31*/ meltfptr[30]), "SLABEL_BIND");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXI_BODY", melt_magic_discr((melt_ptr_t)(/*_.INST__V41*/ meltfptr[40])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V41*/ meltfptr[40]), (3), (/*_.BODYTUP__V39*/ meltfptr[38]), "SEXI_BODY");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V41*/ meltfptr[40], "newly made instance");
        ;
        /*_.EXR__V40*/
        meltfptr[39] = /*_.INST__V41*/ meltfptr[40];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7542:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.EXR__V40*/ meltfptr[39];;

        {
            MELT_LOCATION("warmelt-macro.melt:7542:/ locexp");
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
        meltfptr[32] = /*_.RETURN___V42*/ meltfptr[41];;

        MELT_LOCATION("warmelt-macro.melt:7532:/ clear");
        /*clear*/ /*_.LAMBDA___V37*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYTUP__V39*/
        meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EXR__V40*/
        meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V42*/
        meltfptr[41] = 0 ;
        /*_.LET___V30*/
        meltfptr[29] = /*_.LET___V36*/ meltfptr[32];;

        MELT_LOCATION("warmelt-macro.melt:7526:/ clear");
        /*clear*/ /*_.LABIND__V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L7*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V32*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V36*/
        meltfptr[32] = 0 ;
        /*_.LET___V17*/
        meltfptr[15] = /*_.LET___V30*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:7514:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[5] = 0 ;
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
        /*clear*/ /*_.SLABNAM__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XLABNAM__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NEWENV__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V28*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V29*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V30*/
        meltfptr[29] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7509:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7509:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_EXIT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_157_WARMELTmiMACRO_LAMBDA_cl38(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_157_WARMELTmiMACRO_LAMBDA_cl38 fromline 1716 */

    /** start of frame for meltrout_157_WARMELTmiMACRO_LAMBDA_cl38 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38// fromline 1531
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
        MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38), clos) {};
        MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38)) {};
        MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38


    /** end of frame for meltrout_157_WARMELTmiMACRO_LAMBDA_cl38 fromline 1661**/

    /* end of frame for routine meltrout_157_WARMELTmiMACRO_LAMBDA_cl38 fromline 1720 */

    /* classy proc frame meltrout_157_WARMELTmiMACRO_LAMBDA_cl38 */
    MeltFrame_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_157_WARMELTmiMACRO_LAMBDA_cl38 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl38", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7535:/ getarg");
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*~NEWENV*/ meltfclos->tabval[1]);
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
            MELT_LOCATION("warmelt-macro.melt:7535:/ locexp");
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
    melt_trace_end("LAMBDA_cl38", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_157_WARMELTmiMACRO_LAMBDA_cl38_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_157_WARMELTmiMACRO_LAMBDA_cl38*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN fromline 1716 */

    /** start of frame for meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN// fromline 1531
        : public Melt_CallFrameWithValues<43>
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
        MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<43> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN), clos) {};
        MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<43> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<43> (fil,lin, sizeof(MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN)) {};
        MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<43> (fil,lin, sizeof(MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN


    /** end of frame for meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN fromline 1661**/

    /* end of frame for routine meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN fromline 1720 */

    /* classy proc frame meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN */
    MeltFrame_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_AGAIN", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7552:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7553:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7553:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7553:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7553;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_again sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7553:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7553:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7554:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7554:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7554:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7554;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7554:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7554:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7555:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L4*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:7555:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7555:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check mexpander";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7555;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7555:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7555:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L4*/
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
        MELT_LOCATION("warmelt-macro.melt:7556:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7556:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7556:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7556;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7556:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7556:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7557:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7558:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V18*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V21*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.SLABNAM__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7561:/ apply");
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
            /*_.XLABNAM__V23*/
            meltfptr[22] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SLABNAM__V22*/ meltfptr[21]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.PAIR_TAIL__V24*/
        meltfptr[23] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:7563:/ compute");
        /*_.CURPAIR__V21*/
        meltfptr[20] = /*_.SETQ___V25*/ meltfptr[24] = /*_.PAIR_TAIL__V24*/ meltfptr[23];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L6*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.XLABNAM__V23*/ meltfptr[22]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-macro.melt:7564:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7566:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "bad label in (AGAIN <label>)"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7567:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7567:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7565:/ quasiblock");


                    /*_.PROGN___V28*/
                    meltfptr[27] = /*_.RETURN___V27*/ meltfptr[26];;
                    /*^compute*/
                    /*_._IF___V26*/
                    meltfptr[25] = /*_.PROGN___V28*/ meltfptr[27];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7564:/ clear");
                    /*clear*/ /*_.RETURN___V27*/
                    meltfptr[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V28*/
                    meltfptr[27] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V26*/ meltfptr[25] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7568:/ cond");
        /*cond*/
        if (/*_.CURPAIR__V21*/ meltfptr[20]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7570:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "extra operands to (AGAIN <label>)"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7571:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7571:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7569:/ quasiblock");


                    /*_.PROGN___V31*/
                    meltfptr[30] = /*_.RETURN___V30*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IF___V29*/
                    meltfptr[26] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7568:/ clear");
                    /*clear*/ /*_.RETURN___V30*/
                    meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V31*/
                    meltfptr[30] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V29*/ meltfptr[26] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7572:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.XLABNAM__V23*/ meltfptr[22];
            /*_.LABIND__V33*/
            meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L7*/
        meltfnum[1] =
            !melt_is_instance_of((melt_ptr_t)(/*_.LABIND__V33*/ meltfptr[30]), (melt_ptr_t)((/*!CLASS_LABEL_BINDING*/ meltfrout->tabval[5])));;
        MELT_LOCATION("warmelt-macro.melt:7573:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L7*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:7576:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.XLABNAM__V23*/ meltfptr[22]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V35*/
                        meltfptr[34] = slot;
                    };
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7575:/ locexp");
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "bad label in AGAIN"), (melt_ptr_t)(/*_.NAMED_NAME__V35*/ meltfptr[34]));
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7577:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7577:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7574:/ quasiblock");


                    /*_.PROGN___V37*/
                    meltfptr[36] = /*_.RETURN___V36*/ meltfptr[35];;
                    /*^compute*/
                    /*_._IF___V34*/
                    meltfptr[33] = /*_.PROGN___V37*/ meltfptr[36];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7573:/ clear");
                    /*clear*/ /*_.NAMED_NAME__V35*/
                    meltfptr[34] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V36*/
                    meltfptr[35] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V37*/
                    meltfptr[36] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V34*/ meltfptr[33] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7578:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_AGAIN*/ meltfrout->tabval[6])), (3), "CLASS_SOURCE_AGAIN");
            /*_.INST__V40*/
            meltfptr[36] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V40*/ meltfptr[36])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V40*/ meltfptr[36]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SLABEL_BIND", melt_magic_discr((melt_ptr_t)(/*_.INST__V40*/ meltfptr[36])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V40*/ meltfptr[36]), (2), (/*_.LABIND__V33*/ meltfptr[30]), "SLABEL_BIND");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V40*/ meltfptr[36], "newly made instance");
        ;
        /*_.MAGAIN__V39*/
        meltfptr[35] = /*_.INST__V40*/ meltfptr[36];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7583:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7583:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7583:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7583;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_again result";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MAGAIN__V39*/ meltfptr[35];
                            /*_.MELT_DEBUG_FUN__V42*/
                            meltfptr[41] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V41*/
                        meltfptr[40] = /*_.MELT_DEBUG_FUN__V42*/ meltfptr[41];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7583:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/
                        meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V42*/
                        meltfptr[41] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V41*/ meltfptr[40] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7583:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7584:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MAGAIN__V39*/ meltfptr[35];;

        {
            MELT_LOCATION("warmelt-macro.melt:7584:/ locexp");
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
        /*_.LET___V38*/
        meltfptr[34] = /*_.RETURN___V43*/ meltfptr[41];;

        MELT_LOCATION("warmelt-macro.melt:7578:/ clear");
        /*clear*/ /*_.MAGAIN__V39*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V43*/
        meltfptr[41] = 0 ;
        /*_.LET___V32*/
        meltfptr[27] = /*_.LET___V38*/ meltfptr[34];;

        MELT_LOCATION("warmelt-macro.melt:7572:/ clear");
        /*clear*/ /*_.LABIND__V33*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L7*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V34*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V38*/
        meltfptr[34] = 0 ;
        /*_.LET___V17*/
        meltfptr[15] = /*_.LET___V32*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:7557:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[5] = 0 ;
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
        /*clear*/ /*_.SLABNAM__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XLABNAM__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V29*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V32*/
        meltfptr[27] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7552:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7552:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_AGAIN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING fromline 1716 */

    /** start of frame for meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING// fromline 1531
        : public Melt_CallFrameWithValues<38>
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
        MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<38> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING), clos) {};
        MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING() //the constructor fromline 1606
            : Melt_CallFrameWithValues<38> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING)) {};
        MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING


    /** end of frame for meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING fromline 1661**/

    /* end of frame for routine meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING fromline 1720 */

    /* classy proc frame meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING */
    MeltFrame_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_COMPILE_WARNING", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7596:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7597:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7597:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7597:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7597;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_compile_warning sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7597:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7597:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7598:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7598:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7598:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7598;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7598:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7598:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7599:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L4*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7599:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7599:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7599;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7599:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7599:/ clear");
            /*clear*/ /*_#IS_OBJECT__L4*/
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
        MELT_LOCATION("warmelt-macro.melt:7600:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V15*/ meltfptr[5] = (/*!MACROEXPAND_1*/ meltfrout->tabval[3]);;
                    /*_._IF___V14*/
                    meltfptr[12] = /*_.SETQ___V15*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7600:/ clear");
                    /*clear*/ /*_.SETQ___V15*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7601:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7602:/ getslot");
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
        /*_.SMSG__V21*/
        meltfptr[20] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7605:/ apply");
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
            /*_.XMSG__V22*/
            meltfptr[21] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SMSG__V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L6*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.XMSG__V22*/ meltfptr[21])) == MELTOBMAG_STRING);;
        /*^compute*/
        /*_#NOT__L7*/
        meltfnum[6] =
            (!(/*_#IS_STRING__L6*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:7607:/ cond");
        /*cond*/
        if (/*_#NOT__L7*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7609:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing message string in (COMPILE_WARNING <msg> <exp>)"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7610:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7610:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7608:/ quasiblock");


                    /*_.PROGN___V25*/
                    meltfptr[24] = /*_.RETURN___V24*/ meltfptr[23];;
                    /*^compute*/
                    /*_._IF___V23*/
                    meltfptr[22] = /*_.PROGN___V25*/ meltfptr[24];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7607:/ clear");
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

                /*_._IF___V23*/ meltfptr[22] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V26*/
        meltfptr[23] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:7611:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V27*/ meltfptr[24] = /*_.PAIR_TAIL__V26*/ meltfptr[23];;
        MELT_LOCATION("warmelt-macro.melt:7612:/ quasiblock");


        /*_.SEXP__V29*/
        meltfptr[28] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7613:/ apply");
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
            /*_.XEXP__V30*/
            meltfptr[29] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SEXP__V29*/ meltfptr[28]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.PAIR_TAIL__V31*/
        meltfptr[30] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:7615:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V32*/ meltfptr[31] = /*_.PAIR_TAIL__V31*/ meltfptr[30];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NOTNULL__L8*/
        meltfnum[7] =
            ((/*_.CURPAIR__V20*/ meltfptr[19]) != NULL);;
        MELT_LOCATION("warmelt-macro.melt:7616:/ cond");
        /*cond*/
        if (/*_#NOTNULL__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7617:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "too many arguments in (COMPILE_WARNING <msg> [<exp>])"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7618:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_COMPILEWARNING*/ meltfrout->tabval[4])), (4), "CLASS_SOURCE_COMPILEWARNING");
            /*_.INST__V35*/
            meltfptr[34] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V35*/ meltfptr[34])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V35*/ meltfptr[34]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SCWARN_MSG", melt_magic_discr((melt_ptr_t)(/*_.INST__V35*/ meltfptr[34])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V35*/ meltfptr[34]), (2), (/*_.XMSG__V22*/ meltfptr[21]), "SCWARN_MSG");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SCWARN_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V35*/ meltfptr[34])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V35*/ meltfptr[34]), (3), (/*_.XEXP__V30*/ meltfptr[29]), "SCWARN_EXPR");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V35*/ meltfptr[34], "newly made instance");
        ;
        /*_.RES__V34*/
        meltfptr[33] = /*_.INST__V35*/ meltfptr[34];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7624:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7624:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7624:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7624;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_compile_warning result";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V34*/ meltfptr[33];
                            /*_.MELT_DEBUG_FUN__V37*/
                            meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V36*/
                        meltfptr[35] = /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7624:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L10*/
                        meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V37*/
                        meltfptr[36] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V36*/ meltfptr[35] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7624:/ locexp");
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
            /*clear*/ /*_._IF___V36*/
            meltfptr[35] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


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
        MELT_LOCATION("warmelt-macro.melt:7625:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V34*/ meltfptr[33];;

        {
            MELT_LOCATION("warmelt-macro.melt:7625:/ locexp");
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
        /*_.LET___V33*/
        meltfptr[32] = /*_.RETURN___V38*/ meltfptr[36];;

        MELT_LOCATION("warmelt-macro.melt:7618:/ clear");
        /*clear*/ /*_.RES__V34*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V38*/
        meltfptr[36] = 0 ;
        /*_.LET___V28*/
        meltfptr[27] = /*_.LET___V33*/ meltfptr[32];;

        MELT_LOCATION("warmelt-macro.melt:7612:/ clear");
        /*clear*/ /*_.SEXP__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XEXP__V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V32*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOTNULL__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V33*/
        meltfptr[32] = 0 ;
        /*_.LET___V16*/
        meltfptr[5] = /*_.LET___V28*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:7601:/ clear");
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
        /*clear*/ /*_.SMSG__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XMSG__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L7*/
        meltfnum[6] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:7596:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[5];;

        {
            MELT_LOCATION("warmelt-macro.melt:7596:/ locexp");
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
        /*clear*/ /*_.LET___V16*/
        meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_COMPILE_WARNING", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG fromline 1716 */

    /** start of frame for meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG// fromline 1531
        : public Melt_CallFrameWithValues<82>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<82> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG), clos) {};
        MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG() //the constructor fromline 1606
            : Melt_CallFrameWithValues<82> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<82> (fil,lin, sizeof(MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG)) {};
        MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<82> (fil,lin, sizeof(MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG


    /** end of frame for meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG fromline 1661**/

    /* end of frame for routine meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG fromline 1720 */

    /* classy proc frame meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG */
    MeltFrame_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_ASSERT_MSG", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7639:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7640:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7640:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7640:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7640;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_assert_msg sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7640:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7640:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7641:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7641:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7641:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7641;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7641:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7641:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7642:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:7642:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7642:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7642;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7642:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7642:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7643:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:7643:/ clear");
                    /*clear*/ /*_.SETQ___V15*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*fromline 1341*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7644:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7644:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7644:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7644;
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/
                        meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7644:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                        meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/
            meltfptr[5] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7644:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7645:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7646:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V20*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7647:/ getslot");
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
        /*_.PAIR_HEAD__V24*/
        meltfptr[23] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7649:/ apply");
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
            /*_.XMSG__V25*/
            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.PAIR_HEAD__V24*/ meltfptr[23]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.XTEST__V26*/
        meltfptr[25] = (/*nil*/NULL);;
        /*^compute*/
        /*_.XARGLIST__V27*/
        meltfptr[26] = (/*nil*/NULL);;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7652:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_6_MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[6]);
            /*_.ASSFAIL_BINDING__V28*/
            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L7*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.XMSG__V25*/ meltfptr[24])) == MELTOBMAG_STRING);;
        /*^compute*/
        /*_#NOT__L8*/
        meltfnum[7] =
            (!(/*_#IS_STRING__L7*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:7654:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7655:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "non string message in (ASSERT_MSG <msg> <test>)"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V29*/
        meltfptr[28] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        MELT_LOCATION("warmelt-macro.melt:7656:/ compute");
        /*_.CURPAIR__V23*/
        meltfptr[22] = /*_.SETQ___V30*/ meltfptr[29] = /*_.PAIR_TAIL__V29*/ meltfptr[28];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        MELT_LOCATION("warmelt-macro.melt:7657:/ cond");
        /*cond*/
        if (/*_#IS_PAIR__L9*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.PAIR_HEAD__V32*/ meltfptr[31] =
                        (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7658:/ apply");
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
                        /*_.MEXPANDER__V33*/
                        meltfptr[32] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.PAIR_HEAD__V32*/ meltfptr[31]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*^compute*/
                    /*_.XTEST__V26*/
                    meltfptr[25] = /*_.SETQ___V34*/ meltfptr[33] = /*_.MEXPANDER__V33*/ meltfptr[32];;
                    /*_.PAIR_TAIL__V35*/
                    meltfptr[34] =
                        (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7659:/ apply");
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
                        /*_.EXPAND_PAIRLIST_AS_LIST__V36*/
                        meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_PAIRLIST_AS_LIST*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.PAIR_TAIL__V35*/ meltfptr[34]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*^compute*/
                    /*_.XARGLIST__V27*/
                    meltfptr[26] = /*_.SETQ___V37*/ meltfptr[36] = /*_.EXPAND_PAIRLIST_AS_LIST__V36*/ meltfptr[35];;
                    MELT_LOCATION("warmelt-macro.melt:7657:/ quasiblock");


                    /*_.PROGN___V38*/
                    meltfptr[37] = /*_.SETQ___V37*/ meltfptr[36];;
                    /*^compute*/
                    /*_._IFELSE___V31*/
                    meltfptr[30] = /*_.PROGN___V38*/ meltfptr[37];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7657:/ clear");
                    /*clear*/ /*_.PAIR_HEAD__V32*/
                    meltfptr[31] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MEXPANDER__V33*/
                    meltfptr[32] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V34*/
                    meltfptr[33] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PAIR_TAIL__V35*/
                    meltfptr[34] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.EXPAND_PAIRLIST_AS_LIST__V36*/
                    meltfptr[35] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V37*/
                    meltfptr[36] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V38*/
                    meltfptr[37] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7662:/ locexp");
                        melt_warning_str(0, (melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "empty ASSERT_MSG"), (melt_ptr_t)(/*_.XMSG__V25*/ meltfptr[24]));
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7661:/ quasiblock");


                    /*epilog*/
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7664:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L10*/
        meltfnum[9] =
            ((/*_.ASSFAIL_BINDING__V28*/ meltfptr[27]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:7667:/ cond");
        /*cond*/
        if (/*_#NULL__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7668:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "unbound MELT_ASSERT_FAILURE in (ASSERT_MSG <msg> <test>)"), (melt_ptr_t)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:7669:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
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
                        MELT_LOCATION("warmelt-macro.melt:7669:/ cond");
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
                                    /*^compute*/
                                    /*_.DISCRIM__V42*/
                                    meltfptr[34] =
                                        ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]))));;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-macro.melt:7671:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[1];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_6_MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[6]);
                                        /*_.FIND_ENV_DEBUG__V43*/
                                        meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV_DEBUG*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-macro.melt:7669:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[11];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7669;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_assert_msg  without assert_failed env=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  " env\'s class";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.DISCRIM__V42*/ meltfptr[34];
                                        /*^apply.arg*/
                                        argtab[7].meltbp_cstring =  "find_env giving:";
                                        /*^apply.arg*/
                                        argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.FIND_ENV_DEBUG__V43*/ meltfptr[35];
                                        /*^apply.arg*/
                                        argtab[9].meltbp_cstring =  "\'melt_assert_failure_fun=";
                                        /*^apply.arg*/
                                        argtab[10].meltbp_aptr = (melt_ptr_t*) &(/*!konst_6_MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[6]);
                                        /*_.MELT_DEBUG_FUN__V44*/
                                        meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V41*/
                                    meltfptr[33] = /*_.MELT_DEBUG_FUN__V44*/ meltfptr[36];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:7669:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                                    meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.DISCRIM__V42*/
                                    meltfptr[34] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.FIND_ENV_DEBUG__V43*/
                                    meltfptr[35] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V44*/
                                    meltfptr[36] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V41*/ meltfptr[33] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:7669:/ locexp");
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
                        /*clear*/ /*_._IF___V41*/
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

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:7673:/ cppif.then");
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
                                /*_._IFELSE___V46*/ meltfptr[34] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-macro.melt:7673:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "assfail_binding nul! @@";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7673;
                                        /*_.MELT_ASSERT_FAILURE_FUN__V47*/
                                        meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V46*/
                                    meltfptr[34] = /*_.MELT_ASSERT_FAILURE_FUN__V47*/ meltfptr[35];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:7673:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V47*/
                                    meltfptr[35] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V45*/
                        meltfptr[37] = /*_._IFELSE___V46*/ meltfptr[34];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7673:/ clear");
                        /*clear*/ /*_._IFELSE___V46*/
                        meltfptr[34] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V45*/ meltfptr[37] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7674:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7674:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7667:/ quasiblock");


                    /*_.PROGN___V49*/
                    meltfptr[33] = /*_.RETURN___V48*/ meltfptr[36];;
                    /*^compute*/
                    /*_.ASSFAILFUN__V40*/
                    meltfptr[32] = /*_.PROGN___V49*/ meltfptr[33];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7667:/ clear");
                    /*clear*/ /*_.IFCPP___V45*/
                    meltfptr[37] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V48*/
                    meltfptr[36] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V49*/
                    meltfptr[33] = 0 ;
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
                    /*_#IS_A__L13*/
                    meltfnum[11] =
                        melt_is_instance_of((melt_ptr_t)(/*_.ASSFAIL_BINDING__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_FUNCTION_BINDING*/ meltfrout->tabval[11])));;
                    MELT_LOCATION("warmelt-macro.melt:7675:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L13*/ meltfnum[11]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:7677:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.ASSFAIL_BINDING__V28*/ meltfptr[27]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "FIXBIND_DATA");
                                    /*_.FIXBIND_DATA__V51*/
                                    meltfptr[34] = slot;
                                };
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7676:/ cond");
                                /*cond*/
                                if (/*_.FIXBIND_DATA__V51*/ meltfptr[34]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*_.OR___V52*/ meltfptr[37] = /*_.FIXBIND_DATA__V51*/ meltfptr[34];;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-macro.melt:7676:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-macro.melt:7678:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.ASSFAIL_BINDING__V28*/ meltfptr[27]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 2, "FUBIND_DEFUN");
                                                /*_.FUBIND_DEFUN__V53*/
                                                meltfptr[36] = slot;
                                            };
                                            ;
                                            /*_.OR___V52*/
                                            meltfptr[37] = /*_.FUBIND_DEFUN__V53*/ meltfptr[36];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:7676:/ clear");
                                            /*clear*/ /*_.FUBIND_DEFUN__V53*/
                                            meltfptr[36] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V50*/
                                meltfptr[35] = /*_.OR___V52*/ meltfptr[37];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7675:/ clear");
                                /*clear*/ /*_.FIXBIND_DATA__V51*/
                                meltfptr[34] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OR___V52*/
                                meltfptr[37] = 0 ;
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
                                /*_#IS_A__L14*/
                                meltfnum[10] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.ASSFAIL_BINDING__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[12])));;
                                MELT_LOCATION("warmelt-macro.melt:7680:/ cond");
                                /*cond*/
                                if (/*_#IS_A__L14*/ meltfnum[10]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-macro.melt:7681:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.ASSFAIL_BINDING__V28*/ meltfptr[27]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                                /*_.VBIND_VALUE__V54*/
                                                meltfptr[33] = slot;
                                            };
                                            ;
                                            /*_#IS_CLOSURE__L16*/
                                            meltfnum[15] =
                                                (melt_magic_discr((melt_ptr_t)(/*_.VBIND_VALUE__V54*/ meltfptr[33])) == MELTOBMAG_CLOSURE);;
                                            /*^compute*/
                                            /*_#_IF___L15*/
                                            meltfnum[14] = /*_#IS_CLOSURE__L16*/ meltfnum[15];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:7680:/ clear");
                                            /*clear*/ /*_.VBIND_VALUE__V54*/
                                            meltfptr[33] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#IS_CLOSURE__L16*/
                                            meltfnum[15] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_#_IF___L15*/ meltfnum[14] = 0;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7680:/ cond");
                                /*cond*/
                                if (/*_#_IF___L15*/ meltfnum[14]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-macro.melt:7682:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.ASSFAIL_BINDING__V28*/ meltfptr[27]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                                /*_.VBIND_VALUE__V56*/
                                                meltfptr[34] = slot;
                                            };
                                            ;
                                            /*_._IFELSE___V55*/
                                            meltfptr[36] = /*_.VBIND_VALUE__V56*/ meltfptr[34];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:7680:/ clear");
                                            /*clear*/ /*_.VBIND_VALUE__V56*/
                                            meltfptr[34] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {


#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-macro.melt:7684:/ cppif.then");
                                            /*^block*/
                                            /*anyblock*/
                                            {


                                                {
                                                    /*^locexp*/
                                                    /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                                                    melt_dbgcounter++;
#endif
                                                    ;
                                                }
                                                ;

                                                MELT_CHECK_SIGNAL();
                                                ;
                                                /*_#MELT_NEED_DBG__L17*/
                                                meltfnum[15] =
                                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                    ;;
                                                MELT_LOCATION("warmelt-macro.melt:7684:/ cond");
                                                /*cond*/
                                                if (/*_#MELT_NEED_DBG__L17*/ meltfnum[15]) /*then*/
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
                                                            MELT_LOCATION("warmelt-macro.melt:7684:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[5];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L18*/ meltfnum[17];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 7684;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_cstring =  "mexpand_assert_msg bad assfail_binding=";
                                                                /*^apply.arg*/
                                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ASSFAIL_BINDING__V28*/ meltfptr[27];
                                                                /*_.MELT_DEBUG_FUN__V58*/
                                                                meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IF___V57*/
                                                            meltfptr[37] = /*_.MELT_DEBUG_FUN__V58*/ meltfptr[33];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-macro.melt:7684:/ clear");
                                                            /*clear*/ /*_#MELT_CALLCOUNT__L18*/
                                                            meltfnum[17] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.MELT_DEBUG_FUN__V58*/
                                                            meltfptr[33] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_._IF___V57*/ meltfptr[37] =  /*fromline 1341*/ NULL ;;
                                                    }
                                                ;

                                                {
                                                    MELT_LOCATION("warmelt-macro.melt:7684:/ locexp");
                                                    /*void*/
                                                    (void)0;
                                                }
                                                ;
                                                /*^quasiblock*/


                                                /*epilog*/

                                                /*^clear*/
                                                /*clear*/ /*_#MELT_NEED_DBG__L17*/
                                                meltfnum[15] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IF___V57*/
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
                                                MELT_LOCATION("warmelt-macro.melt:7685:/ locexp");
                                                /* error_plain */
                                                melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "MELT_ASSERT_FAILURE badly bound in  (ASSERT_MSG <msg> <test>)"), (melt_ptr_t)0);
                                            }
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:7686:/ quasiblock");


                                            /*_._RETVAL___V1*/
                                            meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                            {
                                                MELT_LOCATION("warmelt-macro.melt:7686:/ locexp");
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
                                            MELT_LOCATION("warmelt-macro.melt:7683:/ quasiblock");


                                            /*_.PROGN___V60*/
                                            meltfptr[33] = /*_.RETURN___V59*/ meltfptr[34];;
                                            /*^compute*/
                                            /*_._IFELSE___V55*/
                                            meltfptr[36] = /*_.PROGN___V60*/ meltfptr[33];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:7680:/ clear");
                                            /*clear*/ /*_.RETURN___V59*/
                                            meltfptr[34] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PROGN___V60*/
                                            meltfptr[33] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V50*/
                                meltfptr[35] = /*_._IFELSE___V55*/ meltfptr[36];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7675:/ clear");
                                /*clear*/ /*_#IS_A__L14*/
                                meltfnum[10] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#_IF___L15*/
                                meltfnum[14] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V55*/
                                meltfptr[36] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_.ASSFAILFUN__V40*/
                    meltfptr[32] = /*_._IFELSE___V50*/ meltfptr[35];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7667:/ clear");
                    /*clear*/ /*_#IS_A__L13*/
                    meltfnum[11] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V50*/
                    meltfptr[35] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_MIXINT__L19*/
        meltfnum[17] =
            (melt_magic_discr((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20])) == MELTOBMAG_MIXINT);;
        MELT_LOCATION("warmelt-macro.melt:7689:/ cond");
        /*cond*/
        if (/*_#IS_MIXINT__L19*/ meltfnum[17]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MIXINT_VAL__V62*/ meltfptr[34] =
                        (melt_val_mixint((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20])));;
                    /*^compute*/
                    /*_.FILNAM__V61*/
                    meltfptr[37] = /*_.MIXINT_VAL__V62*/ meltfptr[34];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7689:/ clear");
                    /*clear*/ /*_.MIXINT_VAL__V62*/
                    meltfptr[34] = 0 ;
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
                    /*_#IS_MIXLOC__L20*/
                    meltfnum[15] =
                        (melt_magic_discr((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20])) == MELTOBMAG_MIXLOC);;
                    MELT_LOCATION("warmelt-macro.melt:7690:/ cond");
                    /*cond*/
                    if (/*_#IS_MIXLOC__L20*/ meltfnum[15]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_.MIXLOC_VAL__V64*/ meltfptr[36] =
                                    (melt_val_mixloc((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20])));;
                                /*^compute*/
                                /*_._IFELSE___V63*/
                                meltfptr[33] = /*_.MIXLOC_VAL__V64*/ meltfptr[36];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7690:/ clear");
                                /*clear*/ /*_.MIXLOC_VAL__V64*/
                                meltfptr[36] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IFELSE___V63*/ meltfptr[33] =  /*fromline 1341*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_.FILNAM__V61*/
                    meltfptr[37] = /*_._IFELSE___V63*/ meltfptr[33];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7689:/ clear");
                    /*clear*/ /*_#IS_MIXLOC__L20*/
                    meltfnum[15] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V63*/
                    meltfptr[33] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#GET_INT__L21*/
        meltfnum[10] =
            (melt_get_int((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20])));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V65*/
        meltfptr[35] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[17])), (/*_#GET_INT__L21*/ meltfnum[10])));;
        MELT_LOCATION("warmelt-macro.melt:7697:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_1_st
            {
                struct meltpair_st rpair_0___PAIROFLIST_x1;
                struct meltpair_st rpair_1__XMSG_x1;
                struct meltpair_st rpair_2__FILNAM_x1;
                struct meltpair_st rpair_3___PAIROFLIST_x2;
                struct meltlist_st rlist_4__LIST_;
                long meltletrec_1_endgap;
            } *meltletrec_1_ptr = 0;
            meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
            /*^blockmultialloc.initfill*/
            /*inipair rpair_0___PAIROFLIST_x1*/
            /*_._PAIROFLIST__V67*/
            meltfptr[36] = (melt_ptr_t) &meltletrec_1_ptr->rpair_0___PAIROFLIST_x1;
            meltletrec_1_ptr->rpair_0___PAIROFLIST_x1.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

            /*inipair rpair_1__XMSG_x1*/
            /*_.XMSG__V68*/
            meltfptr[33] = (melt_ptr_t) &meltletrec_1_ptr->rpair_1__XMSG_x1;
            meltletrec_1_ptr->rpair_1__XMSG_x1.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

            /*inipair rpair_2__FILNAM_x1*/
            /*_.FILNAM__V69*/
            meltfptr[68] = (melt_ptr_t) &meltletrec_1_ptr->rpair_2__FILNAM_x1;
            meltletrec_1_ptr->rpair_2__FILNAM_x1.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

            /*inipair rpair_3___PAIROFLIST_x2*/
            /*_._PAIROFLIST__V70*/
            meltfptr[69] = (melt_ptr_t) &meltletrec_1_ptr->rpair_3___PAIROFLIST_x2;
            meltletrec_1_ptr->rpair_3___PAIROFLIST_x2.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

            /*inilist rlist_4__LIST_*/
            /*_.LIST___V71*/
            meltfptr[70] = (melt_ptr_t) &meltletrec_1_ptr->rlist_4__LIST_;
            meltletrec_1_ptr->rlist_4__LIST_.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_LIST))));



            /*^putpairhead*/
            /*putpairhead*/
            melt_assertmsg("putpairhead /1 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V67*/ meltfptr[36]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_._PAIROFLIST__V67*/ meltfptr[36]))->hd = (melt_ptr_t) ((/*nil*/NULL));
            ;
            /*^putpairtail*/
            /*putpairtail*/
            melt_assertmsg("putpairtail /29720ba6 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V67*/ meltfptr[36]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_._PAIROFLIST__V67*/ meltfptr[36]))->tl = (meltpair_ptr_t) (/*_.XMSG__V68*/ meltfptr[33]);
            ;
            /*^touch*/
            meltgc_touch(/*_._PAIROFLIST__V67*/ meltfptr[36]);
            ;
            /*^putpairhead*/
            /*putpairhead*/
            melt_assertmsg("putpairhead /2 checkpair", melt_magic_discr((melt_ptr_t)(/*_.XMSG__V68*/ meltfptr[33]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_.XMSG__V68*/ meltfptr[33]))->hd = (melt_ptr_t) (/*_.XMSG__V25*/ meltfptr[24]);
            ;
            /*^putpairtail*/
            /*putpairtail*/
            melt_assertmsg("putpairtail /5148c8 checkpair", melt_magic_discr((melt_ptr_t)(/*_.XMSG__V68*/ meltfptr[33]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_.XMSG__V68*/ meltfptr[33]))->tl = (meltpair_ptr_t) (/*_.FILNAM__V69*/ meltfptr[68]);
            ;
            /*^touch*/
            meltgc_touch(/*_.XMSG__V68*/ meltfptr[33]);
            ;
            /*^putpairhead*/
            /*putpairhead*/
            melt_assertmsg("putpairhead /3 checkpair", melt_magic_discr((melt_ptr_t)(/*_.FILNAM__V69*/ meltfptr[68]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_.FILNAM__V69*/ meltfptr[68]))->hd = (melt_ptr_t) (/*_.FILNAM__V61*/ meltfptr[37]);
            ;
            /*^putpairtail*/
            /*putpairtail*/
            melt_assertmsg("putpairtail /29eb5cbd checkpair", melt_magic_discr((melt_ptr_t)(/*_.FILNAM__V69*/ meltfptr[68]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_.FILNAM__V69*/ meltfptr[68]))->tl = (meltpair_ptr_t) (/*_._PAIROFLIST__V70*/ meltfptr[69]);
            ;
            /*^touch*/
            meltgc_touch(/*_.FILNAM__V69*/ meltfptr[68]);
            ;
            MELT_LOCATION("warmelt-macro.melt:7701:/ putpairhead");
            /*putpairhead*/
            melt_assertmsg("putpairhead /4 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V70*/ meltfptr[69]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_._PAIROFLIST__V70*/ meltfptr[69]))->hd = (melt_ptr_t) (/*_.MAKE_INTEGERBOX__V65*/ meltfptr[35]);
            ;
            /*^touch*/
            meltgc_touch(/*_._PAIROFLIST__V70*/ meltfptr[69]);
            ;
            MELT_LOCATION("warmelt-macro.melt:7697:/ putlist");
            /*putlist*/
            melt_assertmsg("putlist checklist", melt_magic_discr((melt_ptr_t)(/*_.LIST___V71*/ meltfptr[70]))== MELTOBMAG_LIST);
            ((meltlist_ptr_t)(/*_.LIST___V71*/ meltfptr[70]))->first = (meltpair_ptr_t) (/*_._PAIROFLIST__V67*/ meltfptr[36]);
            ((meltlist_ptr_t)(/*_.LIST___V71*/ meltfptr[70]))->last = (meltpair_ptr_t) (/*_._PAIROFLIST__V70*/ meltfptr[69]);
            ;
            /*^touch*/
            meltgc_touch(/*_.LIST___V71*/ meltfptr[70]);
            ;
            /*_.LIST___V66*/
            meltfptr[34] = /*_.LIST___V71*/ meltfptr[70];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7697:/ clear");
            /*clear*/ /*_._PAIROFLIST__V67*/
            meltfptr[36] = 0 ;
            /*^clear*/
            /*clear*/ /*_.XMSG__V68*/
            meltfptr[33] = 0 ;
            /*^clear*/
            /*clear*/ /*_.FILNAM__V69*/
            meltfptr[68] = 0 ;
            /*^clear*/
            /*clear*/ /*_._PAIROFLIST__V70*/
            meltfptr[69] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V71*/
            meltfptr[70] = 0 ;
            /*^clear*/
            /*clear*/ /*_._PAIROFLIST__V67*/
            meltfptr[36] = 0 ;
            /*^clear*/
            /*clear*/ /*_.XMSG__V68*/
            meltfptr[33] = 0 ;
            /*^clear*/
            /*clear*/ /*_.FILNAM__V69*/
            meltfptr[68] = 0 ;
            /*^clear*/
            /*clear*/ /*_._PAIROFLIST__V70*/
            meltfptr[69] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V71*/
            meltfptr[70] = 0 ;
        } /*end multiallocblock*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7696:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.XARGLIST__V27*/ meltfptr[26];
            /*_.LIST_APPEND2LIST__V72*/
            meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_APPEND2LIST*/ meltfrout->tabval[16])), (melt_ptr_t)(/*_.LIST___V66*/ meltfptr[34]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7695:/ apply");
        /*apply*/
        {
            /*_.LIST_TO_MULTIPLE__V73*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[15])), (melt_ptr_t)(/*_.LIST_APPEND2LIST__V72*/ meltfptr[36]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:7691:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_APPLY*/ meltfrout->tabval[13])), (4), "CLASS_SOURCE_APPLY");
            /*_.INST__V75*/
            meltfptr[69] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V75*/ meltfptr[69])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V75*/ meltfptr[69]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SAPP_FUN", melt_magic_discr((melt_ptr_t)(/*_.INST__V75*/ meltfptr[69])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V75*/ meltfptr[69]), (3), ((/*!konst_6_MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[6])), "SAPP_FUN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V75*/ meltfptr[69])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V75*/ meltfptr[69]), (2), (/*_.LIST_TO_MULTIPLE__V73*/ meltfptr[33]), "SARGOP_ARGS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V75*/ meltfptr[69], "newly made instance");
        ;
        /*_.AFAPPLY__V74*/
        meltfptr[68] = /*_.INST__V75*/ meltfptr[69];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7706:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IFELSE*/ meltfrout->tabval[18])), (5), "CLASS_SOURCE_IFELSE");
            /*_.INST__V77*/
            meltfptr[76] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V77*/ meltfptr[76])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V77*/ meltfptr[76]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V77*/ meltfptr[76])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V77*/ meltfptr[76]), (2), (/*_.XTEST__V26*/ meltfptr[25]), "SIF_TEST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V77*/ meltfptr[76])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V77*/ meltfptr[76]), (3), ((/*nil*/NULL)), "SIF_THEN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V77*/ meltfptr[76])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V77*/ meltfptr[76]), (4), (/*_.AFAPPLY__V74*/ meltfptr[68]), "SIF_ELSE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V77*/ meltfptr[76], "newly made instance");
        ;
        /*_.ATEST__V76*/
        meltfptr[70] = /*_.INST__V77*/ meltfptr[76];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7712:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_CPPIF*/ meltfrout->tabval[19])), (5), "CLASS_SOURCE_CPPIF");
            /*_.INST__V79*/
            meltfptr[78] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V79*/ meltfptr[78])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V79*/ meltfptr[78]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_COND", melt_magic_discr((melt_ptr_t)(/*_.INST__V79*/ meltfptr[78])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V79*/ meltfptr[78]), (2), ((/*!konst_20_MELT_HAVE_DEBUG*/ meltfrout->tabval[20])), "SIFP_COND");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V79*/ meltfptr[78])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V79*/ meltfptr[78]), (3), (/*_.ATEST__V76*/ meltfptr[70]), "SIFP_THEN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V79*/ meltfptr[78])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V79*/ meltfptr[78]), (4), ((/*nil*/NULL)), "SIFP_ELSE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V79*/ meltfptr[78], "newly made instance");
        ;
        /*_.ACPPIF__V78*/
        meltfptr[77] = /*_.INST__V79*/ meltfptr[78];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7719:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L22*/
            meltfnum[14] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:7719:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L22*/ meltfnum[14]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L23*/ meltfnum[11] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:7719:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L23*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7719;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_assert_msg result acppif";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ACPPIF__V78*/ meltfptr[77];
                            /*_.MELT_DEBUG_FUN__V81*/
                            meltfptr[80] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V80*/
                        meltfptr[79] = /*_.MELT_DEBUG_FUN__V81*/ meltfptr[80];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7719:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L23*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V81*/
                        meltfptr[80] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V80*/ meltfptr[79] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7719:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L22*/
            meltfnum[14] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V80*/
            meltfptr[79] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


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
        MELT_LOCATION("warmelt-macro.melt:7720:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.ACPPIF__V78*/ meltfptr[77];;

        {
            MELT_LOCATION("warmelt-macro.melt:7720:/ locexp");
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
        /*_.LET___V39*/
        meltfptr[31] = /*_.RETURN___V82*/ meltfptr[80];;

        MELT_LOCATION("warmelt-macro.melt:7664:/ clear");
        /*clear*/ /*_#NULL__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ASSFAILFUN__V40*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_MIXINT__L19*/
        meltfnum[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FILNAM__V61*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L21*/
        meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V65*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST___V66*/
        meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_APPEND2LIST__V72*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_TO_MULTIPLE__V73*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.AFAPPLY__V74*/
        meltfptr[68] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ATEST__V76*/
        meltfptr[70] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ACPPIF__V78*/
        meltfptr[77] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V82*/
        meltfptr[80] = 0 ;
        /*_.LET___V19*/
        meltfptr[17] = /*_.LET___V39*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:7645:/ clear");
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
        /*clear*/ /*_.PAIR_HEAD__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XMSG__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XTEST__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XARGLIST__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ASSFAIL_BINDING__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V39*/
        meltfptr[31] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7639:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V19*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-macro.melt:7639:/ locexp");
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
    melt_trace_end("MEXPAND_ASSERT_MSG", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE fromline 1716 */

    /** start of frame for meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE// fromline 1531
        : public Melt_CallFrameWithValues<24>
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
        MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<24> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE), clos) {};
        MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE() //the constructor fromline 1606
            : Melt_CallFrameWithValues<24> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE)) {};
        MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE


    /** end of frame for meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE fromline 1661**/

    /* end of frame for routine meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE fromline 1720 */

    /* classy proc frame meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE */
    MeltFrame_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_THIS_LINE", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7732:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7733:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7733:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7733:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7733;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_this_line sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7733:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7733:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7734:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7734:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7734:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7734;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7734:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7734:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7735:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:7735:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7735:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7735;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7735:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7735:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7736:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7736:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7736:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7736;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7736:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7736:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7737:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7738:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7739:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V19*/
            meltfptr[18] = slot;
        };
        ;
        /*_#DLINE__L6*/
        meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7742:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7742:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L7*/ meltfnum[1]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:7742:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7742;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_this_line dline=";
                            /*^apply.arg*/
                            argtab[4].meltbp_long = /*_#DLINE__L6*/ meltfnum[0];
                            /*_.MELT_DEBUG_FUN__V21*/
                            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V20*/
                        meltfptr[19] = /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7742:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L8*/
                        meltfnum[7] = 0 ;
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
                MELT_LOCATION("warmelt-macro.melt:7742:/ locexp");
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
        /*_#gtI__L9*/
        meltfnum[7] =
            ((/*_#DLINE__L6*/ meltfnum[0]) > (0));;
        MELT_LOCATION("warmelt-macro.melt:7743:/ cond");
        /*cond*/
        if (/*_#gtI__L9*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.BOX__V23*/ meltfptr[19] =
                        /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#DLINE__L6*/ meltfnum[0]));;
                    /*^compute*/
                    /*_._IF___V22*/
                    meltfptr[20] = /*_.BOX__V23*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7743:/ clear");
                    /*clear*/ /*_.BOX__V23*/
                    meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V22*/ meltfptr[20] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7743:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IF___V22*/ meltfptr[20];;
        MELT_LOCATION("warmelt-macro.melt:7743:/ putxtraresult");
        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
        ;
        /*^finalreturn*/
        ;
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V17*/
        meltfptr[15] = /*_.RETURN___V24*/ meltfptr[19];;

        MELT_LOCATION("warmelt-macro.melt:7737:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#DLINE__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L9*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V22*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V24*/
        meltfptr[19] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7732:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7732:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_THIS_LINE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE fromline 1716 */

    /** start of frame for meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE// fromline 1531
        : public Melt_CallFrameWithValues<26>
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
        MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<26> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE), clos) {};
        MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE() //the constructor fromline 1606
            : Melt_CallFrameWithValues<26> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<26> (fil,lin, sizeof(MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE)) {};
        MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<26> (fil,lin, sizeof(MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE


    /** end of frame for meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE fromline 1661**/

    /* end of frame for routine meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE fromline 1720 */

    /* classy proc frame meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE */
    MeltFrame_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_THIS_FILE", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7756:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7757:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7757:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7757:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7757;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_this_file sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7757:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7757:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7758:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7758:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7758:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7758;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7758:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7758:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7759:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:7759:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7759:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7759;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7759:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7759:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7760:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7760:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7760:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7760;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7760:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7760:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7761:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7762:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7763:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V19*/
            meltfptr[18] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_MIXINT__L6*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])) == MELTOBMAG_MIXINT);;
        MELT_LOCATION("warmelt-macro.melt:7765:/ cond");
        /*cond*/
        if (/*_#IS_MIXINT__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MIXINT_VAL__V21*/ meltfptr[20] =
                        (melt_val_mixint((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])));;
                    /*^compute*/
                    /*_.DFILNAM__V20*/
                    meltfptr[19] = /*_.MIXINT_VAL__V21*/ meltfptr[20];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7765:/ clear");
                    /*clear*/ /*_.MIXINT_VAL__V21*/
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
                    /*_#IS_MIXLOC__L7*/
                    meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])) == MELTOBMAG_MIXLOC);;
                    MELT_LOCATION("warmelt-macro.melt:7766:/ cond");
                    /*cond*/
                    if (/*_#IS_MIXLOC__L7*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_.MIXLOC_VAL__V23*/ meltfptr[22] =
                                    (melt_val_mixloc((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])));;
                                /*^compute*/
                                /*_._IFELSE___V22*/
                                meltfptr[20] = /*_.MIXLOC_VAL__V23*/ meltfptr[22];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7766:/ clear");
                                /*clear*/ /*_.MIXLOC_VAL__V23*/
                                meltfptr[22] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IFELSE___V22*/ meltfptr[20] =  /*fromline 1341*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_.DFILNAM__V20*/
                    meltfptr[19] = /*_._IFELSE___V22*/ meltfptr[20];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7765:/ clear");
                    /*clear*/ /*_#IS_MIXLOC__L7*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V22*/
                    meltfptr[20] = 0 ;
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7768:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:7768:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L8*/ meltfnum[1]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:7768:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7768;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_this_file dfilnam=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.DFILNAM__V20*/ meltfptr[19];
                            /*_.MELT_DEBUG_FUN__V25*/
                            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V24*/
                        meltfptr[22] = /*_.MELT_DEBUG_FUN__V25*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7768:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/
                        meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V25*/
                        meltfptr[20] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V24*/ meltfptr[22] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7768:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L8*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V24*/
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7769:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.DFILNAM__V20*/ meltfptr[19];;

        {
            MELT_LOCATION("warmelt-macro.melt:7769:/ locexp");
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
        /*_.LET___V17*/
        meltfptr[15] = /*_.RETURN___V26*/ meltfptr[20];;

        MELT_LOCATION("warmelt-macro.melt:7761:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_MIXINT__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DFILNAM__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V26*/
        meltfptr[20] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7756:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7756:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_THIS_FILE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG fromline 1716 */

    /** start of frame for meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG// fromline 1531
        : public Melt_CallFrameWithValues<95>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[30];
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
        MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<95> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG), clos) {};
        MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG() //the constructor fromline 1606
            : Melt_CallFrameWithValues<95> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<95> (fil,lin, sizeof(MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG)) {};
        MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<95> (fil,lin, sizeof(MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG


    /** end of frame for meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG fromline 1661**/

    /* end of frame for routine meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG fromline 1720 */

    /* classy proc frame meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG */
    MeltFrame_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_DEBUG", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7782:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7783:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7783:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7783:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7783;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_debug sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7783:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7783:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7784:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7784:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7784:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7784;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7784:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7784:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7785:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:7785:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7785:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7785;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7785:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7785:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7786:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7786:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7786:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7786;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7786:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7786:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7787:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7788:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7789:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V19*/
            meltfptr[18] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_MIXINT__L6*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])) == MELTOBMAG_MIXINT);;
        MELT_LOCATION("warmelt-macro.melt:7791:/ cond");
        /*cond*/
        if (/*_#IS_MIXINT__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MIXINT_VAL__V21*/ meltfptr[20] =
                        (melt_val_mixint((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])));;
                    /*^compute*/
                    /*_.DFILNAM__V20*/
                    meltfptr[19] = /*_.MIXINT_VAL__V21*/ meltfptr[20];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7791:/ clear");
                    /*clear*/ /*_.MIXINT_VAL__V21*/
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
                    /*_#IS_MIXLOC__L7*/
                    meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])) == MELTOBMAG_MIXLOC);;
                    MELT_LOCATION("warmelt-macro.melt:7792:/ cond");
                    /*cond*/
                    if (/*_#IS_MIXLOC__L7*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_.MIXLOC_VAL__V23*/ meltfptr[22] =
                                    (melt_val_mixloc((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])));;
                                /*^compute*/
                                /*_._IFELSE___V22*/
                                meltfptr[20] = /*_.MIXLOC_VAL__V23*/ meltfptr[22];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7792:/ clear");
                                /*clear*/ /*_.MIXLOC_VAL__V23*/
                                meltfptr[22] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IFELSE___V22*/ meltfptr[20] =  /*fromline 1341*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_.DFILNAM__V20*/
                    meltfptr[19] = /*_._IFELSE___V22*/ meltfptr[20];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7791:/ clear");
                    /*clear*/ /*_#IS_MIXLOC__L7*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V22*/
                    meltfptr[20] = 0 ;
                }
                ;
            }
        ;
        /*_#DLINE__L8*/
        meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7794:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_5_MELT_DEBUG_FUN*/ meltfrout->tabval[5]);
            /*_.DEBUGFUNBIND__V24*/
            meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7795:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_6_MELT_CALLCOUNT*/ meltfrout->tabval[6]);
            /*_.THECOUNTBIND__V25*/
            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7796:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_7_MELT_NEED_DBG*/ meltfrout->tabval[7]);
            /*_.THENEEDDBGBIND__V26*/
            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7797:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_8_MELT_INCREMENT_DBGCOUNTER*/ meltfrout->tabval[8]);
            /*_.THEINCRDBGBIND__V27*/
            meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.LIST_FIRST__V28*/
        meltfptr[27] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V18*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V29*/
        meltfptr[28] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V28*/ meltfptr[27])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7799:/ apply");
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
            /*_.XARGLIST__V30*/
            meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_PAIRLIST_AS_LIST*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.CURPAIR__V29*/ meltfptr[28]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:7800:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_1_st
            {
                struct meltpair_st rpair_0___PAIROFLIST_x3;
                struct meltlist_st rlist_1__LIST_;
                long meltletrec_1_endgap;
            } *meltletrec_1_ptr = 0;
            meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
            /*^blockmultialloc.initfill*/
            /*inipair rpair_0___PAIROFLIST_x3*/
            /*_._PAIROFLIST__V32*/
            meltfptr[31] = (melt_ptr_t) &meltletrec_1_ptr->rpair_0___PAIROFLIST_x3;
            meltletrec_1_ptr->rpair_0___PAIROFLIST_x3.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

            /*inilist rlist_1__LIST_*/
            /*_.LIST___V33*/
            meltfptr[32] = (melt_ptr_t) &meltletrec_1_ptr->rlist_1__LIST_;
            meltletrec_1_ptr->rlist_1__LIST_.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_LIST))));



            /*^putpairhead*/
            /*putpairhead*/
            melt_assertmsg("putpairhead /5 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V32*/ meltfptr[31]))== MELTOBMAG_PAIR);
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
            /*_.DBGARGLIST__V31*/
            meltfptr[30] = /*_.LIST___V33*/ meltfptr[32];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7800:/ clear");
            /*clear*/ /*_._PAIROFLIST__V32*/
            meltfptr[31] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V33*/
            meltfptr[32] = 0 ;
            /*^clear*/
            /*clear*/ /*_._PAIROFLIST__V32*/
            meltfptr[31] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V33*/
            meltfptr[32] = 0 ;
        } /*end multiallocblock*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7803:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            /*_.SEXPW__V34*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!S_EXPR_WEIGHT*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;
        /*_#NBSEPW__L9*/
        meltfnum[8] =
            (melt_get_int((melt_ptr_t)(/*_.SEXPW__V34*/ meltfptr[31])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#gtI__L10*/
        meltfnum[9] =
            ((/*_#NBSEPW__L9*/ meltfnum[8]) > (26));;
        MELT_LOCATION("warmelt-macro.melt:7806:/ cond");
        /*cond*/
        if (/*_#gtI__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*_#OR___L11*/ meltfnum[10] = /*_#gtI__L10*/ meltfnum[9];;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:7806:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*_#NULL__L12*/ meltfnum[11] =
                        ((/*_.SEXPW__V34*/ meltfptr[31]) == NULL);;
                    /*^compute*/
                    /*_#OR___L11*/
                    meltfnum[10] = /*_#NULL__L12*/ meltfnum[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7806:/ clear");
                    /*clear*/ /*_#NULL__L12*/
                    meltfnum[11] = 0 ;
                }
                ;
            }
        ;
        /*^cond*/
        /*cond*/
        if (/*_#OR___L11*/ meltfnum[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7811:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "weight:";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPW__V34*/ meltfptr[31];
                        /*_.STRING4OUT__V35*/
                        meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!STRING4OUT*/ meltfrout->tabval[11])), (melt_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[12])), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7809:/ locexp");
                        melt_warning_str(0, (melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "suspicious very heavy (DEBUG ....)"), (melt_ptr_t)(/*_.STRING4OUT__V35*/ meltfptr[32]));
                    }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7806:/ clear");
                    /*clear*/ /*_.STRING4OUT__V35*/
                    meltfptr[32] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-macro.melt:7803:/ clear");
        /*clear*/ /*_.SEXPW__V34*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBSEPW__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OR___L11*/
        meltfnum[10] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L13*/
        meltfnum[11] =
            (melt_magic_discr((melt_ptr_t)(/*_.DFILNAM__V20*/ meltfptr[19])) == MELTOBMAG_STRING);;
        /*^compute*/
        /*_#NOT__L14*/
        meltfnum[8] =
            (!(/*_#IS_STRING__L13*/ meltfnum[11]));;
        MELT_LOCATION("warmelt-macro.melt:7813:/ cond");
        /*cond*/
        if (/*_#NOT__L14*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7815:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "(DEBUG ...) used without file location"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7816:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7816:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7814:/ quasiblock");


                    /*_.PROGN___V38*/
                    meltfptr[37] = /*_.RETURN___V37*/ meltfptr[31];;
                    /*^compute*/
                    /*_._IF___V36*/
                    meltfptr[32] = /*_.PROGN___V38*/ meltfptr[37];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7813:/ clear");
                    /*clear*/ /*_.RETURN___V37*/
                    meltfptr[31] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V38*/
                    meltfptr[37] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V36*/ meltfptr[32] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L15*/
        meltfnum[9] =
            melt_is_instance_of((melt_ptr_t)(/*_.DEBUGFUNBIND__V24*/ meltfptr[22]), (melt_ptr_t)((/*!CLASS_FUNCTION_BINDING*/ meltfrout->tabval[14])));;
        MELT_LOCATION("warmelt-macro.melt:7817:/ cond");
        /*cond*/
        if (/*_#IS_A__L15*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*_#OR___L16*/ meltfnum[10] = /*_#IS_A__L15*/ meltfnum[9];;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:7817:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*_#IS_A__L17*/ meltfnum[16] =
                        melt_is_instance_of((melt_ptr_t)(/*_.DEBUGFUNBIND__V24*/ meltfptr[22]), (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[13])));;
                    /*^compute*/
                    /*_#OR___L16*/
                    meltfnum[10] = /*_#IS_A__L17*/ meltfnum[16];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7817:/ clear");
                    /*clear*/ /*_#IS_A__L17*/
                    meltfnum[16] = 0 ;
                }
                ;
            }
        ;
        /*_#NOT__L18*/
        meltfnum[16] =
            (!(/*_#OR___L16*/ meltfnum[10]));;
        MELT_LOCATION("warmelt-macro.melt:7817:/ cond");
        /*cond*/
        if (/*_#NOT__L18*/ meltfnum[16]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7820:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "(DEBUG ...) used in context with bad MELT_DEBUG_FUN"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7821:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7821:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:7819:/ quasiblock");


                    /*_.PROGN___V41*/
                    meltfptr[40] = /*_.RETURN___V40*/ meltfptr[37];;
                    /*^compute*/
                    /*_._IF___V39*/
                    meltfptr[31] = /*_.PROGN___V41*/ meltfptr[40];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7817:/ clear");
                    /*clear*/ /*_.RETURN___V40*/
                    meltfptr[37] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V41*/
                    meltfptr[40] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V39*/ meltfptr[31] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:7822:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L19*/
        meltfnum[18] =
            melt_is_instance_of((melt_ptr_t)(/*_.THECOUNTBIND__V25*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_PRIMITIVE_BINDING*/ meltfrout->tabval[15])));;
        MELT_LOCATION("warmelt-macro.melt:7825:/ cond");
        /*cond*/
        if (/*_#IS_A__L19*/ meltfnum[18]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:7826:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.THECOUNTBIND__V25*/ meltfptr[20]),
                                                      (melt_ptr_t)((/*!CLASS_PRIMITIVE_BINDING*/ meltfrout->tabval[15])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.THECOUNTBIND__V25*/ meltfptr[20]) /*=obj*/;
                                melt_object_get_field(slot,obj, 3, "PBIND_PRIMITIVE");
                                /*_.PBIND_PRIMITIVE__V43*/
                                meltfptr[40] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.PBIND_PRIMITIVE__V43*/ meltfptr[40] =  /*fromline 1341*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_.THECOUNTPRIM__V42*/
                    meltfptr[37] = /*_.PBIND_PRIMITIVE__V43*/ meltfptr[40];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7825:/ clear");
                    /*clear*/ /*_.PBIND_PRIMITIVE__V43*/
                    meltfptr[40] = 0 ;
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
                    /*_#IS_A__L20*/
                    meltfnum[19] =
                        melt_is_instance_of((melt_ptr_t)(/*_.THECOUNTBIND__V25*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[13])));;
                    MELT_LOCATION("warmelt-macro.melt:7827:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L20*/ meltfnum[19]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:7828:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.THECOUNTBIND__V25*/ meltfptr[20]),
                                                                  (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[13])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.THECOUNTBIND__V25*/ meltfptr[20]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                            /*_.VBIND_VALUE__V45*/
                                            meltfptr[44] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.VBIND_VALUE__V45*/ meltfptr[44] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_._IFELSE___V44*/
                                meltfptr[40] = /*_.VBIND_VALUE__V45*/ meltfptr[44];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7827:/ clear");
                                /*clear*/ /*_.VBIND_VALUE__V45*/
                                meltfptr[44] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:7830:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "(DEBUG ...) used in context with bad THE_MELTCALLCOUNT"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7831:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:7831:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:7829:/ quasiblock");


                                /*_.PROGN___V47*/
                                meltfptr[46] = /*_.RETURN___V46*/ meltfptr[44];;
                                /*^compute*/
                                /*_._IFELSE___V44*/
                                meltfptr[40] = /*_.PROGN___V47*/ meltfptr[46];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7827:/ clear");
                                /*clear*/ /*_.RETURN___V46*/
                                meltfptr[44] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V47*/
                                meltfptr[46] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_.THECOUNTPRIM__V42*/
                    meltfptr[37] = /*_._IFELSE___V44*/ meltfptr[40];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7825:/ clear");
                    /*clear*/ /*_#IS_A__L20*/
                    meltfnum[19] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V44*/
                    meltfptr[40] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7835:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_2_st
            {
                struct MELT_MULTIPLE_STRUCT(0) rtup_0__TUPLREC__x11;
                long meltletrec_2_endgap;
            } *meltletrec_2_ptr = 0;
            meltletrec_2_ptr = (struct meltletrec_2_st *) meltgc_allocate (sizeof (struct meltletrec_2_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x11*/
            /*_.TUPLREC___V49*/
            meltfptr[46] = (melt_ptr_t) &meltletrec_2_ptr->rtup_0__TUPLREC__x11;
            meltletrec_2_ptr->rtup_0__TUPLREC__x11.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_2_ptr->rtup_0__TUPLREC__x11.nbval = 0;


            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V49*/ meltfptr[46]);
            ;
            /*_.TUPLE___V48*/
            meltfptr[44] = /*_.TUPLREC___V49*/ meltfptr[46];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7835:/ clear");
            /*clear*/ /*_.TUPLREC___V49*/
            meltfptr[46] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V49*/
            meltfptr[46] = 0 ;
        } /*end multiallocblock*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7832:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PRIMITIVE*/ meltfrout->tabval[16])), (4), "CLASS_SOURCE_PRIMITIVE");
            /*_.INST__V51*/
            meltfptr[46] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V51*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V51*/ meltfptr[46]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPRIM_OPER", melt_magic_discr((melt_ptr_t)(/*_.INST__V51*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V51*/ meltfptr[46]), (3), (/*_.THECOUNTPRIM__V42*/ meltfptr[37]), "SPRIM_OPER");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V51*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V51*/ meltfptr[46]), (2), (/*_.TUPLE___V48*/ meltfptr[44]), "SARGOP_ARGS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V51*/ meltfptr[46], "newly made instance");
        ;
        /*_.DCOUNT__V50*/
        meltfptr[40] = /*_.INST__V51*/ meltfptr[46];;

        {
            MELT_LOCATION("warmelt-macro.melt:7837:/ locexp");
            meltgc_append_list((melt_ptr_t)(/*_.DBGARGLIST__V31*/ meltfptr[30]), (melt_ptr_t)(/*_.DCOUNT__V50*/ meltfptr[40]));
        }
        ;

        MELT_LOCATION("warmelt-macro.melt:7822:/ clear");
        /*clear*/ /*_#IS_A__L19*/
        meltfnum[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.THECOUNTPRIM__V42*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLE___V48*/
        meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DCOUNT__V50*/
        meltfptr[40] = 0 ;

        {
            MELT_LOCATION("warmelt-macro.melt:7839:/ locexp");
            meltgc_append_list((melt_ptr_t)(/*_.DBGARGLIST__V31*/ meltfptr[30]), (melt_ptr_t)(/*_.DFILNAM__V20*/ meltfptr[19]));
        }
        ;
        /*_.MAKE_INTEGERBOX__V52*/
        meltfptr[37] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_CONSTANT_INTEGER*/ meltfrout->tabval[17])), (/*_#DLINE__L8*/ meltfnum[1])));;

        {
            MELT_LOCATION("warmelt-macro.melt:7840:/ locexp");
            meltgc_append_list((melt_ptr_t)(/*_.DBGARGLIST__V31*/ meltfptr[30]), (melt_ptr_t)(/*_.MAKE_INTEGERBOX__V52*/ meltfptr[37]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7841:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.XARGLIST__V30*/ meltfptr[29];
            /*_.LIST_APPEND2LIST__V53*/
            meltfptr[44] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_APPEND2LIST*/ meltfrout->tabval[18])), (melt_ptr_t)(/*_.DBGARGLIST__V31*/ meltfptr[30]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7842:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7842:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L21*/ meltfnum[19]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:7842:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[18];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7842;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_debug dbgarglist";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.DBGARGLIST__V31*/ meltfptr[30];
                            /*_.MELT_DEBUG_FUN__V55*/
                            meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V54*/
                        meltfptr[40] = /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7842:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L22*/
                        meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V55*/
                        meltfptr[54] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V54*/ meltfptr[40] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7842:/ locexp");
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
            /*clear*/ /*_._IF___V54*/
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
        MELT_LOCATION("warmelt-macro.melt:7843:/ quasiblock");


        /*_#NBDBGARG__L23*/
        meltfnum[18] =
            (melt_list_length((melt_ptr_t)(/*_.DBGARGLIST__V31*/ meltfptr[30])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L24*/
        meltfnum[19] =
            melt_is_instance_of((melt_ptr_t)(/*_.THENEEDDBGBIND__V26*/ meltfptr[25]), (melt_ptr_t)((/*!CLASS_PRIMITIVE_BINDING*/ meltfrout->tabval[15])));;
        MELT_LOCATION("warmelt-macro.melt:7847:/ cond");
        /*cond*/
        if (/*_#IS_A__L24*/ meltfnum[19]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:7848:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.THENEEDDBGBIND__V26*/ meltfptr[25]),
                                                      (melt_ptr_t)((/*!CLASS_PRIMITIVE_BINDING*/ meltfrout->tabval[15])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.THENEEDDBGBIND__V26*/ meltfptr[25]) /*=obj*/;
                                melt_object_get_field(slot,obj, 3, "PBIND_PRIMITIVE");
                                /*_.PBIND_PRIMITIVE__V58*/
                                meltfptr[57] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.PBIND_PRIMITIVE__V58*/ meltfptr[57] =  /*fromline 1341*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_.THENEEDDBG__V57*/
                    meltfptr[40] = /*_.PBIND_PRIMITIVE__V58*/ meltfptr[57];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7847:/ clear");
                    /*clear*/ /*_.PBIND_PRIMITIVE__V58*/
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
                    /*_#IS_A__L25*/
                    meltfnum[24] =
                        melt_is_instance_of((melt_ptr_t)(/*_.THENEEDDBGBIND__V26*/ meltfptr[25]), (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[13])));;
                    MELT_LOCATION("warmelt-macro.melt:7849:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L25*/ meltfnum[24]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:7850:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.THENEEDDBGBIND__V26*/ meltfptr[25]),
                                                                  (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[13])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.THENEEDDBGBIND__V26*/ meltfptr[25]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                            /*_.VBIND_VALUE__V60*/
                                            meltfptr[59] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.VBIND_VALUE__V60*/ meltfptr[59] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_._IFELSE___V59*/
                                meltfptr[57] = /*_.VBIND_VALUE__V60*/ meltfptr[59];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7849:/ clear");
                                /*clear*/ /*_.VBIND_VALUE__V60*/
                                meltfptr[59] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:7852:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "(DEBUG ...) used in context with bad MELT_NEED_DBG"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7853:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:7853:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:7851:/ quasiblock");


                                /*_.PROGN___V62*/
                                meltfptr[61] = /*_.RETURN___V61*/ meltfptr[59];;
                                /*^compute*/
                                /*_._IFELSE___V59*/
                                meltfptr[57] = /*_.PROGN___V62*/ meltfptr[61];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7849:/ clear");
                                /*clear*/ /*_.RETURN___V61*/
                                meltfptr[59] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V62*/
                                meltfptr[61] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_.THENEEDDBG__V57*/
                    meltfptr[40] = /*_._IFELSE___V59*/ meltfptr[57];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7847:/ clear");
                    /*clear*/ /*_#IS_A__L25*/
                    meltfnum[24] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V59*/
                    meltfptr[57] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7857:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_3_st
            {
                struct MELT_MULTIPLE_STRUCT(1) rtup_0__TUPLREC__x12;
                long meltletrec_3_endgap;
            } *meltletrec_3_ptr = 0;
            meltletrec_3_ptr = (struct meltletrec_3_st *) meltgc_allocate (sizeof (struct meltletrec_3_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x12*/
            /*_.TUPLREC___V64*/
            meltfptr[61] = (melt_ptr_t) &meltletrec_3_ptr->rtup_0__TUPLREC__x12;
            meltletrec_3_ptr->rtup_0__TUPLREC__x12.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_3_ptr->rtup_0__TUPLREC__x12.nbval = 1;


            /*^putuple*/
            /*putupl#15*/
            melt_assertmsg("putupl [:7857] #15 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V64*/ meltfptr[61]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7857] #15 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V64*/ meltfptr[61]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V64*/ meltfptr[61]))->tabval[0] = (melt_ptr_t)((/*!konst_19*/ meltfrout->tabval[19]));
            ;
            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V64*/ meltfptr[61]);
            ;
            /*_.TUPLE___V63*/
            meltfptr[59] = /*_.TUPLREC___V64*/ meltfptr[61];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7857:/ clear");
            /*clear*/ /*_.TUPLREC___V64*/
            meltfptr[61] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V64*/
            meltfptr[61] = 0 ;
        } /*end multiallocblock*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7854:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PRIMITIVE*/ meltfrout->tabval[16])), (4), "CLASS_SOURCE_PRIMITIVE");
            /*_.INST__V66*/
            meltfptr[61] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V66*/ meltfptr[61])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V66*/ meltfptr[61]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPRIM_OPER", melt_magic_discr((melt_ptr_t)(/*_.INST__V66*/ meltfptr[61])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V66*/ meltfptr[61]), (3), (/*_.THENEEDDBG__V57*/ meltfptr[40]), "SPRIM_OPER");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V66*/ meltfptr[61])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V66*/ meltfptr[61]), (2), (/*_.TUPLE___V63*/ meltfptr[59]), "SARGOP_ARGS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V66*/ meltfptr[61], "newly made instance");
        ;
        /*_.DNEEDBG__V65*/
        meltfptr[57] = /*_.INST__V66*/ meltfptr[61];;

        MELT_CHECK_SIGNAL();
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7862:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[23]);
            /*_.LIST_TO_MULTIPLE__V67*/
            meltfptr[66] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[22])), (melt_ptr_t)(/*_.DBGARGLIST__V31*/ meltfptr[30]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:7858:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_APPLY*/ meltfrout->tabval[20])), (4), "CLASS_SOURCE_APPLY");
            /*_.INST__V69*/
            meltfptr[68] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V69*/ meltfptr[68])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V69*/ meltfptr[68]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SAPP_FUN", melt_magic_discr((melt_ptr_t)(/*_.INST__V69*/ meltfptr[68])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V69*/ meltfptr[68]), (3), ((/*!konst_5_MELT_DEBUG_FUN*/ meltfrout->tabval[5])), "SAPP_FUN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V69*/ meltfptr[68])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V69*/ meltfptr[68]), (2), (/*_.LIST_TO_MULTIPLE__V67*/ meltfptr[66]), "SARGOP_ARGS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V69*/ meltfptr[68], "newly made instance");
        ;
        /*_.DCALL__V68*/
        meltfptr[67] = /*_.INST__V69*/ meltfptr[68];;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L26*/
        meltfnum[24] =
            melt_is_instance_of((melt_ptr_t)(/*_.THEINCRDBGBIND__V27*/ meltfptr[26]), (melt_ptr_t)((/*!CLASS_PRIMITIVE_BINDING*/ meltfrout->tabval[15])));;
        MELT_LOCATION("warmelt-macro.melt:7865:/ cond");
        /*cond*/
        if (/*_#IS_A__L26*/ meltfnum[24]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:7866:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.THEINCRDBGBIND__V27*/ meltfptr[26]),
                                                      (melt_ptr_t)((/*!CLASS_PRIMITIVE_BINDING*/ meltfrout->tabval[15])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.THEINCRDBGBIND__V27*/ meltfptr[26]) /*=obj*/;
                                melt_object_get_field(slot,obj, 3, "PBIND_PRIMITIVE");
                                /*_.PBIND_PRIMITIVE__V71*/
                                meltfptr[70] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.PBIND_PRIMITIVE__V71*/ meltfptr[70] =  /*fromline 1341*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_.THEINCRDBG__V70*/
                    meltfptr[69] = /*_.PBIND_PRIMITIVE__V71*/ meltfptr[70];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7865:/ clear");
                    /*clear*/ /*_.PBIND_PRIMITIVE__V71*/
                    meltfptr[70] = 0 ;
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
                    /*_#IS_A__L27*/
                    meltfnum[26] =
                        melt_is_instance_of((melt_ptr_t)(/*_.THEINCRDBGBIND__V27*/ meltfptr[26]), (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[13])));;
                    MELT_LOCATION("warmelt-macro.melt:7867:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L27*/ meltfnum[26]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:7868:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.THEINCRDBGBIND__V27*/ meltfptr[26]),
                                                                  (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[13])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.THEINCRDBGBIND__V27*/ meltfptr[26]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                            /*_.VBIND_VALUE__V73*/
                                            meltfptr[72] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.VBIND_VALUE__V73*/ meltfptr[72] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_._IFELSE___V72*/
                                meltfptr[70] = /*_.VBIND_VALUE__V73*/ meltfptr[72];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7867:/ clear");
                                /*clear*/ /*_.VBIND_VALUE__V73*/
                                meltfptr[72] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:7870:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "(DEBUG ...) used in context with bad MELT_INCREMENT_DBGCOUNTER"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7873:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:7873:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:7869:/ quasiblock");


                                /*_.PROGN___V75*/
                                meltfptr[74] = /*_.RETURN___V74*/ meltfptr[72];;
                                /*^compute*/
                                /*_._IFELSE___V72*/
                                meltfptr[70] = /*_.PROGN___V75*/ meltfptr[74];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7867:/ clear");
                                /*clear*/ /*_.RETURN___V74*/
                                meltfptr[72] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V75*/
                                meltfptr[74] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_.THEINCRDBG__V70*/
                    meltfptr[69] = /*_._IFELSE___V72*/ meltfptr[70];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7865:/ clear");
                    /*clear*/ /*_#IS_A__L27*/
                    meltfnum[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V72*/
                    meltfptr[70] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7878:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_4_st
            {
                struct MELT_MULTIPLE_STRUCT(0) rtup_0__TUPLREC__x13;
                long meltletrec_4_endgap;
            } *meltletrec_4_ptr = 0;
            meltletrec_4_ptr = (struct meltletrec_4_st *) meltgc_allocate (sizeof (struct meltletrec_4_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x13*/
            /*_.TUPLREC___V77*/
            meltfptr[74] = (melt_ptr_t) &meltletrec_4_ptr->rtup_0__TUPLREC__x13;
            meltletrec_4_ptr->rtup_0__TUPLREC__x13.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_4_ptr->rtup_0__TUPLREC__x13.nbval = 0;


            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V77*/ meltfptr[74]);
            ;
            /*_.TUPLE___V76*/
            meltfptr[72] = /*_.TUPLREC___V77*/ meltfptr[74];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7878:/ clear");
            /*clear*/ /*_.TUPLREC___V77*/
            meltfptr[74] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V77*/
            meltfptr[74] = 0 ;
        } /*end multiallocblock*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7874:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PRIMITIVE*/ meltfrout->tabval[16])), (4), "CLASS_SOURCE_PRIMITIVE");
            /*_.INST__V79*/
            meltfptr[74] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V79*/ meltfptr[74])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V79*/ meltfptr[74]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPRIM_OPER", melt_magic_discr((melt_ptr_t)(/*_.INST__V79*/ meltfptr[74])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V79*/ meltfptr[74]), (3), (/*_.THEINCRDBG__V70*/ meltfptr[69]), "SPRIM_OPER");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V79*/ meltfptr[74])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V79*/ meltfptr[74]), (2), (/*_.TUPLE___V76*/ meltfptr[72]), "SARGOP_ARGS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V79*/ meltfptr[74], "newly made instance");
        ;
        /*_.DINCRDBG__V78*/
        meltfptr[70] = /*_.INST__V79*/ meltfptr[74];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7879:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IF*/ meltfrout->tabval[24])), (4), "CLASS_SOURCE_IF");
            /*_.INST__V81*/
            meltfptr[80] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V81*/ meltfptr[80])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V81*/ meltfptr[80]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V81*/ meltfptr[80])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V81*/ meltfptr[80]), (2), (/*_.DNEEDBG__V65*/ meltfptr[57]), "SIF_TEST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V81*/ meltfptr[80])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V81*/ meltfptr[80]), (3), (/*_.DCALL__V68*/ meltfptr[67]), "SIF_THEN");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V81*/ meltfptr[80], "newly made instance");
        ;
        /*_.DIFDBG__V80*/
        meltfptr[79] = /*_.INST__V81*/ meltfptr[80];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7888:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_5_st
            {
                struct MELT_MULTIPLE_STRUCT(0) rtup_0__TUPLREC__x14;
                long meltletrec_5_endgap;
            } *meltletrec_5_ptr = 0;
            meltletrec_5_ptr = (struct meltletrec_5_st *) meltgc_allocate (sizeof (struct meltletrec_5_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x14*/
            /*_.TUPLREC___V83*/
            meltfptr[82] = (melt_ptr_t) &meltletrec_5_ptr->rtup_0__TUPLREC__x14;
            meltletrec_5_ptr->rtup_0__TUPLREC__x14.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_5_ptr->rtup_0__TUPLREC__x14.nbval = 0;


            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V83*/ meltfptr[82]);
            ;
            /*_.TUPLE___V82*/
            meltfptr[81] = /*_.TUPLREC___V83*/ meltfptr[82];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7888:/ clear");
            /*clear*/ /*_.TUPLREC___V83*/
            meltfptr[82] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V83*/
            meltfptr[82] = 0 ;
        } /*end multiallocblock*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7884:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PRIMITIVE*/ meltfrout->tabval[16])), (4), "CLASS_SOURCE_PRIMITIVE");
            /*_.INST__V85*/
            meltfptr[84] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V85*/ meltfptr[84])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V85*/ meltfptr[84]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPRIM_OPER", melt_magic_discr((melt_ptr_t)(/*_.INST__V85*/ meltfptr[84])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V85*/ meltfptr[84]), (3), ((/*!VOID*/ meltfrout->tabval[25])), "SPRIM_OPER");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V85*/ meltfptr[84])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V85*/ meltfptr[84]), (2), (/*_.TUPLE___V82*/ meltfptr[81]), "SARGOP_ARGS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V85*/ meltfptr[84], "newly made instance");
        ;
        /*_.DVOID__V84*/
        meltfptr[82] = /*_.INST__V85*/ meltfptr[84];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7892:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_6_st
            {
                struct MELT_MULTIPLE_STRUCT(3) rtup_0__TUPLREC__x15;
                long meltletrec_6_endgap;
            } *meltletrec_6_ptr = 0;
            meltletrec_6_ptr = (struct meltletrec_6_st *) meltgc_allocate (sizeof (struct meltletrec_6_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x15*/
            /*_.TUPLREC___V87*/
            meltfptr[86] = (melt_ptr_t) &meltletrec_6_ptr->rtup_0__TUPLREC__x15;
            meltletrec_6_ptr->rtup_0__TUPLREC__x15.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_6_ptr->rtup_0__TUPLREC__x15.nbval = 3;


            /*^putuple*/
            /*putupl#16*/
            melt_assertmsg("putupl [:7892] #16 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V87*/ meltfptr[86]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7892] #16 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V87*/ meltfptr[86]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V87*/ meltfptr[86]))->tabval[0] = (melt_ptr_t)(/*_.DINCRDBG__V78*/ meltfptr[70]);
            ;
            /*^putuple*/
            /*putupl#17*/
            melt_assertmsg("putupl [:7892] #17 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V87*/ meltfptr[86]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7892] #17 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V87*/ meltfptr[86]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V87*/ meltfptr[86]))->tabval[1] = (melt_ptr_t)(/*_.DIFDBG__V80*/ meltfptr[79]);
            ;
            /*^putuple*/
            /*putupl#18*/
            melt_assertmsg("putupl [:7892] #18 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V87*/ meltfptr[86]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7892] #18 checkoff", (2>=0 && 2< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V87*/ meltfptr[86]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V87*/ meltfptr[86]))->tabval[2] = (melt_ptr_t)(/*_.DVOID__V84*/ meltfptr[82]);
            ;
            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V87*/ meltfptr[86]);
            ;
            /*_.TUPLE___V86*/
            meltfptr[85] = /*_.TUPLREC___V87*/ meltfptr[86];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7892:/ clear");
            /*clear*/ /*_.TUPLREC___V87*/
            meltfptr[86] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V87*/
            meltfptr[86] = 0 ;
        } /*end multiallocblock*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7889:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PROGN*/ meltfrout->tabval[26])), (3), "CLASS_SOURCE_PROGN");
            /*_.INST__V89*/
            meltfptr[88] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V89*/ meltfptr[88])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V89*/ meltfptr[88]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPROGN_BODY", melt_magic_discr((melt_ptr_t)(/*_.INST__V89*/ meltfptr[88])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V89*/ meltfptr[88]), (2), (/*_.TUPLE___V86*/ meltfptr[85]), "SPROGN_BODY");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V89*/ meltfptr[88], "newly made instance");
        ;
        /*_.DPROGN__V88*/
        meltfptr[86] = /*_.INST__V89*/ meltfptr[88];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7894:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_CPPIF*/ meltfrout->tabval[27])), (5), "CLASS_SOURCE_CPPIF");
            /*_.INST__V91*/
            meltfptr[90] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V91*/ meltfptr[90])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V91*/ meltfptr[90]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_COND", melt_magic_discr((melt_ptr_t)(/*_.INST__V91*/ meltfptr[90])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V91*/ meltfptr[90]), (2), ((/*!konst_28_MELT_HAVE_DEBUG*/ meltfrout->tabval[28])), "SIFP_COND");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V91*/ meltfptr[90])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V91*/ meltfptr[90]), (3), (/*_.DPROGN__V88*/ meltfptr[86]), "SIFP_THEN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V91*/ meltfptr[90])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V91*/ meltfptr[90]), (4), (/*_.DVOID__V84*/ meltfptr[82]), "SIFP_ELSE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V91*/ meltfptr[90], "newly made instance");
        ;
        /*_.DCPPIF__V90*/
        meltfptr[89] = /*_.INST__V91*/ meltfptr[90];;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#gteqI__L28*/
        meltfnum[26] =
            ((/*_#NBDBGARG__L23*/ meltfnum[18]) >= (15));;
        MELT_LOCATION("warmelt-macro.melt:7902:/ cond");
        /*cond*/
        if (/*_#gteqI__L28*/ meltfnum[26]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:7903:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_long = /*_#NBDBGARG__L23*/ meltfnum[18];
                        /*_.STRNBARG__V92*/
                        meltfptr[91] =  melt_apply ((meltclosure_ptr_t)((/*!STRING4OUT*/ meltfrout->tabval[11])), (melt_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[12])), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:7905:/ locexp");
                        melt_inform_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "quite long (DEBUG ...), perhaps consider splitting it"), (melt_ptr_t)(/*_.STRNBARG__V92*/ meltfptr[91]));
                    }
                    ;

                    MELT_LOCATION("warmelt-macro.melt:7903:/ clear");
                    /*clear*/ /*_.STRNBARG__V92*/
                    meltfptr[91] = 0 ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7908:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L29*/
            meltfnum[28] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:7908:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L29*/ meltfnum[28]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L30*/ meltfnum[29] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:7908:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L30*/ meltfnum[29];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7908;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_debug dcall=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.DCALL__V68*/ meltfptr[67];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " return dcppif=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.DCPPIF__V90*/ meltfptr[89];
                            /*_.MELT_DEBUG_FUN__V94*/
                            meltfptr[93] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V93*/
                        meltfptr[91] = /*_.MELT_DEBUG_FUN__V94*/ meltfptr[93];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7908:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L30*/
                        meltfnum[29] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V94*/
                        meltfptr[93] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V93*/ meltfptr[91] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7908:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L29*/
            meltfnum[28] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V93*/
            meltfptr[91] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


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
        MELT_LOCATION("warmelt-macro.melt:7909:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.DCPPIF__V90*/ meltfptr[89];;

        {
            MELT_LOCATION("warmelt-macro.melt:7909:/ locexp");
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
        meltfptr[54] = /*_.RETURN___V95*/ meltfptr[93];;

        MELT_LOCATION("warmelt-macro.melt:7843:/ clear");
        /*clear*/ /*_#NBDBGARG__L23*/
        meltfnum[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L24*/
        meltfnum[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.THENEEDDBG__V57*/
        meltfptr[40] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLE___V63*/
        meltfptr[59] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DNEEDBG__V65*/
        meltfptr[57] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_TO_MULTIPLE__V67*/
        meltfptr[66] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DCALL__V68*/
        meltfptr[67] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L26*/
        meltfnum[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.THEINCRDBG__V70*/
        meltfptr[69] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLE___V76*/
        meltfptr[72] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DINCRDBG__V78*/
        meltfptr[70] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DIFDBG__V80*/
        meltfptr[79] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLE___V82*/
        meltfptr[81] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DVOID__V84*/
        meltfptr[82] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLE___V86*/
        meltfptr[85] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DPROGN__V88*/
        meltfptr[86] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DCPPIF__V90*/
        meltfptr[89] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gteqI__L28*/
        meltfnum[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V95*/
        meltfptr[93] = 0 ;
        /*_.LET___V17*/
        meltfptr[15] = /*_.LET___V56*/ meltfptr[54];;

        MELT_LOCATION("warmelt-macro.melt:7787:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_MIXINT__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DFILNAM__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#DLINE__L8*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DEBUGFUNBIND__V24*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.THECOUNTBIND__V25*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.THENEEDDBGBIND__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.THEINCRDBGBIND__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XARGLIST__V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DBGARGLIST__V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L13*/
        meltfnum[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L14*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V36*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L15*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OR___L16*/
        meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L18*/
        meltfnum[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V39*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V52*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_APPEND2LIST__V53*/
        meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V56*/
        meltfptr[54] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7782:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:7782:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_DEBUG", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES fromline 1716 */

    /** start of frame for meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES// fromline 1531
        : public Melt_CallFrameWithValues<32>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES), clos) {};
        MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES() //the constructor fromline 1606
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES)) {};
        MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES


    /** end of frame for meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES fromline 1661**/

    /* end of frame for routine meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES fromline 1720 */

    /* classy proc frame meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES */
    MeltFrame_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_EXPORT_VALUES", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7927:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7928:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7928:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7928:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7928;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_export_values sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7928:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7928:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7929:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7929:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7929:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7929;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7929:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7929:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7930:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:7930:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7930:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7930;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7930:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7930:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7931:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:7931:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7931:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check mexpander";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7931;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7931:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7931:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7932:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7932:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7932:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7932;
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/
                            meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7932:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/
                        meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/
            meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7932:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7933:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7934:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V21*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7935:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V22*/
            meltfptr[21] = slot;
        };
        ;
        /*_.LIST_FIRST__V23*/
        meltfptr[22] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V21*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V24*/
        meltfptr[23] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V23*/ meltfptr[22])));;
        MELT_LOCATION("warmelt-macro.melt:7939:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V26*/
        meltfptr[25] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_8*/ meltfrout->tabval[8])), (4));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V26*/ meltfptr[25])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V26*/ meltfptr[25])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V26*/ meltfptr[25])->tabval[0] = (melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V26*/ meltfptr[25])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V26*/ meltfptr[25])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V26*/ meltfptr[25])->tabval[1] = (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V26*/ meltfptr[25])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V26*/ meltfptr[25])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V26*/ meltfptr[25])->tabval[2] = (melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V26*/ meltfptr[25])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 3>= 0 && 3< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V26*/ meltfptr[25])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V26*/ meltfptr[25])->tabval[3] = (melt_ptr_t)(/*_.LOC__V22*/ meltfptr[21]);
        ;
        /*_.LAMBDA___V25*/
        meltfptr[24] = /*_.LAMBDA___V26*/ meltfptr[25];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7937:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[5]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V25*/ meltfptr[24];
            /*_.SYMBTUP__V27*/
            meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_MULTIPLE*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7947:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_EXPORT_VALUES*/ meltfrout->tabval[9])), (3), "CLASS_SOURCE_EXPORT_VALUES");
            /*_.INST__V29*/
            meltfptr[28] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V29*/ meltfptr[28])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V29*/ meltfptr[28]), (1), (/*_.LOC__V22*/ meltfptr[21]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXPORT_NAMES", melt_magic_discr((melt_ptr_t)(/*_.INST__V29*/ meltfptr[28])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V29*/ meltfptr[28]), (2), (/*_.SYMBTUP__V27*/ meltfptr[26]), "SEXPORT_NAMES");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V29*/ meltfptr[28], "newly made instance");
        ;
        /*_.RES__V28*/
        meltfptr[27] = /*_.INST__V29*/ meltfptr[28];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7951:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7951:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7951:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7951;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_export_values result res";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V28*/ meltfptr[27];
                            /*_.MELT_DEBUG_FUN__V31*/
                            meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V30*/
                        meltfptr[29] = /*_.MELT_DEBUG_FUN__V31*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7951:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L8*/
                        meltfnum[0] = 0 ;
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
                MELT_LOCATION("warmelt-macro.melt:7951:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7952:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V28*/ meltfptr[27];;

        {
            MELT_LOCATION("warmelt-macro.melt:7952:/ locexp");
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
        /*_.LET___V20*/
        meltfptr[18] = /*_.RETURN___V32*/ meltfptr[30];;

        MELT_LOCATION("warmelt-macro.melt:7933:/ clear");
        /*clear*/ /*_.CONT__V21*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SYMBTUP__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V32*/
        meltfptr[30] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7927:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V20*/ meltfptr[18];;

        {
            MELT_LOCATION("warmelt-macro.melt:7927:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
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
    melt_trace_end("MEXPAND_EXPORT_VALUES", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_165_WARMELTmiMACRO_LAMBDA_cl39(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_165_WARMELTmiMACRO_LAMBDA_cl39 fromline 1716 */

    /** start of frame for meltrout_165_WARMELTmiMACRO_LAMBDA_cl39 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39// fromline 1531
        : public Melt_CallFrameWithValues<6>
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
        MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39), clos) {};
        MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39() //the constructor fromline 1606
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39)) {};
        MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39


    /** end of frame for meltrout_165_WARMELTmiMACRO_LAMBDA_cl39 fromline 1661**/

    /* end of frame for routine meltrout_165_WARMELTmiMACRO_LAMBDA_cl39 fromline 1720 */

    /* classy proc frame meltrout_165_WARMELTmiMACRO_LAMBDA_cl39 */
    MeltFrame_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_165_WARMELTmiMACRO_LAMBDA_cl39 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl39", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7939:/ getarg");
    /*_.S__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-macro.melt:7940:/ quasiblock");



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
            /*_.SYM__V4*/
            meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*~MEXPANDER*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.S__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L1*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.SYM__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-macro.melt:7942:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:7943:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
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
                        meltfnum[1] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:7943:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L2*/ meltfnum[1]) /*then*/
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
                                    MELT_LOCATION("warmelt-macro.melt:7943:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7943;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_export_values bad sym=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SYM__V4*/ meltfptr[3];
                                        /*_.MELT_DEBUG_FUN__V6*/
                                        meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V5*/
                                    meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:7943:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L3*/
                                    meltfnum[2] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V6*/
                                    meltfptr[5] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V5*/ meltfptr[4] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:7943:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L2*/
                        meltfnum[1] = 0 ;
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

                    {
                        MELT_LOCATION("warmelt-macro.melt:7944:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)((/*~LOC*/ meltfclos->tabval[3])), ( "(EXPORT_VALUES <sym>...) expecting symbol"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7942:/ quasiblock");


                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.LET___V3*/
        meltfptr[2] = /*_.SYM__V4*/ meltfptr[3];;

        MELT_LOCATION("warmelt-macro.melt:7940:/ clear");
        /*clear*/ /*_.SYM__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L1*/
        meltfnum[0] = 0 ;
        MELT_LOCATION("warmelt-macro.melt:7939:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-macro.melt:7939:/ locexp");
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
        /*clear*/ /*_.LET___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl39", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_165_WARMELTmiMACRO_LAMBDA_cl39_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_165_WARMELTmiMACRO_LAMBDA_cl39*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO fromline 1716 */

    /** start of frame for meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO// fromline 1531
        : public Melt_CallFrameWithValues<63>
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
        MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<63> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO), clos) {};
        MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO() //the constructor fromline 1606
            : Melt_CallFrameWithValues<63> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<63> (fil,lin, sizeof(MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO)) {};
        MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<63> (fil,lin, sizeof(MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO


    /** end of frame for meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO fromline 1661**/

    /* end of frame for routine meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO fromline 1720 */

    /* classy proc frame meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO */
    MeltFrame_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_EXPORT_MACRO", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:7966:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:7967:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7967:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:7967:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7967;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_export_macro sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7967:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:7967:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:7968:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:7968:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7968:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7968;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7968:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7968:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:7969:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:7969:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7969:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7969;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7969:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7969:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7970:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:7970:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7970:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check mexpander";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7970;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7970:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7970:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7971:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:7971:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:7971:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7971;
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/
                            meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7971:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/
                        meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/
            meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:7971:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:7972:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7973:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V21*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:7974:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V22*/
            meltfptr[21] = slot;
        };
        ;
        /*_.LIST_FIRST__V23*/
        meltfptr[22] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V21*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V24*/
        meltfptr[23] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V23*/ meltfptr[22])));;
        /*^compute*/
        /*_.PAIR_HEAD__V25*/
        meltfptr[24] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7976:/ apply");
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
            /*_.SYMB__V26*/
            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.PAIR_HEAD__V25*/ meltfptr[24]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.DOC__V27*/
        meltfptr[26] = (/*nil*/NULL);;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L7*/
        meltfnum[1] =
            !melt_is_instance_of((melt_ptr_t)(/*_.SYMB__V26*/ meltfptr[25]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:7979:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L7*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:7980:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V22*/ meltfptr[21]), ( "(EXPORT_MACRO <sym> [<expander>]) expecting symbol"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V28*/
        meltfptr[27] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
        MELT_LOCATION("warmelt-macro.melt:7981:/ compute");
        /*_.CURPAIR__V24*/
        meltfptr[23] = /*_.SETQ___V29*/ meltfptr[28] = /*_.PAIR_TAIL__V28*/ meltfptr[27];;
        MELT_LOCATION("warmelt-macro.melt:7982:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:7983:/ quasiblock");


        /*_.SE__V32*/
        meltfptr[31] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7985:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:7985:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L8*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:7985:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7985;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_export_macro sexpv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SE__V32*/ meltfptr[31];
                            /*_.MELT_DEBUG_FUN__V34*/
                            meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V33*/
                        meltfptr[32] = /*_.MELT_DEBUG_FUN__V34*/ meltfptr[33];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7985:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/
                        meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V34*/
                        meltfptr[33] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V33*/ meltfptr[32] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:7985:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L8*/
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
        /*_.LET___V31*/
        meltfptr[30] = /*_.SE__V32*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:7983:/ clear");
        /*clear*/ /*_.SE__V32*/
        meltfptr[31] = 0 ;
        /*_.SEXPV__V35*/
        meltfptr[33] = /*_.LET___V31*/ meltfptr[30];;
        MELT_LOCATION("warmelt-macro.melt:7987:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SYMB__V26*/ meltfptr[25];
            /*_.CB__V37*/
            meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:7989:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:7989:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L10*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L11*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:7989:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7989;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_export_macro cbind=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CB__V37*/ meltfptr[31];
                            /*_.MELT_DEBUG_FUN__V39*/
                            meltfptr[38] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V38*/
                        meltfptr[37] = /*_.MELT_DEBUG_FUN__V39*/ meltfptr[38];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:7989:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L11*/
                        meltfnum[0] = 0 ;
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
                MELT_LOCATION("warmelt-macro.melt:7989:/ locexp");
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
        /*_.LET___V36*/
        meltfptr[32] = /*_.CB__V37*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:7987:/ clear");
        /*clear*/ /*_.CB__V37*/
        meltfptr[31] = 0 ;
        /*_.CBIND__V40*/
        meltfptr[38] = /*_.LET___V36*/ meltfptr[32];;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L12*/
        meltfnum[0] =
            ((/*_.SEXPV__V35*/ meltfptr[33]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:7991:/ cond");
        /*cond*/
        if (/*_#NULL__L12*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_NOT_A__L13*/
                    meltfnum[8] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.CBIND__V40*/ meltfptr[38]), (melt_ptr_t)((/*!CLASS_DEFINED_MACRO_BINDING*/ meltfrout->tabval[6])));;
                    MELT_LOCATION("warmelt-macro.melt:7992:/ cond");
                    /*cond*/
                    if (/*_#IS_NOT_A__L13*/ meltfnum[8]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:7994:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SYMB__V26*/ meltfptr[25]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[7])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.SYMB__V26*/ meltfptr[25]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMED_NAME__V43*/
                                            meltfptr[42] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMED_NAME__V43*/ meltfptr[42] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:7993:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V22*/ meltfptr[21]), ( "(EXPORT_MACRO <sym> <expander>) needs an expander for non-macro symbol"), (melt_ptr_t)(/*_.NAMED_NAME__V43*/ meltfptr[42]));
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:7995:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:7995:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:7992:/ quasiblock");


                                /*_.PROGN___V45*/
                                meltfptr[44] = /*_.RETURN___V44*/ meltfptr[43];;
                                /*^compute*/
                                /*_._IF___V42*/
                                meltfptr[31] = /*_.PROGN___V45*/ meltfptr[44];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:7992:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V43*/
                                meltfptr[42] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V44*/
                                meltfptr[43] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V45*/
                                meltfptr[44] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V42*/ meltfptr[31] =  /*fromline 1341*/ NULL ;;
                        }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7991:/ quasiblock");


                    /*_.PROGN___V46*/
                    meltfptr[42] = /*_.SYMB__V26*/ meltfptr[25];;
                    /*^compute*/
                    /*_.EXPV__V41*/
                    meltfptr[37] = /*_.PROGN___V46*/ meltfptr[42];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7991:/ clear");
                    /*clear*/ /*_#IS_NOT_A__L13*/
                    meltfnum[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V42*/
                    meltfptr[31] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V46*/
                    meltfptr[42] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:7999:/ apply");
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
                        /*_.MEXPANDER__V47*/
                        meltfptr[43] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SEXPV__V35*/ meltfptr[33]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:7998:/ quasiblock");


                    /*_.PROGN___V48*/
                    meltfptr[44] = /*_.MEXPANDER__V47*/ meltfptr[43];;
                    /*^compute*/
                    /*_.EXPV__V41*/
                    meltfptr[37] = /*_.PROGN___V48*/ meltfptr[44];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:7991:/ clear");
                    /*clear*/ /*_.MEXPANDER__V47*/
                    meltfptr[43] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V48*/
                    meltfptr[44] = 0 ;
                }
                ;
            }
        ;
        /*_.PAIR_TAIL__V49*/
        meltfptr[31] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
        MELT_LOCATION("warmelt-macro.melt:8001:/ compute");
        /*_.CURPAIR__V24*/
        meltfptr[23] = /*_.SETQ___V50*/ meltfptr[42] = /*_.PAIR_TAIL__V49*/ meltfptr[31];;

        MELT_CHECK_SIGNAL();
        ;
        /*_.PAIR_HEAD__V51*/
        meltfptr[43] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
        /*^compute*/
        /*_#eqeq__L14*/
        meltfnum[8] =
            ((/*_.PAIR_HEAD__V51*/ meltfptr[43]) == ((/*!konst_8_DOC*/ meltfrout->tabval[8])));;
        MELT_LOCATION("warmelt-macro.melt:8002:/ cond");
        /*cond*/
        if (/*_#eqeq__L14*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.PAIR_TAIL__V53*/ meltfptr[52] =
                        (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
                    MELT_LOCATION("warmelt-macro.melt:8004:/ compute");
                    /*_.CURPAIR__V24*/
                    meltfptr[23] = /*_.SETQ___V54*/ meltfptr[53] = /*_.PAIR_TAIL__V53*/ meltfptr[52];;
                    /*_.PAIR_HEAD__V55*/
                    meltfptr[54] =
                        (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
                    MELT_LOCATION("warmelt-macro.melt:8005:/ compute");
                    /*_.DOC__V27*/
                    meltfptr[26] = /*_.SETQ___V56*/ meltfptr[55] = /*_.PAIR_HEAD__V55*/ meltfptr[54];;
                    MELT_LOCATION("warmelt-macro.melt:8003:/ quasiblock");


                    /*_.PROGN___V57*/
                    meltfptr[56] = /*_.SETQ___V56*/ meltfptr[55];;
                    /*^compute*/
                    /*_._IF___V52*/
                    meltfptr[44] = /*_.PROGN___V57*/ meltfptr[56];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:8002:/ clear");
                    /*clear*/ /*_.PAIR_TAIL__V53*/
                    meltfptr[52] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V54*/
                    meltfptr[53] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PAIR_HEAD__V55*/
                    meltfptr[54] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V56*/
                    meltfptr[55] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V57*/
                    meltfptr[56] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V52*/ meltfptr[44] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L15*/
        meltfnum[14] =
            ((/*_.EXPV__V41*/ meltfptr[37]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:8006:/ cond");
        /*cond*/
        if (/*_#NULL__L15*/ meltfnum[14]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:8007:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V22*/ meltfptr[21]), ( "(EXPORT_MACRO <sym> <expander> [:doc <docum>]) expecting expander"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:8008:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_EXPORT_MACRO*/ meltfrout->tabval[9])), (5), "CLASS_SOURCE_EXPORT_MACRO");
            /*_.INST__V60*/
            meltfptr[54] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[54])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[54]), (1), (/*_.LOC__V22*/ meltfptr[21]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXPMAC_MNAME", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[54])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[54]), (2), (/*_.SYMB__V26*/ meltfptr[25]), "SEXPMAC_MNAME");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXPMAC_MVAL", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[54])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[54]), (3), (/*_.EXPV__V41*/ meltfptr[37]), "SEXPMAC_MVAL");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXPMAC_DOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[54])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[54]), (4), (/*_.DOC__V27*/ meltfptr[26]), "SEXPMAC_DOC");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V60*/ meltfptr[54], "newly made instance");
        ;
        /*_.RES__V59*/
        meltfptr[53] = /*_.INST__V60*/ meltfptr[54];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:8015:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:8015:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:8015:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[16];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8015;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_export_macro result res";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V59*/ meltfptr[53];
                            /*_.MELT_DEBUG_FUN__V62*/
                            meltfptr[56] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V61*/
                        meltfptr[55] = /*_.MELT_DEBUG_FUN__V62*/ meltfptr[56];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:8015:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L17*/
                        meltfnum[16] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V62*/
                        meltfptr[56] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V61*/ meltfptr[55] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:8015:/ locexp");
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
            /*clear*/ /*_._IF___V61*/
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
        MELT_LOCATION("warmelt-macro.melt:8016:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V59*/ meltfptr[53];;

        {
            MELT_LOCATION("warmelt-macro.melt:8016:/ locexp");
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
        /*_.LET___V58*/
        meltfptr[52] = /*_.RETURN___V63*/ meltfptr[56];;

        MELT_LOCATION("warmelt-macro.melt:8008:/ clear");
        /*clear*/ /*_.RES__V59*/
        meltfptr[53] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V63*/
        meltfptr[56] = 0 ;
        /*_.LET___V30*/
        meltfptr[29] = /*_.LET___V58*/ meltfptr[52];;

        MELT_LOCATION("warmelt-macro.melt:7982:/ clear");
        /*clear*/ /*_.LET___V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SEXPV__V35*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V36*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CBIND__V40*/
        meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L12*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EXPV__V41*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V49*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V50*/
        meltfptr[42] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V51*/
        meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L14*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V52*/
        meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L15*/
        meltfnum[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V58*/
        meltfptr[52] = 0 ;
        /*_.LET___V20*/
        meltfptr[18] = /*_.LET___V30*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:7972:/ clear");
        /*clear*/ /*_.CONT__V21*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SYMB__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DOC__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L7*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V30*/
        meltfptr[29] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:7966:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V20*/ meltfptr[18];;

        {
            MELT_LOCATION("warmelt-macro.melt:7966:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
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
    melt_trace_end("MEXPAND_EXPORT_MACRO", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO fromline 1716 */

    /** start of frame for meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO// fromline 1531
        : public Melt_CallFrameWithValues<54>
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
        MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<54> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO), clos) {};
        MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO() //the constructor fromline 1606
            : Melt_CallFrameWithValues<54> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<54> (fil,lin, sizeof(MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO)) {};
        MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<54> (fil,lin, sizeof(MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO


    /** end of frame for meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO fromline 1661**/

    /* end of frame for routine meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO fromline 1720 */

    /* classy proc frame meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO */
    MeltFrame_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_EXPORT_PATMACRO", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:8027:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:8028:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:8028:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:8028:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8028;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_export_patmacro sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:8028:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:8028:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:8029:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:8029:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:8029:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8029;
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:8029:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:8029:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:8030:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:8030:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:8030:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8030;
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:8030:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:8030:/ clear");
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:8031:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:8031:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:8031:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check mexpander";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8031;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:8031:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:8031:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:8032:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:8032:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:8032:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8032;
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/
                            meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:8032:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/
                        meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/
            meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:8032:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:8033:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:8034:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V21*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:8035:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V22*/
            meltfptr[21] = slot;
        };
        ;
        /*_.LIST_FIRST__V23*/
        meltfptr[22] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V21*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V24*/
        meltfptr[23] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V23*/ meltfptr[22])));;
        /*^compute*/
        /*_.PAIR_HEAD__V25*/
        meltfptr[24] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:8037:/ apply");
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
            /*_.SYMB__V26*/
            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.PAIR_HEAD__V25*/ meltfptr[24]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.PATEXPV__V27*/
        meltfptr[26] = (/*nil*/NULL);;
        /*^compute*/
        /*_.MACEXPV__V28*/
        meltfptr[27] = (/*nil*/NULL);;
        /*^compute*/
        /*_.DOC__V29*/
        meltfptr[28] = (/*nil*/NULL);;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L7*/
        meltfnum[1] =
            !melt_is_instance_of((melt_ptr_t)(/*_.SYMB__V26*/ meltfptr[25]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:8042:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L7*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:8043:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V22*/ meltfptr[21]), ( "(EXPORT_PATMACRO <sym> <patexpander> <macexpander> [:doc <docum>])\
 expecting symbol"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V30*/
        meltfptr[29] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
        MELT_LOCATION("warmelt-macro.melt:8044:/ compute");
        /*_.CURPAIR__V24*/
        meltfptr[23] = /*_.SETQ___V31*/ meltfptr[30] = /*_.PAIR_TAIL__V30*/ meltfptr[29];;
        /*_.PAIR_HEAD__V32*/
        meltfptr[31] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:8045:/ apply");
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
            /*_.MEXPANDER__V33*/
            meltfptr[32] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.PAIR_HEAD__V32*/ meltfptr[31]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*^compute*/
        /*_.PATEXPV__V27*/
        meltfptr[26] = /*_.SETQ___V34*/ meltfptr[33] = /*_.MEXPANDER__V33*/ meltfptr[32];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L8*/
        meltfnum[0] =
            ((/*_.PATEXPV__V27*/ meltfptr[26]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:8046:/ cond");
        /*cond*/
        if (/*_#NULL__L8*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:8047:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V22*/ meltfptr[21]), ( "(EXPORT_PATMACRO <sym> <patexpander> <macexpander> [:doc <docum>])\
 expecting patexpander"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V35*/
        meltfptr[34] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
        MELT_LOCATION("warmelt-macro.melt:8048:/ compute");
        /*_.CURPAIR__V24*/
        meltfptr[23] = /*_.SETQ___V36*/ meltfptr[35] = /*_.PAIR_TAIL__V35*/ meltfptr[34];;
        /*_.PAIR_HEAD__V37*/
        meltfptr[36] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:8049:/ apply");
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
            /*_.MEXPANDER__V38*/
            meltfptr[37] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.PAIR_HEAD__V37*/ meltfptr[36]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*^compute*/
        /*_.MACEXPV__V28*/
        meltfptr[27] = /*_.SETQ___V39*/ meltfptr[38] = /*_.MEXPANDER__V38*/ meltfptr[37];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L9*/
        meltfnum[8] =
            ((/*_.MACEXPV__V28*/ meltfptr[27]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:8050:/ cond");
        /*cond*/
        if (/*_#NULL__L9*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:8051:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V22*/ meltfptr[21]), ( "(EXPORT_PATMACRO <sym> <patexpander> <macexpander> [:doc <docum>])\
 expecting macexpander"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V40*/
        meltfptr[39] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
        MELT_LOCATION("warmelt-macro.melt:8052:/ compute");
        /*_.CURPAIR__V24*/
        meltfptr[23] = /*_.SETQ___V41*/ meltfptr[40] = /*_.PAIR_TAIL__V40*/ meltfptr[39];;

        MELT_CHECK_SIGNAL();
        ;
        /*_.PAIR_HEAD__V42*/
        meltfptr[41] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
        /*^compute*/
        /*_#eqeq__L10*/
        meltfnum[9] =
            ((/*_.PAIR_HEAD__V42*/ meltfptr[41]) == ((/*!konst_5_DOC*/ meltfrout->tabval[5])));;
        MELT_LOCATION("warmelt-macro.melt:8053:/ cond");
        /*cond*/
        if (/*_#eqeq__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.PAIR_TAIL__V44*/ meltfptr[43] =
                        (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
                    MELT_LOCATION("warmelt-macro.melt:8055:/ compute");
                    /*_.CURPAIR__V24*/
                    meltfptr[23] = /*_.SETQ___V45*/ meltfptr[44] = /*_.PAIR_TAIL__V44*/ meltfptr[43];;
                    /*_.PAIR_HEAD__V46*/
                    meltfptr[45] =
                        (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V24*/ meltfptr[23])));;
                    MELT_LOCATION("warmelt-macro.melt:8056:/ compute");
                    /*_.DOC__V29*/
                    meltfptr[28] = /*_.SETQ___V47*/ meltfptr[46] = /*_.PAIR_HEAD__V46*/ meltfptr[45];;
                    MELT_LOCATION("warmelt-macro.melt:8054:/ quasiblock");


                    /*_.PROGN___V48*/
                    meltfptr[47] = /*_.SETQ___V47*/ meltfptr[46];;
                    /*^compute*/
                    /*_._IF___V43*/
                    meltfptr[42] = /*_.PROGN___V48*/ meltfptr[47];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:8053:/ clear");
                    /*clear*/ /*_.PAIR_TAIL__V44*/
                    meltfptr[43] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V45*/
                    meltfptr[44] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PAIR_HEAD__V46*/
                    meltfptr[45] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V47*/
                    meltfptr[46] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V48*/
                    meltfptr[47] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V43*/ meltfptr[42] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:8059:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_EXPORT_PATMACRO*/ meltfrout->tabval[6])), (6), "CLASS_SOURCE_EXPORT_PATMACRO");
            /*_.INST__V51*/
            meltfptr[45] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V51*/ meltfptr[45])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V51*/ meltfptr[45]), (1), (/*_.LOC__V22*/ meltfptr[21]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXPMAC_MNAME", melt_magic_discr((melt_ptr_t)(/*_.INST__V51*/ meltfptr[45])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V51*/ meltfptr[45]), (2), (/*_.SYMB__V26*/ meltfptr[25]), "SEXPMAC_MNAME");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXPPAT_PVAL", melt_magic_discr((melt_ptr_t)(/*_.INST__V51*/ meltfptr[45])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V51*/ meltfptr[45]), (5), (/*_.PATEXPV__V27*/ meltfptr[26]), "SEXPPAT_PVAL");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXPMAC_MVAL", melt_magic_discr((melt_ptr_t)(/*_.INST__V51*/ meltfptr[45])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V51*/ meltfptr[45]), (3), (/*_.MACEXPV__V28*/ meltfptr[27]), "SEXPMAC_MVAL");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXPMAC_DOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V51*/ meltfptr[45])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V51*/ meltfptr[45]), (4), (/*_.DOC__V29*/ meltfptr[28]), "SEXPMAC_DOC");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V51*/ meltfptr[45], "newly made instance");
        ;
        /*_.RES__V50*/
        meltfptr[44] = /*_.INST__V51*/ meltfptr[45];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:8067:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
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
            MELT_LOCATION("warmelt-macro.melt:8067:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:8067:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8067;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_export_patmacro result res";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V50*/ meltfptr[44];
                            /*_.MELT_DEBUG_FUN__V53*/
                            meltfptr[47] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V52*/
                        meltfptr[46] = /*_.MELT_DEBUG_FUN__V53*/ meltfptr[47];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:8067:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V53*/
                        meltfptr[47] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V52*/ meltfptr[46] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:8067:/ locexp");
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
            /*clear*/ /*_._IF___V52*/
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
        MELT_LOCATION("warmelt-macro.melt:8068:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V50*/ meltfptr[44];;

        {
            MELT_LOCATION("warmelt-macro.melt:8068:/ locexp");
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
        meltfptr[43] = /*_.RETURN___V54*/ meltfptr[47];;

        MELT_LOCATION("warmelt-macro.melt:8059:/ clear");
        /*clear*/ /*_.RES__V50*/
        meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V54*/
        meltfptr[47] = 0 ;
        /*_.LET___V20*/
        meltfptr[18] = /*_.LET___V49*/ meltfptr[43];;

        MELT_LOCATION("warmelt-macro.melt:8033:/ clear");
        /*clear*/ /*_.CONT__V21*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SYMB__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PATEXPV__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MACEXPV__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DOC__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L7*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V32*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MEXPANDER__V33*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V34*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L8*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V35*/
        meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V36*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V37*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MEXPANDER__V38*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V39*/
        meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V40*/
        meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V41*/
        meltfptr[40] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V42*/
        meltfptr[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V43*/
        meltfptr[42] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V49*/
        meltfptr[43] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:8027:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V20*/ meltfptr[18];;

        {
            MELT_LOCATION("warmelt-macro.melt:8027:/ locexp");
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
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
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
    melt_trace_end("MEXPAND_EXPORT_PATMACRO", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO*/



/**** end of warmelt-macro+06.cc ****/
