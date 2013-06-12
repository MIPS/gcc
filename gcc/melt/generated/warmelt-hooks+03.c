/* GCC MELT GENERATED FILE warmelt-hooks+03.c - DO NOT EDIT */
/* secondary MELT generated C file of rank #3 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f3[] =
  MELT_RUN_HASHMD5 /* from melt-run.h */ ;


/**** warmelt-hooks+03.c declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-hooks ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2012 - 2013 Free Software Foundation, Inc.
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


/** ordinary MELT module meltbuild-sources/warmelt-hooks**/
#define MELT_HAS_INITIAL_ENVIRONMENT 1	/*usualmodule */


/**** no MELT module variables ****/

/*** 3 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t
melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdata,
						 melt_ptr_t meltinp0_PREVENV,
						 const char
						 *meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdata,
						      const char
						      *meltinp0_SYMNAMESTR,
						      const char
						      *meltinp1_MODULENAMESTR,
						      melt_ptr_t
						      meltinp2_PARENV);

/*declare MELT called hook #2 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdata,
					       melt_ptr_t meltinp0_SYM,
					       melt_ptr_t meltinp1_VAL,
					       melt_ptr_t meltinp2_CONTENV);

/*** end of 3 MELT called hook declarations ***/

/*** 27 extra MELT c-headers ***/


/** MELT extra c-header 1 : **/


static void
melt_override_gate_callback (void *gccdata, void *userdata ATTRIBUTE_UNUSED)
{
  bool *pgatestatus = (bool *) gccdata;
  bool gatsta = false;
  long oldgatstalng = 0, newgatstalng = 0;
  gcc_assert (pgatestatus != NULL);
  gatsta = *pgatestatus;
  oldgatstalng = newgatstalng = (long) gatsta;
  melthookproc_HOOK_OVERRIDE_GATE (oldgatstalng, &newgatstalng);
  gatsta = (newgatstalng != 0L);
  *pgatestatus = gatsta;
}				/* end of  melt_override_gate_callback */

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 2 : **/

/* flag for PLUGIN_OVERRIDE_GATE */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_override_gate_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 3 : **/

/* flag for PLUGIN_START_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_start_unit_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 4 : **/


void MELT_MODULE_VISIBILITY
melt_startunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			     void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 5 : **/

/* flag for PLUGIN_FINISH_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_finish_unit_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 6 : **/


void MELT_MODULE_VISIBILITY
melt_finishunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			      void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 7 : **/

/* flag for PLUGIN_ALL_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_passes_start_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 8 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				     void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 9 : **/

/* flag for PLUGIN_ALL_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_all_passes_end_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 10 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 11 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_ipa_passes_start_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 12 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					 void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 13 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_ipa_passes_end_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 14 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				       void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 15 : **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_early_gimple_passes_start_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 16 : **/


void MELT_MODULE_VISIBILITY
melt_early_gimple_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					      void *user_data
					      ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 17 : **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_early_gimple_passes_end_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 18 : **/


void MELT_MODULE_VISIBILITY
melt_early_gimple_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					    void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 19 : **/

/* flag for PLUGIN_PRE_GENERICIZE declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_pre_genericize_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 20 : **/


void MELT_MODULE_VISIBILITY
melt_pre_genericize_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 21 : **/

/* flag for PLUGIN_FINISH_TYPE declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_finish_type_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 22 : **/


void MELT_MODULE_VISIBILITY
melt_finish_type_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 23 : **/

/* flag for PLUGIN_FINISH_DECL declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_finish_decl_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 24 : **/


void MELT_MODULE_VISIBILITY
melt_finish_decl_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 25 : **/

/* flag for PLUGIN_PASS_EXECUTION declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_pass_execution_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 26 : **/


void MELT_MODULE_VISIBILITY
melt_pass_execution_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 27 : **/


#include <poll.h>

#define MELT_BUFSIZE 8192


/*** end of 27 extra MELT c-headers ***/

/** declaration of hook melthook_HOOK_LOW_DEBUG_VALUE_AT */
MELT_EXTERN
  void melthook_HOOK_LOW_DEBUG_VALUE_AT (melt_ptr_t melthookdata,
					 melt_ptr_t meltinp0_VAL,
					 const char *meltinp1_FILENAME,
					 long meltinp2_LINENO,
					 const char *meltinp3_MSG,
					 long meltinp4_COUNT);


/* declare hook frame marking routine for melthook_HOOK_LOW_DEBUG_VALUE_AT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_LOW_DEBUG_VALUE_AT_rout (struct
								       melt_callframe_st
								       *,
								       int);




/** declaration of hook melthook_HOOK_OVERRIDE_GATE */
MELT_EXTERN
  void melthook_HOOK_OVERRIDE_GATE (melt_ptr_t melthookdata,
				    long meltinp0_BEFOREGATE,
				    long *meltoutp0_AFTERGATE);


/* declare hook frame marking routine for melthook_HOOK_OVERRIDE_GATE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_OVERRIDE_GATE_rout (struct
								  melt_callframe_st
								  *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_3_WARMELTmiHOOKS_ENABLE_OVERRIDE_GATE (meltclosure_ptr_t meltclosp_,
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
meltrout_4_WARMELTmiHOOKS_MAYBE_DISABLE_OVERRIDE_GATE (meltclosure_ptr_t
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
meltrout_5_WARMELTmiHOOKS_REGISTER_OVERRIDE_GATE_FIRST (meltclosure_ptr_t
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
meltrout_6_WARMELTmiHOOKS_REGISTER_OVERRIDE_GATE_LAST (meltclosure_ptr_t
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
meltrout_7_WARMELTmiHOOKS_UNREGISTER_OVERRIDE_GATE_FIRST (meltclosure_ptr_t
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
meltrout_8_WARMELTmiHOOKS_UNREGISTER_OVERRIDE_GATE_LAST (meltclosure_ptr_t
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
meltrout_9_WARMELTmiHOOKS_LAMBDA_cl1 (meltclosure_ptr_t meltclosp_,
				      melt_ptr_t meltfirstargp_,
				      const melt_argdescr_cell_t
				      meltxargdescr_[],
				      union meltparam_un *meltxargtab_,
				      const melt_argdescr_cell_t
				      meltxresdescr_[],
				      union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_WARMELTmiHOOKS_LAMBDA_cl2 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_WARMELTmiHOOKS_LAMBDA_cl3 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_START_UNIT */
