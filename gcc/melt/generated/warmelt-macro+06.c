/* GCC MELT GENERATED FILE warmelt-macro+06.c - DO NOT EDIT */
/* secondary MELT generated C file of rank #6 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f6[] =
  MELT_RUN_HASHMD5 /* from melt-run.h */ ;


/**** warmelt-macro+06.c declarations ****/
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
#define MELT_HAS_INITIAL_ENVIRONMENT 1	/*usualmodule */


/**** no MELT module variables ****/

/*** 5 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t
melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdata,
						 melt_ptr_t meltinp0_PREVENV,
						 const char
						 *meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_MACRO_EXPORTER (melt_ptr_t melthookdata,
					       melt_ptr_t meltinp0_SYM,
					       melt_ptr_t meltinp1_VAL,
					       melt_ptr_t meltinp2_CONTENV);

/*declare MELT called hook #2 HOOK_PATMACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_PATMACRO_EXPORTER (melt_ptr_t melthookdata,
						  melt_ptr_t meltinp0_SYM,
						  melt_ptr_t meltinp1_MACVAL,
						  melt_ptr_t meltinp2_PATVAL,
						  melt_ptr_t
						  meltinp3_CONTENV);

/*declare MELT called hook #3 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdata,
						      const char
						      *meltinp0_SYMNAMESTR,
						      const char
						      *meltinp1_MODULENAMESTR,
						      melt_ptr_t
						      meltinp2_PARENV);

/*declare MELT called hook #4 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdata,
					       melt_ptr_t meltinp0_SYM,
					       melt_ptr_t meltinp1_VAL,
					       melt_ptr_t meltinp2_CONTENV);

/*** end of 5 MELT called hook declarations ***/