MELT_EXTERN void melthook_HOOK_START_UNIT (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_START_UNIT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_START_UNIT_rout (struct
							       melt_callframe_st
							       *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiHOOKS_ENABLE_START_UNIT (meltclosure_ptr_t meltclosp_,
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
meltrout_14_WARMELTmiHOOKS_AT_START_UNIT_FIRST (meltclosure_ptr_t meltclosp_,
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
meltrout_15_WARMELTmiHOOKS_AT_START_UNIT_LAST (meltclosure_ptr_t meltclosp_,
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
meltrout_16_WARMELTmiHOOKS_LAMBDA_cl4 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiHOOKS_LAMBDA_cl5 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiHOOKS_LAMBDA_cl6 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_FINISH_UNIT */
MELT_EXTERN void melthook_HOOK_FINISH_UNIT (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_FINISH_UNIT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_FINISH_UNIT_rout (struct
								melt_callframe_st
								*, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_WARMELTmiHOOKS_ENABLE_FINISH_UNIT (meltclosure_ptr_t meltclosp_,
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
meltrout_21_WARMELTmiHOOKS_AT_FINISH_UNIT_FIRST (meltclosure_ptr_t meltclosp_,
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
meltrout_22_WARMELTmiHOOKS_AT_FINISH_UNIT_LAST (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);

/** declaration of hook melthook_HOOK_ALL_PASSES_START */
MELT_EXTERN void melthook_HOOK_ALL_PASSES_START (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_ALL_PASSES_START hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_PASSES_START_rout (struct
								     melt_callframe_st
								     *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiHOOKS_ENABLE_ALL_PASSES_START (meltclosure_ptr_t
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
meltrout_25_WARMELTmiHOOKS_REGISTER_ALL_PASSES_START_FIRST (meltclosure_ptr_t
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
meltrout_26_WARMELTmiHOOKS_REGISTER_ALL_PASSES_START_LAST (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_ALL_PASSES_END */
MELT_EXTERN void melthook_HOOK_ALL_PASSES_END (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_ALL_PASSES_END hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_PASSES_END_rout (struct
								   melt_callframe_st
								   *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_WARMELTmiHOOKS_ENABLE_ALL_PASSES_END (meltclosure_ptr_t
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
meltrout_29_WARMELTmiHOOKS_REGISTER_ALL_PASSES_END_FIRST (meltclosure_ptr_t
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
meltrout_30_WARMELTmiHOOKS_REGISTER_ALL_PASSES_END_LAST (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_ALL_IPA_PASSES_START */
MELT_EXTERN void melthook_HOOK_ALL_IPA_PASSES_START (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_ALL_IPA_PASSES_START hook */
void MELT_MODULE_VISIBILITY
melthookmark_HOOK_ALL_IPA_PASSES_START_rout (struct melt_callframe_st *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_32_WARMELTmiHOOKS_ENABLE_ALL_IPA_PASSES_START (meltclosure_ptr_t
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
meltrout_33_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_START_FIRST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_34_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_START_LAST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_ALL_IPA_PASSES_END */
MELT_EXTERN void melthook_HOOK_ALL_IPA_PASSES_END (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_ALL_IPA_PASSES_END hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_IPA_PASSES_END_rout (struct
								       melt_callframe_st
								       *,
								       int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_36_WARMELTmiHOOKS_ENABLE_ALL_IPA_PASSES_END (meltclosure_ptr_t
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
meltrout_37_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_END_FIRST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_38_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_END_LAST (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_EARLY_GIMPLE_PASSES_START */
MELT_EXTERN
  void melthook_HOOK_EARLY_GIMPLE_PASSES_START (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_EARLY_GIMPLE_PASSES_START hook */
void MELT_MODULE_VISIBILITY
melthookmark_HOOK_EARLY_GIMPLE_PASSES_START_rout (struct melt_callframe_st *,
						  int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_40_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_START (meltclosure_ptr_t
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
meltrout_41_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_START_FIRST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_42_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_START_LAST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_EARLY_GIMPLE_PASSES_END */
MELT_EXTERN
  void melthook_HOOK_EARLY_GIMPLE_PASSES_END (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_EARLY_GIMPLE_PASSES_END hook */
void MELT_MODULE_VISIBILITY
melthookmark_HOOK_EARLY_GIMPLE_PASSES_END_rout (struct melt_callframe_st *,
						int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_44_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END (meltclosure_ptr_t
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
meltrout_45_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_47_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_PRE_GENERICIZE */
MELT_EXTERN
  void melthook_HOOK_PRE_GENERICIZE (melt_ptr_t melthookdata,
				     tree meltinp0_TFNDECL);


/* declare hook frame marking routine for melthook_HOOK_PRE_GENERICIZE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_PRE_GENERICIZE_rout (struct
								   melt_callframe_st
								   *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_49_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST (meltclosure_ptr_t
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
meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST (meltclosure_ptr_t
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
meltrout_51_WARMELTmiHOOKS_ENABLE_FINISH_TYPE (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);

/** declaration of hook melthook_HOOK_FINISH_TYPE */
MELT_EXTERN
  void melthook_HOOK_FINISH_TYPE (melt_ptr_t melthookdata,
				  tree meltinp0_TFNDECL);


/* declare hook frame marking routine for melthook_HOOK_FINISH_TYPE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_FINISH_TYPE_rout (struct
								melt_callframe_st
								*, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_53_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST (meltclosure_ptr_t
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
meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST (meltclosure_ptr_t
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
meltrout_55_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS (meltclosure_ptr_t
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
meltrout_56_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST (meltclosure_ptr_t
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
meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST (meltclosure_ptr_t
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
meltrout_58_WARMELTmiHOOKS_ENABLE_FINISH_DECL (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);

/** declaration of hook melthook_HOOK_FINISH_DECL */
MELT_EXTERN
  void melthook_HOOK_FINISH_DECL (melt_ptr_t melthookdata,
				  tree meltinp0_TFNDECL);


/* declare hook frame marking routine for melthook_HOOK_FINISH_DECL hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_FINISH_DECL_rout (struct
								melt_callframe_st
								*, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_60_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST (meltclosure_ptr_t
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
meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_PASS_EXECUTION */
MELT_EXTERN
  void melthook_HOOK_PASS_EXECUTION (melt_ptr_t melthookdata,
				     const char *meltinp0_PASSNAME,
				     long meltinp1_PASSNUM);


/* declare hook frame marking routine for melthook_HOOK_PASS_EXECUTION hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_PASS_EXECUTION_rout (struct
								   melt_callframe_st
								   *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_63_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST (meltclosure_ptr_t
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
meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_GIMPLE_GATE */
MELT_EXTERN long melthook_HOOK_GIMPLE_GATE (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_GIMPLE_GATE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_GIMPLE_GATE_rout (struct
								melt_callframe_st
								*, int);




/** declaration of hook melthook_HOOK_GIMPLE_EXECUTE */
MELT_EXTERN long melthook_HOOK_GIMPLE_EXECUTE (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_GIMPLE_EXECUTE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_GIMPLE_EXECUTE_rout (struct
								   melt_callframe_st
								   *, int);




/** declaration of hook melthook_HOOK_RTL_GATE */
MELT_EXTERN long melthook_HOOK_RTL_GATE (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_RTL_GATE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_RTL_GATE_rout (struct
							     melt_callframe_st
							     *, int);




/** declaration of hook melthook_HOOK_RTL_EXECUTE */
MELT_EXTERN long melthook_HOOK_RTL_EXECUTE (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_RTL_EXECUTE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_RTL_EXECUTE_rout (struct
								melt_callframe_st
								*, int);




/** declaration of hook melthook_HOOK_SIMPLE_IPA_GATE */
MELT_EXTERN long melthook_HOOK_SIMPLE_IPA_GATE (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_SIMPLE_IPA_GATE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_SIMPLE_IPA_GATE_rout (struct
								    melt_callframe_st
								    *, int);




/** declaration of hook melthook_HOOK_SIMPLE_IPA_EXECUTE */
MELT_EXTERN long melthook_HOOK_SIMPLE_IPA_EXECUTE (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_SIMPLE_IPA_EXECUTE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_SIMPLE_IPA_EXECUTE_rout (struct
								       melt_callframe_st
								       *,
								       int);




/** declaration of hook melthook_HOOK_MELT_ATTRIBUTE_DEFINER */
MELT_EXTERN
  void melthook_HOOK_MELT_ATTRIBUTE_DEFINER (melt_ptr_t melthookdata,
					     tree meltinp0_DECL,
					     tree meltinp1_NAME,
					     melt_ptr_t meltinp2_ATTRSEQ,
					     const char *meltinp3_FILELOC,
					     long meltinp4_LINENO);


/* declare hook frame marking routine for melthook_HOOK_MELT_ATTRIBUTE_DEFINER hook */
void MELT_MODULE_VISIBILITY
melthookmark_HOOK_MELT_ATTRIBUTE_DEFINER_rout (struct melt_callframe_st *,
					       int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_72_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST (meltclosure_ptr_t
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
meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_EXIT_FINALIZER */
MELT_EXTERN void melthook_HOOK_EXIT_FINALIZER (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_EXIT_FINALIZER hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_EXIT_FINALIZER_rout (struct
								   melt_callframe_st
								   *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_75_WARMELTmiHOOKS_AT_EXIT_FIRST (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_HANDLE_SIGALRM */
MELT_EXTERN void melthook_HOOK_HANDLE_SIGALRM (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_HANDLE_SIGALRM hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_HANDLE_SIGALRM_rout (struct
								   melt_callframe_st
								   *, int);




/** declaration of hook melthook_HOOK_HANDLE_SIGIO */
MELT_EXTERN void melthook_HOOK_HANDLE_SIGIO (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_HANDLE_SIGIO hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_HANDLE_SIGIO_rout (struct
								 melt_callframe_st
								 *, int);




/** declaration of hook melthook_HOOK_HANDLE_SIGCHLD */
MELT_EXTERN void melthook_HOOK_HANDLE_SIGCHLD (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_HANDLE_SIGCHLD hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_HANDLE_SIGCHLD_rout (struct
								   melt_callframe_st
								   *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER (meltclosure_ptr_t
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
meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL (meltclosure_ptr_t meltclosp_,
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
meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER (meltclosure_ptr_t meltclosp_,
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
meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER (meltclosure_ptr_t
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
meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER (meltclosure_ptr_t
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
meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER (meltclosure_ptr_t
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
meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER (meltclosure_ptr_t
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



MELT_EXTERN void *melt_start_this_module (void *);
struct melt_initial_frame_st;

/*declare opaque initial frame: */
typedef struct melt_initial_frame_st meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiHOOKS__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiHOOKS__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG */
MELT_EXTERN const char meltmodule_WARMELTmiHOOKS__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiHOOKS__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG */



void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_0 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_1 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_2 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_3 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_4 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_5 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_6 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_7 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_8 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__initialmeltchunk_9 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiHOOKS__forward_or_mark_module_start_frame (struct
							     melt_callframe_st
							     *fp,
							     int marking);



/**** warmelt-hooks+03.c implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_)
{
  long current_blocklevel_signals_meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST_melt
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

  struct meltframe_meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST_st
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
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST_st *)
	meltfirstargp_;
      /* use arguments meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST nbval 3*/
  meltfram__.mcfr_nbvar = 3 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("AT_EXIT_LAST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:2241:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:2244:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (( /*!FINAL_EXIT_DELAYED_QUEUE */ meltfrout->
		       tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 3, "DELQU_LAST");
  /*_.LASTLIST__V3*/ meltfptr[2] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:2246:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    /*^locexp */
	    meltgc_append_list ((melt_ptr_t)
				( /*_.LASTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:2244:/ clear");
	   /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("AT_EXIT_LAST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_76_WARMELTmiHOOKS_AT_EXIT_LAST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER (meltclosure_ptr_t
							   meltclosp_,
							   melt_ptr_t
							   meltfirstargp_,
							   const
							   melt_argdescr_cell_t
							   meltxargdescr_[],
							   union meltparam_un
							   * meltxargtab_,
							   const
							   melt_argdescr_cell_t
							   meltxresdescr_[],
							   union meltparam_un
							   * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER_melt
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

  struct meltframe_meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER_st
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
#define MELTFRAM_NBVARPTR 19
    melt_ptr_t mcfr_varptr[19];
#define MELTFRAM_NBVARNUM 10
    long mcfr_varnum[10];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER_st
	 *) meltfirstargp_;
      /* use arguments meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 19; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER nbval 19*/
  meltfram__.mcfr_nbvar = 19 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_INPUT_CHANNEL_HANDLER", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:2472:/ getarg");
 /*_.CLOS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.DATA__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.DATA__V3*/ meltfptr[2])) !=
	      NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_LONG)
    goto meltlab_endgetargs;
 /*_#INCHFD__L1*/ meltfnum[0] = meltxargtab_[1].meltbp_long;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:2483:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:2483:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:2483:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 2483;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"register_input_channel_handler clos=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CLOS__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " data=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.DATA__V3*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = " inchfd=";
	      /*^apply.arg */
	      argtab[8].meltbp_long = /*_#INCHFD__L1*/ meltfnum[0];
	      /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V5*/ meltfptr[4] =
	      /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2483:/ clear");
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
      MELT_LOCATION ("warmelt-hooks.melt:2483:/ quasiblock");


      /*_.PROGN___V7*/ meltfptr[5] = /*_._IF___V5*/ meltfptr[4];;
      /*^compute */
      /*_.IFCPP___V4*/ meltfptr[3] = /*_.PROGN___V7*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:2483:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V5*/ meltfptr[4] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V7*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V4*/ meltfptr[3] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2485:/ quasiblock");


 /*_#GOODINCHFD__L4*/ meltfnum[2] = 0;;

    MELT_CHECK_SIGNAL ();
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_CLOSURE__L5*/ meltfnum[1] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.CLOS__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:2486:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L5*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_#gteqI__L7*/ meltfnum[6] =
	    (( /*_#INCHFD__L1*/ meltfnum[0]) >= (0));;
	  /*^compute */
	  /*_#_IF___L6*/ meltfnum[5] = /*_#gteqI__L7*/ meltfnum[6];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2486:/ clear");
	     /*clear *//*_#gteqI__L7*/ meltfnum[6] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_#_IF___L6*/ meltfnum[5] = 0;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2486:/ cond");
    /*cond */ if ( /*_#_IF___L6*/ meltfnum[5])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2488:/ locexp");
	    /*register_input_channel_handler STATINCHFDCHK__1 */
	    {
	      struct stat STATINCHFDCHK__1_stat;
	      memset (&STATINCHFDCHK__1_stat, 0, sizeof (struct stat));
	      if (fstat
		  ( /*_#INCHFD__L1*/ meltfnum[0], &STATINCHFDCHK__1_stat))
		warning (0, "MELT bad registered input channel fd#%d - %s",
			 (int) /*_#INCHFD__L1*/ meltfnum[0],
			 xstrerror (errno));
	      else
	 /*_#GOODINCHFD__L4*/ meltfnum[2] = 1;
	    } /* end STATINCHFDCHK__1 */ ;
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#NOT__L8*/ meltfnum[6] =
      (!( /*_#GOODINCHFD__L4*/ meltfnum[2]));;
    MELT_LOCATION ("warmelt-hooks.melt:2500:/ cond");
    /*cond */ if ( /*_#NOT__L8*/ meltfnum[6])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^quasiblock */


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2500:/ locexp");
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
	  /*_._IF___V9*/ meltfptr[5] = /*_.RETURN___V10*/ meltfptr[9];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2500:/ clear");
	     /*clear *//*_.RETURN___V10*/ meltfptr[9] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V9*/ meltfptr[5] = NULL;;
      }
    ;
    /*^compute */
    /*_.LET___V8*/ meltfptr[4] = /*_._IF___V9*/ meltfptr[5];;

    MELT_LOCATION ("warmelt-hooks.melt:2485:/ clear");
	   /*clear *//*_#GOODINCHFD__L4*/ meltfnum[2] = 0;
    /*^clear */
	   /*clear *//*_#IS_CLOSURE__L5*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_#_IF___L6*/ meltfnum[5] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L8*/ meltfnum[6] = 0;
    /*^clear */
	   /*clear *//*_._IF___V9*/ meltfptr[5] = 0;
    MELT_LOCATION ("warmelt-hooks.melt:2501:/ quasiblock");


 /*_.INSBUF__V11*/ meltfptr[9] =
      (melt_ptr_t)
      meltgc_new_strbuf ((meltobject_ptr_t)
			 (( /*!DISCR_STRBUF */ meltfrout->tabval[1])),
			 (const char *) 0);;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2502:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_INPUT_CHANNEL_HANDLER */ meltfrout->tabval[2])), (4), "CLASS_INPUT_CHANNEL_HANDLER");
  /*_.INST__V13*/ meltfptr[12] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @INCH_SBUF",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V13*/ meltfptr[12])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V13*/ meltfptr[12]), (1),
			  ( /*_.INSBUF__V11*/ meltfptr[9]), "INCH_SBUF");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @INCH_CLOS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V13*/ meltfptr[12])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V13*/ meltfptr[12]), (2),
			  ( /*_.CLOS__V2*/ meltfptr[1]), "INCH_CLOS");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @INCH_DATA",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V13*/ meltfptr[12])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V13*/ meltfptr[12]), (3),
			  ( /*_.DATA__V3*/ meltfptr[2]), "INCH_DATA");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V13*/ meltfptr[12],
				  "newly made instance");
    ;
    /*_.INCHDLR__V12*/ meltfptr[5] = /*_.INST__V13*/ meltfptr[12];;
    MELT_LOCATION ("warmelt-hooks.melt:2506:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!INITIAL_SYSTEM_DATA */
					  meltfrout->tabval[3])),
					(melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[4])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*!INITIAL_SYSTEM_DATA */ meltfrout->
			   tabval[3])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 12, "SYSDATA_INCHANNEL_DATA");
   /*_.INBUCK__V14*/ meltfptr[13] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.INBUCK__V14*/ meltfptr[13] = NULL;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:2508:/ locexp");
      melt_put_int ((melt_ptr_t) ( /*_.INCHDLR__V12*/ meltfptr[5]),
		    ( /*_#INCHFD__L1*/ meltfnum[0]));
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_BUCKETLONG__L9*/ meltfnum[2] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.INBUCK__V14*/ meltfptr[13])) ==
       MELTOBMAG_BUCKETLONGS);;
    /*^compute */
 /*_#NOT__L10*/ meltfnum[1] =
      (!( /*_#IS_BUCKETLONG__L9*/ meltfnum[2]));;
    MELT_LOCATION ("warmelt-hooks.melt:2509:/ cond");
    /*cond */ if ( /*_#NOT__L10*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:2510:/ quasiblock");


   /*_.NEWINBUCK__V17*/ meltfptr[16] =
	    (meltgc_new_longsbucket
	     ((meltobject_ptr_t)
	      ( /*!DISCR_BUCKET_LONGS */ meltfrout->tabval[5]), (50)));;
	  MELT_LOCATION ("warmelt-hooks.melt:2512:/ compute");
	  /*_.INBUCK__V14*/ meltfptr[13] = /*_.SETQ___V18*/ meltfptr[17] =
	    /*_.NEWINBUCK__V17*/ meltfptr[16];;
	  /*_.LET___V16*/ meltfptr[15] = /*_.SETQ___V18*/ meltfptr[17];;

	  MELT_LOCATION ("warmelt-hooks.melt:2510:/ clear");
	     /*clear *//*_.NEWINBUCK__V17*/ meltfptr[16] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V18*/ meltfptr[17] = 0;
	  /*_._IF___V15*/ meltfptr[14] = /*_.LET___V16*/ meltfptr[15];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2509:/ clear");
	     /*clear *//*_.LET___V16*/ meltfptr[15] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V15*/ meltfptr[14] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2513:/ quasiblock");


 /*_.UPDATEDINBUCK__V19*/ meltfptr[16] =
      meltgc_longsbucket_put ((melt_ptr_t) /*_.INBUCK__V14*/ meltfptr[13],
			      ( /*_#INCHFD__L1*/ meltfnum[0]),
			      (melt_ptr_t) /*_.INCHDLR__V12*/ meltfptr[5]);;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2515:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!INITIAL_SYSTEM_DATA */
					  meltfrout->tabval[3])),
					(melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[4])))
      )				/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @SYSDATA_INCHANNEL_DATA",
			  melt_magic_discr ((melt_ptr_t)
					    (( /*!INITIAL_SYSTEM_DATA */
					      meltfrout->tabval[3]))) ==
			  MELTOBMAG_OBJECT);
	  melt_putfield_object ((( /*!INITIAL_SYSTEM_DATA */ meltfrout->
				  tabval[3])), (12),
				( /*_.UPDATEDINBUCK__V19*/ meltfptr[16]),
				"SYSDATA_INCHANNEL_DATA");
	  ;
	  /*^touch */
	  meltgc_touch (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[3]));
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object (( /*!INITIAL_SYSTEM_DATA */
					 meltfrout->tabval[3]), "put-fields");
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:2516:/ locexp");
      /* FCNTLINCHFDCHK__1 start */
      if (fcntl ( /*_#INCHFD__L1*/ meltfnum[0], F_SETOWN, getpid ()))
	melt_fatal_error
	  ("MELT failed to set ownership (F_SETOWN) fd #%d - %s",
	   (int) /*_#INCHFD__L1*/ meltfnum[0], xstrerror (errno));
      /* end  FCNTLINCHFDCHK__1 */
      ;
    }
    ;

    MELT_LOCATION ("warmelt-hooks.melt:2513:/ clear");
	   /*clear *//*_.UPDATEDINBUCK__V19*/ meltfptr[16] = 0;

    MELT_LOCATION ("warmelt-hooks.melt:2501:/ clear");
	   /*clear *//*_.INSBUF__V11*/ meltfptr[9] = 0;
    /*^clear */
	   /*clear *//*_.INCHDLR__V12*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.INBUCK__V14*/ meltfptr[13] = 0;
    /*^clear */
	   /*clear *//*_#IS_BUCKETLONG__L9*/ meltfnum[2] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L10*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V15*/ meltfptr[14] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:2472:/ clear");
	   /*clear *//*_.IFCPP___V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_.LET___V8*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_INPUT_CHANNEL_HANDLER", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_80_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL_melt
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

  struct meltframe_meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL_st
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
#define MELTFRAM_NBVARPTR 41
    melt_ptr_t mcfr_varptr[41];
#define MELTFRAM_NBVARNUM 19
    long mcfr_varnum[19];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL_st *)
	meltfirstargp_;
      /* use arguments meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 41; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL nbval 41*/
  meltfram__.mcfr_nbvar = 41 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("HANDLE_ALARM_SIGNAL", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:2530:/ block");
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:2531:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!ALARM_BUCKET_REFERENCE */
					  meltfrout->tabval[0])),
					(melt_ptr_t) (( /*!CLASS_REFERENCE */
						       meltfrout->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
   /*_.ALARMBUCK__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.ALARMBUCK__V2*/ meltfptr[1] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2532:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_REFERENCE */
					     meltfrout->tabval[1])), (1),
			      "CLASS_REFERENCE");
  /*_.INST__V4*/ meltfptr[3] =
	newobj;
    };
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V4*/ meltfptr[3],
				  "newly made instance");
    ;
    /*_.GOTALARM_CONT__V3*/ meltfptr[2] = /*_.INST__V4*/ meltfptr[3];;
    /*^compute */
 /*_#CURELTIM__L1*/ meltfnum[0] =
      melt_relative_time_millisec ();;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:2535:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:2535:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:2535:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 2535;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"handle_alarm_signal start alarmbuck=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.ALARMBUCK__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " cureltim=";
	      /*^apply.arg */
	      argtab[6].meltbp_long = /*_#CURELTIM__L1*/ meltfnum[0];
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[2])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V6*/ meltfptr[5] =
	      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2535:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L3*/ meltfnum[2] = 0;
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
      MELT_LOCATION ("warmelt-hooks.melt:2535:/ quasiblock");


      /*_.PROGN___V8*/ meltfptr[6] = /*_._IF___V6*/ meltfptr[5];;
      /*^compute */
      /*_.IFCPP___V5*/ meltfptr[4] = /*_.PROGN___V8*/ meltfptr[6];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:2535:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V8*/ meltfptr[6] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V5*/ meltfptr[4] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2536:/ cond");
    /*cond */ if ( /*_.ALARMBUCK__V2*/ meltfptr[1])	/*then */
      {
	/*^cond.then */
	/*_._IFELSE___V9*/ meltfptr[5] = ( /*nil */ NULL);;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:2536:/ cond.else");

	/*^block */
	/*anyblock */
	{

	  /*^quasiblock */


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2536:/ locexp");
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
	  /*^quasiblock */


	  /*_.PROGN___V11*/ meltfptr[10] = /*_.RETURN___V10*/ meltfptr[6];;
	  /*^compute */
	  /*_._IFELSE___V9*/ meltfptr[5] = /*_.PROGN___V11*/ meltfptr[10];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2536:/ clear");
	     /*clear *//*_.RETURN___V10*/ meltfptr[6] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V11*/ meltfptr[10] = 0;
	}
	;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2537:/ loop");
    /*loop */
    {
    meltlabloop_HANDLELOOP_1:;	/*^loopbody */

      /*^block */
      /*anyblock */
      {


	MELT_CHECK_SIGNAL ();
	;

#if MELT_HAVE_DEBUG
	MELT_LOCATION ("warmelt-hooks.melt:2538:/ cppif.then");
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
     /*_#IS_BUCKETLONG__L4*/ meltfnum[2] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.ALARMBUCK__V2*/ meltfptr[1])) ==
	     MELTOBMAG_BUCKETLONGS);;
	  MELT_LOCATION ("warmelt-hooks.melt:2538:/ cond");
	  /*cond */ if ( /*_#IS_BUCKETLONG__L4*/ meltfnum[2])	/*then */
	    {
	      /*^cond.then */
	      /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-hooks.melt:2538:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {




		{
		  /*^locexp */
		  melt_assert_failed (("check alarmbuck"),
				      ("warmelt-hooks.melt")
				      ? ("warmelt-hooks.melt") : __FILE__,
				      (2538) ? (2538) : __LINE__,
				      __FUNCTION__);
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
	  /*_.IFCPP___V14*/ meltfptr[13] = /*_._IFELSE___V15*/ meltfptr[14];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2538:/ clear");
	       /*clear *//*_#IS_BUCKETLONG__L4*/ meltfnum[2] = 0;
	  /*^clear */
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	}

#else /*MELT_HAVE_DEBUG */
	/*^cppif.else */
	/*_.IFCPP___V14*/ meltfptr[13] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	;

	MELT_CHECK_SIGNAL ();
	;
	MELT_LOCATION ("warmelt-hooks.melt:2542:/ quasiblock");


	/*^newclosure */
		   /*newclosure *//*_.LAMBDA___V17*/ meltfptr[16] =
	  (melt_ptr_t)
	  meltgc_new_closure ((meltobject_ptr_t)
			      (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			      (meltroutine_ptr_t) (( /*!konst_5 */ meltfrout->
						    tabval[5])), (1));
	;
	/*^putclosedv */
	/*putclosv */
	melt_assertmsg ("putclosv checkclo",
			melt_magic_discr ((melt_ptr_t)
					  ( /*_.LAMBDA___V17*/ meltfptr[16]))
			== MELTOBMAG_CLOSURE);
	melt_assertmsg ("putclosv checkoff", 0 >= 0
			&& 0 <
			melt_closure_size ((melt_ptr_t)
					   ( /*_.LAMBDA___V17*/
					    meltfptr[16])));
	((meltclosure_ptr_t) /*_.LAMBDA___V17*/ meltfptr[16])->tabval[0] =
	  (melt_ptr_t) ( /*_.GOTALARM_CONT__V3*/ meltfptr[2]);
	;
	/*_.LAMBDA___V16*/ meltfptr[14] = /*_.LAMBDA___V17*/ meltfptr[16];;
	MELT_LOCATION ("warmelt-hooks.melt:2540:/ cond");
	/*cond */ if (
		       /*ifisa */
		       melt_is_instance_of ((melt_ptr_t)
					    (( /*!INITIAL_SYSTEM_DATA */
					      meltfrout->tabval[3])),
					    (melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[6])))
	  )			/*then */
	  {
	    /*^cond.then */
	    /*^block */
	    /*anyblock */
	    {

	      /*^putslot */
	      /*putslot */
	      melt_assertmsg ("putslot checkobj @SYSDATA_ALARM_HOOK",
			      melt_magic_discr ((melt_ptr_t)
						(( /*!INITIAL_SYSTEM_DATA */
						  meltfrout->tabval[3]))) ==
			      MELTOBMAG_OBJECT);
	      melt_putfield_object ((( /*!INITIAL_SYSTEM_DATA */ meltfrout->
				      tabval[3])), (13),
				    ( /*_.LAMBDA___V16*/ meltfptr[14]),
				    "SYSDATA_ALARM_HOOK");
	      ;
	      /*^touch */
	      meltgc_touch (( /*!INITIAL_SYSTEM_DATA */ meltfrout->
			     tabval[3]));
	      ;
	      /*^touchobj */

	      melt_dbgtrace_written_object (( /*!INITIAL_SYSTEM_DATA */
					     meltfrout->tabval[3]),
					    "put-fields");
	      ;
	      /*epilog */
	    }
	    ;
	  }			/*noelse */
	;

	MELT_CHECK_SIGNAL ();
	;
   /*_#BUCKETLONG_COUNT__L5*/ meltfnum[1] =
	  melt_longsbucket_count ((melt_ptr_t) /*_.ALARMBUCK__V2*/
				  meltfptr[1]);;
	MELT_LOCATION ("warmelt-hooks.melt:2543:/ cond");
	/*cond */ if ( /*_#BUCKETLONG_COUNT__L5*/ meltfnum[1])	/*then */
	  {
	    /*^cond.then */
	    /*_._IFELSE___V18*/ meltfptr[17] = ( /*nil */ NULL);;
	  }
	else
	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2543:/ cond.else");

	    /*^block */
	    /*anyblock */
	    {

	      /*^quasiblock */


	      /*^compute */
     /*_.HANDLELOOP__V13*/ meltfptr[10] = NULL;;

	      /*^exit */
	      /*exit */
	      {
		goto meltlabexit_HANDLELOOP_1;
	      }
	      ;
	      /*^quasiblock */


	      /*epilog */
	    }
	    ;
	  }
	;
	MELT_LOCATION ("warmelt-hooks.melt:2544:/ quasiblock");


   /*_#NEXTIM__L6*/ meltfnum[2] =
	  melt_longsbucket_nth_key ((melt_ptr_t) /*_.ALARMBUCK__V2*/
				    meltfptr[1], (int) 0);;
	/*^compute */
   /*_.NEXTALHD__V19*/ meltfptr[18] =
	  melt_longsbucket_nth_val ((melt_ptr_t) /*_.ALARMBUCK__V2*/
				    meltfptr[1], (int) 0);;

#if MELT_HAVE_DEBUG
	MELT_LOCATION ("warmelt-hooks.melt:2547:/ cppif.then");
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
     /*_#MELT_NEED_DBG__L7*/ meltfnum[6] =
	    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	    0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	    ;;
	  MELT_LOCATION ("warmelt-hooks.melt:2547:/ cond");
	  /*cond */ if ( /*_#MELT_NEED_DBG__L7*/ meltfnum[6])	/*then */
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
		MELT_LOCATION ("warmelt-hooks.melt:2547:/ apply");
		/*apply */
		{
		  union meltparam_un argtab[7];
		  memset (&argtab, 0, sizeof (argtab));
		  /*^apply.arg */
		  argtab[0].meltbp_long =
		    /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
		  /*^apply.arg */
		  argtab[1].meltbp_cstring = "warmelt-hooks.melt";
		  /*^apply.arg */
		  argtab[2].meltbp_long = 2547;
		  /*^apply.arg */
		  argtab[3].meltbp_cstring = "handle_alarm_signal nextim=";
		  /*^apply.arg */
		  argtab[4].meltbp_long = /*_#NEXTIM__L6*/ meltfnum[2];
		  /*^apply.arg */
		  argtab[5].meltbp_cstring = " nextalhd=";
		  /*^apply.arg */
		  argtab[6].meltbp_aptr =
		    (melt_ptr_t *) & /*_.NEXTALHD__V19*/ meltfptr[18];
		  /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] =
		    melt_apply ((meltclosure_ptr_t)
				(( /*!MELT_DEBUG_FUN */ meltfrout->
				  tabval[2])),
				(melt_ptr_t) (( /*nil */ NULL)),
				(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				 MELTBPARSTR_PTR ""), argtab, "",
				(union meltparam_un *) 0);
		}
		;
		/*_._IF___V21*/ meltfptr[20] =
		  /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:2547:/ clear");
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
	  MELT_LOCATION ("warmelt-hooks.melt:2547:/ quasiblock");


	  /*_.PROGN___V23*/ meltfptr[21] = /*_._IF___V21*/ meltfptr[20];;
	  /*^compute */
	  /*_.IFCPP___V20*/ meltfptr[19] = /*_.PROGN___V23*/ meltfptr[21];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2547:/ clear");
	       /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[6] = 0;
	  /*^clear */
	       /*clear *//*_._IF___V21*/ meltfptr[20] = 0;
	  /*^clear */
	       /*clear *//*_.PROGN___V23*/ meltfptr[21] = 0;
	}

#else /*MELT_HAVE_DEBUG */
	/*^cppif.else */
	/*_.IFCPP___V20*/ meltfptr[19] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	;

	MELT_CHECK_SIGNAL ();
	;
	MELT_LOCATION ("warmelt-hooks.melt:2548:/ cond");
	/*cond */ if ( /*_#NEXTIM__L6*/ meltfnum[2])	/*then */
	  {
	    /*^cond.then */
	    /*_._IFELSE___V24*/ meltfptr[20] = ( /*nil */ NULL);;
	  }
	else
	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2548:/ cond.else");

	    /*^block */
	    /*anyblock */
	    {

	      /*^quasiblock */


	      /*^compute */
     /*_.HANDLELOOP__V13*/ meltfptr[10] = NULL;;

	      /*^exit */
	      /*exit */
	      {
		goto meltlabexit_HANDLELOOP_1;
	      }
	      ;
	      /*^quasiblock */


	      /*epilog */
	    }
	    ;
	  }
	;

	MELT_CHECK_SIGNAL ();
	;
   /*_#gtI__L9*/ meltfnum[7] =
	  (( /*_#NEXTIM__L6*/ meltfnum[2]) >
	   ( /*_#CURELTIM__L1*/ meltfnum[0]));;
	MELT_LOCATION ("warmelt-hooks.melt:2549:/ cond");
	/*cond */ if ( /*_#gtI__L9*/ meltfnum[7])	/*then */
	  {
	    /*^cond.then */
	    /*_._IFELSE___V25*/ meltfptr[21] = ( /*nil */ NULL);;
	  }
	else
	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2549:/ cond.else");

	    /*^block */
	    /*anyblock */
	    {

	      /*^quasiblock */


	      /*^compute */
     /*_.HANDLELOOP__V13*/ meltfptr[10] = NULL;;

	      /*^exit */
	      /*exit */
	      {
		goto meltlabexit_HANDLELOOP_1;
	      }
	      ;
	      /*^quasiblock */


	      /*epilog */
	    }
	    ;
	  }
	;

#if MELT_HAVE_DEBUG
	MELT_LOCATION ("warmelt-hooks.melt:2550:/ cppif.then");
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
     /*_#IS_A__L10*/ meltfnum[6] =
	    melt_is_instance_of ((melt_ptr_t)
				 ( /*_.NEXTALHD__V19*/ meltfptr[18]),
				 (melt_ptr_t) (( /*!CLASS_ALARM_HANDLER */
						meltfrout->tabval[7])));;
	  MELT_LOCATION ("warmelt-hooks.melt:2550:/ cond");
	  /*cond */ if ( /*_#IS_A__L10*/ meltfnum[6])	/*then */
	    {
	      /*^cond.then */
	      /*_._IFELSE___V27*/ meltfptr[26] = ( /*nil */ NULL);;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-hooks.melt:2550:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {




		{
		  /*^locexp */
		  melt_assert_failed (("check mextalhd"),
				      ("warmelt-hooks.melt")
				      ? ("warmelt-hooks.melt") : __FILE__,
				      (2550) ? (2550) : __LINE__,
				      __FUNCTION__);
		  ;
		}
		;
		 /*clear *//*_._IFELSE___V27*/ meltfptr[26] = 0;
		/*epilog */
	      }
	      ;
	    }
	  ;
	  /*^compute */
	  /*_.IFCPP___V26*/ meltfptr[25] = /*_._IFELSE___V27*/ meltfptr[26];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2550:/ clear");
	       /*clear *//*_#IS_A__L10*/ meltfnum[6] = 0;
	  /*^clear */
	       /*clear *//*_._IFELSE___V27*/ meltfptr[26] = 0;
	}

#else /*MELT_HAVE_DEBUG */
	/*^cppif.else */
	/*_.IFCPP___V26*/ meltfptr[25] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	;
	/*^compute */
   /*_.BUCKETLONG_REMOVE__V28*/ meltfptr[26] =
	  meltgc_longsbucket_remove ((melt_ptr_t) /*_.ALARMBUCK__V2*/
				     meltfptr[1],
				     ( /*_#NEXTIM__L6*/ meltfnum[2]));;
	MELT_LOCATION ("warmelt-hooks.melt:2551:/ compute");
	/*_.ALARMBUCK__V2*/ meltfptr[1] = /*_.SETQ___V29*/ meltfptr[28] =
	  /*_.BUCKETLONG_REMOVE__V28*/ meltfptr[26];;
	MELT_LOCATION ("warmelt-hooks.melt:2552:/ quasiblock");


	/*^cond */
	/*cond */ if (
		       /*ifisa */
		       melt_is_instance_of ((melt_ptr_t)
					    ( /*_.NEXTALHD__V19*/
					     meltfptr[18]),
					    (melt_ptr_t) (( /*!CLASS_ALARM_HANDLER */ meltfrout->tabval[7])))
	  )			/*then */
	  {
	    /*^cond.then */
	    /*^getslot */
	    {
	      melt_ptr_t slot = NULL, obj = NULL;
	      obj =
		(melt_ptr_t) ( /*_.NEXTALHD__V19*/ meltfptr[18]) /*=obj*/ ;
	      melt_object_get_field (slot, obj, 2, "ALARMH_CLOS");
     /*_.CLOS__V31*/ meltfptr[30] = slot;
	    };
	    ;
	  }
	else
	  {			/*^cond.else */

    /*_.CLOS__V31*/ meltfptr[30] = NULL;;
	  }
	;

	MELT_CHECK_SIGNAL ();
	;
	/*^compute */
   /*_#IS_CLOSURE__L11*/ meltfnum[6] =
	  (melt_magic_discr ((melt_ptr_t) ( /*_.CLOS__V31*/ meltfptr[30])) ==
	   MELTOBMAG_CLOSURE);;
	MELT_LOCATION ("warmelt-hooks.melt:2554:/ cond");
	/*cond */ if ( /*_#IS_CLOSURE__L11*/ meltfnum[6])	/*then */
	  {
	    /*^cond.then */
	    /*^block */
	    /*anyblock */
	    {

	      MELT_LOCATION ("warmelt-hooks.melt:2555:/ quasiblock");



	      MELT_CHECK_SIGNAL ();
	      ;
	      /*^apply */
	      /*apply */
	      {
		/*_.RES__V34*/ meltfptr[33] =
		  melt_apply ((meltclosure_ptr_t)
			      ( /*_.CLOS__V31*/ meltfptr[30]),
			      (melt_ptr_t) ( /*_.NEXTALHD__V19*/
					    meltfptr[18]), (""),
			      (union meltparam_un *) 0, "",
			      (union meltparam_un *) 0);
	      }
	      ;

	      MELT_CHECK_SIGNAL ();
	      ;
	      MELT_LOCATION ("warmelt-hooks.melt:2557:/ cond");
	      /*cond */ if ( /*_.RES__V34*/ meltfptr[33])	/*then */
		{
		  /*^cond.then */
		  /*^block */
		  /*anyblock */
		  {

		    MELT_LOCATION ("warmelt-hooks.melt:2558:/ quasiblock");


		    /*^cond */
		    /*cond */ if (
				   /*ifisa */
				   melt_is_instance_of ((melt_ptr_t)
							( /*_.NEXTALHD__V19*/
							 meltfptr[18]),
							(melt_ptr_t) (( /*!CLASS_ALARM_HANDLER */ meltfrout->tabval[7])))
		      )		/*then */
		      {
			/*^cond.then */
			/*^getslot */
			{
			  melt_ptr_t slot = NULL, obj = NULL;
			  obj =
			    (melt_ptr_t) ( /*_.NEXTALHD__V19*/ meltfptr[18])
			    /*=obj*/ ;
			  melt_object_get_field (slot, obj, 1,
						 "ALARMH_PERIOD");
	 /*_.ALARMH_PERIOD__V37*/ meltfptr[36] = slot;
			};
			;
		      }
		    else
		      {		/*^cond.else */

	/*_.ALARMH_PERIOD__V37*/ meltfptr[36] = NULL;;
		      }
		    ;
		    /*^compute */
       /*_#NEWPERIOD__L12*/ meltfnum[11] =
		      (melt_get_int
		       ((melt_ptr_t)
			( /*_.ALARMH_PERIOD__V37*/ meltfptr[36])));;

		    MELT_CHECK_SIGNAL ();
		    ;
		    /*^compute */
       /*_#gtI__L13*/ meltfnum[12] =
		      (( /*_#NEWPERIOD__L12*/ meltfnum[11]) > (10));;
		    MELT_LOCATION ("warmelt-hooks.melt:2560:/ cond");
		    /*cond */ if ( /*_#gtI__L13*/ meltfnum[12])	/*then */
		      {
			/*^cond.then */
			/*^block */
			/*anyblock */
			{

	 /*_#plI__L14*/ meltfnum[13] =
			    (( /*_#NEWPERIOD__L12*/ meltfnum[11]) +
			     ( /*_#CURELTIM__L1*/ meltfnum[0]));;
			  /*^compute */
	 /*_.BUCKETLONG_PUT__V39*/ meltfptr[38] =
			    meltgc_longsbucket_put ((melt_ptr_t)
						    /*_.ALARMBUCK__V2*/
						    meltfptr[1],
						    ( /*_#plI__L14*/
						     meltfnum[13]),
						    (melt_ptr_t)
						    /*_.NEXTALHD__V19*/
						    meltfptr[18]);;
			  MELT_LOCATION ("warmelt-hooks.melt:2561:/ compute");
			  /*_.ALARMBUCK__V2*/ meltfptr[1] =
			    /*_.SETQ___V40*/ meltfptr[39] =
			    /*_.BUCKETLONG_PUT__V39*/ meltfptr[38];;
			  /*_._IF___V38*/ meltfptr[37] =
			    /*_.SETQ___V40*/ meltfptr[39];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:2560:/ clear");
		   /*clear *//*_#plI__L14*/ meltfnum[13] = 0;
			  /*^clear */
		   /*clear *//*_.BUCKETLONG_PUT__V39*/ meltfptr[38] =
			    0;
			  /*^clear */
		   /*clear *//*_.SETQ___V40*/ meltfptr[39] = 0;
			}
			;
		      }
		    else
		      {		/*^cond.else */

	/*_._IF___V38*/ meltfptr[37] = NULL;;
		      }
		    ;
		    /*^compute */
		    /*_.LET___V36*/ meltfptr[35] =
		      /*_._IF___V38*/ meltfptr[37];;

		    MELT_LOCATION ("warmelt-hooks.melt:2558:/ clear");
		 /*clear *//*_.ALARMH_PERIOD__V37*/ meltfptr[36] = 0;
		    /*^clear */
		 /*clear *//*_#NEWPERIOD__L12*/ meltfnum[11] = 0;
		    /*^clear */
		 /*clear *//*_#gtI__L13*/ meltfnum[12] = 0;
		    /*^clear */
		 /*clear *//*_._IF___V38*/ meltfptr[37] = 0;
		    /*_._IF___V35*/ meltfptr[34] =
		      /*_.LET___V36*/ meltfptr[35];;
		    /*epilog */

		    MELT_LOCATION ("warmelt-hooks.melt:2557:/ clear");
		 /*clear *//*_.LET___V36*/ meltfptr[35] = 0;
		  }
		  ;
		}
	      else
		{		/*^cond.else */

      /*_._IF___V35*/ meltfptr[34] = NULL;;
		}
	      ;
	      /*^compute */
	      /*_.LET___V33*/ meltfptr[32] = /*_._IF___V35*/ meltfptr[34];;

	      MELT_LOCATION ("warmelt-hooks.melt:2555:/ clear");
	       /*clear *//*_.RES__V34*/ meltfptr[33] = 0;
	      /*^clear */
	       /*clear *//*_._IF___V35*/ meltfptr[34] = 0;
	      /*_._IF___V32*/ meltfptr[31] = /*_.LET___V33*/ meltfptr[32];;
	      /*epilog */

	      MELT_LOCATION ("warmelt-hooks.melt:2554:/ clear");
	       /*clear *//*_.LET___V33*/ meltfptr[32] = 0;
	    }
	    ;
	  }
	else
	  {			/*^cond.else */

    /*_._IF___V32*/ meltfptr[31] = NULL;;
	  }
	;
	/*^compute */
	/*_.LET___V30*/ meltfptr[29] = /*_._IF___V32*/ meltfptr[31];;

	MELT_LOCATION ("warmelt-hooks.melt:2552:/ clear");
	     /*clear *//*_.CLOS__V31*/ meltfptr[30] = 0;
	/*^clear */
	     /*clear *//*_#IS_CLOSURE__L11*/ meltfnum[6] = 0;
	/*^clear */
	     /*clear *//*_._IF___V32*/ meltfptr[31] = 0;

	MELT_CHECK_SIGNAL ();
	;
	MELT_LOCATION ("warmelt-hooks.melt:2564:/ cond");
	/*cond */ if (
		       /*ifisa */
		       melt_is_instance_of ((melt_ptr_t)
					    ( /*_.GOTALARM_CONT__V3*/
					     meltfptr[2]),
					    (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[1])))
	  )			/*then */
	  {
	    /*^cond.then */
	    /*^getslot */
	    {
	      melt_ptr_t slot = NULL, obj = NULL;
	      obj =
		(melt_ptr_t) ( /*_.GOTALARM_CONT__V3*/ meltfptr[2]) /*=obj*/ ;
	      melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
     /*_.REFERENCED_VALUE__V41*/ meltfptr[38] = slot;
	    };
	    ;
	  }
	else
	  {			/*^cond.else */

    /*_.REFERENCED_VALUE__V41*/ meltfptr[38] = NULL;;
	  }
	;
	MELT_LOCATION ("warmelt-hooks.melt:2564:/ cond");
	/*cond */ if ( /*_.REFERENCED_VALUE__V41*/ meltfptr[38])	/*then */
	  {
	    /*^cond.then */
	    /*^block */
	    /*anyblock */
	    {


	      MELT_CHECK_SIGNAL ();
	      ;
	      MELT_LOCATION ("warmelt-hooks.melt:2565:/ cond");
	      /*cond */ if (
			     /*ifisa */
			     melt_is_instance_of ((melt_ptr_t)
						  ( /*_.GOTALARM_CONT__V3*/
						   meltfptr[2]),
						  (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[1])))
		)		/*then */
		{
		  /*^cond.then */
		  /*^block */
		  /*anyblock */
		  {

		    /*^putslot */
		    /*putslot */
		    melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
				    melt_magic_discr ((melt_ptr_t)
						      ( /*_.GOTALARM_CONT__V3*/ meltfptr[2])) == MELTOBMAG_OBJECT);
		    melt_putfield_object (( /*_.GOTALARM_CONT__V3*/
					   meltfptr[2]), (0),
					  (( /*nil */ NULL)),
					  "REFERENCED_VALUE");
		    ;
		    /*^touch */
		    meltgc_touch ( /*_.GOTALARM_CONT__V3*/ meltfptr[2]);
		    ;
		    /*^touchobj */

		    melt_dbgtrace_written_object ( /*_.GOTALARM_CONT__V3*/
						  meltfptr[2], "put-fields");
		    ;
		    /*epilog */
		  }
		  ;
		}		/*noelse */
	      ;
	      MELT_LOCATION ("warmelt-hooks.melt:2566:/ again");
	      /*again */
	      {
		goto meltlabloop_HANDLELOOP_1;
	      }
	      ;
	      MELT_LOCATION ("warmelt-hooks.melt:2564:/ quasiblock");


	      /*epilog */
	    }
	    ;
	  }			/*noelse */
	;

	MELT_LOCATION ("warmelt-hooks.melt:2544:/ clear");
	     /*clear *//*_#NEXTIM__L6*/ meltfnum[2] = 0;
	/*^clear */
	     /*clear *//*_.NEXTALHD__V19*/ meltfptr[18] = 0;
	/*^clear */
	     /*clear *//*_.IFCPP___V20*/ meltfptr[19] = 0;
	/*^clear */
	     /*clear *//*_._IFELSE___V24*/ meltfptr[20] = 0;
	/*^clear */
	     /*clear *//*_#gtI__L9*/ meltfnum[7] = 0;
	/*^clear */
	     /*clear *//*_._IFELSE___V25*/ meltfptr[21] = 0;
	/*^clear */
	     /*clear *//*_.IFCPP___V26*/ meltfptr[25] = 0;
	/*^clear */
	     /*clear *//*_.BUCKETLONG_REMOVE__V28*/ meltfptr[26] = 0;
	/*^clear */
	     /*clear *//*_.SETQ___V29*/ meltfptr[28] = 0;
	/*^clear */
	     /*clear *//*_.LET___V30*/ meltfptr[29] = 0;
	/*^clear */
	     /*clear *//*_.REFERENCED_VALUE__V41*/ meltfptr[38] = 0;

	MELT_CHECK_SIGNAL ();
	;
	/*epilog */

	MELT_LOCATION ("warmelt-hooks.melt:2537:/ clear");
	     /*clear *//*_.IFCPP___V14*/ meltfptr[13] = 0;
	/*^clear */
	     /*clear *//*_.LAMBDA___V16*/ meltfptr[14] = 0;
	/*^clear */
	     /*clear *//*_#BUCKETLONG_COUNT__L5*/ meltfnum[1] = 0;
	/*^clear */
	     /*clear *//*_._IFELSE___V18*/ meltfptr[17] = 0;
      }
      ;
      ;
      goto meltlabloop_HANDLELOOP_1;
    meltlabexit_HANDLELOOP_1:;	/*^loopepilog */
      /*loopepilog */
      /*_.FOREVER___V12*/ meltfptr[6] = /*_.HANDLELOOP__V13*/ meltfptr[10];;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2568:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!ALARM_BUCKET_REFERENCE */
					  meltfrout->tabval[0])),
					(melt_ptr_t) (( /*!CLASS_REFERENCE */
						       meltfrout->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
			  melt_magic_discr ((melt_ptr_t)
					    (( /*!ALARM_BUCKET_REFERENCE */
					      meltfrout->tabval[0]))) ==
			  MELTOBMAG_OBJECT);
	  melt_putfield_object ((( /*!ALARM_BUCKET_REFERENCE */ meltfrout->
				  tabval[0])), (0),
				( /*_.ALARMBUCK__V2*/ meltfptr[1]),
				"REFERENCED_VALUE");
	  ;
	  /*^touch */
	  meltgc_touch (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->tabval[0]));
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object (( /*!ALARM_BUCKET_REFERENCE */
					 meltfrout->tabval[0]), "put-fields");
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2569:/ quasiblock");


 /*_#NOWTIM__L15*/ meltfnum[13] =
      melt_relative_time_millisec ();;
    /*^compute */
 /*_#NEXTALARM__L16*/ meltfnum[11] =
      melt_longsbucket_nth_key ((melt_ptr_t) /*_.ALARMBUCK__V2*/ meltfptr[1],
				(int) 0);;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2572:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!INITIAL_SYSTEM_DATA */
					  meltfrout->tabval[3])),
					(melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[6])))
      )				/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @SYSDATA_ALARM_HOOK",
			  melt_magic_discr ((melt_ptr_t)
					    (( /*!INITIAL_SYSTEM_DATA */
					      meltfrout->tabval[3]))) ==
			  MELTOBMAG_OBJECT);
	  melt_putfield_object ((( /*!INITIAL_SYSTEM_DATA */ meltfrout->
				  tabval[3])), (13),
				(( /*!HANDLE_ALARM_SIGNAL */ meltfrout->
				  tabval[8])), "SYSDATA_ALARM_HOOK");
	  ;
	  /*^touch */
	  meltgc_touch (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[3]));
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object (( /*!INITIAL_SYSTEM_DATA */
					 meltfrout->tabval[3]), "put-fields");
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#gtI__L17*/ meltfnum[12] =
      (( /*_#NEXTALARM__L16*/ meltfnum[11]) >
       ( /*_#NOWTIM__L15*/ meltfnum[13]));;
    MELT_LOCATION ("warmelt-hooks.melt:2574:/ cond");
    /*cond */ if ( /*_#gtI__L17*/ meltfnum[12])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_#miI__L18*/ meltfnum[6] =
	    (( /*_#NEXTALARM__L16*/ meltfnum[11]) -
	     ( /*_#NOWTIM__L15*/ meltfnum[13]));;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2576:/ locexp");
	    melt_set_real_timer_millisec ( /*_#miI__L18*/ meltfnum[6]);
	  }
	  ;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2574:/ clear");
	     /*clear *//*_#miI__L18*/ meltfnum[6] = 0;
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
   /*_#gtI__L19*/ meltfnum[2] =
	    (( /*_#NEXTALARM__L16*/ meltfnum[11]) > (0));;
	  MELT_LOCATION ("warmelt-hooks.melt:2577:/ cond");
	  /*cond */ if ( /*_#gtI__L19*/ meltfnum[2])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-hooks.melt:2579:/ locexp");
		  melt_set_real_timer_millisec (30);
		}
		;
		/*epilog */
	      }
	      ;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-hooks.melt:2577:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-hooks.melt:2582:/ locexp");
		  melt_set_real_timer_millisec (0);
		}
		;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-hooks.melt:2583:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[3])),
						    (melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[6])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @SYSDATA_ALARM_HOOK",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[3]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!INITIAL_SYSTEM_DATA */
					      meltfrout->tabval[3])), (13),
					    (( /*nil */ NULL)),
					    "SYSDATA_ALARM_HOOK");
		      ;
		      /*^touch */
		      meltgc_touch (( /*!INITIAL_SYSTEM_DATA */ meltfrout->
				     tabval[3]));
		      ;
		      /*^touchobj */

		      melt_dbgtrace_written_object (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[3]), "put-fields");
		      ;
		      /*epilog */
		    }
		    ;
		  }		/*noelse */
		;
		MELT_LOCATION ("warmelt-hooks.melt:2580:/ quasiblock");


		/*epilog */
	      }
	      ;
	    }
	  ;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2574:/ clear");
	     /*clear *//*_#gtI__L19*/ meltfnum[2] = 0;
	}
	;
      }
    ;

    MELT_LOCATION ("warmelt-hooks.melt:2569:/ clear");
	   /*clear *//*_#NOWTIM__L15*/ meltfnum[13] = 0;
    /*^clear */
	   /*clear *//*_#NEXTALARM__L16*/ meltfnum[11] = 0;
    /*^clear */
	   /*clear *//*_#gtI__L17*/ meltfnum[12] = 0;

    MELT_LOCATION ("warmelt-hooks.melt:2531:/ clear");
	   /*clear *//*_.ALARMBUCK__V2*/ meltfptr[1] = 0;
    /*^clear */
	   /*clear *//*_.GOTALARM_CONT__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_#CURELTIM__L1*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V5*/ meltfptr[4] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V9*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.FOREVER___V12*/ meltfptr[6] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("HANDLE_ALARM_SIGNAL", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_81_WARMELTmiHOOKS_HANDLE_ALARM_SIGNAL */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un * meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7_melt =
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

  struct meltframe_meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7_st
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
#define MELTFRAM_NBVARPTR 1
    melt_ptr_t mcfr_varptr[1];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7 is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7_st *)
	meltfirstargp_;
      /* use arguments meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7 output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 1; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7 nbval 1*/
  meltfram__.mcfr_nbvar = 1 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_cl7", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:2542:/ block");
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~GOTALARM_CONT */ meltfclos->
					  tabval[0])),
					(melt_ptr_t) (( /*!CLASS_REFERENCE */
						       meltfrout->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
			  melt_magic_discr ((melt_ptr_t)
					    (( /*~GOTALARM_CONT */ meltfclos->
					      tabval[0]))) ==
			  MELTOBMAG_OBJECT);
	  melt_putfield_object ((( /*~GOTALARM_CONT */ meltfclos->tabval[0])),
				(0),
				(( /*!konst_0_TRUE */ meltfrout->tabval[0])),
				"REFERENCED_VALUE");
	  ;
	  /*^touch */
	  meltgc_touch (( /*~GOTALARM_CONT */ meltfclos->tabval[0]));
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object (( /*~GOTALARM_CONT */ meltfclos->
					 tabval[0]), "put-fields");
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_cl7", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_82_WARMELTmiHOOKS_LAMBDA_cl7 */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER_melt
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

  struct meltframe_meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER_st
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
#define MELTFRAM_NBVARPTR 21
    melt_ptr_t mcfr_varptr[21];
#define MELTFRAM_NBVARNUM 10
    long mcfr_varnum[10];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER_st
	 *) meltfirstargp_;
      /* use arguments meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 21; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER nbval 21*/
  meltfram__.mcfr_nbvar = 21 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_ALARM_TIMER", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:2587:/ getarg");
 /*_.CLOS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_LONG)
    goto meltlab_endgetargs;
 /*_#PERIODMS__L1*/ meltfnum[0] = meltxargtab_[0].meltbp_long;

  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.DATA__V3*/ meltfptr[2] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.DATA__V3*/ meltfptr[2])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:2593:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:2593:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:2593:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 2593;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "register_alarm_timer clos=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CLOS__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " periodms=";
	      /*^apply.arg */
	      argtab[6].meltbp_long = /*_#PERIODMS__L1*/ meltfnum[0];
	      /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V5*/ meltfptr[4] =
	      /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2593:/ clear");
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
      MELT_LOCATION ("warmelt-hooks.melt:2593:/ quasiblock");


      /*_.PROGN___V7*/ meltfptr[5] = /*_._IF___V5*/ meltfptr[4];;
      /*^compute */
      /*_.IFCPP___V4*/ meltfptr[3] = /*_.PROGN___V7*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:2593:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V5*/ meltfptr[4] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V7*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V4*/ meltfptr[3] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_CLOSURE__L4*/ meltfnum[2] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.CLOS__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:2594:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L4*/ meltfnum[2])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#gteqI__L5*/ meltfnum[1] =
	    (( /*_#PERIODMS__L1*/ meltfnum[0]) >= (50));;
	  MELT_LOCATION ("warmelt-hooks.melt:2595:/ cond");
	  /*cond */ if ( /*_#gteqI__L5*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-hooks.melt:2596:/ quasiblock");


		/*_.RES__V11*/ meltfptr[10] = ( /*nil */ NULL);;
		/*citerblock BLOCK_SIGNALS */
		{
		  /* block_signals meltcit1__BLKSIGNAL start */
		  long meltcit1__BLKSIGNAL_lev = melt_blocklevel_signals;
		  melt_blocklevel_signals = meltcit1__BLKSIGNAL_lev + 1;



		  MELT_LOCATION ("warmelt-hooks.melt:2600:/ quasiblock");


      /*_.PERIODBOX__V13*/ meltfptr[12] =
		    (meltgc_new_int
		     ((meltobject_ptr_t)
		      (( /*!DISCR_CONSTANT_INTEGER */ meltfrout->tabval[1])),
		      ( /*_#PERIODMS__L1*/ meltfnum[0])));;
		  MELT_LOCATION ("warmelt-hooks.melt:2601:/ cond");
		  /*cond */ if (
				 /*ifisa */
				 melt_is_instance_of ((melt_ptr_t)
						      (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->tabval[3])),
						      (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[4])))
		    )		/*then */
		    {
		      /*^cond.then */
		      /*^getslot */
		      {
			melt_ptr_t slot = NULL, obj = NULL;
			obj =
			  (melt_ptr_t) (( /*!ALARM_BUCKET_REFERENCE */
					 meltfrout->tabval[3])) /*=obj*/ ;
			melt_object_get_field (slot, obj, 0,
					       "REFERENCED_VALUE");
	/*_.REFERENCED_VALUE__V14*/ meltfptr[13] = slot;
		      };
		      ;
		    }
		  else
		    {		/*^cond.else */

       /*_.REFERENCED_VALUE__V14*/ meltfptr[13] = NULL;;
		    }
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2601:/ cond");
		  /*cond */ if ( /*_.REFERENCED_VALUE__V14*/ meltfptr[13])	/*then */
		    {
		      /*^cond.then */
		      /*_.OLDBUCK__V15*/ meltfptr[14] =
			/*_.REFERENCED_VALUE__V14*/ meltfptr[13];;
		    }
		  else
		    {
		      MELT_LOCATION ("warmelt-hooks.melt:2601:/ cond.else");

		      /*^block */
		      /*anyblock */
		      {

	/*_.MAKE_BUCKETLONG__V16*/ meltfptr[15] =
			  (meltgc_new_longsbucket
			   ((meltobject_ptr_t)
			    ( /*!DISCR_BUCKET_LONGS */ meltfrout->tabval[2]),
			    (31)));;
			/*^compute */
			/*_.OLDBUCK__V15*/ meltfptr[14] =
			  /*_.MAKE_BUCKETLONG__V16*/ meltfptr[15];;
			/*epilog */

			MELT_LOCATION ("warmelt-hooks.melt:2601:/ clear");
		  /*clear *//*_.MAKE_BUCKETLONG__V16*/ meltfptr[15] = 0;
		      }
		      ;
		    }
		  ;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2602:/ quasiblock");


		  /*^rawallocobj */
		  /*rawallocobj */
		  {
		    melt_ptr_t newobj = 0;
		    melt_raw_object_create (newobj,
					    (melt_ptr_t) (( /*!CLASS_ALARM_HANDLER */ meltfrout->tabval[5])), (4), "CLASS_ALARM_HANDLER");
       /*_.INST__V18*/ meltfptr[17] =
		      newobj;
		  };
		  ;
		  /*^putslot */
		  /*putslot */
		  melt_assertmsg ("putslot checkobj @ALARMH_PERIOD",
				  melt_magic_discr ((melt_ptr_t)
						    ( /*_.INST__V18*/
						     meltfptr[17])) ==
				  MELTOBMAG_OBJECT);
		  melt_putfield_object (( /*_.INST__V18*/ meltfptr[17]), (1),
					( /*_.PERIODBOX__V13*/ meltfptr[12]),
					"ALARMH_PERIOD");
		  ;
		  /*^putslot */
		  /*putslot */
		  melt_assertmsg ("putslot checkobj @ALARMH_CLOS",
				  melt_magic_discr ((melt_ptr_t)
						    ( /*_.INST__V18*/
						     meltfptr[17])) ==
				  MELTOBMAG_OBJECT);
		  melt_putfield_object (( /*_.INST__V18*/ meltfptr[17]), (2),
					( /*_.CLOS__V2*/ meltfptr[1]),
					"ALARMH_CLOS");
		  ;
		  /*^putslot */
		  /*putslot */
		  melt_assertmsg ("putslot checkobj @ALARMH_DATA",
				  melt_magic_discr ((melt_ptr_t)
						    ( /*_.INST__V18*/
						     meltfptr[17])) ==
				  MELTOBMAG_OBJECT);
		  melt_putfield_object (( /*_.INST__V18*/ meltfptr[17]), (3),
					( /*_.DATA__V3*/ meltfptr[2]),
					"ALARMH_DATA");
		  ;
		  /*^touchobj */

		  melt_dbgtrace_written_object ( /*_.INST__V18*/ meltfptr[17],
						"newly made instance");
		  ;
		  /*_.ALHD__V17*/ meltfptr[15] =
		    /*_.INST__V18*/ meltfptr[17];;
		  /*^compute */
      /*_#CURELTIM__L6*/ meltfnum[5] =
		    melt_relative_time_millisec ();;
		  /*^compute */
      /*_#NEXTIM__L7*/ meltfnum[6] =
		    (( /*_#CURELTIM__L6*/ meltfnum[5]) +
		     ( /*_#PERIODMS__L1*/ meltfnum[0]));;
		  /*^compute */
      /*_.NEWBUCK__V19*/ meltfptr[18] =
		    meltgc_longsbucket_put ((melt_ptr_t) /*_.OLDBUCK__V15*/
					    meltfptr[14],
					    ( /*_#NEXTIM__L7*/ meltfnum[6]),
					    (melt_ptr_t) /*_.ALHD__V17*/
					    meltfptr[15]);;
		  /*^compute */
      /*_#FIRSTKEY__L8*/ meltfnum[7] =
		    melt_longsbucket_nth_key ((melt_ptr_t) /*_.NEWBUCK__V19*/
					      meltfptr[18], (int) 0);;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2610:/ cond");
		  /*cond */ if ( /*_#FIRSTKEY__L8*/ meltfnum[7])	/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

	/*_#miI__L10*/ meltfnum[9] =
			  (( /*_#FIRSTKEY__L8*/ meltfnum[7]) -
			   ( /*_#CURELTIM__L6*/ meltfnum[5]));;
			/*^compute */
			/*_#FIRSTDELAY__L9*/ meltfnum[8] =
			  /*_#miI__L10*/ meltfnum[9];;
			/*epilog */

			MELT_LOCATION ("warmelt-hooks.melt:2610:/ clear");
		  /*clear *//*_#miI__L10*/ meltfnum[9] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

       /*_#FIRSTDELAY__L9*/ meltfnum[8] = 0;;
		    }
		  ;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2612:/ cond");
		  /*cond */ if (
				 /*ifisa */
				 melt_is_instance_of ((melt_ptr_t)
						      (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->tabval[3])),
						      (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[4])))
		    )		/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

			/*^putslot */
			/*putslot */
			melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
					melt_magic_discr ((melt_ptr_t)
							  (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->tabval[3]))) == MELTOBMAG_OBJECT);
			melt_putfield_object ((( /*!ALARM_BUCKET_REFERENCE */
						meltfrout->tabval[3])), (0),
					      ( /*_.NEWBUCK__V19*/
					       meltfptr[18]),
					      "REFERENCED_VALUE");
			;
			/*^touch */
			meltgc_touch (( /*!ALARM_BUCKET_REFERENCE */
				       meltfrout->tabval[3]));
			;
			/*^touchobj */

			melt_dbgtrace_written_object (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->tabval[3]), "put-fields");
			;
			/*epilog */
		      }
		      ;
		    }		/*noelse */
		  ;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2613:/ cond");
		  /*cond */ if (
				 /*ifisa */
				 melt_is_instance_of ((melt_ptr_t)
						      (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[6])),
						      (melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[8])))
		    )		/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

			/*^putslot */
			/*putslot */
			melt_assertmsg
			  ("putslot checkobj @SYSDATA_ALARM_HOOK",
			   melt_magic_discr ((melt_ptr_t)
					     (( /*!INITIAL_SYSTEM_DATA */
					       meltfrout->tabval[6]))) ==
			   MELTOBMAG_OBJECT);
			melt_putfield_object ((( /*!INITIAL_SYSTEM_DATA */
						meltfrout->tabval[6])), (13),
					      (( /*!HANDLE_ALARM_SIGNAL */
						meltfrout->tabval[7])),
					      "SYSDATA_ALARM_HOOK");
			;
			/*^touch */
			meltgc_touch (( /*!INITIAL_SYSTEM_DATA */ meltfrout->
				       tabval[6]));
			;
			/*^touchobj */

			melt_dbgtrace_written_object (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[6]), "put-fields");
			;
			/*epilog */
		      }
		      ;
		    }		/*noelse */
		  ;

		  {
		    MELT_LOCATION ("warmelt-hooks.melt:2614:/ locexp");
		    melt_set_real_timer_millisec ( /*_#FIRSTDELAY__L9*/
						  meltfnum[8]);
		  }
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2615:/ compute");
		  /*_.RES__V11*/ meltfptr[10] =
		    /*_.SETQ___V20*/ meltfptr[19] =
		    /*_.ALHD__V17*/ meltfptr[15];;
		  /*_.LET___V12*/ meltfptr[11] =
		    /*_.SETQ___V20*/ meltfptr[19];;

		  MELT_LOCATION ("warmelt-hooks.melt:2600:/ clear");
		/*clear *//*_.PERIODBOX__V13*/ meltfptr[12] = 0;
		  /*^clear */
		/*clear *//*_.REFERENCED_VALUE__V14*/ meltfptr[13] = 0;
		  /*^clear */
		/*clear *//*_.OLDBUCK__V15*/ meltfptr[14] = 0;
		  /*^clear */
		/*clear *//*_.ALHD__V17*/ meltfptr[15] = 0;
		  /*^clear */
		/*clear *//*_#CURELTIM__L6*/ meltfnum[5] = 0;
		  /*^clear */
		/*clear *//*_#NEXTIM__L7*/ meltfnum[6] = 0;
		  /*^clear */
		/*clear *//*_.NEWBUCK__V19*/ meltfptr[18] = 0;
		  /*^clear */
		/*clear *//*_#FIRSTKEY__L8*/ meltfnum[7] = 0;
		  /*^clear */
		/*clear *//*_#FIRSTDELAY__L9*/ meltfnum[8] = 0;
		  /*^clear */
		/*clear *//*_.SETQ___V20*/ meltfptr[19] = 0;
		  /* block_signals meltcit1__BLKSIGNAL end */
		  melt_blocklevel_signals = meltcit1__BLKSIGNAL_lev;
		  MELT_CHECK_SIGNAL ();


		  /*citerepilog */

		  MELT_LOCATION ("warmelt-hooks.melt:2598:/ clear");
		/*clear *//*_.LET___V12*/ meltfptr[11] = 0;
		}		/*endciterblock BLOCK_SIGNALS */
		;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-hooks.melt:2616:/ quasiblock");


		/*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V11*/ meltfptr[10];;

		{
		  MELT_LOCATION ("warmelt-hooks.melt:2616:/ locexp");
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
		/*_.LET___V10*/ meltfptr[9] =
		  /*_.RETURN___V21*/ meltfptr[12];;

		MELT_LOCATION ("warmelt-hooks.melt:2596:/ clear");
	       /*clear *//*_.RES__V11*/ meltfptr[10] = 0;
		/*^clear */
	       /*clear *//*_.RETURN___V21*/ meltfptr[12] = 0;
		/*_._IF___V9*/ meltfptr[5] = /*_.LET___V10*/ meltfptr[9];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:2595:/ clear");
	       /*clear *//*_.LET___V10*/ meltfptr[9] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_._IF___V9*/ meltfptr[5] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_._IF___V8*/ meltfptr[4] = /*_._IF___V9*/ meltfptr[5];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2594:/ clear");
	     /*clear *//*_#gteqI__L5*/ meltfnum[1] = 0;
	  /*^clear */
	     /*clear *//*_._IF___V9*/ meltfptr[5] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V8*/ meltfptr[4] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2587:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_._IF___V8*/ meltfptr[4];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:2587:/ locexp");
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
	   /*clear *//*_.IFCPP___V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_#IS_CLOSURE__L4*/ meltfnum[2] = 0;
    /*^clear */
	   /*clear *//*_._IF___V8*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_ALARM_TIMER", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_83_WARMELTmiHOOKS_REGISTER_ALARM_TIMER */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER_melt
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

  struct meltframe_meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER_st
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
#define MELTFRAM_NBVARPTR 26
    melt_ptr_t mcfr_varptr[26];
#define MELTFRAM_NBVARNUM 8
    long mcfr_varnum[8];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER_st
	 *) meltfirstargp_;
      /* use arguments meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 26; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER nbval 26*/
  meltfram__.mcfr_nbvar = 26 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("UNREGISTER_ALARM_TIMER", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:2620:/ getarg");
 /*_.TIM__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:2622:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:2622:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:2622:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 2622;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "unregister_alarm_timer tim=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.TIM__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V4*/ meltfptr[3] =
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2622:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V4*/ meltfptr[3] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:2622:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_._IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:2622:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V4*/ meltfptr[3] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V6*/ meltfptr[4] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V3*/ meltfptr[2] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2623:/ cond");
    /*cond */ if ( /*_.TIM__V2*/ meltfptr[1])	/*then */
      {
	/*^cond.then */
	/*_._IFELSE___V7*/ meltfptr[3] = ( /*nil */ NULL);;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:2623:/ cond.else");

	/*^block */
	/*anyblock */
	{

	  /*^quasiblock */


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2623:/ locexp");
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
	  /*^quasiblock */


	  /*_.PROGN___V9*/ meltfptr[8] = /*_.RETURN___V8*/ meltfptr[4];;
	  /*^compute */
	  /*_._IFELSE___V7*/ meltfptr[3] = /*_.PROGN___V9*/ meltfptr[8];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2623:/ clear");
	     /*clear *//*_.RETURN___V8*/ meltfptr[4] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V9*/ meltfptr[8] = 0;
	}
	;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_NOT_A__L3*/ meltfnum[1] =
      !melt_is_instance_of ((melt_ptr_t) ( /*_.TIM__V2*/ meltfptr[1]),
			    (melt_ptr_t) (( /*!CLASS_ALARM_HANDLER */
					   meltfrout->tabval[1])));;
    MELT_LOCATION ("warmelt-hooks.melt:2624:/ cond");
    /*cond */ if ( /*_#IS_NOT_A__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*_._IFELSE___V10*/ meltfptr[4] = ( /*nil */ NULL);;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:2624:/ cond.else");

	/*^block */
	/*anyblock */
	{


#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-hooks.melt:2625:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    MELT_CHECK_SIGNAL ();
	    ;
     /*_#IS_A__L4*/ meltfnum[0] =
	      melt_is_instance_of ((melt_ptr_t) ( /*_.TIM__V2*/ meltfptr[1]),
				   (melt_ptr_t) (( /*!CLASS_ALARM_HANDLER */
						  meltfrout->tabval[1])));;
	    MELT_LOCATION ("warmelt-hooks.melt:2625:/ cond");
	    /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	      {
		/*^cond.then */
		/*_._IFELSE___V12*/ meltfptr[11] = ( /*nil */ NULL);;
	      }
	    else
	      {
		MELT_LOCATION ("warmelt-hooks.melt:2625:/ cond.else");

		/*^block */
		/*anyblock */
		{




		  {
		    /*^locexp */
		    melt_assert_failed (("check tim"),
					("warmelt-hooks.melt")
					? ("warmelt-hooks.melt") : __FILE__,
					(2625) ? (2625) : __LINE__,
					__FUNCTION__);
		    ;
		  }
		  ;
		 /*clear *//*_._IFELSE___V12*/ meltfptr[11] = 0;
		  /*epilog */
		}
		;
	      }
	    ;
	    /*^compute */
	    /*_.IFCPP___V11*/ meltfptr[8] = /*_._IFELSE___V12*/ meltfptr[11];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2625:/ clear");
	       /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_._IFELSE___V12*/ meltfptr[11] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V11*/ meltfptr[8] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:2626:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2626:/ locexp");
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
	  MELT_LOCATION ("warmelt-hooks.melt:2624:/ quasiblock");


	  /*_.PROGN___V14*/ meltfptr[13] = /*_.RETURN___V13*/ meltfptr[11];;
	  /*^compute */
	  /*_._IFELSE___V10*/ meltfptr[4] = /*_.PROGN___V14*/ meltfptr[13];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2624:/ clear");
	     /*clear *//*_.IFCPP___V11*/ meltfptr[8] = 0;
	  /*^clear */
	     /*clear *//*_.RETURN___V13*/ meltfptr[11] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V14*/ meltfptr[13] = 0;
	}
	;
      }
    ;
    /*citerblock BLOCK_SIGNALS */
    {
      /* block_signals meltcit1__BLKSIGNAL start */
      long meltcit1__BLKSIGNAL_lev = melt_blocklevel_signals;
      melt_blocklevel_signals = meltcit1__BLKSIGNAL_lev + 1;



      MELT_LOCATION ("warmelt-hooks.melt:2629:/ quasiblock");


      /*^cond */
      /*cond */ if (
		     /*ifisa */
		     melt_is_instance_of ((melt_ptr_t)
					  (( /*!ALARM_BUCKET_REFERENCE */
					    meltfrout->tabval[2])),
					  (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[3])))
	)			/*then */
	{
	  /*^cond.then */
	  /*^getslot */
	  {
	    melt_ptr_t slot = NULL, obj = NULL;
	    obj =
	      (melt_ptr_t) (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->
			     tabval[2])) /*=obj*/ ;
	    melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
    /*_.OLDBUCK__V15*/ meltfptr[8] = slot;
	  };
	  ;
	}
      else
	{			/*^cond.else */

   /*_.OLDBUCK__V15*/ meltfptr[8] = NULL;;
	}
      ;
      /*^compute */
  /*_#OLDBUCKLEN__L5*/ meltfnum[0] =
	melt_longsbucket_count ((melt_ptr_t) /*_.OLDBUCK__V15*/ meltfptr[8]);;
      /*^compute */
  /*_.NEWBUCK__V16*/ meltfptr[11] =
	(meltgc_new_longsbucket
	 ((meltobject_ptr_t) ( /*!DISCR_BUCKET_LONGS */ meltfrout->tabval[4]),
	  ( /*_#OLDBUCKLEN__L5*/ meltfnum[0])));;

      MELT_CHECK_SIGNAL ();
      ;
      MELT_LOCATION ("warmelt-hooks.melt:2633:/ cond");
      /*cond */ if ( /*_#OLDBUCKLEN__L5*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V17*/ meltfptr[13] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-hooks.melt:2633:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {


	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:2635:/ cond");
	    /*cond */ if (
			   /*ifisa */
			   melt_is_instance_of ((melt_ptr_t)
						(( /*!ALARM_BUCKET_REFERENCE */ meltfrout->tabval[2])),
						(melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[3])))
	      )			/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

		  /*^putslot */
		  /*putslot */
		  melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
				  melt_magic_discr ((melt_ptr_t)
						    (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->tabval[2]))) == MELTOBMAG_OBJECT);
		  melt_putfield_object ((( /*!ALARM_BUCKET_REFERENCE */
					  meltfrout->tabval[2])), (0),
					(( /*nil */ NULL)),
					"REFERENCED_VALUE");
		  ;
		  /*^touch */
		  meltgc_touch (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->
				 tabval[2]));
		  ;
		  /*^touchobj */

		  melt_dbgtrace_written_object (( /*!ALARM_BUCKET_REFERENCE */
						 meltfrout->tabval[2]),
						"put-fields");
		  ;
		  /*epilog */
		}
		;
	      }			/*noelse */
	    ;

	    {
	      MELT_LOCATION ("warmelt-hooks.melt:2636:/ locexp");
	      melt_set_real_timer_millisec (0);
	    }
	    ;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:2637:/ cond");
	    /*cond */ if (
			   /*ifisa */
			   melt_is_instance_of ((melt_ptr_t)
						(( /*!INITIAL_SYSTEM_DATA */
						  meltfrout->tabval[5])),
						(melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[6])))
	      )			/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

		  /*^putslot */
		  /*putslot */
		  melt_assertmsg ("putslot checkobj @SYSDATA_ALARM_HOOK",
				  melt_magic_discr ((melt_ptr_t)
						    (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[5]))) == MELTOBMAG_OBJECT);
		  melt_putfield_object ((( /*!INITIAL_SYSTEM_DATA */
					  meltfrout->tabval[5])), (13),
					(( /*nil */ NULL)),
					"SYSDATA_ALARM_HOOK");
		  ;
		  /*^touch */
		  meltgc_touch (( /*!INITIAL_SYSTEM_DATA */ meltfrout->
				 tabval[5]));
		  ;
		  /*^touchobj */

		  melt_dbgtrace_written_object (( /*!INITIAL_SYSTEM_DATA */
						 meltfrout->tabval[5]),
						"put-fields");
		  ;
		  /*epilog */
		}
		;
	      }			/*noelse */
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:2639:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	    {
	      MELT_LOCATION ("warmelt-hooks.melt:2639:/ locexp");
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
	    MELT_LOCATION ("warmelt-hooks.melt:2633:/ quasiblock");


	    /*_.PROGN___V19*/ meltfptr[18] = /*_.RETURN___V18*/ meltfptr[17];;
	    /*^compute */
	    /*_._IFELSE___V17*/ meltfptr[13] =
	      /*_.PROGN___V19*/ meltfptr[18];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2633:/ clear");
	      /*clear *//*_.RETURN___V18*/ meltfptr[17] = 0;
	    /*^clear */
	      /*clear *//*_.PROGN___V19*/ meltfptr[18] = 0;
	  }
	  ;
	}
      ;
      /*citerblock FOREACH_IN_BUCKETLONG */
      {
	/*foreach_in_bucketlong meltcit2__EACHBUCKLONG */ unsigned
	  meltcit2__EACHBUCKLONG_ix = 0, meltcit2__EACHBUCKLONG_cnt = 0;
  /*_#OLDKEY__L6*/ meltfnum[5] = 0L;
  /*_.OLDALA__V20*/ meltfptr[17] = NULL;
	for (meltcit2__EACHBUCKLONG_ix = 0;
	     /* retrieve in meltcit2__EACHBUCKLONG iteration the count, it might change! */
	     (meltcit2__EACHBUCKLONG_cnt =
	      melt_longsbucket_count ((melt_ptr_t) /*_.OLDBUCK__V15*/
				      meltfptr[8])) > 0
	     && meltcit2__EACHBUCKLONG_ix < meltcit2__EACHBUCKLONG_cnt;
	     meltcit2__EACHBUCKLONG_ix++)
	  {
     /*_#OLDKEY__L6*/ meltfnum[5] = 0L;
     /*_.OLDALA__V20*/ meltfptr[17] = NULL;
	    {
	      struct melt_bucketlongentry_st *meltcit2__EACHBUCKLONG_buent
		=
		((struct meltbucketlongs_st *) /*_.OLDBUCK__V15*/
		 meltfptr[8])->buckl_entab + meltcit2__EACHBUCKLONG_ix;
	      if (!meltcit2__EACHBUCKLONG_buent->ebl_va)
		continue;
       /*_#OLDKEY__L6*/ meltfnum[5] =
		meltcit2__EACHBUCKLONG_buent->ebl_at;
       /*_.OLDALA__V20*/ meltfptr[17] =
		meltcit2__EACHBUCKLONG_buent->ebl_va;
	      meltcit2__EACHBUCKLONG_buent = NULL;
	    }
	    /*foreach_in_bucketlong meltcit2__EACHBUCKLONG body: */




#if MELT_HAVE_DEBUG
	    MELT_LOCATION ("warmelt-hooks.melt:2643:/ cppif.then");
	    /*^block */
	    /*anyblock */
	    {


	      MELT_CHECK_SIGNAL ();
	      ;
     /*_#IS_A__L7*/ meltfnum[6] =
		melt_is_instance_of ((melt_ptr_t)
				     ( /*_.OLDALA__V20*/ meltfptr[17]),
				     (melt_ptr_t) (( /*!CLASS_ALARM_HANDLER */
						    meltfrout->tabval[1])));;
	      MELT_LOCATION ("warmelt-hooks.melt:2643:/ cond");
	      /*cond */ if ( /*_#IS_A__L7*/ meltfnum[6])	/*then */
		{
		  /*^cond.then */
		  /*_._IFELSE___V22*/ meltfptr[21] = ( /*nil */ NULL);;
		}
	      else
		{
		  MELT_LOCATION ("warmelt-hooks.melt:2643:/ cond.else");

		  /*^block */
		  /*anyblock */
		  {




		    {
		      /*^locexp */
		      melt_assert_failed (("check oldala"),
					  ("warmelt-hooks.melt")
					  ? ("warmelt-hooks.melt") : __FILE__,
					  (2643) ? (2643) : __LINE__,
					  __FUNCTION__);
		      ;
		    }
		    ;
		 /*clear *//*_._IFELSE___V22*/ meltfptr[21] = 0;
		    /*epilog */
		  }
		  ;
		}
	      ;
	      /*^compute */
	      /*_.IFCPP___V21*/ meltfptr[18] =
		/*_._IFELSE___V22*/ meltfptr[21];;
	      /*epilog */

	      MELT_LOCATION ("warmelt-hooks.melt:2643:/ clear");
	       /*clear *//*_#IS_A__L7*/ meltfnum[6] = 0;
	      /*^clear */
	       /*clear *//*_._IFELSE___V22*/ meltfptr[21] = 0;
	    }

#else /*MELT_HAVE_DEBUG */
	    /*^cppif.else */
	    /*_.IFCPP___V21*/ meltfptr[18] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	    ;

	    MELT_CHECK_SIGNAL ();
	    ;
	    /*^compute */
   /*_#eqeq__L8*/ meltfnum[6] =
	      (( /*_.OLDALA__V20*/ meltfptr[17]) ==
	       ( /*_.TIM__V2*/ meltfptr[1]));;
	    MELT_LOCATION ("warmelt-hooks.melt:2644:/ cond");
	    /*cond */ if ( /*_#eqeq__L8*/ meltfnum[6])	/*then */
	      {
		/*^cond.then */
		/*_._IFELSE___V23*/ meltfptr[21] = ( /*nil */ NULL);;
	      }
	    else
	      {
		MELT_LOCATION ("warmelt-hooks.melt:2644:/ cond.else");

		/*^block */
		/*anyblock */
		{

     /*_.BUCKETLONG_PUT__V24*/ meltfptr[23] =
		    meltgc_longsbucket_put ((melt_ptr_t) /*_.NEWBUCK__V16*/
					    meltfptr[11],
					    ( /*_#OLDKEY__L6*/ meltfnum[5]),
					    (melt_ptr_t) /*_.OLDALA__V20*/
					    meltfptr[17]);;
		  MELT_LOCATION ("warmelt-hooks.melt:2645:/ compute");
		  /*_.NEWBUCK__V16*/ meltfptr[11] =
		    /*_.SETQ___V25*/ meltfptr[24] =
		    /*_.BUCKETLONG_PUT__V24*/ meltfptr[23];;
		  MELT_LOCATION ("warmelt-hooks.melt:2644:/ quasiblock");


		  /*_.PROGN___V26*/ meltfptr[25] =
		    /*_.SETQ___V25*/ meltfptr[24];;
		  /*^compute */
		  /*_._IFELSE___V23*/ meltfptr[21] =
		    /*_.PROGN___V26*/ meltfptr[25];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:2644:/ clear");
	       /*clear *//*_.BUCKETLONG_PUT__V24*/ meltfptr[23] = 0;
		  /*^clear */
	       /*clear *//*_.SETQ___V25*/ meltfptr[24] = 0;
		  /*^clear */
	       /*clear *//*_.PROGN___V26*/ meltfptr[25] = 0;
		}
		;
	      }
	    ;
	    /* ending foreach_in_bucketlong meltcit2__EACHBUCKLONG */
     /*_#OLDKEY__L6*/ meltfnum[5] = 0L;
     /*_.OLDALA__V20*/ meltfptr[17] = NULL;
	  }			/* end foreach_in_bucketlong meltcit2__EACHBUCKLONG_ix */


	/*citerepilog */

	MELT_LOCATION ("warmelt-hooks.melt:2640:/ clear");
	     /*clear *//*_#OLDKEY__L6*/ meltfnum[5] = 0;
	/*^clear */
	     /*clear *//*_.OLDALA__V20*/ meltfptr[17] = 0;
	/*^clear */
	     /*clear *//*_.IFCPP___V21*/ meltfptr[18] = 0;
	/*^clear */
	     /*clear *//*_#eqeq__L8*/ meltfnum[6] = 0;
	/*^clear */
	     /*clear *//*_._IFELSE___V23*/ meltfptr[21] = 0;
      }				/*endciterblock FOREACH_IN_BUCKETLONG */
      ;

      MELT_CHECK_SIGNAL ();
      ;
      MELT_LOCATION ("warmelt-hooks.melt:2647:/ cond");
      /*cond */ if (
		     /*ifisa */
		     melt_is_instance_of ((melt_ptr_t)
					  (( /*!ALARM_BUCKET_REFERENCE */
					    meltfrout->tabval[2])),
					  (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[3])))
	)			/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

	    /*^putslot */
	    /*putslot */
	    melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
			    melt_magic_discr ((melt_ptr_t)
					      (( /*!ALARM_BUCKET_REFERENCE */
						meltfrout->tabval[2]))) ==
			    MELTOBMAG_OBJECT);
	    melt_putfield_object ((( /*!ALARM_BUCKET_REFERENCE */ meltfrout->
				    tabval[2])), (0),
				  ( /*_.NEWBUCK__V16*/ meltfptr[11]),
				  "REFERENCED_VALUE");
	    ;
	    /*^touch */
	    meltgc_touch (( /*!ALARM_BUCKET_REFERENCE */ meltfrout->
			   tabval[2]));
	    ;
	    /*^touchobj */

	    melt_dbgtrace_written_object (( /*!ALARM_BUCKET_REFERENCE */
					   meltfrout->tabval[2]),
					  "put-fields");
	    ;
	    /*epilog */
	  }
	  ;
	}			/*noelse */
      ;

      MELT_LOCATION ("warmelt-hooks.melt:2629:/ clear");
	    /*clear *//*_.OLDBUCK__V15*/ meltfptr[8] = 0;
      /*^clear */
	    /*clear *//*_#OLDBUCKLEN__L5*/ meltfnum[0] = 0;
      /*^clear */
	    /*clear *//*_.NEWBUCK__V16*/ meltfptr[11] = 0;
      /*^clear */
	    /*clear *//*_._IFELSE___V17*/ meltfptr[13] = 0;
      /* block_signals meltcit1__BLKSIGNAL end */
      melt_blocklevel_signals = meltcit1__BLKSIGNAL_lev;
      MELT_CHECK_SIGNAL ();


      /*citerepilog */
    }				/*endciterblock BLOCK_SIGNALS */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:2620:/ clear");
	   /*clear *//*_.IFCPP___V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V7*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_#IS_NOT_A__L3*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V10*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("UNREGISTER_ALARM_TIMER", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_84_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER_melt
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

  struct meltframe_meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER_st
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
#define MELTFRAM_NBVARPTR 63
    melt_ptr_t mcfr_varptr[63];
#define MELTFRAM_NBVARNUM 27
    long mcfr_varnum[27];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER_st
	 *) meltfirstargp_;
      /* use arguments meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 63; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER nbval 63*/
  meltfram__.mcfr_nbvar = 63 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("CHILD_PROCESS_SIGCHLD_HANDLER", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:2655:/ block");
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:2656:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:2656:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:2656:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 2656;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"child_process_sigchld_handler childproc_bucket_reference=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & ( /*!CHILDPROC_BUCKET_REFERENCE */
				  meltfrout->tabval[1]);
	      /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V3*/ meltfptr[2] =
	      /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2656:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V3*/ meltfptr[2] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:2656:/ quasiblock");


      /*_.PROGN___V5*/ meltfptr[3] = /*_._IF___V3*/ meltfptr[2];;
      /*^compute */
      /*_.IFCPP___V2*/ meltfptr[1] = /*_.PROGN___V5*/ meltfptr[3];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:2656:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V3*/ meltfptr[2] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V5*/ meltfptr[3] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V2*/ meltfptr[1] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2657:/ quasiblock");


 /*_.LISCHPH__V6*/ meltfptr[2] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*!DISCR_LIST */ meltfrout->tabval[2]))));;
    /*^compute */
 /*_.LISWSTAT__V7*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*!DISCR_LIST */ meltfrout->tabval[2]))));;
    MELT_LOCATION ("warmelt-hooks.melt:2659:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!CHILDPROC_BUCKET_REFERENCE */
					  meltfrout->tabval[1])),
					(melt_ptr_t) (( /*!CLASS_REFERENCE */
						       meltfrout->tabval[3])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->
			   tabval[1])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
   /*_.BUCK__V8*/ meltfptr[7] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.BUCK__V8*/ meltfptr[7] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_BUCKETLONG__L3*/ meltfnum[1] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.BUCK__V8*/ meltfptr[7])) ==
       MELTOBMAG_BUCKETLONGS);;
    MELT_LOCATION ("warmelt-hooks.melt:2661:/ cond");
    /*cond */ if ( /*_#IS_BUCKETLONG__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*_._IFELSE___V9*/ meltfptr[8] = ( /*nil */ NULL);;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:2661:/ cond.else");

	/*^block */
	/*anyblock */
	{

	  /*^quasiblock */


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2661:/ locexp");
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
	  /*^quasiblock */


	  /*_.PROGN___V11*/ meltfptr[10] = /*_.RETURN___V10*/ meltfptr[9];;
	  /*^compute */
	  /*_._IFELSE___V9*/ meltfptr[8] = /*_.PROGN___V11*/ meltfptr[10];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2661:/ clear");
	     /*clear *//*_.RETURN___V10*/ meltfptr[9] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V11*/ meltfptr[10] = 0;
	}
	;
      }
    ;
    /*citerblock BLOCK_SIGNALS */
    {
      /* block_signals meltcit1__BLKSIGNAL start */
      long meltcit1__BLKSIGNAL_lev = melt_blocklevel_signals;
      melt_blocklevel_signals = meltcit1__BLKSIGNAL_lev + 1;



      MELT_LOCATION ("warmelt-hooks.melt:2664:/ quasiblock");


  /*_#WAITFAIL__L4*/ meltfnum[0] = 0;;
      /*^compute */
  /*_#PIDSTAT__L5*/ meltfnum[4] = 0;;

      {
	MELT_LOCATION ("warmelt-hooks.melt:2667:/ locexp");
	/* child_process_sigchld_handler WAITPROBCHK__1 */
	/*_#WAITFAIL__L4*/ meltfnum[0] = melt_wait_for_probe (WNOHANG);
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
      MELT_LOCATION ("warmelt-hooks.melt:2672:/ cond");
      /*cond */ if ( /*_#WAITFAIL__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V12*/ meltfptr[9] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-hooks.melt:2672:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {


#if MELT_HAVE_DEBUG
	    MELT_LOCATION ("warmelt-hooks.melt:2673:/ cppif.then");
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
      /*_#MELT_NEED_DBG__L6*/ meltfnum[5] =
		/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		;;
	      MELT_LOCATION ("warmelt-hooks.melt:2673:/ cond");
	      /*cond */ if ( /*_#MELT_NEED_DBG__L6*/ meltfnum[5])	/*then */
		{
		  /*^cond.then */
		  /*^block */
		  /*anyblock */
		  {

	/*_#MELT_CALLCOUNT__L7*/ meltfnum[6] =
		      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		      meltcallcount	/* melt_callcount debugging */
#else
		      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		      ;;

		    MELT_CHECK_SIGNAL ();
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:2673:/ apply");
		    /*apply */
		    {
		      union meltparam_un argtab[4];
		      memset (&argtab, 0, sizeof (argtab));
		      /*^apply.arg */
		      argtab[0].meltbp_long =
			/*_#MELT_CALLCOUNT__L7*/ meltfnum[6];
		      /*^apply.arg */
		      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
		      /*^apply.arg */
		      argtab[2].meltbp_long = 2673;
		      /*^apply.arg */
		      argtab[3].meltbp_cstring =
			"child_process_sigchld_handler waited for probe";
		      /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14] =
			melt_apply ((meltclosure_ptr_t)
				    (( /*!MELT_DEBUG_FUN */ meltfrout->
				      tabval[0])),
				    (melt_ptr_t) (( /*nil */ NULL)),
				    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				     MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""),
				    argtab, "", (union meltparam_un *) 0);
		    }
		    ;
		    /*_._IF___V14*/ meltfptr[13] =
		      /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14];;
		    /*epilog */

		    MELT_LOCATION ("warmelt-hooks.melt:2673:/ clear");
		  /*clear *//*_#MELT_CALLCOUNT__L7*/ meltfnum[6] = 0;
		    /*^clear */
		  /*clear *//*_.MELT_DEBUG_FUN__V15*/ meltfptr[14] = 0;
		  }
		  ;
		}
	      else
		{		/*^cond.else */

       /*_._IF___V14*/ meltfptr[13] = NULL;;
		}
	      ;
	      MELT_LOCATION ("warmelt-hooks.melt:2673:/ quasiblock");


	      /*_.PROGN___V16*/ meltfptr[14] = /*_._IF___V14*/ meltfptr[13];;
	      /*^compute */
	      /*_.IFCPP___V13*/ meltfptr[10] =
		/*_.PROGN___V16*/ meltfptr[14];;
	      /*epilog */

	      MELT_LOCATION ("warmelt-hooks.melt:2673:/ clear");
		/*clear *//*_#MELT_NEED_DBG__L6*/ meltfnum[5] = 0;
	      /*^clear */
		/*clear *//*_._IF___V14*/ meltfptr[13] = 0;
	      /*^clear */
		/*clear *//*_.PROGN___V16*/ meltfptr[14] = 0;
	    }

#else /*MELT_HAVE_DEBUG */
	    /*^cppif.else */
	    /*_.IFCPP___V13*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:2674:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	    {
	      MELT_LOCATION ("warmelt-hooks.melt:2674:/ locexp");
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
	    MELT_LOCATION ("warmelt-hooks.melt:2672:/ quasiblock");


	    /*_.PROGN___V18*/ meltfptr[14] = /*_.RETURN___V17*/ meltfptr[13];;
	    /*^compute */
	    /*_._IFELSE___V12*/ meltfptr[9] = /*_.PROGN___V18*/ meltfptr[14];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2672:/ clear");
	      /*clear *//*_.IFCPP___V13*/ meltfptr[10] = 0;
	    /*^clear */
	      /*clear *//*_.RETURN___V17*/ meltfptr[13] = 0;
	    /*^clear */
	      /*clear *//*_.PROGN___V18*/ meltfptr[14] = 0;
	  }
	  ;
	}
      ;
      /*citerblock FOREACH_IN_BUCKETLONG */
      {
	/*foreach_in_bucketlong meltcit2__EACHBUCKLONG */ unsigned
	  meltcit2__EACHBUCKLONG_ix = 0, meltcit2__EACHBUCKLONG_cnt = 0;
  /*_#KEYPID__L8*/ meltfnum[6] = 0L;
  /*_.CHPH__V19*/ meltfptr[10] = NULL;
	for (meltcit2__EACHBUCKLONG_ix = 0;
	     /* retrieve in meltcit2__EACHBUCKLONG iteration the count, it might change! */
	     (meltcit2__EACHBUCKLONG_cnt =
	      melt_longsbucket_count ((melt_ptr_t) /*_.BUCK__V8*/
				      meltfptr[7])) > 0
	     && meltcit2__EACHBUCKLONG_ix < meltcit2__EACHBUCKLONG_cnt;
	     meltcit2__EACHBUCKLONG_ix++)
	  {
     /*_#KEYPID__L8*/ meltfnum[6] = 0L;
     /*_.CHPH__V19*/ meltfptr[10] = NULL;
	    {
	      struct melt_bucketlongentry_st *meltcit2__EACHBUCKLONG_buent
		=
		((struct meltbucketlongs_st *) /*_.BUCK__V8*/ meltfptr[7])->
		buckl_entab + meltcit2__EACHBUCKLONG_ix;
	      if (!meltcit2__EACHBUCKLONG_buent->ebl_va)
		continue;
       /*_#KEYPID__L8*/ meltfnum[6] =
		meltcit2__EACHBUCKLONG_buent->ebl_at;
       /*_.CHPH__V19*/ meltfptr[10] =
		meltcit2__EACHBUCKLONG_buent->ebl_va;
	      meltcit2__EACHBUCKLONG_buent = NULL;
	    }
	    /*foreach_in_bucketlong meltcit2__EACHBUCKLONG body: */




#if MELT_HAVE_DEBUG
	    MELT_LOCATION ("warmelt-hooks.melt:2678:/ cppif.then");
	    /*^block */
	    /*anyblock */
	    {


	      MELT_CHECK_SIGNAL ();
	      ;
     /*_#IS_A__L9*/ meltfnum[5] =
		melt_is_instance_of ((melt_ptr_t)
				     ( /*_.CHPH__V19*/ meltfptr[10]),
				     (melt_ptr_t) (( /*!CLASS_CHILD_PROCESS_HANDLER */ meltfrout->tabval[4])));;
	      MELT_LOCATION ("warmelt-hooks.melt:2678:/ cond");
	      /*cond */ if ( /*_#IS_A__L9*/ meltfnum[5])	/*then */
		{
		  /*^cond.then */
		  /*_._IFELSE___V21*/ meltfptr[14] = ( /*nil */ NULL);;
		}
	      else
		{
		  MELT_LOCATION ("warmelt-hooks.melt:2678:/ cond.else");

		  /*^block */
		  /*anyblock */
		  {




		    {
		      /*^locexp */
		      melt_assert_failed (("check chph"),
					  ("warmelt-hooks.melt")
					  ? ("warmelt-hooks.melt") : __FILE__,
					  (2678) ? (2678) : __LINE__,
					  __FUNCTION__);
		      ;
		    }
		    ;
		 /*clear *//*_._IFELSE___V21*/ meltfptr[14] = 0;
		    /*epilog */
		  }
		  ;
		}
	      ;
	      /*^compute */
	      /*_.IFCPP___V20*/ meltfptr[13] =
		/*_._IFELSE___V21*/ meltfptr[14];;
	      /*epilog */

	      MELT_LOCATION ("warmelt-hooks.melt:2678:/ clear");
	       /*clear *//*_#IS_A__L9*/ meltfnum[5] = 0;
	      /*^clear */
	       /*clear *//*_._IFELSE___V21*/ meltfptr[14] = 0;
	    }

#else /*MELT_HAVE_DEBUG */
	    /*^cppif.else */
	    /*_.IFCPP___V20*/ meltfptr[13] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	    ;

#if MELT_HAVE_DEBUG
	    MELT_LOCATION ("warmelt-hooks.melt:2679:/ cppif.then");
	    /*^block */
	    /*anyblock */
	    {


	      MELT_CHECK_SIGNAL ();
	      ;
	      MELT_LOCATION ("warmelt-hooks.melt:2680:/ cond");
	      /*cond */ if (
			     /*ifisa */
			     melt_is_instance_of ((melt_ptr_t)
						  ( /*_.CHPH__V19*/
						   meltfptr[10]),
						  (melt_ptr_t) (( /*!CLASS_CHILD_PROCESS_HANDLER */ meltfrout->tabval[4])))
		)		/*then */
		{
		  /*^cond.then */
		  /*^getslot */
		  {
		    melt_ptr_t slot = NULL, obj = NULL;
		    obj =
		      (melt_ptr_t) ( /*_.CHPH__V19*/ meltfptr[10]) /*=obj*/ ;
		    melt_object_get_field (slot, obj, 1, "CHILPROH_PID");
       /*_.CHILPROH_PID__V23*/ meltfptr[22] = slot;
		  };
		  ;
		}
	      else
		{		/*^cond.else */

      /*_.CHILPROH_PID__V23*/ meltfptr[22] = NULL;;
		}
	      ;
	      /*^compute */
     /*_#GET_INT__L10*/ meltfnum[5] =
		(melt_get_int
		 ((melt_ptr_t) ( /*_.CHILPROH_PID__V23*/ meltfptr[22])));;
	      /*^compute */
     /*_#eqeqI__L11*/ meltfnum[10] =
		(( /*_#KEYPID__L8*/ meltfnum[6]) ==
		 ( /*_#GET_INT__L10*/ meltfnum[5]));;
	      MELT_LOCATION ("warmelt-hooks.melt:2679:/ cond");
	      /*cond */ if ( /*_#eqeqI__L11*/ meltfnum[10])	/*then */
		{
		  /*^cond.then */
		  /*_._IFELSE___V24*/ meltfptr[23] = ( /*nil */ NULL);;
		}
	      else
		{
		  MELT_LOCATION ("warmelt-hooks.melt:2679:/ cond.else");

		  /*^block */
		  /*anyblock */
		  {




		    {
		      /*^locexp */
		      melt_assert_failed (("check pid"),
					  ("warmelt-hooks.melt")
					  ? ("warmelt-hooks.melt") : __FILE__,
					  (2679) ? (2679) : __LINE__,
					  __FUNCTION__);
		      ;
		    }
		    ;
		 /*clear *//*_._IFELSE___V24*/ meltfptr[23] = 0;
		    /*epilog */
		  }
		  ;
		}
	      ;
	      /*^compute */
	      /*_.IFCPP___V22*/ meltfptr[14] =
		/*_._IFELSE___V24*/ meltfptr[23];;
	      /*epilog */

	      MELT_LOCATION ("warmelt-hooks.melt:2679:/ clear");
	       /*clear *//*_.CHILPROH_PID__V23*/ meltfptr[22] = 0;
	      /*^clear */
	       /*clear *//*_#GET_INT__L10*/ meltfnum[5] = 0;
	      /*^clear */
	       /*clear *//*_#eqeqI__L11*/ meltfnum[10] = 0;
	      /*^clear */
	       /*clear *//*_._IFELSE___V24*/ meltfptr[23] = 0;
	    }

#else /*MELT_HAVE_DEBUG */
	    /*^cppif.else */
	    /*_.IFCPP___V22*/ meltfptr[14] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	    ;

	    {
	      MELT_LOCATION ("warmelt-hooks.melt:2681:/ locexp");
	      /* child_process_sigchld_handler WAITPIDCHK__1 */
	      {
		pid_t wpid = 0;
		int pstatus = 0;
	 /*_#WAITFAIL__L4*/ meltfnum[0] = 0L;
	 /*_#PIDSTAT__L5*/ meltfnum[4] = 0L;
		wpid =
		  waitpid ((pid_t) /*_#KEYPID__L8*/ meltfnum[6], &pstatus,
			   WNOHANG);
		/*_#WAITFAIL__L4*/ meltfnum[0] =
		  (wpid != (pid_t) /*_#KEYPID__L8*/ meltfnum[6]);
		if (! /*_#WAITFAIL__L4*/ meltfnum[0])
	   /*_#PIDSTAT__L5*/ meltfnum[4] = pstatus;
	      } /* child_process_sigchld_handler end WAITPIDCHK__1 */ ;
	    }
	    ;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:2693:/ cond");
	    /*cond */ if ( /*_#WAITFAIL__L4*/ meltfnum[0])	/*then */
	      {
		/*^cond.then */
		/*_._IFELSE___V25*/ meltfptr[22] = ( /*nil */ NULL);;
	      }
	    else
	      {
		MELT_LOCATION ("warmelt-hooks.melt:2693:/ cond.else");

		/*^block */
		/*anyblock */
		{


		  {
		    MELT_LOCATION ("warmelt-hooks.melt:2694:/ locexp");
		    meltgc_append_list ((melt_ptr_t)
					( /*_.LISCHPH__V6*/ meltfptr[2]),
					(melt_ptr_t) ( /*_.CHPH__V19*/
						      meltfptr[10]));
		  }
		  ;
     /*_.MAKE_INTEGERBOX__V26*/ meltfptr[23] =
		    (meltgc_new_int
		     ((meltobject_ptr_t)
		      (( /*!DISCR_CONSTANT_INTEGER */ meltfrout->tabval[5])),
		      ( /*_#PIDSTAT__L5*/ meltfnum[4])));;

		  {
		    MELT_LOCATION ("warmelt-hooks.melt:2695:/ locexp");
		    meltgc_append_list ((melt_ptr_t)
					( /*_.LISWSTAT__V7*/ meltfptr[3]),
					(melt_ptr_t) ( /*_.MAKE_INTEGERBOX__V26*/ meltfptr[23]));
		  }
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2693:/ quasiblock");


		  /*epilog */

		  /*^clear */
	       /*clear *//*_.MAKE_INTEGERBOX__V26*/ meltfptr[23] = 0;
		}
		;
	      }
	    ;
	    /* ending foreach_in_bucketlong meltcit2__EACHBUCKLONG */
     /*_#KEYPID__L8*/ meltfnum[6] = 0L;
     /*_.CHPH__V19*/ meltfptr[10] = NULL;
	  }			/* end foreach_in_bucketlong meltcit2__EACHBUCKLONG_ix */


	/*citerepilog */

	MELT_LOCATION ("warmelt-hooks.melt:2675:/ clear");
	     /*clear *//*_#KEYPID__L8*/ meltfnum[6] = 0;
	/*^clear */
	     /*clear *//*_.CHPH__V19*/ meltfptr[10] = 0;
	/*^clear */
	     /*clear *//*_.IFCPP___V20*/ meltfptr[13] = 0;
	/*^clear */
	     /*clear *//*_.IFCPP___V22*/ meltfptr[14] = 0;
	/*^clear */
	     /*clear *//*_._IFELSE___V25*/ meltfptr[22] = 0;
      }				/*endciterblock FOREACH_IN_BUCKETLONG */
      ;

      MELT_LOCATION ("warmelt-hooks.melt:2664:/ clear");
	    /*clear *//*_#WAITFAIL__L4*/ meltfnum[0] = 0;
      /*^clear */
	    /*clear *//*_#PIDSTAT__L5*/ meltfnum[4] = 0;
      /*^clear */
	    /*clear *//*_._IFELSE___V12*/ meltfptr[9] = 0;
      /* block_signals meltcit1__BLKSIGNAL end */
      melt_blocklevel_signals = meltcit1__BLKSIGNAL_lev;
      MELT_CHECK_SIGNAL ();


      /*citerepilog */
    }				/*endciterblock BLOCK_SIGNALS */
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2699:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!DISCR_MULTIPLE */ meltfrout->tabval[7]);
      /*_.TUPCHPH__V27*/ meltfptr[23] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_TO_MULTIPLE */ meltfrout->tabval[6])),
		    (melt_ptr_t) ( /*_.LISCHPH__V6*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2700:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!DISCR_MULTIPLE */ meltfrout->tabval[7]);
      /*_.TUPWSTAT__V28*/ meltfptr[9] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_TO_MULTIPLE */ meltfrout->tabval[6])),
		    (melt_ptr_t) ( /*_.LISWSTAT__V7*/ meltfptr[3]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    /*citerblock FOREACH_IN_MULTIPLE */
    {
      /* start foreach_in_multiple meltcit3__EACHTUP */
      long meltcit3__EACHTUP_ln =
	melt_multiple_length ((melt_ptr_t) /*_.TUPCHPH__V27*/ meltfptr[23]);
      for ( /*_#IX__L12*/ meltfnum[5] = 0;
	   ( /*_#IX__L12*/ meltfnum[5] >= 0)
	   && ( /*_#IX__L12*/ meltfnum[5] < meltcit3__EACHTUP_ln);
	/*_#IX__L12*/ meltfnum[5]++)
	{
	  /*_.CURCHPH__V29*/ meltfptr[28] =
	    melt_multiple_nth ((melt_ptr_t)
			       ( /*_.TUPCHPH__V27*/ meltfptr[23]),
			       /*_#IX__L12*/ meltfnum[5]);



	  MELT_LOCATION ("warmelt-hooks.melt:2705:/ quasiblock");


  /*_.MULTIPLE_NTH__V31*/ meltfptr[30] =
	    (melt_multiple_nth
	     ((melt_ptr_t) ( /*_.TUPWSTAT__V28*/ meltfptr[9]),
	      ( /*_#IX__L12*/ meltfnum[5])));;
	  /*^compute */
  /*_#CURWSTAT__L13*/ meltfnum[10] =
	    (melt_get_int
	     ((melt_ptr_t) ( /*_.MULTIPLE_NTH__V31*/ meltfptr[30])));;
	  MELT_LOCATION ("warmelt-hooks.melt:2706:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.CURCHPH__V29*/
					       meltfptr[28]),
					      (melt_ptr_t) (( /*!CLASS_CHILD_PROCESS_HANDLER */ meltfrout->tabval[4])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) ( /*_.CURCHPH__V29*/ meltfptr[28]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 1, "CHILPROH_PID");
    /*_.CHILPROH_PID__V32*/ meltfptr[31] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

   /*_.CHILPROH_PID__V32*/ meltfptr[31] = NULL;;
	    }
	  ;
	  /*^compute */
  /*_#CURPID__L14*/ meltfnum[0] =
	    (melt_get_int
	     ((melt_ptr_t) ( /*_.CHILPROH_PID__V32*/ meltfptr[31])));;
	  MELT_LOCATION ("warmelt-hooks.melt:2707:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.CURCHPH__V29*/
					       meltfptr[28]),
					      (melt_ptr_t) (( /*!CLASS_CHILD_PROCESS_HANDLER */ meltfrout->tabval[4])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) ( /*_.CURCHPH__V29*/ meltfptr[28]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 2, "CHILPROH_CLOS");
    /*_.CURCLOS__V33*/ meltfptr[32] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

   /*_.CURCLOS__V33*/ meltfptr[32] = NULL;;
	    }
	  ;
	  /*^compute */
  /*_#EXITED__L15*/ meltfnum[4] = 0;;
	  /*^compute */
  /*_#EXITSTAT__L16*/ meltfnum[15] = 0;;
	  /*^compute */
  /*_#SIGNALED__L17*/ meltfnum[16] = 0;;
	  /*^compute */
  /*_#TERMSIG__L18*/ meltfnum[17] = 0;;
	  /*^compute */
	  /*_.TERMSIGNAME__V34*/ meltfptr[33] = ( /*nil */ NULL);;

#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-hooks.melt:2714:/ cppif.then");
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
    /*_#MELT_NEED_DBG__L19*/ meltfnum[18] =
	      /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	      ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-hooks.melt:2714:/ cond");
	    /*cond */ if ( /*_#MELT_NEED_DBG__L19*/ meltfnum[18])	/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

      /*_#MELT_CALLCOUNT__L20*/ meltfnum[19] =
		    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		    meltcallcount	/* melt_callcount debugging */
#else
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2714:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[9];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L20*/ meltfnum[19];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-hooks.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 2714;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "child_process_sigchld_handler curchph=";
		    /*^apply.arg */
		    argtab[4].meltbp_aptr =
		      (melt_ptr_t *) & /*_.CURCHPH__V29*/ meltfptr[28];
		    /*^apply.arg */
		    argtab[5].meltbp_cstring = " curwstat=";
		    /*^apply.arg */
		    argtab[6].meltbp_long = /*_#CURWSTAT__L13*/ meltfnum[10];
		    /*^apply.arg */
		    argtab[7].meltbp_cstring = " ix=";
		    /*^apply.arg */
		    argtab[8].meltbp_long = /*_#IX__L12*/ meltfnum[5];
		    /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36] =
		      melt_apply ((meltclosure_ptr_t)
				  (( /*!MELT_DEBUG_FUN */ meltfrout->
				    tabval[0])),
				  (melt_ptr_t) (( /*nil */ NULL)),
				  (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG ""), argtab, "",
				  (union meltparam_un *) 0);
		  }
		  ;
		  /*_._IF___V36*/ meltfptr[35] =
		    /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:2714:/ clear");
		/*clear *//*_#MELT_CALLCOUNT__L20*/ meltfnum[19] = 0;
		  /*^clear */
		/*clear *//*_.MELT_DEBUG_FUN__V37*/ meltfptr[36] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

     /*_._IF___V36*/ meltfptr[35] = NULL;;
	      }
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:2714:/ quasiblock");


	    /*_.PROGN___V38*/ meltfptr[36] = /*_._IF___V36*/ meltfptr[35];;
	    /*^compute */
	    /*_.IFCPP___V35*/ meltfptr[34] = /*_.PROGN___V38*/ meltfptr[36];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2714:/ clear");
	      /*clear *//*_#MELT_NEED_DBG__L19*/ meltfnum[18] = 0;
	    /*^clear */
	      /*clear *//*_._IF___V36*/ meltfptr[35] = 0;
	    /*^clear */
	      /*clear *//*_.PROGN___V38*/ meltfptr[36] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V35*/ meltfptr[34] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;
	  /*citerblock BLOCK_SIGNALS */
	  {
	    /* block_signals meltcit4__BLKSIGNAL start */
	    long meltcit4__BLKSIGNAL_lev = melt_blocklevel_signals;
	    melt_blocklevel_signals = meltcit4__BLKSIGNAL_lev + 1;



	    MELT_LOCATION ("warmelt-hooks.melt:2718:/ quasiblock");


	    /*^cond */
	    /*cond */ if (
			   /*ifisa */
			   melt_is_instance_of ((melt_ptr_t)
						(( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->tabval[1])),
						(melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[3])))
	      )			/*then */
	      {
		/*^cond.then */
		/*^getslot */
		{
		  melt_ptr_t slot = NULL, obj = NULL;
		  obj =
		    (melt_ptr_t) (( /*!CHILDPROC_BUCKET_REFERENCE */
				   meltfrout->tabval[1])) /*=obj*/ ;
		  melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
     /*_.CBUCK__V39*/ meltfptr[35] = slot;
		};
		;
	      }
	    else
	      {			/*^cond.else */

    /*_.CBUCK__V39*/ meltfptr[35] = NULL;;
	      }
	    ;
	    /*^compute */
   /*_.BUCKETLONG_REMOVE__V40*/ meltfptr[36] =
	      meltgc_longsbucket_remove ((melt_ptr_t) /*_.CBUCK__V39*/
					 meltfptr[35],
					 ( /*_#CURPID__L14*/ meltfnum[0]));;
	    MELT_LOCATION ("warmelt-hooks.melt:2720:/ compute");
	    /*_.CBUCK__V39*/ meltfptr[35] = /*_.SETQ___V41*/ meltfptr[40] =
	      /*_.BUCKETLONG_REMOVE__V40*/ meltfptr[36];;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:2721:/ cond");
	    /*cond */ if (
			   /*ifisa */
			   melt_is_instance_of ((melt_ptr_t)
						(( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->tabval[1])),
						(melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[3])))
	      )			/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

		  /*^putslot */
		  /*putslot */
		  melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
				  melt_magic_discr ((melt_ptr_t)
						    (( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->tabval[1]))) == MELTOBMAG_OBJECT);
		  melt_putfield_object ((( /*!CHILDPROC_BUCKET_REFERENCE */
					  meltfrout->tabval[1])), (0),
					( /*_.CBUCK__V39*/ meltfptr[35]),
					"REFERENCED_VALUE");
		  ;
		  /*^touch */
		  meltgc_touch (( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->
				 tabval[1]));
		  ;
		  /*^touchobj */

		  melt_dbgtrace_written_object (( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->tabval[1]), "put-fields");
		  ;
		  /*epilog */
		}
		;
	      }			/*noelse */
	    ;

	    MELT_LOCATION ("warmelt-hooks.melt:2718:/ clear");
	     /*clear *//*_.CBUCK__V39*/ meltfptr[35] = 0;
	    /*^clear */
	     /*clear *//*_.BUCKETLONG_REMOVE__V40*/ meltfptr[36] = 0;
	    /*^clear */
	     /*clear *//*_.SETQ___V41*/ meltfptr[40] = 0;
	    /* block_signals meltcit4__BLKSIGNAL end */
	    melt_blocklevel_signals = meltcit4__BLKSIGNAL_lev;
	    MELT_CHECK_SIGNAL ();


	    /*citerepilog */
	  }			/*endciterblock BLOCK_SIGNALS */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:2723:/ locexp");
	    /* child_process_sigchld_handler LOOKWSTATCHK__1 */
	    {
	      int wstat = (int) /*_#CURWSTAT__L13*/ meltfnum[10];
	      if (WIFEXITED (wstat))
		{
	    /*_#EXITED__L15*/ meltfnum[4] = 1;
	    /*_#EXITSTAT__L16*/ meltfnum[15] = WEXITSTATUS (wstat);
		}
	      else if (WIFSIGNALED (wstat))
		{
	    /*_#SIGNALED__L17*/ meltfnum[16] = 1;
	    /*_#TERMSIG__L18*/ meltfnum[17] = WTERMSIG (wstat);
	    /*_.TERMSIGNAME__V34*/ meltfptr[33] =
		    meltgc_new_string
		    ((meltobject_ptr_t) MELT_PREDEF (DISCR_STRING),
		     strsignal ( /*_#TERMSIG__L18*/ meltfnum[17]));
		}
	    } /* end child_process_sigchld_handler LOOKWSTATCHK__1 */ ;
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:2741:/ cond");
	  /*cond */ if ( /*_#EXITED__L15*/ meltfnum[4])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		MELT_CHECK_SIGNAL ();
		;
    /*_#eqeqI__L21*/ meltfnum[19] =
		  (( /*_#EXITSTAT__L16*/ meltfnum[15]) == (0));;
		MELT_LOCATION ("warmelt-hooks.melt:2743:/ cond");
		/*cond */ if ( /*_#eqeqI__L21*/ meltfnum[19])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {


#if MELT_HAVE_DEBUG
		      MELT_LOCATION ("warmelt-hooks.melt:2745:/ cppif.then");
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
	/*_#MELT_NEED_DBG__L22*/ meltfnum[18] =
			  /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			  ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			  0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			  ;;
			MELT_LOCATION ("warmelt-hooks.melt:2745:/ cond");
			/*cond */ if ( /*_#MELT_NEED_DBG__L22*/ meltfnum[18])	/*then */
			  {
			    /*^cond.then */
			    /*^block */
			    /*anyblock */
			    {

	  /*_#MELT_CALLCOUNT__L23*/ meltfnum[22] =
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
				("warmelt-hooks.melt:2745:/ apply");
			      /*apply */
			      {
				union meltparam_un argtab[5];
				memset (&argtab, 0, sizeof (argtab));
				/*^apply.arg */
				argtab[0].meltbp_long =
				  /*_#MELT_CALLCOUNT__L23*/ meltfnum[22];
				/*^apply.arg */
				argtab[1].meltbp_cstring =
				  "warmelt-hooks.melt";
				/*^apply.arg */
				argtab[2].meltbp_long = 2745;
				/*^apply.arg */
				argtab[3].meltbp_cstring =
				  "child_process_sigchld_handler successful curchph=";
				/*^apply.arg */
				argtab[4].meltbp_aptr =
				  (melt_ptr_t *) & /*_.CURCHPH__V29*/
				  meltfptr[28];
				/*_.MELT_DEBUG_FUN__V46*/ meltfptr[45] =
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
			      /*_._IF___V45*/ meltfptr[44] =
				/*_.MELT_DEBUG_FUN__V46*/ meltfptr[45];;
			      /*epilog */

			      MELT_LOCATION
				("warmelt-hooks.melt:2745:/ clear");
		    /*clear *//*_#MELT_CALLCOUNT__L23*/ meltfnum[22]
				= 0;
			      /*^clear */
		    /*clear *//*_.MELT_DEBUG_FUN__V46*/ meltfptr[45]
				= 0;
			    }
			    ;
			  }
			else
			  {	/*^cond.else */

	 /*_._IF___V45*/ meltfptr[44] = NULL;;
			  }
			;
			MELT_LOCATION
			  ("warmelt-hooks.melt:2745:/ quasiblock");


			/*_.PROGN___V47*/ meltfptr[45] =
			  /*_._IF___V45*/ meltfptr[44];;
			/*^compute */
			/*_.IFCPP___V44*/ meltfptr[40] =
			  /*_.PROGN___V47*/ meltfptr[45];;
			/*epilog */

			MELT_LOCATION ("warmelt-hooks.melt:2745:/ clear");
		  /*clear *//*_#MELT_NEED_DBG__L22*/ meltfnum[18] = 0;
			/*^clear */
		  /*clear *//*_._IF___V45*/ meltfptr[44] = 0;
			/*^clear */
		  /*clear *//*_.PROGN___V47*/ meltfptr[45] = 0;
		      }

#else /*MELT_HAVE_DEBUG */
		      /*^cppif.else */
		      /*_.IFCPP___V44*/ meltfptr[40] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		      ;

		      MELT_CHECK_SIGNAL ();
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:2747:/ apply");
		      /*apply */
		      {
			union meltparam_un argtab[1];
			memset (&argtab, 0, sizeof (argtab));
			/*^apply.arg */
			argtab[0].meltbp_aptr = /*nil */ (melt_ptr_t *) NULL;
			/*_.CURCLOS__V48*/ meltfptr[44] =
			  melt_apply ((meltclosure_ptr_t)
				      ( /*_.CURCLOS__V33*/ meltfptr[32]),
				      (melt_ptr_t) ( /*_.CURCHPH__V29*/
						    meltfptr[28]),
				      (MELTBPARSTR_PTR ""), argtab, "",
				      (union meltparam_un *) 0);
		      }
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:2743:/ quasiblock");


		      /*_.PROGN___V49*/ meltfptr[45] =
			/*_.CURCLOS__V48*/ meltfptr[44];;
		      /*^compute */
		      /*_._IFELSE___V43*/ meltfptr[36] =
			/*_.PROGN___V49*/ meltfptr[45];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:2743:/ clear");
		/*clear *//*_.IFCPP___V44*/ meltfptr[40] = 0;
		      /*^clear */
		/*clear *//*_.CURCLOS__V48*/ meltfptr[44] = 0;
		      /*^clear */
		/*clear *//*_.PROGN___V49*/ meltfptr[45] = 0;
		    }
		    ;
		  }
		else
		  {		/*^cond.else */

		    /*^block */
		    /*anyblock */
		    {


#if MELT_HAVE_DEBUG
		      MELT_LOCATION ("warmelt-hooks.melt:2751:/ cppif.then");
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
	/*_#MELT_NEED_DBG__L24*/ meltfnum[22] =
			  /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			  ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			  0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			  ;;
			MELT_LOCATION ("warmelt-hooks.melt:2751:/ cond");
			/*cond */ if ( /*_#MELT_NEED_DBG__L24*/ meltfnum[22])	/*then */
			  {
			    /*^cond.then */
			    /*^block */
			    /*anyblock */
			    {

	  /*_#MELT_CALLCOUNT__L25*/ meltfnum[18] =
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
				("warmelt-hooks.melt:2751:/ apply");
			      /*apply */
			      {
				union meltparam_un argtab[7];
				memset (&argtab, 0, sizeof (argtab));
				/*^apply.arg */
				argtab[0].meltbp_long =
				  /*_#MELT_CALLCOUNT__L25*/ meltfnum[18];
				/*^apply.arg */
				argtab[1].meltbp_cstring =
				  "warmelt-hooks.melt";
				/*^apply.arg */
				argtab[2].meltbp_long = 2751;
				/*^apply.arg */
				argtab[3].meltbp_cstring =
				  "child_process_sigchld_handler failed curchph=";
				/*^apply.arg */
				argtab[4].meltbp_aptr =
				  (melt_ptr_t *) & /*_.CURCHPH__V29*/
				  meltfptr[28];
				/*^apply.arg */
				argtab[5].meltbp_cstring = " exitstat=";
				/*^apply.arg */
				argtab[6].meltbp_long =
				  /*_#EXITSTAT__L16*/ meltfnum[15];
				/*_.MELT_DEBUG_FUN__V52*/ meltfptr[45] =
				  melt_apply ((meltclosure_ptr_t)
					      (( /*!MELT_DEBUG_FUN */
						meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*nil */ NULL)),
					      (MELTBPARSTR_LONG
					       MELTBPARSTR_CSTRING
					       MELTBPARSTR_LONG
					       MELTBPARSTR_CSTRING
					       MELTBPARSTR_PTR
					       MELTBPARSTR_CSTRING
					       MELTBPARSTR_LONG ""), argtab,
					      "", (union meltparam_un *) 0);
			      }
			      ;
			      /*_._IF___V51*/ meltfptr[44] =
				/*_.MELT_DEBUG_FUN__V52*/ meltfptr[45];;
			      /*epilog */

			      MELT_LOCATION
				("warmelt-hooks.melt:2751:/ clear");
		    /*clear *//*_#MELT_CALLCOUNT__L25*/ meltfnum[18]
				= 0;
			      /*^clear */
		    /*clear *//*_.MELT_DEBUG_FUN__V52*/ meltfptr[45]
				= 0;
			    }
			    ;
			  }
			else
			  {	/*^cond.else */

	 /*_._IF___V51*/ meltfptr[44] = NULL;;
			  }
			;
			MELT_LOCATION
			  ("warmelt-hooks.melt:2751:/ quasiblock");


			/*_.PROGN___V53*/ meltfptr[45] =
			  /*_._IF___V51*/ meltfptr[44];;
			/*^compute */
			/*_.IFCPP___V50*/ meltfptr[40] =
			  /*_.PROGN___V53*/ meltfptr[45];;
			/*epilog */

			MELT_LOCATION ("warmelt-hooks.melt:2751:/ clear");
		  /*clear *//*_#MELT_NEED_DBG__L24*/ meltfnum[22] = 0;
			/*^clear */
		  /*clear *//*_._IF___V51*/ meltfptr[44] = 0;
			/*^clear */
		  /*clear *//*_.PROGN___V53*/ meltfptr[45] = 0;
		      }

#else /*MELT_HAVE_DEBUG */
		      /*^cppif.else */
		      /*_.IFCPP___V50*/ meltfptr[40] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		      ;
		      /*^compute */
      /*_.MAKE_INTEGERBOX__V54*/ meltfptr[44] =
			(meltgc_new_int
			 ((meltobject_ptr_t)
			  (( /*!DISCR_CONSTANT_INTEGER */ meltfrout->
			    tabval[5])),
			  ( /*_#EXITSTAT__L16*/ meltfnum[15])));;

		      MELT_CHECK_SIGNAL ();
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:2753:/ apply");
		      /*apply */
		      {
			union meltparam_un argtab[1];
			memset (&argtab, 0, sizeof (argtab));
			/*^apply.arg */
			argtab[0].meltbp_aptr =
			  (melt_ptr_t *) & /*_.MAKE_INTEGERBOX__V54*/
			  meltfptr[44];
			/*_.CURCLOS__V55*/ meltfptr[45] =
			  melt_apply ((meltclosure_ptr_t)
				      ( /*_.CURCLOS__V33*/ meltfptr[32]),
				      (melt_ptr_t) ( /*_.CURCHPH__V29*/
						    meltfptr[28]),
				      (MELTBPARSTR_PTR ""), argtab, "",
				      (union meltparam_un *) 0);
		      }
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:2749:/ quasiblock");


		      /*_.PROGN___V56*/ meltfptr[55] =
			/*_.CURCLOS__V55*/ meltfptr[45];;
		      /*^compute */
		      /*_._IFELSE___V43*/ meltfptr[36] =
			/*_.PROGN___V56*/ meltfptr[55];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:2743:/ clear");
		/*clear *//*_.IFCPP___V50*/ meltfptr[40] = 0;
		      /*^clear */
		/*clear *//*_.MAKE_INTEGERBOX__V54*/ meltfptr[44] = 0;
		      /*^clear */
		/*clear *//*_.CURCLOS__V55*/ meltfptr[45] = 0;
		      /*^clear */
		/*clear *//*_.PROGN___V56*/ meltfptr[55] = 0;
		    }
		    ;
		  }
		;
		/*_._IFELSE___V42*/ meltfptr[35] =
		  /*_._IFELSE___V43*/ meltfptr[36];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:2741:/ clear");
	      /*clear *//*_#eqeqI__L21*/ meltfnum[19] = 0;
		/*^clear */
	      /*clear *//*_._IFELSE___V43*/ meltfptr[36] = 0;
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
		MELT_LOCATION ("warmelt-hooks.melt:2757:/ cond");
		/*cond */ if ( /*_#SIGNALED__L17*/ meltfnum[16])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {


#if MELT_HAVE_DEBUG
		      MELT_LOCATION ("warmelt-hooks.melt:2758:/ cppif.then");
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
	/*_#MELT_NEED_DBG__L26*/ meltfnum[18] =
			  /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			  ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			  0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			  ;;
			MELT_LOCATION ("warmelt-hooks.melt:2758:/ cond");
			/*cond */ if ( /*_#MELT_NEED_DBG__L26*/ meltfnum[18])	/*then */
			  {
			    /*^cond.then */
			    /*^block */
			    /*anyblock */
			    {

	  /*_#MELT_CALLCOUNT__L27*/ meltfnum[22] =
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
				("warmelt-hooks.melt:2758:/ apply");
			      /*apply */
			      {
				union meltparam_un argtab[7];
				memset (&argtab, 0, sizeof (argtab));
				/*^apply.arg */
				argtab[0].meltbp_long =
				  /*_#MELT_CALLCOUNT__L27*/ meltfnum[22];
				/*^apply.arg */
				argtab[1].meltbp_cstring =
				  "warmelt-hooks.melt";
				/*^apply.arg */
				argtab[2].meltbp_long = 2758;
				/*^apply.arg */
				argtab[3].meltbp_cstring =
				  "child_process_sigchld_handler signaled curchph=";
				/*^apply.arg */
				argtab[4].meltbp_aptr =
				  (melt_ptr_t *) & /*_.CURCHPH__V29*/
				  meltfptr[28];
				/*^apply.arg */
				argtab[5].meltbp_cstring = " termsigname=";
				/*^apply.arg */
				argtab[6].meltbp_aptr =
				  (melt_ptr_t *) & /*_.TERMSIGNAME__V34*/
				  meltfptr[33];
				/*_.MELT_DEBUG_FUN__V60*/ meltfptr[55] =
				  melt_apply ((meltclosure_ptr_t)
					      (( /*!MELT_DEBUG_FUN */
						meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*nil */ NULL)),
					      (MELTBPARSTR_LONG
					       MELTBPARSTR_CSTRING
					       MELTBPARSTR_LONG
					       MELTBPARSTR_CSTRING
					       MELTBPARSTR_PTR
					       MELTBPARSTR_CSTRING
					       MELTBPARSTR_PTR ""), argtab,
					      "", (union meltparam_un *) 0);
			      }
			      ;
			      /*_._IF___V59*/ meltfptr[45] =
				/*_.MELT_DEBUG_FUN__V60*/ meltfptr[55];;
			      /*epilog */

			      MELT_LOCATION
				("warmelt-hooks.melt:2758:/ clear");
		    /*clear *//*_#MELT_CALLCOUNT__L27*/ meltfnum[22]
				= 0;
			      /*^clear */
		    /*clear *//*_.MELT_DEBUG_FUN__V60*/ meltfptr[55]
				= 0;
			    }
			    ;
			  }
			else
			  {	/*^cond.else */

	 /*_._IF___V59*/ meltfptr[45] = NULL;;
			  }
			;
			MELT_LOCATION
			  ("warmelt-hooks.melt:2758:/ quasiblock");


			/*_.PROGN___V61*/ meltfptr[36] =
			  /*_._IF___V59*/ meltfptr[45];;
			/*^compute */
			/*_.IFCPP___V58*/ meltfptr[44] =
			  /*_.PROGN___V61*/ meltfptr[36];;
			/*epilog */

			MELT_LOCATION ("warmelt-hooks.melt:2758:/ clear");
		  /*clear *//*_#MELT_NEED_DBG__L26*/ meltfnum[18] = 0;
			/*^clear */
		  /*clear *//*_._IF___V59*/ meltfptr[45] = 0;
			/*^clear */
		  /*clear *//*_.PROGN___V61*/ meltfptr[36] = 0;
		      }