/*** no extra MELT c-headers ***/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_1_WARMELTmiMACRO_S_EXPR_WEIGHT (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiMACRO (meltclosure_ptr_t
							     meltclosp_,
							     melt_ptr_t
							     meltfirstargp_,
							     const
							     melt_argdescr_cell_t
							     meltxargdescr_[],
							     union
							     meltparam_un
							     *meltxargtab_,
							     const
							     melt_argdescr_cell_t
							     meltxresdescr_[],
							     union
							     meltparam_un
							     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_3_WARMELTmiMACRO_EXPAND_RESTLIST_AS_LIST (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_4_WARMELTmiMACRO_EXPAND_RESTLIST_AS_TUPLE (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_5_WARMELTmiMACRO_EXPAND_PAIRLIST_AS_LIST (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_6_WARMELTmiMACRO_EXPAND_PAIRLIST_AS_TUPLE (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_7_WARMELTmiMACRO_REGISTER_GENERATOR_DEVICE (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_8_WARMELTmiMACRO_EXPAND_APPLY (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_9_WARMELTmiMACRO_EXPAND_MSEND (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_WARMELTmiMACRO_EXPAND_FIELDEXPR (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_WARMELTmiMACRO_EXPAND_CITERATION (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_12_WARMELTmiMACRO_EXPAND_CMATCHEXPR (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiMACRO_EXPAND_FUNMATCHEXPR (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_WARMELTmiMACRO_EXPAND_KEYWORDFUN (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_15_WARMELTmiMACRO_MACROEXPAND_1 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_16_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiMACRO_EXPAND_PRIMITIVE (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiMACRO_EXPAND_HOOK (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_19_WARMELTmiMACRO_PATTERNEXPAND_PAIRLIST_AS_TUPLE (meltclosure_ptr_t
							    meltclosp_,
							    melt_ptr_t
							    meltfirstargp_,
							    const
							    melt_argdescr_cell_t
							    meltxargdescr_[],
							    union meltparam_un
							    *meltxargtab_,
							    const
							    melt_argdescr_cell_t
							    meltxresdescr_[],
							    union meltparam_un
							    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_21_WARMELTmiMACRO_PATMACEXPAND_FOR_MATCHER (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_22_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_23_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiMACRO_PATTERN_WEIGHT_TUPLE (meltclosure_ptr_t meltclosp_,
						 melt_ptr_t meltfirstargp_,
						 const melt_argdescr_cell_t
						 meltxargdescr_[],
						 union meltparam_un
						 *meltxargtab_,
						 const melt_argdescr_cell_t
						 meltxresdescr_[],
						 union meltparam_un
						 *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_25_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_26_WARMELTmiMACRO_PATTERNEXPAND_EXPR (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_27_WARMELTmiMACRO_PATTERNEXPAND_1 (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_WARMELTmiMACRO_MACROEXPAND_TOPLEVEL_LIST (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un
						      *meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un
						      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_29_WARMELTmiMACRO_LAMBDA_ARG_BINDINGS (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_30_WARMELTmiMACRO_INSTALL_INITIAL_MACRO (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_31_WARMELTmiMACRO_INSTALL_INITIAL_PATMACRO (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_32_WARMELTmiMACRO_WARN_IF_REDEFINED (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_33_WARMELTmiMACRO_FLATTEN_FOR_C_CODE_EXPANSION (meltclosure_ptr_t
							 meltclosp_,
							 melt_ptr_t
							 meltfirstargp_,
							 const
							 melt_argdescr_cell_t
							 meltxargdescr_[],
							 union meltparam_un
							 *meltxargtab_,
							 const
							 melt_argdescr_cell_t
							 meltxresdescr_[],
							 union meltparam_un
							 *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_34_WARMELTmiMACRO_PARSE_PAIRLIST_C_CODE_EXPANSION (meltclosure_ptr_t
							    meltclosp_,
							    melt_ptr_t
							    meltfirstargp_,
							    const
							    melt_argdescr_cell_t
							    meltxargdescr_[],
							    union meltparam_un
							    *meltxargtab_,
							    const
							    melt_argdescr_cell_t
							    meltxresdescr_[],
							    union meltparam_un
							    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_35_WARMELTmiMACRO_CHECK_C_EXPANSION (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_36_WARMELTmiMACRO_MEXPAND_DEFPRIMITIVE (meltclosure_ptr_t meltclosp_,
						 melt_ptr_t meltfirstargp_,
						 const melt_argdescr_cell_t
						 meltxargdescr_[],
						 union meltparam_un
						 *meltxargtab_,
						 const melt_argdescr_cell_t
						 meltxresdescr_[],
						 union meltparam_un
						 *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_37_WARMELTmiMACRO_MEXPAND_DEFCITERATOR (meltclosure_ptr_t meltclosp_,
						 melt_ptr_t meltfirstargp_,
						 const melt_argdescr_cell_t
						 meltxargdescr_[],
						 union meltparam_un
						 *meltxargtab_,
						 const melt_argdescr_cell_t
						 meltxresdescr_[],
						 union meltparam_un
						 *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_38_WARMELTmiMACRO_MEXPAND_DEFCMATCHER (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_39_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_40_WARMELTmiMACRO_MEXPAND_DEFUNMATCHER (meltclosure_ptr_t meltclosp_,
						 melt_ptr_t meltfirstargp_,
						 const melt_argdescr_cell_t
						 meltxargdescr_[],
						 union meltparam_un
						 *meltxargtab_,
						 const melt_argdescr_cell_t
						 meltxresdescr_[],
						 union meltparam_un
						 *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_41_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_42_WARMELTmiMACRO_MEXPAND_DEFUN (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_43_WARMELTmiMACRO_MEXPAND_DEFVAR (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_44_WARMELTmiMACRO_MEXPAND_DEFHOOK (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_45_WARMELTmiMACRO_MEXPAND_DEFINE (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_46_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_47_WARMELTmiMACRO_SCAN_DEFCLASS (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_48_WARMELTmiMACRO_MEXPAND_DEFCLASS (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_49_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_50_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_51_WARMELTmiMACRO_PARSE_FIELD_ASSIGNMENT (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_52_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_53_WARMELTmiMACRO_MEXPAND_DEFINSTANCE (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_54_WARMELTmiMACRO_MEXPAND_DEFSELECTOR (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_55_WARMELTmiMACRO_MEXPAND_INSTANCE (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_56_WARMELTmiMACRO_MEXPAND_LOAD (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_57_WARMELTmiMACRO_PARSE_FIELD_PATTERN (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_58_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_59_WARMELTmiMACRO_PATEXPAND_INSTANCE (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_60_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_61_WARMELTmiMACRO_PATEXPAND_OBJECT (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_62_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_63_WARMELTmiMACRO_MEXPAND_OBJECT (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_64_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_65_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_66_WARMELTmiMACRO_MEXPAND_CODE_CHUNK (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_67_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_68_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un
						      *meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un
						      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_69_WARMELTmiMACRO_MEXPAND_PUT_FIELDS (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_70_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_71_WARMELTmiMACRO_MEXPAND_GET_FIELD (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_72_WARMELTmiMACRO_PAIRLIST_TO_PROGN (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_73_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_74_WARMELTmiMACRO_MEXPAND_SETQ (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_75_WARMELTmiMACRO_MEXPAND_IF (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiMACRO_MEXPAND_WHEN (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_77_WARMELTmiMACRO_MEXPAND_UNLESS (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_78_WARMELTmiMACRO_MEXPAND_CPPIF (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_79_WARMELTmiMACRO_FILTERGCCVERSION (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_80_WARMELTmiMACRO_MEXPAND_GCCIF (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_81_WARMELTmiMACRO_MEXPAND_COND (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_82_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_83_WARMELTmiMACRO_MEXPAND_AND (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_84_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_85_WARMELTmiMACRO_PATEXPAND_AS (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_86_WARMELTmiMACRO_MEXPAND_AS (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_87_WARMELTmiMACRO_PATEXPAND_WHEN (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_88_WARMELTmiMACRO_PATEXPAND_AND (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_89_WARMELTmiMACRO_MEXPAND_OR (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_90_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			    melt_ptr_t meltfirstargp_,
			    const melt_argdescr_cell_t meltxargdescr_[],
			    union meltparam_un *meltxargtab_,
			    const melt_argdescr_cell_t meltxresdescr_[],
			    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_91_WARMELTmiMACRO_PATEXPAND_OR (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_92_WARMELTmiMACRO_MEXPAND_REFERENCE (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_93_WARMELTmiMACRO_PATEXPAND_REFERENCE (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_94_WARMELTmiMACRO_MEXPANDOBSOLETE_CONTAINER (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un
						      *meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un
						      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_95_WARMELTmiMACRO_PATEXPANDOBSOLETE_CONTAINER (meltclosure_ptr_t
							meltclosp_,
							melt_ptr_t
							meltfirstargp_,
							const
							melt_argdescr_cell_t
							meltxargdescr_[],
							union meltparam_un
							*meltxargtab_,
							const
							melt_argdescr_cell_t
							meltxresdescr_[],
							union meltparam_un
							*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_96_WARMELTmiMACRO_MEXPAND_DEREF (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_97_WARMELTmiMACRO_MEXPANDOBSOLETE_CONTENT (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_98_WARMELTmiMACRO_MEXPAND_SET_REF (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_99_WARMELTmiMACRO_MEXPAND_PLUS (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_100_WARMELTmiMACRO_MEXPAND_MINUS (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_101_WARMELTmiMACRO_MEXPAND_TIMES (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_102_WARMELTmiMACRO_MEXPAND_DIV (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_103_WARMELTmiMACRO_MEXPAND_TUPLE (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_104_WARMELTmiMACRO_PATEXPAND_TUPLE (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_105_WARMELTmiMACRO_MEXPAND_LIST (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_106_WARMELTmiMACRO_PATEXPAND_LIST (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_107_WARMELTmiMACRO_MEXPAND_MATCH (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_108_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_109_WARMELTmiMACRO_MEXPAND_MATCHALT (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_110_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_111_WARMELTmiMACRO_MEXPAND_LETBINDING (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_112_WARMELTmiMACRO_MEXPAND_LET (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_113_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_114_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_115_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_116_WARMELTmiMACRO_YES_RECURSIVELY_CONSTRUCTIBLE (meltclosure_ptr_t
							   meltclosp_,
							   melt_ptr_t
							   meltfirstargp_,
							   const
							   melt_argdescr_cell_t
							   meltxargdescr_[],
							   union meltparam_un
							   *meltxargtab_,
							   const
							   melt_argdescr_cell_t
							   meltxresdescr_[],
							   union meltparam_un
							   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_117_WARMELTmiMACRO_MEXPAND_LETREC (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_118_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_119_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_120_WARMELTmiMACRO_MEXPAND_LAMBDA (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_121_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_122_WARMELTmiMACRO_MEXPAND_VARIADIC (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_123_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_124_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_125_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_126_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_127_WARMELTmiMACRO_MEXPAND_MULTICALL (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_128_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_129_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_130_WARMELTmiMACRO_MEXPAND_QUOTE (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_131_WARMELTmiMACRO_MEXPAND_BOX (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_132_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_133_WARMELTmiMACRO_MEXPAND_UNBOX (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_134_WARMELTmiMACRO_MEXPAND_COMMENT (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_135_WARMELTmiMACRO_MEXPAND_CHEADER (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_136_WARMELTmiMACRO_MEXPAND_CIMPLEMENT (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_137_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_138_WARMELTmiMACRO_PAIRLIST_TO_RETURN (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_139_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_140_WARMELTmiMACRO_MEXPAND_PROGN (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_141_WARMELTmiMACRO_MEXPAND_RETURN (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_142_WARMELTmiMACRO_MEXPAND_FOREVER (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_143_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_145_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_152_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_156_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un
						      *meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un
						      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un
						      *meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un
						      *meltxrestab_);



MELT_EXTERN void *melt_start_this_module (void *);
struct melt_initial_frame_st;

/*declare opaque initial frame: */
typedef struct melt_initial_frame_st meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiMACRO__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMACRO__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG */
MELT_EXTERN const char meltmodule_WARMELTmiMACRO__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMACRO__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG */



void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_0 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_1 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_2 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_3 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_4 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_5 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_6 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_7 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_8 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_9 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_10 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_11 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_12 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_13 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_14 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_15 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_16 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_17 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_18 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_19 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_20 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_21 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_22 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_23 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_24 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_25 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_26 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_27 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_28 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_29 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_30 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_31 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_32 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_33 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_34 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_35 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_36 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_37 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_38 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_39 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_40 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_41 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_42 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_43 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_44 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__initialmeltchunk_45 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMACRO__forward_or_mark_module_start_frame (struct
							     melt_callframe_st
							     *fp,
							     int marking);



/**** warmelt-macro+06.c implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_143_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un *meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un *meltxrestab_)
{
  long current_blocklevel_signals_meltrout_143_WARMELTmiMACRO_melt =
    melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_143_WARMELTmiMACRO_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 3
    melt_ptr_t mcfr_varptr[3];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_143_WARMELTmiMACRO is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_143_WARMELTmiMACRO_st *) meltfirstargp_;
      /* use arguments meltrout_143_WARMELTmiMACRO output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 3; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_143_WARMELTmiMACRO nbval 3*/
  meltfram__.mcfr_nbvar = 3 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("**", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:6903:/ getarg");
 /*_.E__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*~NEWENV */ meltfclos->tabval[1]);
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & ( /*~MEXPANDER */ meltfclos->tabval[0]);
      /*^apply.arg */
      argtab[2].meltbp_aptr =
	(melt_ptr_t *) & ( /*~MODCTX */ meltfclos->tabval[2]);
      /*_.MEXPANDER__V3*/ meltfptr[2] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~MEXPANDER */ meltfclos->tabval[0])),
		    (melt_ptr_t) ( /*_.E__V2*/ meltfptr[1]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*^quasiblock */


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.MEXPANDER__V3*/ meltfptr[2];;

    {
      MELT_LOCATION ("warmelt-macro.melt:6903:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.MEXPANDER__V3*/ meltfptr[2] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("**", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_143_WARMELTmiMACRO_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_143_WARMELTmiMACRO */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un * meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT_melt =
    melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 39
    melt_ptr_t mcfr_varptr[39];
#define MELTFRAM_NBVARNUM 7
    long mcfr_varnum[7];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT_st *)
	meltfirstargp_;
      /* use arguments meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 39; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT nbval 39*/
  meltfram__.mcfr_nbvar = 39 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_EXIT", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:6922:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:6923:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:6923:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:6923:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 6923;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_exit sexpr=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:6923:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:6923:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:6924:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:6924:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:6924:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (6924) ? (6924) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:6924:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:6925:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L4*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:6925:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:6925:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (6925) ? (6925) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:6925:/ clear");
	     /*clear *//*_#IS_OBJECT__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:6926:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_CLOSURE__L5*/ meltfnum[1] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) ==
	 MELTOBMAG_CLOSURE);;
      MELT_LOCATION ("warmelt-macro.melt:6926:/ cond");
      /*cond */ if ( /*_#IS_CLOSURE__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:6926:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mexpander"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (6926) ? (6926) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:6926:/ clear");
	     /*clear *//*_#IS_CLOSURE__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:6927:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V15*/ meltfptr[14] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:6928:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V16*/ meltfptr[15] = slot;
    };
    ;
 /*_.LIST_FIRST__V17*/ meltfptr[16] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V15*/ meltfptr[14])));;
    /*^compute */
 /*_.CURPAIR__V18*/ meltfptr[17] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.SLABNAM__V19*/ meltfptr[18] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V18*/ meltfptr[17])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6931:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.XLABNAM__V20*/ meltfptr[19] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.SLABNAM__V19*/ meltfptr[18]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6932:/ apply");
    /*apply */
    {
      /*_.NEWENV__V21*/ meltfptr[20] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!FRESH_ENV */ meltfrout->tabval[2])),
		    (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]), (""),
		    (union meltparam_un *) 0, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_NOT_A__L6*/ meltfnum[0] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.XLABNAM__V20*/ meltfptr[19]),
			    (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					   tabval[3])));;
    MELT_LOCATION ("warmelt-macro.melt:6934:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L6*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:6936:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			      ("missing label in EXIT"), (melt_ptr_t) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:6937:/ quasiblock");


	  /*_._RETVAL___V1*/ meltfptr[0] = ( /*nil */ NULL);;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:6937:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-macro.melt:6935:/ quasiblock");


	  /*_.PROGN___V24*/ meltfptr[23] = /*_.RETURN___V23*/ meltfptr[22];;
	  /*^compute */
	  /*_._IF___V22*/ meltfptr[21] = /*_.PROGN___V24*/ meltfptr[23];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:6934:/ clear");
	     /*clear *//*_.RETURN___V23*/ meltfptr[22] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V24*/ meltfptr[23] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V22*/ meltfptr[21] = NULL;;
      }
    ;
    /*^compute */
 /*_.PAIR_TAIL__V25*/ meltfptr[22] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V18*/ meltfptr[17])));;
    MELT_LOCATION ("warmelt-macro.melt:6938:/ compute");
    /*_.CURPAIR__V18*/ meltfptr[17] = /*_.SETQ___V26*/ meltfptr[23] =
      /*_.PAIR_TAIL__V25*/ meltfptr[22];;
    MELT_LOCATION ("warmelt-macro.melt:6939:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.XLABNAM__V20*/ meltfptr[19];
      /*_.LABIND__V28*/ meltfptr[27] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!FIND_ENV */ meltfrout->tabval[4])),
		    (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_NOT_A__L7*/ meltfnum[1] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.LABIND__V28*/ meltfptr[27]),
			    (melt_ptr_t) (( /*!CLASS_LABEL_BINDING */
					   meltfrout->tabval[5])));;
    MELT_LOCATION ("warmelt-macro.melt:6940:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L7*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-macro.melt:6943:/ getslot");
	  {
	    melt_ptr_t slot = NULL, obj = NULL;
	    obj = (melt_ptr_t) ( /*_.XLABNAM__V20*/ meltfptr[19]) /*=obj*/ ;
	    melt_object_get_field (slot, obj, 1, "NAMED_NAME");
    /*_.NAMED_NAME__V30*/ meltfptr[29] = slot;
	  };
	  ;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:6942:/ locexp");
	    melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			    ("bad label in EXIT"),
			    (melt_ptr_t) ( /*_.NAMED_NAME__V30*/
					  meltfptr[29]));
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:6944:/ quasiblock");


	  /*_._RETVAL___V1*/ meltfptr[0] = ( /*nil */ NULL);;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:6944:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-macro.melt:6941:/ quasiblock");


	  /*_.PROGN___V32*/ meltfptr[31] = /*_.RETURN___V31*/ meltfptr[30];;
	  /*^compute */
	  /*_._IF___V29*/ meltfptr[28] = /*_.PROGN___V32*/ meltfptr[31];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:6940:/ clear");
	     /*clear *//*_.NAMED_NAME__V30*/ meltfptr[29] = 0;
	  /*^clear */
	     /*clear *//*_.RETURN___V31*/ meltfptr[30] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V32*/ meltfptr[31] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V29*/ meltfptr[28] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-macro.melt:6945:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:6948:/ quasiblock");


    /*^newclosure */
		 /*newclosure *//*_.__V35*/ meltfptr[31] =
      (melt_ptr_t)
      meltgc_new_closure ((meltobject_ptr_t)
			  (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			  (meltroutine_ptr_t) (( /*!konst_8 */ meltfrout->
						tabval[8])), (3));
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V35*/ meltfptr[31])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 0 >= 0
		    && 0 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V35*/ meltfptr[31])));
    ((meltclosure_ptr_t) /*_.__V35*/ meltfptr[31])->tabval[0] =
      (melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]);
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V35*/ meltfptr[31])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 1 >= 0
		    && 1 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V35*/ meltfptr[31])));
    ((meltclosure_ptr_t) /*_.__V35*/ meltfptr[31])->tabval[1] =
      (melt_ptr_t) ( /*_.NEWENV__V21*/ meltfptr[20]);
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V35*/ meltfptr[31])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 2 >= 0
		    && 2 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V35*/ meltfptr[31])));
    ((meltclosure_ptr_t) /*_.__V35*/ meltfptr[31])->tabval[2] =
      (melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4]);
    ;
    /*_.LAMBDA___V34*/ meltfptr[30] = /*_.__V35*/ meltfptr[31];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6945:/ apply");
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!DISCR_MULTIPLE */ meltfrout->tabval[7]);
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.LAMBDA___V34*/ meltfptr[30];
      /*_.BODYTUP__V36*/ meltfptr[35] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!PAIRLIST_TO_MULTIPLE */ meltfrout->tabval[6])),
		    (melt_ptr_t) ( /*_.CURPAIR__V18*/ meltfptr[17]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6950:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_EXIT */
					     meltfrout->tabval[9])), (4),
			      "CLASS_SOURCE_EXIT");
  /*_.INST__V38*/ meltfptr[37] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V38*/ meltfptr[37])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V38*/ meltfptr[37]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SLABEL_BIND",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V38*/ meltfptr[37])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V38*/ meltfptr[37]), (2),
			  ( /*_.LABIND__V28*/ meltfptr[27]), "SLABEL_BIND");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXI_BODY",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V38*/ meltfptr[37])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V38*/ meltfptr[37]), (3),
			  ( /*_.BODYTUP__V36*/ meltfptr[35]), "SEXI_BODY");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V38*/ meltfptr[37],
				  "newly made instance");
    ;
    /*_.EXR__V37*/ meltfptr[36] = /*_.INST__V38*/ meltfptr[37];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6955:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.EXR__V37*/ meltfptr[36];;

    {
      MELT_LOCATION ("warmelt-macro.melt:6955:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V33*/ meltfptr[29] = /*_.RETURN___V39*/ meltfptr[38];;

    MELT_LOCATION ("warmelt-macro.melt:6945:/ clear");
	   /*clear *//*_.LAMBDA___V34*/ meltfptr[30] = 0;
    /*^clear */
	   /*clear *//*_.BODYTUP__V36*/ meltfptr[35] = 0;
    /*^clear */
	   /*clear *//*_.EXR__V37*/ meltfptr[36] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V39*/ meltfptr[38] = 0;
    /*_.LET___V27*/ meltfptr[26] = /*_.LET___V33*/ meltfptr[29];;

    MELT_LOCATION ("warmelt-macro.melt:6939:/ clear");
	   /*clear *//*_.LABIND__V28*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L7*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V29*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_.LET___V33*/ meltfptr[29] = 0;
    /*_.LET___V14*/ meltfptr[12] = /*_.LET___V27*/ meltfptr[26];;

    MELT_LOCATION ("warmelt-macro.melt:6927:/ clear");
	   /*clear *//*_.CONT__V15*/ meltfptr[14] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V16*/ meltfptr[15] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.SLABNAM__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.XLABNAM__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.NEWENV__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L6*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V25*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V26*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.LET___V27*/ meltfptr[26] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6922:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V14*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-macro.melt:6922:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V14*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_EXIT", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_144_WARMELTmiMACRO_MEXPAND_EXIT */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_145_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un * meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_145_WARMELTmiMACRO_melt =
    melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_145_WARMELTmiMACRO_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 3
    melt_ptr_t mcfr_varptr[3];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_145_WARMELTmiMACRO is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_145_WARMELTmiMACRO_st *) meltfirstargp_;
      /* use arguments meltrout_145_WARMELTmiMACRO output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 3; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_145_WARMELTmiMACRO nbval 3*/
  meltfram__.mcfr_nbvar = 3 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("**", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:6948:/ getarg");
 /*_.E__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*~NEWENV */ meltfclos->tabval[1]);
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & ( /*~MEXPANDER */ meltfclos->tabval[0]);
      /*^apply.arg */
      argtab[2].meltbp_aptr =
	(melt_ptr_t *) & ( /*~MODCTX */ meltfclos->tabval[2]);
      /*_.MEXPANDER__V3*/ meltfptr[2] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~MEXPANDER */ meltfclos->tabval[0])),
		    (melt_ptr_t) ( /*_.E__V2*/ meltfptr[1]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*^quasiblock */


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.MEXPANDER__V3*/ meltfptr[2];;

    {
      MELT_LOCATION ("warmelt-macro.melt:6948:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.MEXPANDER__V3*/ meltfptr[2] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("**", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_145_WARMELTmiMACRO_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_145_WARMELTmiMACRO */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un * meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 40
    melt_ptr_t mcfr_varptr[40];
#define MELTFRAM_NBVARNUM 9
    long mcfr_varnum[9];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN_st *)
	meltfirstargp_;
      /* use arguments meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 40; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN nbval 40*/
  meltfram__.mcfr_nbvar = 40 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_AGAIN", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:6965:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:6966:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:6966:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:6966:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 6966;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_again sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:6966:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:6966:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:6967:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:6967:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:6967:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (6967) ? (6967) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:6967:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:6968:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_CLOSURE__L4*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) ==
	 MELTOBMAG_CLOSURE);;
      MELT_LOCATION ("warmelt-macro.melt:6968:/ cond");
      /*cond */ if ( /*_#IS_CLOSURE__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:6968:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mexpander"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (6968) ? (6968) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:6968:/ clear");
	     /*clear *//*_#IS_CLOSURE__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:6969:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L5*/ meltfnum[1] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:6969:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:6969:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (6969) ? (6969) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:6969:/ clear");
	     /*clear *//*_#IS_OBJECT__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:6970:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V15*/ meltfptr[14] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:6971:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V16*/ meltfptr[15] = slot;
    };
    ;
 /*_.LIST_FIRST__V17*/ meltfptr[16] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V15*/ meltfptr[14])));;
    /*^compute */
 /*_.CURPAIR__V18*/ meltfptr[17] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.SLABNAM__V19*/ meltfptr[18] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V18*/ meltfptr[17])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6974:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.XLABNAM__V20*/ meltfptr[19] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.SLABNAM__V19*/ meltfptr[18]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
 /*_.PAIR_TAIL__V21*/ meltfptr[20] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V18*/ meltfptr[17])));;
    MELT_LOCATION ("warmelt-macro.melt:6976:/ compute");
    /*_.CURPAIR__V18*/ meltfptr[17] = /*_.SETQ___V22*/ meltfptr[21] =
      /*_.PAIR_TAIL__V21*/ meltfptr[20];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_NOT_A__L6*/ meltfnum[0] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.XLABNAM__V20*/ meltfptr[19]),
			    (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					   tabval[2])));;
    MELT_LOCATION ("warmelt-macro.melt:6977:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L6*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:6979:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			      ("bad label in (AGAIN <label>)"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:6980:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:6980:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-macro.melt:6978:/ quasiblock");


	  /*_.PROGN___V25*/ meltfptr[24] = /*_.RETURN___V24*/ meltfptr[23];;
	  /*^compute */
	  /*_._IF___V23*/ meltfptr[22] = /*_.PROGN___V25*/ meltfptr[24];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:6977:/ clear");
	     /*clear *//*_.RETURN___V24*/ meltfptr[23] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V25*/ meltfptr[24] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V23*/ meltfptr[22] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6981:/ cond");
    /*cond */ if ( /*_.CURPAIR__V18*/ meltfptr[17])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:6983:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			      ("extra operands to (AGAIN <label>)"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:6984:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:6984:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-macro.melt:6982:/ quasiblock");


	  /*_.PROGN___V28*/ meltfptr[27] = /*_.RETURN___V27*/ meltfptr[24];;
	  /*^compute */
	  /*_._IF___V26*/ meltfptr[23] = /*_.PROGN___V28*/ meltfptr[27];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:6981:/ clear");
	     /*clear *//*_.RETURN___V27*/ meltfptr[24] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V28*/ meltfptr[27] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V26*/ meltfptr[23] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-macro.melt:6985:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.XLABNAM__V20*/ meltfptr[19];
      /*_.LABIND__V30*/ meltfptr[27] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!FIND_ENV */ meltfrout->tabval[3])),
		    (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_NOT_A__L7*/ meltfnum[1] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.LABIND__V30*/ meltfptr[27]),
			    (melt_ptr_t) (( /*!CLASS_LABEL_BINDING */
					   meltfrout->tabval[4])));;
    MELT_LOCATION ("warmelt-macro.melt:6986:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L7*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-macro.melt:6989:/ getslot");
	  {
	    melt_ptr_t slot = NULL, obj = NULL;
	    obj = (melt_ptr_t) ( /*_.XLABNAM__V20*/ meltfptr[19]) /*=obj*/ ;
	    melt_object_get_field (slot, obj, 1, "NAMED_NAME");
    /*_.NAMED_NAME__V32*/ meltfptr[31] = slot;
	  };
	  ;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:6988:/ locexp");
	    melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			    ("bad label in AGAIN"),
			    (melt_ptr_t) ( /*_.NAMED_NAME__V32*/
					  meltfptr[31]));
	  }
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:6990:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:6990:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-macro.melt:6987:/ quasiblock");


	  /*_.PROGN___V34*/ meltfptr[33] = /*_.RETURN___V33*/ meltfptr[32];;
	  /*^compute */
	  /*_._IF___V31*/ meltfptr[30] = /*_.PROGN___V34*/ meltfptr[33];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:6986:/ clear");
	     /*clear *//*_.NAMED_NAME__V32*/ meltfptr[31] = 0;
	  /*^clear */
	     /*clear *//*_.RETURN___V33*/ meltfptr[32] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V34*/ meltfptr[33] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V31*/ meltfptr[30] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-macro.melt:6991:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^quasiblock */


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_AGAIN */
					     meltfrout->tabval[5])), (3),
			      "CLASS_SOURCE_AGAIN");
  /*_.INST__V37*/ meltfptr[33] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V37*/ meltfptr[33])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V37*/ meltfptr[33]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SLABEL_BIND",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V37*/ meltfptr[33])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V37*/ meltfptr[33]), (2),
			  ( /*_.LABIND__V30*/ meltfptr[27]), "SLABEL_BIND");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V37*/ meltfptr[33],
				  "newly made instance");
    ;
    /*_.MAGAIN__V36*/ meltfptr[32] = /*_.INST__V37*/ meltfptr[33];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:6996:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L8*/ meltfnum[7] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:6996:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L8*/ meltfnum[7])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:6996:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 6996;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_again result";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MAGAIN__V36*/ meltfptr[32];
	      /*_.MELT_DEBUG_FUN__V39*/ meltfptr[38] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V38*/ meltfptr[37] =
	      /*_.MELT_DEBUG_FUN__V39*/ meltfptr[38];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:6996:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L9*/ meltfnum[8] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V39*/ meltfptr[38] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V38*/ meltfptr[37] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:6996:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L8*/ meltfnum[7] = 0;
      /*^clear */
	     /*clear *//*_._IF___V38*/ meltfptr[37] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6997:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.MAGAIN__V36*/ meltfptr[32];;

    {
      MELT_LOCATION ("warmelt-macro.melt:6997:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V35*/ meltfptr[31] = /*_.RETURN___V40*/ meltfptr[38];;

    MELT_LOCATION ("warmelt-macro.melt:6991:/ clear");
	   /*clear *//*_.MAGAIN__V36*/ meltfptr[32] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V40*/ meltfptr[38] = 0;
    /*_.LET___V29*/ meltfptr[24] = /*_.LET___V35*/ meltfptr[31];;

    MELT_LOCATION ("warmelt-macro.melt:6985:/ clear");
	   /*clear *//*_.LABIND__V30*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L7*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V31*/ meltfptr[30] = 0;
    /*^clear */
	   /*clear *//*_.LET___V35*/ meltfptr[31] = 0;
    /*_.LET___V14*/ meltfptr[12] = /*_.LET___V29*/ meltfptr[24];;

    MELT_LOCATION ("warmelt-macro.melt:6970:/ clear");
	   /*clear *//*_.CONT__V15*/ meltfptr[14] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V16*/ meltfptr[15] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.SLABNAM__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.XLABNAM__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L6*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_._IF___V26*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.LET___V29*/ meltfptr[24] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:6965:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V14*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-macro.melt:6965:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V14*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_AGAIN", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_146_WARMELTmiMACRO_MEXPAND_AGAIN */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un *
						     meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un *
						     meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 36
    melt_ptr_t mcfr_varptr[36];
#define MELTFRAM_NBVARNUM 10
    long mcfr_varnum[10];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING_st *)
	meltfirstargp_;
      /* use arguments meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 36; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING nbval 36*/
  meltfram__.mcfr_nbvar = 36 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_COMPILE_WARNING", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7009:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7010:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7010:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7010:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7010;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_compile_warning sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7010:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7010:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7011:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7011:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7011:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7011) ? (7011) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7011:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7012:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L4*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7012:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7012:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7012) ? (7012) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7012:/ clear");
	     /*clear *//*_#IS_OBJECT__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NULL__L5*/ meltfnum[1] =
      (( /*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7013:/ cond");
    /*cond */ if ( /*_#NULL__L5*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^compute */
	  /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[12] =
	    ( /*!MACROEXPAND_1 */ meltfrout->tabval[2]);;
	  /*_._IF___V12*/ meltfptr[10] = /*_.SETQ___V13*/ meltfptr[12];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7013:/ clear");
	     /*clear *//*_.SETQ___V13*/ meltfptr[12] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V12*/ meltfptr[10] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-macro.melt:7014:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V15*/ meltfptr[14] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7015:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V16*/ meltfptr[15] = slot;
    };
    ;
 /*_.LIST_FIRST__V17*/ meltfptr[16] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V15*/ meltfptr[14])));;
    /*^compute */
 /*_.CURPAIR__V18*/ meltfptr[17] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.SMSG__V19*/ meltfptr[18] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V18*/ meltfptr[17])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7018:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.XMSG__V20*/ meltfptr[19] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.SMSG__V19*/ meltfptr[18]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_STRING__L6*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.XMSG__V20*/ meltfptr[19])) ==
       MELTOBMAG_STRING);;
    /*^compute */
 /*_#NOT__L7*/ meltfnum[6] =
      (!( /*_#IS_STRING__L6*/ meltfnum[0]));;
    MELT_LOCATION ("warmelt-macro.melt:7020:/ cond");
    /*cond */ if ( /*_#NOT__L7*/ meltfnum[6])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7022:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			      ("missing message string in (COMPILE_WARNING <msg> <exp>)"),
			      (melt_ptr_t) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7023:/ quasiblock");


	  /*_._RETVAL___V1*/ meltfptr[0] = ( /*nil */ NULL);;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:7023:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-macro.melt:7021:/ quasiblock");


	  /*_.PROGN___V23*/ meltfptr[22] = /*_.RETURN___V22*/ meltfptr[21];;
	  /*^compute */
	  /*_._IF___V21*/ meltfptr[20] = /*_.PROGN___V23*/ meltfptr[22];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7020:/ clear");
	     /*clear *//*_.RETURN___V22*/ meltfptr[21] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V23*/ meltfptr[22] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V21*/ meltfptr[20] = NULL;;
      }
    ;
    /*^compute */
 /*_.PAIR_TAIL__V24*/ meltfptr[21] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V18*/ meltfptr[17])));;
    MELT_LOCATION ("warmelt-macro.melt:7024:/ compute");
    /*_.CURPAIR__V18*/ meltfptr[17] = /*_.SETQ___V25*/ meltfptr[22] =
      /*_.PAIR_TAIL__V24*/ meltfptr[21];;
    MELT_LOCATION ("warmelt-macro.melt:7025:/ quasiblock");


 /*_.SEXP__V27*/ meltfptr[26] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V18*/ meltfptr[17])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7026:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.XEXP__V28*/ meltfptr[27] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.SEXP__V27*/ meltfptr[26]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
 /*_.PAIR_TAIL__V29*/ meltfptr[28] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V18*/ meltfptr[17])));;
    MELT_LOCATION ("warmelt-macro.melt:7028:/ compute");
    /*_.CURPAIR__V18*/ meltfptr[17] = /*_.SETQ___V30*/ meltfptr[29] =
      /*_.PAIR_TAIL__V29*/ meltfptr[28];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#NOTNULL__L8*/ meltfnum[7] =
      (( /*_.CURPAIR__V18*/ meltfptr[17]) != NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7029:/ cond");
    /*cond */ if ( /*_#NOTNULL__L8*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7030:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			      ("too many arguments in (COMPILE_WARNING <msg> [<exp>])"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7031:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^quasiblock */


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_COMPILEWARNING */ meltfrout->tabval[3])), (4), "CLASS_SOURCE_COMPILEWARNING");
  /*_.INST__V33*/ meltfptr[32] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V33*/ meltfptr[32])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V33*/ meltfptr[32]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SCWARN_MSG",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V33*/ meltfptr[32])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V33*/ meltfptr[32]), (2),
			  ( /*_.XMSG__V20*/ meltfptr[19]), "SCWARN_MSG");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SCWARN_EXPR",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V33*/ meltfptr[32])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V33*/ meltfptr[32]), (3),
			  ( /*_.XEXP__V28*/ meltfptr[27]), "SCWARN_EXPR");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V33*/ meltfptr[32],
				  "newly made instance");
    ;
    /*_.RES__V32*/ meltfptr[31] = /*_.INST__V33*/ meltfptr[32];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7037:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L9*/ meltfnum[8] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7037:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L9*/ meltfnum[8])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7037:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7037;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_compile_warning result";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V32*/ meltfptr[31];
	      /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V34*/ meltfptr[33] =
	      /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7037:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L10*/ meltfnum[9] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V35*/ meltfptr[34] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V34*/ meltfptr[33] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7037:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L9*/ meltfnum[8] = 0;
      /*^clear */
	     /*clear *//*_._IF___V34*/ meltfptr[33] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7038:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V32*/ meltfptr[31];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7038:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V31*/ meltfptr[30] = /*_.RETURN___V36*/ meltfptr[34];;

    MELT_LOCATION ("warmelt-macro.melt:7031:/ clear");
	   /*clear *//*_.RES__V32*/ meltfptr[31] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V36*/ meltfptr[34] = 0;
    /*_.LET___V26*/ meltfptr[25] = /*_.LET___V31*/ meltfptr[30];;

    MELT_LOCATION ("warmelt-macro.melt:7025:/ clear");
	   /*clear *//*_.SEXP__V27*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.XEXP__V28*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V29*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V30*/ meltfptr[29] = 0;
    /*^clear */
	   /*clear *//*_#NOTNULL__L8*/ meltfnum[7] = 0;
    /*^clear */
	   /*clear *//*_.LET___V31*/ meltfptr[30] = 0;
    /*_.LET___V14*/ meltfptr[12] = /*_.LET___V26*/ meltfptr[25];;

    MELT_LOCATION ("warmelt-macro.melt:7014:/ clear");
	   /*clear *//*_.CONT__V15*/ meltfptr[14] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V16*/ meltfptr[15] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.SMSG__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.XMSG__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_#IS_STRING__L6*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L7*/ meltfnum[6] = 0;
    /*^clear */
	   /*clear *//*_._IF___V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V24*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V25*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.LET___V26*/ meltfptr[25] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7009:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V14*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7009:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L5*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V14*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_COMPILE_WARNING", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_147_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un *
						meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un *
						meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 69
    melt_ptr_t mcfr_varptr[69];
#define MELTFRAM_NBVARNUM 23
    long mcfr_varnum[23];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG_st *)
	meltfirstargp_;
      /* use arguments meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 69; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG nbval 69*/
  meltfram__.mcfr_nbvar = 69 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_ASSERT_MSG", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7052:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7053:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7053:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7053:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7053;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_assert_msg sexpr=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7053:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7053:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7054:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7054:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7054:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7054) ? (7054) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7054:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7055:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7055:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7055:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7055) ? (7055) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7055:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NULL__L5*/ meltfnum[1] =
      (( /*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7056:/ cond");
    /*cond */ if ( /*_#NULL__L5*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^compute */
	  /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[12] =
	    ( /*!MACROEXPAND_1 */ meltfrout->tabval[3]);;
	  /*_._IF___V12*/ meltfptr[10] = /*_.SETQ___V13*/ meltfptr[12];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7056:/ clear");
	     /*clear *//*_.SETQ___V13*/ meltfptr[12] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V12*/ meltfptr[10] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7057:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L6*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7057:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7057:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7057) ? (7057) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7057:/ clear");
	     /*clear *//*_#IS_OBJECT__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7058:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7059:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7060:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;
    /*^compute */
 /*_.PAIR_HEAD__V21*/ meltfptr[20] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7062:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.XMSG__V22*/ meltfptr[21] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V21*/ meltfptr[20]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*_.XTEST__V23*/ meltfptr[22] = ( /*nil */ NULL);;
    /*^compute */
    /*_.ASSFAIL_SYMB__V24*/ meltfptr[23] =
      ( /*!konst_4_ASSERT_FAILED */ meltfrout->tabval[4]);;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7065:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.ASSFAIL_SYMB__V24*/ meltfptr[23];
      /*_.ASSFAIL_BINDING__V25*/ meltfptr[24] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!FIND_ENV */ meltfrout->tabval[5])),
		    (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_STRING__L7*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.XMSG__V22*/ meltfptr[21])) ==
       MELTOBMAG_STRING);;
    /*^compute */
 /*_#NOT__L8*/ meltfnum[7] =
      (!( /*_#IS_STRING__L7*/ meltfnum[0]));;
    MELT_LOCATION ("warmelt-macro.melt:7067:/ cond");
    /*cond */ if ( /*_#NOT__L8*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7068:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("non string message in (ASSERT_MSG <msg> <test>)"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_TAIL__V26*/ meltfptr[25] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7069:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V27*/ meltfptr[26] =
      /*_.PAIR_TAIL__V26*/ meltfptr[25];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_PAIR__L9*/ meltfnum[8] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])) ==
       MELTOBMAG_PAIR);;
    MELT_LOCATION ("warmelt-macro.melt:7070:/ cond");
    /*cond */ if ( /*_#IS_PAIR__L9*/ meltfnum[8])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_.PAIR_TAIL__V29*/ meltfptr[28] =
	    (melt_pair_tail
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
	  MELT_LOCATION ("warmelt-macro.melt:7071:/ cond");
	  /*cond */ if ( /*_.PAIR_TAIL__V29*/ meltfptr[28])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-macro.melt:7072:/ locexp");
		  /* error_plain */
		    melt_error_str ((melt_ptr_t)
				    ( /*_.LOC__V18*/ meltfptr[17]),
				    ("extra arg for (ASSERT_MSG <msg> <test>)"),
				    (melt_ptr_t) 0);
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
   /*_.PAIR_HEAD__V30*/ meltfptr[29] =
	    (melt_pair_head
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7073:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[3];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
	    /*^apply.arg */
	    argtab[1].meltbp_aptr =
	      (melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
	    /*^apply.arg */
	    argtab[2].meltbp_aptr =
	      (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
	    /*_.MEXPANDER__V31*/ meltfptr[30] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.MEXPANDER__V4*/ meltfptr[3]),
			  (melt_ptr_t) ( /*_.PAIR_HEAD__V30*/ meltfptr[29]),
			  (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR
			   ""), argtab, "", (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.XTEST__V23*/ meltfptr[22] = /*_.SETQ___V32*/ meltfptr[31] =
	    /*_.MEXPANDER__V31*/ meltfptr[30];;
	  MELT_LOCATION ("warmelt-macro.melt:7070:/ quasiblock");


	  /*_.PROGN___V33*/ meltfptr[32] = /*_.SETQ___V32*/ meltfptr[31];;
	  /*^compute */
	  /*_._IFELSE___V28*/ meltfptr[27] = /*_.PROGN___V33*/ meltfptr[32];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7070:/ clear");
	     /*clear *//*_.PAIR_TAIL__V29*/ meltfptr[28] = 0;
	  /*^clear */
	     /*clear *//*_.PAIR_HEAD__V30*/ meltfptr[29] = 0;
	  /*^clear */
	     /*clear *//*_.MEXPANDER__V31*/ meltfptr[30] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V32*/ meltfptr[31] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V33*/ meltfptr[32] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-macro.melt:7075:/ compute");
	  /*_.XTEST__V23*/ meltfptr[22] = /*_.SETQ___V34*/ meltfptr[28] =
	    ( /*nil */ NULL);;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:7076:/ locexp");
	    melt_warning_str (0, (melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("empty ASSERT_MSG"),
			      (melt_ptr_t) ( /*_.XMSG__V22*/ meltfptr[21]));
	  }
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7074:/ quasiblock");


	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7070:/ clear");
	     /*clear *//*_.SETQ___V34*/ meltfptr[28] = 0;
	}
	;
      }
    ;
    MELT_LOCATION ("warmelt-macro.melt:7078:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
 /*_#NULL__L10*/ meltfnum[9] =
      (( /*_.ASSFAIL_BINDING__V25*/ meltfptr[24]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7081:/ cond");
    /*cond */ if ( /*_#NULL__L10*/ meltfnum[9])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7082:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("unbound ASSERT_FAILED in (ASSERT_MSG <msg> <test>)"),
			      (melt_ptr_t) 0);
	  }
	  ;

#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-macro.melt:7083:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    {
	      /*^locexp */
	      /*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	      melt_dbgcounter++;
#endif
	      ;
	    }
	    ;

	    MELT_CHECK_SIGNAL ();
	    ;
     /*_#MELT_NEED_DBG__L11*/ meltfnum[10] =
	      /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	      ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-macro.melt:7083:/ cond");
	    /*cond */ if ( /*_#MELT_NEED_DBG__L11*/ meltfnum[10])	/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

       /*_#MELT_CALLCOUNT__L12*/ meltfnum[11] =
		    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		    meltcallcount	/* melt_callcount debugging */
#else
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;
		  /*^compute */
       /*_.DISCRIM__V38*/ meltfptr[32] =
		    ((melt_ptr_t)
		     (melt_discr
		      ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]))));;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-macro.melt:7085:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[1];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_aptr =
		      (melt_ptr_t *) & /*_.ASSFAIL_SYMB__V24*/ meltfptr[23];
		    /*_.FIND_ENV_DEBUG__V39*/ meltfptr[28] =
		      melt_apply ((meltclosure_ptr_t)
				  (( /*!FIND_ENV_DEBUG */ meltfrout->
				    tabval[6])),
				  (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
				  (MELTBPARSTR_PTR ""), argtab, "",
				  (union meltparam_un *) 0);
		  }
		  ;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-macro.melt:7083:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[11];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-macro.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 7083;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "mexpand_assert_msg  without assert_failed env=";
		    /*^apply.arg */
		    argtab[4].meltbp_aptr =
		      (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
		    /*^apply.arg */
		    argtab[5].meltbp_cstring = " env\'s class";
		    /*^apply.arg */
		    argtab[6].meltbp_aptr =
		      (melt_ptr_t *) & /*_.DISCRIM__V38*/ meltfptr[32];
		    /*^apply.arg */
		    argtab[7].meltbp_cstring = "find_env giving:";
		    /*^apply.arg */
		    argtab[8].meltbp_aptr =
		      (melt_ptr_t *) & /*_.FIND_ENV_DEBUG__V39*/ meltfptr[28];
		    /*^apply.arg */
		    argtab[9].meltbp_cstring = "assfail_symb=";
		    /*^apply.arg */
		    argtab[10].meltbp_aptr =
		      (melt_ptr_t *) & /*_.ASSFAIL_SYMB__V24*/ meltfptr[23];
		    /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39] =
		      melt_apply ((meltclosure_ptr_t)
				  (( /*!MELT_DEBUG_FUN */ meltfrout->
				    tabval[0])),
				  (melt_ptr_t) (( /*nil */ NULL)),
				  (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR ""), argtab, "",
				  (union meltparam_un *) 0);
		  }
		  ;
		  /*_._IF___V37*/ meltfptr[31] =
		    /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-macro.melt:7083:/ clear");
		 /*clear *//*_#MELT_CALLCOUNT__L12*/ meltfnum[11] = 0;
		  /*^clear */
		 /*clear *//*_.DISCRIM__V38*/ meltfptr[32] = 0;
		  /*^clear */
		 /*clear *//*_.FIND_ENV_DEBUG__V39*/ meltfptr[28] = 0;
		  /*^clear */
		 /*clear *//*_.MELT_DEBUG_FUN__V40*/ meltfptr[39] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

      /*_._IF___V37*/ meltfptr[31] = NULL;;
	      }
	    ;

	    {
	      MELT_LOCATION ("warmelt-macro.melt:7083:/ locexp");
	      /*void */ (void) 0;
	    }
	    ;
	    /*^quasiblock */


	    /*epilog */

	    /*^clear */
	       /*clear *//*_#MELT_NEED_DBG__L11*/ meltfnum[10] = 0;
	    /*^clear */
	       /*clear *//*_._IF___V37*/ meltfptr[31] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*^block */
	  /*anyblock */
	  {


	    {
	      /*^locexp */
	      /*void */ (void) 0;
	    }
	    ;
	    /*epilog */
	  }

#endif /*MELT_HAVE_DEBUG */
	  ;

#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-macro.melt:7087:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    MELT_CHECK_SIGNAL ();
	    ;
	    /*^cond */
	    /*cond */ if (( /*nil */ NULL))	/*then */
	      {
		/*^cond.then */
		/*_._IFELSE___V42*/ meltfptr[28] = ( /*nil */ NULL);;
	      }
	    else
	      {
		MELT_LOCATION ("warmelt-macro.melt:7087:/ cond.else");

		/*^block */
		/*anyblock */
		{




		  {
		    /*^locexp */
		    melt_assert_failed (("assfail_binding nul! @@"),
					("warmelt-macro.melt")
					? ("warmelt-macro.melt") : __FILE__,
					(7087) ? (7087) : __LINE__,
					__FUNCTION__);
		    ;
		  }
		  ;
		 /*clear *//*_._IFELSE___V42*/ meltfptr[28] = 0;
		  /*epilog */
		}
		;
	      }
	    ;
	    /*^compute */
	    /*_.IFCPP___V41*/ meltfptr[32] =
	      /*_._IFELSE___V42*/ meltfptr[28];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7087:/ clear");
	       /*clear *//*_._IFELSE___V42*/ meltfptr[28] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V41*/ meltfptr[32] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7088:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:7088:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-macro.melt:7081:/ quasiblock");


	  /*_.PROGN___V44*/ meltfptr[31] = /*_.RETURN___V43*/ meltfptr[39];;
	  /*^compute */
	  /*_.AFPRIM__V36*/ meltfptr[30] = /*_.PROGN___V44*/ meltfptr[31];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7081:/ clear");
	     /*clear *//*_.IFCPP___V41*/ meltfptr[32] = 0;
	  /*^clear */
	     /*clear *//*_.RETURN___V43*/ meltfptr[39] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V44*/ meltfptr[31] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_A__L13*/ meltfnum[11] =
	    melt_is_instance_of ((melt_ptr_t)
				 ( /*_.ASSFAIL_BINDING__V25*/ meltfptr[24]),
				 (melt_ptr_t) (( /*!CLASS_PRIMITIVE_BINDING */
						meltfrout->tabval[7])));;
	  MELT_LOCATION ("warmelt-macro.melt:7089:/ cond");
	  /*cond */ if ( /*_#IS_A__L13*/ meltfnum[11])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-macro.melt:7090:/ getslot");
		{
		  melt_ptr_t slot = NULL, obj = NULL;
		  obj =
		    (melt_ptr_t) ( /*_.ASSFAIL_BINDING__V25*/ meltfptr[24])
		    /*=obj*/ ;
		  melt_object_get_field (slot, obj, 3, "PBIND_PRIMITIVE");
      /*_.PBIND_PRIMITIVE__V46*/ meltfptr[32] = slot;
		};
		;
		/*_._IFELSE___V45*/ meltfptr[28] =
		  /*_.PBIND_PRIMITIVE__V46*/ meltfptr[32];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7089:/ clear");
	       /*clear *//*_.PBIND_PRIMITIVE__V46*/ meltfptr[32] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


		MELT_CHECK_SIGNAL ();
		;

		MELT_CHECK_SIGNAL ();
		;
     /*_#IS_A__L14*/ meltfnum[10] =
		  melt_is_instance_of ((melt_ptr_t)
				       ( /*_.ASSFAIL_BINDING__V25*/
					meltfptr[24]),
				       (melt_ptr_t) (( /*!CLASS_VALUE_BINDING */ meltfrout->tabval[8])));;
		MELT_LOCATION ("warmelt-macro.melt:7091:/ cond");
		/*cond */ if ( /*_#IS_A__L14*/ meltfnum[10])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      MELT_LOCATION ("warmelt-macro.melt:7092:/ getslot");
		      {
			melt_ptr_t slot = NULL, obj = NULL;
			obj =
			  (melt_ptr_t) ( /*_.ASSFAIL_BINDING__V25*/
					meltfptr[24]) /*=obj*/ ;
			melt_object_get_field (slot, obj, 1, "VBIND_VALUE");
	/*_.VBIND_VALUE__V47*/ meltfptr[39] = slot;
		      };
		      ;
       /*_#IS_A__L16*/ meltfnum[15] =
			melt_is_instance_of ((melt_ptr_t)
					     ( /*_.VBIND_VALUE__V47*/
					      meltfptr[39]),
					     (melt_ptr_t) (( /*!CLASS_PRIMITIVE */ meltfrout->tabval[9])));;
		      /*^compute */
		      /*_#_IF___L15*/ meltfnum[14] =
			/*_#IS_A__L16*/ meltfnum[15];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-macro.melt:7091:/ clear");
		 /*clear *//*_.VBIND_VALUE__V47*/ meltfptr[39] = 0;
		      /*^clear */
		 /*clear *//*_#IS_A__L16*/ meltfnum[15] = 0;
		    }
		    ;
		  }
		else
		  {		/*^cond.else */

      /*_#_IF___L15*/ meltfnum[14] = 0;;
		  }
		;
		MELT_LOCATION ("warmelt-macro.melt:7091:/ cond");
		/*cond */ if ( /*_#_IF___L15*/ meltfnum[14])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      MELT_LOCATION ("warmelt-macro.melt:7094:/ getslot");
		      {
			melt_ptr_t slot = NULL, obj = NULL;
			obj =
			  (melt_ptr_t) ( /*_.ASSFAIL_BINDING__V25*/
					meltfptr[24]) /*=obj*/ ;
			melt_object_get_field (slot, obj, 1, "VBIND_VALUE");
	/*_.VBIND_VALUE__V49*/ meltfptr[32] = slot;
		      };
		      ;
		      /*_._IFELSE___V48*/ meltfptr[31] =
			/*_.VBIND_VALUE__V49*/ meltfptr[32];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-macro.melt:7091:/ clear");
		 /*clear *//*_.VBIND_VALUE__V49*/ meltfptr[32] = 0;
		    }
		    ;
		  }
		else
		  {		/*^cond.else */

		    /*^block */
		    /*anyblock */
		    {


#if MELT_HAVE_DEBUG
		      MELT_LOCATION ("warmelt-macro.melt:7096:/ cppif.then");
		      /*^block */
		      /*anyblock */
		      {


			{
			  /*^locexp */
			  /*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
			  melt_dbgcounter++;
#endif
			  ;
			}
			;

			MELT_CHECK_SIGNAL ();
			;
	 /*_#MELT_NEED_DBG__L17*/ meltfnum[15] =
			  /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			  ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			  0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			  ;;
			MELT_LOCATION ("warmelt-macro.melt:7096:/ cond");
			/*cond */ if ( /*_#MELT_NEED_DBG__L17*/ meltfnum[15])	/*then */
			  {
			    /*^cond.then */
			    /*^block */
			    /*anyblock */
			    {

	   /*_#MELT_CALLCOUNT__L18*/ meltfnum[17] =
				/* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
				meltcallcount	/* melt_callcount debugging */
#else
				0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
				;;

			      MELT_CHECK_SIGNAL ();
			      ;
			      MELT_LOCATION
				("warmelt-macro.melt:7096:/ apply");
			      /*apply */
			      {
				union meltparam_un argtab[5];
				memset (&argtab, 0, sizeof (argtab));
				/*^apply.arg */
				argtab[0].meltbp_long =
				  /*_#MELT_CALLCOUNT__L18*/ meltfnum[17];
				/*^apply.arg */
				argtab[1].meltbp_cstring =
				  "warmelt-macro.melt";
				/*^apply.arg */
				argtab[2].meltbp_long = 7096;
				/*^apply.arg */
				argtab[3].meltbp_cstring =
				  "mexpand_assert_msg bad assfail_binding=";
				/*^apply.arg */
				argtab[4].meltbp_aptr =
				  (melt_ptr_t *) & /*_.ASSFAIL_BINDING__V25*/
				  meltfptr[24];
				/*_.MELT_DEBUG_FUN__V51*/ meltfptr[32] =
				  melt_apply ((meltclosure_ptr_t)
					      (( /*!MELT_DEBUG_FUN */
						meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*nil */ NULL)),
					      (MELTBPARSTR_LONG
					       MELTBPARSTR_CSTRING
					       MELTBPARSTR_LONG
					       MELTBPARSTR_CSTRING
					       MELTBPARSTR_PTR ""), argtab,
					      "", (union meltparam_un *) 0);
			      }
			      ;
			      /*_._IF___V50*/ meltfptr[39] =
				/*_.MELT_DEBUG_FUN__V51*/ meltfptr[32];;
			      /*epilog */

			      MELT_LOCATION
				("warmelt-macro.melt:7096:/ clear");
		     /*clear *//*_#MELT_CALLCOUNT__L18*/ meltfnum[17]
				= 0;
			      /*^clear */
		     /*clear *//*_.MELT_DEBUG_FUN__V51*/ meltfptr[32]
				= 0;
			    }
			    ;
			  }
			else
			  {	/*^cond.else */

	  /*_._IF___V50*/ meltfptr[39] = NULL;;
			  }
			;

			{
			  MELT_LOCATION ("warmelt-macro.melt:7096:/ locexp");
			  /*void */ (void) 0;
			}
			;
			/*^quasiblock */


			/*epilog */

			/*^clear */
		   /*clear *//*_#MELT_NEED_DBG__L17*/ meltfnum[15] = 0;
			/*^clear */
		   /*clear *//*_._IF___V50*/ meltfptr[39] = 0;
		      }

#else /*MELT_HAVE_DEBUG */
		      /*^cppif.else */
		      /*^block */
		      /*anyblock */
		      {


			{
			  /*^locexp */
			  /*void */ (void) 0;
			}
			;
			/*epilog */
		      }

#endif /*MELT_HAVE_DEBUG */
		      ;

		      {
			MELT_LOCATION ("warmelt-macro.melt:7097:/ locexp");
			/* error_plain */
			  melt_error_str ((melt_ptr_t)
					  ( /*_.LOC__V18*/ meltfptr[17]),
					  ("ASSERT_FAILED not bound to a primitive in  (ASSERT_MSG <msg> <test>)"),
					  (melt_ptr_t) 0);
		      }
		      ;
		      MELT_LOCATION ("warmelt-macro.melt:7098:/ quasiblock");


       /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

		      {
			MELT_LOCATION ("warmelt-macro.melt:7098:/ locexp");
			/*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
			if (meltxresdescr_ && meltxresdescr_[0]
			    && meltxrestab_)
			  melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
			;
		      }
		      ;
		      /*^finalreturn */
		      ;
		      /*finalret */ goto meltlabend_rout;
		      MELT_LOCATION ("warmelt-macro.melt:7095:/ quasiblock");


		      /*_.PROGN___V53*/ meltfptr[39] =
			/*_.RETURN___V52*/ meltfptr[32];;
		      /*^compute */
		      /*_._IFELSE___V48*/ meltfptr[31] =
			/*_.PROGN___V53*/ meltfptr[39];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-macro.melt:7091:/ clear");
		 /*clear *//*_.RETURN___V52*/ meltfptr[32] = 0;
		      /*^clear */
		 /*clear *//*_.PROGN___V53*/ meltfptr[39] = 0;
		    }
		    ;
		  }
		;
		/*_._IFELSE___V45*/ meltfptr[28] =
		  /*_._IFELSE___V48*/ meltfptr[31];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7089:/ clear");
	       /*clear *//*_#IS_A__L14*/ meltfnum[10] = 0;
		/*^clear */
	       /*clear *//*_#_IF___L15*/ meltfnum[14] = 0;
		/*^clear */
	       /*clear *//*_._IFELSE___V48*/ meltfptr[31] = 0;
	      }
	      ;
	    }
	  ;
	  /*_.AFPRIM__V36*/ meltfptr[30] = /*_._IFELSE___V45*/ meltfptr[28];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7081:/ clear");
	     /*clear *//*_#IS_A__L13*/ meltfnum[11] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V45*/ meltfptr[28] = 0;
	}
	;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_MIXINT__L19*/ meltfnum[17] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])) ==
       MELTOBMAG_MIXINT);;
    MELT_LOCATION ("warmelt-macro.melt:7101:/ cond");
    /*cond */ if ( /*_#IS_MIXINT__L19*/ meltfnum[17])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.MIXINT_VAL__V55*/ meltfptr[39] =
	    (melt_val_mixint ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])));;
	  /*^compute */
	  /*_.FILNAM__V54*/ meltfptr[32] =
	    /*_.MIXINT_VAL__V55*/ meltfptr[39];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7101:/ clear");
	     /*clear *//*_.MIXINT_VAL__V55*/ meltfptr[39] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_MIXLOC__L20*/ meltfnum[15] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])) ==
	     MELTOBMAG_MIXLOC);;
	  MELT_LOCATION ("warmelt-macro.melt:7102:/ cond");
	  /*cond */ if ( /*_#IS_MIXLOC__L20*/ meltfnum[15])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MIXLOC_VAL__V57*/ meltfptr[28] =
		  (melt_val_mixloc
		   ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])));;
		/*^compute */
		/*_._IFELSE___V56*/ meltfptr[31] =
		  /*_.MIXLOC_VAL__V57*/ meltfptr[28];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7102:/ clear");
	       /*clear *//*_.MIXLOC_VAL__V57*/ meltfptr[28] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_._IFELSE___V56*/ meltfptr[31] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_.FILNAM__V54*/ meltfptr[32] = /*_._IFELSE___V56*/ meltfptr[31];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7101:/ clear");
	     /*clear *//*_#IS_MIXLOC__L20*/ meltfnum[15] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V56*/ meltfptr[31] = 0;
	}
	;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#GET_INT__L21*/ meltfnum[10] =
      (melt_get_int ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])));;
    /*^compute */
 /*_.MAKE_INTEGERBOX__V58*/ meltfptr[39] =
      (meltgc_new_int
       ((meltobject_ptr_t) (( /*!DISCR_INTEGER */ meltfrout->tabval[11])),
	( /*_#GET_INT__L21*/ meltfnum[10])));;
    MELT_LOCATION ("warmelt-macro.melt:7107:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_1_st
      {
	struct MELT_MULTIPLE_STRUCT (3) rtup_0__TUPLREC__x9;
	long meltletrec_1_endgap;
      } *meltletrec_1_ptr = 0;
      meltletrec_1_ptr =
	(struct meltletrec_1_st *)
	meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x9 */
 /*_.TUPLREC___V60*/ meltfptr[31] =
	(melt_ptr_t) & meltletrec_1_ptr->rtup_0__TUPLREC__x9;
      meltletrec_1_ptr->rtup_0__TUPLREC__x9.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_1_ptr->rtup_0__TUPLREC__x9.nbval = 3;


      /*^putuple */
      /*putupl#12 */
      melt_assertmsg ("putupl [:7107] #12 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V60*/ meltfptr[31]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7107] #12 checkoff",
		      (0 >= 0
		       && 0 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V60*/
					      meltfptr[31]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V60*/ meltfptr[31]))->tabval[0] =
	(melt_ptr_t) ( /*_.XMSG__V22*/ meltfptr[21]);
      ;
      /*^putuple */
      /*putupl#13 */
      melt_assertmsg ("putupl [:7107] #13 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V60*/ meltfptr[31]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7107] #13 checkoff",
		      (1 >= 0
		       && 1 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V60*/
					      meltfptr[31]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V60*/ meltfptr[31]))->tabval[1] =
	(melt_ptr_t) ( /*_.FILNAM__V54*/ meltfptr[32]);
      ;
      /*^putuple */
      /*putupl#14 */
      melt_assertmsg ("putupl [:7107] #14 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V60*/ meltfptr[31]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7107] #14 checkoff",
		      (2 >= 0
		       && 2 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V60*/
					      meltfptr[31]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V60*/ meltfptr[31]))->tabval[2] =
	(melt_ptr_t) ( /*_.MAKE_INTEGERBOX__V58*/ meltfptr[39]);
      ;
      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V60*/ meltfptr[31]);
      ;
      /*_.TUPLE___V59*/ meltfptr[28] = /*_.TUPLREC___V60*/ meltfptr[31];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7107:/ clear");
	    /*clear *//*_.TUPLREC___V60*/ meltfptr[31] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V60*/ meltfptr[31] = 0;
    }				/*end multiallocblock */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7103:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_PRIMITIVE */
					     meltfrout->tabval[10])), (4),
			      "CLASS_SOURCE_PRIMITIVE");
  /*_.INST__V62*/ meltfptr[61] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V62*/ meltfptr[61])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V62*/ meltfptr[61]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SPRIM_OPER",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V62*/ meltfptr[61])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V62*/ meltfptr[61]), (3),
			  ( /*_.AFPRIM__V36*/ meltfptr[30]), "SPRIM_OPER");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SARGOP_ARGS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V62*/ meltfptr[61])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V62*/ meltfptr[61]), (2),
			  ( /*_.TUPLE___V59*/ meltfptr[28]), "SARGOP_ARGS");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V62*/ meltfptr[61],
				  "newly made instance");
    ;
    /*_.APRIM__V61*/ meltfptr[31] = /*_.INST__V62*/ meltfptr[61];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7111:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_IFELSE */
					     meltfrout->tabval[12])), (5),
			      "CLASS_SOURCE_IFELSE");
  /*_.INST__V64*/ meltfptr[63] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V64*/ meltfptr[63])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V64*/ meltfptr[63]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIF_TEST",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V64*/ meltfptr[63])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V64*/ meltfptr[63]), (2),
			  ( /*_.XTEST__V23*/ meltfptr[22]), "SIF_TEST");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIF_THEN",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V64*/ meltfptr[63])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V64*/ meltfptr[63]), (3),
			  (( /*nil */ NULL)), "SIF_THEN");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIF_ELSE",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V64*/ meltfptr[63])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V64*/ meltfptr[63]), (4),
			  ( /*_.APRIM__V61*/ meltfptr[31]), "SIF_ELSE");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V64*/ meltfptr[63],
				  "newly made instance");
    ;
    /*_.ATEST__V63*/ meltfptr[62] = /*_.INST__V64*/ meltfptr[63];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7117:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_CPPIF */
					     meltfrout->tabval[13])), (5),
			      "CLASS_SOURCE_CPPIF");
  /*_.INST__V66*/ meltfptr[65] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V66*/ meltfptr[65])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V66*/ meltfptr[65]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIFP_COND",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V66*/ meltfptr[65])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V66*/ meltfptr[65]), (2),
			  (( /*!konst_14_MELT_HAVE_DEBUG */ meltfrout->
			    tabval[14])), "SIFP_COND");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIFP_THEN",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V66*/ meltfptr[65])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V66*/ meltfptr[65]), (3),
			  ( /*_.ATEST__V63*/ meltfptr[62]), "SIFP_THEN");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIFP_ELSE",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V66*/ meltfptr[65])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V66*/ meltfptr[65]), (4),
			  (( /*nil */ NULL)), "SIFP_ELSE");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V66*/ meltfptr[65],
				  "newly made instance");
    ;
    /*_.ACPPIF__V65*/ meltfptr[64] = /*_.INST__V66*/ meltfptr[65];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7124:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L22*/ meltfnum[14] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7124:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L22*/ meltfnum[14])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L23*/ meltfnum[11] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7124:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L23*/ meltfnum[11];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7124;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_assert_msg result acppif";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.ACPPIF__V65*/ meltfptr[64];
	      /*_.MELT_DEBUG_FUN__V68*/ meltfptr[67] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V67*/ meltfptr[66] =
	      /*_.MELT_DEBUG_FUN__V68*/ meltfptr[67];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7124:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L23*/ meltfnum[11] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V68*/ meltfptr[67] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V67*/ meltfptr[66] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7124:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L22*/ meltfnum[14] = 0;
      /*^clear */
	     /*clear *//*_._IF___V67*/ meltfptr[66] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7125:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.ACPPIF__V65*/ meltfptr[64];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7125:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V35*/ meltfptr[29] = /*_.RETURN___V69*/ meltfptr[67];;

    MELT_LOCATION ("warmelt-macro.melt:7078:/ clear");
	   /*clear *//*_#NULL__L10*/ meltfnum[9] = 0;
    /*^clear */
	   /*clear *//*_.AFPRIM__V36*/ meltfptr[30] = 0;
    /*^clear */
	   /*clear *//*_#IS_MIXINT__L19*/ meltfnum[17] = 0;
    /*^clear */
	   /*clear *//*_.FILNAM__V54*/ meltfptr[32] = 0;
    /*^clear */
	   /*clear *//*_#GET_INT__L21*/ meltfnum[10] = 0;
    /*^clear */
	   /*clear *//*_.MAKE_INTEGERBOX__V58*/ meltfptr[39] = 0;
    /*^clear */
	   /*clear *//*_.TUPLE___V59*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_.APRIM__V61*/ meltfptr[31] = 0;
    /*^clear */
	   /*clear *//*_.ATEST__V63*/ meltfptr[62] = 0;
    /*^clear */
	   /*clear *//*_.ACPPIF__V65*/ meltfptr[64] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V69*/ meltfptr[67] = 0;
    /*_.LET___V16*/ meltfptr[14] = /*_.LET___V35*/ meltfptr[29];;

    MELT_LOCATION ("warmelt-macro.melt:7058:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.XMSG__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.XTEST__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.ASSFAIL_SYMB__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.ASSFAIL_BINDING__V25*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_#IS_STRING__L7*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L8*/ meltfnum[7] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V26*/ meltfptr[25] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V27*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_#IS_PAIR__L9*/ meltfnum[8] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V28*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.LET___V35*/ meltfptr[29] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7052:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7052:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L5*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_ASSERT_MSG", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_148_WARMELTmiMACRO_MEXPAND_ASSERT_MSG */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un *
					       meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un *
					       meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 60
    melt_ptr_t mcfr_varptr[60];
#define MELTFRAM_NBVARNUM 17
    long mcfr_varnum[17];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG_st *)
	meltfirstargp_;
      /* use arguments meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 60; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG nbval 60*/
  meltfram__.mcfr_nbvar = 60 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_DEBUG_MSG", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7141:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7142:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7142:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7142:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7142;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_debug_msg sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7142:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7142:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7143:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7143:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7143:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7143) ? (7143) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7143:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7144:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7144:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7144:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7144) ? (7144) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7144:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7145:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L5*/ meltfnum[1] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7145:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7145:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7145) ? (7145) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7145:/ clear");
	     /*clear *//*_#IS_OBJECT__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7146:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_CLOSURE__L6*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) ==
	 MELTOBMAG_CLOSURE);;
      MELT_LOCATION ("warmelt-macro.melt:7146:/ cond");
      /*cond */ if ( /*_#IS_CLOSURE__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7146:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mexpander"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7146) ? (7146) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7146:/ clear");
	     /*clear *//*_#IS_CLOSURE__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7147:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7148:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7149:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;
    /*^compute */
 /*_.PAIR_HEAD__V21*/ meltfptr[20] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7151:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.XVAL__V22*/ meltfptr[21] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V21*/ meltfptr[20]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*_.XMSG__V23*/ meltfptr[22] = ( /*nil */ NULL);;
    /*^compute */
    /*_.XCOUNT__V24*/ meltfptr[23] = ( /*nil */ NULL);;

    {
      MELT_LOCATION ("warmelt-macro.melt:7155:/ locexp");
      melt_warning_str (0, (melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			("(DEBUG_MSG ....) is obsolete, use (DEBUG ...) variadically"),
			(melt_ptr_t) 0);
    }
    ;
 /*_.PAIR_TAIL__V25*/ meltfptr[24] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7156:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V26*/ meltfptr[25] =
      /*_.PAIR_TAIL__V25*/ meltfptr[24];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_PAIR__L7*/ meltfnum[1] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])) ==
       MELTOBMAG_PAIR);;
    /*^compute */
 /*_#NOT__L8*/ meltfnum[0] =
      (!( /*_#IS_PAIR__L7*/ meltfnum[1]));;
    MELT_LOCATION ("warmelt-macro.melt:7157:/ cond");
    /*cond */ if ( /*_#NOT__L8*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7158:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("missing message in DEBUG_MSG"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_HEAD__V27*/ meltfptr[26] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7159:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.MEXPANDER__V28*/ meltfptr[27] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V27*/ meltfptr[26]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*^compute */
    /*_.XMSG__V23*/ meltfptr[22] = /*_.SETQ___V29*/ meltfptr[28] =
      /*_.MEXPANDER__V28*/ meltfptr[27];;
 /*_.PAIR_TAIL__V30*/ meltfptr[29] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7160:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V31*/ meltfptr[30] =
      /*_.PAIR_TAIL__V30*/ meltfptr[29];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_PAIR__L9*/ meltfnum[8] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])) ==
       MELTOBMAG_PAIR);;
    MELT_LOCATION ("warmelt-macro.melt:7161:/ cond");
    /*cond */ if ( /*_#IS_PAIR__L9*/ meltfnum[8])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.PAIR_HEAD__V33*/ meltfptr[32] =
	    (melt_pair_head
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7163:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[3];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
	    /*^apply.arg */
	    argtab[1].meltbp_aptr =
	      (melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
	    /*^apply.arg */
	    argtab[2].meltbp_aptr =
	      (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
	    /*_.MEXPANDER__V34*/ meltfptr[33] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.MEXPANDER__V4*/ meltfptr[3]),
			  (melt_ptr_t) ( /*_.PAIR_HEAD__V33*/ meltfptr[32]),
			  (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR
			   ""), argtab, "", (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.XCOUNT__V24*/ meltfptr[23] = /*_.SETQ___V35*/ meltfptr[34] =
	    /*_.MEXPANDER__V34*/ meltfptr[33];;
   /*_.PAIR_TAIL__V36*/ meltfptr[35] =
	    (melt_pair_tail
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
	  MELT_LOCATION ("warmelt-macro.melt:7164:/ compute");
	  /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V37*/ meltfptr[36] =
	    /*_.PAIR_TAIL__V36*/ meltfptr[35];;
	  MELT_LOCATION ("warmelt-macro.melt:7162:/ quasiblock");


	  /*_.PROGN___V38*/ meltfptr[37] = /*_.SETQ___V37*/ meltfptr[36];;
	  /*^compute */
	  /*_._IF___V32*/ meltfptr[31] = /*_.PROGN___V38*/ meltfptr[37];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7161:/ clear");
	     /*clear *//*_.PAIR_HEAD__V33*/ meltfptr[32] = 0;
	  /*^clear */
	     /*clear *//*_.MEXPANDER__V34*/ meltfptr[33] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V35*/ meltfptr[34] = 0;
	  /*^clear */
	     /*clear *//*_.PAIR_TAIL__V36*/ meltfptr[35] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V37*/ meltfptr[36] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V38*/ meltfptr[37] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V32*/ meltfptr[31] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7165:/ cond");
    /*cond */ if ( /*_.CURPAIR__V20*/ meltfptr[19])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7166:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("too many arguments to DEBUG_MSG"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_STRING__L10*/ meltfnum[9] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.XMSG__V23*/ meltfptr[22])) ==
       MELTOBMAG_STRING);;
    /*^compute */
 /*_#NOT__L11*/ meltfnum[10] =
      (!( /*_#IS_STRING__L10*/ meltfnum[9]));;
    MELT_LOCATION ("warmelt-macro.melt:7167:/ cond");
    /*cond */ if ( /*_#NOT__L11*/ meltfnum[10])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7168:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("message argument should be string in DEBUG_MSG"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#NULL__L12*/ meltfnum[11] =
      (( /*_.XCOUNT__V24*/ meltfptr[23]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7169:/ cond");
    /*cond */ if ( /*_#NULL__L12*/ meltfnum[11])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-macro.melt:7170:/ quasiblock");


   /*_.CONLIS__V41*/ meltfptr[34] =
	    (meltgc_new_list
	     ((meltobject_ptr_t)
	      (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:7171:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.CONLIS__V41*/ meltfptr[34]),
				(melt_ptr_t) (( /*!konst_4_MELT_CALLCOUNT */
					       meltfrout->tabval[4])));
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7172:/ quasiblock");


	  /*^rawallocobj */
	  /*rawallocobj */
	  {
	    melt_ptr_t newobj = 0;
	    melt_raw_object_create (newobj,
				    (melt_ptr_t) (( /*!CLASS_SEXPR */
						   meltfrout->tabval[1])),
				    (3), "CLASS_SEXPR");
    /*_.INST__V43*/ meltfptr[36] =
	      newobj;
	  };
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("checkobj putslot  _ @SEXP_CONTENTS",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V43*/ meltfptr[36]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V43*/ meltfptr[36]), (2),
				( /*_.CONLIS__V41*/ meltfptr[34]),
				"SEXP_CONTENTS");
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V43*/ meltfptr[36]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V43*/ meltfptr[36]), (1),
				( /*_.LOC__V18*/ meltfptr[17]),
				"LOCA_LOCATION");
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object ( /*_.INST__V43*/ meltfptr[36],
					"newly made instance");
	  ;
	  /*_.INST___V42*/ meltfptr[35] = /*_.INST__V43*/ meltfptr[36];;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7172:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[2];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
	    /*^apply.arg */
	    argtab[1].meltbp_aptr =
	      (melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
	    /*_.MACROEXPAND_1__V44*/ meltfptr[37] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!MACROEXPAND_1 */ meltfrout->tabval[5])),
			  (melt_ptr_t) ( /*_.INST___V42*/ meltfptr[35]),
			  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.XCOUNT__V24*/ meltfptr[23] = /*_.SETQ___V45*/ meltfptr[44] =
	    /*_.MACROEXPAND_1__V44*/ meltfptr[37];;
	  /*_.LET___V40*/ meltfptr[33] = /*_.SETQ___V45*/ meltfptr[44];;

	  MELT_LOCATION ("warmelt-macro.melt:7170:/ clear");
	     /*clear *//*_.CONLIS__V41*/ meltfptr[34] = 0;
	  /*^clear */
	     /*clear *//*_.INST___V42*/ meltfptr[35] = 0;
	  /*^clear */
	     /*clear *//*_.MACROEXPAND_1__V44*/ meltfptr[37] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V45*/ meltfptr[44] = 0;
	  /*_._IF___V39*/ meltfptr[32] = /*_.LET___V40*/ meltfptr[33];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7169:/ clear");
	     /*clear *//*_.LET___V40*/ meltfptr[33] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V39*/ meltfptr[32] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-macro.melt:7176:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_MIXINT__L13*/ meltfnum[12] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])) ==
       MELTOBMAG_MIXINT);;
    MELT_LOCATION ("warmelt-macro.melt:7178:/ cond");
    /*cond */ if ( /*_#IS_MIXINT__L13*/ meltfnum[12])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.MIXINT_VAL__V48*/ meltfptr[37] =
	    (melt_val_mixint ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])));;
	  /*^compute */
	  /*_.DFILNAM__V47*/ meltfptr[35] =
	    /*_.MIXINT_VAL__V48*/ meltfptr[37];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7178:/ clear");
	     /*clear *//*_.MIXINT_VAL__V48*/ meltfptr[37] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_MIXLOC__L14*/ meltfnum[13] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])) ==
	     MELTOBMAG_MIXLOC);;
	  MELT_LOCATION ("warmelt-macro.melt:7179:/ cond");
	  /*cond */ if ( /*_#IS_MIXLOC__L14*/ meltfnum[13])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MIXLOC_VAL__V50*/ meltfptr[33] =
		  (melt_val_mixloc
		   ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])));;
		/*^compute */
		/*_._IFELSE___V49*/ meltfptr[44] =
		  /*_.MIXLOC_VAL__V50*/ meltfptr[33];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7179:/ clear");
	       /*clear *//*_.MIXLOC_VAL__V50*/ meltfptr[33] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_._IFELSE___V49*/ meltfptr[44] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_.DFILNAM__V47*/ meltfptr[35] = /*_._IFELSE___V49*/ meltfptr[44];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7178:/ clear");
	     /*clear *//*_#IS_MIXLOC__L14*/ meltfnum[13] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V49*/ meltfptr[44] = 0;
	}
	;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#GET_INT__L15*/ meltfnum[13] =
      (melt_get_int ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17])));;
    /*^compute */
 /*_.MAKE_INTEGERBOX__V51*/ meltfptr[37] =
      (meltgc_new_int
       ((meltobject_ptr_t) (( /*!DISCR_INTEGER */ meltfrout->tabval[8])),
	( /*_#GET_INT__L15*/ meltfnum[13])));;
    MELT_LOCATION ("warmelt-macro.melt:7184:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_1_st
      {
	struct MELT_MULTIPLE_STRUCT (5) rtup_0__TUPLREC__x10;
	long meltletrec_1_endgap;
      } *meltletrec_1_ptr = 0;
      meltletrec_1_ptr =
	(struct meltletrec_1_st *)
	meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x10 */
 /*_.TUPLREC___V53*/ meltfptr[44] =
	(melt_ptr_t) & meltletrec_1_ptr->rtup_0__TUPLREC__x10;
      meltletrec_1_ptr->rtup_0__TUPLREC__x10.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_1_ptr->rtup_0__TUPLREC__x10.nbval = 5;


      /*^putuple */
      /*putupl#15 */
      melt_assertmsg ("putupl [:7184] #15 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V53*/ meltfptr[44]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7184] #15 checkoff",
		      (0 >= 0
		       && 0 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V53*/
					      meltfptr[44]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V53*/ meltfptr[44]))->tabval[0] =
	(melt_ptr_t) ( /*_.XVAL__V22*/ meltfptr[21]);
      ;
      /*^putuple */
      /*putupl#16 */
      melt_assertmsg ("putupl [:7184] #16 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V53*/ meltfptr[44]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7184] #16 checkoff",
		      (1 >= 0
		       && 1 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V53*/
					      meltfptr[44]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V53*/ meltfptr[44]))->tabval[1] =
	(melt_ptr_t) ( /*_.XMSG__V23*/ meltfptr[22]);
      ;
      /*^putuple */
      /*putupl#17 */
      melt_assertmsg ("putupl [:7184] #17 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V53*/ meltfptr[44]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7184] #17 checkoff",
		      (2 >= 0
		       && 2 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V53*/
					      meltfptr[44]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V53*/ meltfptr[44]))->tabval[2] =
	(melt_ptr_t) ( /*_.XCOUNT__V24*/ meltfptr[23]);
      ;
      /*^putuple */
      /*putupl#18 */
      melt_assertmsg ("putupl [:7184] #18 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V53*/ meltfptr[44]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7184] #18 checkoff",
		      (3 >= 0
		       && 3 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V53*/
					      meltfptr[44]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V53*/ meltfptr[44]))->tabval[3] =
	(melt_ptr_t) ( /*_.DFILNAM__V47*/ meltfptr[35]);
      ;
      /*^putuple */
      /*putupl#19 */
      melt_assertmsg ("putupl [:7184] #19 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V53*/ meltfptr[44]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7184] #19 checkoff",
		      (4 >= 0
		       && 4 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V53*/
					      meltfptr[44]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V53*/ meltfptr[44]))->tabval[4] =
	(melt_ptr_t) ( /*_.MAKE_INTEGERBOX__V51*/ meltfptr[37]);
      ;
      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V53*/ meltfptr[44]);
      ;
      /*_.TUPLE___V52*/ meltfptr[33] = /*_.TUPLREC___V53*/ meltfptr[44];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7184:/ clear");
	    /*clear *//*_.TUPLREC___V53*/ meltfptr[44] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V53*/ meltfptr[44] = 0;
    }				/*end multiallocblock */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7180:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_APPLY */
					     meltfrout->tabval[6])), (4),
			      "CLASS_SOURCE_APPLY");
  /*_.INST__V55*/ meltfptr[54] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V55*/ meltfptr[54])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V55*/ meltfptr[54]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SAPP_FUN",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V55*/ meltfptr[54])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V55*/ meltfptr[54]), (3),
			  (( /*!konst_7_DEBUG_MSG_FUN */ meltfrout->
			    tabval[7])), "SAPP_FUN");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SARGOP_ARGS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V55*/ meltfptr[54])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V55*/ meltfptr[54]), (2),
			  ( /*_.TUPLE___V52*/ meltfptr[33]), "SARGOP_ARGS");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V55*/ meltfptr[54],
				  "newly made instance");
    ;
    /*_.DCALL__V54*/ meltfptr[44] = /*_.INST__V55*/ meltfptr[54];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7190:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_CPPIF */
					     meltfrout->tabval[9])), (5),
			      "CLASS_SOURCE_CPPIF");
  /*_.INST__V57*/ meltfptr[56] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V57*/ meltfptr[56])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V57*/ meltfptr[56]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIFP_COND",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V57*/ meltfptr[56])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V57*/ meltfptr[56]), (2),
			  (( /*!konst_10_MELT_HAVE_DEBUG */ meltfrout->
			    tabval[10])), "SIFP_COND");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIFP_THEN",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V57*/ meltfptr[56])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V57*/ meltfptr[56]), (3),
			  ( /*_.DCALL__V54*/ meltfptr[44]), "SIFP_THEN");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIFP_ELSE",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V57*/ meltfptr[56])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V57*/ meltfptr[56]), (4),
			  (( /*nil */ NULL)), "SIFP_ELSE");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V57*/ meltfptr[56],
				  "newly made instance");
    ;
    /*_.DCPPIF__V56*/ meltfptr[55] = /*_.INST__V57*/ meltfptr[56];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7197:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L16*/ meltfnum[15] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7197:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L16*/ meltfnum[15])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L17*/ meltfnum[16] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7197:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[16];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7197;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_debug_msg result dcppif";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.DCPPIF__V56*/ meltfptr[55];
	      /*_.MELT_DEBUG_FUN__V59*/ meltfptr[58] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V58*/ meltfptr[57] =
	      /*_.MELT_DEBUG_FUN__V59*/ meltfptr[58];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7197:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L17*/ meltfnum[16] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V59*/ meltfptr[58] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V58*/ meltfptr[57] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7197:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L16*/ meltfnum[15] = 0;
      /*^clear */
	     /*clear *//*_._IF___V58*/ meltfptr[57] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7198:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.DCPPIF__V56*/ meltfptr[55];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7198:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V46*/ meltfptr[34] = /*_.RETURN___V60*/ meltfptr[58];;

    MELT_LOCATION ("warmelt-macro.melt:7176:/ clear");
	   /*clear *//*_#IS_MIXINT__L13*/ meltfnum[12] = 0;
    /*^clear */
	   /*clear *//*_.DFILNAM__V47*/ meltfptr[35] = 0;
    /*^clear */
	   /*clear *//*_#GET_INT__L15*/ meltfnum[13] = 0;
    /*^clear */
	   /*clear *//*_.MAKE_INTEGERBOX__V51*/ meltfptr[37] = 0;
    /*^clear */
	   /*clear *//*_.TUPLE___V52*/ meltfptr[33] = 0;
    /*^clear */
	   /*clear *//*_.DCALL__V54*/ meltfptr[44] = 0;
    /*^clear */
	   /*clear *//*_.DCPPIF__V56*/ meltfptr[55] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V60*/ meltfptr[58] = 0;
    /*_.LET___V16*/ meltfptr[14] = /*_.LET___V46*/ meltfptr[34];;

    MELT_LOCATION ("warmelt-macro.melt:7147:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.XVAL__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.XMSG__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.XCOUNT__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V25*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V26*/ meltfptr[25] = 0;
    /*^clear */
	   /*clear *//*_#IS_PAIR__L7*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L8*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V27*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.MEXPANDER__V28*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V29*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V30*/ meltfptr[29] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V31*/ meltfptr[30] = 0;
    /*^clear */
	   /*clear *//*_#IS_PAIR__L9*/ meltfnum[8] = 0;
    /*^clear */
	   /*clear *//*_._IF___V32*/ meltfptr[31] = 0;
    /*^clear */
	   /*clear *//*_#IS_STRING__L10*/ meltfnum[9] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L11*/ meltfnum[10] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L12*/ meltfnum[11] = 0;
    /*^clear */
	   /*clear *//*_._IF___V39*/ meltfptr[32] = 0;
    /*^clear */
	   /*clear *//*_.LET___V46*/ meltfptr[34] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7141:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7141:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_DEBUG_MSG", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_149_WARMELTmiMACRO_MEXPAND_DEBUG_MSG */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un * meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 92
    melt_ptr_t mcfr_varptr[92];
#define MELTFRAM_NBVARNUM 30
    long mcfr_varnum[30];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG_st *)
	meltfirstargp_;
      /* use arguments meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 92; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG nbval 92*/
  meltfram__.mcfr_nbvar = 92 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_DEBUG", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7214:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7215:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7215:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7215:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7215;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_debug sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7215:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7215:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7216:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7216:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7216:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7216) ? (7216) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7216:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7217:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7217:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7217:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7217) ? (7217) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7217:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7218:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L5*/ meltfnum[1] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7218:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7218:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7218) ? (7218) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7218:/ clear");
	     /*clear *//*_#IS_OBJECT__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7219:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7220:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V15*/ meltfptr[14] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7221:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V16*/ meltfptr[15] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_MIXINT__L6*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15])) ==
       MELTOBMAG_MIXINT);;
    MELT_LOCATION ("warmelt-macro.melt:7223:/ cond");
    /*cond */ if ( /*_#IS_MIXINT__L6*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.MIXINT_VAL__V18*/ meltfptr[17] =
	    (melt_val_mixint ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15])));;
	  /*^compute */
	  /*_.DFILNAM__V17*/ meltfptr[16] =
	    /*_.MIXINT_VAL__V18*/ meltfptr[17];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7223:/ clear");
	     /*clear *//*_.MIXINT_VAL__V18*/ meltfptr[17] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_MIXLOC__L7*/ meltfnum[1] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15])) ==
	     MELTOBMAG_MIXLOC);;
	  MELT_LOCATION ("warmelt-macro.melt:7224:/ cond");
	  /*cond */ if ( /*_#IS_MIXLOC__L7*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MIXLOC_VAL__V20*/ meltfptr[19] =
		  (melt_val_mixloc
		   ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15])));;
		/*^compute */
		/*_._IFELSE___V19*/ meltfptr[17] =
		  /*_.MIXLOC_VAL__V20*/ meltfptr[19];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7224:/ clear");
	       /*clear *//*_.MIXLOC_VAL__V20*/ meltfptr[19] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_._IFELSE___V19*/ meltfptr[17] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_.DFILNAM__V17*/ meltfptr[16] = /*_._IFELSE___V19*/ meltfptr[17];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7223:/ clear");
	     /*clear *//*_#IS_MIXLOC__L7*/ meltfnum[1] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V19*/ meltfptr[17] = 0;
	}
	;
      }
    ;
 /*_#DLINE__L8*/ meltfnum[1] =
      (melt_get_int ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7226:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!konst_4_MELT_DEBUG_FUN */ meltfrout->tabval[4]);
      /*_.DEBUGFUNBIND__V21*/ meltfptr[19] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!FIND_ENV */ meltfrout->tabval[3])),
		    (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7227:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!konst_5_MELT_CALLCOUNT */ meltfrout->tabval[5]);
      /*_.THECOUNTBIND__V22*/ meltfptr[17] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!FIND_ENV */ meltfrout->tabval[3])),
		    (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7228:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!konst_6_MELT_NEED_DBG */ meltfrout->tabval[6]);
      /*_.THENEEDDBGBIND__V23*/ meltfptr[22] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!FIND_ENV */ meltfrout->tabval[3])),
		    (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7229:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!konst_7_MELT_INCREMENT_DBGCOUNTER */ meltfrout->
			  tabval[7]);
      /*_.THEINCRDBGBIND__V24*/ meltfptr[23] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!FIND_ENV */ meltfrout->tabval[3])),
		    (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
 /*_.LIST_FIRST__V25*/ meltfptr[24] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V15*/ meltfptr[14])));;
    /*^compute */
 /*_.CURPAIR__V26*/ meltfptr[25] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V25*/ meltfptr[24])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7231:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.XARGLIST__V27*/ meltfptr[26] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!EXPAND_PAIRLIST_AS_LIST */ meltfrout->tabval[8])),
		    (melt_ptr_t) ( /*_.CURPAIR__V26*/ meltfptr[25]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    MELT_LOCATION ("warmelt-macro.melt:7232:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_1_st
      {
	struct meltpair_st rpair_0___PAIROFLIST_x1;
	struct meltlist_st rlist_1__LIST_;
	long meltletrec_1_endgap;
      } *meltletrec_1_ptr = 0;
      meltletrec_1_ptr =
	(struct meltletrec_1_st *)
	meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
      /*^blockmultialloc.initfill */
      /*inipair rpair_0___PAIROFLIST_x1 */
   /*_._PAIROFLIST__V29*/ meltfptr[28] =
	(melt_ptr_t) & meltletrec_1_ptr->rpair_0___PAIROFLIST_x1;
      meltletrec_1_ptr->rpair_0___PAIROFLIST_x1.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_PAIR))));

      /*inilist rlist_1__LIST_ */
   /*_.LIST___V30*/ meltfptr[29] =
	(melt_ptr_t) & meltletrec_1_ptr->rlist_1__LIST_;
      meltletrec_1_ptr->rlist_1__LIST_.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_LIST))));



      /*^putpairhead */
      /*putpairhead */
      melt_assertmsg ("putpairhead /1 checkpair",
		      melt_magic_discr ((melt_ptr_t)
					( /*_._PAIROFLIST__V29*/
					 meltfptr[28])) == MELTOBMAG_PAIR);
      ((meltpair_ptr_t) ( /*_._PAIROFLIST__V29*/ meltfptr[28]))->hd =
	(melt_ptr_t) (( /*nil */ NULL));
      ;
      /*^touch */
      meltgc_touch ( /*_._PAIROFLIST__V29*/ meltfptr[28]);
      ;
      /*^putlist */
      /*putlist */
      melt_assertmsg ("putlist checklist",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.LIST___V30*/ meltfptr[29])) ==
		      MELTOBMAG_LIST);
      ((meltlist_ptr_t) ( /*_.LIST___V30*/ meltfptr[29]))->first =
	(meltpair_ptr_t) ( /*_._PAIROFLIST__V29*/ meltfptr[28]);
      ((meltlist_ptr_t) ( /*_.LIST___V30*/ meltfptr[29]))->last =
	(meltpair_ptr_t) ( /*_._PAIROFLIST__V29*/ meltfptr[28]);
      ;
      /*^touch */
      meltgc_touch ( /*_.LIST___V30*/ meltfptr[29]);
      ;
      /*_.DBGARGLIST__V28*/ meltfptr[27] = /*_.LIST___V30*/ meltfptr[29];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7232:/ clear");
	    /*clear *//*_._PAIROFLIST__V29*/ meltfptr[28] = 0;
      /*^clear */
	    /*clear *//*_.LIST___V30*/ meltfptr[29] = 0;
      /*^clear */
	    /*clear *//*_._PAIROFLIST__V29*/ meltfptr[28] = 0;
      /*^clear */
	    /*clear *//*_.LIST___V30*/ meltfptr[29] = 0;
    }				/*end multiallocblock */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7235:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      /*_.SEXPW__V31*/ meltfptr[28] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!S_EXPR_WEIGHT */ meltfrout->tabval[9])),
		    (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]), (""),
		    (union meltparam_un *) 0, "", (union meltparam_un *) 0);
    }
    ;
 /*_#NBSEPW__L9*/ meltfnum[8] =
      (melt_get_int ((melt_ptr_t) ( /*_.SEXPW__V31*/ meltfptr[28])));;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#gtI__L10*/ meltfnum[9] =
      (( /*_#NBSEPW__L9*/ meltfnum[8]) > (26));;
    MELT_LOCATION ("warmelt-macro.melt:7238:/ cond");
    /*cond */ if ( /*_#gtI__L10*/ meltfnum[9])	/*then */
      {
	/*^cond.then */
	/*_#OR___L11*/ meltfnum[10] = /*_#gtI__L10*/ meltfnum[9];;
      }
    else
      {
	MELT_LOCATION ("warmelt-macro.melt:7238:/ cond.else");

	/*^block */
	/*anyblock */
	{

   /*_#NULL__L12*/ meltfnum[11] =
	    (( /*_.SEXPW__V31*/ meltfptr[28]) == NULL);;
	  /*^compute */
	  /*_#OR___L11*/ meltfnum[10] = /*_#NULL__L12*/ meltfnum[11];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7238:/ clear");
	     /*clear *//*_#NULL__L12*/ meltfnum[11] = 0;
	}
	;
      }
    ;
    /*^cond */
    /*cond */ if ( /*_#OR___L11*/ meltfnum[10])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7243:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[2];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_cstring = "weight:";
	    /*^apply.arg */
	    argtab[1].meltbp_aptr =
	      (melt_ptr_t *) & /*_.SEXPW__V31*/ meltfptr[28];
	    /*_.STRING4OUT__V32*/ meltfptr[29] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!STRING4OUT */ meltfrout->tabval[10])),
			  (melt_ptr_t) (( /*!DISCR_STRING */ meltfrout->
					 tabval[11])),
			  (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab,
			  "", (union meltparam_un *) 0);
	  }
	  ;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:7241:/ locexp");
	    melt_warning_str (0, (melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			      ("suspicious very heavy (DEBUG ....)"),
			      (melt_ptr_t) ( /*_.STRING4OUT__V32*/
					    meltfptr[29]));
	  }
	  ;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7238:/ clear");
	     /*clear *//*_.STRING4OUT__V32*/ meltfptr[29] = 0;
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-macro.melt:7235:/ clear");
	   /*clear *//*_.SEXPW__V31*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_#NBSEPW__L9*/ meltfnum[8] = 0;
    /*^clear */
	   /*clear *//*_#gtI__L10*/ meltfnum[9] = 0;
    /*^clear */
	   /*clear *//*_#OR___L11*/ meltfnum[10] = 0;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_STRING__L13*/ meltfnum[11] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.DFILNAM__V17*/ meltfptr[16])) ==
       MELTOBMAG_STRING);;
    /*^compute */
 /*_#NOT__L14*/ meltfnum[8] =
      (!( /*_#IS_STRING__L13*/ meltfnum[11]));;
    MELT_LOCATION ("warmelt-macro.melt:7245:/ cond");
    /*cond */ if ( /*_#NOT__L14*/ meltfnum[8])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7247:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			      ("(DEBUG ...) used without file location"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7248:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:7248:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-macro.melt:7246:/ quasiblock");


	  /*_.PROGN___V35*/ meltfptr[34] = /*_.RETURN___V34*/ meltfptr[28];;
	  /*^compute */
	  /*_._IF___V33*/ meltfptr[29] = /*_.PROGN___V35*/ meltfptr[34];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7245:/ clear");
	     /*clear *//*_.RETURN___V34*/ meltfptr[28] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V35*/ meltfptr[34] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V33*/ meltfptr[29] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_A__L15*/ meltfnum[9] =
      melt_is_instance_of ((melt_ptr_t)
			   ( /*_.DEBUGFUNBIND__V21*/ meltfptr[19]),
			   (melt_ptr_t) (( /*!CLASS_FUNCTION_BINDING */
					  meltfrout->tabval[13])));;
    MELT_LOCATION ("warmelt-macro.melt:7249:/ cond");
    /*cond */ if ( /*_#IS_A__L15*/ meltfnum[9])	/*then */
      {
	/*^cond.then */
	/*_#OR___L16*/ meltfnum[10] = /*_#IS_A__L15*/ meltfnum[9];;
      }
    else
      {
	MELT_LOCATION ("warmelt-macro.melt:7249:/ cond.else");

	/*^block */
	/*anyblock */
	{

   /*_#IS_A__L17*/ meltfnum[16] =
	    melt_is_instance_of ((melt_ptr_t)
				 ( /*_.DEBUGFUNBIND__V21*/ meltfptr[19]),
				 (melt_ptr_t) (( /*!CLASS_VALUE_BINDING */
						meltfrout->tabval[12])));;
	  /*^compute */
	  /*_#OR___L16*/ meltfnum[10] = /*_#IS_A__L17*/ meltfnum[16];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7249:/ clear");
	     /*clear *//*_#IS_A__L17*/ meltfnum[16] = 0;
	}
	;
      }
    ;
 /*_#NOT__L18*/ meltfnum[16] =
      (!( /*_#OR___L16*/ meltfnum[10]));;
    MELT_LOCATION ("warmelt-macro.melt:7249:/ cond");
    /*cond */ if ( /*_#NOT__L18*/ meltfnum[16])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7252:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			      ("(DEBUG ...) used in context with bad MELT_DEBUG_FUN"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7253:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:7253:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-macro.melt:7251:/ quasiblock");


	  /*_.PROGN___V38*/ meltfptr[37] = /*_.RETURN___V37*/ meltfptr[34];;
	  /*^compute */
	  /*_._IF___V36*/ meltfptr[28] = /*_.PROGN___V38*/ meltfptr[37];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7249:/ clear");
	     /*clear *//*_.RETURN___V37*/ meltfptr[34] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V38*/ meltfptr[37] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V36*/ meltfptr[28] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-macro.melt:7254:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_A__L19*/ meltfnum[18] =
      melt_is_instance_of ((melt_ptr_t)
			   ( /*_.THECOUNTBIND__V22*/ meltfptr[17]),
			   (melt_ptr_t) (( /*!CLASS_PRIMITIVE_BINDING */
					  meltfrout->tabval[14])));;
    MELT_LOCATION ("warmelt-macro.melt:7257:/ cond");
    /*cond */ if ( /*_#IS_A__L19*/ meltfnum[18])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-macro.melt:7258:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.THECOUNTBIND__V22*/
					       meltfptr[17]),
					      (melt_ptr_t) (( /*!CLASS_PRIMITIVE_BINDING */ meltfrout->tabval[14])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) ( /*_.THECOUNTBIND__V22*/ meltfptr[17])
		  /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "PBIND_PRIMITIVE");
     /*_.PBIND_PRIMITIVE__V40*/ meltfptr[37] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.PBIND_PRIMITIVE__V40*/ meltfptr[37] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_.THECOUNTPRIM__V39*/ meltfptr[34] =
	    /*_.PBIND_PRIMITIVE__V40*/ meltfptr[37];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7257:/ clear");
	     /*clear *//*_.PBIND_PRIMITIVE__V40*/ meltfptr[37] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_A__L20*/ meltfnum[19] =
	    melt_is_instance_of ((melt_ptr_t)
				 ( /*_.THECOUNTBIND__V22*/ meltfptr[17]),
				 (melt_ptr_t) (( /*!CLASS_VALUE_BINDING */
						meltfrout->tabval[12])));;
	  MELT_LOCATION ("warmelt-macro.melt:7259:/ cond");
	  /*cond */ if ( /*_#IS_A__L20*/ meltfnum[19])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-macro.melt:7260:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    ( /*_.THECOUNTBIND__V22*/
						     meltfptr[17]),
						    (melt_ptr_t) (( /*!CLASS_VALUE_BINDING */ meltfrout->tabval[12])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^getslot */
		    {
		      melt_ptr_t slot = NULL, obj = NULL;
		      obj =
			(melt_ptr_t) ( /*_.THECOUNTBIND__V22*/ meltfptr[17])
			/*=obj*/ ;
		      melt_object_get_field (slot, obj, 1, "VBIND_VALUE");
       /*_.VBIND_VALUE__V42*/ meltfptr[41] = slot;
		    };
		    ;
		  }
		else
		  {		/*^cond.else */

      /*_.VBIND_VALUE__V42*/ meltfptr[41] = NULL;;
		  }
		;
		/*^compute */
		/*_._IFELSE___V41*/ meltfptr[37] =
		  /*_.VBIND_VALUE__V42*/ meltfptr[41];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7259:/ clear");
	       /*clear *//*_.VBIND_VALUE__V42*/ meltfptr[41] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-macro.melt:7262:/ locexp");
		  /* error_plain */
		    melt_error_str ((melt_ptr_t)
				    ( /*_.LOC__V16*/ meltfptr[15]),
				    ("(DEBUG ...) used in context with bad THE_MELTCALLCOUNT"),
				    (melt_ptr_t) 0);
		}
		;
		MELT_LOCATION ("warmelt-macro.melt:7263:/ quasiblock");


     /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

		{
		  MELT_LOCATION ("warmelt-macro.melt:7263:/ locexp");
		  /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
		  if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
		    melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
		  ;
		}
		;
		/*^finalreturn */
		;
		/*finalret */ goto meltlabend_rout;
		MELT_LOCATION ("warmelt-macro.melt:7261:/ quasiblock");


		/*_.PROGN___V44*/ meltfptr[43] =
		  /*_.RETURN___V43*/ meltfptr[41];;
		/*^compute */
		/*_._IFELSE___V41*/ meltfptr[37] =
		  /*_.PROGN___V44*/ meltfptr[43];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7259:/ clear");
	       /*clear *//*_.RETURN___V43*/ meltfptr[41] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V44*/ meltfptr[43] = 0;
	      }
	      ;
	    }
	  ;
	  /*_.THECOUNTPRIM__V39*/ meltfptr[34] =
	    /*_._IFELSE___V41*/ meltfptr[37];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7257:/ clear");
	     /*clear *//*_#IS_A__L20*/ meltfnum[19] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V41*/ meltfptr[37] = 0;
	}
	;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7267:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_2_st
      {
	struct MELT_MULTIPLE_STRUCT (0) rtup_0__TUPLREC__x11;
	long meltletrec_2_endgap;
      } *meltletrec_2_ptr = 0;
      meltletrec_2_ptr =
	(struct meltletrec_2_st *)
	meltgc_allocate (sizeof (struct meltletrec_2_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x11 */
 /*_.TUPLREC___V46*/ meltfptr[43] =
	(melt_ptr_t) & meltletrec_2_ptr->rtup_0__TUPLREC__x11;
      meltletrec_2_ptr->rtup_0__TUPLREC__x11.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_2_ptr->rtup_0__TUPLREC__x11.nbval = 0;


      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V46*/ meltfptr[43]);
      ;
      /*_.TUPLE___V45*/ meltfptr[41] = /*_.TUPLREC___V46*/ meltfptr[43];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7267:/ clear");
	    /*clear *//*_.TUPLREC___V46*/ meltfptr[43] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V46*/ meltfptr[43] = 0;
    }				/*end multiallocblock */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7264:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_PRIMITIVE */
					     meltfrout->tabval[15])), (4),
			      "CLASS_SOURCE_PRIMITIVE");
  /*_.INST__V48*/ meltfptr[43] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V48*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V48*/ meltfptr[43]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SPRIM_OPER",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V48*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V48*/ meltfptr[43]), (3),
			  ( /*_.THECOUNTPRIM__V39*/ meltfptr[34]),
			  "SPRIM_OPER");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SARGOP_ARGS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V48*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V48*/ meltfptr[43]), (2),
			  ( /*_.TUPLE___V45*/ meltfptr[41]), "SARGOP_ARGS");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V48*/ meltfptr[43],
				  "newly made instance");
    ;
    /*_.DCOUNT__V47*/ meltfptr[37] = /*_.INST__V48*/ meltfptr[43];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7269:/ locexp");
      meltgc_append_list ((melt_ptr_t) ( /*_.DBGARGLIST__V28*/ meltfptr[27]),
			  (melt_ptr_t) ( /*_.DCOUNT__V47*/ meltfptr[37]));
    }
    ;

    MELT_LOCATION ("warmelt-macro.melt:7254:/ clear");
	   /*clear *//*_#IS_A__L19*/ meltfnum[18] = 0;
    /*^clear */
	   /*clear *//*_.THECOUNTPRIM__V39*/ meltfptr[34] = 0;
    /*^clear */
	   /*clear *//*_.TUPLE___V45*/ meltfptr[41] = 0;
    /*^clear */
	   /*clear *//*_.DCOUNT__V47*/ meltfptr[37] = 0;

    {
      MELT_LOCATION ("warmelt-macro.melt:7271:/ locexp");
      meltgc_append_list ((melt_ptr_t) ( /*_.DBGARGLIST__V28*/ meltfptr[27]),
			  (melt_ptr_t) ( /*_.DFILNAM__V17*/ meltfptr[16]));
    }
    ;
 /*_.MAKE_INTEGERBOX__V49*/ meltfptr[34] =
      (meltgc_new_int
       ((meltobject_ptr_t)
	(( /*!DISCR_CONSTANT_INTEGER */ meltfrout->tabval[16])),
	( /*_#DLINE__L8*/ meltfnum[1])));;

    {
      MELT_LOCATION ("warmelt-macro.melt:7272:/ locexp");
      meltgc_append_list ((melt_ptr_t) ( /*_.DBGARGLIST__V28*/ meltfptr[27]),
			  (melt_ptr_t) ( /*_.MAKE_INTEGERBOX__V49*/
					meltfptr[34]));
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7273:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.XARGLIST__V27*/ meltfptr[26];
      /*_.LIST_APPEND2LIST__V50*/ meltfptr[41] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_APPEND2LIST */ meltfrout->tabval[17])),
		    (melt_ptr_t) ( /*_.DBGARGLIST__V28*/ meltfptr[27]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7274:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L21*/ meltfnum[19] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7274:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L21*/ meltfnum[19])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L22*/ meltfnum[18] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7274:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[18];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7274;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_debug dbgarglist";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.DBGARGLIST__V28*/ meltfptr[27];
	      /*_.MELT_DEBUG_FUN__V52*/ meltfptr[51] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V51*/ meltfptr[37] =
	      /*_.MELT_DEBUG_FUN__V52*/ meltfptr[51];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7274:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L22*/ meltfnum[18] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V52*/ meltfptr[51] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V51*/ meltfptr[37] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7274:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L21*/ meltfnum[19] = 0;
      /*^clear */
	     /*clear *//*_._IF___V51*/ meltfptr[37] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7275:/ quasiblock");


 /*_#NBDBGARG__L23*/ meltfnum[18] =
      (melt_list_length
       ((melt_ptr_t) ( /*_.DBGARGLIST__V28*/ meltfptr[27])));;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_A__L24*/ meltfnum[19] =
      melt_is_instance_of ((melt_ptr_t)
			   ( /*_.THENEEDDBGBIND__V23*/ meltfptr[22]),
			   (melt_ptr_t) (( /*!CLASS_PRIMITIVE_BINDING */
					  meltfrout->tabval[14])));;
    MELT_LOCATION ("warmelt-macro.melt:7279:/ cond");
    /*cond */ if ( /*_#IS_A__L24*/ meltfnum[19])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-macro.melt:7280:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.THENEEDDBGBIND__V23*/
					       meltfptr[22]),
					      (melt_ptr_t) (( /*!CLASS_PRIMITIVE_BINDING */ meltfrout->tabval[14])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) ( /*_.THENEEDDBGBIND__V23*/ meltfptr[22])
		  /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "PBIND_PRIMITIVE");
     /*_.PBIND_PRIMITIVE__V55*/ meltfptr[54] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.PBIND_PRIMITIVE__V55*/ meltfptr[54] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_.THENEEDDBG__V54*/ meltfptr[37] =
	    /*_.PBIND_PRIMITIVE__V55*/ meltfptr[54];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7279:/ clear");
	     /*clear *//*_.PBIND_PRIMITIVE__V55*/ meltfptr[54] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_A__L25*/ meltfnum[24] =
	    melt_is_instance_of ((melt_ptr_t)
				 ( /*_.THENEEDDBGBIND__V23*/ meltfptr[22]),
				 (melt_ptr_t) (( /*!CLASS_VALUE_BINDING */
						meltfrout->tabval[12])));;
	  MELT_LOCATION ("warmelt-macro.melt:7281:/ cond");
	  /*cond */ if ( /*_#IS_A__L25*/ meltfnum[24])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-macro.melt:7282:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    ( /*_.THENEEDDBGBIND__V23*/ meltfptr[22]),
						    (melt_ptr_t) (( /*!CLASS_VALUE_BINDING */ meltfrout->tabval[12])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^getslot */
		    {
		      melt_ptr_t slot = NULL, obj = NULL;
		      obj =
			(melt_ptr_t) ( /*_.THENEEDDBGBIND__V23*/ meltfptr[22])
			/*=obj*/ ;
		      melt_object_get_field (slot, obj, 1, "VBIND_VALUE");
       /*_.VBIND_VALUE__V57*/ meltfptr[56] = slot;
		    };
		    ;
		  }
		else
		  {		/*^cond.else */

      /*_.VBIND_VALUE__V57*/ meltfptr[56] = NULL;;
		  }
		;
		/*^compute */
		/*_._IFELSE___V56*/ meltfptr[54] =
		  /*_.VBIND_VALUE__V57*/ meltfptr[56];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7281:/ clear");
	       /*clear *//*_.VBIND_VALUE__V57*/ meltfptr[56] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-macro.melt:7284:/ locexp");
		  /* error_plain */
		    melt_error_str ((melt_ptr_t)
				    ( /*_.LOC__V16*/ meltfptr[15]),
				    ("(DEBUG ...) used in context with bad MELT_NEED_DBG"),
				    (melt_ptr_t) 0);
		}
		;
		MELT_LOCATION ("warmelt-macro.melt:7285:/ quasiblock");


     /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

		{
		  MELT_LOCATION ("warmelt-macro.melt:7285:/ locexp");
		  /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
		  if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
		    melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
		  ;
		}
		;
		/*^finalreturn */
		;
		/*finalret */ goto meltlabend_rout;
		MELT_LOCATION ("warmelt-macro.melt:7283:/ quasiblock");


		/*_.PROGN___V59*/ meltfptr[58] =
		  /*_.RETURN___V58*/ meltfptr[56];;
		/*^compute */
		/*_._IFELSE___V56*/ meltfptr[54] =
		  /*_.PROGN___V59*/ meltfptr[58];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7281:/ clear");
	       /*clear *//*_.RETURN___V58*/ meltfptr[56] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V59*/ meltfptr[58] = 0;
	      }
	      ;
	    }
	  ;
	  /*_.THENEEDDBG__V54*/ meltfptr[37] =
	    /*_._IFELSE___V56*/ meltfptr[54];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7279:/ clear");
	     /*clear *//*_#IS_A__L25*/ meltfnum[24] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V56*/ meltfptr[54] = 0;
	}
	;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7289:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_3_st
      {
	struct MELT_MULTIPLE_STRUCT (1) rtup_0__TUPLREC__x12;
	long meltletrec_3_endgap;
      } *meltletrec_3_ptr = 0;
      meltletrec_3_ptr =
	(struct meltletrec_3_st *)
	meltgc_allocate (sizeof (struct meltletrec_3_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x12 */
 /*_.TUPLREC___V61*/ meltfptr[58] =
	(melt_ptr_t) & meltletrec_3_ptr->rtup_0__TUPLREC__x12;
      meltletrec_3_ptr->rtup_0__TUPLREC__x12.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_3_ptr->rtup_0__TUPLREC__x12.nbval = 1;


      /*^putuple */
      /*putupl#20 */
      melt_assertmsg ("putupl [:7289] #20 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V61*/ meltfptr[58]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7289] #20 checkoff",
		      (0 >= 0
		       && 0 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V61*/
					      meltfptr[58]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V61*/ meltfptr[58]))->tabval[0] =
	(melt_ptr_t) (( /*!konst_18 */ meltfrout->tabval[18]));
      ;
      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V61*/ meltfptr[58]);
      ;
      /*_.TUPLE___V60*/ meltfptr[56] = /*_.TUPLREC___V61*/ meltfptr[58];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7289:/ clear");
	    /*clear *//*_.TUPLREC___V61*/ meltfptr[58] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V61*/ meltfptr[58] = 0;
    }				/*end multiallocblock */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7286:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_PRIMITIVE */
					     meltfrout->tabval[15])), (4),
			      "CLASS_SOURCE_PRIMITIVE");
  /*_.INST__V63*/ meltfptr[58] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V63*/ meltfptr[58])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V63*/ meltfptr[58]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SPRIM_OPER",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V63*/ meltfptr[58])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V63*/ meltfptr[58]), (3),
			  ( /*_.THENEEDDBG__V54*/ meltfptr[37]),
			  "SPRIM_OPER");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SARGOP_ARGS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V63*/ meltfptr[58])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V63*/ meltfptr[58]), (2),
			  ( /*_.TUPLE___V60*/ meltfptr[56]), "SARGOP_ARGS");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V63*/ meltfptr[58],
				  "newly made instance");
    ;
    /*_.DNEEDBG__V62*/ meltfptr[54] = /*_.INST__V63*/ meltfptr[58];;

    MELT_CHECK_SIGNAL ();
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7294:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!DISCR_MULTIPLE */ meltfrout->tabval[22]);
      /*_.LIST_TO_MULTIPLE__V64*/ meltfptr[63] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_TO_MULTIPLE */ meltfrout->tabval[21])),
		    (melt_ptr_t) ( /*_.DBGARGLIST__V28*/ meltfptr[27]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    MELT_LOCATION ("warmelt-macro.melt:7290:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_APPLY */
					     meltfrout->tabval[19])), (4),
			      "CLASS_SOURCE_APPLY");
  /*_.INST__V66*/ meltfptr[65] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V66*/ meltfptr[65])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V66*/ meltfptr[65]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SAPP_FUN",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V66*/ meltfptr[65])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V66*/ meltfptr[65]), (3),
			  (( /*!konst_4_MELT_DEBUG_FUN */ meltfrout->
			    tabval[4])), "SAPP_FUN");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SARGOP_ARGS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V66*/ meltfptr[65])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V66*/ meltfptr[65]), (2),
			  ( /*_.LIST_TO_MULTIPLE__V64*/ meltfptr[63]),
			  "SARGOP_ARGS");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V66*/ meltfptr[65],
				  "newly made instance");
    ;
    /*_.DCALL__V65*/ meltfptr[64] = /*_.INST__V66*/ meltfptr[65];;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_A__L26*/ meltfnum[24] =
      melt_is_instance_of ((melt_ptr_t)
			   ( /*_.THEINCRDBGBIND__V24*/ meltfptr[23]),
			   (melt_ptr_t) (( /*!CLASS_PRIMITIVE_BINDING */
					  meltfrout->tabval[14])));;
    MELT_LOCATION ("warmelt-macro.melt:7297:/ cond");
    /*cond */ if ( /*_#IS_A__L26*/ meltfnum[24])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-macro.melt:7298:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.THEINCRDBGBIND__V24*/
					       meltfptr[23]),
					      (melt_ptr_t) (( /*!CLASS_PRIMITIVE_BINDING */ meltfrout->tabval[14])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) ( /*_.THEINCRDBGBIND__V24*/ meltfptr[23])
		  /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "PBIND_PRIMITIVE");
     /*_.PBIND_PRIMITIVE__V68*/ meltfptr[67] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.PBIND_PRIMITIVE__V68*/ meltfptr[67] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_.THEINCRDBG__V67*/ meltfptr[66] =
	    /*_.PBIND_PRIMITIVE__V68*/ meltfptr[67];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7297:/ clear");
	     /*clear *//*_.PBIND_PRIMITIVE__V68*/ meltfptr[67] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_A__L27*/ meltfnum[26] =
	    melt_is_instance_of ((melt_ptr_t)
				 ( /*_.THEINCRDBGBIND__V24*/ meltfptr[23]),
				 (melt_ptr_t) (( /*!CLASS_VALUE_BINDING */
						meltfrout->tabval[12])));;
	  MELT_LOCATION ("warmelt-macro.melt:7299:/ cond");
	  /*cond */ if ( /*_#IS_A__L27*/ meltfnum[26])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-macro.melt:7300:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    ( /*_.THEINCRDBGBIND__V24*/ meltfptr[23]),
						    (melt_ptr_t) (( /*!CLASS_VALUE_BINDING */ meltfrout->tabval[12])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^getslot */
		    {
		      melt_ptr_t slot = NULL, obj = NULL;
		      obj =
			(melt_ptr_t) ( /*_.THEINCRDBGBIND__V24*/ meltfptr[23])
			/*=obj*/ ;
		      melt_object_get_field (slot, obj, 1, "VBIND_VALUE");
       /*_.VBIND_VALUE__V70*/ meltfptr[69] = slot;
		    };
		    ;
		  }
		else
		  {		/*^cond.else */

      /*_.VBIND_VALUE__V70*/ meltfptr[69] = NULL;;
		  }
		;
		/*^compute */
		/*_._IFELSE___V69*/ meltfptr[67] =
		  /*_.VBIND_VALUE__V70*/ meltfptr[69];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7299:/ clear");
	       /*clear *//*_.VBIND_VALUE__V70*/ meltfptr[69] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-macro.melt:7302:/ locexp");
		  /* error_plain */
		    melt_error_str ((melt_ptr_t)
				    ( /*_.LOC__V16*/ meltfptr[15]),
				    ("(DEBUG ...) used in context with bad MELT_INCREMENT_DBGCOUNTER"),
				    (melt_ptr_t) 0);
		}
		;
		MELT_LOCATION ("warmelt-macro.melt:7305:/ quasiblock");


     /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

		{
		  MELT_LOCATION ("warmelt-macro.melt:7305:/ locexp");
		  /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
		  if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
		    melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
		  ;
		}
		;
		/*^finalreturn */
		;
		/*finalret */ goto meltlabend_rout;
		MELT_LOCATION ("warmelt-macro.melt:7301:/ quasiblock");


		/*_.PROGN___V72*/ meltfptr[71] =
		  /*_.RETURN___V71*/ meltfptr[69];;
		/*^compute */
		/*_._IFELSE___V69*/ meltfptr[67] =
		  /*_.PROGN___V72*/ meltfptr[71];;
		/*epilog */

		MELT_LOCATION ("warmelt-macro.melt:7299:/ clear");
	       /*clear *//*_.RETURN___V71*/ meltfptr[69] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V72*/ meltfptr[71] = 0;
	      }
	      ;
	    }
	  ;
	  /*_.THEINCRDBG__V67*/ meltfptr[66] =
	    /*_._IFELSE___V69*/ meltfptr[67];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7297:/ clear");
	     /*clear *//*_#IS_A__L27*/ meltfnum[26] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V69*/ meltfptr[67] = 0;
	}
	;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7310:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_4_st
      {
	struct MELT_MULTIPLE_STRUCT (0) rtup_0__TUPLREC__x13;
	long meltletrec_4_endgap;
      } *meltletrec_4_ptr = 0;
      meltletrec_4_ptr =
	(struct meltletrec_4_st *)
	meltgc_allocate (sizeof (struct meltletrec_4_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x13 */
 /*_.TUPLREC___V74*/ meltfptr[71] =
	(melt_ptr_t) & meltletrec_4_ptr->rtup_0__TUPLREC__x13;
      meltletrec_4_ptr->rtup_0__TUPLREC__x13.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_4_ptr->rtup_0__TUPLREC__x13.nbval = 0;


      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V74*/ meltfptr[71]);
      ;
      /*_.TUPLE___V73*/ meltfptr[69] = /*_.TUPLREC___V74*/ meltfptr[71];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7310:/ clear");
	    /*clear *//*_.TUPLREC___V74*/ meltfptr[71] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V74*/ meltfptr[71] = 0;
    }				/*end multiallocblock */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7306:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_PRIMITIVE */
					     meltfrout->tabval[15])), (4),
			      "CLASS_SOURCE_PRIMITIVE");
  /*_.INST__V76*/ meltfptr[71] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V76*/ meltfptr[71])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V76*/ meltfptr[71]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SPRIM_OPER",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V76*/ meltfptr[71])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V76*/ meltfptr[71]), (3),
			  ( /*_.THEINCRDBG__V67*/ meltfptr[66]),
			  "SPRIM_OPER");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SARGOP_ARGS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V76*/ meltfptr[71])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V76*/ meltfptr[71]), (2),
			  ( /*_.TUPLE___V73*/ meltfptr[69]), "SARGOP_ARGS");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V76*/ meltfptr[71],
				  "newly made instance");
    ;
    /*_.DINCRDBG__V75*/ meltfptr[67] = /*_.INST__V76*/ meltfptr[71];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7311:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_IF */
					     meltfrout->tabval[23])), (4),
			      "CLASS_SOURCE_IF");
  /*_.INST__V78*/ meltfptr[77] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V78*/ meltfptr[77])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V78*/ meltfptr[77]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIF_TEST",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V78*/ meltfptr[77])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V78*/ meltfptr[77]), (2),
			  ( /*_.DNEEDBG__V62*/ meltfptr[54]), "SIF_TEST");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIF_THEN",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V78*/ meltfptr[77])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V78*/ meltfptr[77]), (3),
			  ( /*_.DCALL__V65*/ meltfptr[64]), "SIF_THEN");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V78*/ meltfptr[77],
				  "newly made instance");
    ;
    /*_.DIFDBG__V77*/ meltfptr[76] = /*_.INST__V78*/ meltfptr[77];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7320:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_5_st
      {
	struct MELT_MULTIPLE_STRUCT (0) rtup_0__TUPLREC__x14;
	long meltletrec_5_endgap;
      } *meltletrec_5_ptr = 0;
      meltletrec_5_ptr =
	(struct meltletrec_5_st *)
	meltgc_allocate (sizeof (struct meltletrec_5_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x14 */
 /*_.TUPLREC___V80*/ meltfptr[79] =
	(melt_ptr_t) & meltletrec_5_ptr->rtup_0__TUPLREC__x14;
      meltletrec_5_ptr->rtup_0__TUPLREC__x14.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_5_ptr->rtup_0__TUPLREC__x14.nbval = 0;


      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V80*/ meltfptr[79]);
      ;
      /*_.TUPLE___V79*/ meltfptr[78] = /*_.TUPLREC___V80*/ meltfptr[79];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7320:/ clear");
	    /*clear *//*_.TUPLREC___V80*/ meltfptr[79] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V80*/ meltfptr[79] = 0;
    }				/*end multiallocblock */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7316:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_PRIMITIVE */
					     meltfrout->tabval[15])), (4),
			      "CLASS_SOURCE_PRIMITIVE");
  /*_.INST__V82*/ meltfptr[81] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V82*/ meltfptr[81])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V82*/ meltfptr[81]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SPRIM_OPER",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V82*/ meltfptr[81])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V82*/ meltfptr[81]), (3),
			  (( /*!VOID */ meltfrout->tabval[24])),
			  "SPRIM_OPER");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SARGOP_ARGS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V82*/ meltfptr[81])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V82*/ meltfptr[81]), (2),
			  ( /*_.TUPLE___V79*/ meltfptr[78]), "SARGOP_ARGS");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V82*/ meltfptr[81],
				  "newly made instance");
    ;
    /*_.DVOID__V81*/ meltfptr[79] = /*_.INST__V82*/ meltfptr[81];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7324:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_6_st
      {
	struct MELT_MULTIPLE_STRUCT (3) rtup_0__TUPLREC__x15;
	long meltletrec_6_endgap;
      } *meltletrec_6_ptr = 0;
      meltletrec_6_ptr =
	(struct meltletrec_6_st *)
	meltgc_allocate (sizeof (struct meltletrec_6_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x15 */
 /*_.TUPLREC___V84*/ meltfptr[83] =
	(melt_ptr_t) & meltletrec_6_ptr->rtup_0__TUPLREC__x15;
      meltletrec_6_ptr->rtup_0__TUPLREC__x15.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_6_ptr->rtup_0__TUPLREC__x15.nbval = 3;


      /*^putuple */
      /*putupl#21 */
      melt_assertmsg ("putupl [:7324] #21 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V84*/ meltfptr[83]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7324] #21 checkoff",
		      (0 >= 0
		       && 0 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V84*/
					      meltfptr[83]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V84*/ meltfptr[83]))->tabval[0] =
	(melt_ptr_t) ( /*_.DINCRDBG__V75*/ meltfptr[67]);
      ;
      /*^putuple */
      /*putupl#22 */
      melt_assertmsg ("putupl [:7324] #22 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V84*/ meltfptr[83]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7324] #22 checkoff",
		      (1 >= 0
		       && 1 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V84*/
					      meltfptr[83]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V84*/ meltfptr[83]))->tabval[1] =
	(melt_ptr_t) ( /*_.DIFDBG__V77*/ meltfptr[76]);
      ;
      /*^putuple */
      /*putupl#23 */
      melt_assertmsg ("putupl [:7324] #23 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V84*/ meltfptr[83]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:7324] #23 checkoff",
		      (2 >= 0
		       && 2 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V84*/
					      meltfptr[83]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V84*/ meltfptr[83]))->tabval[2] =
	(melt_ptr_t) ( /*_.DVOID__V81*/ meltfptr[79]);
      ;
      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V84*/ meltfptr[83]);
      ;
      /*_.TUPLE___V83*/ meltfptr[82] = /*_.TUPLREC___V84*/ meltfptr[83];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7324:/ clear");
	    /*clear *//*_.TUPLREC___V84*/ meltfptr[83] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V84*/ meltfptr[83] = 0;
    }				/*end multiallocblock */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7321:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_PROGN */
					     meltfrout->tabval[25])), (3),
			      "CLASS_SOURCE_PROGN");
  /*_.INST__V86*/ meltfptr[85] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V86*/ meltfptr[85])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V86*/ meltfptr[85]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SPROGN_BODY",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V86*/ meltfptr[85])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V86*/ meltfptr[85]), (2),
			  ( /*_.TUPLE___V83*/ meltfptr[82]), "SPROGN_BODY");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V86*/ meltfptr[85],
				  "newly made instance");
    ;
    /*_.DPROGN__V85*/ meltfptr[83] = /*_.INST__V86*/ meltfptr[85];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7326:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_CPPIF */
					     meltfrout->tabval[26])), (5),
			      "CLASS_SOURCE_CPPIF");
  /*_.INST__V88*/ meltfptr[87] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V88*/ meltfptr[87])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V88*/ meltfptr[87]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIFP_COND",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V88*/ meltfptr[87])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V88*/ meltfptr[87]), (2),
			  (( /*!konst_27_MELT_HAVE_DEBUG */ meltfrout->
			    tabval[27])), "SIFP_COND");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIFP_THEN",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V88*/ meltfptr[87])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V88*/ meltfptr[87]), (3),
			  ( /*_.DPROGN__V85*/ meltfptr[83]), "SIFP_THEN");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SIFP_ELSE",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V88*/ meltfptr[87])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V88*/ meltfptr[87]), (4),
			  ( /*_.DVOID__V81*/ meltfptr[79]), "SIFP_ELSE");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V88*/ meltfptr[87],
				  "newly made instance");
    ;
    /*_.DCPPIF__V87*/ meltfptr[86] = /*_.INST__V88*/ meltfptr[87];;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#gteqI__L28*/ meltfnum[26] =
      (( /*_#NBDBGARG__L23*/ meltfnum[18]) >= (15));;
    MELT_LOCATION ("warmelt-macro.melt:7334:/ cond");
    /*cond */ if ( /*_#gteqI__L28*/ meltfnum[26])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-macro.melt:7335:/ quasiblock");



	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^apply */
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_long = /*_#NBDBGARG__L23*/ meltfnum[18];
	    /*_.STRNBARG__V89*/ meltfptr[88] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!STRING4OUT */ meltfrout->tabval[10])),
			  (melt_ptr_t) (( /*!DISCR_STRING */ meltfrout->
					 tabval[11])), (MELTBPARSTR_LONG ""),
			  argtab, "", (union meltparam_un *) 0);
	  }
	  ;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:7337:/ locexp");
	    melt_inform_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			     ("quite long (DEBUG ...), perhaps consider splitting it"),
			     (melt_ptr_t) ( /*_.STRNBARG__V89*/
					   meltfptr[88]));
	  }
	  ;

	  MELT_LOCATION ("warmelt-macro.melt:7335:/ clear");
	     /*clear *//*_.STRNBARG__V89*/ meltfptr[88] = 0;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7340:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L29*/ meltfnum[28] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7340:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L29*/ meltfnum[28])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L30*/ meltfnum[29] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7340:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L30*/ meltfnum[29];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7340;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_debug dcall=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.DCALL__V65*/ meltfptr[64];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " return dcppif=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.DCPPIF__V87*/ meltfptr[86];
	      /*_.MELT_DEBUG_FUN__V91*/ meltfptr[90] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V90*/ meltfptr[88] =
	      /*_.MELT_DEBUG_FUN__V91*/ meltfptr[90];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7340:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L30*/ meltfnum[29] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V91*/ meltfptr[90] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V90*/ meltfptr[88] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7340:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L29*/ meltfnum[28] = 0;
      /*^clear */
	     /*clear *//*_._IF___V90*/ meltfptr[88] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7341:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.DCPPIF__V87*/ meltfptr[86];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7341:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V53*/ meltfptr[51] = /*_.RETURN___V92*/ meltfptr[90];;

    MELT_LOCATION ("warmelt-macro.melt:7275:/ clear");
	   /*clear *//*_#NBDBGARG__L23*/ meltfnum[18] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L24*/ meltfnum[19] = 0;
    /*^clear */
	   /*clear *//*_.THENEEDDBG__V54*/ meltfptr[37] = 0;
    /*^clear */
	   /*clear *//*_.TUPLE___V60*/ meltfptr[56] = 0;
    /*^clear */
	   /*clear *//*_.DNEEDBG__V62*/ meltfptr[54] = 0;
    /*^clear */
	   /*clear *//*_.LIST_TO_MULTIPLE__V64*/ meltfptr[63] = 0;
    /*^clear */
	   /*clear *//*_.DCALL__V65*/ meltfptr[64] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L26*/ meltfnum[24] = 0;
    /*^clear */
	   /*clear *//*_.THEINCRDBG__V67*/ meltfptr[66] = 0;
    /*^clear */
	   /*clear *//*_.TUPLE___V73*/ meltfptr[69] = 0;
    /*^clear */
	   /*clear *//*_.DINCRDBG__V75*/ meltfptr[67] = 0;
    /*^clear */
	   /*clear *//*_.DIFDBG__V77*/ meltfptr[76] = 0;
    /*^clear */
	   /*clear *//*_.TUPLE___V79*/ meltfptr[78] = 0;
    /*^clear */
	   /*clear *//*_.DVOID__V81*/ meltfptr[79] = 0;
    /*^clear */
	   /*clear *//*_.TUPLE___V83*/ meltfptr[82] = 0;
    /*^clear */
	   /*clear *//*_.DPROGN__V85*/ meltfptr[83] = 0;
    /*^clear */
	   /*clear *//*_.DCPPIF__V87*/ meltfptr[86] = 0;
    /*^clear */
	   /*clear *//*_#gteqI__L28*/ meltfnum[26] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V92*/ meltfptr[90] = 0;
    /*_.LET___V14*/ meltfptr[12] = /*_.LET___V53*/ meltfptr[51];;

    MELT_LOCATION ("warmelt-macro.melt:7219:/ clear");
	   /*clear *//*_.CONT__V15*/ meltfptr[14] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V16*/ meltfptr[15] = 0;
    /*^clear */
	   /*clear *//*_#IS_MIXINT__L6*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.DFILNAM__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_#DLINE__L8*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_.DEBUGFUNBIND__V21*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.THECOUNTBIND__V22*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.THENEEDDBGBIND__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.THEINCRDBGBIND__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V25*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V26*/ meltfptr[25] = 0;
    /*^clear */
	   /*clear *//*_.XARGLIST__V27*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.DBGARGLIST__V28*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_#IS_STRING__L13*/ meltfnum[11] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L14*/ meltfnum[8] = 0;
    /*^clear */
	   /*clear *//*_._IF___V33*/ meltfptr[29] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L15*/ meltfnum[9] = 0;
    /*^clear */
	   /*clear *//*_#OR___L16*/ meltfnum[10] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L18*/ meltfnum[16] = 0;
    /*^clear */
	   /*clear *//*_._IF___V36*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_.MAKE_INTEGERBOX__V49*/ meltfptr[34] = 0;
    /*^clear */
	   /*clear *//*_.LIST_APPEND2LIST__V50*/ meltfptr[41] = 0;
    /*^clear */
	   /*clear *//*_.LET___V53*/ meltfptr[51] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7214:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V14*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7214:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V14*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_DEBUG", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_150_WARMELTmiMACRO_MEXPAND_DEBUG */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un *
						   meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un *
						   meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 28
    melt_ptr_t mcfr_varptr[28];
#define MELTFRAM_NBVARNUM 8
    long mcfr_varnum[8];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES_st
	 *) meltfirstargp_;
      /* use arguments meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 28; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES nbval 28*/
  meltfram__.mcfr_nbvar = 28 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_EXPORT_VALUES", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7359:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7360:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7360:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7360:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7360;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_export_values sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7360:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7360:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7361:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7361:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7361:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7361) ? (7361) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7361:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7362:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7362:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7362:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7362) ? (7362) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7362:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7363:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_CLOSURE__L5*/ meltfnum[1] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) ==
	 MELTOBMAG_CLOSURE);;
      MELT_LOCATION ("warmelt-macro.melt:7363:/ cond");
      /*cond */ if ( /*_#IS_CLOSURE__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7363:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mexpander"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7363) ? (7363) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7363:/ clear");
	     /*clear *//*_#IS_CLOSURE__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7364:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L6*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7364:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7364:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7364) ? (7364) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7364:/ clear");
	     /*clear *//*_#IS_OBJECT__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7365:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7366:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7367:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;
    MELT_LOCATION ("warmelt-macro.melt:7371:/ quasiblock");


    /*^newclosure */
		 /*newclosure *//*_.__V22*/ meltfptr[21] =
      (melt_ptr_t)
      meltgc_new_closure ((meltobject_ptr_t)
			  (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			  (meltroutine_ptr_t) (( /*!konst_7 */ meltfrout->
						tabval[7])), (4));
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V22*/ meltfptr[21])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 0 >= 0
		    && 0 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V22*/ meltfptr[21])));
    ((meltclosure_ptr_t) /*_.__V22*/ meltfptr[21])->tabval[0] =
      (melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]);
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V22*/ meltfptr[21])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 1 >= 0
		    && 1 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V22*/ meltfptr[21])));
    ((meltclosure_ptr_t) /*_.__V22*/ meltfptr[21])->tabval[1] =
      (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]);
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V22*/ meltfptr[21])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 2 >= 0
		    && 2 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V22*/ meltfptr[21])));
    ((meltclosure_ptr_t) /*_.__V22*/ meltfptr[21])->tabval[2] =
      (melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4]);
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V22*/ meltfptr[21])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 3 >= 0
		    && 3 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V22*/ meltfptr[21])));
    ((meltclosure_ptr_t) /*_.__V22*/ meltfptr[21])->tabval[3] =
      (melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]);
    ;
    /*_.LAMBDA___V21*/ meltfptr[20] = /*_.__V22*/ meltfptr[21];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7369:/ apply");
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!DISCR_MULTIPLE */ meltfrout->tabval[4]);
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.LAMBDA___V21*/ meltfptr[20];
      /*_.SYMBTUP__V23*/ meltfptr[22] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!PAIRLIST_TO_MULTIPLE */ meltfrout->tabval[3])),
		    (melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7379:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_EXPORT_VALUES */
					     meltfrout->tabval[8])), (3),
			      "CLASS_SOURCE_EXPORT_VALUES");
  /*_.INST__V25*/ meltfptr[24] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V25*/ meltfptr[24])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V25*/ meltfptr[24]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPORT_NAMES",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V25*/ meltfptr[24])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V25*/ meltfptr[24]), (2),
			  ( /*_.SYMBTUP__V23*/ meltfptr[22]),
			  "SEXPORT_NAMES");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V25*/ meltfptr[24],
				  "newly made instance");
    ;
    /*_.RES__V24*/ meltfptr[23] = /*_.INST__V25*/ meltfptr[24];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7383:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L7*/ meltfnum[1] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7383:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L7*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L8*/ meltfnum[0] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7383:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[0];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7383;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_export_values result res";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V24*/ meltfptr[23];
	      /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V26*/ meltfptr[25] =
	      /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7383:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L8*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V27*/ meltfptr[26] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V26*/ meltfptr[25] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7383:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V26*/ meltfptr[25] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7384:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V24*/ meltfptr[23];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7384:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V16*/ meltfptr[14] = /*_.RETURN___V28*/ meltfptr[26];;

    MELT_LOCATION ("warmelt-macro.melt:7365:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.LAMBDA___V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.SYMBTUP__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.RES__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V28*/ meltfptr[26] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7359:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7359:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_EXPORT_VALUES", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_151_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_152_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un * meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_152_WARMELTmiMACRO_melt =
    melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_152_WARMELTmiMACRO_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 6
    melt_ptr_t mcfr_varptr[6];
#define MELTFRAM_NBVARNUM 3
    long mcfr_varnum[3];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_152_WARMELTmiMACRO is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_152_WARMELTmiMACRO_st *) meltfirstargp_;
      /* use arguments meltrout_152_WARMELTmiMACRO output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 6; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_152_WARMELTmiMACRO nbval 6*/
  meltfram__.mcfr_nbvar = 6 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("**", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7371:/ getarg");
 /*_.S__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-macro.melt:7372:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*~ENV */ meltfclos->tabval[1]);
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & ( /*~MEXPANDER */ meltfclos->tabval[0]);
      /*^apply.arg */
      argtab[2].meltbp_aptr =
	(melt_ptr_t *) & ( /*~MODCTX */ meltfclos->tabval[2]);
      /*_.SYM__V4*/ meltfptr[3] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~MEXPANDER */ meltfclos->tabval[0])),
		    (melt_ptr_t) ( /*_.S__V2*/ meltfptr[1]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_NOT_A__L1*/ meltfnum[0] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.SYM__V4*/ meltfptr[3]),
			    (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					   tabval[0])));;
    MELT_LOCATION ("warmelt-macro.melt:7373:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-macro.melt:7375:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    {
	      /*^locexp */
	      /*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	      melt_dbgcounter++;
#endif
	      ;
	    }
	    ;

	    MELT_CHECK_SIGNAL ();
	    ;
     /*_#MELT_NEED_DBG__L2*/ meltfnum[1] =
	      /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	      ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-macro.melt:7375:/ cond");
	    /*cond */ if ( /*_#MELT_NEED_DBG__L2*/ meltfnum[1])	/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

       /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] =
		    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		    meltcallcount	/* melt_callcount debugging */