#else /*MELT_HAVE_DEBUG */
		      /*^cppif.else */
		      /*_.IFCPP___V58*/ meltfptr[44] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		      ;

		      MELT_CHECK_SIGNAL ();
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:2760:/ apply");
		      /*apply */
		      {
			union meltparam_un argtab[1];
			memset (&argtab, 0, sizeof (argtab));
			/*^apply.arg */
			argtab[0].meltbp_aptr =
			  (melt_ptr_t *) & /*_.TERMSIGNAME__V34*/
			  meltfptr[33];
			/*_.CURCLOS__V62*/ meltfptr[55] =
			  melt_apply ((meltclosure_ptr_t)
				      ( /*_.CURCLOS__V33*/ meltfptr[32]),
				      (melt_ptr_t) ( /*_.CURCHPH__V29*/
						    meltfptr[28]),
				      (MELTBPARSTR_PTR ""), argtab, "",
				      (union meltparam_un *) 0);
		      }
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:2757:/ quasiblock");


		      /*_.PROGN___V63*/ meltfptr[45] =
			/*_.CURCLOS__V62*/ meltfptr[55];;
		      /*^compute */
		      /*_._IFELSE___V57*/ meltfptr[40] =
			/*_.PROGN___V63*/ meltfptr[45];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:2757:/ clear");
		/*clear *//*_.IFCPP___V58*/ meltfptr[44] = 0;
		      /*^clear */
		/*clear *//*_.CURCLOS__V62*/ meltfptr[55] = 0;
		      /*^clear */
		/*clear *//*_.PROGN___V63*/ meltfptr[45] = 0;
		    }
		    ;
		  }
		else
		  {		/*^cond.else */

     /*_._IFELSE___V57*/ meltfptr[40] = NULL;;
		  }
		;
		/*^compute */
		/*_._IFELSE___V42*/ meltfptr[35] =
		  /*_._IFELSE___V57*/ meltfptr[40];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:2741:/ clear");
	      /*clear *//*_._IFELSE___V57*/ meltfptr[40] = 0;
	      }
	      ;
	    }
	  ;
	  /*_.LET___V30*/ meltfptr[29] = /*_._IFELSE___V42*/ meltfptr[35];;

	  MELT_LOCATION ("warmelt-hooks.melt:2705:/ clear");
	    /*clear *//*_.MULTIPLE_NTH__V31*/ meltfptr[30] = 0;
	  /*^clear */
	    /*clear *//*_#CURWSTAT__L13*/ meltfnum[10] = 0;
	  /*^clear */
	    /*clear *//*_.CHILPROH_PID__V32*/ meltfptr[31] = 0;
	  /*^clear */
	    /*clear *//*_#CURPID__L14*/ meltfnum[0] = 0;
	  /*^clear */
	    /*clear *//*_.CURCLOS__V33*/ meltfptr[32] = 0;
	  /*^clear */
	    /*clear *//*_#EXITED__L15*/ meltfnum[4] = 0;
	  /*^clear */
	    /*clear *//*_#EXITSTAT__L16*/ meltfnum[15] = 0;
	  /*^clear */
	    /*clear *//*_#SIGNALED__L17*/ meltfnum[16] = 0;
	  /*^clear */
	    /*clear *//*_#TERMSIG__L18*/ meltfnum[17] = 0;
	  /*^clear */
	    /*clear *//*_.TERMSIGNAME__V34*/ meltfptr[33] = 0;
	  /*^clear */
	    /*clear *//*_.IFCPP___V35*/ meltfptr[34] = 0;
	  /*^clear */
	    /*clear *//*_._IFELSE___V42*/ meltfptr[35] = 0;
	  if ( /*_#IX__L12*/ meltfnum[5] < 0)
	    break;
	}			/* end  foreach_in_multiple meltcit3__EACHTUP */

      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:2702:/ clear");
	    /*clear *//*_.CURCHPH__V29*/ meltfptr[28] = 0;
      /*^clear */
	    /*clear *//*_#IX__L12*/ meltfnum[5] = 0;
      /*^clear */
	    /*clear *//*_.LET___V30*/ meltfptr[29] = 0;
    }				/*endciterblock FOREACH_IN_MULTIPLE */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:2699:/ clear");
	   /*clear *//*_.TUPCHPH__V27*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.TUPWSTAT__V28*/ meltfptr[9] = 0;

    MELT_LOCATION ("warmelt-hooks.melt:2657:/ clear");
	   /*clear *//*_.LISCHPH__V6*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_.LISWSTAT__V7*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_.BUCK__V8*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_#IS_BUCKETLONG__L3*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:2655:/ clear");
	   /*clear *//*_.IFCPP___V2*/ meltfptr[1] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("CHILD_PROCESS_SIGCHLD_HANDLER", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_85_WARMELTmiHOOKS_CHILD_PROCESS_SIGCHLD_HANDLER */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER (meltclosure_ptr_t
							   meltclosp_,
							   melt_ptr_t
							   meltfirstargp_,
							   const
							   melt_argdescr_cell_t
							   meltxargdescr_[],
							   union meltparam_un
							   * meltxargtab_,
							   const
							   melt_argdescr_cell_t
							   meltxresdescr_[],
							   union meltparam_un
							   * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER_melt
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

  struct meltframe_meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER_st
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
#define MELTFRAM_NBVARPTR 32
    melt_ptr_t mcfr_varptr[32];
#define MELTFRAM_NBVARNUM 11
    long mcfr_varnum[11];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER_st
	 *) meltfirstargp_;
      /* use arguments meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 32; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER nbval 32*/
  meltfram__.mcfr_nbvar = 32 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_CHILD_PROCESS_HANDLER", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:2763:/ getarg");
 /*_.CLOS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_LONG)
    goto meltlab_endgetargs;
 /*_#PID__L1*/ meltfnum[0] = meltxargtab_[0].meltbp_long;

  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.DATA__V3*/ meltfptr[2] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.DATA__V3*/ meltfptr[2])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:2767:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:2767:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:2767:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 2767;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"register_child_process_handler clos=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CLOS__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " pid=";
	      /*^apply.arg */
	      argtab[6].meltbp_long = /*_#PID__L1*/ meltfnum[0];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = " data=";
	      /*^apply.arg */
	      argtab[8].meltbp_aptr =
		(melt_ptr_t *) & /*_.DATA__V3*/ meltfptr[2];
	      /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V5*/ meltfptr[4] =
	      /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2767:/ clear");
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
      MELT_LOCATION ("warmelt-hooks.melt:2767:/ quasiblock");


      /*_.PROGN___V7*/ meltfptr[5] = /*_._IF___V5*/ meltfptr[4];;
      /*^compute */
      /*_.IFCPP___V4*/ meltfptr[3] = /*_.PROGN___V7*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:2767:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V5*/ meltfptr[4] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V7*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V4*/ meltfptr[3] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_CLOSURE__L4*/ meltfnum[2] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.CLOS__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:2768:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L4*/ meltfnum[2])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#gtI__L5*/ meltfnum[1] =
	    (( /*_#PID__L1*/ meltfnum[0]) > (0));;
	  MELT_LOCATION ("warmelt-hooks.melt:2769:/ cond");
	  /*cond */ if ( /*_#gtI__L5*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-hooks.melt:2770:/ quasiblock");


     /*_#PIDISBAD__L6*/ meltfnum[5] = 0;;

		{
		  MELT_LOCATION ("warmelt-hooks.melt:2772:/ locexp");
		  /* register_child_process_handler TRYKILLCHK__1 */
		  if (kill ((pid_t) /*_#PID__L1*/ meltfnum[0], 0))
	     /*_#PIDISBAD__L6*/ meltfnum[5] = -1L;
		  ;
		}
		;

#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-hooks.melt:2778:/ cppif.then");
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
       /*_#MELT_NEED_DBG__L7*/ meltfnum[6] =
		    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		    0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		    ;;
		  MELT_LOCATION ("warmelt-hooks.melt:2778:/ cond");
		  /*cond */ if ( /*_#MELT_NEED_DBG__L7*/ meltfnum[6])	/*then */
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
			  0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			  ;;

			MELT_CHECK_SIGNAL ();
			;
			MELT_LOCATION ("warmelt-hooks.melt:2778:/ apply");
			/*apply */
			{
			  union meltparam_un argtab[5];
			  memset (&argtab, 0, sizeof (argtab));
			  /*^apply.arg */
			  argtab[0].meltbp_long =
			    /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
			  /*^apply.arg */
			  argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			  /*^apply.arg */
			  argtab[2].meltbp_long = 2778;
			  /*^apply.arg */
			  argtab[3].meltbp_cstring =
			    "register_child_process_handler pidisbad=";
			  /*^apply.arg */
			  argtab[4].meltbp_long =
			    /*_#PIDISBAD__L6*/ meltfnum[5];
			  /*_.MELT_DEBUG_FUN__V13*/ meltfptr[12] =
			    melt_apply ((meltclosure_ptr_t)
					(( /*!MELT_DEBUG_FUN */ meltfrout->
					  tabval[0])),
					(melt_ptr_t) (( /*nil */ NULL)),
					(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG ""), argtab, "",
					(union meltparam_un *) 0);
			}
			;
			/*_._IF___V12*/ meltfptr[11] =
			  /*_.MELT_DEBUG_FUN__V13*/ meltfptr[12];;
			/*epilog */

			MELT_LOCATION ("warmelt-hooks.melt:2778:/ clear");
		   /*clear *//*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0;
			/*^clear */
		   /*clear *//*_.MELT_DEBUG_FUN__V13*/ meltfptr[12] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

	/*_._IF___V12*/ meltfptr[11] = NULL;;
		    }
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2778:/ quasiblock");


		  /*_.PROGN___V14*/ meltfptr[12] =
		    /*_._IF___V12*/ meltfptr[11];;
		  /*^compute */
		  /*_.IFCPP___V11*/ meltfptr[10] =
		    /*_.PROGN___V14*/ meltfptr[12];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:2778:/ clear");
		 /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[6] = 0;
		  /*^clear */
		 /*clear *//*_._IF___V12*/ meltfptr[11] = 0;
		  /*^clear */
		 /*clear *//*_.PROGN___V14*/ meltfptr[12] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*_.IFCPP___V11*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-hooks.melt:2779:/ cond");
		/*cond */ if ( /*_#PIDISBAD__L6*/ meltfnum[5])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^quasiblock */


       /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

		      {
			MELT_LOCATION ("warmelt-hooks.melt:2779:/ locexp");
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
		      /*_._IF___V15*/ meltfptr[11] =
			/*_.RETURN___V16*/ meltfptr[12];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:2779:/ clear");
		 /*clear *//*_.RETURN___V16*/ meltfptr[12] = 0;
		    }
		    ;
		  }
		else
		  {		/*^cond.else */

      /*_._IF___V15*/ meltfptr[11] = NULL;;
		  }
		;
		MELT_LOCATION ("warmelt-hooks.melt:2780:/ quasiblock");



		MELT_CHECK_SIGNAL ();
		;
     /*_.MAKE_INTEGERBOX__V18*/ meltfptr[17] =
		  (meltgc_new_int
		   ((meltobject_ptr_t)
		    (( /*!DISCR_CONSTANT_INTEGER */ meltfrout->tabval[2])),
		    ( /*_#PID__L1*/ meltfnum[0])));;
		MELT_LOCATION ("warmelt-hooks.melt:2780:/ quasiblock");


		/*^rawallocobj */
		/*rawallocobj */
		{
		  melt_ptr_t newobj = 0;
		  melt_raw_object_create (newobj,
					  (melt_ptr_t) (( /*!CLASS_CHILD_PROCESS_HANDLER */ meltfrout->tabval[1])), (4), "CLASS_CHILD_PROCESS_HANDLER");
      /*_.INST__V20*/ meltfptr[19] =
		    newobj;
		};
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @CHILPROH_PID",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V20*/
						   meltfptr[19])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V20*/ meltfptr[19]), (1),
				      ( /*_.MAKE_INTEGERBOX__V18*/
				       meltfptr[17]), "CHILPROH_PID");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @CHILPROH_CLOS",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V20*/
						   meltfptr[19])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V20*/ meltfptr[19]), (2),
				      ( /*_.CLOS__V2*/ meltfptr[1]),
				      "CHILPROH_CLOS");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @CHILPROH_DATA",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V20*/
						   meltfptr[19])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V20*/ meltfptr[19]), (3),
				      ( /*_.DATA__V3*/ meltfptr[2]),
				      "CHILPROH_DATA");
		;
		/*^touchobj */

		melt_dbgtrace_written_object ( /*_.INST__V20*/ meltfptr[19],
					      "newly made instance");
		;
		/*_.CHPH__V19*/ meltfptr[18] = /*_.INST__V20*/ meltfptr[19];;