#else
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-macro.melt:7375:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[5];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-macro.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 7375;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "mexpand_export_values bad sym";
		    /*^apply.arg */
		    argtab[4].meltbp_aptr =
		      (melt_ptr_t *) & /*_.SYM__V4*/ meltfptr[3];
		    /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =
		      melt_apply ((meltclosure_ptr_t)
				  (( /*!MELT_DEBUG_FUN */ meltfrout->
				    tabval[1])),
				  (melt_ptr_t) (( /*nil */ NULL)),
				  (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR ""), argtab, "",
				  (union meltparam_un *) 0);
		  }
		  ;
		  /*_._IF___V5*/ meltfptr[4] =
		    /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-macro.melt:7375:/ clear");
		 /*clear *//*_#MELT_CALLCOUNT__L3*/ meltfnum[2] = 0;
		  /*^clear */
		 /*clear *//*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

      /*_._IF___V5*/ meltfptr[4] = NULL;;
	      }
	    ;

	    {
	      MELT_LOCATION ("warmelt-macro.melt:7375:/ locexp");
	      /*void */ (void) 0;
	    }
	    ;
	    /*^quasiblock */


	    /*epilog */

	    /*^clear */
	       /*clear *//*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_._IF___V5*/ meltfptr[4] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*^block */
	  /*anyblock */
	  {


	    {
	      /*^locexp */
	      /*void */ (void) 0;
	    }
	    ;
	    /*epilog */
	  }

#endif /*MELT_HAVE_DEBUG */
	  ;

	  {
	    MELT_LOCATION ("warmelt-macro.melt:7376:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t)
			      (( /*~LOC */ meltfclos->tabval[3])),
			      ("(EXPORT_VALUES <sym>...) expecting symbol"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-macro.melt:7374:/ quasiblock");


	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*_.LET___V3*/ meltfptr[2] = /*_.SYM__V4*/ meltfptr[3];;

    MELT_LOCATION ("warmelt-macro.melt:7372:/ clear");
	   /*clear *//*_.SYM__V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L1*/ meltfnum[0] = 0;
    MELT_LOCATION ("warmelt-macro.melt:7371:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7371:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.LET___V3*/ meltfptr[2] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("**", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_152_WARMELTmiMACRO_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_152_WARMELTmiMACRO */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un *
						  meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un *
						  meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 43
    melt_ptr_t mcfr_varptr[43];
#define MELTFRAM_NBVARNUM 11
    long mcfr_varnum[11];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO_st
	 *) meltfirstargp_;
      /* use arguments meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 43; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO nbval 43*/
  meltfram__.mcfr_nbvar = 43 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_EXPORT_MACRO", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7397:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7398:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7398:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7398:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7398;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_export_macro sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7398:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7398:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7399:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7399:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7399:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7399) ? (7399) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7399:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7400:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7400:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7400:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7400) ? (7400) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7400:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7401:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_CLOSURE__L5*/ meltfnum[1] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) ==
	 MELTOBMAG_CLOSURE);;
      MELT_LOCATION ("warmelt-macro.melt:7401:/ cond");
      /*cond */ if ( /*_#IS_CLOSURE__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7401:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mexpander"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7401) ? (7401) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7401:/ clear");
	     /*clear *//*_#IS_CLOSURE__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7402:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L6*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7402:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7402:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7402) ? (7402) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7402:/ clear");
	     /*clear *//*_#IS_OBJECT__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7403:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7404:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7405:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;
    /*^compute */
 /*_.PAIR_HEAD__V21*/ meltfptr[20] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7407:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.SYMB__V22*/ meltfptr[21] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V21*/ meltfptr[20]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*_.DOC__V23*/ meltfptr[22] = ( /*nil */ NULL);;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_NOT_A__L7*/ meltfnum[1] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.SYMB__V22*/ meltfptr[21]),
			    (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					   tabval[3])));;
    MELT_LOCATION ("warmelt-macro.melt:7410:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L7*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7411:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(EXPORT_MACRO <sym> <expander>) expecting symbol"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_TAIL__V24*/ meltfptr[23] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7412:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V25*/ meltfptr[24] =
      /*_.PAIR_TAIL__V24*/ meltfptr[23];;
    MELT_LOCATION ("warmelt-macro.melt:7413:/ quasiblock");


 /*_.PAIR_HEAD__V27*/ meltfptr[26] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7413:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.EXPV__V28*/ meltfptr[27] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V27*/ meltfptr[26]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
 /*_.PAIR_TAIL__V29*/ meltfptr[28] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7415:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V30*/ meltfptr[29] =
      /*_.PAIR_TAIL__V29*/ meltfptr[28];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_.PAIR_HEAD__V31*/ meltfptr[30] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    /*^compute */
 /*_#eqeq__L8*/ meltfnum[0] =
      (( /*_.PAIR_HEAD__V31*/ meltfptr[30]) ==
       (( /*!konst_4_DOC */ meltfrout->tabval[4])));;
    MELT_LOCATION ("warmelt-macro.melt:7416:/ cond");
    /*cond */ if ( /*_#eqeq__L8*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.PAIR_TAIL__V33*/ meltfptr[32] =
	    (melt_pair_tail
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
	  MELT_LOCATION ("warmelt-macro.melt:7418:/ compute");
	  /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V34*/ meltfptr[33] =
	    /*_.PAIR_TAIL__V33*/ meltfptr[32];;
   /*_.PAIR_HEAD__V35*/ meltfptr[34] =
	    (melt_pair_head
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
	  MELT_LOCATION ("warmelt-macro.melt:7419:/ compute");
	  /*_.DOC__V23*/ meltfptr[22] = /*_.SETQ___V36*/ meltfptr[35] =
	    /*_.PAIR_HEAD__V35*/ meltfptr[34];;
	  MELT_LOCATION ("warmelt-macro.melt:7417:/ quasiblock");


	  /*_.PROGN___V37*/ meltfptr[36] = /*_.SETQ___V36*/ meltfptr[35];;
	  /*^compute */
	  /*_._IF___V32*/ meltfptr[31] = /*_.PROGN___V37*/ meltfptr[36];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7416:/ clear");
	     /*clear *//*_.PAIR_TAIL__V33*/ meltfptr[32] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V34*/ meltfptr[33] = 0;
	  /*^clear */
	     /*clear *//*_.PAIR_HEAD__V35*/ meltfptr[34] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V36*/ meltfptr[35] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V37*/ meltfptr[36] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V32*/ meltfptr[31] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NULL__L9*/ meltfnum[8] =
      (( /*_.EXPV__V28*/ meltfptr[27]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7420:/ cond");
    /*cond */ if ( /*_#NULL__L9*/ meltfnum[8])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7421:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(EXPORT_MACRO <sym> <expander> [:doc <docum>]) expecting expander"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7422:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^quasiblock */


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_EXPORT_MACRO */
					     meltfrout->tabval[5])), (5),
			      "CLASS_SOURCE_EXPORT_MACRO");
  /*_.INST__V40*/ meltfptr[34] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V40*/ meltfptr[34])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V40*/ meltfptr[34]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPMAC_MNAME",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V40*/ meltfptr[34])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V40*/ meltfptr[34]), (2),
			  ( /*_.SYMB__V22*/ meltfptr[21]), "SEXPMAC_MNAME");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPMAC_MVAL",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V40*/ meltfptr[34])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V40*/ meltfptr[34]), (3),
			  ( /*_.EXPV__V28*/ meltfptr[27]), "SEXPMAC_MVAL");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPMAC_DOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V40*/ meltfptr[34])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V40*/ meltfptr[34]), (4),
			  ( /*_.DOC__V23*/ meltfptr[22]), "SEXPMAC_DOC");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V40*/ meltfptr[34],
				  "newly made instance");
    ;
    /*_.RES__V39*/ meltfptr[33] = /*_.INST__V40*/ meltfptr[34];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7429:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L10*/ meltfnum[9] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7429:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L10*/ meltfnum[9])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L11*/ meltfnum[10] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7429:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7429;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_export_macro result res";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V39*/ meltfptr[33];
	      /*_.MELT_DEBUG_FUN__V42*/ meltfptr[36] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V41*/ meltfptr[35] =
	      /*_.MELT_DEBUG_FUN__V42*/ meltfptr[36];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7429:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L11*/ meltfnum[10] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V42*/ meltfptr[36] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V41*/ meltfptr[35] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7429:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L10*/ meltfnum[9] = 0;
      /*^clear */
	     /*clear *//*_._IF___V41*/ meltfptr[35] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7430:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V39*/ meltfptr[33];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7430:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V38*/ meltfptr[32] = /*_.RETURN___V43*/ meltfptr[36];;

    MELT_LOCATION ("warmelt-macro.melt:7422:/ clear");
	   /*clear *//*_.RES__V39*/ meltfptr[33] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V43*/ meltfptr[36] = 0;
    /*_.LET___V26*/ meltfptr[25] = /*_.LET___V38*/ meltfptr[32];;

    MELT_LOCATION ("warmelt-macro.melt:7413:/ clear");
	   /*clear *//*_.PAIR_HEAD__V27*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.EXPV__V28*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V29*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V30*/ meltfptr[29] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V31*/ meltfptr[30] = 0;
    /*^clear */
	   /*clear *//*_#eqeq__L8*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V32*/ meltfptr[31] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L9*/ meltfnum[8] = 0;
    /*^clear */
	   /*clear *//*_.LET___V38*/ meltfptr[32] = 0;
    /*_.LET___V16*/ meltfptr[14] = /*_.LET___V26*/ meltfptr[25];;

    MELT_LOCATION ("warmelt-macro.melt:7403:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.SYMB__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.DOC__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L7*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V25*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_.LET___V26*/ meltfptr[25] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7397:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7397:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_EXPORT_MACRO", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_153_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un *
						     meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un *
						     meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 50
    melt_ptr_t mcfr_varptr[50];
#define MELTFRAM_NBVARNUM 12
    long mcfr_varnum[12];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO_st *)
	meltfirstargp_;
      /* use arguments meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 50; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO nbval 50*/
  meltfram__.mcfr_nbvar = 50 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_EXPORT_PATMACRO", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7441:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7442:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7442:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7442:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7442;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_export_patmacro sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7442:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7442:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7443:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7443:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7443:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7443) ? (7443) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7443:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7444:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7444:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7444:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7444) ? (7444) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7444:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7445:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_CLOSURE__L5*/ meltfnum[1] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) ==
	 MELTOBMAG_CLOSURE);;
      MELT_LOCATION ("warmelt-macro.melt:7445:/ cond");
      /*cond */ if ( /*_#IS_CLOSURE__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7445:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mexpander"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7445) ? (7445) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7445:/ clear");
	     /*clear *//*_#IS_CLOSURE__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7446:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L6*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7446:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7446:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7446) ? (7446) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7446:/ clear");
	     /*clear *//*_#IS_OBJECT__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7447:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7448:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7449:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;
    /*^compute */
 /*_.PAIR_HEAD__V21*/ meltfptr[20] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7451:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.SYMB__V22*/ meltfptr[21] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V21*/ meltfptr[20]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*_.PATEXPV__V23*/ meltfptr[22] = ( /*nil */ NULL);;
    /*^compute */
    /*_.MACEXPV__V24*/ meltfptr[23] = ( /*nil */ NULL);;
    /*^compute */
    /*_.DOC__V25*/ meltfptr[24] = ( /*nil */ NULL);;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_NOT_A__L7*/ meltfnum[1] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.SYMB__V22*/ meltfptr[21]),
			    (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					   tabval[3])));;
    MELT_LOCATION ("warmelt-macro.melt:7456:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L7*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7457:/ locexp");
	    /* error_plain */ melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]), ("(EXPORT_PATMACRO <sym> <patexpander> <macexpander> [:doc <docum>])\
 expecting symbol"), (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_TAIL__V26*/ meltfptr[25] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7458:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V27*/ meltfptr[26] =
      /*_.PAIR_TAIL__V26*/ meltfptr[25];;
 /*_.PAIR_HEAD__V28*/ meltfptr[27] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7459:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.MEXPANDER__V29*/ meltfptr[28] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V28*/ meltfptr[27]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*^compute */
    /*_.PATEXPV__V23*/ meltfptr[22] = /*_.SETQ___V30*/ meltfptr[29] =
      /*_.MEXPANDER__V29*/ meltfptr[28];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#NULL__L8*/ meltfnum[0] =
      (( /*_.PATEXPV__V23*/ meltfptr[22]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7460:/ cond");
    /*cond */ if ( /*_#NULL__L8*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7461:/ locexp");
	    /* error_plain */ melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]), ("(EXPORT_PATMACRO <sym> <patexpander> <macexpander> [:doc <docum>])\
 expecting patexpander"), (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_TAIL__V31*/ meltfptr[30] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7462:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V32*/ meltfptr[31] =
      /*_.PAIR_TAIL__V31*/ meltfptr[30];;
 /*_.PAIR_HEAD__V33*/ meltfptr[32] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7463:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.MEXPANDER__V34*/ meltfptr[33] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V33*/ meltfptr[32]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*^compute */
    /*_.MACEXPV__V24*/ meltfptr[23] = /*_.SETQ___V35*/ meltfptr[34] =
      /*_.MEXPANDER__V34*/ meltfptr[33];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#NULL__L9*/ meltfnum[8] =
      (( /*_.MACEXPV__V24*/ meltfptr[23]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7464:/ cond");
    /*cond */ if ( /*_#NULL__L9*/ meltfnum[8])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7465:/ locexp");
	    /* error_plain */ melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]), ("(EXPORT_PATMACRO <sym> <patexpander> <macexpander> [:doc <docum>])\
 expecting macexpander"), (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_TAIL__V36*/ meltfptr[35] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7466:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V37*/ meltfptr[36] =
      /*_.PAIR_TAIL__V36*/ meltfptr[35];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_.PAIR_HEAD__V38*/ meltfptr[37] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    /*^compute */
 /*_#eqeq__L10*/ meltfnum[9] =
      (( /*_.PAIR_HEAD__V38*/ meltfptr[37]) ==
       (( /*!konst_4_DOC */ meltfrout->tabval[4])));;
    MELT_LOCATION ("warmelt-macro.melt:7467:/ cond");
    /*cond */ if ( /*_#eqeq__L10*/ meltfnum[9])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.PAIR_TAIL__V40*/ meltfptr[39] =
	    (melt_pair_tail
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
	  MELT_LOCATION ("warmelt-macro.melt:7469:/ compute");
	  /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V41*/ meltfptr[40] =
	    /*_.PAIR_TAIL__V40*/ meltfptr[39];;
   /*_.PAIR_HEAD__V42*/ meltfptr[41] =
	    (melt_pair_head
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
	  MELT_LOCATION ("warmelt-macro.melt:7470:/ compute");
	  /*_.DOC__V25*/ meltfptr[24] = /*_.SETQ___V43*/ meltfptr[42] =
	    /*_.PAIR_HEAD__V42*/ meltfptr[41];;
	  MELT_LOCATION ("warmelt-macro.melt:7468:/ quasiblock");


	  /*_.PROGN___V44*/ meltfptr[43] = /*_.SETQ___V43*/ meltfptr[42];;
	  /*^compute */
	  /*_._IF___V39*/ meltfptr[38] = /*_.PROGN___V44*/ meltfptr[43];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7467:/ clear");
	     /*clear *//*_.PAIR_TAIL__V40*/ meltfptr[39] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V41*/ meltfptr[40] = 0;
	  /*^clear */
	     /*clear *//*_.PAIR_HEAD__V42*/ meltfptr[41] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V43*/ meltfptr[42] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V44*/ meltfptr[43] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V39*/ meltfptr[38] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-macro.melt:7473:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^quasiblock */


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_EXPORT_PATMACRO */ meltfrout->tabval[5])), (6), "CLASS_SOURCE_EXPORT_PATMACRO");
  /*_.INST__V47*/ meltfptr[41] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V47*/ meltfptr[41])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V47*/ meltfptr[41]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPMAC_MNAME",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V47*/ meltfptr[41])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V47*/ meltfptr[41]), (2),
			  ( /*_.SYMB__V22*/ meltfptr[21]), "SEXPMAC_MNAME");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPPAT_PVAL",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V47*/ meltfptr[41])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V47*/ meltfptr[41]), (5),
			  ( /*_.PATEXPV__V23*/ meltfptr[22]), "SEXPPAT_PVAL");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPMAC_MVAL",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V47*/ meltfptr[41])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V47*/ meltfptr[41]), (3),
			  ( /*_.MACEXPV__V24*/ meltfptr[23]), "SEXPMAC_MVAL");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPMAC_DOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V47*/ meltfptr[41])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V47*/ meltfptr[41]), (4),
			  ( /*_.DOC__V25*/ meltfptr[24]), "SEXPMAC_DOC");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V47*/ meltfptr[41],
				  "newly made instance");
    ;
    /*_.RES__V46*/ meltfptr[40] = /*_.INST__V47*/ meltfptr[41];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7481:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L11*/ meltfnum[10] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7481:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L11*/ meltfnum[10])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L12*/ meltfnum[11] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7481:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7481;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_export_patmacro result res";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V46*/ meltfptr[40];
	      /*_.MELT_DEBUG_FUN__V49*/ meltfptr[43] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V48*/ meltfptr[42] =
	      /*_.MELT_DEBUG_FUN__V49*/ meltfptr[43];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7481:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L12*/ meltfnum[11] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V49*/ meltfptr[43] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V48*/ meltfptr[42] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7481:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L11*/ meltfnum[10] = 0;
      /*^clear */
	     /*clear *//*_._IF___V48*/ meltfptr[42] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7482:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V46*/ meltfptr[40];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7482:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V45*/ meltfptr[39] = /*_.RETURN___V50*/ meltfptr[43];;

    MELT_LOCATION ("warmelt-macro.melt:7473:/ clear");
	   /*clear *//*_.RES__V46*/ meltfptr[40] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V50*/ meltfptr[43] = 0;
    /*_.LET___V16*/ meltfptr[14] = /*_.LET___V45*/ meltfptr[39];;

    MELT_LOCATION ("warmelt-macro.melt:7447:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.SYMB__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.PATEXPV__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.MACEXPV__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.DOC__V25*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L7*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V26*/ meltfptr[25] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V27*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V28*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.MEXPANDER__V29*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V30*/ meltfptr[29] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L8*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V31*/ meltfptr[30] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V32*/ meltfptr[31] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V33*/ meltfptr[32] = 0;
    /*^clear */
	   /*clear *//*_.MEXPANDER__V34*/ meltfptr[33] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V35*/ meltfptr[34] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L9*/ meltfnum[8] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V36*/ meltfptr[35] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V37*/ meltfptr[36] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V38*/ meltfptr[37] = 0;
    /*^clear */
	   /*clear *//*_#eqeq__L10*/ meltfnum[9] = 0;
    /*^clear */
	   /*clear *//*_._IF___V39*/ meltfptr[38] = 0;
    /*^clear */
	   /*clear *//*_.LET___V45*/ meltfptr[39] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7441:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7441:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_EXPORT_PATMACRO", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_154_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un *
						  meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un *
						  meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 28
    melt_ptr_t mcfr_varptr[28];
#define MELTFRAM_NBVARNUM 8
    long mcfr_varnum[8];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS_st
	 *) meltfirstargp_;
      /* use arguments meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 28; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS nbval 28*/
  meltfram__.mcfr_nbvar = 28 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_EXPORT_CLASS", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7491:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7492:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L1*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[0])));;
      MELT_LOCATION ("warmelt-macro.melt:7492:/ cond");
      /*cond */ if ( /*_#IS_A__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V7*/ meltfptr[6] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7492:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7492) ? (7492) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7492:/ clear");
	     /*clear *//*_#IS_A__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7493:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L2*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7493:/ cond");
      /*cond */ if ( /*_#IS_A__L2*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[8] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7493:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7493) ? (7493) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7493:/ clear");
	     /*clear *//*_#IS_A__L2*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NULL__L3*/ meltfnum[0] =
      (( /*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7494:/ cond");
    /*cond */ if ( /*_#NULL__L3*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^compute */
	  /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V11*/ meltfptr[10] =
	    ( /*!MACROEXPAND_1 */ meltfrout->tabval[2]);;
	  /*_._IF___V10*/ meltfptr[8] = /*_.SETQ___V11*/ meltfptr[10];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7494:/ clear");
	     /*clear *//*_.SETQ___V11*/ meltfptr[10] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V10*/ meltfptr[8] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7495:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L4*/ meltfnum[3] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7495:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L4*/ meltfnum[3])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7495:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7495) ? (7495) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7495:/ clear");
	     /*clear *//*_#IS_OBJECT__L4*/ meltfnum[3] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7496:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L5*/ meltfnum[3] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7496:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L5*/ meltfnum[3])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L6*/ meltfnum[5] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7496:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7496;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_export_class sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V14*/ meltfptr[12] =
	      /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7496:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L6*/ meltfnum[5] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V15*/ meltfptr[14] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V14*/ meltfptr[12] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7496:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L5*/ meltfnum[3] = 0;
      /*^clear */
	     /*clear *//*_._IF___V14*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7497:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7498:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[12] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7499:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[12])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;
    MELT_LOCATION ("warmelt-macro.melt:7503:/ quasiblock");


    /*^newclosure */
		 /*newclosure *//*_.__V22*/ meltfptr[21] =
      (melt_ptr_t)
      meltgc_new_closure ((meltobject_ptr_t)
			  (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			  (meltroutine_ptr_t) (( /*!konst_7 */ meltfrout->
						tabval[7])), (4));
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V22*/ meltfptr[21])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 0 >= 0
		    && 0 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V22*/ meltfptr[21])));
    ((meltclosure_ptr_t) /*_.__V22*/ meltfptr[21])->tabval[0] =
      (melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]);
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V22*/ meltfptr[21])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 1 >= 0
		    && 1 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V22*/ meltfptr[21])));
    ((meltclosure_ptr_t) /*_.__V22*/ meltfptr[21])->tabval[1] =
      (melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]);
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V22*/ meltfptr[21])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 2 >= 0
		    && 2 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V22*/ meltfptr[21])));
    ((meltclosure_ptr_t) /*_.__V22*/ meltfptr[21])->tabval[2] =
      (melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4]);
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.__V22*/ meltfptr[21])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 3 >= 0
		    && 3 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.__V22*/ meltfptr[21])));
    ((meltclosure_ptr_t) /*_.__V22*/ meltfptr[21])->tabval[3] =
      (melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]);
    ;
    /*_.LAMBDA___V21*/ meltfptr[20] = /*_.__V22*/ meltfptr[21];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7501:/ apply");
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!DISCR_MULTIPLE */ meltfrout->tabval[5]);
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.LAMBDA___V21*/ meltfptr[20];
      /*_.SYMBTUP__V23*/ meltfptr[22] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!PAIRLIST_TO_MULTIPLE */ meltfrout->tabval[4])),
		    (melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7509:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_EXPORT_CLASS */
					     meltfrout->tabval[8])), (3),
			      "CLASS_SOURCE_EXPORT_CLASS");
  /*_.INST__V25*/ meltfptr[24] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V25*/ meltfptr[24])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V25*/ meltfptr[24]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPORT_NAMES",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V25*/ meltfptr[24])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V25*/ meltfptr[24]), (2),
			  ( /*_.SYMBTUP__V23*/ meltfptr[22]),
			  "SEXPORT_NAMES");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V25*/ meltfptr[24],
				  "newly made instance");
    ;
    /*_.RES__V24*/ meltfptr[23] = /*_.INST__V25*/ meltfptr[24];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7513:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L7*/ meltfnum[5] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7513:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L7*/ meltfnum[5])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L8*/ meltfnum[3] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7513:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[3];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7513;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_export_class result res";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V24*/ meltfptr[23];
	      /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V26*/ meltfptr[25] =
	      /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7513:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L8*/ meltfnum[3] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V27*/ meltfptr[26] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V26*/ meltfptr[25] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7513:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[5] = 0;
      /*^clear */
	     /*clear *//*_._IF___V26*/ meltfptr[25] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7514:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V24*/ meltfptr[23];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7514:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V16*/ meltfptr[14] = /*_.RETURN___V28*/ meltfptr[26];;

    MELT_LOCATION ("warmelt-macro.melt:7497:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.LAMBDA___V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.SYMBTUP__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.RES__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V28*/ meltfptr[26] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7491:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7491:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L3*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V10*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_EXPORT_CLASS", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_155_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_156_WARMELTmiMACRO (meltclosure_ptr_t meltclosp_,
			     melt_ptr_t meltfirstargp_,
			     const melt_argdescr_cell_t meltxargdescr_[],
			     union meltparam_un * meltxargtab_,
			     const melt_argdescr_cell_t meltxresdescr_[],
			     union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_156_WARMELTmiMACRO_melt =
    melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_156_WARMELTmiMACRO_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 4
    melt_ptr_t mcfr_varptr[4];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_156_WARMELTmiMACRO is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_156_WARMELTmiMACRO_st *) meltfirstargp_;
      /* use arguments meltrout_156_WARMELTmiMACRO output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 4; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_156_WARMELTmiMACRO nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("**", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7503:/ getarg");
 /*_.S__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-macro.melt:7504:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*~ENV */ meltfclos->tabval[1]);
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & ( /*~MEXPANDER */ meltfclos->tabval[0]);
      /*^apply.arg */
      argtab[2].meltbp_aptr =
	(melt_ptr_t *) & ( /*~MODCTX */ meltfclos->tabval[2]);
      /*_.SYM__V4*/ meltfptr[3] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~MEXPANDER */ meltfclos->tabval[0])),
		    (melt_ptr_t) ( /*_.S__V2*/ meltfptr[1]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_NOT_A__L1*/ meltfnum[0] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.SYM__V4*/ meltfptr[3]),
			    (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					   tabval[0])));;
    MELT_LOCATION ("warmelt-macro.melt:7505:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7506:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t)
			      (( /*~LOC */ meltfclos->tabval[3])),
			      ("(EXPORT_CLASS <sym>...) expecting symbol"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*_.LET___V3*/ meltfptr[2] = /*_.SYM__V4*/ meltfptr[3];;

    MELT_LOCATION ("warmelt-macro.melt:7504:/ clear");
	   /*clear *//*_.SYM__V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L1*/ meltfnum[0] = 0;
    MELT_LOCATION ("warmelt-macro.melt:7503:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7503:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.LET___V3*/ meltfptr[2] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("**", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_156_WARMELTmiMACRO_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_156_WARMELTmiMACRO */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un *
						    meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un *
						    meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 52
    melt_ptr_t mcfr_varptr[52];
#define MELTFRAM_NBVARNUM 17
    long mcfr_varnum[17];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM_st *)
	meltfirstargp_;
      /* use arguments meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 52; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM nbval 52*/
  meltfram__.mcfr_nbvar = 52 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_EXPORT_SYNONYM", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7523:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7524:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7524:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7524:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7524;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_export_synonym sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7524:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7524:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7525:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7525:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7525:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7525) ? (7525) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7525:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7526:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7526:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7526:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7526) ? (7526) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7526:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NULL__L5*/ meltfnum[1] =
      (( /*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7527:/ cond");
    /*cond */ if ( /*_#NULL__L5*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^compute */
	  /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[12] =
	    ( /*!MACROEXPAND_1 */ meltfrout->tabval[3]);;
	  /*_._IF___V12*/ meltfptr[10] = /*_.SETQ___V13*/ meltfptr[12];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7527:/ clear");
	     /*clear *//*_.SETQ___V13*/ meltfptr[12] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V12*/ meltfptr[10] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7528:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L6*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7528:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7528:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7528) ? (7528) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7528:/ clear");
	     /*clear *//*_#IS_OBJECT__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7529:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7530:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7531:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;
    /*^compute */
    /*_.NEWSYMB__V21*/ meltfptr[20] = ( /*nil */ NULL);;
    /*^compute */
    /*_.OLDSYMB__V22*/ meltfptr[21] = ( /*nil */ NULL);;
    /*^compute */
    /*_.DOC__V23*/ meltfptr[22] = ( /*nil */ NULL);;
    /*^compute */
 /*_.PAIR_HEAD__V24*/ meltfptr[23] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7538:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.MEXPANDER__V25*/ meltfptr[24] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V24*/ meltfptr[23]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*^compute */
    /*_.NEWSYMB__V21*/ meltfptr[20] = /*_.SETQ___V26*/ meltfptr[25] =
      /*_.MEXPANDER__V25*/ meltfptr[24];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7539:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L7*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7539:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L7*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7539:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7539;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpander_export_synonym newsymb";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NEWSYMB__V21*/ meltfptr[20];
	      /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V27*/ meltfptr[26] =
	      /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7539:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V27*/ meltfptr[26] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7539:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V27*/ meltfptr[26] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_NOT_A__L9*/ meltfnum[7] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.NEWSYMB__V21*/ meltfptr[20]),
			    (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					   tabval[4])));;
    MELT_LOCATION ("warmelt-macro.melt:7540:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L9*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7541:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("bad new name in (EXPORT_SYNONYM <newname> <oldname> [:doc <docum>])"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_TAIL__V29*/ meltfptr[27] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7542:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V30*/ meltfptr[26] =
      /*_.PAIR_TAIL__V29*/ meltfptr[27];;
 /*_.PAIR_HEAD__V31*/ meltfptr[30] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7544:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.MEXPANDER__V32*/ meltfptr[31] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V31*/ meltfptr[30]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*^compute */
    /*_.OLDSYMB__V22*/ meltfptr[21] = /*_.SETQ___V33*/ meltfptr[32] =
      /*_.MEXPANDER__V32*/ meltfptr[31];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7545:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L10*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7545:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L10*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L11*/ meltfnum[10] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7545:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7545;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpander_export_synonym oldsymb";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.OLDSYMB__V22*/ meltfptr[21];
	      /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V34*/ meltfptr[33] =
	      /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7545:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L11*/ meltfnum[10] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V35*/ meltfptr[34] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V34*/ meltfptr[33] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7545:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L10*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V34*/ meltfptr[33] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_NOT_A__L12*/ meltfnum[10] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.OLDSYMB__V22*/ meltfptr[21]),
			    (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					   tabval[4])));;
    MELT_LOCATION ("warmelt-macro.melt:7546:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L12*/ meltfnum[10])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7547:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("bad old name in (EXPORT_SYNONYM <newname> <oldname> [:doc <docum>])"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_TAIL__V36*/ meltfptr[34] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7548:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V37*/ meltfptr[33] =
      /*_.PAIR_TAIL__V36*/ meltfptr[34];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_.PAIR_HEAD__V38*/ meltfptr[37] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    /*^compute */
 /*_#eqeq__L13*/ meltfnum[0] =
      (( /*_.PAIR_HEAD__V38*/ meltfptr[37]) ==
       (( /*!konst_5_DOC */ meltfrout->tabval[5])));;
    MELT_LOCATION ("warmelt-macro.melt:7549:/ cond");
    /*cond */ if ( /*_#eqeq__L13*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.PAIR_TAIL__V40*/ meltfptr[39] =
	    (melt_pair_tail
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
	  MELT_LOCATION ("warmelt-macro.melt:7551:/ compute");
	  /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V41*/ meltfptr[40] =
	    /*_.PAIR_TAIL__V40*/ meltfptr[39];;
   /*_.PAIR_HEAD__V42*/ meltfptr[41] =
	    (melt_pair_head
	     ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
	  MELT_LOCATION ("warmelt-macro.melt:7552:/ compute");
	  /*_.DOC__V23*/ meltfptr[22] = /*_.SETQ___V43*/ meltfptr[42] =
	    /*_.PAIR_HEAD__V42*/ meltfptr[41];;
	  MELT_LOCATION ("warmelt-macro.melt:7550:/ quasiblock");


	  /*_.PROGN___V44*/ meltfptr[43] = /*_.SETQ___V43*/ meltfptr[42];;
	  /*^compute */
	  /*_._IF___V39*/ meltfptr[38] = /*_.PROGN___V44*/ meltfptr[43];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7549:/ clear");
	     /*clear *//*_.PAIR_TAIL__V40*/ meltfptr[39] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V41*/ meltfptr[40] = 0;
	  /*^clear */
	     /*clear *//*_.PAIR_HEAD__V42*/ meltfptr[41] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V43*/ meltfptr[42] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V44*/ meltfptr[43] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V39*/ meltfptr[38] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7554:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L14*/ meltfnum[13] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7554:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L14*/ meltfnum[13])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L15*/ meltfnum[14] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7554:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[14];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7554;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpander_export_synonym doc";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.DOC__V23*/ meltfptr[22];
	      /*_.MELT_DEBUG_FUN__V46*/ meltfptr[40] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V45*/ meltfptr[39] =
	      /*_.MELT_DEBUG_FUN__V46*/ meltfptr[40];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7554:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L15*/ meltfnum[14] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V46*/ meltfptr[40] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V45*/ meltfptr[39] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7554:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L14*/ meltfnum[13] = 0;
      /*^clear */
	     /*clear *//*_._IF___V45*/ meltfptr[39] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7555:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7556:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_EXPORT_SYNONYM */ meltfrout->tabval[6])), (5), "CLASS_SOURCE_EXPORT_SYNONYM");
  /*_.INST__V49*/ meltfptr[43] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V49*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V49*/ meltfptr[43]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPSYN_NEWNAME",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V49*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V49*/ meltfptr[43]), (2),
			  ( /*_.NEWSYMB__V21*/ meltfptr[20]),
			  "SEXPSYN_NEWNAME");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPSYN_OLDNAME",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V49*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V49*/ meltfptr[43]), (3),
			  ( /*_.OLDSYMB__V22*/ meltfptr[21]),
			  "SEXPSYN_OLDNAME");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SEXPSYN_DOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V49*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V49*/ meltfptr[43]), (4),
			  ( /*_.DOC__V23*/ meltfptr[22]), "SEXPSYN_DOC");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V49*/ meltfptr[43],
				  "newly made instance");
    ;
    /*_.RES__V48*/ meltfptr[42] = /*_.INST__V49*/ meltfptr[43];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7562:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L16*/ meltfnum[14] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7562:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L16*/ meltfnum[14])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L17*/ meltfnum[13] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7562:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[13];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7562;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpander_export_synonym result";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V48*/ meltfptr[42];
	      /*_.MELT_DEBUG_FUN__V51*/ meltfptr[39] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V50*/ meltfptr[40] =
	      /*_.MELT_DEBUG_FUN__V51*/ meltfptr[39];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7562:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L17*/ meltfnum[13] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V51*/ meltfptr[39] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V50*/ meltfptr[40] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7562:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L16*/ meltfnum[14] = 0;
      /*^clear */
	     /*clear *//*_._IF___V50*/ meltfptr[40] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7563:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V48*/ meltfptr[42];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7563:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V47*/ meltfptr[41] = /*_.RETURN___V52*/ meltfptr[39];;

    MELT_LOCATION ("warmelt-macro.melt:7555:/ clear");
	   /*clear *//*_.RES__V48*/ meltfptr[42] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V52*/ meltfptr[39] = 0;
    /*_.LET___V16*/ meltfptr[14] = /*_.LET___V47*/ meltfptr[41];;

    MELT_LOCATION ("warmelt-macro.melt:7529:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.NEWSYMB__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.OLDSYMB__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.DOC__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.MEXPANDER__V25*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V26*/ meltfptr[25] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L9*/ meltfnum[7] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V29*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V30*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V31*/ meltfptr[30] = 0;
    /*^clear */
	   /*clear *//*_.MEXPANDER__V32*/ meltfptr[31] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V33*/ meltfptr[32] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L12*/ meltfnum[10] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V36*/ meltfptr[34] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V37*/ meltfptr[33] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V38*/ meltfptr[37] = 0;
    /*^clear */
	   /*clear *//*_#eqeq__L13*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V39*/ meltfptr[38] = 0;
    /*^clear */
	   /*clear *//*_.LET___V47*/ meltfptr[41] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7523:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7523:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L5*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_EXPORT_SYNONYM", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_157_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 23
    melt_ptr_t mcfr_varptr[23];
#define MELTFRAM_NBVARNUM 8
    long mcfr_varnum[8];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE_st
	 *) meltfirstargp_;
      /* use arguments meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 23; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE nbval 23*/
  meltfram__.mcfr_nbvar = 23 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE",
		    meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7575:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7576:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L1*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[0])));;
      MELT_LOCATION ("warmelt-macro.melt:7576:/ cond");
      /*cond */ if ( /*_#IS_A__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V7*/ meltfptr[6] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7576:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7576) ? (7576) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7576:/ clear");
	     /*clear *//*_#IS_A__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7577:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L2*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7577:/ cond");
      /*cond */ if ( /*_#IS_A__L2*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[8] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7577:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7577) ? (7577) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7577:/ clear");
	     /*clear *//*_#IS_A__L2*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7578:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L3*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7578:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L3*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7578:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7578) ? (7578) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7578:/ clear");
	     /*clear *//*_#IS_OBJECT__L3*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[8] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7579:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L4*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7579:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L5*/ meltfnum[4] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7579:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7579;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"mexpand_current_module_environment_reference sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V13*/ meltfptr[12] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[2])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V12*/ meltfptr[10] =
	      /*_.MELT_DEBUG_FUN__V13*/ meltfptr[12];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7579:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L5*/ meltfnum[4] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V13*/ meltfptr[12] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V12*/ meltfptr[10] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7579:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V12*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7580:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7581:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V15*/ meltfptr[10] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7582:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V16*/ meltfptr[15] = slot;
    };
    ;
 /*_.LIST_FIRST__V17*/ meltfptr[16] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V15*/ meltfptr[10])));;
    /*^compute */
 /*_.CURPAIR__V18*/ meltfptr[17] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V17*/ meltfptr[16])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7584:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_CURRENT_MODULE_ENVIRONMENT_REFERENCE */ meltfrout->tabval[3])), (3), "CLASS_SOURCE_CURRENT_MODULE_ENVIRONMENT_REFERENCE");
  /*_.INST__V20*/ meltfptr[19] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V20*/ meltfptr[19])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V20*/ meltfptr[19]), (1),
			  ( /*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V20*/ meltfptr[19],
				  "newly made instance");
    ;
    /*_.RES__V19*/ meltfptr[18] = /*_.INST__V20*/ meltfptr[19];;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NOTNULL__L6*/ meltfnum[4] =
      (( /*_.CURPAIR__V18*/ meltfptr[17]) != NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7587:/ cond");
    /*cond */ if ( /*_#NOTNULL__L6*/ meltfnum[4])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7588:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V16*/ meltfptr[15]),
			      ("(CURRENT_MODULE_ENVIRONMENT_REFERENCE) should not have any argument"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7589:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L7*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7589:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L7*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7589:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7589;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"mexpand_current_module_environment_reference return res";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V19*/ meltfptr[18];
	      /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[2])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V21*/ meltfptr[20] =
	      /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7589:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V21*/ meltfptr[20] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7589:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V21*/ meltfptr[20] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7590:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V19*/ meltfptr[18];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7590:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V14*/ meltfptr[12] = /*_.RETURN___V23*/ meltfptr[21];;

    MELT_LOCATION ("warmelt-macro.melt:7580:/ clear");
	   /*clear *//*_.CONT__V15*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V16*/ meltfptr[15] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.RES__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_#NOTNULL__L6*/ meltfnum[4] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V23*/ meltfptr[21] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7575:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V14*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7575:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_.LET___V14*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE",
		  meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_158_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 25
    melt_ptr_t mcfr_varptr[25];