#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-hooks.melt:2786:/ cppif.then");
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
       /*_#MELT_NEED_DBG__L9*/ meltfnum[7] =
		    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		    0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		    ;;
		  MELT_LOCATION ("warmelt-hooks.melt:2786:/ cond");
		  /*cond */ if ( /*_#MELT_NEED_DBG__L9*/ meltfnum[7])	/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

	 /*_#MELT_CALLCOUNT__L10*/ meltfnum[6] =
			  /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			  meltcallcount	/* melt_callcount debugging */
#else
			  0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			  ;;

			MELT_CHECK_SIGNAL ();
			;
			MELT_LOCATION ("warmelt-hooks.melt:2786:/ apply");
			/*apply */
			{
			  union meltparam_un argtab[5];
			  memset (&argtab, 0, sizeof (argtab));
			  /*^apply.arg */
			  argtab[0].meltbp_long =
			    /*_#MELT_CALLCOUNT__L10*/ meltfnum[6];
			  /*^apply.arg */
			  argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			  /*^apply.arg */
			  argtab[2].meltbp_long = 2786;
			  /*^apply.arg */
			  argtab[3].meltbp_cstring =
			    "register_child_process_handler chph=";
			  /*^apply.arg */
			  argtab[4].meltbp_aptr =
			    (melt_ptr_t *) & /*_.CHPH__V19*/ meltfptr[18];
			  /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] =
			    melt_apply ((meltclosure_ptr_t)
					(( /*!MELT_DEBUG_FUN */ meltfrout->
					  tabval[0])),
					(melt_ptr_t) (( /*nil */ NULL)),
					(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR ""), argtab, "",
					(union meltparam_un *) 0);
			}
			;
			/*_._IF___V22*/ meltfptr[21] =
			  /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22];;
			/*epilog */

			MELT_LOCATION ("warmelt-hooks.melt:2786:/ clear");
		   /*clear *//*_#MELT_CALLCOUNT__L10*/ meltfnum[6] = 0;
			/*^clear */
		   /*clear *//*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

	/*_._IF___V22*/ meltfptr[21] = NULL;;
		    }
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2786:/ quasiblock");


		  /*_.PROGN___V24*/ meltfptr[22] =
		    /*_._IF___V22*/ meltfptr[21];;
		  /*^compute */
		  /*_.IFCPP___V21*/ meltfptr[20] =
		    /*_.PROGN___V24*/ meltfptr[22];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:2786:/ clear");
		 /*clear *//*_#MELT_NEED_DBG__L9*/ meltfnum[7] = 0;
		  /*^clear */
		 /*clear *//*_._IF___V22*/ meltfptr[21] = 0;
		  /*^clear */
		 /*clear *//*_.PROGN___V24*/ meltfptr[22] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*_.IFCPP___V21*/ meltfptr[20] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		;
		/*citerblock BLOCK_SIGNALS */
		{
		  /* block_signals meltcit1__BLKSIGNAL start */
		  long meltcit1__BLKSIGNAL_lev = melt_blocklevel_signals;
		  melt_blocklevel_signals = meltcit1__BLKSIGNAL_lev + 1;



		  MELT_LOCATION ("warmelt-hooks.melt:2789:/ quasiblock");


		  /*^cond */
		  /*cond */ if (
				 /*ifisa */
				 melt_is_instance_of ((melt_ptr_t)
						      (( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->tabval[4])),
						      (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[5])))
		    )		/*then */
		    {
		      /*^cond.then */
		      /*^getslot */
		      {
			melt_ptr_t slot = NULL, obj = NULL;
			obj =
			  (melt_ptr_t) (( /*!CHILDPROC_BUCKET_REFERENCE */
					 meltfrout->tabval[4])) /*=obj*/ ;
			melt_object_get_field (slot, obj, 0,
					       "REFERENCED_VALUE");
	/*_.REFERENCED_VALUE__V25*/ meltfptr[21] = slot;
		      };
		      ;
		    }
		  else
		    {		/*^cond.else */

       /*_.REFERENCED_VALUE__V25*/ meltfptr[21] = NULL;;
		    }
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2789:/ cond");
		  /*cond */ if ( /*_.REFERENCED_VALUE__V25*/ meltfptr[21])	/*then */
		    {
		      /*^cond.then */
		      /*_.BUCK__V26*/ meltfptr[22] =
			/*_.REFERENCED_VALUE__V25*/ meltfptr[21];;
		    }
		  else
		    {
		      MELT_LOCATION ("warmelt-hooks.melt:2789:/ cond.else");

		      /*^block */
		      /*anyblock */
		      {

	/*_.MAKE_BUCKETLONG__V27*/ meltfptr[26] =
			  (meltgc_new_longsbucket
			   ((meltobject_ptr_t)
			    ( /*!DISCR_BUCKET_LONGS */ meltfrout->tabval[3]),
			    (13)));;
			/*^compute */
			/*_.BUCK__V26*/ meltfptr[22] =
			  /*_.MAKE_BUCKETLONG__V27*/ meltfptr[26];;
			/*epilog */

			MELT_LOCATION ("warmelt-hooks.melt:2789:/ clear");
		  /*clear *//*_.MAKE_BUCKETLONG__V27*/ meltfptr[26] = 0;
		      }
		      ;
		    }
		  ;

#if MELT_HAVE_DEBUG
		  MELT_LOCATION ("warmelt-hooks.melt:2792:/ cppif.then");
		  /*^block */
		  /*anyblock */
		  {


		    MELT_CHECK_SIGNAL ();
		    ;
	/*_#IS_BUCKETLONG__L11*/ meltfnum[6] =
		      (melt_magic_discr
		       ((melt_ptr_t) ( /*_.BUCK__V26*/ meltfptr[22])) ==
		       MELTOBMAG_BUCKETLONGS);;
		    MELT_LOCATION ("warmelt-hooks.melt:2792:/ cond");
		    /*cond */ if ( /*_#IS_BUCKETLONG__L11*/ meltfnum[6])	/*then */
		      {
			/*^cond.then */
			/*_._IFELSE___V29*/ meltfptr[28] = ( /*nil */ NULL);;
		      }
		    else
		      {
			MELT_LOCATION ("warmelt-hooks.melt:2792:/ cond.else");

			/*^block */
			/*anyblock */
			{




			  {
			    /*^locexp */
			    melt_assert_failed (("check buck"),
						("warmelt-hooks.melt")
						? ("warmelt-hooks.melt") :
						__FILE__,
						(2792) ? (2792) : __LINE__,
						__FUNCTION__);
			    ;
			  }
			  ;
		    /*clear *//*_._IFELSE___V29*/ meltfptr[28] = 0;
			  /*epilog */
			}
			;
		      }
		    ;
		    /*^compute */
		    /*_.IFCPP___V28*/ meltfptr[26] =
		      /*_._IFELSE___V29*/ meltfptr[28];;
		    /*epilog */

		    MELT_LOCATION ("warmelt-hooks.melt:2792:/ clear");
		  /*clear *//*_#IS_BUCKETLONG__L11*/ meltfnum[6] = 0;
		    /*^clear */
		  /*clear *//*_._IFELSE___V29*/ meltfptr[28] = 0;
		  }