#define MELTFRAM_NBVARNUM 9
    long mcfr_varnum[9];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT_st
	 *) meltfirstargp_;
      /* use arguments meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 25; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT nbval 25*/
  meltfram__.mcfr_nbvar = 25 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_PARENT_MODULE_ENVIRONMENT", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7609:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7610:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L1*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[0])));;
      MELT_LOCATION ("warmelt-macro.melt:7610:/ cond");
      /*cond */ if ( /*_#IS_A__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V7*/ meltfptr[6] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7610:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7610) ? (7610) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7610:/ clear");
	     /*clear *//*_#IS_A__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7611:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L2*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7611:/ cond");
      /*cond */ if ( /*_#IS_A__L2*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[8] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7611:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7611) ? (7611) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7611:/ clear");
	     /*clear *//*_#IS_A__L2*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NULL__L3*/ meltfnum[0] =
      (( /*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7612:/ cond");
    /*cond */ if ( /*_#NULL__L3*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^compute */
	  /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V11*/ meltfptr[10] =
	    ( /*!MACROEXPAND_1 */ meltfrout->tabval[2]);;
	  /*_._IF___V10*/ meltfptr[8] = /*_.SETQ___V11*/ meltfptr[10];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7612:/ clear");
	     /*clear *//*_.SETQ___V11*/ meltfptr[10] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V10*/ meltfptr[8] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7613:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L4*/ meltfnum[3] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7613:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L4*/ meltfnum[3])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7613:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7613) ? (7613) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7613:/ clear");
	     /*clear *//*_#IS_OBJECT__L4*/ meltfnum[3] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7614:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L5*/ meltfnum[3] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7614:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L5*/ meltfnum[3])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L6*/ meltfnum[5] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7614:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7614;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"mexpand_parent_module_environment sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V14*/ meltfptr[12] =
	      /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7614:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L6*/ meltfnum[5] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V15*/ meltfptr[14] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V14*/ meltfptr[12] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7614:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L5*/ meltfnum[3] = 0;
      /*^clear */
	     /*clear *//*_._IF___V14*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7615:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7616:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[12] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7617:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[12])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7619:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_PARENT_MODULE_ENVIRONMENT */ meltfrout->tabval[4])), (2), "CLASS_SOURCE_PARENT_MODULE_ENVIRONMENT");
  /*_.INST__V22*/ meltfptr[21] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V22*/ meltfptr[21])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V22*/ meltfptr[21]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V22*/ meltfptr[21],
				  "newly made instance");
    ;
    /*_.RES__V21*/ meltfptr[20] = /*_.INST__V22*/ meltfptr[21];;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NOTNULL__L7*/ meltfnum[5] =
      (( /*_.CURPAIR__V20*/ meltfptr[19]) != NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7622:/ cond");
    /*cond */ if ( /*_#NOTNULL__L7*/ meltfnum[5])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7623:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(PARENT_MODULE_ENVIRONMENT) should not have any argument"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7624:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L8*/ meltfnum[3] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7624:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L8*/ meltfnum[3])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7624:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7624;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"mexpand_parent_module_environment return res";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V21*/ meltfptr[20];
	      /*_.MELT_DEBUG_FUN__V24*/ meltfptr[23] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V23*/ meltfptr[22] =
	      /*_.MELT_DEBUG_FUN__V24*/ meltfptr[23];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7624:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L9*/ meltfnum[8] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V24*/ meltfptr[23] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V23*/ meltfptr[22] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7624:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L8*/ meltfnum[3] = 0;
      /*^clear */
	     /*clear *//*_._IF___V23*/ meltfptr[22] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7625:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V21*/ meltfptr[20];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7625:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V16*/ meltfptr[14] = /*_.RETURN___V25*/ meltfptr[23];;

    MELT_LOCATION ("warmelt-macro.melt:7615:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.RES__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_#NOTNULL__L7*/ meltfnum[5] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V25*/ meltfptr[23] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7609:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7609:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L3*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V10*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_PARENT_MODULE_ENVIRONMENT", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_159_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 23
    melt_ptr_t mcfr_varptr[23];
#define MELTFRAM_NBVARNUM 10
    long mcfr_varnum[10];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE_st
	 *) meltfirstargp_;
      /* use arguments meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 23; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE nbval 23*/
  meltfram__.mcfr_nbvar = 23 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE",
		    meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7639:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7640:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7640:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7640:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7640;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"mexpand_update_current_module_environment_reference sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7640:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7640:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7641:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7641:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7641:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7641) ? (7641) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7641:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7642:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7642:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7642:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7642) ? (7642) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7642:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7643:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L5*/ meltfnum[1] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7643:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7643:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7643) ? (7643) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7643:/ clear");
	     /*clear *//*_#IS_OBJECT__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7644:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7645:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V15*/ meltfptr[14] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7646:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V16*/ meltfptr[15] = slot;
    };
    ;
 /*_.LIST_FIRST__V17*/ meltfptr[16] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V16*/ meltfptr[15])));;
    /*^compute */
 /*_.CURPAIR__V18*/ meltfptr[17] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V17*/ meltfptr[16])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7648:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE */ meltfrout->tabval[3])), (3), "CLASS_SOURCE_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE");
  /*_.INST__V20*/ meltfptr[19] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V20*/ meltfptr[19])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V20*/ meltfptr[19]), (1),
			  ( /*_.LOC__V15*/ meltfptr[14]), "LOCA_LOCATION");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V20*/ meltfptr[19],
				  "newly made instance");
    ;
    /*_.RES__V19*/ meltfptr[18] = /*_.INST__V20*/ meltfptr[19];;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NOTNULL__L6*/ meltfnum[0] =
      (( /*_.CURPAIR__V18*/ meltfptr[17]) != NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7651:/ cond");
    /*cond */ if ( /*_#NOTNULL__L6*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7652:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V15*/ meltfptr[14]),
			      ("(UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE) should not have any argument"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#MELT_IS_BOOTSTRAPPING__L7*/ meltfnum[1] =
      (melt_flag_bootstrapping);;
    /*^compute */
 /*_#NOT__L8*/ meltfnum[7] =
      (!( /*_#MELT_IS_BOOTSTRAPPING__L7*/ meltfnum[1]));;
    MELT_LOCATION ("warmelt-macro.melt:7653:/ cond");
    /*cond */ if ( /*_#NOT__L8*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7654:/ locexp");
	    /* error_plain */ melt_error_str ((melt_ptr_t) ( /*_.LOC__V15*/ meltfptr[14]), ("(UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE) can only be used while\
 bootstrapping MELT."), (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7655:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L9*/ meltfnum[8] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7655:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L9*/ meltfnum[8])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7655:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7655;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"mexpand_update_current_module_environment_reference return res";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V19*/ meltfptr[18];
	      /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V21*/ meltfptr[20] =
	      /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7655:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L10*/ meltfnum[9] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V21*/ meltfptr[20] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7655:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L9*/ meltfnum[8] = 0;
      /*^clear */
	     /*clear *//*_._IF___V21*/ meltfptr[20] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7656:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V19*/ meltfptr[18];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7656:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V14*/ meltfptr[12] = /*_.RETURN___V23*/ meltfptr[21];;

    MELT_LOCATION ("warmelt-macro.melt:7644:/ clear");
	   /*clear *//*_.LOC__V15*/ meltfptr[14] = 0;
    /*^clear */
	   /*clear *//*_.CONT__V16*/ meltfptr[15] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.RES__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_#NOTNULL__L6*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_#MELT_IS_BOOTSTRAPPING__L7*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L8*/ meltfnum[7] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V23*/ meltfptr[21] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7639:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V14*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7639:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V14*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE",
		  meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_160_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un *
						      meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un *
						      meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 29
    melt_ptr_t mcfr_varptr[29];
#define MELTFRAM_NBVARNUM 14
    long mcfr_varnum[14];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED_st *)
	meltfirstargp_;
      /* use arguments meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 29; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED nbval 29*/
  meltfram__.mcfr_nbvar = 29 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_FETCH_PREDEFINED", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7674:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7675:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7675:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7675:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7675;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_fetch_predefined sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7675:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7675:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7676:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7676:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7676:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7676) ? (7676) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7676:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7677:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7677:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7677:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7677) ? (7677) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7677:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NULL__L5*/ meltfnum[1] =
      (( /*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7678:/ cond");
    /*cond */ if ( /*_#NULL__L5*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^compute */
	  /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[12] =
	    ( /*!MACROEXPAND_1 */ meltfrout->tabval[3]);;
	  /*_._IF___V12*/ meltfptr[10] = /*_.SETQ___V13*/ meltfptr[12];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7678:/ clear");
	     /*clear *//*_.SETQ___V13*/ meltfptr[12] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V12*/ meltfptr[10] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7679:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L6*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7679:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7679:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7679) ? (7679) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7679:/ clear");
	     /*clear *//*_#IS_OBJECT__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7680:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7681:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7682:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;
    /*^compute */
 /*_.PAIR_HEAD__V21*/ meltfptr[20] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7684:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.SPREDEF__V22*/ meltfptr[21] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V21*/ meltfptr[20]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#NULL__L7*/ meltfnum[0] =
      (( /*_.CURPAIR__V20*/ meltfptr[19]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7686:/ cond");
    /*cond */ if ( /*_#NULL__L7*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7687:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(FETCH_PREDEFINED <predef-name-or-rank>) missing argument"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_.PAIR_TAIL__V23*/ meltfptr[22] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    /*^compute */
 /*_#NOTNULL__L8*/ meltfnum[7] =
      (( /*_.PAIR_TAIL__V23*/ meltfptr[22]) != NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7688:/ cond");
    /*cond */ if ( /*_#NOTNULL__L8*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7689:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(FETCH_PREDEFINED <predef-name-or-rank>) extra argument"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_A__L9*/ meltfnum[8] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.SPREDEF__V22*/ meltfptr[21]),
			   (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					  tabval[4])));;
    MELT_LOCATION ("warmelt-macro.melt:7690:/ cond");
    /*cond */ if ( /*_#IS_A__L9*/ meltfnum[8])	/*then */
      {
	/*^cond.then */
	/*_#OR___L10*/ meltfnum[9] = /*_#IS_A__L9*/ meltfnum[8];;
      }
    else
      {
	MELT_LOCATION ("warmelt-macro.melt:7690:/ cond.else");

	/*^block */
	/*anyblock */
	{

   /*_#IS_INTEGERBOX__L11*/ meltfnum[10] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.SPREDEF__V22*/ meltfptr[21])) ==
	     MELTOBMAG_INT);;
	  /*^compute */
	  /*_#OR___L10*/ meltfnum[9] = /*_#IS_INTEGERBOX__L11*/ meltfnum[10];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7690:/ clear");
	     /*clear *//*_#IS_INTEGERBOX__L11*/ meltfnum[10] = 0;
	}
	;
      }
    ;
 /*_#NOT__L12*/ meltfnum[10] =
      (!( /*_#OR___L10*/ meltfnum[9]));;
    MELT_LOCATION ("warmelt-macro.melt:7690:/ cond");
    /*cond */ if ( /*_#NOT__L12*/ meltfnum[10])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7691:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(FETCH_PREDEFINED <predef-name-or-rank>) invalid argument"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7692:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^quasiblock */


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_FETCH_PREDEFINED */ meltfrout->tabval[5])), (3), "CLASS_SOURCE_FETCH_PREDEFINED");
  /*_.INST__V26*/ meltfptr[25] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V26*/ meltfptr[25])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V26*/ meltfptr[25]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SFEPD_PREDEF",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V26*/ meltfptr[25])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V26*/ meltfptr[25]), (2),
			  ( /*_.SPREDEF__V22*/ meltfptr[21]), "SFEPD_PREDEF");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V26*/ meltfptr[25],
				  "newly made instance");
    ;
    /*_.RES__V25*/ meltfptr[24] = /*_.INST__V26*/ meltfptr[25];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7695:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L13*/ meltfnum[12] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7695:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L13*/ meltfnum[12])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L14*/ meltfnum[13] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7695:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[13];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7695;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_fetch_predefined result";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V25*/ meltfptr[24];
	      /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V27*/ meltfptr[26] =
	      /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7695:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L14*/ meltfnum[13] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V27*/ meltfptr[26] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7695:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L13*/ meltfnum[12] = 0;
      /*^clear */
	     /*clear *//*_._IF___V27*/ meltfptr[26] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7696:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V25*/ meltfptr[24];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7696:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V24*/ meltfptr[23] = /*_.RETURN___V29*/ meltfptr[27];;

    MELT_LOCATION ("warmelt-macro.melt:7692:/ clear");
	   /*clear *//*_.RES__V25*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V29*/ meltfptr[27] = 0;
    /*_.LET___V16*/ meltfptr[14] = /*_.LET___V24*/ meltfptr[23];;

    MELT_LOCATION ("warmelt-macro.melt:7680:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.SPREDEF__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L7*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_#NOTNULL__L8*/ meltfnum[7] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L9*/ meltfnum[8] = 0;
    /*^clear */
	   /*clear *//*_#OR___L10*/ meltfnum[9] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L12*/ meltfnum[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V24*/ meltfptr[23] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7674:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7674:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L5*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_FETCH_PREDEFINED", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_161_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un *
						      meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un *
						      meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 35
    melt_ptr_t mcfr_varptr[35];
#define MELTFRAM_NBVARNUM 15
    long mcfr_varnum[15];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED_st *)
	meltfirstargp_;
      /* use arguments meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 35; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED nbval 35*/
  meltfram__.mcfr_nbvar = 35 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MEXPAND_STORE_PREDEFINED", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-macro.melt:7703:/ getarg");
 /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7704:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7704:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7704:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7704;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_store_predefined sexpr";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SEXPR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7704:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7704:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7705:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SEXPR */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-macro.melt:7705:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7705:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sexpr"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7705) ? (7705) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7705:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7706:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-macro.melt:7706:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7706:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7706) ? (7706) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7706:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NULL__L5*/ meltfnum[1] =
      (( /*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7707:/ cond");
    /*cond */ if ( /*_#NULL__L5*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^compute */
	  /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[12] =
	    ( /*!MACROEXPAND_1 */ meltfrout->tabval[3]);;
	  /*_._IF___V12*/ meltfptr[10] = /*_.SETQ___V13*/ meltfptr[12];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7707:/ clear");
	     /*clear *//*_.SETQ___V13*/ meltfptr[12] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V12*/ meltfptr[10] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7708:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_OBJECT__L6*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MODCTX__V5*/ meltfptr[4])) ==
	 MELTOBMAG_OBJECT);;
      MELT_LOCATION ("warmelt-macro.melt:7708:/ cond");
      /*cond */ if ( /*_#IS_OBJECT__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-macro.melt:7708:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-macro.melt")
				  ? ("warmelt-macro.melt") : __FILE__,
				  (7708) ? (7708) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-macro.melt:7708:/ clear");
	     /*clear *//*_#IS_OBJECT__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7709:/ quasiblock");


    MELT_LOCATION ("warmelt-macro.melt:7710:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXP_CONTENTS");
  /*_.CONT__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-macro.melt:7711:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.LOC__V18*/ meltfptr[17] = slot;
    };
    ;
 /*_.LIST_FIRST__V19*/ meltfptr[18] =
      (melt_list_first ((melt_ptr_t) ( /*_.CONT__V17*/ meltfptr[16])));;
    /*^compute */
 /*_.CURPAIR__V20*/ meltfptr[19] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.LIST_FIRST__V19*/ meltfptr[18])));;
    /*^compute */
 /*_.PAIR_HEAD__V21*/ meltfptr[20] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7713:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.SPREDEF__V22*/ meltfptr[21] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V21*/ meltfptr[20]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*_.SVAL__V23*/ meltfptr[22] = ( /*nil */ NULL);;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#NULL__L7*/ meltfnum[0] =
      (( /*_.CURPAIR__V20*/ meltfptr[19]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7716:/ cond");
    /*cond */ if ( /*_#NULL__L7*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7717:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(STORE_PREDEFINED <predef-name-or-rank> <value>) missing predef"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_TAIL__V24*/ meltfptr[23] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    MELT_LOCATION ("warmelt-macro.melt:7718:/ compute");
    /*_.CURPAIR__V20*/ meltfptr[19] = /*_.SETQ___V25*/ meltfptr[24] =
      /*_.PAIR_TAIL__V24*/ meltfptr[23];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#NULL__L8*/ meltfnum[7] =
      (( /*_.CURPAIR__V20*/ meltfptr[19]) == NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7719:/ cond");
    /*cond */ if ( /*_#NULL__L8*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7720:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(STORE_PREDEFINED <predef-name-or-rank> <value>) missing predef"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
 /*_.PAIR_HEAD__V26*/ meltfptr[25] =
      (melt_pair_head ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7721:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.MEXPANDER__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.MODCTX__V5*/ meltfptr[4];
      /*_.MEXPANDER__V27*/ meltfptr[26] =
	melt_apply ((meltclosure_ptr_t) ( /*_.MEXPANDER__V4*/ meltfptr[3]),
		    (melt_ptr_t) ( /*_.PAIR_HEAD__V26*/ meltfptr[25]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    /*^compute */
    /*_.SVAL__V23*/ meltfptr[22] = /*_.SETQ___V28*/ meltfptr[27] =
      /*_.MEXPANDER__V27*/ meltfptr[26];;

    MELT_CHECK_SIGNAL ();
    ;
 /*_.PAIR_TAIL__V29*/ meltfptr[28] =
      (melt_pair_tail ((melt_ptr_t) ( /*_.CURPAIR__V20*/ meltfptr[19])));;
    /*^compute */
 /*_#NOTNULL__L9*/ meltfnum[8] =
      (( /*_.PAIR_TAIL__V29*/ meltfptr[28]) != NULL);;
    MELT_LOCATION ("warmelt-macro.melt:7722:/ cond");
    /*cond */ if ( /*_#NOTNULL__L9*/ meltfnum[8])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7723:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(STORE_PREDEFINED <predef-name-or-rank> <value>) extra argument"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_A__L10*/ meltfnum[9] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.SPREDEF__V22*/ meltfptr[21]),
			   (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					  tabval[4])));;
    MELT_LOCATION ("warmelt-macro.melt:7724:/ cond");
    /*cond */ if ( /*_#IS_A__L10*/ meltfnum[9])	/*then */
      {
	/*^cond.then */
	/*_#OR___L11*/ meltfnum[10] = /*_#IS_A__L10*/ meltfnum[9];;
      }
    else
      {
	MELT_LOCATION ("warmelt-macro.melt:7724:/ cond.else");

	/*^block */
	/*anyblock */
	{

   /*_#IS_INTEGERBOX__L12*/ meltfnum[11] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.SPREDEF__V22*/ meltfptr[21])) ==
	     MELTOBMAG_INT);;
	  /*^compute */
	  /*_#OR___L11*/ meltfnum[10] =
	    /*_#IS_INTEGERBOX__L12*/ meltfnum[11];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-macro.melt:7724:/ clear");
	     /*clear *//*_#IS_INTEGERBOX__L12*/ meltfnum[11] = 0;
	}
	;
      }
    ;
 /*_#NOT__L13*/ meltfnum[11] =
      (!( /*_#OR___L11*/ meltfnum[10]));;
    MELT_LOCATION ("warmelt-macro.melt:7724:/ cond");
    /*cond */ if ( /*_#NOT__L13*/ meltfnum[11])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-macro.melt:7725:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.LOC__V18*/ meltfptr[17]),
			      ("(STORE_PREDEFINED <predef-name-or-rank> <value>) invalid predef"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    MELT_LOCATION ("warmelt-macro.melt:7726:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^quasiblock */


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_STORE_PREDEFINED */ meltfrout->tabval[5])), (4), "CLASS_SOURCE_STORE_PREDEFINED");
  /*_.INST__V32*/ meltfptr[31] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V32*/ meltfptr[31])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V32*/ meltfptr[31]), (1),
			  ( /*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SSTPD_PREDEF",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V32*/ meltfptr[31])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V32*/ meltfptr[31]), (2),
			  ( /*_.SPREDEF__V22*/ meltfptr[21]), "SSTPD_PREDEF");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("checkobj putslot  _ @SSTPD_VALUE",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V32*/ meltfptr[31])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V32*/ meltfptr[31]), (3),
			  ( /*_.SVAL__V23*/ meltfptr[22]), "SSTPD_VALUE");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V32*/ meltfptr[31],
				  "newly made instance");
    ;
    /*_.RES__V31*/ meltfptr[30] = /*_.INST__V32*/ meltfptr[31];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-macro.melt:7731:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L14*/ meltfnum[13] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-macro.melt:7731:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L14*/ meltfnum[13])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L15*/ meltfnum[14] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-macro.melt:7731:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[14];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-macro.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 7731;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "mexpand_store_predefined result";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RES__V31*/ meltfptr[30];
	      /*_.MELT_DEBUG_FUN__V34*/ meltfptr[33] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V33*/ meltfptr[32] =
	      /*_.MELT_DEBUG_FUN__V34*/ meltfptr[33];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-macro.melt:7731:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L15*/ meltfnum[14] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V34*/ meltfptr[33] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V33*/ meltfptr[32] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-macro.melt:7731:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L14*/ meltfnum[13] = 0;
      /*^clear */
	     /*clear *//*_._IF___V33*/ meltfptr[32] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7732:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V31*/ meltfptr[30];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7732:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V30*/ meltfptr[29] = /*_.RETURN___V35*/ meltfptr[33];;

    MELT_LOCATION ("warmelt-macro.melt:7726:/ clear");
	   /*clear *//*_.RES__V31*/ meltfptr[30] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V35*/ meltfptr[33] = 0;
    /*_.LET___V16*/ meltfptr[14] = /*_.LET___V30*/ meltfptr[29];;

    MELT_LOCATION ("warmelt-macro.melt:7709:/ clear");
	   /*clear *//*_.CONT__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.LOC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPAIR__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.SPREDEF__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.SVAL__V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L7*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V25*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L8*/ meltfnum[7] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_HEAD__V26*/ meltfptr[25] = 0;
    /*^clear */
	   /*clear *//*_.MEXPANDER__V27*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.SETQ___V28*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.PAIR_TAIL__V29*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_#NOTNULL__L9*/ meltfnum[8] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L10*/ meltfnum[9] = 0;
    /*^clear */
	   /*clear *//*_#OR___L11*/ meltfnum[10] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L13*/ meltfnum[11] = 0;
    /*^clear */
	   /*clear *//*_.LET___V30*/ meltfptr[29] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-macro.melt:7703:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-macro.melt:7703:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L5*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MEXPAND_STORE_PREDEFINED", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_162_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED */



/**** end of warmelt-macro+06.c ****/