#else /*MELT_HAVE_DEBUG */
		  /*^cppif.else */
		  /*_.IFCPP___V28*/ meltfptr[26] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		  ;
		  /*^compute */
      /*_.BUCKETLONG_PUT__V30*/ meltfptr[28] =
		    meltgc_longsbucket_put ((melt_ptr_t) /*_.BUCK__V26*/
					    meltfptr[22],
					    ( /*_#PID__L1*/ meltfnum[0]),
					    (melt_ptr_t) /*_.CHPH__V19*/
					    meltfptr[18]);;
		  MELT_LOCATION ("warmelt-hooks.melt:2793:/ compute");
		  /*_.BUCK__V26*/ meltfptr[22] =
		    /*_.SETQ___V31*/ meltfptr[30] =
		    /*_.BUCKETLONG_PUT__V30*/ meltfptr[28];;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2794:/ cond");
		  /*cond */ if (
				 /*ifisa */
				 melt_is_instance_of ((melt_ptr_t)
						      (( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->tabval[4])),
						      (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[5])))
		    )		/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

			/*^putslot */
			/*putslot */
			melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
					melt_magic_discr ((melt_ptr_t)
							  (( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->tabval[4]))) == MELTOBMAG_OBJECT);
			melt_putfield_object ((( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->tabval[4])), (0), ( /*_.BUCK__V26*/ meltfptr[22]), "REFERENCED_VALUE");
			;
			/*^touch */
			meltgc_touch (( /*!CHILDPROC_BUCKET_REFERENCE */
				       meltfrout->tabval[4]));
			;
			/*^touchobj */

			melt_dbgtrace_written_object (( /*!CHILDPROC_BUCKET_REFERENCE */ meltfrout->tabval[4]), "put-fields");
			;
			/*epilog */
		      }
		      ;
		    }		/*noelse */
		  ;

		  MELT_LOCATION ("warmelt-hooks.melt:2789:/ clear");
		/*clear *//*_.REFERENCED_VALUE__V25*/ meltfptr[21] = 0;
		  /*^clear */
		/*clear *//*_.BUCK__V26*/ meltfptr[22] = 0;
		  /*^clear */
		/*clear *//*_.IFCPP___V28*/ meltfptr[26] = 0;
		  /*^clear */
		/*clear *//*_.BUCKETLONG_PUT__V30*/ meltfptr[28] = 0;
		  /*^clear */
		/*clear *//*_.SETQ___V31*/ meltfptr[30] = 0;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:2796:/ cond");
		  /*cond */ if (
				 /*ifisa */
				 melt_is_instance_of ((melt_ptr_t)
						      (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[6])),
						      (melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[8])))
		    )		/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

			/*^putslot */
			/*putslot */
			melt_assertmsg
			  ("putslot checkobj @SYSDATA_CHILD_HOOK",
			   melt_magic_discr ((melt_ptr_t)
					     (( /*!INITIAL_SYSTEM_DATA */
					       meltfrout->tabval[6]))) ==
			   MELTOBMAG_OBJECT);
			melt_putfield_object ((( /*!INITIAL_SYSTEM_DATA */
						meltfrout->tabval[6])), (14),
					      (( /*!CHILD_PROCESS_SIGCHLD_HANDLER */ meltfrout->tabval[7])), "SYSDATA_CHILD_HOOK");
			;
			/*^touch */
			meltgc_touch (( /*!INITIAL_SYSTEM_DATA */ meltfrout->
				       tabval[6]));
			;
			/*^touchobj */

			melt_dbgtrace_written_object (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[6]), "put-fields");
			;
			/*epilog */
		      }
		      ;
		    }		/*noelse */
		  ;
		  /* block_signals meltcit1__BLKSIGNAL end */
		  melt_blocklevel_signals = meltcit1__BLKSIGNAL_lev;
		  MELT_CHECK_SIGNAL ();


		  /*citerepilog */
		}		/*endciterblock BLOCK_SIGNALS */
		;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-hooks.melt:2799:/ quasiblock");


		/*_._RETVAL___V1*/ meltfptr[0] =
		  /*_.CHPH__V19*/ meltfptr[18];;

		{
		  MELT_LOCATION ("warmelt-hooks.melt:2799:/ locexp");
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
		/*_.LET___V17*/ meltfptr[12] =
		  /*_.RETURN___V32*/ meltfptr[21];;

		MELT_LOCATION ("warmelt-hooks.melt:2780:/ clear");
	       /*clear *//*_.MAKE_INTEGERBOX__V18*/ meltfptr[17] = 0;
		/*^clear */
	       /*clear *//*_.CHPH__V19*/ meltfptr[18] = 0;
		/*^clear */
	       /*clear *//*_.IFCPP___V21*/ meltfptr[20] = 0;
		/*^clear */
	       /*clear *//*_.RETURN___V32*/ meltfptr[21] = 0;
		/*_.LET___V10*/ meltfptr[9] = /*_.LET___V17*/ meltfptr[12];;

		MELT_LOCATION ("warmelt-hooks.melt:2770:/ clear");
	       /*clear *//*_#PIDISBAD__L6*/ meltfnum[5] = 0;
		/*^clear */
	       /*clear *//*_.IFCPP___V11*/ meltfptr[10] = 0;
		/*^clear */
	       /*clear *//*_._IF___V15*/ meltfptr[11] = 0;
		/*^clear */
	       /*clear *//*_.LET___V17*/ meltfptr[12] = 0;
		/*_._IF___V9*/ meltfptr[5] = /*_.LET___V10*/ meltfptr[9];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:2769:/ clear");
	       /*clear *//*_.LET___V10*/ meltfptr[9] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_._IF___V9*/ meltfptr[5] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_._IF___V8*/ meltfptr[4] = /*_._IF___V9*/ meltfptr[5];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:2768:/ clear");
	     /*clear *//*_#gtI__L5*/ meltfnum[1] = 0;
	  /*^clear */
	     /*clear *//*_._IF___V9*/ meltfptr[5] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V8*/ meltfptr[4] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2763:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_._IF___V8*/ meltfptr[4];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:2763:/ locexp");
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
	   /*clear *//*_.IFCPP___V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_#IS_CLOSURE__L4*/ meltfnum[2] = 0;
    /*^clear */
	   /*clear *//*_._IF___V8*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_CHILD_PROCESS_HANDLER", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_86_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER (meltclosure_ptr_t
							     meltclosp_,
							     melt_ptr_t
							     meltfirstargp_,
							     const
							     melt_argdescr_cell_t
							     meltxargdescr_[],
							     union
							     meltparam_un *
							     meltxargtab_,
							     const
							     melt_argdescr_cell_t
							     meltxresdescr_[],
							     union
							     meltparam_un *
							     meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER_melt
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

  struct meltframe_meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER_st
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
#define MELTFRAM_NBVARPTR 8
    melt_ptr_t mcfr_varptr[8];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER_st
	 *) meltfirstargp_;
      /* use arguments meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 8; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER nbval 8*/
  meltfram__.mcfr_nbvar = 8 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("UNREGISTER_CHILD_PROCESS_HANDLER", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:2807:/ getarg");
 /*_.CHD__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:2809:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:2809:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:2809:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 2809;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"unregister_child_process_handler chd=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CHD__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V4*/ meltfptr[3] =
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:2809:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V4*/ meltfptr[3] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:2809:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_._IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:2809:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V4*/ meltfptr[3] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V6*/ meltfptr[4] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V3*/ meltfptr[2] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:2810:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
      /*^cond */
      /*cond */ if (( /*nil */ NULL))	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V8*/ meltfptr[4] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-hooks.melt:2810:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("@$@unimplemented unregister_child_process_handler"), ("warmelt-hooks.melt") ? ("warmelt-hooks.melt") : __FILE__, (2810) ? (2810) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V8*/ meltfptr[4] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V7*/ meltfptr[3] = /*_._IFELSE___V8*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:2810:/ clear");
	     /*clear *//*_._IFELSE___V8*/ meltfptr[4] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V7*/ meltfptr[3] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:2807:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.IFCPP___V7*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:2807:/ locexp");
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
	   /*clear *//*_.IFCPP___V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V7*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("UNREGISTER_CHILD_PROCESS_HANDLER", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_87_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER */



/**** end of warmelt-hooks+03.c ****/
