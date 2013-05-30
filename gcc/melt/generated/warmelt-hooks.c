/* GCC MELT GENERATED FILE warmelt-hooks.c - DO NOT EDIT */

/***+ warmelt-hooks without options +***/
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt[] =
  MELT_RUN_HASHMD5 /* from melt-run.h */ ;


/**** warmelt-hooks declarations ****/
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


/*** 24 extra MELT c-headers ***/


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

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 2 : **/

/* flag for PLUGIN_OVERRIDE_GATE */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_override_gate_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 3 : **/

/* flag for PLUGIN_START_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_start_unit_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 4 : **/


void MELT_MODULE_VISIBILITY
melt_startunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			     void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 5 : **/

/* flag for PLUGIN_FINISH_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_finish_unit_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 6 : **/


void MELT_MODULE_VISIBILITY
melt_finishunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			      void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 7 : **/

/* flag for PLUGIN_ALL_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_passes_start_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 8 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				     void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 9 : **/

/* flag for PLUGIN_ALL_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_all_passes_end_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 10 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 11 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_ipa_passes_start_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 12 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					 void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 13 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_ipa_passes_end_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 14 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				       void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 15 : **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_early_gimple_passes_start_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 16 : **/


void MELT_MODULE_VISIBILITY
melt_early_gimple_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					      void *user_data
					      ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 17 : **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_early_gimple_passes_end_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 18 : **/


void MELT_MODULE_VISIBILITY
melt_early_gimple_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					    void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 19 : **/

/* flag for PLUGIN_PRE_GENERICIZE declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_pre_genericize_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 20 : **/


void MELT_MODULE_VISIBILITY
melt_pre_genericize_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 21 : **/

/* flag for PLUGIN_FINISH_TYPE declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_finish_type_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 22 : **/


void MELT_MODULE_VISIBILITY
melt_finish_type_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 23 : **/

/* flag for PLUGIN_PASS_EXECUTION declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_pass_execution_registered_flag;

/*** end of 24 extra MELT c-headers ***/


/** MELT extra c-header 24 : **/


void MELT_MODULE_VISIBILITY
melt_pass_execution_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED);

/*** end of 24 extra MELT c-headers ***/

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
meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE (meltclosure_ptr_t meltclosp_,
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
meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE (meltclosure_ptr_t
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
meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST (meltclosure_ptr_t
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
meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST (meltclosure_ptr_t
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
meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST (meltclosure_ptr_t
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
meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST (meltclosure_ptr_t
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
meltrout_9_warmelt_hooks_LAMBDA___1__ (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_warmelt_hooks_LAMBDA___2__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_warmelt_hooks_LAMBDA___3__ (meltclosure_ptr_t meltclosp_,
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
meltrout_13_warmelt_hooks_ENABLE_START_UNIT (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST (meltclosure_ptr_t meltclosp_,
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
meltrout_15_warmelt_hooks_AT_START_UNIT_LAST (meltclosure_ptr_t meltclosp_,
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
meltrout_16_warmelt_hooks_LAMBDA___4__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_warmelt_hooks_LAMBDA___5__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_warmelt_hooks_LAMBDA___6__ (meltclosure_ptr_t meltclosp_,
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
meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT (meltclosure_ptr_t meltclosp_,
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
meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST (meltclosure_ptr_t meltclosp_,
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
meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST (meltclosure_ptr_t meltclosp_,
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
meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START (meltclosure_ptr_t
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
meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST (meltclosure_ptr_t
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
meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST (meltclosure_ptr_t
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
meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END (meltclosure_ptr_t meltclosp_,
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
meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST (meltclosure_ptr_t
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
meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST (meltclosure_ptr_t
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
meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START (meltclosure_ptr_t
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
meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST
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
meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END (meltclosure_ptr_t
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
meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST (meltclosure_ptr_t
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
meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_EARLY_GIMPLE_PASSES_START */
MELT_EXTERN
  void melthook_HOOK_EARLY_GIMPLE_PASSES_START (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_EARLY_GIMPLE_PASSES_START hook */
void MELT_MODULE_VISIBILITY
melthookmark_HOOK_EARLY_GIMPLE_PASSES_START_rout (struct melt_callframe_st *,
						  int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_40_warmelt_hooks_ENABLE_EARLY_GIMPLE_PASSES_START (meltclosure_ptr_t
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
meltrout_41_warmelt_hooks_REGISTER_EARLY_GIMPLE_PASSES_START_FIRST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_42_warmelt_hooks_REGISTER_EARLY_GIMPLE_PASSES_START_LAST
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
meltrout_44_warmelt_hooks_ENABLE_EARLY_GIMPLE_PASSES_END (meltclosure_ptr_t
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
meltrout_45_warmelt_hooks_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_46_warmelt_hooks_REGISTER_EARLY_GIMPLE_PASSES_END_LAST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_47_warmelt_hooks_ENABLE_PRE_GENERICIZE (meltclosure_ptr_t meltclosp_,
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
meltrout_49_warmelt_hooks_REGISTER_PRE_GENERICIZE_FIRST (meltclosure_ptr_t
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
meltrout_50_warmelt_hooks_REGISTER_PRE_GENERICIZE_LAST (meltclosure_ptr_t
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
meltrout_51_warmelt_hooks_ENABLE_FINISH_TYPE (meltclosure_ptr_t meltclosp_,
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
meltrout_53_warmelt_hooks_REGISTER_FINISH_TYPE_FIRST (meltclosure_ptr_t
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
meltrout_54_warmelt_hooks_REGISTER_FINISH_TYPE_LAST (meltclosure_ptr_t
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
meltrout_56_warmelt_hooks_REGISTER_PASS_EXECUTION_FIRST (meltclosure_ptr_t
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
meltrout_57_warmelt_hooks_REGISTER_PASS_EXECUTION_LAST (meltclosure_ptr_t
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






MELT_EXTERN void *melt_start_this_module (void *);

/*declare opaque initial frame: */
typedef struct melt_initial_frame_st meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_warmelt_hooks__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_warmelt_hooks__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG */
MELT_EXTERN const char meltmodule_warmelt_hooks__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_warmelt_hooks__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG */



void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_0 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_1 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_2 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_3 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_4 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_5 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_6 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking);



/**** warmelt-hooks implementations ****/
/**** MELT GENERATED IMPLEMENTATIONS for warmelt-hooks ** DO NOT EDIT ; see gcc-melt.org ****/
/*** 23 extra MELT implementations ***/


/** MELT extra c-implementation 1 : **/

/* flag for PLUGIN_OVERRIDE_GATE */
bool melthk_override_gate_registered_flag;



/** MELT extra c-implementation 2 : **/

/* flag for for PLUGIN_START_UNIT */
bool melthk_start_unit_registered_flag;



/** MELT extra c-implementation 3 : **/


void
melt_startunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			     void *user_data ATTRIBUTE_UNUSED)
{
  melthookproc_HOOK_START_UNIT ();
}



/** MELT extra c-implementation 4 : **/

/* flag for PLUGIN_FINISH_UNIT */
bool MELT_MODULE_VISIBILITY melthk_finish_unit_registered_flag;



/** MELT extra c-implementation 5 : **/


void
melt_finishunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			      void *user_data ATTRIBUTE_UNUSED)
{
  melthookproc_HOOK_FINISH_UNIT ();
}



/** MELT extra c-implementation 6 : **/

/* flag for PLUGIN_ALL_PASSES_START define */
bool melthk_all_passes_start_registered_flag;



/** MELT extra c-implementation 7 : **/


void
melt_all_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				     void *user_data ATTRIBUTE_UNUSED)
{
  melthookproc_HOOK_ALL_PASSES_START ();
}



/** MELT extra c-implementation 8 : **/

/* flag for PLUGIN_ALL_PASSES_END define */
bool melthk_all_passes_end_registered_flag;



/** MELT extra c-implementation 9 : **/


void
melt_all_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED)
{
  melthookproc_HOOK_ALL_PASSES_END ();
}



/** MELT extra c-implementation 10 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_START define */
bool melthk_all_ipa_passes_start_registered_flag;



/** MELT extra c-implementation 11 : **/


void
melt_all_ipa_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					 void *user_data ATTRIBUTE_UNUSED)
{
  melthookproc_HOOK_ALL_IPA_PASSES_START ();
}



/** MELT extra c-implementation 12 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_END define */
bool melthk_all_ipa_passes_end_registered_flag;



/** MELT extra c-implementation 13 : **/


void
melt_all_ipa_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				       void *user_data ATTRIBUTE_UNUSED)
{
  melthookproc_HOOK_ALL_IPA_PASSES_END ();
}



/** MELT extra c-implementation 14 : **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_START define */
bool melthk_early_gimple_passes_start_registered_flag;



/** MELT extra c-implementation 15 : **/


void
melt_early_gimple_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					      void *user_data
					      ATTRIBUTE_UNUSED)
{
  melthookproc_HOOK_EARLY_GIMPLE_PASSES_START ();
}



/** MELT extra c-implementation 16 : **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_END define */
bool melthk_early_gimple_passes_end_registered_flag;



/** MELT extra c-implementation 17 : **/


void
melt_early_gimple_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					    void *user_data ATTRIBUTE_UNUSED)
{
  melthookproc_HOOK_EARLY_GIMPLE_PASSES_END ();
}



/** MELT extra c-implementation 18 : **/

/* flag for PLUGIN_PRE_GENERICIZE define */
bool melthk_pre_genericize_registered_flag;



/** MELT extra c-implementation 19 : **/


void
melt_pre_genericize_hook_callback (void *gcc_data,
				   void *user_data ATTRIBUTE_UNUSED)
{
  melthookproc_HOOK_PRE_GENERICIZE ((tree) gcc_data);
}



/** MELT extra c-implementation 20 : **/

/* flag for PLUGIN_FINISH_TYPE define */
bool melthk_finish_type_registered_flag;



/** MELT extra c-implementation 21 : **/


void
melt_finish_type_hook_callback (void *gcc_data,
				void *user_data ATTRIBUTE_UNUSED)
{
#warning should call melthookproc_HOOK_FINISH_TYPE ((tree)gcc_data);
}



/** MELT extra c-implementation 22 : **/

/* flag for PLUGIN_PASS_EXECUTION define */
bool melthk_pass_execution_registered_flag;



/** MELT extra c-implementation 23 : **/


void
melt_pass_execution_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED)
{
  struct opt_pass *pass = (struct opt_pass *) gcc_data;
#warning should call melthookproc_HOOK_PASS_EXECUTION (pass->name, (long) pass->static_pass_number);
}

/*** end of 23 extra MELT c-implementations ***/
/* frame for melthook_HOOK_LOW_DEBUG_VALUE_AT hook: */
typedef struct meltframe_melthook_HOOK_LOW_DEBUG_VALUE_AT_st
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
#define MELTFRAM_NBVARNUM 2
  long mcfr_varnum[2];
/*others*/
  const char *loc_CSTRING__o0;
  const char *loc_CSTRING__o1;
  long _spare_;
}
melthookframe_HOOK_LOW_DEBUG_VALUE_AT_t;	/* end of hook frame melthook_HOOK_LOW_DEBUG_VALUE_AT */


/** implementation of hook melthook_HOOK_LOW_DEBUG_VALUE_AT */
void
melthook_HOOK_LOW_DEBUG_VALUE_AT (melt_ptr_t melthookdata,
				  melt_ptr_t meltinp0_VAL,
				  const char *meltinp1_FILENAME,
				  long meltinp2_LINENO,
				  const char *meltinp3_MSG,
				  long meltinp4_COUNT)
{				/* body of hook melthook_HOOK_LOW_DEBUG_VALUE_AT from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_LOW_DEBUG_VALUE_AT hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_LOW_DEBUG_VALUE_AT;
  long melt_thishookcall_counter_HOOK_LOW_DEBUG_VALUE_AT ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_LOW_DEBUG_VALUE_AT++;
#define meltcallcount melt_thishookcall_counter_HOOK_LOW_DEBUG_VALUE_AT
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_LOW_DEBUG_VALUE_AT_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_LOW_DEBUG_VALUE_AT */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -3437 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_LOW_DEBUG_VALUE_AT_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_LOW_DEBUG_VALUE_AT*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_LOW_DEBUG_VALUE_AT hook */
  MELT_LOCATION ("warmelt-hooks.melt:32:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^compute */
  /*_.VAL__V2*/ meltfptr[1] = /*gethookinput */ meltinp0_VAL;;
  /*^compute */
  /*_?*/ meltfram__.loc_CSTRING__o0 = /*gethookinput */ meltinp1_FILENAME;;
  /*^compute */
  /*_#LINENO__L1*/ meltfnum[0] = /*gethookinput */ meltinp2_LINENO;;
  /*^compute */
  /*_?*/ meltfram__.loc_CSTRING__o1 = /*gethookinput */ meltinp3_MSG;;
  /*^compute */
  /*_#COUNT__L2*/ meltfnum[1] = /*gethookinput */ meltinp4_COUNT;;
  /*^block */
  /*anyblock */
  {


    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:38:/ apply");
    /*apply */
    {
      union meltparam_un argtab[5];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_long = /*_#COUNT__L2*/ meltfnum[1];
      /*^apply.arg */
      argtab[1].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o0;
      /*^apply.arg */
      argtab[2].meltbp_long = /*_#LINENO__L1*/ meltfnum[0];
      /*^apply.arg */
      argtab[3].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o1;
      /*^apply.arg */
      argtab[4].meltbp_aptr = (melt_ptr_t *) & /*_.VAL__V2*/ meltfptr[1];
      /*_.MELT_DEBUG_FUN__V3*/ meltfptr[2] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~MELT_DEBUG_FUN */ meltfhook->tabval[0])),
		    (melt_ptr_t) (( /*nil */ NULL)),
		    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG
		     MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:32:/ clear");
	    /*clear *//*_.MELT_DEBUG_FUN__V3*/ meltfptr[2] = 0;
  }
  ;
  /* epilogue of melthook_HOOK_LOW_DEBUG_VALUE_AT hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_LOW_DEBUG_VALUE_AT hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_LOW_DEBUG_VALUE_AT hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_LOW_DEBUG_VALUE_AThook */
void
melthookmark_HOOK_LOW_DEBUG_VALUE_AT_rout (struct melt_callframe_st
					   *meltcfram, int meltmarking)
{

  melthookframe_HOOK_LOW_DEBUG_VALUE_AT_t *meltframptr_
    = (melthookframe_HOOK_LOW_DEBUG_VALUE_AT_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_LOW_DEBUG_VALUE_AT good hash",
		  meltframptr_->mcfr_nbvar == -3437);
  if (meltmarking)
    {				/* marking hook HOOK_LOW_DEBUG_VALUE_AT */
      int meltix;
      for (meltix = 0; meltix < 3; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_LOW_DEBUG_VALUE_AT */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 3; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_LOW_DEBUG_VALUE_AT */
}				/* end  melthookmark_HOOK_LOW_DEBUG_VALUE_AT_rout */

/* frame for melthook_HOOK_OVERRIDE_GATE hook: */
typedef struct meltframe_melthook_HOOK_OVERRIDE_GATE_st
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
#define MELTFRAM_NBVARPTR 51
  melt_ptr_t mcfr_varptr[51];
#define MELTFRAM_NBVARNUM 24
  long mcfr_varnum[24];
/*others*/
  long _spare_;
}
melthookframe_HOOK_OVERRIDE_GATE_t;	/* end of hook frame melthook_HOOK_OVERRIDE_GATE */


/** implementation of hook melthook_HOOK_OVERRIDE_GATE */
void
melthook_HOOK_OVERRIDE_GATE (melt_ptr_t melthookdata,
			     long meltinp0_BEFOREGATE,
			     long *meltoutp0_AFTERGATE)
{				/* body of hook melthook_HOOK_OVERRIDE_GATE from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_OVERRIDE_GATE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_OVERRIDE_GATE;
  long melt_thishookcall_counter_HOOK_OVERRIDE_GATE ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_OVERRIDE_GATE++;
#define meltcallcount melt_thishookcall_counter_HOOK_OVERRIDE_GATE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_OVERRIDE_GATE_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_OVERRIDE_GATE */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -806 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_OVERRIDE_GATE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_OVERRIDE_GATE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_OVERRIDE_GATE hook */
  MELT_LOCATION ("warmelt-hooks.melt:49:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^compute */
  /*_#BEFOREGATE__L1*/ meltfnum[0] = /*gethookinput */ meltinp0_BEFOREGATE;;
  /*^block */
  /*anyblock */
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:54:/ cppif.then");
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
    /*_#MELT_NEED_DBG__L3*/ meltfnum[2] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-hooks.melt:54:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L3*/ meltfnum[2])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

      /*_#MELT_CALLCOUNT__L4*/ meltfnum[3] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:54:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 54;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "hook_override_gate beforegate=";
	      /*^apply.arg */
	      argtab[4].meltbp_long = /*_#BEFOREGATE__L1*/ meltfnum[0];
	      /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*~MELT_DEBUG_FUN */ meltfhook->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_.IF___V3*/ meltfptr[2] = /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:54:/ clear");
		/*clear *//*_#MELT_CALLCOUNT__L4*/ meltfnum[3] = 0;
	    /*^clear */
		/*clear *//*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

     /*_.IF___V3*/ meltfptr[2] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:54:/ quasiblock");


      /*_.PROGN___V5*/ meltfptr[3] = /*_.IF___V3*/ meltfptr[2];;
      /*^compute */
      /*_.IFCPP___V2*/ meltfptr[1] = /*_.PROGN___V5*/ meltfptr[3];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:54:/ clear");
	      /*clear *//*_#MELT_NEED_DBG__L3*/ meltfnum[2] = 0;
      /*^clear */
	      /*clear *//*_.IF___V3*/ meltfptr[2] = 0;
      /*^clear */
	      /*clear *//*_.PROGN___V5*/ meltfptr[3] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V2*/ meltfptr[1] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-hooks.melt:55:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:56:/ cond");
    /*cond */ if ( /*_#BEFOREGATE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*_.GATEFLAG__V6*/ meltfptr[2] =
	/*hooknth: */
	(((melthook_ptr_t) ( /*_.HOOKDATA___V1*/ meltfptr[0]))->tabval[5]);;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:56:/ cond.else");

   /*_.GATEFLAG__V6*/ meltfptr[2] = NULL;;
      }
    ;
    /*^compute */
    /*_.PASSNAME__V7*/ meltfptr[3] = ( /*nil */ NULL);;
    /*^compute */
  /*_#PASSNUM__L6*/ meltfnum[2] = 0;;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:59:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*~CLASS_REFERENCE */
					     meltfhook->tabval[1])), (1),
			      "CLASS_REFERENCE");
   /*_.INST__V9*/ meltfptr[8] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V9*/ meltfptr[8])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V9*/ meltfptr[8]), (0),
			  (( /*nil */ NULL)), "REFERENCED_VALUE");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V9*/ meltfptr[8],
				  "newly made instance");
    ;
    /*_.MEMREF__V8*/ meltfptr[7] = /*_.INST__V9*/ meltfptr[8];;
    /*^compute */
  /*_.REVLASTLIST__V10*/ meltfptr[9] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    MELT_LOCATION ("warmelt-hooks.melt:61:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~OVERRIDE_GATE_DELAYED_QUEUE */
					  meltfhook->tabval[3])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[4])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~OVERRIDE_GATE_DELAYED_QUEUE */ meltfhook->
			   tabval[3])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.QUFIRST__V11*/ meltfptr[10] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.QUFIRST__V11*/ meltfptr[10] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:62:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~OVERRIDE_GATE_DELAYED_QUEUE */
					  meltfhook->tabval[3])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[4])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~OVERRIDE_GATE_DELAYED_QUEUE */ meltfhook->
			   tabval[3])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.QULAST__V12*/ meltfptr[11] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.QULAST__V12*/ meltfptr[11] = NULL;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:65:/ locexp");
      /* hook_override_gate GETPASSNAME_CHK__1 */
      gcc_assert (current_pass != NULL);
     /*_.PASSNAME__V7*/ meltfptr[3] = meltgc_new_stringdup
	((meltobject_ptr_t) MELT_PREDEF (DISCR_STRING), current_pass->name);
     /*_#PASSNUM__L6*/ meltfnum[2] = current_pass->static_pass_number;
      ;
    }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:74:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:74:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:74:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[13];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 74;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "hook_override_gate memref=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MEMREF__V8*/ meltfptr[7];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n gateflag=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.GATEFLAG__V6*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = "\n override_gate_delayed_queue=";
	      /*^apply.arg */
	      argtab[8].meltbp_aptr =
		(melt_ptr_t *) & ( /*~OVERRIDE_GATE_DELAYED_QUEUE */
				  meltfhook->tabval[3]);
	      /*^apply.arg */
	      argtab[9].meltbp_cstring = "\n passname=";
	      /*^apply.arg */
	      argtab[10].meltbp_aptr =
		(melt_ptr_t *) & /*_.PASSNAME__V7*/ meltfptr[3];
	      /*^apply.arg */
	      argtab[11].meltbp_cstring = " passnum=";
	      /*^apply.arg */
	      argtab[12].meltbp_long = /*_#PASSNUM__L6*/ meltfnum[2];
	      /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*~MELT_DEBUG_FUN */ meltfhook->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_.IF___V14*/ meltfptr[13] =
	      /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:74:/ clear");
		/*clear *//*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0;
	    /*^clear */
		/*clear *//*_.MELT_DEBUG_FUN__V15*/ meltfptr[14] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

     /*_.IF___V14*/ meltfptr[13] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:74:/ quasiblock");


      /*_.PROGN___V16*/ meltfptr[14] = /*_.IF___V14*/ meltfptr[13];;
      /*^compute */
      /*_.IFCPP___V13*/ meltfptr[12] = /*_.PROGN___V16*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:74:/ clear");
	      /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[6] = 0;
      /*^clear */
	      /*clear *//*_.IF___V14*/ meltfptr[13] = 0;
      /*^clear */
	      /*clear *//*_.PROGN___V16*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V13*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.CURPAIR__V17*/ meltfptr[13] =
	   melt_list_first ((melt_ptr_t) /*_.QUFIRST__V11*/ meltfptr[10]);
	   melt_magic_discr ((melt_ptr_t) /*_.CURPAIR__V17*/ meltfptr[13]) ==
	   MELTOBMAG_PAIR;
	   /*_.CURPAIR__V17*/ meltfptr[13] =
	   melt_pair_tail ((melt_ptr_t) /*_.CURPAIR__V17*/ meltfptr[13]))
	{
	  /*_.CURCLO__V18*/ meltfptr[14] =
	    melt_pair_head ((melt_ptr_t) /*_.CURPAIR__V17*/ meltfptr[13]);



#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-hooks.melt:82:/ cppif.then");
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
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-hooks.melt:82:/ cond");
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
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:82:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[7];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L10*/ meltfnum[6];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-hooks.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 82;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "hook_override_gate first curclo=";
		    /*^apply.arg */
		    argtab[4].meltbp_aptr =
		      (melt_ptr_t *) & /*_.CURCLO__V18*/ meltfptr[14];
		    /*^apply.arg */
		    argtab[5].meltbp_cstring = " memref=";
		    /*^apply.arg */
		    argtab[6].meltbp_aptr =
		      (melt_ptr_t *) & /*_.MEMREF__V8*/ meltfptr[7];
		    /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20] =
		      melt_apply ((meltclosure_ptr_t)
				  (( /*~MELT_DEBUG_FUN */ meltfhook->
				    tabval[0])),
				  (melt_ptr_t) (( /*nil */ NULL)),
				  (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR ""), argtab, "",
				  (union meltparam_un *) 0);
		  }
		  ;
		  /*_.IF___V20*/ meltfptr[19] =
		    /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:82:/ clear");
		 /*clear *//*_#MELT_CALLCOUNT__L10*/ meltfnum[6] = 0;
		  /*^clear */
		 /*clear *//*_.MELT_DEBUG_FUN__V21*/ meltfptr[20] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

      /*_.IF___V20*/ meltfptr[19] = NULL;;
	      }
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:82:/ quasiblock");


	    /*_.PROGN___V22*/ meltfptr[20] = /*_.IF___V20*/ meltfptr[19];;
	    /*^compute */
	    /*_.IFCPP___V19*/ meltfptr[18] = /*_.PROGN___V22*/ meltfptr[20];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:82:/ clear");
	       /*clear *//*_#MELT_NEED_DBG__L9*/ meltfnum[7] = 0;
	    /*^clear */
	       /*clear *//*_.IF___V20*/ meltfptr[19] = 0;
	    /*^clear */
	       /*clear *//*_.PROGN___V22*/ meltfptr[20] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V19*/ meltfptr[18] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
   /*_#IS_CLOSURE__L11*/ meltfnum[6] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.CURCLO__V18*/ meltfptr[14]))
	     == MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:83:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L11*/ meltfnum[6])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-hooks.melt:84:/ quasiblock");



		MELT_CHECK_SIGNAL ();
		;
		/*^apply */
		/*apply */
		{
		  union meltparam_un argtab[3];
		  memset (&argtab, 0, sizeof (argtab));
		  /*^apply.arg */
		  argtab[0].meltbp_aptr =
		    (melt_ptr_t *) & /*_.MEMREF__V8*/ meltfptr[7];
		  /*^apply.arg */
		  argtab[1].meltbp_aptr =
		    (melt_ptr_t *) & /*_.PASSNAME__V7*/ meltfptr[3];
		  /*^apply.arg */
		  argtab[2].meltbp_long = /*_#PASSNUM__L6*/ meltfnum[2];
		  /*_.CLORES__V25*/ meltfptr[24] =
		    melt_apply ((meltclosure_ptr_t)
				( /*_.CURCLO__V18*/ meltfptr[14]),
				(melt_ptr_t) ( /*_.GATEFLAG__V6*/
					      meltfptr[2]),
				(MELTBPARSTR_PTR MELTBPARSTR_PTR
				 MELTBPARSTR_LONG ""), argtab, "",
				(union meltparam_un *) 0);
		}
		;

#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-hooks.melt:86:/ cppif.then");
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
       /*_#MELT_NEED_DBG__L12*/ meltfnum[7] =
		    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		    0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		    ;;
		  MELT_LOCATION ("warmelt-hooks.melt:86:/ cond");
		  /*cond */ if ( /*_#MELT_NEED_DBG__L12*/ meltfnum[7])	/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

	 /*_#MELT_CALLCOUNT__L13*/ meltfnum[12] =
			  /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			  meltcallcount	/* melt_callcount debugging */
#else
			  0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			  ;;

			MELT_CHECK_SIGNAL ();
			;
			MELT_LOCATION ("warmelt-hooks.melt:86:/ apply");
			/*apply */
			{
			  union meltparam_un argtab[5];
			  memset (&argtab, 0, sizeof (argtab));
			  /*^apply.arg */
			  argtab[0].meltbp_long =
			    /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
			  /*^apply.arg */
			  argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			  /*^apply.arg */
			  argtab[2].meltbp_long = 86;
			  /*^apply.arg */
			  argtab[3].meltbp_cstring =
			    "hook_override_gate first clores=";
			  /*^apply.arg */
			  argtab[4].meltbp_aptr =
			    (melt_ptr_t *) & /*_.CLORES__V25*/ meltfptr[24];
			  /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] =
			    melt_apply ((meltclosure_ptr_t)
					(( /*~MELT_DEBUG_FUN */ meltfhook->
					  tabval[0])),
					(melt_ptr_t) (( /*nil */ NULL)),
					(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR ""), argtab, "",
					(union meltparam_un *) 0);
			}
			;
			/*_.IF___V27*/ meltfptr[26] =
			  /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27];;
			/*epilog */

			MELT_LOCATION ("warmelt-hooks.melt:86:/ clear");
		   /*clear *//*_#MELT_CALLCOUNT__L13*/ meltfnum[12] = 0;
			/*^clear */
		   /*clear *//*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

	/*_.IF___V27*/ meltfptr[26] = NULL;;
		    }
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:86:/ quasiblock");


		  /*_.PROGN___V29*/ meltfptr[27] =
		    /*_.IF___V27*/ meltfptr[26];;
		  /*^compute */
		  /*_.IFCPP___V26*/ meltfptr[25] =
		    /*_.PROGN___V29*/ meltfptr[27];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:86:/ clear");
		 /*clear *//*_#MELT_NEED_DBG__L12*/ meltfnum[7] = 0;
		  /*^clear */
		 /*clear *//*_.IF___V27*/ meltfptr[26] = 0;
		  /*^clear */
		 /*clear *//*_.PROGN___V29*/ meltfptr[27] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*_.IFCPP___V26*/ meltfptr[25] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		;
		MELT_LOCATION ("warmelt-hooks.melt:87:/ compute");
		/*_.GATEFLAG__V6*/ meltfptr[2] =
		  /*_.SETQ___V30*/ meltfptr[26] =
		  /*_.CLORES__V25*/ meltfptr[24];;
		/*_.LET___V24*/ meltfptr[20] = /*_.SETQ___V30*/ meltfptr[26];;

		MELT_LOCATION ("warmelt-hooks.melt:84:/ clear");
	       /*clear *//*_.CLORES__V25*/ meltfptr[24] = 0;
		/*^clear */
	       /*clear *//*_.IFCPP___V26*/ meltfptr[25] = 0;
		/*^clear */
	       /*clear *//*_.SETQ___V30*/ meltfptr[26] = 0;
		/*_.IF___V23*/ meltfptr[19] = /*_.LET___V24*/ meltfptr[20];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:83:/ clear");
	       /*clear *//*_.LET___V24*/ meltfptr[20] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.IF___V23*/ meltfptr[19] = NULL;;
	    }
	  ;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.CURPAIR__V17*/ meltfptr[13] = NULL;
     /*_.CURCLO__V18*/ meltfptr[14] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:79:/ clear");
	     /*clear *//*_.CURPAIR__V17*/ meltfptr[13] = 0;
      /*^clear */
	     /*clear *//*_.CURCLO__V18*/ meltfptr[14] = 0;
      /*^clear */
	     /*clear *//*_.IFCPP___V19*/ meltfptr[18] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L11*/ meltfnum[6] = 0;
      /*^clear */
	     /*clear *//*_.IF___V23*/ meltfptr[19] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.CURLPAIR__V31*/ meltfptr[27] =
	   melt_list_first ((melt_ptr_t) /*_.QULAST__V12*/ meltfptr[11]);
	   melt_magic_discr ((melt_ptr_t) /*_.CURLPAIR__V31*/ meltfptr[27]) ==
	   MELTOBMAG_PAIR;
	   /*_.CURLPAIR__V31*/ meltfptr[27] =
	   melt_pair_tail ((melt_ptr_t) /*_.CURLPAIR__V31*/ meltfptr[27]))
	{
	  /*_.CURLCLO__V32*/ meltfptr[24] =
	    melt_pair_head ((melt_ptr_t) /*_.CURLPAIR__V31*/ meltfptr[27]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L14*/ meltfnum[12] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.CURLCLO__V32*/ meltfptr[24])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:93:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L14*/ meltfnum[12])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-hooks.melt:94:/ locexp");
		  meltgc_append_list ((melt_ptr_t)
				      ( /*_.REVLASTLIST__V10*/ meltfptr[9]),
				      (melt_ptr_t) ( /*_.CURLCLO__V32*/
						    meltfptr[24]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.CURLPAIR__V31*/ meltfptr[27] = NULL;
     /*_.CURLCLO__V32*/ meltfptr[24] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:90:/ clear");
	     /*clear *//*_.CURLPAIR__V31*/ meltfptr[27] = 0;
      /*^clear */
	     /*clear *//*_.CURLCLO__V32*/ meltfptr[24] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L14*/ meltfnum[12] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit3__EACHLIST */
      for ( /*_.CURLPAIR__V33*/ meltfptr[25] =
	   melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V10*/ meltfptr[9]);
	   melt_magic_discr ((melt_ptr_t) /*_.CURLPAIR__V33*/ meltfptr[25]) ==
	   MELTOBMAG_PAIR;
	   /*_.CURLPAIR__V33*/ meltfptr[25] =
	   melt_pair_tail ((melt_ptr_t) /*_.CURLPAIR__V33*/ meltfptr[25]))
	{
	  /*_.CURLCLO__V34*/ meltfptr[26] =
	    melt_pair_head ((melt_ptr_t) /*_.CURLPAIR__V33*/ meltfptr[25]);



#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-hooks.melt:99:/ cppif.then");
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
     /*_#MELT_NEED_DBG__L15*/ meltfnum[7] =
	      /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	      ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-hooks.melt:99:/ cond");
	    /*cond */ if ( /*_#MELT_NEED_DBG__L15*/ meltfnum[7])	/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

       /*_#MELT_CALLCOUNT__L16*/ meltfnum[15] =
		    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		    meltcallcount	/* melt_callcount debugging */
#else
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:99:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[7];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-hooks.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 99;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "hook_override_gate last curlclo=";
		    /*^apply.arg */
		    argtab[4].meltbp_aptr =
		      (melt_ptr_t *) & /*_.CURLCLO__V34*/ meltfptr[26];
		    /*^apply.arg */
		    argtab[5].meltbp_cstring = " memref=";
		    /*^apply.arg */
		    argtab[6].meltbp_aptr =
		      (melt_ptr_t *) & /*_.MEMREF__V8*/ meltfptr[7];
		    /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36] =
		      melt_apply ((meltclosure_ptr_t)
				  (( /*~MELT_DEBUG_FUN */ meltfhook->
				    tabval[0])),
				  (melt_ptr_t) (( /*nil */ NULL)),
				  (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR ""), argtab, "",
				  (union meltparam_un *) 0);
		  }
		  ;
		  /*_.IF___V36*/ meltfptr[35] =
		    /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:99:/ clear");
		 /*clear *//*_#MELT_CALLCOUNT__L16*/ meltfnum[15] = 0;
		  /*^clear */
		 /*clear *//*_.MELT_DEBUG_FUN__V37*/ meltfptr[36] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

      /*_.IF___V36*/ meltfptr[35] = NULL;;
	      }
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:99:/ quasiblock");


	    /*_.PROGN___V38*/ meltfptr[36] = /*_.IF___V36*/ meltfptr[35];;
	    /*^compute */
	    /*_.IFCPP___V35*/ meltfptr[20] = /*_.PROGN___V38*/ meltfptr[36];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:99:/ clear");
	       /*clear *//*_#MELT_NEED_DBG__L15*/ meltfnum[7] = 0;
	    /*^clear */
	       /*clear *//*_.IF___V36*/ meltfptr[35] = 0;
	    /*^clear */
	       /*clear *//*_.PROGN___V38*/ meltfptr[36] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V35*/ meltfptr[20] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;

#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-hooks.melt:100:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    MELT_CHECK_SIGNAL ();
	    ;
     /*_#IS_CLOSURE__L17*/ meltfnum[15] =
	      (melt_magic_discr
	       ((melt_ptr_t) ( /*_.CURLCLO__V34*/ meltfptr[26])) ==
	       MELTOBMAG_CLOSURE);;
	    MELT_LOCATION ("warmelt-hooks.melt:100:/ cond");
	    /*cond */ if ( /*_#IS_CLOSURE__L17*/ meltfnum[15])	/*then */
	      {
		/*^cond.then */
		/*_.IFELSE___V40*/ meltfptr[36] = ( /*nil */ NULL);;
	      }
	    else
	      {
		MELT_LOCATION ("warmelt-hooks.melt:100:/ cond.else");

		/*^block */
		/*anyblock */
		{




		  {
		    /*^locexp */
		    melt_assert_failed (("check curlclo"),
					("warmelt-hooks.melt")
					? ("warmelt-hooks.melt") : __FILE__,
					(100) ? (100) : __LINE__,
					__FUNCTION__);
		    ;
		  }
		  ;
		 /*clear *//*_.IFELSE___V40*/ meltfptr[36] = 0;
		  /*epilog */
		}
		;
	      }
	    ;
	    /*^compute */
	    /*_.IFCPP___V39*/ meltfptr[35] = /*_.IFELSE___V40*/ meltfptr[36];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:100:/ clear");
	       /*clear *//*_#IS_CLOSURE__L17*/ meltfnum[15] = 0;
	    /*^clear */
	       /*clear *//*_.IFELSE___V40*/ meltfptr[36] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V39*/ meltfptr[35] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:101:/ quasiblock");



	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^apply */
	  /*apply */
	  {
	    union meltparam_un argtab[3];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.MEMREF__V8*/ meltfptr[7];
	    /*^apply.arg */
	    argtab[1].meltbp_aptr =
	      (melt_ptr_t *) & /*_.PASSNAME__V7*/ meltfptr[3];
	    /*^apply.arg */
	    argtab[2].meltbp_long = /*_#PASSNUM__L6*/ meltfnum[2];
	    /*_.CLORES__V42*/ meltfptr[41] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.CURLCLO__V34*/ meltfptr[26]),
			  (melt_ptr_t) ( /*_.GATEFLAG__V6*/ meltfptr[2]),
			  (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG
			   ""), argtab, "", (union meltparam_un *) 0);
	  }
	  ;

#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-hooks.melt:103:/ cppif.then");
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
     /*_#MELT_NEED_DBG__L18*/ meltfnum[7] =
	      /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	      ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-hooks.melt:103:/ cond");
	    /*cond */ if ( /*_#MELT_NEED_DBG__L18*/ meltfnum[7])	/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

       /*_#MELT_CALLCOUNT__L19*/ meltfnum[15] =
		    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		    meltcallcount	/* melt_callcount debugging */
#else
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:103:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[5];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L19*/ meltfnum[15];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-hooks.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 103;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "hook_override_gate last clores=";
		    /*^apply.arg */
		    argtab[4].meltbp_aptr =
		      (melt_ptr_t *) & /*_.CLORES__V42*/ meltfptr[41];
		    /*_.MELT_DEBUG_FUN__V45*/ meltfptr[44] =
		      melt_apply ((meltclosure_ptr_t)
				  (( /*~MELT_DEBUG_FUN */ meltfhook->
				    tabval[0])),
				  (melt_ptr_t) (( /*nil */ NULL)),
				  (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR ""), argtab, "",
				  (union meltparam_un *) 0);
		  }
		  ;
		  /*_.IF___V44*/ meltfptr[43] =
		    /*_.MELT_DEBUG_FUN__V45*/ meltfptr[44];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:103:/ clear");
		 /*clear *//*_#MELT_CALLCOUNT__L19*/ meltfnum[15] = 0;
		  /*^clear */
		 /*clear *//*_.MELT_DEBUG_FUN__V45*/ meltfptr[44] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

      /*_.IF___V44*/ meltfptr[43] = NULL;;
	      }
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:103:/ quasiblock");


	    /*_.PROGN___V46*/ meltfptr[44] = /*_.IF___V44*/ meltfptr[43];;
	    /*^compute */
	    /*_.IFCPP___V43*/ meltfptr[42] = /*_.PROGN___V46*/ meltfptr[44];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:103:/ clear");
	       /*clear *//*_#MELT_NEED_DBG__L18*/ meltfnum[7] = 0;
	    /*^clear */
	       /*clear *//*_.IF___V44*/ meltfptr[43] = 0;
	    /*^clear */
	       /*clear *//*_.PROGN___V46*/ meltfptr[44] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V43*/ meltfptr[42] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:104:/ compute");
	  /*_.GATEFLAG__V6*/ meltfptr[2] = /*_.SETQ___V47*/ meltfptr[43] =
	    /*_.CLORES__V42*/ meltfptr[41];;
	  /*_.LET___V41*/ meltfptr[36] = /*_.SETQ___V47*/ meltfptr[43];;

	  MELT_LOCATION ("warmelt-hooks.melt:101:/ clear");
	     /*clear *//*_.CLORES__V42*/ meltfptr[41] = 0;
	  /*^clear */
	     /*clear *//*_.IFCPP___V43*/ meltfptr[42] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V47*/ meltfptr[43] = 0;
	}			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.CURLPAIR__V33*/ meltfptr[25] = NULL;
     /*_.CURLCLO__V34*/ meltfptr[26] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:96:/ clear");
	     /*clear *//*_.CURLPAIR__V33*/ meltfptr[25] = 0;
      /*^clear */
	     /*clear *//*_.CURLCLO__V34*/ meltfptr[26] = 0;
      /*^clear */
	     /*clear *//*_.IFCPP___V35*/ meltfptr[20] = 0;
      /*^clear */
	     /*clear *//*_.IFCPP___V39*/ meltfptr[35] = 0;
      /*^clear */
	     /*clear *//*_.LET___V41*/ meltfptr[36] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:106:/ cppif.then");
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
    /*_#MELT_NEED_DBG__L20*/ meltfnum[15] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-hooks.melt:106:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L20*/ meltfnum[15])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

      /*_#MELT_CALLCOUNT__L21*/ meltfnum[7] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:106:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L21*/ meltfnum[7];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 106;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "hook_override_gate final gateflag=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.GATEFLAG__V6*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n passname=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.PASSNAME__V7*/ meltfptr[3];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = " passnum=";
	      /*^apply.arg */
	      argtab[8].meltbp_long = /*_#PASSNUM__L6*/ meltfnum[2];
	      /*_.MELT_DEBUG_FUN__V50*/ meltfptr[42] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*~MELT_DEBUG_FUN */ meltfhook->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_.IF___V49*/ meltfptr[41] =
	      /*_.MELT_DEBUG_FUN__V50*/ meltfptr[42];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:106:/ clear");
		/*clear *//*_#MELT_CALLCOUNT__L21*/ meltfnum[7] = 0;
	    /*^clear */
		/*clear *//*_.MELT_DEBUG_FUN__V50*/ meltfptr[42] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

     /*_.IF___V49*/ meltfptr[41] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:106:/ quasiblock");


      /*_.PROGN___V51*/ meltfptr[43] = /*_.IF___V49*/ meltfptr[41];;
      /*^compute */
      /*_.IFCPP___V48*/ meltfptr[44] = /*_.PROGN___V51*/ meltfptr[43];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:106:/ clear");
	      /*clear *//*_#MELT_NEED_DBG__L20*/ meltfnum[15] = 0;
      /*^clear */
	      /*clear *//*_.IF___V49*/ meltfptr[41] = 0;
      /*^clear */
	      /*clear *//*_.PROGN___V51*/ meltfptr[43] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V48*/ meltfptr[44] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:108:/ cond");
    /*cond */ if ( /*_.GATEFLAG__V6*/ meltfptr[2])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:109:/ compute");
	  /*_#AFTERGATE__L2*/ meltfnum[1] = /*_#SETQ___L23*/ meltfnum[15] =
	    1;;
	  /*_#IFELSE___L22*/ meltfnum[7] = /*_#SETQ___L23*/ meltfnum[15];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:108:/ clear");
	      /*clear *//*_#SETQ___L23*/ meltfnum[15] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:110:/ compute");
	  /*_#AFTERGATE__L2*/ meltfnum[1] = /*_#SETQ___L24*/ meltfnum[15] =
	    0;;
	  /*_#IFELSE___L22*/ meltfnum[7] = /*_#SETQ___L24*/ meltfnum[15];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:108:/ clear");
	      /*clear *//*_#SETQ___L24*/ meltfnum[15] = 0;
	}
	;
      }
    ;
    /*_#LET___L5*/ meltfnum[3] = /*_#IFELSE___L22*/ meltfnum[7];;

    MELT_LOCATION ("warmelt-hooks.melt:55:/ clear");
	    /*clear *//*_.GATEFLAG__V6*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.PASSNAME__V7*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_#PASSNUM__L6*/ meltfnum[2] = 0;
    /*^clear */
	    /*clear *//*_.MEMREF__V8*/ meltfptr[7] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V10*/ meltfptr[9] = 0;
    /*^clear */
	    /*clear *//*_.QUFIRST__V11*/ meltfptr[10] = 0;
    /*^clear */
	    /*clear *//*_.QULAST__V12*/ meltfptr[11] = 0;
    /*^clear */
	    /*clear *//*_.IFCPP___V13*/ meltfptr[12] = 0;
    /*^clear */
	    /*clear *//*_.IFCPP___V48*/ meltfptr[44] = 0;
    /*^clear */
	    /*clear *//*_#IFELSE___L22*/ meltfnum[7] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:49:/ clear");
	    /*clear *//*_.IFCPP___V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_#LET___L5*/ meltfnum[3] = 0;
  }
  ;
  /* epilogue of melthook_HOOK_OVERRIDE_GATE hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /*^putoutarg */
  if ((meltoutp0_AFTERGATE) != NULL)
    *meltoutp0_AFTERGATE /*putout */  = /*_#AFTERGATE__L2*/ meltfnum[1];
  ;
  /* final of melthook_HOOK_OVERRIDE_GATE hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_OVERRIDE_GATE hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_OVERRIDE_GATEhook */
void
melthookmark_HOOK_OVERRIDE_GATE_rout (struct melt_callframe_st *meltcfram,
				      int meltmarking)
{

  melthookframe_HOOK_OVERRIDE_GATE_t *meltframptr_
    = (melthookframe_HOOK_OVERRIDE_GATE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_OVERRIDE_GATE good hash",
		  meltframptr_->mcfr_nbvar == -806);
  if (meltmarking)
    {				/* marking hook HOOK_OVERRIDE_GATE */
      int meltix;
      for (meltix = 0; meltix < 51; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_OVERRIDE_GATE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 51; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_OVERRIDE_GATE */
}				/* end  melthookmark_HOOK_OVERRIDE_GATE_rout */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE_melt
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

  struct meltframe_meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE_st
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
  /*meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE_st *)
	meltfirstargp_;
      /* use arguments meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE nbval 1*/
  meltfram__.mcfr_nbvar = 1 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ENABLE_OVERRIDE_GATE", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:144:/ block");
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-hooks.melt:145:/ locexp");
      /* enable_override_gate REGPLUGINOVERRIDE_GATE_CHK__1 */
      if (!melthk_override_gate_registered_flag)
	{
	  melthk_override_gate_registered_flag = true;
	  register_callback (melt_plugin_name, PLUGIN_OVERRIDE_GATE,
			     melt_override_gate_callback, NULL);
	}
      ;
    }
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ENABLE_OVERRIDE_GATE", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE_melt
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

  struct meltframe_meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
#define MELTFRAM_NBVARNUM 3
    long mcfr_varnum[3];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE_st *)
	meltfirstargp_;
      /* use arguments meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MAYBE_DISABLE_OVERRIDE_GATE", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:159:/ block");
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:161:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!OVERRIDE_GATE_DELAYED_QUEUE */
					  meltfrout->tabval[0])),
					(melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
   /*_.DELQU_FIRST__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.DELQU_FIRST__V2*/ meltfptr[1] = NULL;;
      }
    ;
    /*^compute */
 /*_.LIST_FIRST__V3*/ meltfptr[2] =
      (melt_list_first ((melt_ptr_t) ( /*_.DELQU_FIRST__V2*/ meltfptr[1])));;
    /*^compute */
 /*_#NULL__L1*/ meltfnum[0] =
      (( /*_.LIST_FIRST__V3*/ meltfptr[2]) == NULL);;
    MELT_LOCATION ("warmelt-hooks.melt:160:/ cond");
    /*cond */ if ( /*_#NULL__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:162:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!OVERRIDE_GATE_DELAYED_QUEUE */
				 meltfrout->tabval[0])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "DELQU_LAST");
     /*_.DELQU_LAST__V4*/ meltfptr[3] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.DELQU_LAST__V4*/ meltfptr[3] = NULL;;
	    }
	  ;
	  /*^compute */
   /*_.LIST_FIRST__V5*/ meltfptr[4] =
	    (melt_list_first
	     ((melt_ptr_t) ( /*_.DELQU_LAST__V4*/ meltfptr[3])));;
	  /*^compute */
   /*_#NULL__L3*/ meltfnum[2] =
	    (( /*_.LIST_FIRST__V5*/ meltfptr[4]) == NULL);;
	  /*^compute */
	  /*_#IF___L2*/ meltfnum[1] = /*_#NULL__L3*/ meltfnum[2];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:160:/ clear");
	     /*clear *//*_.DELQU_LAST__V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_.LIST_FIRST__V5*/ meltfptr[4] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L3*/ meltfnum[2] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_#IF___L2*/ meltfnum[1] = 0;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:160:/ cond");
    /*cond */ if ( /*_#IF___L2*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:163:/ locexp");
	    /* disable_override_gate UNREGPLUGINOVERRIDE_GATE_CHK__1 */
	    if (melthk_override_gate_registered_flag)
	      {
		melthk_override_gate_registered_flag = false;
		unregister_callback (melt_plugin_name, PLUGIN_OVERRIDE_GATE);
	      }
	    ;
	  }
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:160:/ quasiblock");


	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:159:/ clear");
	   /*clear *//*_.DELQU_FIRST__V2*/ meltfptr[1] = 0;
    /*^clear */
	   /*clear *//*_.LIST_FIRST__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L1*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_#IF___L2*/ meltfnum[1] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MAYBE_DISABLE_OVERRIDE_GATE", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST_melt
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

  struct meltframe_meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST_st
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
#define MELTFRAM_NBVARPTR 10
    melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 3
    long mcfr_varnum[3];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST_st *)
	meltfirstargp_;
      /* use arguments meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 10; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST nbval 10*/
  meltfram__.mcfr_nbvar = 10 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_OVERRIDE_GATE_FIRST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:175:/ getarg");
 /*_.CLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:178:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:178:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:178:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 178;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "register_override_gate_first clo=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CLO__V2*/ meltfptr[1];
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
	    /*_.IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:178:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_.IF___V4*/ meltfptr[3] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:178:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_.IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:178:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_.IF___V4*/ meltfptr[3] = 0;
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
    /*^compute */
 /*_#IS_CLOSURE__L3*/ meltfnum[1] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.CLO__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:179:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:180:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[1])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[2])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!OVERRIDE_GATE_DELAYED_QUEUE */
				 meltfrout->tabval[1])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
     /*_.DELQU_FIRST__V8*/ meltfptr[4] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.DELQU_FIRST__V8*/ meltfptr[4] = NULL;;
	    }
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:180:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.DELQU_FIRST__V8*/ meltfptr[4]),
				(melt_ptr_t) ( /*_.CLO__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:181:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_OVERRIDE_GATE__V9*/ meltfptr[8] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_OVERRIDE_GATE */ meltfrout->
			    tabval[3])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:179:/ quasiblock");


	  /*_.PROGN___V10*/ meltfptr[9] =
	    /*_.ENABLE_OVERRIDE_GATE__V9*/ meltfptr[8];;
	  /*^compute */
	  /*_.IF___V7*/ meltfptr[3] = /*_.PROGN___V10*/ meltfptr[9];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:179:/ clear");
	     /*clear *//*_.DELQU_FIRST__V8*/ meltfptr[4] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_OVERRIDE_GATE__V9*/ meltfptr[8] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V10*/ meltfptr[9] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_.IF___V7*/ meltfptr[3] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:175:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = /*_.IF___V7*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:175:/ locexp");
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
	   /*clear *//*_#IS_CLOSURE__L3*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_.IF___V7*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_OVERRIDE_GATE_FIRST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_.RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST_melt
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

  struct meltframe_meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST_st
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
#define MELTFRAM_NBVARPTR 10
    melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 3
    long mcfr_varnum[3];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST_st *)
	meltfirstargp_;
      /* use arguments meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 10; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST nbval 10*/
  meltfram__.mcfr_nbvar = 10 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_OVERRIDE_GATE_LAST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:183:/ getarg");
 /*_.CLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:186:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:186:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:186:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 186;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "register_override_gate_last clo=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CLO__V2*/ meltfptr[1];
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
	    /*_.IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:186:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_.IF___V4*/ meltfptr[3] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:186:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_.IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:186:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_.IF___V4*/ meltfptr[3] = 0;
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
    /*^compute */
 /*_#IS_CLOSURE__L3*/ meltfnum[1] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.CLO__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:187:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:188:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[1])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[2])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!OVERRIDE_GATE_DELAYED_QUEUE */
				 meltfrout->tabval[1])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "DELQU_LAST");
     /*_.DELQU_LAST__V8*/ meltfptr[4] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.DELQU_LAST__V8*/ meltfptr[4] = NULL;;
	    }
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:188:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.DELQU_LAST__V8*/ meltfptr[4]),
				(melt_ptr_t) ( /*_.CLO__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:189:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_OVERRIDE_GATE__V9*/ meltfptr[8] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_OVERRIDE_GATE */ meltfrout->
			    tabval[3])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:187:/ quasiblock");


	  /*_.PROGN___V10*/ meltfptr[9] =
	    /*_.ENABLE_OVERRIDE_GATE__V9*/ meltfptr[8];;
	  /*^compute */
	  /*_.IF___V7*/ meltfptr[3] = /*_.PROGN___V10*/ meltfptr[9];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:187:/ clear");
	     /*clear *//*_.DELQU_LAST__V8*/ meltfptr[4] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_OVERRIDE_GATE__V9*/ meltfptr[8] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V10*/ meltfptr[9] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_.IF___V7*/ meltfptr[3] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:183:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = /*_.IF___V7*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:183:/ locexp");
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
	   /*clear *//*_#IS_CLOSURE__L3*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_.IF___V7*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_OVERRIDE_GATE_LAST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_.RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST_melt
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

  struct meltframe_meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST_st
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
#define MELTFRAM_NBVARNUM 11
    long mcfr_varnum[11];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST nbval 25*/
  meltfram__.mcfr_nbvar = 25 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("UNREGISTER_OVERRIDE_GATE_FIRST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:191:/ getarg");
 /*_.CLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:195:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:195:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:195:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 195;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"unregister_override_gate_first clo=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CLO__V2*/ meltfptr[1];
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
	    /*_.IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:195:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_.IF___V4*/ meltfptr[3] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:195:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_.IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:195:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_.IF___V4*/ meltfptr[3] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V6*/ meltfptr[4] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V3*/ meltfptr[2] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-hooks.melt:196:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!OVERRIDE_GATE_DELAYED_QUEUE */
					  meltfrout->tabval[1])),
					(melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[2])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->
			   tabval[1])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
   /*_.OLDLIS__V8*/ meltfptr[4] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.OLDLIS__V8*/ meltfptr[4] = NULL;;
      }
    ;
    /*^compute */
 /*_.NEWLIS__V9*/ meltfptr[8] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
    /*^compute */
 /*_#COUNT__L3*/ meltfnum[1] = 0;;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.CURPAIR__V10*/ meltfptr[9] =
	   melt_list_first ((melt_ptr_t) /*_.OLDLIS__V8*/ meltfptr[4]);
	   melt_magic_discr ((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[9]) ==
	   MELTOBMAG_PAIR;
	   /*_.CURPAIR__V10*/ meltfptr[9] =
	   melt_pair_tail ((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[9]))
	{
	  /*_.CURCLO__V11*/ meltfptr[10] =
	    melt_pair_head ((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[9]);



	  MELT_CHECK_SIGNAL ();
	  ;
  /*_#__L4*/ meltfnum[0] =
	    (( /*_.CURCLO__V11*/ meltfptr[10]) ==
	     ( /*_.CLO__V2*/ meltfptr[1]));;
	  MELT_LOCATION ("warmelt-hooks.melt:203:/ cond");
	  /*cond */ if ( /*_#__L4*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

    /*_#I__L6*/ meltfnum[5] =
		  (( /*_#COUNT__L3*/ meltfnum[1]) + (1));;
		MELT_LOCATION ("warmelt-hooks.melt:204:/ compute");
		/*_#COUNT__L3*/ meltfnum[1] = /*_#SETQ___L7*/ meltfnum[6] =
		  /*_#I__L6*/ meltfnum[5];;
		/*_#IFELSE___L5*/ meltfnum[4] = /*_#SETQ___L7*/ meltfnum[6];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:203:/ clear");
	      /*clear *//*_#I__L6*/ meltfnum[5] = 0;
		/*^clear */
	      /*clear *//*_#SETQ___L7*/ meltfnum[6] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {




		{
		  MELT_LOCATION ("warmelt-hooks.melt:205:/ locexp");
		  meltgc_append_list ((melt_ptr_t)
				      ( /*_.NEWLIS__V9*/ meltfptr[8]),
				      (melt_ptr_t) ( /*_.CURCLO__V11*/
						    meltfptr[10]));
		}
		;
	      /*clear *//*_#IFELSE___L5*/ meltfnum[4] = 0;
		/*epilog */
	      }
	      ;
	    }
	  ;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.CURPAIR__V10*/ meltfptr[9] = NULL;
     /*_.CURCLO__V11*/ meltfptr[10] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:200:/ clear");
	    /*clear *//*_.CURPAIR__V10*/ meltfptr[9] = 0;
      /*^clear */
	    /*clear *//*_.CURCLO__V11*/ meltfptr[10] = 0;
      /*^clear */
	    /*clear *//*_#__L4*/ meltfnum[0] = 0;
      /*^clear */
	    /*clear *//*_#IFELSE___L5*/ meltfnum[4] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:206:/ cond");
    /*cond */ if ( /*_#COUNT__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:207:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[1])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[2])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @DELQU_FIRST",
				melt_magic_discr ((melt_ptr_t)
						  (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[1]))) == MELTOBMAG_OBJECT);
		melt_putfield_object ((( /*!OVERRIDE_GATE_DELAYED_QUEUE */
					meltfrout->tabval[1])), (2),
				      ( /*_.NEWLIS__V9*/ meltfptr[8]),
				      "DELQU_FIRST");
		;
		/*^touch */
		meltgc_touch (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->
			       tabval[1]));
		;
		/*^touchobj */

		melt_dbgtrace_written_object (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[1]), "put-fields");
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:208:/ apply");
	  /*apply */
	  {
	    /*_.MAYBE_DISABLE_OVERRIDE_GATE__V13*/ meltfptr[12] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!MAYBE_DISABLE_OVERRIDE_GATE */ meltfrout->
			    tabval[4])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-hooks.melt:209:/ cppif.then");
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
     /*_#MELT_NEED_DBG__L8*/ meltfnum[5] =
	      /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	      ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-hooks.melt:209:/ cond");
	    /*cond */ if ( /*_#MELT_NEED_DBG__L8*/ meltfnum[5])	/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

       /*_#MELT_CALLCOUNT__L9*/ meltfnum[6] =
		    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		    meltcallcount	/* melt_callcount debugging */
#else
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:209:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[5];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L9*/ meltfnum[6];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-hooks.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 209;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "unregister_override_gate_first count=";
		    /*^apply.arg */
		    argtab[4].meltbp_long = /*_#COUNT__L3*/ meltfnum[1];
		    /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15] =
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
		  /*_.IF___V15*/ meltfptr[14] =
		    /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:209:/ clear");
		 /*clear *//*_#MELT_CALLCOUNT__L9*/ meltfnum[6] = 0;
		  /*^clear */
		 /*clear *//*_.MELT_DEBUG_FUN__V16*/ meltfptr[15] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

      /*_.IF___V15*/ meltfptr[14] = NULL;;
	      }
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:209:/ quasiblock");


	    /*_.PROGN___V17*/ meltfptr[15] = /*_.IF___V15*/ meltfptr[14];;
	    /*^compute */
	    /*_.IFCPP___V14*/ meltfptr[13] = /*_.PROGN___V17*/ meltfptr[15];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:209:/ clear");
	       /*clear *//*_#MELT_NEED_DBG__L8*/ meltfnum[5] = 0;
	    /*^clear */
	       /*clear *//*_.IF___V15*/ meltfptr[14] = 0;
	    /*^clear */
	       /*clear *//*_.PROGN___V17*/ meltfptr[15] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V14*/ meltfptr[13] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;
	  /*^compute */
   /*_.BOX__V18*/ meltfptr[14] =
	    /*full constboxing *//*boxing ctype_long */
	    meltgc_new_int ((meltobject_ptr_t)
			    (((melt_ptr_t)
			      (MELT_PREDEF (DISCR_CONSTANT_INTEGER)))),
			    ( /*_#COUNT__L3*/ meltfnum[1]));;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:210:/ quasiblock");


	  /*_.RETVAL___V1*/ meltfptr[0] = /*_.BOX__V18*/ meltfptr[14];;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:210:/ locexp");
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
	  MELT_LOCATION ("warmelt-hooks.melt:206:/ quasiblock");


	  /*_.PROGN___V20*/ meltfptr[19] = /*_.RETURN___V19*/ meltfptr[15];;
	  /*^compute */
	  /*_.IF___V12*/ meltfptr[11] = /*_.PROGN___V20*/ meltfptr[19];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:206:/ clear");
	     /*clear *//*_.MAYBE_DISABLE_OVERRIDE_GATE__V13*/ meltfptr[12] = 0;
	  /*^clear */
	     /*clear *//*_.IFCPP___V14*/ meltfptr[13] = 0;
	  /*^clear */
	     /*clear *//*_.BOX__V18*/ meltfptr[14] = 0;
	  /*^clear */
	     /*clear *//*_.RETURN___V19*/ meltfptr[15] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V20*/ meltfptr[19] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_.IF___V12*/ meltfptr[11] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:211:/ cppif.then");
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
   /*_#MELT_NEED_DBG__L10*/ meltfnum[6] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-hooks.melt:211:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L10*/ meltfnum[6])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L11*/ meltfnum[5] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:211:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[4];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[5];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 211;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"unregister_override_gate_first fail";
	      /*_.MELT_DEBUG_FUN__V23*/ meltfptr[14] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab,
			    "", (union meltparam_un *) 0);
	    }
	    ;
	    /*_.IF___V22*/ meltfptr[13] =
	      /*_.MELT_DEBUG_FUN__V23*/ meltfptr[14];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:211:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L11*/ meltfnum[5] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V23*/ meltfptr[14] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_.IF___V22*/ meltfptr[13] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:211:/ quasiblock");


      /*_.PROGN___V24*/ meltfptr[15] = /*_.IF___V22*/ meltfptr[13];;
      /*^compute */
      /*_.IFCPP___V21*/ meltfptr[12] = /*_.PROGN___V24*/ meltfptr[15];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:211:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L10*/ meltfnum[6] = 0;
      /*^clear */
	     /*clear *//*_.IF___V22*/ meltfptr[13] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V24*/ meltfptr[15] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V21*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:212:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = ( /*nil */ NULL);;

    {
      MELT_LOCATION ("warmelt-hooks.melt:212:/ locexp");
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
    /*_.LET___V7*/ meltfptr[3] = /*_.RETURN___V25*/ meltfptr[19];;

    MELT_LOCATION ("warmelt-hooks.melt:196:/ clear");
	   /*clear *//*_.OLDLIS__V8*/ meltfptr[4] = 0;
    /*^clear */
	   /*clear *//*_.NEWLIS__V9*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_#COUNT__L3*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_.IF___V12*/ meltfptr[11] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V21*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V25*/ meltfptr[19] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:191:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = /*_.LET___V7*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:191:/ locexp");
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
	   /*clear *//*_.LET___V7*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("UNREGISTER_OVERRIDE_GATE_FIRST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_.RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST_melt
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

  struct meltframe_meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST_st
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
#define MELTFRAM_NBVARNUM 11
    long mcfr_varnum[11];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST nbval 25*/
  meltfram__.mcfr_nbvar = 25 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("UNREGISTER_OVERRIDE_GATE_LAST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:214:/ getarg");
 /*_.CLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:218:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:218:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:218:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 218;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "unregister_override_gate_last clo=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CLO__V2*/ meltfptr[1];
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
	    /*_.IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:218:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_.IF___V4*/ meltfptr[3] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:218:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_.IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:218:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_.IF___V4*/ meltfptr[3] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V6*/ meltfptr[4] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V3*/ meltfptr[2] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-hooks.melt:219:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!OVERRIDE_GATE_DELAYED_QUEUE */
					  meltfrout->tabval[1])),
					(melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[2])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->
			   tabval[1])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
   /*_.OLDLIS__V8*/ meltfptr[4] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.OLDLIS__V8*/ meltfptr[4] = NULL;;
      }
    ;
    /*^compute */
 /*_.NEWLIS__V9*/ meltfptr[8] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
    /*^compute */
 /*_#COUNT__L3*/ meltfnum[1] = 0;;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.CURPAIR__V10*/ meltfptr[9] =
	   melt_list_first ((melt_ptr_t) /*_.OLDLIS__V8*/ meltfptr[4]);
	   melt_magic_discr ((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[9]) ==
	   MELTOBMAG_PAIR;
	   /*_.CURPAIR__V10*/ meltfptr[9] =
	   melt_pair_tail ((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[9]))
	{
	  /*_.CURCLO__V11*/ meltfptr[10] =
	    melt_pair_head ((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[9]);



	  MELT_CHECK_SIGNAL ();
	  ;
  /*_#__L4*/ meltfnum[0] =
	    (( /*_.CURCLO__V11*/ meltfptr[10]) ==
	     ( /*_.CLO__V2*/ meltfptr[1]));;
	  MELT_LOCATION ("warmelt-hooks.melt:226:/ cond");
	  /*cond */ if ( /*_#__L4*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

    /*_#I__L6*/ meltfnum[5] =
		  (( /*_#COUNT__L3*/ meltfnum[1]) + (1));;
		MELT_LOCATION ("warmelt-hooks.melt:227:/ compute");
		/*_#COUNT__L3*/ meltfnum[1] = /*_#SETQ___L7*/ meltfnum[6] =
		  /*_#I__L6*/ meltfnum[5];;
		/*_#IFELSE___L5*/ meltfnum[4] = /*_#SETQ___L7*/ meltfnum[6];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:226:/ clear");
	      /*clear *//*_#I__L6*/ meltfnum[5] = 0;
		/*^clear */
	      /*clear *//*_#SETQ___L7*/ meltfnum[6] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {




		{
		  MELT_LOCATION ("warmelt-hooks.melt:228:/ locexp");
		  meltgc_append_list ((melt_ptr_t)
				      ( /*_.NEWLIS__V9*/ meltfptr[8]),
				      (melt_ptr_t) ( /*_.CURCLO__V11*/
						    meltfptr[10]));
		}
		;
	      /*clear *//*_#IFELSE___L5*/ meltfnum[4] = 0;
		/*epilog */
	      }
	      ;
	    }
	  ;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.CURPAIR__V10*/ meltfptr[9] = NULL;
     /*_.CURCLO__V11*/ meltfptr[10] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:223:/ clear");
	    /*clear *//*_.CURPAIR__V10*/ meltfptr[9] = 0;
      /*^clear */
	    /*clear *//*_.CURCLO__V11*/ meltfptr[10] = 0;
      /*^clear */
	    /*clear *//*_#__L4*/ meltfnum[0] = 0;
      /*^clear */
	    /*clear *//*_#IFELSE___L5*/ meltfnum[4] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:229:/ cond");
    /*cond */ if ( /*_#COUNT__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:230:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[1])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[2])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @DELQU_LAST",
				melt_magic_discr ((melt_ptr_t)
						  (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[1]))) == MELTOBMAG_OBJECT);
		melt_putfield_object ((( /*!OVERRIDE_GATE_DELAYED_QUEUE */
					meltfrout->tabval[1])), (3),
				      ( /*_.NEWLIS__V9*/ meltfptr[8]),
				      "DELQU_LAST");
		;
		/*^touch */
		meltgc_touch (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->
			       tabval[1]));
		;
		/*^touchobj */

		melt_dbgtrace_written_object (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[1]), "put-fields");
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:231:/ apply");
	  /*apply */
	  {
	    /*_.MAYBE_DISABLE_OVERRIDE_GATE__V13*/ meltfptr[12] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!MAYBE_DISABLE_OVERRIDE_GATE */ meltfrout->
			    tabval[4])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-hooks.melt:232:/ cppif.then");
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
     /*_#MELT_NEED_DBG__L8*/ meltfnum[5] =
	      /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	      ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-hooks.melt:232:/ cond");
	    /*cond */ if ( /*_#MELT_NEED_DBG__L8*/ meltfnum[5])	/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

       /*_#MELT_CALLCOUNT__L9*/ meltfnum[6] =
		    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		    meltcallcount	/* melt_callcount debugging */
#else
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-hooks.melt:232:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[5];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L9*/ meltfnum[6];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-hooks.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 232;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "unregister_override_gate_last count=";
		    /*^apply.arg */
		    argtab[4].meltbp_long = /*_#COUNT__L3*/ meltfnum[1];
		    /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15] =
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
		  /*_.IF___V15*/ meltfptr[14] =
		    /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-hooks.melt:232:/ clear");
		 /*clear *//*_#MELT_CALLCOUNT__L9*/ meltfnum[6] = 0;
		  /*^clear */
		 /*clear *//*_.MELT_DEBUG_FUN__V16*/ meltfptr[15] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

      /*_.IF___V15*/ meltfptr[14] = NULL;;
	      }
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:232:/ quasiblock");


	    /*_.PROGN___V17*/ meltfptr[15] = /*_.IF___V15*/ meltfptr[14];;
	    /*^compute */
	    /*_.IFCPP___V14*/ meltfptr[13] = /*_.PROGN___V17*/ meltfptr[15];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:232:/ clear");
	       /*clear *//*_#MELT_NEED_DBG__L8*/ meltfnum[5] = 0;
	    /*^clear */
	       /*clear *//*_.IF___V15*/ meltfptr[14] = 0;
	    /*^clear */
	       /*clear *//*_.PROGN___V17*/ meltfptr[15] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V14*/ meltfptr[13] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;
	  /*^compute */
   /*_.BOX__V18*/ meltfptr[14] =
	    /*full constboxing *//*boxing ctype_long */
	    meltgc_new_int ((meltobject_ptr_t)
			    (((melt_ptr_t)
			      (MELT_PREDEF (DISCR_CONSTANT_INTEGER)))),
			    ( /*_#COUNT__L3*/ meltfnum[1]));;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:233:/ quasiblock");


	  /*_.RETVAL___V1*/ meltfptr[0] = /*_.BOX__V18*/ meltfptr[14];;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:233:/ locexp");
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
	  MELT_LOCATION ("warmelt-hooks.melt:229:/ quasiblock");


	  /*_.PROGN___V20*/ meltfptr[19] = /*_.RETURN___V19*/ meltfptr[15];;
	  /*^compute */
	  /*_.IF___V12*/ meltfptr[11] = /*_.PROGN___V20*/ meltfptr[19];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:229:/ clear");
	     /*clear *//*_.MAYBE_DISABLE_OVERRIDE_GATE__V13*/ meltfptr[12] = 0;
	  /*^clear */
	     /*clear *//*_.IFCPP___V14*/ meltfptr[13] = 0;
	  /*^clear */
	     /*clear *//*_.BOX__V18*/ meltfptr[14] = 0;
	  /*^clear */
	     /*clear *//*_.RETURN___V19*/ meltfptr[15] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V20*/ meltfptr[19] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_.IF___V12*/ meltfptr[11] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:234:/ cppif.then");
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
   /*_#MELT_NEED_DBG__L10*/ meltfnum[6] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-hooks.melt:234:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L10*/ meltfnum[6])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L11*/ meltfnum[5] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-hooks.melt:234:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[4];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[5];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 234;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "unregister_override_gate_last fail";
	      /*_.MELT_DEBUG_FUN__V23*/ meltfptr[14] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab,
			    "", (union meltparam_un *) 0);
	    }
	    ;
	    /*_.IF___V22*/ meltfptr[13] =
	      /*_.MELT_DEBUG_FUN__V23*/ meltfptr[14];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:234:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L11*/ meltfnum[5] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V23*/ meltfptr[14] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_.IF___V22*/ meltfptr[13] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-hooks.melt:234:/ quasiblock");


      /*_.PROGN___V24*/ meltfptr[15] = /*_.IF___V22*/ meltfptr[13];;
      /*^compute */
      /*_.IFCPP___V21*/ meltfptr[12] = /*_.PROGN___V24*/ meltfptr[15];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:234:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L10*/ meltfnum[6] = 0;
      /*^clear */
	     /*clear *//*_.IF___V22*/ meltfptr[13] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V24*/ meltfptr[15] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V21*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:235:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = ( /*nil */ NULL);;

    {
      MELT_LOCATION ("warmelt-hooks.melt:235:/ locexp");
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
    /*_.LET___V7*/ meltfptr[3] = /*_.RETURN___V25*/ meltfptr[19];;

    MELT_LOCATION ("warmelt-hooks.melt:219:/ clear");
	   /*clear *//*_.OLDLIS__V8*/ meltfptr[4] = 0;
    /*^clear */
	   /*clear *//*_.NEWLIS__V9*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_#COUNT__L3*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_.IF___V12*/ meltfptr[11] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V21*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V25*/ meltfptr[19] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:214:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = /*_.LET___V7*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:214:/ locexp");
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
	   /*clear *//*_.LET___V7*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("UNREGISTER_OVERRIDE_GATE_LAST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_.RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_9_warmelt_hooks_LAMBDA___1__ (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un * meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_9_warmelt_hooks_LAMBDA___1___melt =
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

  struct meltframe_meltrout_9_warmelt_hooks_LAMBDA___1___st
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
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_9_warmelt_hooks_LAMBDA___1__ is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_9_warmelt_hooks_LAMBDA___1___st *)
	meltfirstargp_;
      /* use arguments meltrout_9_warmelt_hooks_LAMBDA___1__ output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_9_warmelt_hooks_LAMBDA___1__ nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:269:/ getarg");
 /*_.FIRSTPROC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:270:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) (( /*~RESCONT */ meltfclos->tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
  /*_.PREVRES__V3*/ meltfptr[2] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:271:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!START_UNIT_DELAYED_QUEUE */ meltfrout->
			  tabval[0]);
      /*_.NEXTRES__V4*/ meltfptr[3] =
	melt_apply ((meltclosure_ptr_t) ( /*_.FIRSTPROC__V2*/ meltfptr[1]),
		    (melt_ptr_t) ( /*_.PREVRES__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:273:/ quasiblock");


    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
		    melt_magic_discr ((melt_ptr_t)
				      (( /*~RESCONT */ meltfclos->
					tabval[0]))) == MELTOBMAG_OBJECT);
    melt_putfield_object ((( /*~RESCONT */ meltfclos->tabval[0])), (0),
			  ( /*_.NEXTRES__V4*/ meltfptr[3]),
			  "REFERENCED_VALUE");
    ;
    /*^touch */
    meltgc_touch (( /*~RESCONT */ meltfclos->tabval[0]));
    ;
    /*^touchobj */

    melt_dbgtrace_written_object (( /*~RESCONT */ meltfclos->tabval[0]),
				  "put-fields");
    ;


    MELT_LOCATION ("warmelt-hooks.melt:270:/ clear");
	   /*clear *//*_.PREVRES__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_.NEXTRES__V4*/ meltfptr[3] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_9_warmelt_hooks_LAMBDA___1___melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_9_warmelt_hooks_LAMBDA___1__ */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_warmelt_hooks_LAMBDA___2__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un * meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_10_warmelt_hooks_LAMBDA___2___melt
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

  struct meltframe_meltrout_10_warmelt_hooks_LAMBDA___2___st
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
#define MELTFRAM_NBVARPTR 2
    melt_ptr_t mcfr_varptr[2];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_10_warmelt_hooks_LAMBDA___2__ is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_10_warmelt_hooks_LAMBDA___2___st *)
	meltfirstargp_;
      /* use arguments meltrout_10_warmelt_hooks_LAMBDA___2__ output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 2; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_10_warmelt_hooks_LAMBDA___2__ nbval 2*/
  meltfram__.mcfr_nbvar = 2 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:278:/ getarg");
 /*_.LASTPROC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.LASTPROC__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:279:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    /*^locexp */
	    meltgc_prepend_list ((melt_ptr_t)
				 (( /*~REVLASTLIST */ meltfclos->tabval[0])),
				 (melt_ptr_t) ( /*_.LASTPROC__V2*/
					       meltfptr[1]));
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:278:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_10_warmelt_hooks_LAMBDA___2___melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_10_warmelt_hooks_LAMBDA___2__ */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_warmelt_hooks_LAMBDA___3__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un * meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_11_warmelt_hooks_LAMBDA___3___melt
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

  struct meltframe_meltrout_11_warmelt_hooks_LAMBDA___3___st
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
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_11_warmelt_hooks_LAMBDA___3__ is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_11_warmelt_hooks_LAMBDA___3___st *)
	meltfirstargp_;
      /* use arguments meltrout_11_warmelt_hooks_LAMBDA___3__ output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_11_warmelt_hooks_LAMBDA___3__ nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:283:/ getarg");
 /*_.LASTPROC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:284:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) (( /*~RESCONT */ meltfclos->tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
  /*_.PREVRES__V3*/ meltfptr[2] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:285:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!START_UNIT_DELAYED_QUEUE */ meltfrout->
			  tabval[0]);
      /*_.NEXTRES__V4*/ meltfptr[3] =
	melt_apply ((meltclosure_ptr_t) ( /*_.LASTPROC__V2*/ meltfptr[1]),
		    (melt_ptr_t) ( /*_.PREVRES__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:287:/ quasiblock");


    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
		    melt_magic_discr ((melt_ptr_t)
				      (( /*~RESCONT */ meltfclos->
					tabval[0]))) == MELTOBMAG_OBJECT);
    melt_putfield_object ((( /*~RESCONT */ meltfclos->tabval[0])), (0),
			  ( /*_.NEXTRES__V4*/ meltfptr[3]),
			  "REFERENCED_VALUE");
    ;
    /*^touch */
    meltgc_touch (( /*~RESCONT */ meltfclos->tabval[0]));
    ;
    /*^touchobj */

    melt_dbgtrace_written_object (( /*~RESCONT */ meltfclos->tabval[0]),
				  "put-fields");
    ;


    MELT_LOCATION ("warmelt-hooks.melt:284:/ clear");
	   /*clear *//*_.PREVRES__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_.NEXTRES__V4*/ meltfptr[3] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_11_warmelt_hooks_LAMBDA___3___melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_11_warmelt_hooks_LAMBDA___3__ */

/* frame for melthook_HOOK_START_UNIT hook: */
typedef struct meltframe_melthook_HOOK_START_UNIT_st
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
#define MELTFRAM_NBVARPTR 16
  melt_ptr_t mcfr_varptr[16];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
  long _spare_;
}
melthookframe_HOOK_START_UNIT_t;	/* end of hook frame melthook_HOOK_START_UNIT */


/** implementation of hook melthook_HOOK_START_UNIT */
void
melthook_HOOK_START_UNIT (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_START_UNIT from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_START_UNIT hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_START_UNIT;
  long melt_thishookcall_counter_HOOK_START_UNIT ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_START_UNIT++;
#define meltcallcount melt_thishookcall_counter_HOOK_START_UNIT
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_START_UNIT_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_START_UNIT */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -3135 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_START_UNIT_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_START_UNIT*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_START_UNIT hook */
  MELT_LOCATION ("warmelt-hooks.melt:259:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:261:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (( /*~START_UNIT_DELAYED_QUEUE */ meltfhook->
		       tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
   /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-hooks.melt:262:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (( /*~START_UNIT_DELAYED_QUEUE */ meltfhook->
		       tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 3, "DELQU_LAST");
   /*_.LASTLIST__V4*/ meltfptr[3] = slot;
    };
    ;
  /*_.REVLASTLIST__V5*/ meltfptr[4] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[1]))));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:264:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*~CLASS_REFERENCE */
					     meltfhook->tabval[2])), (1),
			      "CLASS_REFERENCE");
   /*_.INST__V7*/ meltfptr[6] =
	newobj;
    };
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V7*/ meltfptr[6],
				  "newly made instance");
    ;
    /*_.RESCONT__V6*/ meltfptr[5] = /*_.INST__V7*/ meltfptr[6];;
    MELT_LOCATION ("warmelt-hooks.melt:269:/ quasiblock");


    /*^newclosure */
		  /*newclosure *//*_.LAMBDA___V9*/ meltfptr[8] =
      (melt_ptr_t)
      meltgc_new_closure ((meltobject_ptr_t)
			  (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			  (meltroutine_ptr_t) ( /*hooknth: */
			  (((melthook_ptr_t)
			    ( /*_.HOOKDATA___V1*/ meltfptr[0]))->tabval[6])),
			  (1));
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.LAMBDA___V9*/ meltfptr[8])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 0 >= 0
		    && 0 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.LAMBDA___V9*/ meltfptr[8])));
    ((meltclosure_ptr_t) /*_.LAMBDA___V9*/ meltfptr[8])->tabval[0] =
      (melt_ptr_t) ( /*_.RESCONT__V6*/ meltfptr[5]);
    ;
    /*_.LAMBDA___V8*/ meltfptr[7] = /*_.LAMBDA___V9*/ meltfptr[8];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:267:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.LAMBDA___V8*/ meltfptr[7];
      /*_.LIST_EVERY__V10*/ meltfptr[9] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~LIST_EVERY */ meltfhook->tabval[3])),
		    (melt_ptr_t) ( /*_.FIRSTLIST__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:278:/ quasiblock");


    /*^newclosure */
		  /*newclosure *//*_.LAMBDA___V12*/ meltfptr[11] =
      (melt_ptr_t)
      meltgc_new_closure ((meltobject_ptr_t)
			  (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			  (meltroutine_ptr_t) ( /*hooknth: */
			  (((melthook_ptr_t)
			    ( /*_.HOOKDATA___V1*/ meltfptr[0]))->tabval[7])),
			  (1));
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.LAMBDA___V12*/ meltfptr[11])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 0 >= 0
		    && 0 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.LAMBDA___V12*/ meltfptr[11])));
    ((meltclosure_ptr_t) /*_.LAMBDA___V12*/ meltfptr[11])->tabval[0] =
      (melt_ptr_t) ( /*_.REVLASTLIST__V5*/ meltfptr[4]);
    ;
    /*_.LAMBDA___V11*/ meltfptr[10] = /*_.LAMBDA___V12*/ meltfptr[11];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:276:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.LAMBDA___V11*/ meltfptr[10];
      /*_.LIST_EVERY__V13*/ meltfptr[12] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~LIST_EVERY */ meltfhook->tabval[3])),
		    (melt_ptr_t) ( /*_.LASTLIST__V4*/ meltfptr[3]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:283:/ quasiblock");


    /*^newclosure */
		  /*newclosure *//*_.LAMBDA___V15*/ meltfptr[14] =
      (melt_ptr_t)
      meltgc_new_closure ((meltobject_ptr_t)
			  (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			  (meltroutine_ptr_t) ( /*hooknth: */
			  (((melthook_ptr_t)
			    ( /*_.HOOKDATA___V1*/ meltfptr[0]))->tabval[8])),
			  (1));
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.LAMBDA___V15*/ meltfptr[14])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 0 >= 0
		    && 0 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.LAMBDA___V15*/ meltfptr[14])));
    ((meltclosure_ptr_t) /*_.LAMBDA___V15*/ meltfptr[14])->tabval[0] =
      (melt_ptr_t) ( /*_.RESCONT__V6*/ meltfptr[5]);
    ;
    /*_.LAMBDA___V14*/ meltfptr[13] = /*_.LAMBDA___V15*/ meltfptr[14];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:281:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.LAMBDA___V14*/ meltfptr[13];
      /*_.LIST_EVERY__V16*/ meltfptr[15] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~LIST_EVERY */ meltfhook->tabval[3])),
		    (melt_ptr_t) ( /*_.REVLASTLIST__V5*/ meltfptr[4]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    /*_.LET___V2*/ meltfptr[1] = /*_.LIST_EVERY__V16*/ meltfptr[15];;

    MELT_LOCATION ("warmelt-hooks.melt:261:/ clear");
	    /*clear *//*_.FIRSTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V5*/ meltfptr[4] = 0;
    /*^clear */
	    /*clear *//*_.RESCONT__V6*/ meltfptr[5] = 0;
    /*^clear */
	    /*clear *//*_.LAMBDA___V8*/ meltfptr[7] = 0;
    /*^clear */
	    /*clear *//*_.LIST_EVERY__V10*/ meltfptr[9] = 0;
    /*^clear */
	    /*clear *//*_.LAMBDA___V11*/ meltfptr[10] = 0;
    /*^clear */
	    /*clear *//*_.LIST_EVERY__V13*/ meltfptr[12] = 0;
    /*^clear */
	    /*clear *//*_.LAMBDA___V14*/ meltfptr[13] = 0;
    /*^clear */
	    /*clear *//*_.LIST_EVERY__V16*/ meltfptr[15] = 0;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:259:/ clear");
	    /*clear *//*_.LET___V2*/ meltfptr[1] = 0;
  }
  ;
  /* epilogue of melthook_HOOK_START_UNIT hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_START_UNIT hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_START_UNIT hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_START_UNIThook */
void
melthookmark_HOOK_START_UNIT_rout (struct melt_callframe_st *meltcfram,
				   int meltmarking)
{

  melthookframe_HOOK_START_UNIT_t *meltframptr_
    = (melthookframe_HOOK_START_UNIT_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_START_UNIT good hash",
		  meltframptr_->mcfr_nbvar == -3135);
  if (meltmarking)
    {				/* marking hook HOOK_START_UNIT */
      int meltix;
      for (meltix = 0; meltix < 16; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_START_UNIT */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 16; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_START_UNIT */
}				/* end  melthookmark_HOOK_START_UNIT_rout */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_warmelt_hooks_ENABLE_START_UNIT (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un *meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_13_warmelt_hooks_ENABLE_START_UNIT_melt
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

  struct meltframe_meltrout_13_warmelt_hooks_ENABLE_START_UNIT_st
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
  /*meltrout_13_warmelt_hooks_ENABLE_START_UNIT is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_13_warmelt_hooks_ENABLE_START_UNIT_st *)
	meltfirstargp_;
      /* use arguments meltrout_13_warmelt_hooks_ENABLE_START_UNIT output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_13_warmelt_hooks_ENABLE_START_UNIT nbval 1*/
  meltfram__.mcfr_nbvar = 1 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ENABLE_START_UNIT", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:307:/ block");
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-hooks.melt:308:/ locexp");

      if (!melthk_start_unit_registered_flag)
	{
	  melthk_start_unit_registered_flag = true;
	  register_callback (melt_plugin_name, PLUGIN_START_UNIT,
			     melt_startunithook_callback, NULL);
	}
      ;
    }
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ENABLE_START_UNIT", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_13_warmelt_hooks_ENABLE_START_UNIT_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_13_warmelt_hooks_ENABLE_START_UNIT */

/* frame for melthook_HOOK_FINISH_UNIT hook: */
typedef struct meltframe_melthook_HOOK_FINISH_UNIT_st
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
#define MELTFRAM_NBVARPTR 15
  melt_ptr_t mcfr_varptr[15];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
  long _spare_;
}
melthookframe_HOOK_FINISH_UNIT_t;	/* end of hook frame melthook_HOOK_FINISH_UNIT */


/** implementation of hook melthook_HOOK_FINISH_UNIT */
void
melthook_HOOK_FINISH_UNIT (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_FINISH_UNIT from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_FINISH_UNIT hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_FINISH_UNIT;
  long melt_thishookcall_counter_HOOK_FINISH_UNIT ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_FINISH_UNIT++;
#define meltcallcount melt_thishookcall_counter_HOOK_FINISH_UNIT
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_FINISH_UNIT_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_FINISH_UNIT */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -1467 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_FINISH_UNIT_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_FINISH_UNIT*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_FINISH_UNIT hook */
  MELT_LOCATION ("warmelt-hooks.melt:361:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:363:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (( /*~FINISH_UNIT_DELAYED_QUEUE */ meltfhook->
		       tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
   /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-hooks.melt:364:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (( /*~FINISH_UNIT_DELAYED_QUEUE */ meltfhook->
		       tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 3, "DELQU_LAST");
   /*_.LASTLIST__V3*/ meltfptr[2] = slot;
    };
    ;
  /*_.REVLASTLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[1]))));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:366:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*~CLASS_REFERENCE */
					     meltfhook->tabval[2])), (1),
			      "CLASS_REFERENCE");
   /*_.INST__V6*/ meltfptr[5] =
	newobj;
    };
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V6*/ meltfptr[5],
				  "newly made instance");
    ;
    /*_.RESCONT__V5*/ meltfptr[4] = /*_.INST__V6*/ meltfptr[5];;
    MELT_LOCATION ("warmelt-hooks.melt:371:/ quasiblock");


    /*^newclosure */
		  /*newclosure *//*_.LAMBDA___V8*/ meltfptr[7] =
      (melt_ptr_t)
      meltgc_new_closure ((meltobject_ptr_t)
			  (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			  (meltroutine_ptr_t) ( /*hooknth: */
			  (((melthook_ptr_t)
			    ( /*_.HOOKDATA___V1*/ meltfptr[0]))->tabval[4])),
			  (1));
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.LAMBDA___V8*/ meltfptr[7])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 0 >= 0
		    && 0 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.LAMBDA___V8*/ meltfptr[7])));
    ((meltclosure_ptr_t) /*_.LAMBDA___V8*/ meltfptr[7])->tabval[0] =
      (melt_ptr_t) ( /*_.RESCONT__V5*/ meltfptr[4]);
    ;
    /*_.LAMBDA___V7*/ meltfptr[6] = /*_.LAMBDA___V8*/ meltfptr[7];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:369:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.LAMBDA___V7*/ meltfptr[6];
      /*_.LIST_EVERY__V9*/ meltfptr[8] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~LIST_EVERY */ meltfhook->tabval[3])),
		    (melt_ptr_t) ( /*_.FIRSTLIST__V2*/ meltfptr[1]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:380:/ quasiblock");


    /*^newclosure */
		  /*newclosure *//*_.LAMBDA___V11*/ meltfptr[10] =
      (melt_ptr_t)
      meltgc_new_closure ((meltobject_ptr_t)
			  (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			  (meltroutine_ptr_t) ( /*hooknth: */
			  (((melthook_ptr_t)
			    ( /*_.HOOKDATA___V1*/ meltfptr[0]))->tabval[5])),
			  (1));
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.LAMBDA___V11*/ meltfptr[10])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 0 >= 0
		    && 0 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.LAMBDA___V11*/ meltfptr[10])));
    ((meltclosure_ptr_t) /*_.LAMBDA___V11*/ meltfptr[10])->tabval[0] =
      (melt_ptr_t) ( /*_.REVLASTLIST__V4*/ meltfptr[3]);
    ;
    /*_.LAMBDA___V10*/ meltfptr[9] = /*_.LAMBDA___V11*/ meltfptr[10];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:378:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.LAMBDA___V10*/ meltfptr[9];
      /*_.LIST_EVERY__V12*/ meltfptr[11] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~LIST_EVERY */ meltfhook->tabval[3])),
		    (melt_ptr_t) ( /*_.LASTLIST__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:385:/ quasiblock");


    /*^newclosure */
		  /*newclosure *//*_.LAMBDA___V14*/ meltfptr[13] =
      (melt_ptr_t)
      meltgc_new_closure ((meltobject_ptr_t)
			  (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE)))),
			  (meltroutine_ptr_t) ( /*hooknth: */
			  (((melthook_ptr_t)
			    ( /*_.HOOKDATA___V1*/ meltfptr[0]))->tabval[6])),
			  (1));
    ;
    /*^putclosedv */
    /*putclosv */
    melt_assertmsg ("putclosv checkclo",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.LAMBDA___V14*/ meltfptr[13])) ==
		    MELTOBMAG_CLOSURE);
    melt_assertmsg ("putclosv checkoff", 0 >= 0
		    && 0 <
		    melt_closure_size ((melt_ptr_t)
				       ( /*_.LAMBDA___V14*/ meltfptr[13])));
    ((meltclosure_ptr_t) /*_.LAMBDA___V14*/ meltfptr[13])->tabval[0] =
      (melt_ptr_t) ( /*_.RESCONT__V5*/ meltfptr[4]);
    ;
    /*_.LAMBDA___V13*/ meltfptr[12] = /*_.LAMBDA___V14*/ meltfptr[13];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:383:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.LAMBDA___V13*/ meltfptr[12];
      /*_.LIST_EVERY__V15*/ meltfptr[14] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*~LIST_EVERY */ meltfhook->tabval[3])),
		    (melt_ptr_t) ( /*_.REVLASTLIST__V4*/ meltfptr[3]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:392:/ locexp");
      /*hook_finish_unit FINISHUNITMINORGC_CHK__1 */ melt_garbcoll (0,
								    MELT_ONLY_MINOR);
    }
    ;

    MELT_LOCATION ("warmelt-hooks.melt:363:/ clear");
	    /*clear *//*_.FIRSTLIST__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.RESCONT__V5*/ meltfptr[4] = 0;
    /*^clear */
	    /*clear *//*_.LAMBDA___V7*/ meltfptr[6] = 0;
    /*^clear */
	    /*clear *//*_.LIST_EVERY__V9*/ meltfptr[8] = 0;
    /*^clear */
	    /*clear *//*_.LAMBDA___V10*/ meltfptr[9] = 0;
    /*^clear */
	    /*clear *//*_.LIST_EVERY__V12*/ meltfptr[11] = 0;
    /*^clear */
	    /*clear *//*_.LAMBDA___V13*/ meltfptr[12] = 0;
    /*^clear */
	    /*clear *//*_.LIST_EVERY__V15*/ meltfptr[14] = 0;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_FINISH_UNIT hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_FINISH_UNIT hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_FINISH_UNIT hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_FINISH_UNIThook */
void
melthookmark_HOOK_FINISH_UNIT_rout (struct melt_callframe_st *meltcfram,
				    int meltmarking)
{

  melthookframe_HOOK_FINISH_UNIT_t *meltframptr_
    = (melthookframe_HOOK_FINISH_UNIT_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_FINISH_UNIT good hash",
		  meltframptr_->mcfr_nbvar == -1467);
  if (meltmarking)
    {				/* marking hook HOOK_FINISH_UNIT */
      int meltix;
      for (meltix = 0; meltix < 15; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_FINISH_UNIT */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 15; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_FINISH_UNIT */
}				/* end  melthookmark_HOOK_FINISH_UNIT_rout */

/* frame for melthook_HOOK_ALL_PASSES_START hook: */
typedef struct meltframe_melthook_HOOK_ALL_PASSES_START_st
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
#define MELTFRAM_NBVARPTR 15
  melt_ptr_t mcfr_varptr[15];
#define MELTFRAM_NBVARNUM 1
  long mcfr_varnum[1];
/*others*/
  long _spare_;
}
melthookframe_HOOK_ALL_PASSES_START_t;	/* end of hook frame melthook_HOOK_ALL_PASSES_START */


/** implementation of hook melthook_HOOK_ALL_PASSES_START */
void
melthook_HOOK_ALL_PASSES_START (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_ALL_PASSES_START from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_ALL_PASSES_START hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_ALL_PASSES_START;
  long melt_thishookcall_counter_HOOK_ALL_PASSES_START ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_ALL_PASSES_START++;
#define meltcallcount melt_thishookcall_counter_HOOK_ALL_PASSES_START
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_ALL_PASSES_START_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_ALL_PASSES_START */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -184 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_ALL_PASSES_START_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_ALL_PASSES_START*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_ALL_PASSES_START hook */
  MELT_LOCATION ("warmelt-hooks.melt:472:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:474:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~ALL_PASSES_START_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~ALL_PASSES_START_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.FIRSTLIST__V2*/ meltfptr[1] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:475:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~ALL_PASSES_START_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~ALL_PASSES_START_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
      }
    ;
    /*^compute */
  /*_.REVLASTLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    /*^compute */
    /*_.RES__V5*/ meltfptr[4] = ( /*nil */ NULL);;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_list_first ((melt_ptr_t) /*_.FIRSTLIST__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]) ==
	   MELTOBMAG_PAIR;
	   /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_pair_tail ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]))
	{
	  /*_.FIRSTPROC__V7*/ meltfptr[6] =
	    melt_pair_head ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:483:/ apply");
	  /*apply */
	  {
	    /*_.FIRSTPROC__V8*/ meltfptr[7] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.FIRSTPROC__V7*/ meltfptr[6]),
			  (melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V9*/ meltfptr[8] =
	    /*_.FIRSTPROC__V8*/ meltfptr[7];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.FIRSTPAIR__V6*/ meltfptr[5] = NULL;
     /*_.FIRSTPROC__V7*/ meltfptr[6] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:480:/ clear");
	     /*clear *//*_.FIRSTPAIR__V6*/ meltfptr[5] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V8*/ meltfptr[7] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V9*/ meltfptr[8] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_list_first ((melt_ptr_t) /*_.LASTLIST__V3*/ meltfptr[2]);
	   melt_magic_discr ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]) ==
	   MELTOBMAG_PAIR;
	   /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_pair_tail ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]))
	{
	  /*_.LASTPROC__V11*/ meltfptr[10] =
	    melt_pair_head ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L1*/ meltfnum[0] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.LASTPROC__V11*/ meltfptr[10])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:489:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  /*^locexp */
		  meltgc_prepend_list ((melt_ptr_t)
				       ( /*_.REVLASTLIST__V4*/ meltfptr[3]),
				       (melt_ptr_t) ( /*_.LASTPROC__V11*/
						     meltfptr[10]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.LASTPAIR__V10*/ meltfptr[9] = NULL;
     /*_.LASTPROC__V11*/ meltfptr[10] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:486:/ clear");
	     /*clear *//*_.LASTPAIR__V10*/ meltfptr[9] = 0;
      /*^clear */
	     /*clear *//*_.LASTPROC__V11*/ meltfptr[10] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:491:/ cond");
    /*cond */ if ( /*_.LASTLIST__V3*/ meltfptr[2])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*citerblock FOREACH_IN_LIST */
	  {
	    /* start foreach_in_list meltcit3__EACHLIST */
	    for ( /*_.REVLASTPAIR__V12*/ meltfptr[11] =
		 melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V4*/
				  meltfptr[3]);
		 melt_magic_discr ((melt_ptr_t) /*_.REVLASTPAIR__V12*/
				   meltfptr[11]) == MELTOBMAG_PAIR;
		 /*_.REVLASTPAIR__V12*/ meltfptr[11] =
		 melt_pair_tail ((melt_ptr_t) /*_.REVLASTPAIR__V12*/
				 meltfptr[11]))
	      {
		/*_.REVLASTPROC__V13*/ meltfptr[12] =
		  melt_pair_head ((melt_ptr_t) /*_.REVLASTPAIR__V12*/
				  meltfptr[11]);



		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-hooks.melt:495:/ apply");
		/*apply */
		{
		  /*_.REVLASTPROC__V14*/ meltfptr[13] =
		    melt_apply ((meltclosure_ptr_t)
				( /*_.REVLASTPROC__V13*/ meltfptr[12]),
				(melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]),
				(""), (union meltparam_un *) 0, "",
				(union meltparam_un *) 0);
		}
		;
		/*^compute */
		/*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V15*/ meltfptr[14] =
		  /*_.REVLASTPROC__V14*/ meltfptr[13];;
	      }			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.REVLASTPAIR__V12*/ meltfptr[11] = NULL;
     /*_.REVLASTPROC__V13*/ meltfptr[12] = NULL;


	    /*citerepilog */

	    MELT_LOCATION ("warmelt-hooks.melt:492:/ clear");
	       /*clear *//*_.REVLASTPAIR__V12*/ meltfptr[11] = 0;
	    /*^clear */
	       /*clear *//*_.REVLASTPROC__V13*/ meltfptr[12] = 0;
	    /*^clear */
	       /*clear *//*_.REVLASTPROC__V14*/ meltfptr[13] = 0;
	    /*^clear */
	       /*clear *//*_.SETQ___V15*/ meltfptr[14] = 0;
	  }			/*endciterblock FOREACH_IN_LIST */
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:474:/ clear");
	    /*clear *//*_.FIRSTLIST__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.RES__V5*/ meltfptr[4] = 0;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_ALL_PASSES_START hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_ALL_PASSES_START hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_ALL_PASSES_START hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_ALL_PASSES_STARThook */
void
melthookmark_HOOK_ALL_PASSES_START_rout (struct melt_callframe_st *meltcfram,
					 int meltmarking)
{

  melthookframe_HOOK_ALL_PASSES_START_t *meltframptr_
    = (melthookframe_HOOK_ALL_PASSES_START_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_ALL_PASSES_START good hash",
		  meltframptr_->mcfr_nbvar == -184);
  if (meltmarking)
    {				/* marking hook HOOK_ALL_PASSES_START */
      int meltix;
      for (meltix = 0; meltix < 15; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_ALL_PASSES_START */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 15; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_ALL_PASSES_START */
}				/* end  melthookmark_HOOK_ALL_PASSES_START_rout */

/* frame for melthook_HOOK_ALL_PASSES_END hook: */
typedef struct meltframe_melthook_HOOK_ALL_PASSES_END_st
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
#define MELTFRAM_NBVARPTR 15
  melt_ptr_t mcfr_varptr[15];
#define MELTFRAM_NBVARNUM 1
  long mcfr_varnum[1];
/*others*/
  long _spare_;
}
melthookframe_HOOK_ALL_PASSES_END_t;	/* end of hook frame melthook_HOOK_ALL_PASSES_END */


/** implementation of hook melthook_HOOK_ALL_PASSES_END */
void
melthook_HOOK_ALL_PASSES_END (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_ALL_PASSES_END from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_ALL_PASSES_END hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_ALL_PASSES_END;
  long melt_thishookcall_counter_HOOK_ALL_PASSES_END ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_ALL_PASSES_END++;
#define meltcallcount melt_thishookcall_counter_HOOK_ALL_PASSES_END
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_ALL_PASSES_END_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_ALL_PASSES_END */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -3890 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_ALL_PASSES_END_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_ALL_PASSES_END*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_ALL_PASSES_END hook */
  MELT_LOCATION ("warmelt-hooks.melt:596:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:598:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~ALL_PASSES_END_DELAYED_QUEUE */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~ALL_PASSES_END_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.FIRSTLIST__V2*/ meltfptr[1] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:599:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~ALL_PASSES_END_DELAYED_QUEUE */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~ALL_PASSES_END_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
      }
    ;
    /*^compute */
  /*_.REVLASTLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    /*^compute */
    /*_.RES__V5*/ meltfptr[4] = ( /*nil */ NULL);;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_list_first ((melt_ptr_t) /*_.FIRSTLIST__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]) ==
	   MELTOBMAG_PAIR;
	   /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_pair_tail ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]))
	{
	  /*_.FIRSTPROC__V7*/ meltfptr[6] =
	    melt_pair_head ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:607:/ apply");
	  /*apply */
	  {
	    /*_.FIRSTPROC__V8*/ meltfptr[7] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.FIRSTPROC__V7*/ meltfptr[6]),
			  (melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V9*/ meltfptr[8] =
	    /*_.FIRSTPROC__V8*/ meltfptr[7];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.FIRSTPAIR__V6*/ meltfptr[5] = NULL;
     /*_.FIRSTPROC__V7*/ meltfptr[6] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:604:/ clear");
	     /*clear *//*_.FIRSTPAIR__V6*/ meltfptr[5] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V8*/ meltfptr[7] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V9*/ meltfptr[8] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_list_first ((melt_ptr_t) /*_.LASTLIST__V3*/ meltfptr[2]);
	   melt_magic_discr ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]) ==
	   MELTOBMAG_PAIR;
	   /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_pair_tail ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]))
	{
	  /*_.LASTPROC__V11*/ meltfptr[10] =
	    melt_pair_head ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L1*/ meltfnum[0] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.LASTPROC__V11*/ meltfptr[10])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:613:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  /*^locexp */
		  meltgc_prepend_list ((melt_ptr_t)
				       ( /*_.REVLASTLIST__V4*/ meltfptr[3]),
				       (melt_ptr_t) ( /*_.LASTPROC__V11*/
						     meltfptr[10]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.LASTPAIR__V10*/ meltfptr[9] = NULL;
     /*_.LASTPROC__V11*/ meltfptr[10] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:610:/ clear");
	     /*clear *//*_.LASTPAIR__V10*/ meltfptr[9] = 0;
      /*^clear */
	     /*clear *//*_.LASTPROC__V11*/ meltfptr[10] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit3__EACHLIST */
      for ( /*_.REVLASTPAIR__V12*/ meltfptr[11] =
	   melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V4*/ meltfptr[3]);
	   melt_magic_discr ((melt_ptr_t) /*_.REVLASTPAIR__V12*/ meltfptr[11])
	   == MELTOBMAG_PAIR;
	   /*_.REVLASTPAIR__V12*/ meltfptr[11] =
	   melt_pair_tail ((melt_ptr_t) /*_.REVLASTPAIR__V12*/ meltfptr[11]))
	{
	  /*_.REVLASTPROC__V13*/ meltfptr[12] =
	    melt_pair_head ((melt_ptr_t) /*_.REVLASTPAIR__V12*/ meltfptr[11]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:618:/ apply");
	  /*apply */
	  {
	    /*_.REVLASTPROC__V14*/ meltfptr[13] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.REVLASTPROC__V13*/ meltfptr[12]),
			  (melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V15*/ meltfptr[14] =
	    /*_.REVLASTPROC__V14*/ meltfptr[13];;
	}			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.REVLASTPAIR__V12*/ meltfptr[11] = NULL;
     /*_.REVLASTPROC__V13*/ meltfptr[12] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:615:/ clear");
	     /*clear *//*_.REVLASTPAIR__V12*/ meltfptr[11] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V14*/ meltfptr[13] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V15*/ meltfptr[14] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:598:/ clear");
	    /*clear *//*_.FIRSTLIST__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.RES__V5*/ meltfptr[4] = 0;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_ALL_PASSES_END hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_ALL_PASSES_END hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_ALL_PASSES_END hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_ALL_PASSES_ENDhook */
void
melthookmark_HOOK_ALL_PASSES_END_rout (struct melt_callframe_st *meltcfram,
				       int meltmarking)
{

  melthookframe_HOOK_ALL_PASSES_END_t *meltframptr_
    = (melthookframe_HOOK_ALL_PASSES_END_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_ALL_PASSES_END good hash",
		  meltframptr_->mcfr_nbvar == -3890);
  if (meltmarking)
    {				/* marking hook HOOK_ALL_PASSES_END */
      int meltix;
      for (meltix = 0; meltix < 15; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_ALL_PASSES_END */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 15; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_ALL_PASSES_END */
}				/* end  melthookmark_HOOK_ALL_PASSES_END_rout */

/* frame for melthook_HOOK_ALL_IPA_PASSES_START hook: */
typedef struct meltframe_melthook_HOOK_ALL_IPA_PASSES_START_st
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
#define MELTFRAM_NBVARPTR 15
  melt_ptr_t mcfr_varptr[15];
#define MELTFRAM_NBVARNUM 1
  long mcfr_varnum[1];
/*others*/
  long _spare_;
}
melthookframe_HOOK_ALL_IPA_PASSES_START_t;	/* end of hook frame melthook_HOOK_ALL_IPA_PASSES_START */


/** implementation of hook melthook_HOOK_ALL_IPA_PASSES_START */
void
melthook_HOOK_ALL_IPA_PASSES_START (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_ALL_IPA_PASSES_START from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_ALL_IPA_PASSES_START hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_ALL_IPA_PASSES_START;
  long melt_thishookcall_counter_HOOK_ALL_IPA_PASSES_START ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_ALL_IPA_PASSES_START++;
#define meltcallcount melt_thishookcall_counter_HOOK_ALL_IPA_PASSES_START
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_ALL_IPA_PASSES_START_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_ALL_IPA_PASSES_START */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -1767 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_ALL_IPA_PASSES_START_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_ALL_IPA_PASSES_START*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_ALL_IPA_PASSES_START hook */
  MELT_LOCATION ("warmelt-hooks.melt:717:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:719:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~ALL_IPA_PASSES_START_DELAYED_QUEUE */
			   meltfhook->tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.FIRSTLIST__V2*/ meltfptr[1] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:720:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~ALL_IPA_PASSES_START_DELAYED_QUEUE */
			   meltfhook->tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
      }
    ;
    /*^compute */
  /*_.REVLASTLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    /*^compute */
    /*_.RES__V5*/ meltfptr[4] = ( /*nil */ NULL);;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_list_first ((melt_ptr_t) /*_.FIRSTLIST__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]) ==
	   MELTOBMAG_PAIR;
	   /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_pair_tail ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]))
	{
	  /*_.FIRSTPROC__V7*/ meltfptr[6] =
	    melt_pair_head ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:728:/ apply");
	  /*apply */
	  {
	    /*_.FIRSTPROC__V8*/ meltfptr[7] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.FIRSTPROC__V7*/ meltfptr[6]),
			  (melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V9*/ meltfptr[8] =
	    /*_.FIRSTPROC__V8*/ meltfptr[7];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.FIRSTPAIR__V6*/ meltfptr[5] = NULL;
     /*_.FIRSTPROC__V7*/ meltfptr[6] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:725:/ clear");
	     /*clear *//*_.FIRSTPAIR__V6*/ meltfptr[5] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V8*/ meltfptr[7] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V9*/ meltfptr[8] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_list_first ((melt_ptr_t) /*_.LASTLIST__V3*/ meltfptr[2]);
	   melt_magic_discr ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]) ==
	   MELTOBMAG_PAIR;
	   /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_pair_tail ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]))
	{
	  /*_.LASTPROC__V11*/ meltfptr[10] =
	    melt_pair_head ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L1*/ meltfnum[0] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.LASTPROC__V11*/ meltfptr[10])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:734:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  /*^locexp */
		  meltgc_prepend_list ((melt_ptr_t)
				       ( /*_.REVLASTLIST__V4*/ meltfptr[3]),
				       (melt_ptr_t) ( /*_.LASTPROC__V11*/
						     meltfptr[10]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.LASTPAIR__V10*/ meltfptr[9] = NULL;
     /*_.LASTPROC__V11*/ meltfptr[10] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:731:/ clear");
	     /*clear *//*_.LASTPAIR__V10*/ meltfptr[9] = 0;
      /*^clear */
	     /*clear *//*_.LASTPROC__V11*/ meltfptr[10] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:736:/ cond");
    /*cond */ if ( /*_.LASTLIST__V3*/ meltfptr[2])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*citerblock FOREACH_IN_LIST */
	  {
	    /* start foreach_in_list meltcit3__EACHLIST */
	    for ( /*_.REVLASTPAIR__V12*/ meltfptr[11] =
		 melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V4*/
				  meltfptr[3]);
		 melt_magic_discr ((melt_ptr_t) /*_.REVLASTPAIR__V12*/
				   meltfptr[11]) == MELTOBMAG_PAIR;
		 /*_.REVLASTPAIR__V12*/ meltfptr[11] =
		 melt_pair_tail ((melt_ptr_t) /*_.REVLASTPAIR__V12*/
				 meltfptr[11]))
	      {
		/*_.REVLASTPROC__V13*/ meltfptr[12] =
		  melt_pair_head ((melt_ptr_t) /*_.REVLASTPAIR__V12*/
				  meltfptr[11]);



		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-hooks.melt:740:/ apply");
		/*apply */
		{
		  /*_.REVLASTPROC__V14*/ meltfptr[13] =
		    melt_apply ((meltclosure_ptr_t)
				( /*_.REVLASTPROC__V13*/ meltfptr[12]),
				(melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]),
				(""), (union meltparam_un *) 0, "",
				(union meltparam_un *) 0);
		}
		;
		/*^compute */
		/*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V15*/ meltfptr[14] =
		  /*_.REVLASTPROC__V14*/ meltfptr[13];;
	      }			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.REVLASTPAIR__V12*/ meltfptr[11] = NULL;
     /*_.REVLASTPROC__V13*/ meltfptr[12] = NULL;


	    /*citerepilog */

	    MELT_LOCATION ("warmelt-hooks.melt:737:/ clear");
	       /*clear *//*_.REVLASTPAIR__V12*/ meltfptr[11] = 0;
	    /*^clear */
	       /*clear *//*_.REVLASTPROC__V13*/ meltfptr[12] = 0;
	    /*^clear */
	       /*clear *//*_.REVLASTPROC__V14*/ meltfptr[13] = 0;
	    /*^clear */
	       /*clear *//*_.SETQ___V15*/ meltfptr[14] = 0;
	  }			/*endciterblock FOREACH_IN_LIST */
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:719:/ clear");
	    /*clear *//*_.FIRSTLIST__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.RES__V5*/ meltfptr[4] = 0;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_ALL_IPA_PASSES_START hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_ALL_IPA_PASSES_START hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_ALL_IPA_PASSES_START hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_ALL_IPA_PASSES_STARThook */
void
melthookmark_HOOK_ALL_IPA_PASSES_START_rout (struct melt_callframe_st
					     *meltcfram, int meltmarking)
{

  melthookframe_HOOK_ALL_IPA_PASSES_START_t *meltframptr_
    = (melthookframe_HOOK_ALL_IPA_PASSES_START_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_ALL_IPA_PASSES_START good hash",
		  meltframptr_->mcfr_nbvar == -1767);
  if (meltmarking)
    {				/* marking hook HOOK_ALL_IPA_PASSES_START */
      int meltix;
      for (meltix = 0; meltix < 15; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_ALL_IPA_PASSES_START */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 15; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_ALL_IPA_PASSES_START */
}				/* end  melthookmark_HOOK_ALL_IPA_PASSES_START_rout */

/* frame for melthook_HOOK_ALL_IPA_PASSES_END hook: */
typedef struct meltframe_melthook_HOOK_ALL_IPA_PASSES_END_st
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
#define MELTFRAM_NBVARPTR 15
  melt_ptr_t mcfr_varptr[15];
#define MELTFRAM_NBVARNUM 1
  long mcfr_varnum[1];
/*others*/
  long _spare_;
}
melthookframe_HOOK_ALL_IPA_PASSES_END_t;	/* end of hook frame melthook_HOOK_ALL_IPA_PASSES_END */


/** implementation of hook melthook_HOOK_ALL_IPA_PASSES_END */
void
melthook_HOOK_ALL_IPA_PASSES_END (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_ALL_IPA_PASSES_END from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_ALL_IPA_PASSES_END hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_ALL_IPA_PASSES_END;
  long melt_thishookcall_counter_HOOK_ALL_IPA_PASSES_END ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_ALL_IPA_PASSES_END++;
#define meltcallcount melt_thishookcall_counter_HOOK_ALL_IPA_PASSES_END
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_ALL_IPA_PASSES_END_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_ALL_IPA_PASSES_END */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -3137 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_ALL_IPA_PASSES_END_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_ALL_IPA_PASSES_END*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_ALL_IPA_PASSES_END hook */
  MELT_LOCATION ("warmelt-hooks.melt:841:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:843:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.FIRSTLIST__V2*/ meltfptr[1] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:844:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
      }
    ;
    /*^compute */
  /*_.REVLASTLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    /*^compute */
    /*_.RES__V5*/ meltfptr[4] = ( /*nil */ NULL);;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_list_first ((melt_ptr_t) /*_.FIRSTLIST__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]) ==
	   MELTOBMAG_PAIR;
	   /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_pair_tail ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]))
	{
	  /*_.FIRSTPROC__V7*/ meltfptr[6] =
	    melt_pair_head ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:852:/ apply");
	  /*apply */
	  {
	    /*_.FIRSTPROC__V8*/ meltfptr[7] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.FIRSTPROC__V7*/ meltfptr[6]),
			  (melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V9*/ meltfptr[8] =
	    /*_.FIRSTPROC__V8*/ meltfptr[7];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.FIRSTPAIR__V6*/ meltfptr[5] = NULL;
     /*_.FIRSTPROC__V7*/ meltfptr[6] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:849:/ clear");
	     /*clear *//*_.FIRSTPAIR__V6*/ meltfptr[5] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V8*/ meltfptr[7] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V9*/ meltfptr[8] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_list_first ((melt_ptr_t) /*_.LASTLIST__V3*/ meltfptr[2]);
	   melt_magic_discr ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]) ==
	   MELTOBMAG_PAIR;
	   /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_pair_tail ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]))
	{
	  /*_.LASTPROC__V11*/ meltfptr[10] =
	    melt_pair_head ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L1*/ meltfnum[0] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.LASTPROC__V11*/ meltfptr[10])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:858:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  /*^locexp */
		  meltgc_prepend_list ((melt_ptr_t)
				       ( /*_.REVLASTLIST__V4*/ meltfptr[3]),
				       (melt_ptr_t) ( /*_.LASTPROC__V11*/
						     meltfptr[10]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.LASTPAIR__V10*/ meltfptr[9] = NULL;
     /*_.LASTPROC__V11*/ meltfptr[10] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:855:/ clear");
	     /*clear *//*_.LASTPAIR__V10*/ meltfptr[9] = 0;
      /*^clear */
	     /*clear *//*_.LASTPROC__V11*/ meltfptr[10] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit3__EACHLIST */
      for ( /*_.REVLASTPAIR__V12*/ meltfptr[11] =
	   melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V4*/ meltfptr[3]);
	   melt_magic_discr ((melt_ptr_t) /*_.REVLASTPAIR__V12*/ meltfptr[11])
	   == MELTOBMAG_PAIR;
	   /*_.REVLASTPAIR__V12*/ meltfptr[11] =
	   melt_pair_tail ((melt_ptr_t) /*_.REVLASTPAIR__V12*/ meltfptr[11]))
	{
	  /*_.REVLASTPROC__V13*/ meltfptr[12] =
	    melt_pair_head ((melt_ptr_t) /*_.REVLASTPAIR__V12*/ meltfptr[11]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:863:/ apply");
	  /*apply */
	  {
	    /*_.REVLASTPROC__V14*/ meltfptr[13] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.REVLASTPROC__V13*/ meltfptr[12]),
			  (melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V15*/ meltfptr[14] =
	    /*_.REVLASTPROC__V14*/ meltfptr[13];;
	}			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.REVLASTPAIR__V12*/ meltfptr[11] = NULL;
     /*_.REVLASTPROC__V13*/ meltfptr[12] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:860:/ clear");
	     /*clear *//*_.REVLASTPAIR__V12*/ meltfptr[11] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V14*/ meltfptr[13] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V15*/ meltfptr[14] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:843:/ clear");
	    /*clear *//*_.FIRSTLIST__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.RES__V5*/ meltfptr[4] = 0;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_ALL_IPA_PASSES_END hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_ALL_IPA_PASSES_END hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_ALL_IPA_PASSES_END hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_ALL_IPA_PASSES_ENDhook */
void
melthookmark_HOOK_ALL_IPA_PASSES_END_rout (struct melt_callframe_st
					   *meltcfram, int meltmarking)
{

  melthookframe_HOOK_ALL_IPA_PASSES_END_t *meltframptr_
    = (melthookframe_HOOK_ALL_IPA_PASSES_END_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_ALL_IPA_PASSES_END good hash",
		  meltframptr_->mcfr_nbvar == -3137);
  if (meltmarking)
    {				/* marking hook HOOK_ALL_IPA_PASSES_END */
      int meltix;
      for (meltix = 0; meltix < 15; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_ALL_IPA_PASSES_END */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 15; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_ALL_IPA_PASSES_END */
}				/* end  melthookmark_HOOK_ALL_IPA_PASSES_END_rout */

/* frame for melthook_HOOK_EARLY_GIMPLE_PASSES_START hook: */
typedef struct meltframe_melthook_HOOK_EARLY_GIMPLE_PASSES_START_st
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
#define MELTFRAM_NBVARPTR 15
  melt_ptr_t mcfr_varptr[15];
#define MELTFRAM_NBVARNUM 1
  long mcfr_varnum[1];
/*others*/
  long _spare_;
}
melthookframe_HOOK_EARLY_GIMPLE_PASSES_START_t;	/* end of hook frame melthook_HOOK_EARLY_GIMPLE_PASSES_START */


/** implementation of hook melthook_HOOK_EARLY_GIMPLE_PASSES_START */
void
melthook_HOOK_EARLY_GIMPLE_PASSES_START (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_EARLY_GIMPLE_PASSES_START from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_EARLY_GIMPLE_PASSES_START hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_EARLY_GIMPLE_PASSES_START;
  long melt_thishookcall_counter_HOOK_EARLY_GIMPLE_PASSES_START
    ATTRIBUTE_UNUSED = melthook_call_counter_HOOK_EARLY_GIMPLE_PASSES_START++;
#define meltcallcount melt_thishookcall_counter_HOOK_EARLY_GIMPLE_PASSES_START
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_EARLY_GIMPLE_PASSES_START_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_EARLY_GIMPLE_PASSES_START */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -262 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_EARLY_GIMPLE_PASSES_START_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_EARLY_GIMPLE_PASSES_START*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_EARLY_GIMPLE_PASSES_START hook */
  MELT_LOCATION ("warmelt-hooks.melt:963:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:965:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE */
			   meltfhook->tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.FIRSTLIST__V2*/ meltfptr[1] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:966:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE */
			   meltfhook->tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
      }
    ;
    /*^compute */
  /*_.REVLASTLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    /*^compute */
    /*_.RES__V5*/ meltfptr[4] = ( /*nil */ NULL);;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_list_first ((melt_ptr_t) /*_.FIRSTLIST__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]) ==
	   MELTOBMAG_PAIR;
	   /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_pair_tail ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]))
	{
	  /*_.FIRSTPROC__V7*/ meltfptr[6] =
	    melt_pair_head ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:974:/ apply");
	  /*apply */
	  {
	    /*_.FIRSTPROC__V8*/ meltfptr[7] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.FIRSTPROC__V7*/ meltfptr[6]),
			  (melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V9*/ meltfptr[8] =
	    /*_.FIRSTPROC__V8*/ meltfptr[7];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.FIRSTPAIR__V6*/ meltfptr[5] = NULL;
     /*_.FIRSTPROC__V7*/ meltfptr[6] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:971:/ clear");
	     /*clear *//*_.FIRSTPAIR__V6*/ meltfptr[5] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V8*/ meltfptr[7] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V9*/ meltfptr[8] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_list_first ((melt_ptr_t) /*_.LASTLIST__V3*/ meltfptr[2]);
	   melt_magic_discr ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]) ==
	   MELTOBMAG_PAIR;
	   /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_pair_tail ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]))
	{
	  /*_.LASTPROC__V11*/ meltfptr[10] =
	    melt_pair_head ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L1*/ meltfnum[0] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.LASTPROC__V11*/ meltfptr[10])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:980:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  /*^locexp */
		  meltgc_prepend_list ((melt_ptr_t)
				       ( /*_.REVLASTLIST__V4*/ meltfptr[3]),
				       (melt_ptr_t) ( /*_.LASTPROC__V11*/
						     meltfptr[10]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.LASTPAIR__V10*/ meltfptr[9] = NULL;
     /*_.LASTPROC__V11*/ meltfptr[10] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:977:/ clear");
	     /*clear *//*_.LASTPAIR__V10*/ meltfptr[9] = 0;
      /*^clear */
	     /*clear *//*_.LASTPROC__V11*/ meltfptr[10] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:982:/ cond");
    /*cond */ if ( /*_.LASTLIST__V3*/ meltfptr[2])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*citerblock FOREACH_IN_LIST */
	  {
	    /* start foreach_in_list meltcit3__EACHLIST */
	    for ( /*_.REVLASTPAIR__V12*/ meltfptr[11] =
		 melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V4*/
				  meltfptr[3]);
		 melt_magic_discr ((melt_ptr_t) /*_.REVLASTPAIR__V12*/
				   meltfptr[11]) == MELTOBMAG_PAIR;
		 /*_.REVLASTPAIR__V12*/ meltfptr[11] =
		 melt_pair_tail ((melt_ptr_t) /*_.REVLASTPAIR__V12*/
				 meltfptr[11]))
	      {
		/*_.REVLASTPROC__V13*/ meltfptr[12] =
		  melt_pair_head ((melt_ptr_t) /*_.REVLASTPAIR__V12*/
				  meltfptr[11]);



		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-hooks.melt:986:/ apply");
		/*apply */
		{
		  /*_.REVLASTPROC__V14*/ meltfptr[13] =
		    melt_apply ((meltclosure_ptr_t)
				( /*_.REVLASTPROC__V13*/ meltfptr[12]),
				(melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]),
				(""), (union meltparam_un *) 0, "",
				(union meltparam_un *) 0);
		}
		;
		/*^compute */
		/*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V15*/ meltfptr[14] =
		  /*_.REVLASTPROC__V14*/ meltfptr[13];;
	      }			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.REVLASTPAIR__V12*/ meltfptr[11] = NULL;
     /*_.REVLASTPROC__V13*/ meltfptr[12] = NULL;


	    /*citerepilog */

	    MELT_LOCATION ("warmelt-hooks.melt:983:/ clear");
	       /*clear *//*_.REVLASTPAIR__V12*/ meltfptr[11] = 0;
	    /*^clear */
	       /*clear *//*_.REVLASTPROC__V13*/ meltfptr[12] = 0;
	    /*^clear */
	       /*clear *//*_.REVLASTPROC__V14*/ meltfptr[13] = 0;
	    /*^clear */
	       /*clear *//*_.SETQ___V15*/ meltfptr[14] = 0;
	  }			/*endciterblock FOREACH_IN_LIST */
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:965:/ clear");
	    /*clear *//*_.FIRSTLIST__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.RES__V5*/ meltfptr[4] = 0;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_EARLY_GIMPLE_PASSES_START hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_EARLY_GIMPLE_PASSES_START hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_EARLY_GIMPLE_PASSES_START hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_EARLY_GIMPLE_PASSES_STARThook */
void
melthookmark_HOOK_EARLY_GIMPLE_PASSES_START_rout (struct melt_callframe_st
						  *meltcfram, int meltmarking)
{

  melthookframe_HOOK_EARLY_GIMPLE_PASSES_START_t *meltframptr_
    = (melthookframe_HOOK_EARLY_GIMPLE_PASSES_START_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_EARLY_GIMPLE_PASSES_START good hash",
		  meltframptr_->mcfr_nbvar == -262);
  if (meltmarking)
    {				/* marking hook HOOK_EARLY_GIMPLE_PASSES_START */
      int meltix;
      for (meltix = 0; meltix < 15; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_EARLY_GIMPLE_PASSES_START */
  if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 15; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_EARLY_GIMPLE_PASSES_START */
}				/* end  melthookmark_HOOK_EARLY_GIMPLE_PASSES_START_rout */

/* frame for melthook_HOOK_EARLY_GIMPLE_PASSES_END hook: */
typedef struct meltframe_melthook_HOOK_EARLY_GIMPLE_PASSES_END_st
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
#define MELTFRAM_NBVARPTR 15
  melt_ptr_t mcfr_varptr[15];
#define MELTFRAM_NBVARNUM 1
  long mcfr_varnum[1];
/*others*/
  long _spare_;
}
melthookframe_HOOK_EARLY_GIMPLE_PASSES_END_t;	/* end of hook frame melthook_HOOK_EARLY_GIMPLE_PASSES_END */


/** implementation of hook melthook_HOOK_EARLY_GIMPLE_PASSES_END */
void
melthook_HOOK_EARLY_GIMPLE_PASSES_END (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_EARLY_GIMPLE_PASSES_END from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_EARLY_GIMPLE_PASSES_END hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_EARLY_GIMPLE_PASSES_END;
  long melt_thishookcall_counter_HOOK_EARLY_GIMPLE_PASSES_END ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_EARLY_GIMPLE_PASSES_END++;
#define meltcallcount melt_thishookcall_counter_HOOK_EARLY_GIMPLE_PASSES_END
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_EARLY_GIMPLE_PASSES_END_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_EARLY_GIMPLE_PASSES_END */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -2146 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_EARLY_GIMPLE_PASSES_END_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_EARLY_GIMPLE_PASSES_END*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_EARLY_GIMPLE_PASSES_END hook */
  MELT_LOCATION ("warmelt-hooks.melt:1087:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1089:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE */
			   meltfhook->tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.FIRSTLIST__V2*/ meltfptr[1] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1090:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE */ meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE */
			   meltfhook->tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
      }
    ;
    /*^compute */
  /*_.REVLASTLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    /*^compute */
    /*_.RES__V5*/ meltfptr[4] = ( /*nil */ NULL);;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_list_first ((melt_ptr_t) /*_.FIRSTLIST__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]) ==
	   MELTOBMAG_PAIR;
	   /*_.FIRSTPAIR__V6*/ meltfptr[5] =
	   melt_pair_tail ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]))
	{
	  /*_.FIRSTPROC__V7*/ meltfptr[6] =
	    melt_pair_head ((melt_ptr_t) /*_.FIRSTPAIR__V6*/ meltfptr[5]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1098:/ apply");
	  /*apply */
	  {
	    /*_.FIRSTPROC__V8*/ meltfptr[7] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.FIRSTPROC__V7*/ meltfptr[6]),
			  (melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V9*/ meltfptr[8] =
	    /*_.FIRSTPROC__V8*/ meltfptr[7];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.FIRSTPAIR__V6*/ meltfptr[5] = NULL;
     /*_.FIRSTPROC__V7*/ meltfptr[6] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1095:/ clear");
	     /*clear *//*_.FIRSTPAIR__V6*/ meltfptr[5] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V8*/ meltfptr[7] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V9*/ meltfptr[8] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_list_first ((melt_ptr_t) /*_.LASTLIST__V3*/ meltfptr[2]);
	   melt_magic_discr ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]) ==
	   MELTOBMAG_PAIR;
	   /*_.LASTPAIR__V10*/ meltfptr[9] =
	   melt_pair_tail ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]))
	{
	  /*_.LASTPROC__V11*/ meltfptr[10] =
	    melt_pair_head ((melt_ptr_t) /*_.LASTPAIR__V10*/ meltfptr[9]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L1*/ meltfnum[0] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.LASTPROC__V11*/ meltfptr[10])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1104:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  /*^locexp */
		  meltgc_prepend_list ((melt_ptr_t)
				       ( /*_.REVLASTLIST__V4*/ meltfptr[3]),
				       (melt_ptr_t) ( /*_.LASTPROC__V11*/
						     meltfptr[10]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.LASTPAIR__V10*/ meltfptr[9] = NULL;
     /*_.LASTPROC__V11*/ meltfptr[10] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1101:/ clear");
	     /*clear *//*_.LASTPAIR__V10*/ meltfptr[9] = 0;
      /*^clear */
	     /*clear *//*_.LASTPROC__V11*/ meltfptr[10] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit3__EACHLIST */
      for ( /*_.REVLASTPAIR__V12*/ meltfptr[11] =
	   melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V4*/ meltfptr[3]);
	   melt_magic_discr ((melt_ptr_t) /*_.REVLASTPAIR__V12*/ meltfptr[11])
	   == MELTOBMAG_PAIR;
	   /*_.REVLASTPAIR__V12*/ meltfptr[11] =
	   melt_pair_tail ((melt_ptr_t) /*_.REVLASTPAIR__V12*/ meltfptr[11]))
	{
	  /*_.REVLASTPROC__V13*/ meltfptr[12] =
	    melt_pair_head ((melt_ptr_t) /*_.REVLASTPAIR__V12*/ meltfptr[11]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1109:/ apply");
	  /*apply */
	  {
	    /*_.REVLASTPROC__V14*/ meltfptr[13] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.REVLASTPROC__V13*/ meltfptr[12]),
			  (melt_ptr_t) ( /*_.RES__V5*/ meltfptr[4]), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V15*/ meltfptr[14] =
	    /*_.REVLASTPROC__V14*/ meltfptr[13];;
	}			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.REVLASTPAIR__V12*/ meltfptr[11] = NULL;
     /*_.REVLASTPROC__V13*/ meltfptr[12] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1106:/ clear");
	     /*clear *//*_.REVLASTPAIR__V12*/ meltfptr[11] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V14*/ meltfptr[13] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V15*/ meltfptr[14] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:1089:/ clear");
	    /*clear *//*_.FIRSTLIST__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.RES__V5*/ meltfptr[4] = 0;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_EARLY_GIMPLE_PASSES_END hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_EARLY_GIMPLE_PASSES_END hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_EARLY_GIMPLE_PASSES_END hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_EARLY_GIMPLE_PASSES_ENDhook */
void
melthookmark_HOOK_EARLY_GIMPLE_PASSES_END_rout (struct melt_callframe_st
						*meltcfram, int meltmarking)
{

  melthookframe_HOOK_EARLY_GIMPLE_PASSES_END_t *meltframptr_
    = (melthookframe_HOOK_EARLY_GIMPLE_PASSES_END_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_EARLY_GIMPLE_PASSES_END good hash",
		  meltframptr_->mcfr_nbvar == -2146);
  if (meltmarking)
    {				/* marking hook HOOK_EARLY_GIMPLE_PASSES_END */
      int meltix;
      for (meltix = 0; meltix < 15; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_EARLY_GIMPLE_PASSES_END */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 15; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_EARLY_GIMPLE_PASSES_END */
}				/* end  melthookmark_HOOK_EARLY_GIMPLE_PASSES_END_rout */

/* frame for melthook_HOOK_PRE_GENERICIZE hook: */
typedef struct meltframe_melthook_HOOK_PRE_GENERICIZE_st
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
#define MELTFRAM_NBVARPTR 16
  melt_ptr_t mcfr_varptr[16];
#define MELTFRAM_NBVARNUM 1
  long mcfr_varnum[1];
/*others*/
  tree loc_TREE__o0;
  long _spare_;
}
melthookframe_HOOK_PRE_GENERICIZE_t;	/* end of hook frame melthook_HOOK_PRE_GENERICIZE */


/** implementation of hook melthook_HOOK_PRE_GENERICIZE */
void
melthook_HOOK_PRE_GENERICIZE (melt_ptr_t melthookdata, tree meltinp0_TFNDECL)
{				/* body of hook melthook_HOOK_PRE_GENERICIZE from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_PRE_GENERICIZE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_PRE_GENERICIZE;
  long melt_thishookcall_counter_HOOK_PRE_GENERICIZE ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_PRE_GENERICIZE++;
#define meltcallcount melt_thishookcall_counter_HOOK_PRE_GENERICIZE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_PRE_GENERICIZE_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_PRE_GENERICIZE */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -2017 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_PRE_GENERICIZE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_PRE_GENERICIZE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_PRE_GENERICIZE hook */
  MELT_LOCATION ("warmelt-hooks.melt:1237:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^compute */
  /*_?*/ meltfram__.loc_TREE__o0 = /*gethookinput */ meltinp0_TFNDECL;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1239:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~PREGENERICIZE_DELAYED_QUEUE */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~PREGENERICIZE_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.FIRSTLIST__V2*/ meltfptr[1] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1240:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~PREGENERICIZE_DELAYED_QUEUE */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~PREGENERICIZE_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
      }
    ;
    /*^compute */
  /*_.REVLASTLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    /*^compute */
    /*_.RES__V5*/ meltfptr[4] = ( /*nil */ NULL);;
    MELT_LOCATION ("warmelt-hooks.melt:1244:/ quasiblock");


  /*_.CFNDECLV__V6*/ meltfptr[5] =
      /*full constboxing */
      meltgc_new_tree ((meltobject_ptr_t)
		       (((melt_ptr_t) (MELT_PREDEF (DISCR_CONSTANT_TREE)))),
		       ( /*_?*/ meltfram__.loc_TREE__o0));;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.FIRSTPAIR__V7*/ meltfptr[6] =
	   melt_list_first ((melt_ptr_t) /*_.FIRSTLIST__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]) ==
	   MELTOBMAG_PAIR;
	   /*_.FIRSTPAIR__V7*/ meltfptr[6] =
	   melt_pair_tail ((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]))
	{
	  /*_.FIRSTPROC__V8*/ meltfptr[7] =
	    melt_pair_head ((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1250:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.RES__V5*/ meltfptr[4];
	    /*_.FIRSTPROC__V9*/ meltfptr[8] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.FIRSTPROC__V8*/ meltfptr[7]),
			  (melt_ptr_t) ( /*_.CFNDECLV__V6*/ meltfptr[5]),
			  (MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V10*/ meltfptr[9] =
	    /*_.FIRSTPROC__V9*/ meltfptr[8];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.FIRSTPAIR__V7*/ meltfptr[6] = NULL;
     /*_.FIRSTPROC__V8*/ meltfptr[7] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1247:/ clear");
	     /*clear *//*_.FIRSTPAIR__V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V8*/ meltfptr[7] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V9*/ meltfptr[8] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V10*/ meltfptr[9] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.LASTPAIR__V11*/ meltfptr[10] =
	   melt_list_first ((melt_ptr_t) /*_.LASTLIST__V3*/ meltfptr[2]);
	   melt_magic_discr ((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[10]) ==
	   MELTOBMAG_PAIR;
	   /*_.LASTPAIR__V11*/ meltfptr[10] =
	   melt_pair_tail ((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[10]))
	{
	  /*_.LASTPROC__V12*/ meltfptr[11] =
	    melt_pair_head ((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[10]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L1*/ meltfnum[0] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.LASTPROC__V12*/ meltfptr[11])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1256:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  /*^locexp */
		  meltgc_prepend_list ((melt_ptr_t)
				       ( /*_.REVLASTLIST__V4*/ meltfptr[3]),
				       (melt_ptr_t) ( /*_.LASTPROC__V12*/
						     meltfptr[11]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.LASTPAIR__V11*/ meltfptr[10] = NULL;
     /*_.LASTPROC__V12*/ meltfptr[11] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1253:/ clear");
	     /*clear *//*_.LASTPAIR__V11*/ meltfptr[10] = 0;
      /*^clear */
	     /*clear *//*_.LASTPROC__V12*/ meltfptr[11] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit3__EACHLIST */
      for ( /*_.REVLASTPAIR__V13*/ meltfptr[12] =
	   melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V4*/ meltfptr[3]);
	   melt_magic_discr ((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12])
	   == MELTOBMAG_PAIR;
	   /*_.REVLASTPAIR__V13*/ meltfptr[12] =
	   melt_pair_tail ((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]))
	{
	  /*_.REVLASTPROC__V14*/ meltfptr[13] =
	    melt_pair_head ((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1261:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.RES__V5*/ meltfptr[4];
	    /*_.REVLASTPROC__V15*/ meltfptr[14] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.REVLASTPROC__V14*/ meltfptr[13]),
			  (melt_ptr_t) ( /*_.CFNDECLV__V6*/ meltfptr[5]),
			  (MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V16*/ meltfptr[15] =
	    /*_.REVLASTPROC__V15*/ meltfptr[14];;
	}			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.REVLASTPAIR__V13*/ meltfptr[12] = NULL;
     /*_.REVLASTPROC__V14*/ meltfptr[13] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1258:/ clear");
	     /*clear *//*_.REVLASTPAIR__V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V14*/ meltfptr[13] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V15*/ meltfptr[14] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V16*/ meltfptr[15] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:1244:/ clear");
	    /*clear *//*_.CFNDECLV__V6*/ meltfptr[5] = 0;

    MELT_LOCATION ("warmelt-hooks.melt:1239:/ clear");
	    /*clear *//*_.FIRSTLIST__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.RES__V5*/ meltfptr[4] = 0;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_PRE_GENERICIZE hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_PRE_GENERICIZE hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_PRE_GENERICIZE hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_PRE_GENERICIZEhook */
void
melthookmark_HOOK_PRE_GENERICIZE_rout (struct melt_callframe_st *meltcfram,
				       int meltmarking)
{

  melthookframe_HOOK_PRE_GENERICIZE_t *meltframptr_
    = (melthookframe_HOOK_PRE_GENERICIZE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_PRE_GENERICIZE good hash",
		  meltframptr_->mcfr_nbvar == -2017);
  if (meltmarking)
    {				/* marking hook HOOK_PRE_GENERICIZE */
      int meltix;
      for (meltix = 0; meltix < 16; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      if (meltframptr_->loc_TREE__o0)
	gt_ggc_mx_tree_node (meltframptr_->loc_TREE__o0);
    }
  else /* copying hook HOOK_PRE_GENERICIZE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 16; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_PRE_GENERICIZE */
}				/* end  melthookmark_HOOK_PRE_GENERICIZE_rout */

/* frame for melthook_HOOK_FINISH_TYPE hook: */
typedef struct meltframe_melthook_HOOK_FINISH_TYPE_st
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
#define MELTFRAM_NBVARPTR 16
  melt_ptr_t mcfr_varptr[16];
#define MELTFRAM_NBVARNUM 1
  long mcfr_varnum[1];
/*others*/
  tree loc_TREE__o0;
  long _spare_;
}
melthookframe_HOOK_FINISH_TYPE_t;	/* end of hook frame melthook_HOOK_FINISH_TYPE */


/** implementation of hook melthook_HOOK_FINISH_TYPE */
void
melthook_HOOK_FINISH_TYPE (melt_ptr_t melthookdata, tree meltinp0_TFNDECL)
{				/* body of hook melthook_HOOK_FINISH_TYPE from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_FINISH_TYPE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_FINISH_TYPE;
  long melt_thishookcall_counter_HOOK_FINISH_TYPE ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_FINISH_TYPE++;
#define meltcallcount melt_thishookcall_counter_HOOK_FINISH_TYPE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_FINISH_TYPE_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_FINISH_TYPE */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -1459 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_FINISH_TYPE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_FINISH_TYPE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_FINISH_TYPE hook */
  MELT_LOCATION ("warmelt-hooks.melt:1368:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^compute */
  /*_?*/ meltfram__.loc_TREE__o0 = /*gethookinput */ meltinp0_TFNDECL;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1370:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~FINISHTYPE_DELAYED_QUEUE */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~FINISHTYPE_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.FIRSTLIST__V2*/ meltfptr[1] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1371:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~FINISHTYPE_DELAYED_QUEUE */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~FINISHTYPE_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
      }
    ;
    /*^compute */
  /*_.REVLASTLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    /*^compute */
    /*_.RES__V5*/ meltfptr[4] = ( /*nil */ NULL);;
    MELT_LOCATION ("warmelt-hooks.melt:1375:/ quasiblock");


  /*_.CFNDECLV__V6*/ meltfptr[5] =
      /*full constboxing */
      meltgc_new_tree ((meltobject_ptr_t)
		       (((melt_ptr_t) (MELT_PREDEF (DISCR_CONSTANT_TREE)))),
		       ( /*_?*/ meltfram__.loc_TREE__o0));;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.FIRSTPAIR__V7*/ meltfptr[6] =
	   melt_list_first ((melt_ptr_t) /*_.FIRSTLIST__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]) ==
	   MELTOBMAG_PAIR;
	   /*_.FIRSTPAIR__V7*/ meltfptr[6] =
	   melt_pair_tail ((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]))
	{
	  /*_.FIRSTPROC__V8*/ meltfptr[7] =
	    melt_pair_head ((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1381:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.RES__V5*/ meltfptr[4];
	    /*_.FIRSTPROC__V9*/ meltfptr[8] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.FIRSTPROC__V8*/ meltfptr[7]),
			  (melt_ptr_t) ( /*_.CFNDECLV__V6*/ meltfptr[5]),
			  (MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V10*/ meltfptr[9] =
	    /*_.FIRSTPROC__V9*/ meltfptr[8];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.FIRSTPAIR__V7*/ meltfptr[6] = NULL;
     /*_.FIRSTPROC__V8*/ meltfptr[7] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1378:/ clear");
	     /*clear *//*_.FIRSTPAIR__V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V8*/ meltfptr[7] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V9*/ meltfptr[8] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V10*/ meltfptr[9] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.LASTPAIR__V11*/ meltfptr[10] =
	   melt_list_first ((melt_ptr_t) /*_.LASTLIST__V3*/ meltfptr[2]);
	   melt_magic_discr ((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[10]) ==
	   MELTOBMAG_PAIR;
	   /*_.LASTPAIR__V11*/ meltfptr[10] =
	   melt_pair_tail ((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[10]))
	{
	  /*_.LASTPROC__V12*/ meltfptr[11] =
	    melt_pair_head ((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[10]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L1*/ meltfnum[0] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.LASTPROC__V12*/ meltfptr[11])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1387:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  /*^locexp */
		  meltgc_prepend_list ((melt_ptr_t)
				       ( /*_.REVLASTLIST__V4*/ meltfptr[3]),
				       (melt_ptr_t) ( /*_.LASTPROC__V12*/
						     meltfptr[11]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.LASTPAIR__V11*/ meltfptr[10] = NULL;
     /*_.LASTPROC__V12*/ meltfptr[11] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1384:/ clear");
	     /*clear *//*_.LASTPAIR__V11*/ meltfptr[10] = 0;
      /*^clear */
	     /*clear *//*_.LASTPROC__V12*/ meltfptr[11] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit3__EACHLIST */
      for ( /*_.REVLASTPAIR__V13*/ meltfptr[12] =
	   melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V4*/ meltfptr[3]);
	   melt_magic_discr ((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12])
	   == MELTOBMAG_PAIR;
	   /*_.REVLASTPAIR__V13*/ meltfptr[12] =
	   melt_pair_tail ((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]))
	{
	  /*_.REVLASTPROC__V14*/ meltfptr[13] =
	    melt_pair_head ((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1392:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.RES__V5*/ meltfptr[4];
	    /*_.REVLASTPROC__V15*/ meltfptr[14] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.REVLASTPROC__V14*/ meltfptr[13]),
			  (melt_ptr_t) ( /*_.CFNDECLV__V6*/ meltfptr[5]),
			  (MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V5*/ meltfptr[4] = /*_.SETQ___V16*/ meltfptr[15] =
	    /*_.REVLASTPROC__V15*/ meltfptr[14];;
	}			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.REVLASTPAIR__V13*/ meltfptr[12] = NULL;
     /*_.REVLASTPROC__V14*/ meltfptr[13] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1389:/ clear");
	     /*clear *//*_.REVLASTPAIR__V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V14*/ meltfptr[13] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V15*/ meltfptr[14] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V16*/ meltfptr[15] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:1375:/ clear");
	    /*clear *//*_.CFNDECLV__V6*/ meltfptr[5] = 0;

    MELT_LOCATION ("warmelt-hooks.melt:1370:/ clear");
	    /*clear *//*_.FIRSTLIST__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.RES__V5*/ meltfptr[4] = 0;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_FINISH_TYPE hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_FINISH_TYPE hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_FINISH_TYPE hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_FINISH_TYPEhook */
void
melthookmark_HOOK_FINISH_TYPE_rout (struct melt_callframe_st *meltcfram,
				    int meltmarking)
{

  melthookframe_HOOK_FINISH_TYPE_t *meltframptr_
    = (melthookframe_HOOK_FINISH_TYPE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_FINISH_TYPE good hash",
		  meltframptr_->mcfr_nbvar == -1459);
  if (meltmarking)
    {				/* marking hook HOOK_FINISH_TYPE */
      int meltix;
      for (meltix = 0; meltix < 16; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      if (meltframptr_->loc_TREE__o0)
	gt_ggc_mx_tree_node (meltframptr_->loc_TREE__o0);
    }
  else /* copying hook HOOK_FINISH_TYPE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 16; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_FINISH_TYPE */
}				/* end  melthookmark_HOOK_FINISH_TYPE_rout */

/* frame for melthook_HOOK_PASS_EXECUTION hook: */
typedef struct meltframe_melthook_HOOK_PASS_EXECUTION_st
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
#define MELTFRAM_NBVARPTR 16
  melt_ptr_t mcfr_varptr[16];
#define MELTFRAM_NBVARNUM 2
  long mcfr_varnum[2];
/*others*/
  const char *loc_CSTRING__o0;
  long _spare_;
}
melthookframe_HOOK_PASS_EXECUTION_t;	/* end of hook frame melthook_HOOK_PASS_EXECUTION */


/** implementation of hook melthook_HOOK_PASS_EXECUTION */
void
melthook_HOOK_PASS_EXECUTION (melt_ptr_t melthookdata,
			      const char *meltinp0_PASSNAME,
			      long meltinp1_PASSNUM)
{				/* body of hook melthook_HOOK_PASS_EXECUTION from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_PASS_EXECUTION hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_PASS_EXECUTION;
  long melt_thishookcall_counter_HOOK_PASS_EXECUTION ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_PASS_EXECUTION++;
#define meltcallcount melt_thishookcall_counter_HOOK_PASS_EXECUTION
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_PASS_EXECUTION_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_PASS_EXECUTION */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -1587 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_PASS_EXECUTION_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_PASS_EXECUTION*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_PASS_EXECUTION hook */
  MELT_LOCATION ("warmelt-hooks.melt:1494:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^compute */
  /*_?*/ meltfram__.loc_CSTRING__o0 = /*gethookinput */ meltinp0_PASSNAME;;
  /*^compute */
  /*_#PASSNUM__L1*/ meltfnum[0] = /*gethookinput */ meltinp1_PASSNUM;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1496:/ quasiblock");


  /*_.PASSNAMEVAL__V2*/ meltfptr[1] =
      /*hook_pass_execution MAKEPASSNAME_CHK__1 */
      meltgc_new_stringdup ((meltobject_ptr_t) MELT_PREDEF (DISCR_STRING),
		       /*_?*/ meltfram__.loc_CSTRING__o0)
      ;;
    MELT_LOCATION ("warmelt-hooks.melt:1505:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~PASS_EXECUTION_DELAYED_QUEUE */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~PASS_EXECUTION_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
    /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.FIRSTLIST__V3*/ meltfptr[2] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1506:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~PASS_EXECUTION_DELAYED_QUEUE */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_DELAYED_QUEUE */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~PASS_EXECUTION_DELAYED_QUEUE */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "DELQU_LAST");
    /*_.LASTLIST__V4*/ meltfptr[3] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.LASTLIST__V4*/ meltfptr[3] = NULL;;
      }
    ;
    /*^compute */
  /*_.REVLASTLIST__V5*/ meltfptr[4] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*~DISCR_LIST */ meltfhook->tabval[2]))));;
    /*^compute */
    /*_.RES__V6*/ meltfptr[5] = ( /*nil */ NULL);;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.FIRSTPAIR__V7*/ meltfptr[6] =
	   melt_list_first ((melt_ptr_t) /*_.FIRSTLIST__V3*/ meltfptr[2]);
	   melt_magic_discr ((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]) ==
	   MELTOBMAG_PAIR;
	   /*_.FIRSTPAIR__V7*/ meltfptr[6] =
	   melt_pair_tail ((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]))
	{
	  /*_.FIRSTPROC__V8*/ meltfptr[7] =
	    melt_pair_head ((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1514:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[2];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_long = /*_#PASSNUM__L1*/ meltfnum[0];
	    /*^apply.arg */
	    argtab[1].meltbp_aptr =
	      (melt_ptr_t *) & /*_.RES__V6*/ meltfptr[5];
	    /*_.FIRSTPROC__V9*/ meltfptr[8] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.FIRSTPROC__V8*/ meltfptr[7]),
			  (melt_ptr_t) ( /*_.PASSNAMEVAL__V2*/ meltfptr[1]),
			  (MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V6*/ meltfptr[5] = /*_.SETQ___V10*/ meltfptr[9] =
	    /*_.FIRSTPROC__V9*/ meltfptr[8];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.FIRSTPAIR__V7*/ meltfptr[6] = NULL;
     /*_.FIRSTPROC__V8*/ meltfptr[7] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1511:/ clear");
	     /*clear *//*_.FIRSTPAIR__V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V8*/ meltfptr[7] = 0;
      /*^clear */
	     /*clear *//*_.FIRSTPROC__V9*/ meltfptr[8] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V10*/ meltfptr[9] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.LASTPAIR__V11*/ meltfptr[10] =
	   melt_list_first ((melt_ptr_t) /*_.LASTLIST__V4*/ meltfptr[3]);
	   melt_magic_discr ((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[10]) ==
	   MELTOBMAG_PAIR;
	   /*_.LASTPAIR__V11*/ meltfptr[10] =
	   melt_pair_tail ((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[10]))
	{
	  /*_.LASTPROC__V12*/ meltfptr[11] =
	    melt_pair_head ((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[10]);



	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L2*/ meltfnum[1] =
	    (melt_magic_discr
	     ((melt_ptr_t) ( /*_.LASTPROC__V12*/ meltfptr[11])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1520:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  /*^locexp */
		  meltgc_prepend_list ((melt_ptr_t)
				       ( /*_.REVLASTLIST__V5*/ meltfptr[4]),
				       (melt_ptr_t) ( /*_.LASTPROC__V12*/
						     meltfptr[11]));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.LASTPAIR__V11*/ meltfptr[10] = NULL;
     /*_.LASTPROC__V12*/ meltfptr[11] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1517:/ clear");
	     /*clear *//*_.LASTPAIR__V11*/ meltfptr[10] = 0;
      /*^clear */
	     /*clear *//*_.LASTPROC__V12*/ meltfptr[11] = 0;
      /*^clear */
	     /*clear *//*_#IS_CLOSURE__L2*/ meltfnum[1] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit3__EACHLIST */
      for ( /*_.REVLASTPAIR__V13*/ meltfptr[12] =
	   melt_list_first ((melt_ptr_t) /*_.REVLASTLIST__V5*/ meltfptr[4]);
	   melt_magic_discr ((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12])
	   == MELTOBMAG_PAIR;
	   /*_.REVLASTPAIR__V13*/ meltfptr[12] =
	   melt_pair_tail ((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]))
	{
	  /*_.REVLASTPROC__V14*/ meltfptr[13] =
	    melt_pair_head ((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]);



	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1525:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[2];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_long = /*_#PASSNUM__L1*/ meltfnum[0];
	    /*^apply.arg */
	    argtab[1].meltbp_aptr =
	      (melt_ptr_t *) & /*_.RES__V6*/ meltfptr[5];
	    /*_.REVLASTPROC__V15*/ meltfptr[14] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.REVLASTPROC__V14*/ meltfptr[13]),
			  (melt_ptr_t) ( /*_.PASSNAMEVAL__V2*/ meltfptr[1]),
			  (MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*^compute */
	  /*_.RES__V6*/ meltfptr[5] = /*_.SETQ___V16*/ meltfptr[15] =
	    /*_.REVLASTPROC__V15*/ meltfptr[14];;
	}			/* end foreach_in_list meltcit3__EACHLIST */
     /*_.REVLASTPAIR__V13*/ meltfptr[12] = NULL;
     /*_.REVLASTPROC__V14*/ meltfptr[13] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-hooks.melt:1522:/ clear");
	     /*clear *//*_.REVLASTPAIR__V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V14*/ meltfptr[13] = 0;
      /*^clear */
	     /*clear *//*_.REVLASTPROC__V15*/ meltfptr[14] = 0;
      /*^clear */
	     /*clear *//*_.SETQ___V16*/ meltfptr[15] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:1496:/ clear");
	    /*clear *//*_.PASSNAMEVAL__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.FIRSTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_.LASTLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	    /*clear *//*_.REVLASTLIST__V5*/ meltfptr[4] = 0;
    /*^clear */
	    /*clear *//*_.RES__V6*/ meltfptr[5] = 0;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1530:/ locexp");
      melt_garbcoll ((1024), MELT_MINOR_OR_FULL);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */
  }
  ;
  /* epilogue of melthook_HOOK_PASS_EXECUTION hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_PASS_EXECUTION hook */
  melt_topframe = meltfram__.mcfr_prev;
  return;
}				/* end of melthook_HOOK_PASS_EXECUTION hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_PASS_EXECUTIONhook */
void
melthookmark_HOOK_PASS_EXECUTION_rout (struct melt_callframe_st *meltcfram,
				       int meltmarking)
{

  melthookframe_HOOK_PASS_EXECUTION_t *meltframptr_
    = (melthookframe_HOOK_PASS_EXECUTION_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_PASS_EXECUTION good hash",
		  meltframptr_->mcfr_nbvar == -1587);
  if (meltmarking)
    {				/* marking hook HOOK_PASS_EXECUTION */
      int meltix;
      for (meltix = 0; meltix < 16; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_PASS_EXECUTION */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 16; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_PASS_EXECUTION */
}				/* end  melthookmark_HOOK_PASS_EXECUTION_rout */

/* frame for melthook_HOOK_GIMPLE_GATE hook: */
typedef struct meltframe_melthook_HOOK_GIMPLE_GATE_st
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
#define MELTFRAM_NBVARPTR 13
  melt_ptr_t mcfr_varptr[13];
#define MELTFRAM_NBVARNUM 20
  long mcfr_varnum[20];
/*others*/
  const char *loc_CSTRING__o0;
  long _spare_;
}
melthookframe_HOOK_GIMPLE_GATE_t;	/* end of hook frame melthook_HOOK_GIMPLE_GATE */


/** implementation of hook melthook_HOOK_GIMPLE_GATE */
long
melthook_HOOK_GIMPLE_GATE (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_GIMPLE_GATE from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_GIMPLE_GATE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_GIMPLE_GATE;
  long melt_thishookcall_counter_HOOK_GIMPLE_GATE ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_GIMPLE_GATE++;
#define meltcallcount melt_thishookcall_counter_HOOK_GIMPLE_GATE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_GIMPLE_GATE_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_GIMPLE_GATE */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -1770 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_GIMPLE_GATE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_GIMPLE_GATE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_GIMPLE_GATE hook */
  MELT_LOCATION ("warmelt-hooks.melt:1579:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1582:/ quasiblock");


  /*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;;
    /*^compute */
  /*_#OKRES__L4*/ meltfnum[3] = 0;;
    MELT_LOCATION ("warmelt-hooks.melt:1584:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~INITIAL_SYSTEM_DATA */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_SYSTEM_DATA */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~INITIAL_SYSTEM_DATA */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 9, "SYSDATA_PASS_DICT");
    /*_.PASSDICT__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.PASSDICT__V2*/ meltfptr[1] = NULL;;
      }
    ;
    /*^compute */
    /*_.MYPASS__V3*/ meltfptr[2] = ( /*nil */ NULL);;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1587:/ locexp");
      /* hook_gimple_gate GETHASMODE_CHK__1 */
      static const char *GETHASMODE_CHK__1modstr;
      if (!GETHASMODE_CHK__1modstr)
	GETHASMODE_CHK__1modstr = melt_argument ("mode");
		/*_#HASMODEFLAG__L3*/ meltfnum[2] = GETHASMODE_CHK__1modstr != NULL
	&& GETHASMODE_CHK__1modstr[0];
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#NOT__L5*/ meltfnum[4] =
      (!( /*_#HASMODEFLAG__L3*/ meltfnum[2]));;
    MELT_LOCATION ("warmelt-hooks.melt:1594:/ cond");
    /*cond */ if ( /*_#NOT__L5*/ meltfnum[4])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1595:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = 0;;
	  MELT_LOCATION ("warmelt-hooks.melt:1595:/ finalreturn");
	  ;
	  /*finalret */ goto meltlabend_rout;
	  /*_#IF___L6*/ meltfnum[5] = /*_#RETURN___L7*/ meltfnum[6];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:1594:/ clear");
	      /*clear *//*_#RETURN___L7*/ meltfnum[6] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

   /*_#IF___L6*/ meltfnum[5] = 0;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1596:/ locexp");
      /* hook_gimple_gate GETMYPASS_CHK__1 */
      gcc_assert (current_pass != NULL);
      gcc_assert (current_pass->name != NULL);
      gcc_assert (current_pass->type == GIMPLE_PASS);
      /*_.MYPASS__V3*/ meltfptr[2] =
	melt_get_mapstrings ((struct meltmapstrings_st *) /*_.PASSDICT__V2*/
			     meltfptr[1],
			     current_pass->name);
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#IS_A__L8*/ meltfnum[6] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]),
			   (melt_ptr_t) (( /*~CLASS_GCC_GIMPLE_PASS */
					  meltfhook->tabval[2])));;
    MELT_LOCATION ("warmelt-hooks.melt:1605:/ cond");
    /*cond */ if ( /*_#IS_A__L8*/ meltfnum[6])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:1606:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.MYPASS__V3*/ meltfptr[2]),
					      (melt_ptr_t) (( /*~CLASS_GCC_PASS */ meltfhook->tabval[3])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj = (melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 2, "GCCPASS_GATE");
      /*_.GATEFUN__V4*/ meltfptr[3] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

     /*_.GATEFUN__V4*/ meltfptr[3] = NULL;;
	    }
	  ;
	  /*^compute */
    /*_?*/ meltfram__.loc_CSTRING__o0 =
	    (char *) 0;;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
    /*_#IS_CLOSURE__L11*/ meltfnum[10] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.GATEFUN__V4*/ meltfptr[3]))
	     == MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1609:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L11*/ meltfnum[10])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {




		{
		  MELT_LOCATION ("warmelt-hooks.melt:1610:/ locexp");
		  /* hook_gimple_gate DOGATEFUN_CHK__1 */
		  FILE *oldf = meltgc_set_dump_file (dump_file);
	       /*_?*/ meltfram__.loc_CSTRING__o0 = current_pass->name;
		  /*^block */
		  /*anyblock */
		  {


#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1616:/ cppif.then");
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
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1616:/ cond");
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
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1616:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[7];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L14*/ meltfnum[13];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1616;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_gimple_gate curpassname=";
			      /*^apply.arg */
			      argtab[4].meltbp_cstring =
				/*_?*/ meltfram__.loc_CSTRING__o0;
			      /*^apply.arg */
			      argtab[5].meltbp_cstring = " mypass=";
			      /*^apply.arg */
			      argtab[6].meltbp_aptr =
				(melt_ptr_t *) & /*_.MYPASS__V3*/ meltfptr[2];
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_PTR ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V6*/ meltfptr[5] =
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1616:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L14*/ meltfnum[13] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V6*/ meltfptr[5] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1616:/ quasiblock");


		      /*_.PROGN___V8*/ meltfptr[6] =
			/*_.IF___V6*/ meltfptr[5];;
		      /*^compute */
		      /*_.IFCPP___V5*/ meltfptr[4] =
			/*_.PROGN___V8*/ meltfptr[6];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1616:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L13*/ meltfnum[12] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V6*/ meltfptr[5] = 0;
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

		    MELT_CHECK_SIGNAL ();
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:1617:/ apply");
		    /*apply */
		    {
		      /*_.GATEFUN__V9*/ meltfptr[5] =
			melt_apply ((meltclosure_ptr_t)
				    ( /*_.GATEFUN__V4*/ meltfptr[3]),
				    (melt_ptr_t) ( /*_.MYPASS__V3*/
						  meltfptr[2]), (""),
				    (union meltparam_un *) 0, "",
				    (union meltparam_un *) 0);
		    }
		    ;
		    /*^cond */
		    /*cond */ if ( /*_.GATEFUN__V9*/ meltfptr[5])	/*then */
		      {
			/*^cond.then */
			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-hooks.melt:1618:/ compute");
			  /*_#OKRES__L4*/ meltfnum[3] =
			    /*_#SETQ___L16*/ meltfnum[12] = 1;;
			  /*_#IF___L15*/ meltfnum[13] =
			    /*_#SETQ___L16*/ meltfnum[12];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:1617:/ clear");
		    /*clear *//*_#SETQ___L16*/ meltfnum[12] = 0;
			}
			;
		      }
		    else
		      {		/*^cond.else */

	 /*_#IF___L15*/ meltfnum[13] = 0;;
		      }
		    ;

#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1619:/ cppif.then");
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
	  /*_#MELT_NEED_DBG__L17*/ meltfnum[12] =
			/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1619:/ cond");
		      /*cond */ if ( /*_#MELT_NEED_DBG__L17*/ meltfnum[12])	/*then */
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
			    MELT_LOCATION ("warmelt-hooks.melt:1619:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[5];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L18*/ meltfnum[17];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1619;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_gimple_gate after call okres=";
			      /*^apply.arg */
			      argtab[4].meltbp_long =
				/*_#OKRES__L4*/ meltfnum[3];
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V11*/ meltfptr[10] =
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1619:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L18*/ meltfnum[17] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V11*/ meltfptr[10] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1619:/ quasiblock");


		      /*_.PROGN___V13*/ meltfptr[11] =
			/*_.IF___V11*/ meltfptr[10];;
		      /*^compute */
		      /*_.IFCPP___V10*/ meltfptr[6] =
			/*_.PROGN___V13*/ meltfptr[11];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1619:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L17*/ meltfnum[12] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V11*/ meltfptr[10] = 0;
		      /*^clear */
		    /*clear *//*_.PROGN___V13*/ meltfptr[11] = 0;
		    }

#else /*MELT_HAVE_DEBUG */
		    /*^cppif.else */
		    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		    ;

		    {
		      MELT_LOCATION ("warmelt-hooks.melt:1620:/ locexp");
		      /*void */ (void) 0;
		    }
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:1615:/ quasiblock");


		    /*epilog */

		    MELT_LOCATION ("warmelt-hooks.melt:1610:/ clear");
		  /*clear *//*_.IFCPP___V5*/ meltfptr[4] = 0;
		    /*^clear */
		  /*clear *//*_.GATEFUN__V9*/ meltfptr[5] = 0;
		    /*^clear */
		  /*clear *//*_#IF___L15*/ meltfnum[13] = 0;
		    /*^clear */
		  /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
		  }

		  meltgc_restore_dump_file (oldf);
		  oldf = NULL;
		  ;
		}
		;
		/*clear *//*_#IFELSE___L12*/ meltfnum[11] = 0;
		/*epilog */
	      }
	      ;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-hooks.melt:1609:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-hooks.melt:1625:/ compute");
		/*_#OKRES__L4*/ meltfnum[3] = /*_#SETQ___L19*/ meltfnum[17] =
		  1;;
		/*_#IFELSE___L12*/ meltfnum[11] =
		  /*_#SETQ___L19*/ meltfnum[17];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:1609:/ clear");
		/*clear *//*_#SETQ___L19*/ meltfnum[17] = 0;
	      }
	      ;
	    }
	  ;
	  /*_#LET___L10*/ meltfnum[9] = /*_#IFELSE___L12*/ meltfnum[11];;

	  MELT_LOCATION ("warmelt-hooks.melt:1606:/ clear");
	      /*clear *//*_.GATEFUN__V4*/ meltfptr[3] = 0;
	  /*^clear */
	      /*clear *//*_?*/ meltfram__.loc_CSTRING__o0 = 0;
	  /*^clear */
	      /*clear *//*_#IS_CLOSURE__L11*/ meltfnum[10] = 0;
	  /*^clear */
	      /*clear *//*_#IFELSE___L12*/ meltfnum[11] = 0;
	  /*_#IF___L9*/ meltfnum[8] = /*_#LET___L10*/ meltfnum[9];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:1605:/ clear");
	      /*clear *//*_#LET___L10*/ meltfnum[9] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

   /*_#IF___L9*/ meltfnum[8] = 0;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1627:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = /*_#OKRES__L4*/ meltfnum[3];;
    MELT_LOCATION ("warmelt-hooks.melt:1627:/ finalreturn");
    ;
    /*finalret */ goto meltlabend_rout;
    /*_#LET___L2*/ meltfnum[1] = /*_#RETURN___L20*/ meltfnum[12];;

    MELT_LOCATION ("warmelt-hooks.melt:1582:/ clear");
	    /*clear *//*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;
    /*^clear */
	    /*clear *//*_#OKRES__L4*/ meltfnum[3] = 0;
    /*^clear */
	    /*clear *//*_.PASSDICT__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.MYPASS__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_#NOT__L5*/ meltfnum[4] = 0;
    /*^clear */
	    /*clear *//*_#IF___L6*/ meltfnum[5] = 0;
    /*^clear */
	    /*clear *//*_#IS_A__L8*/ meltfnum[6] = 0;
    /*^clear */
	    /*clear *//*_#IF___L9*/ meltfnum[8] = 0;
    /*^clear */
	    /*clear *//*_#RETURN___L20*/ meltfnum[12] = 0;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:1579:/ clear");
	    /*clear *//*_#LET___L2*/ meltfnum[1] = 0;
  }
  ;
  /* epilogue of melthook_HOOK_GIMPLE_GATE hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_GIMPLE_GATE hook */
  melt_topframe = meltfram__.mcfr_prev;
  return
   /*_#RESHOOK___L1*/ meltfnum[0];
}				/* end of melthook_HOOK_GIMPLE_GATE hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_GIMPLE_GATEhook */
void
melthookmark_HOOK_GIMPLE_GATE_rout (struct melt_callframe_st *meltcfram,
				    int meltmarking)
{

  melthookframe_HOOK_GIMPLE_GATE_t *meltframptr_
    = (melthookframe_HOOK_GIMPLE_GATE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_GIMPLE_GATE good hash",
		  meltframptr_->mcfr_nbvar == -1770);
  if (meltmarking)
    {				/* marking hook HOOK_GIMPLE_GATE */
      int meltix;
      for (meltix = 0; meltix < 13; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_GIMPLE_GATE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 13; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_GIMPLE_GATE */
}				/* end  melthookmark_HOOK_GIMPLE_GATE_rout */

/* frame for melthook_HOOK_GIMPLE_EXECUTE hook: */
typedef struct meltframe_melthook_HOOK_GIMPLE_EXECUTE_st
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
#define MELTFRAM_NBVARPTR 13
  melt_ptr_t mcfr_varptr[13];
#define MELTFRAM_NBVARNUM 21
  long mcfr_varnum[21];
/*others*/
  const char *loc_CSTRING__o0;
  long _spare_;
}
melthookframe_HOOK_GIMPLE_EXECUTE_t;	/* end of hook frame melthook_HOOK_GIMPLE_EXECUTE */


/** implementation of hook melthook_HOOK_GIMPLE_EXECUTE */
long
melthook_HOOK_GIMPLE_EXECUTE (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_GIMPLE_EXECUTE from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_GIMPLE_EXECUTE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_GIMPLE_EXECUTE;
  long melt_thishookcall_counter_HOOK_GIMPLE_EXECUTE ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_GIMPLE_EXECUTE++;
#define meltcallcount melt_thishookcall_counter_HOOK_GIMPLE_EXECUTE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_GIMPLE_EXECUTE_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_GIMPLE_EXECUTE */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -2134 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_GIMPLE_EXECUTE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_GIMPLE_EXECUTE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_GIMPLE_EXECUTE hook */
  MELT_LOCATION ("warmelt-hooks.melt:1632:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1635:/ quasiblock");


  /*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;;
    /*^compute */
  /*_#OKRES__L4*/ meltfnum[3] = 0;;
    /*^compute */
  /*_#PASSNUM__L5*/ meltfnum[4] = 0;;
    MELT_LOCATION ("warmelt-hooks.melt:1638:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~INITIAL_SYSTEM_DATA */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_SYSTEM_DATA */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~INITIAL_SYSTEM_DATA */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 9, "SYSDATA_PASS_DICT");
    /*_.PASSDICT__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.PASSDICT__V2*/ meltfptr[1] = NULL;;
      }
    ;
    /*^compute */
    /*_.MYPASS__V3*/ meltfptr[2] = ( /*nil */ NULL);;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1641:/ locexp");
      /* hook_gimple_execute GETHASMODE_CHK__2 */
      static const char *GETHASMODE_CHK__2modstr;
      if (!GETHASMODE_CHK__2modstr)
	GETHASMODE_CHK__2modstr = melt_argument ("mode");
		/*_#HASMODEFLAG__L3*/ meltfnum[2] = GETHASMODE_CHK__2modstr != NULL
	&& GETHASMODE_CHK__2modstr[0];
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#NOT__L6*/ meltfnum[5] =
      (!( /*_#HASMODEFLAG__L3*/ meltfnum[2]));;
    MELT_LOCATION ("warmelt-hooks.melt:1648:/ cond");
    /*cond */ if ( /*_#NOT__L6*/ meltfnum[5])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1649:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = 0;;
	  MELT_LOCATION ("warmelt-hooks.melt:1649:/ finalreturn");
	  ;
	  /*finalret */ goto meltlabend_rout;
	  /*_#IF___L7*/ meltfnum[6] = /*_#RETURN___L8*/ meltfnum[7];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:1648:/ clear");
	      /*clear *//*_#RETURN___L8*/ meltfnum[7] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

   /*_#IF___L7*/ meltfnum[6] = 0;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1650:/ locexp");
      /* hook_gimple_execute GETMYPASS_CHK__2 */
      gcc_assert (current_pass != NULL);
      gcc_assert (current_pass->name != NULL);
      gcc_assert (current_pass->type == GIMPLE_PASS);
      /*_.MYPASS__V3*/ meltfptr[2] =
	melt_get_mapstrings ((struct meltmapstrings_st *) /*_.PASSDICT__V2*/
			     meltfptr[1],
			     current_pass->name);
     /*_#PASSNUM__L5*/ meltfnum[4] = current_pass->static_pass_number;
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#IS_A__L9*/ meltfnum[7] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]),
			   (melt_ptr_t) (( /*~CLASS_GCC_GIMPLE_PASS */
					  meltfhook->tabval[2])));;
    MELT_LOCATION ("warmelt-hooks.melt:1660:/ cond");
    /*cond */ if ( /*_#IS_A__L9*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:1661:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.MYPASS__V3*/ meltfptr[2]),
					      (melt_ptr_t) (( /*~CLASS_GCC_PASS */ meltfhook->tabval[3])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj = (melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "GCCPASS_EXEC");
      /*_.EXECFUN__V4*/ meltfptr[3] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

     /*_.EXECFUN__V4*/ meltfptr[3] = NULL;;
	    }
	  ;
	  /*^compute */
    /*_?*/ meltfram__.loc_CSTRING__o0 =
	    (char *) 0;;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
    /*_#IS_CLOSURE__L10*/ meltfnum[9] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.EXECFUN__V4*/ meltfptr[3]))
	     == MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1664:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L10*/ meltfnum[9])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-hooks.melt:1665:/ locexp");
		  /* hook_gimple_execute DOEXECFUN_CHK__1 */
		  FILE *oldf = meltgc_set_dump_file (dump_file);
	       /*_?*/ meltfram__.loc_CSTRING__o0 = current_pass->name;
		  /*^block */
		  /*anyblock */
		  {


#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1671:/ cppif.then");
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
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1671:/ cond");
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
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1671:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[9];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1671;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_gimple_execute curpassname=";
			      /*^apply.arg */
			      argtab[4].meltbp_cstring =
				/*_?*/ meltfram__.loc_CSTRING__o0;
			      /*^apply.arg */
			      argtab[5].meltbp_cstring = " mypass=";
			      /*^apply.arg */
			      argtab[6].meltbp_aptr =
				(melt_ptr_t *) & /*_.MYPASS__V3*/ meltfptr[2];
			      /*^apply.arg */
			      argtab[7].meltbp_cstring = " passnum=";
			      /*^apply.arg */
			      argtab[8].meltbp_long =
				/*_#PASSNUM__L5*/ meltfnum[4];
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_PTR
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V6*/ meltfptr[5] =
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1671:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L12*/ meltfnum[11] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V6*/ meltfptr[5] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1671:/ quasiblock");


		      /*_.PROGN___V8*/ meltfptr[6] =
			/*_.IF___V6*/ meltfptr[5];;
		      /*^compute */
		      /*_.IFCPP___V5*/ meltfptr[4] =
			/*_.PROGN___V8*/ meltfptr[6];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1671:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L11*/ meltfnum[10] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V6*/ meltfptr[5] = 0;
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
		    MELT_LOCATION ("warmelt-hooks.melt:1673:/ quasiblock");


		    /*^multiapply */
		    /*multiapply 2args, 1x.res */
		    {
		      union meltparam_un argtab[1];

		      union meltparam_un restab[1];
		      memset (&restab, 0, sizeof (restab));
		      memset (&argtab, 0, sizeof (argtab));
		      /*^multiapply.arg */
		      argtab[0].meltbp_long = /*_#PASSNUM__L5*/ meltfnum[4];
		      /*^multiapply.xres */
		      restab[0].meltbp_longptr =
			& /*_#FLAGEXEC__L14*/ meltfnum[10];
		      /*^multiapply.appl */
		      /*_.RESEXEC__V9*/ meltfptr[5] =
			melt_apply ((meltclosure_ptr_t)
				    ( /*_.EXECFUN__V4*/ meltfptr[3]),
				    (melt_ptr_t) ( /*_.MYPASS__V3*/
						  meltfptr[2]),
				    (MELTBPARSTR_LONG ""), argtab,
				    (MELTBPARSTR_LONG ""), restab);
		    }
		    ;
		    /*^quasiblock */



#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1676:/ cppif.then");
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
	  /*_#MELT_NEED_DBG__L15*/ meltfnum[14] =
			/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1676:/ cond");
		      /*cond */ if ( /*_#MELT_NEED_DBG__L15*/ meltfnum[14])	/*then */
			{
			  /*^cond.then */
			  /*^block */
			  /*anyblock */
			  {

	    /*_#MELT_CALLCOUNT__L16*/ meltfnum[15] =
			      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			      meltcallcount	/* melt_callcount debugging */
#else
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1676:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[7];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1676;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_gimple_execute after call resexec=";
			      /*^apply.arg */
			      argtab[4].meltbp_aptr =
				(melt_ptr_t *) & /*_.RESEXEC__V9*/
				meltfptr[5];
			      /*^apply.arg */
			      argtab[5].meltbp_cstring = " flagexec=";
			      /*^apply.arg */
			      argtab[6].meltbp_long =
				/*_#FLAGEXEC__L14*/ meltfnum[10];
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_PTR
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V11*/ meltfptr[10] =
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1676:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L16*/ meltfnum[15] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V11*/ meltfptr[10] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1676:/ quasiblock");


		      /*_.PROGN___V13*/ meltfptr[11] =
			/*_.IF___V11*/ meltfptr[10];;
		      /*^compute */
		      /*_.IFCPP___V10*/ meltfptr[6] =
			/*_.PROGN___V13*/ meltfptr[11];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1676:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L15*/ meltfnum[14] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V11*/ meltfptr[10] = 0;
		      /*^clear */
		    /*clear *//*_.PROGN___V13*/ meltfptr[11] = 0;
		    }

#else /*MELT_HAVE_DEBUG */
		    /*^cppif.else */
		    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		    ;

		    MELT_CHECK_SIGNAL ();
		    ;
		    /*^compute */
	/*_#NULL__L17*/ meltfnum[15] =
		      (( /*_.RESEXEC__V9*/ meltfptr[5]) == NULL);;
		    MELT_LOCATION ("warmelt-hooks.melt:1678:/ cond");
		    /*cond */ if ( /*_#NULL__L17*/ meltfnum[15])	/*then */
		      {
			/*^cond.then */
			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-hooks.melt:1679:/ compute");
			  /*_#OKRES__L4*/ meltfnum[3] =
			    /*_#SETQ___L19*/ meltfnum[18] =
			    /*_#FLAGEXEC__L14*/ meltfnum[10];;
			  /*_#IFELSE___L18*/ meltfnum[14] =
			    /*_#SETQ___L19*/ meltfnum[18];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:1678:/ clear");
		    /*clear *//*_#SETQ___L19*/ meltfnum[18] = 0;
			}
			;
		      }
		    else
		      {		/*^cond.else */

			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-hooks.melt:1680:/ compute");
			  /*_#OKRES__L4*/ meltfnum[3] =
			    /*_#SETQ___L20*/ meltfnum[18] = 1;;
			  /*_#IFELSE___L18*/ meltfnum[14] =
			    /*_#SETQ___L20*/ meltfnum[18];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:1678:/ clear");
		    /*clear *//*_#SETQ___L20*/ meltfnum[18] = 0;
			}
			;
		      }
		    ;

		    MELT_CHECK_SIGNAL ();
		    ;
		    /*_#MULTI___L13*/ meltfnum[11] =
		      /*_#IFELSE___L18*/ meltfnum[14];;

		    MELT_LOCATION ("warmelt-hooks.melt:1673:/ clear");
		  /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
		    /*^clear */
		  /*clear *//*_#NULL__L17*/ meltfnum[15] = 0;
		    /*^clear */
		  /*clear *//*_#IFELSE___L18*/ meltfnum[14] = 0;

		    /*^clear */
		  /*clear *//*_#FLAGEXEC__L14*/ meltfnum[10] = 0;

		    {
		      MELT_LOCATION ("warmelt-hooks.melt:1682:/ locexp");
		      /*void */ (void) 0;
		    }
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:1670:/ quasiblock");


		    /*epilog */

		    MELT_LOCATION ("warmelt-hooks.melt:1665:/ clear");
		  /*clear *//*_.IFCPP___V5*/ meltfptr[4] = 0;
		    /*^clear */
		  /*clear *//*_#MULTI___L13*/ meltfnum[11] = 0;
		  }

		  meltgc_restore_dump_file (oldf);
		  oldf = NULL;
		  ;
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:1661:/ clear");
	      /*clear *//*_.EXECFUN__V4*/ meltfptr[3] = 0;
	  /*^clear */
	      /*clear *//*_?*/ meltfram__.loc_CSTRING__o0 = 0;
	  /*^clear */
	      /*clear *//*_#IS_CLOSURE__L10*/ meltfnum[9] = 0;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1688:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = /*_#OKRES__L4*/ meltfnum[3];;
    MELT_LOCATION ("warmelt-hooks.melt:1688:/ finalreturn");
    ;
    /*finalret */ goto meltlabend_rout;
    /*_#LET___L2*/ meltfnum[1] = /*_#RETURN___L21*/ meltfnum[18];;

    MELT_LOCATION ("warmelt-hooks.melt:1635:/ clear");
	    /*clear *//*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;
    /*^clear */
	    /*clear *//*_#OKRES__L4*/ meltfnum[3] = 0;
    /*^clear */
	    /*clear *//*_#PASSNUM__L5*/ meltfnum[4] = 0;
    /*^clear */
	    /*clear *//*_.PASSDICT__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.MYPASS__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_#NOT__L6*/ meltfnum[5] = 0;
    /*^clear */
	    /*clear *//*_#IF___L7*/ meltfnum[6] = 0;
    /*^clear */
	    /*clear *//*_#IS_A__L9*/ meltfnum[7] = 0;
    /*^clear */
	    /*clear *//*_#RETURN___L21*/ meltfnum[18] = 0;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:1632:/ clear");
	    /*clear *//*_#LET___L2*/ meltfnum[1] = 0;
  }
  ;
  /* epilogue of melthook_HOOK_GIMPLE_EXECUTE hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_GIMPLE_EXECUTE hook */
  melt_topframe = meltfram__.mcfr_prev;
  return
   /*_#RESHOOK___L1*/ meltfnum[0];
}				/* end of melthook_HOOK_GIMPLE_EXECUTE hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_GIMPLE_EXECUTEhook */
void
melthookmark_HOOK_GIMPLE_EXECUTE_rout (struct melt_callframe_st *meltcfram,
				       int meltmarking)
{

  melthookframe_HOOK_GIMPLE_EXECUTE_t *meltframptr_
    = (melthookframe_HOOK_GIMPLE_EXECUTE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_GIMPLE_EXECUTE good hash",
		  meltframptr_->mcfr_nbvar == -2134);
  if (meltmarking)
    {				/* marking hook HOOK_GIMPLE_EXECUTE */
      int meltix;
      for (meltix = 0; meltix < 13; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_GIMPLE_EXECUTE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 13; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_GIMPLE_EXECUTE */
}				/* end  melthookmark_HOOK_GIMPLE_EXECUTE_rout */

/* frame for melthook_HOOK_RTL_GATE hook: */
typedef struct meltframe_melthook_HOOK_RTL_GATE_st
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
#define MELTFRAM_NBVARPTR 13
  melt_ptr_t mcfr_varptr[13];
#define MELTFRAM_NBVARNUM 21
  long mcfr_varnum[21];
/*others*/
  const char *loc_CSTRING__o0;
  long _spare_;
}
melthookframe_HOOK_RTL_GATE_t;	/* end of hook frame melthook_HOOK_RTL_GATE */


/** implementation of hook melthook_HOOK_RTL_GATE */
long
melthook_HOOK_RTL_GATE (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_RTL_GATE from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_RTL_GATE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_RTL_GATE;
  long melt_thishookcall_counter_HOOK_RTL_GATE ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_RTL_GATE++;
#define meltcallcount melt_thishookcall_counter_HOOK_RTL_GATE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_RTL_GATE_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_RTL_GATE */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -111 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_RTL_GATE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_RTL_GATE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_RTL_GATE hook */
  MELT_LOCATION ("warmelt-hooks.melt:1695:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1698:/ quasiblock");


  /*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;;
    /*^compute */
  /*_#OKRES__L4*/ meltfnum[3] = 0;;
    MELT_LOCATION ("warmelt-hooks.melt:1700:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~INITIAL_SYSTEM_DATA */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_SYSTEM_DATA */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~INITIAL_SYSTEM_DATA */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 9, "SYSDATA_PASS_DICT");
    /*_.PASSDICT__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.PASSDICT__V2*/ meltfptr[1] = NULL;;
      }
    ;
    /*^compute */
    /*_.MYPASS__V3*/ meltfptr[2] = ( /*nil */ NULL);;
    /*^compute */
  /*_#PASSNUM__L5*/ meltfnum[4] = 0;;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1704:/ locexp");
      /* hook_rtl_gate GETHASMODE_CHK__3 */
      static const char *GETHASMODE_CHK__3modstr;
      if (!GETHASMODE_CHK__3modstr)
	GETHASMODE_CHK__3modstr = melt_argument ("mode");
		/*_#HASMODEFLAG__L3*/ meltfnum[2] = GETHASMODE_CHK__3modstr != NULL
	&& GETHASMODE_CHK__3modstr[0];
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#NOT__L6*/ meltfnum[5] =
      (!( /*_#HASMODEFLAG__L3*/ meltfnum[2]));;
    MELT_LOCATION ("warmelt-hooks.melt:1711:/ cond");
    /*cond */ if ( /*_#NOT__L6*/ meltfnum[5])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1712:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = 0;;
	  MELT_LOCATION ("warmelt-hooks.melt:1712:/ finalreturn");
	  ;
	  /*finalret */ goto meltlabend_rout;
	  /*_#IF___L7*/ meltfnum[6] = /*_#RETURN___L8*/ meltfnum[7];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:1711:/ clear");
	      /*clear *//*_#RETURN___L8*/ meltfnum[7] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

   /*_#IF___L7*/ meltfnum[6] = 0;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1713:/ locexp");
      /* hook_rtl_gate GETMYPASS_CHK__3 */
      gcc_assert (current_pass != NULL);
      gcc_assert (current_pass->name != NULL);
      gcc_assert (current_pass->type == RTL_PASS);
      /*_.MYPASS__V3*/ meltfptr[2] =
	melt_get_mapstrings ((struct meltmapstrings_st *) /*_.PASSDICT__V2*/
			     meltfptr[1],
			     current_pass->name);
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#IS_A__L9*/ meltfnum[7] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]),
			   (melt_ptr_t) (( /*~CLASS_GCC_RTL_PASS */
					  meltfhook->tabval[2])));;
    MELT_LOCATION ("warmelt-hooks.melt:1722:/ cond");
    /*cond */ if ( /*_#IS_A__L9*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:1723:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.MYPASS__V3*/ meltfptr[2]),
					      (melt_ptr_t) (( /*~CLASS_GCC_PASS */ meltfhook->tabval[3])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj = (melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 2, "GCCPASS_GATE");
      /*_.GATEFUN__V4*/ meltfptr[3] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

     /*_.GATEFUN__V4*/ meltfptr[3] = NULL;;
	    }
	  ;
	  /*^compute */
    /*_?*/ meltfram__.loc_CSTRING__o0 =
	    (char *) 0;;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
    /*_#IS_CLOSURE__L12*/ meltfnum[11] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.GATEFUN__V4*/ meltfptr[3]))
	     == MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1726:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L12*/ meltfnum[11])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {




		{
		  MELT_LOCATION ("warmelt-hooks.melt:1727:/ locexp");
		  /* hook_rtl_gate DOGATEFUN_CHK__2 */
		  FILE *oldf = meltgc_set_dump_file (dump_file);
	       /*_?*/ meltfram__.loc_CSTRING__o0 = current_pass->name;
	       /*_#PASSNUM__L5*/ meltfnum[4] =
		    current_pass->static_pass_number;
		  /*^block */
		  /*anyblock */
		  {


#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1734:/ cppif.then");
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
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1734:/ cond");
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
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1734:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[7];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L15*/ meltfnum[14];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1734;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_rtl_gate curpassname=";
			      /*^apply.arg */
			      argtab[4].meltbp_cstring =
				/*_?*/ meltfram__.loc_CSTRING__o0;
			      /*^apply.arg */
			      argtab[5].meltbp_cstring = " mypass=";
			      /*^apply.arg */
			      argtab[6].meltbp_aptr =
				(melt_ptr_t *) & /*_.MYPASS__V3*/ meltfptr[2];
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_PTR ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V6*/ meltfptr[5] =
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1734:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L15*/ meltfnum[14] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V6*/ meltfptr[5] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1734:/ quasiblock");


		      /*_.PROGN___V8*/ meltfptr[6] =
			/*_.IF___V6*/ meltfptr[5];;
		      /*^compute */
		      /*_.IFCPP___V5*/ meltfptr[4] =
			/*_.PROGN___V8*/ meltfptr[6];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1734:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L14*/ meltfnum[13] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V6*/ meltfptr[5] = 0;
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

		    MELT_CHECK_SIGNAL ();
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:1735:/ apply");
		    /*apply */
		    {
		      union meltparam_un argtab[1];
		      memset (&argtab, 0, sizeof (argtab));
		      /*^apply.arg */
		      argtab[0].meltbp_long = /*_#PASSNUM__L5*/ meltfnum[4];
		      /*_.GATEFUN__V9*/ meltfptr[5] =
			melt_apply ((meltclosure_ptr_t)
				    ( /*_.GATEFUN__V4*/ meltfptr[3]),
				    (melt_ptr_t) ( /*_.MYPASS__V3*/
						  meltfptr[2]),
				    (MELTBPARSTR_LONG ""), argtab, "",
				    (union meltparam_un *) 0);
		    }
		    ;
		    /*^cond */
		    /*cond */ if ( /*_.GATEFUN__V9*/ meltfptr[5])	/*then */
		      {
			/*^cond.then */
			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-hooks.melt:1736:/ compute");
			  /*_#OKRES__L4*/ meltfnum[3] =
			    /*_#SETQ___L17*/ meltfnum[13] = 1;;
			  /*_#IF___L16*/ meltfnum[14] =
			    /*_#SETQ___L17*/ meltfnum[13];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:1735:/ clear");
		    /*clear *//*_#SETQ___L17*/ meltfnum[13] = 0;
			}
			;
		      }
		    else
		      {		/*^cond.else */

	 /*_#IF___L16*/ meltfnum[14] = 0;;
		      }
		    ;

#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1737:/ cppif.then");
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
	  /*_#MELT_NEED_DBG__L18*/ meltfnum[13] =
			/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1737:/ cond");
		      /*cond */ if ( /*_#MELT_NEED_DBG__L18*/ meltfnum[13])	/*then */
			{
			  /*^cond.then */
			  /*^block */
			  /*anyblock */
			  {

	    /*_#MELT_CALLCOUNT__L19*/ meltfnum[18] =
			      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			      meltcallcount	/* melt_callcount debugging */
#else
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1737:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[5];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1737;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_rtl_gate after call okres=";
			      /*^apply.arg */
			      argtab[4].meltbp_long =
				/*_#OKRES__L4*/ meltfnum[3];
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V11*/ meltfptr[10] =
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1737:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L19*/ meltfnum[18] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V11*/ meltfptr[10] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1737:/ quasiblock");


		      /*_.PROGN___V13*/ meltfptr[11] =
			/*_.IF___V11*/ meltfptr[10];;
		      /*^compute */
		      /*_.IFCPP___V10*/ meltfptr[6] =
			/*_.PROGN___V13*/ meltfptr[11];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1737:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L18*/ meltfnum[13] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V11*/ meltfptr[10] = 0;
		      /*^clear */
		    /*clear *//*_.PROGN___V13*/ meltfptr[11] = 0;
		    }

#else /*MELT_HAVE_DEBUG */
		    /*^cppif.else */
		    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		    ;

		    {
		      MELT_LOCATION ("warmelt-hooks.melt:1738:/ locexp");
		      /*void */ (void) 0;
		    }
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:1733:/ quasiblock");


		    /*epilog */

		    MELT_LOCATION ("warmelt-hooks.melt:1727:/ clear");
		  /*clear *//*_.IFCPP___V5*/ meltfptr[4] = 0;
		    /*^clear */
		  /*clear *//*_.GATEFUN__V9*/ meltfptr[5] = 0;
		    /*^clear */
		  /*clear *//*_#IF___L16*/ meltfnum[14] = 0;
		    /*^clear */
		  /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
		  }

		  meltgc_restore_dump_file (oldf);
		  oldf = NULL;
		  ;
		}
		;
		/*clear *//*_#IFELSE___L13*/ meltfnum[12] = 0;
		/*epilog */
	      }
	      ;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-hooks.melt:1726:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-hooks.melt:1743:/ compute");
		/*_#OKRES__L4*/ meltfnum[3] = /*_#SETQ___L20*/ meltfnum[18] =
		  1;;
		/*_#IFELSE___L13*/ meltfnum[12] =
		  /*_#SETQ___L20*/ meltfnum[18];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:1726:/ clear");
		/*clear *//*_#SETQ___L20*/ meltfnum[18] = 0;
	      }
	      ;
	    }
	  ;
	  /*_#LET___L11*/ meltfnum[10] = /*_#IFELSE___L13*/ meltfnum[12];;

	  MELT_LOCATION ("warmelt-hooks.melt:1723:/ clear");
	      /*clear *//*_.GATEFUN__V4*/ meltfptr[3] = 0;
	  /*^clear */
	      /*clear *//*_?*/ meltfram__.loc_CSTRING__o0 = 0;
	  /*^clear */
	      /*clear *//*_#IS_CLOSURE__L12*/ meltfnum[11] = 0;
	  /*^clear */
	      /*clear *//*_#IFELSE___L13*/ meltfnum[12] = 0;
	  /*_#IF___L10*/ meltfnum[9] = /*_#LET___L11*/ meltfnum[10];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:1722:/ clear");
	      /*clear *//*_#LET___L11*/ meltfnum[10] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

   /*_#IF___L10*/ meltfnum[9] = 0;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1745:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = /*_#OKRES__L4*/ meltfnum[3];;
    MELT_LOCATION ("warmelt-hooks.melt:1745:/ finalreturn");
    ;
    /*finalret */ goto meltlabend_rout;
    /*_#LET___L2*/ meltfnum[1] = /*_#RETURN___L21*/ meltfnum[13];;

    MELT_LOCATION ("warmelt-hooks.melt:1698:/ clear");
	    /*clear *//*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;
    /*^clear */
	    /*clear *//*_#OKRES__L4*/ meltfnum[3] = 0;
    /*^clear */
	    /*clear *//*_.PASSDICT__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.MYPASS__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_#PASSNUM__L5*/ meltfnum[4] = 0;
    /*^clear */
	    /*clear *//*_#NOT__L6*/ meltfnum[5] = 0;
    /*^clear */
	    /*clear *//*_#IF___L7*/ meltfnum[6] = 0;
    /*^clear */
	    /*clear *//*_#IS_A__L9*/ meltfnum[7] = 0;
    /*^clear */
	    /*clear *//*_#IF___L10*/ meltfnum[9] = 0;
    /*^clear */
	    /*clear *//*_#RETURN___L21*/ meltfnum[13] = 0;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:1695:/ clear");
	    /*clear *//*_#LET___L2*/ meltfnum[1] = 0;
  }
  ;
  /* epilogue of melthook_HOOK_RTL_GATE hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_RTL_GATE hook */
  melt_topframe = meltfram__.mcfr_prev;
  return
   /*_#RESHOOK___L1*/ meltfnum[0];
}				/* end of melthook_HOOK_RTL_GATE hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_RTL_GATEhook */
void
melthookmark_HOOK_RTL_GATE_rout (struct melt_callframe_st *meltcfram,
				 int meltmarking)
{

  melthookframe_HOOK_RTL_GATE_t *meltframptr_
    = (melthookframe_HOOK_RTL_GATE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_RTL_GATE good hash",
		  meltframptr_->mcfr_nbvar == -111);
  if (meltmarking)
    {				/* marking hook HOOK_RTL_GATE */
      int meltix;
      for (meltix = 0; meltix < 13; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_RTL_GATE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 13; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_RTL_GATE */
}				/* end  melthookmark_HOOK_RTL_GATE_rout */

/* frame for melthook_HOOK_RTL_EXECUTE hook: */
typedef struct meltframe_melthook_HOOK_RTL_EXECUTE_st
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
#define MELTFRAM_NBVARPTR 13
  melt_ptr_t mcfr_varptr[13];
#define MELTFRAM_NBVARNUM 21
  long mcfr_varnum[21];
/*others*/
  const char *loc_CSTRING__o0;
  long _spare_;
}
melthookframe_HOOK_RTL_EXECUTE_t;	/* end of hook frame melthook_HOOK_RTL_EXECUTE */


/** implementation of hook melthook_HOOK_RTL_EXECUTE */
long
melthook_HOOK_RTL_EXECUTE (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_RTL_EXECUTE from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_RTL_EXECUTE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_RTL_EXECUTE;
  long melt_thishookcall_counter_HOOK_RTL_EXECUTE ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_RTL_EXECUTE++;
#define meltcallcount melt_thishookcall_counter_HOOK_RTL_EXECUTE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_RTL_EXECUTE_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_RTL_EXECUTE */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -2110 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_RTL_EXECUTE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_RTL_EXECUTE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_RTL_EXECUTE hook */
  MELT_LOCATION ("warmelt-hooks.melt:1750:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1753:/ quasiblock");


  /*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;;
    /*^compute */
  /*_#OKRES__L4*/ meltfnum[3] = 0;;
    MELT_LOCATION ("warmelt-hooks.melt:1755:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~INITIAL_SYSTEM_DATA */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_SYSTEM_DATA */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~INITIAL_SYSTEM_DATA */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 9, "SYSDATA_PASS_DICT");
    /*_.PASSDICT__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.PASSDICT__V2*/ meltfptr[1] = NULL;;
      }
    ;
    /*^compute */
    /*_.MYPASS__V3*/ meltfptr[2] = ( /*nil */ NULL);;
    /*^compute */
  /*_#PASSNUM__L5*/ meltfnum[4] = 0;;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1759:/ locexp");
      /* hook_rtl_execute GETHASMODE_CHK__4 */
      static const char *GETHASMODE_CHK__4modstr;
      if (!GETHASMODE_CHK__4modstr)
	GETHASMODE_CHK__4modstr = melt_argument ("mode");
		/*_#HASMODEFLAG__L3*/ meltfnum[2] = GETHASMODE_CHK__4modstr != NULL
	&& GETHASMODE_CHK__4modstr[0];
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#NOT__L6*/ meltfnum[5] =
      (!( /*_#HASMODEFLAG__L3*/ meltfnum[2]));;
    MELT_LOCATION ("warmelt-hooks.melt:1766:/ cond");
    /*cond */ if ( /*_#NOT__L6*/ meltfnum[5])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1767:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = 0;;
	  MELT_LOCATION ("warmelt-hooks.melt:1767:/ finalreturn");
	  ;
	  /*finalret */ goto meltlabend_rout;
	  /*_#IF___L7*/ meltfnum[6] = /*_#RETURN___L8*/ meltfnum[7];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:1766:/ clear");
	      /*clear *//*_#RETURN___L8*/ meltfnum[7] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

   /*_#IF___L7*/ meltfnum[6] = 0;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1768:/ locexp");
      /* hook_rtl_execute GETMYPASS_CHK__4 */
      gcc_assert (current_pass != NULL);
      gcc_assert (current_pass->name != NULL);
      gcc_assert (current_pass->type == RTL_PASS);
      /*_.MYPASS__V3*/ meltfptr[2] =
	melt_get_mapstrings ((struct meltmapstrings_st *) /*_.PASSDICT__V2*/
			     meltfptr[1],
			     current_pass->name);
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#IS_A__L9*/ meltfnum[7] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]),
			   (melt_ptr_t) (( /*~CLASS_GCC_RTL_PASS */
					  meltfhook->tabval[2])));;
    MELT_LOCATION ("warmelt-hooks.melt:1777:/ cond");
    /*cond */ if ( /*_#IS_A__L9*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:1778:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.MYPASS__V3*/ meltfptr[2]),
					      (melt_ptr_t) (( /*~CLASS_GCC_PASS */ meltfhook->tabval[3])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj = (melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "GCCPASS_EXEC");
      /*_.EXECFUN__V4*/ meltfptr[3] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

     /*_.EXECFUN__V4*/ meltfptr[3] = NULL;;
	    }
	  ;
	  /*^compute */
    /*_?*/ meltfram__.loc_CSTRING__o0 =
	    (char *) 0;;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
    /*_#IS_CLOSURE__L10*/ meltfnum[9] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.EXECFUN__V4*/ meltfptr[3]))
	     == MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1781:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L10*/ meltfnum[9])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-hooks.melt:1782:/ locexp");
		  /* hook_rtl_execute DOEXECFUN_CHK__2 */
	       /*_#PASSNUM__L5*/ meltfnum[4] =
		    current_pass->static_pass_number;
		  FILE *oldf = meltgc_set_dump_file (dump_file);
	       /*_?*/ meltfram__.loc_CSTRING__o0 = current_pass->name;
		  /*^block */
		  /*anyblock */
		  {


#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1789:/ cppif.then");
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
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1789:/ cond");
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
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1789:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[9];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1789;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_rtl_execute curpassname=";
			      /*^apply.arg */
			      argtab[4].meltbp_cstring =
				/*_?*/ meltfram__.loc_CSTRING__o0;
			      /*^apply.arg */
			      argtab[5].meltbp_cstring = " mypass=";
			      /*^apply.arg */
			      argtab[6].meltbp_aptr =
				(melt_ptr_t *) & /*_.MYPASS__V3*/ meltfptr[2];
			      /*^apply.arg */
			      argtab[7].meltbp_cstring = " passnum=";
			      /*^apply.arg */
			      argtab[8].meltbp_long =
				/*_#PASSNUM__L5*/ meltfnum[4];
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_PTR
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V6*/ meltfptr[5] =
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1789:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L12*/ meltfnum[11] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V6*/ meltfptr[5] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1789:/ quasiblock");


		      /*_.PROGN___V8*/ meltfptr[6] =
			/*_.IF___V6*/ meltfptr[5];;
		      /*^compute */
		      /*_.IFCPP___V5*/ meltfptr[4] =
			/*_.PROGN___V8*/ meltfptr[6];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1789:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L11*/ meltfnum[10] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V6*/ meltfptr[5] = 0;
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
		    MELT_LOCATION ("warmelt-hooks.melt:1791:/ quasiblock");


		    /*^multiapply */
		    /*multiapply 2args, 1x.res */
		    {
		      union meltparam_un argtab[1];

		      union meltparam_un restab[1];
		      memset (&restab, 0, sizeof (restab));
		      memset (&argtab, 0, sizeof (argtab));
		      /*^multiapply.arg */
		      argtab[0].meltbp_long = /*_#PASSNUM__L5*/ meltfnum[4];
		      /*^multiapply.xres */
		      restab[0].meltbp_longptr =
			& /*_#FLAGEXEC__L14*/ meltfnum[10];
		      /*^multiapply.appl */
		      /*_.RESEXEC__V9*/ meltfptr[5] =
			melt_apply ((meltclosure_ptr_t)
				    ( /*_.EXECFUN__V4*/ meltfptr[3]),
				    (melt_ptr_t) ( /*_.MYPASS__V3*/
						  meltfptr[2]),
				    (MELTBPARSTR_LONG ""), argtab,
				    (MELTBPARSTR_LONG ""), restab);
		    }
		    ;
		    /*^quasiblock */



#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1794:/ cppif.then");
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
	  /*_#MELT_NEED_DBG__L15*/ meltfnum[14] =
			/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1794:/ cond");
		      /*cond */ if ( /*_#MELT_NEED_DBG__L15*/ meltfnum[14])	/*then */
			{
			  /*^cond.then */
			  /*^block */
			  /*anyblock */
			  {

	    /*_#MELT_CALLCOUNT__L16*/ meltfnum[15] =
			      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			      meltcallcount	/* melt_callcount debugging */
#else
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1794:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[7];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1794;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_rtl_execute after call resexec=";
			      /*^apply.arg */
			      argtab[4].meltbp_aptr =
				(melt_ptr_t *) & /*_.RESEXEC__V9*/
				meltfptr[5];
			      /*^apply.arg */
			      argtab[5].meltbp_cstring = " flagexec=";
			      /*^apply.arg */
			      argtab[6].meltbp_long =
				/*_#FLAGEXEC__L14*/ meltfnum[10];
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_PTR
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V11*/ meltfptr[10] =
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1794:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L16*/ meltfnum[15] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V11*/ meltfptr[10] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1794:/ quasiblock");


		      /*_.PROGN___V13*/ meltfptr[11] =
			/*_.IF___V11*/ meltfptr[10];;
		      /*^compute */
		      /*_.IFCPP___V10*/ meltfptr[6] =
			/*_.PROGN___V13*/ meltfptr[11];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1794:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L15*/ meltfnum[14] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V11*/ meltfptr[10] = 0;
		      /*^clear */
		    /*clear *//*_.PROGN___V13*/ meltfptr[11] = 0;
		    }

#else /*MELT_HAVE_DEBUG */
		    /*^cppif.else */
		    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		    ;

		    MELT_CHECK_SIGNAL ();
		    ;
		    /*^compute */
	/*_#NULL__L17*/ meltfnum[15] =
		      (( /*_.RESEXEC__V9*/ meltfptr[5]) == NULL);;
		    MELT_LOCATION ("warmelt-hooks.melt:1796:/ cond");
		    /*cond */ if ( /*_#NULL__L17*/ meltfnum[15])	/*then */
		      {
			/*^cond.then */
			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-hooks.melt:1797:/ compute");
			  /*_#OKRES__L4*/ meltfnum[3] =
			    /*_#SETQ___L19*/ meltfnum[18] =
			    /*_#FLAGEXEC__L14*/ meltfnum[10];;
			  /*_#IFELSE___L18*/ meltfnum[14] =
			    /*_#SETQ___L19*/ meltfnum[18];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:1796:/ clear");
		    /*clear *//*_#SETQ___L19*/ meltfnum[18] = 0;
			}
			;
		      }
		    else
		      {		/*^cond.else */

			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-hooks.melt:1798:/ compute");
			  /*_#OKRES__L4*/ meltfnum[3] =
			    /*_#SETQ___L20*/ meltfnum[18] = 1;;
			  /*_#IFELSE___L18*/ meltfnum[14] =
			    /*_#SETQ___L20*/ meltfnum[18];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:1796:/ clear");
		    /*clear *//*_#SETQ___L20*/ meltfnum[18] = 0;
			}
			;
		      }
		    ;

		    MELT_CHECK_SIGNAL ();
		    ;
		    /*_#MULTI___L13*/ meltfnum[11] =
		      /*_#IFELSE___L18*/ meltfnum[14];;

		    MELT_LOCATION ("warmelt-hooks.melt:1791:/ clear");
		  /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
		    /*^clear */
		  /*clear *//*_#NULL__L17*/ meltfnum[15] = 0;
		    /*^clear */
		  /*clear *//*_#IFELSE___L18*/ meltfnum[14] = 0;

		    /*^clear */
		  /*clear *//*_#FLAGEXEC__L14*/ meltfnum[10] = 0;

		    {
		      MELT_LOCATION ("warmelt-hooks.melt:1800:/ locexp");
		      /*void */ (void) 0;
		    }
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:1788:/ quasiblock");


		    /*epilog */

		    MELT_LOCATION ("warmelt-hooks.melt:1782:/ clear");
		  /*clear *//*_.IFCPP___V5*/ meltfptr[4] = 0;
		    /*^clear */
		  /*clear *//*_#MULTI___L13*/ meltfnum[11] = 0;
		  }

		  meltgc_restore_dump_file (oldf);
		  oldf = NULL;
		  ;
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:1778:/ clear");
	      /*clear *//*_.EXECFUN__V4*/ meltfptr[3] = 0;
	  /*^clear */
	      /*clear *//*_?*/ meltfram__.loc_CSTRING__o0 = 0;
	  /*^clear */
	      /*clear *//*_#IS_CLOSURE__L10*/ meltfnum[9] = 0;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1806:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = /*_#OKRES__L4*/ meltfnum[3];;
    MELT_LOCATION ("warmelt-hooks.melt:1806:/ finalreturn");
    ;
    /*finalret */ goto meltlabend_rout;
    /*_#LET___L2*/ meltfnum[1] = /*_#RETURN___L21*/ meltfnum[18];;

    MELT_LOCATION ("warmelt-hooks.melt:1753:/ clear");
	    /*clear *//*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;
    /*^clear */
	    /*clear *//*_#OKRES__L4*/ meltfnum[3] = 0;
    /*^clear */
	    /*clear *//*_.PASSDICT__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.MYPASS__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_#PASSNUM__L5*/ meltfnum[4] = 0;
    /*^clear */
	    /*clear *//*_#NOT__L6*/ meltfnum[5] = 0;
    /*^clear */
	    /*clear *//*_#IF___L7*/ meltfnum[6] = 0;
    /*^clear */
	    /*clear *//*_#IS_A__L9*/ meltfnum[7] = 0;
    /*^clear */
	    /*clear *//*_#RETURN___L21*/ meltfnum[18] = 0;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:1750:/ clear");
	    /*clear *//*_#LET___L2*/ meltfnum[1] = 0;
  }
  ;
  /* epilogue of melthook_HOOK_RTL_EXECUTE hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_RTL_EXECUTE hook */
  melt_topframe = meltfram__.mcfr_prev;
  return
   /*_#RESHOOK___L1*/ meltfnum[0];
}				/* end of melthook_HOOK_RTL_EXECUTE hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_RTL_EXECUTEhook */
void
melthookmark_HOOK_RTL_EXECUTE_rout (struct melt_callframe_st *meltcfram,
				    int meltmarking)
{

  melthookframe_HOOK_RTL_EXECUTE_t *meltframptr_
    = (melthookframe_HOOK_RTL_EXECUTE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_RTL_EXECUTE good hash",
		  meltframptr_->mcfr_nbvar == -2110);
  if (meltmarking)
    {				/* marking hook HOOK_RTL_EXECUTE */
      int meltix;
      for (meltix = 0; meltix < 13; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_RTL_EXECUTE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 13; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_RTL_EXECUTE */
}				/* end  melthookmark_HOOK_RTL_EXECUTE_rout */

/* frame for melthook_HOOK_SIMPLE_IPA_GATE hook: */
typedef struct meltframe_melthook_HOOK_SIMPLE_IPA_GATE_st
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
#define MELTFRAM_NBVARPTR 13
  melt_ptr_t mcfr_varptr[13];
#define MELTFRAM_NBVARNUM 21
  long mcfr_varnum[21];
/*others*/
  const char *loc_CSTRING__o0;
  long _spare_;
}
melthookframe_HOOK_SIMPLE_IPA_GATE_t;	/* end of hook frame melthook_HOOK_SIMPLE_IPA_GATE */


/** implementation of hook melthook_HOOK_SIMPLE_IPA_GATE */
long
melthook_HOOK_SIMPLE_IPA_GATE (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_SIMPLE_IPA_GATE from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_SIMPLE_IPA_GATE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_SIMPLE_IPA_GATE;
  long melt_thishookcall_counter_HOOK_SIMPLE_IPA_GATE ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_SIMPLE_IPA_GATE++;
#define meltcallcount melt_thishookcall_counter_HOOK_SIMPLE_IPA_GATE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_SIMPLE_IPA_GATE_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_SIMPLE_IPA_GATE */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -729 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_SIMPLE_IPA_GATE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_SIMPLE_IPA_GATE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_SIMPLE_IPA_GATE hook */
  MELT_LOCATION ("warmelt-hooks.melt:1812:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1815:/ quasiblock");


  /*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;;
    /*^compute */
  /*_#OKRES__L4*/ meltfnum[3] = 0;;
    MELT_LOCATION ("warmelt-hooks.melt:1817:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~INITIAL_SYSTEM_DATA */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_SYSTEM_DATA */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~INITIAL_SYSTEM_DATA */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 9, "SYSDATA_PASS_DICT");
    /*_.PASSDICT__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.PASSDICT__V2*/ meltfptr[1] = NULL;;
      }
    ;
    /*^compute */
    /*_.MYPASS__V3*/ meltfptr[2] = ( /*nil */ NULL);;
    /*^compute */
  /*_#PASSNUM__L5*/ meltfnum[4] = 0;;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1821:/ locexp");
      /* hook_simple_ipa_gate GETHASMODE_CHK__5 */
      static const char *GETHASMODE_CHK__5modstr;
      if (!GETHASMODE_CHK__5modstr)
	GETHASMODE_CHK__5modstr = melt_argument ("mode");
		/*_#HASMODEFLAG__L3*/ meltfnum[2] = GETHASMODE_CHK__5modstr != NULL
	&& GETHASMODE_CHK__5modstr[0];
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#NOT__L6*/ meltfnum[5] =
      (!( /*_#HASMODEFLAG__L3*/ meltfnum[2]));;
    MELT_LOCATION ("warmelt-hooks.melt:1828:/ cond");
    /*cond */ if ( /*_#NOT__L6*/ meltfnum[5])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1829:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = 0;;
	  MELT_LOCATION ("warmelt-hooks.melt:1829:/ finalreturn");
	  ;
	  /*finalret */ goto meltlabend_rout;
	  /*_#IF___L7*/ meltfnum[6] = /*_#RETURN___L8*/ meltfnum[7];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:1828:/ clear");
	      /*clear *//*_#RETURN___L8*/ meltfnum[7] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

   /*_#IF___L7*/ meltfnum[6] = 0;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1830:/ locexp");
      /* hook_simple_ipa_gate GETMYPASS_CHK__5 */
      gcc_assert (current_pass != NULL);
      gcc_assert (current_pass->name != NULL);
      gcc_assert (current_pass->type == SIMPLE_IPA_PASS);
      /*_.MYPASS__V3*/ meltfptr[2] =
	melt_get_mapstrings ((struct meltmapstrings_st *) /*_.PASSDICT__V2*/
			     meltfptr[1],
			     current_pass->name);
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#IS_A__L9*/ meltfnum[7] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]),
			   (melt_ptr_t) (( /*~CLASS_GCC_SIMPLE_IPA_PASS */
					  meltfhook->tabval[2])));;
    MELT_LOCATION ("warmelt-hooks.melt:1839:/ cond");
    /*cond */ if ( /*_#IS_A__L9*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:1840:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.MYPASS__V3*/ meltfptr[2]),
					      (melt_ptr_t) (( /*~CLASS_GCC_PASS */ meltfhook->tabval[3])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj = (melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 2, "GCCPASS_GATE");
      /*_.GATEFUN__V4*/ meltfptr[3] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

     /*_.GATEFUN__V4*/ meltfptr[3] = NULL;;
	    }
	  ;
	  /*^compute */
    /*_?*/ meltfram__.loc_CSTRING__o0 =
	    (char *) 0;;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
    /*_#IS_CLOSURE__L12*/ meltfnum[11] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.GATEFUN__V4*/ meltfptr[3]))
	     == MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1843:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L12*/ meltfnum[11])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {




		{
		  MELT_LOCATION ("warmelt-hooks.melt:1844:/ locexp");
		  /* hook_simple_ipa_gate DOGATEFUN_CHK__3 */
		  FILE *oldf = meltgc_set_dump_file (dump_file);
	       /*_?*/ meltfram__.loc_CSTRING__o0 = current_pass->name;
	       /*_#PASSNUM__L5*/ meltfnum[4] =
		    current_pass->static_pass_number;
		  /*^block */
		  /*anyblock */
		  {


#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1851:/ cppif.then");
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
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1851:/ cond");
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
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1851:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[7];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L15*/ meltfnum[14];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1851;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_simple_ipa_gate curpassname=";
			      /*^apply.arg */
			      argtab[4].meltbp_cstring =
				/*_?*/ meltfram__.loc_CSTRING__o0;
			      /*^apply.arg */
			      argtab[5].meltbp_cstring = " mypass=";
			      /*^apply.arg */
			      argtab[6].meltbp_aptr =
				(melt_ptr_t *) & /*_.MYPASS__V3*/ meltfptr[2];
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_PTR ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V6*/ meltfptr[5] =
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1851:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L15*/ meltfnum[14] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V6*/ meltfptr[5] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1851:/ quasiblock");


		      /*_.PROGN___V8*/ meltfptr[6] =
			/*_.IF___V6*/ meltfptr[5];;
		      /*^compute */
		      /*_.IFCPP___V5*/ meltfptr[4] =
			/*_.PROGN___V8*/ meltfptr[6];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1851:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L14*/ meltfnum[13] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V6*/ meltfptr[5] = 0;
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

		    MELT_CHECK_SIGNAL ();
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:1852:/ apply");
		    /*apply */
		    {
		      union meltparam_un argtab[1];
		      memset (&argtab, 0, sizeof (argtab));
		      /*^apply.arg */
		      argtab[0].meltbp_long = /*_#PASSNUM__L5*/ meltfnum[4];
		      /*_.GATEFUN__V9*/ meltfptr[5] =
			melt_apply ((meltclosure_ptr_t)
				    ( /*_.GATEFUN__V4*/ meltfptr[3]),
				    (melt_ptr_t) ( /*_.MYPASS__V3*/
						  meltfptr[2]),
				    (MELTBPARSTR_LONG ""), argtab, "",
				    (union meltparam_un *) 0);
		    }
		    ;
		    /*^cond */
		    /*cond */ if ( /*_.GATEFUN__V9*/ meltfptr[5])	/*then */
		      {
			/*^cond.then */
			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-hooks.melt:1853:/ compute");
			  /*_#OKRES__L4*/ meltfnum[3] =
			    /*_#SETQ___L17*/ meltfnum[13] = 1;;
			  /*_#IF___L16*/ meltfnum[14] =
			    /*_#SETQ___L17*/ meltfnum[13];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:1852:/ clear");
		    /*clear *//*_#SETQ___L17*/ meltfnum[13] = 0;
			}
			;
		      }
		    else
		      {		/*^cond.else */

	 /*_#IF___L16*/ meltfnum[14] = 0;;
		      }
		    ;

#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1854:/ cppif.then");
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
	  /*_#MELT_NEED_DBG__L18*/ meltfnum[13] =
			/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1854:/ cond");
		      /*cond */ if ( /*_#MELT_NEED_DBG__L18*/ meltfnum[13])	/*then */
			{
			  /*^cond.then */
			  /*^block */
			  /*anyblock */
			  {

	    /*_#MELT_CALLCOUNT__L19*/ meltfnum[18] =
			      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			      meltcallcount	/* melt_callcount debugging */
#else
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1854:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[5];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1854;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_simple_ipa_gate after call okres=";
			      /*^apply.arg */
			      argtab[4].meltbp_long =
				/*_#OKRES__L4*/ meltfnum[3];
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V11*/ meltfptr[10] =
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1854:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L19*/ meltfnum[18] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V11*/ meltfptr[10] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1854:/ quasiblock");


		      /*_.PROGN___V13*/ meltfptr[11] =
			/*_.IF___V11*/ meltfptr[10];;
		      /*^compute */
		      /*_.IFCPP___V10*/ meltfptr[6] =
			/*_.PROGN___V13*/ meltfptr[11];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1854:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L18*/ meltfnum[13] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V11*/ meltfptr[10] = 0;
		      /*^clear */
		    /*clear *//*_.PROGN___V13*/ meltfptr[11] = 0;
		    }

#else /*MELT_HAVE_DEBUG */
		    /*^cppif.else */
		    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		    ;

		    {
		      MELT_LOCATION ("warmelt-hooks.melt:1855:/ locexp");
		      /*void */ (void) 0;
		    }
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:1850:/ quasiblock");


		    /*epilog */

		    MELT_LOCATION ("warmelt-hooks.melt:1844:/ clear");
		  /*clear *//*_.IFCPP___V5*/ meltfptr[4] = 0;
		    /*^clear */
		  /*clear *//*_.GATEFUN__V9*/ meltfptr[5] = 0;
		    /*^clear */
		  /*clear *//*_#IF___L16*/ meltfnum[14] = 0;
		    /*^clear */
		  /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
		  }

		  meltgc_restore_dump_file (oldf);
		  oldf = NULL;
		  ;
		}
		;
		/*clear *//*_#IFELSE___L13*/ meltfnum[12] = 0;
		/*epilog */
	      }
	      ;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-hooks.melt:1843:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-hooks.melt:1860:/ compute");
		/*_#OKRES__L4*/ meltfnum[3] = /*_#SETQ___L20*/ meltfnum[18] =
		  1;;
		/*_#IFELSE___L13*/ meltfnum[12] =
		  /*_#SETQ___L20*/ meltfnum[18];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:1843:/ clear");
		/*clear *//*_#SETQ___L20*/ meltfnum[18] = 0;
	      }
	      ;
	    }
	  ;
	  /*_#LET___L11*/ meltfnum[10] = /*_#IFELSE___L13*/ meltfnum[12];;

	  MELT_LOCATION ("warmelt-hooks.melt:1840:/ clear");
	      /*clear *//*_.GATEFUN__V4*/ meltfptr[3] = 0;
	  /*^clear */
	      /*clear *//*_?*/ meltfram__.loc_CSTRING__o0 = 0;
	  /*^clear */
	      /*clear *//*_#IS_CLOSURE__L12*/ meltfnum[11] = 0;
	  /*^clear */
	      /*clear *//*_#IFELSE___L13*/ meltfnum[12] = 0;
	  /*_#IF___L10*/ meltfnum[9] = /*_#LET___L11*/ meltfnum[10];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:1839:/ clear");
	      /*clear *//*_#LET___L11*/ meltfnum[10] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

   /*_#IF___L10*/ meltfnum[9] = 0;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1862:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = /*_#OKRES__L4*/ meltfnum[3];;
    MELT_LOCATION ("warmelt-hooks.melt:1862:/ finalreturn");
    ;
    /*finalret */ goto meltlabend_rout;
    /*_#LET___L2*/ meltfnum[1] = /*_#RETURN___L21*/ meltfnum[13];;

    MELT_LOCATION ("warmelt-hooks.melt:1815:/ clear");
	    /*clear *//*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;
    /*^clear */
	    /*clear *//*_#OKRES__L4*/ meltfnum[3] = 0;
    /*^clear */
	    /*clear *//*_.PASSDICT__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.MYPASS__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_#PASSNUM__L5*/ meltfnum[4] = 0;
    /*^clear */
	    /*clear *//*_#NOT__L6*/ meltfnum[5] = 0;
    /*^clear */
	    /*clear *//*_#IF___L7*/ meltfnum[6] = 0;
    /*^clear */
	    /*clear *//*_#IS_A__L9*/ meltfnum[7] = 0;
    /*^clear */
	    /*clear *//*_#IF___L10*/ meltfnum[9] = 0;
    /*^clear */
	    /*clear *//*_#RETURN___L21*/ meltfnum[13] = 0;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:1812:/ clear");
	    /*clear *//*_#LET___L2*/ meltfnum[1] = 0;
  }
  ;
  /* epilogue of melthook_HOOK_SIMPLE_IPA_GATE hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_SIMPLE_IPA_GATE hook */
  melt_topframe = meltfram__.mcfr_prev;
  return
   /*_#RESHOOK___L1*/ meltfnum[0];
}				/* end of melthook_HOOK_SIMPLE_IPA_GATE hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_SIMPLE_IPA_GATEhook */
void
melthookmark_HOOK_SIMPLE_IPA_GATE_rout (struct melt_callframe_st *meltcfram,
					int meltmarking)
{

  melthookframe_HOOK_SIMPLE_IPA_GATE_t *meltframptr_
    = (melthookframe_HOOK_SIMPLE_IPA_GATE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_SIMPLE_IPA_GATE good hash",
		  meltframptr_->mcfr_nbvar == -729);
  if (meltmarking)
    {				/* marking hook HOOK_SIMPLE_IPA_GATE */
      int meltix;
      for (meltix = 0; meltix < 13; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_SIMPLE_IPA_GATE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 13; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_SIMPLE_IPA_GATE */
}				/* end  melthookmark_HOOK_SIMPLE_IPA_GATE_rout */

/* frame for melthook_HOOK_SIMPLE_IPA_EXECUTE hook: */
typedef struct meltframe_melthook_HOOK_SIMPLE_IPA_EXECUTE_st
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
#define MELTFRAM_NBVARPTR 13
  melt_ptr_t mcfr_varptr[13];
#define MELTFRAM_NBVARNUM 21
  long mcfr_varnum[21];
/*others*/
  const char *loc_CSTRING__o0;
  long _spare_;
}
melthookframe_HOOK_SIMPLE_IPA_EXECUTE_t;	/* end of hook frame melthook_HOOK_SIMPLE_IPA_EXECUTE */


/** implementation of hook melthook_HOOK_SIMPLE_IPA_EXECUTE */
long
melthook_HOOK_SIMPLE_IPA_EXECUTE (melt_ptr_t melthookdata)
{				/* body of hook melthook_HOOK_SIMPLE_IPA_EXECUTE from outpucod_hookroutine */
/* declare and initialize frame for melthook_HOOK_SIMPLE_IPA_EXECUTE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_SIMPLE_IPA_EXECUTE;
  long melt_thishookcall_counter_HOOK_SIMPLE_IPA_EXECUTE ATTRIBUTE_UNUSED
    = melthook_call_counter_HOOK_SIMPLE_IPA_EXECUTE++;
#define meltcallcount melt_thishookcall_counter_HOOK_SIMPLE_IPA_EXECUTE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_SIMPLE_IPA_EXECUTE_t meltfram__;
  /* clear and push the new call frame for hook melthook_HOOK_SIMPLE_IPA_EXECUTE */
  memset (&meltfram__, 0, sizeof (meltfram__));
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -3863 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_SIMPLE_IPA_EXECUTE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
#define meltfhook /*hook data of HOOK_SIMPLE_IPA_EXECUTE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_SIMPLE_IPA_EXECUTE hook */
  MELT_LOCATION ("warmelt-hooks.melt:1867:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^block */
  /*anyblock */
  {

    MELT_LOCATION ("warmelt-hooks.melt:1870:/ quasiblock");


  /*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;;
    /*^compute */
  /*_#OKRES__L4*/ meltfnum[3] = 0;;
    MELT_LOCATION ("warmelt-hooks.melt:1872:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*~INITIAL_SYSTEM_DATA */
					  meltfhook->tabval[0])),
					(melt_ptr_t) (( /*~CLASS_SYSTEM_DATA */ meltfhook->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*~INITIAL_SYSTEM_DATA */ meltfhook->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 9, "SYSDATA_PASS_DICT");
    /*_.PASSDICT__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

   /*_.PASSDICT__V2*/ meltfptr[1] = NULL;;
      }
    ;
    /*^compute */
    /*_.MYPASS__V3*/ meltfptr[2] = ( /*nil */ NULL);;
    /*^compute */
  /*_#PASSNUM__L5*/ meltfnum[4] = 0;;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1876:/ locexp");
      /* hook_simple_ipa_execute GETHASMODE_CHK__6 */
      static const char *GETHASMODE_CHK__6modstr;
      if (!GETHASMODE_CHK__6modstr)
	GETHASMODE_CHK__6modstr = melt_argument ("mode");
		/*_#HASMODEFLAG__L3*/ meltfnum[2] = GETHASMODE_CHK__6modstr != NULL
	&& GETHASMODE_CHK__6modstr[0];
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#NOT__L6*/ meltfnum[5] =
      (!( /*_#HASMODEFLAG__L3*/ meltfnum[2]));;
    MELT_LOCATION ("warmelt-hooks.melt:1883:/ cond");
    /*cond */ if ( /*_#NOT__L6*/ meltfnum[5])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:1884:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = 0;;
	  MELT_LOCATION ("warmelt-hooks.melt:1884:/ finalreturn");
	  ;
	  /*finalret */ goto meltlabend_rout;
	  /*_#IF___L7*/ meltfnum[6] = /*_#RETURN___L8*/ meltfnum[7];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:1883:/ clear");
	      /*clear *//*_#RETURN___L8*/ meltfnum[7] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

   /*_#IF___L7*/ meltfnum[6] = 0;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-hooks.melt:1885:/ locexp");
      /* hook_simple_ipa_execute GETMYPASS_CHK__6 */
      gcc_assert (current_pass != NULL);
      gcc_assert (current_pass->name != NULL);
      gcc_assert (current_pass->type == SIMPLE_IPA_PASS);
      /*_.MYPASS__V3*/ meltfptr[2] =
	melt_get_mapstrings ((struct meltmapstrings_st *) /*_.PASSDICT__V2*/
			     meltfptr[1],
			     current_pass->name);
      ;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
  /*_#IS_A__L9*/ meltfnum[7] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]),
			   (melt_ptr_t) (( /*~CLASS_GCC_SIMPLE_IPA_PASS */
					  meltfhook->tabval[2])));;
    MELT_LOCATION ("warmelt-hooks.melt:1894:/ cond");
    /*cond */ if ( /*_#IS_A__L9*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:1895:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.MYPASS__V3*/ meltfptr[2]),
					      (melt_ptr_t) (( /*~CLASS_GCC_PASS */ meltfhook->tabval[3])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj = (melt_ptr_t) ( /*_.MYPASS__V3*/ meltfptr[2]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "GCCPASS_EXEC");
      /*_.EXECFUN__V4*/ meltfptr[3] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

     /*_.EXECFUN__V4*/ meltfptr[3] = NULL;;
	    }
	  ;
	  /*^compute */
    /*_?*/ meltfram__.loc_CSTRING__o0 =
	    (char *) 0;;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
    /*_#IS_CLOSURE__L10*/ meltfnum[9] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.EXECFUN__V4*/ meltfptr[3]))
	     == MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-hooks.melt:1898:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L10*/ meltfnum[9])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-hooks.melt:1899:/ locexp");
		  /* hook_simple_ipa_execute DOEXECFUN_CHK__3 */
	       /*_#PASSNUM__L5*/ meltfnum[4] =
		    current_pass->static_pass_number;
		  FILE *oldf = meltgc_set_dump_file (dump_file);
	       /*_?*/ meltfram__.loc_CSTRING__o0 = current_pass->name;
		  /*^block */
		  /*anyblock */
		  {


#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1906:/ cppif.then");
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
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1906:/ cond");
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
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1906:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[9];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1906;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_simple_ipa_execute curpassname=";
			      /*^apply.arg */
			      argtab[4].meltbp_cstring =
				/*_?*/ meltfram__.loc_CSTRING__o0;
			      /*^apply.arg */
			      argtab[5].meltbp_cstring = " mypass=";
			      /*^apply.arg */
			      argtab[6].meltbp_aptr =
				(melt_ptr_t *) & /*_.MYPASS__V3*/ meltfptr[2];
			      /*^apply.arg */
			      argtab[7].meltbp_cstring = " passnum=";
			      /*^apply.arg */
			      argtab[8].meltbp_long =
				/*_#PASSNUM__L5*/ meltfnum[4];
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_PTR
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V6*/ meltfptr[5] =
			      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1906:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L12*/ meltfnum[11] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V6*/ meltfptr[5] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1906:/ quasiblock");


		      /*_.PROGN___V8*/ meltfptr[6] =
			/*_.IF___V6*/ meltfptr[5];;
		      /*^compute */
		      /*_.IFCPP___V5*/ meltfptr[4] =
			/*_.PROGN___V8*/ meltfptr[6];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1906:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L11*/ meltfnum[10] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V6*/ meltfptr[5] = 0;
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
		    MELT_LOCATION ("warmelt-hooks.melt:1908:/ quasiblock");


		    /*^multiapply */
		    /*multiapply 2args, 1x.res */
		    {
		      union meltparam_un argtab[1];

		      union meltparam_un restab[1];
		      memset (&restab, 0, sizeof (restab));
		      memset (&argtab, 0, sizeof (argtab));
		      /*^multiapply.arg */
		      argtab[0].meltbp_long = /*_#PASSNUM__L5*/ meltfnum[4];
		      /*^multiapply.xres */
		      restab[0].meltbp_longptr =
			& /*_#FLAGEXEC__L14*/ meltfnum[10];
		      /*^multiapply.appl */
		      /*_.RESEXEC__V9*/ meltfptr[5] =
			melt_apply ((meltclosure_ptr_t)
				    ( /*_.EXECFUN__V4*/ meltfptr[3]),
				    (melt_ptr_t) ( /*_.MYPASS__V3*/
						  meltfptr[2]),
				    (MELTBPARSTR_LONG ""), argtab,
				    (MELTBPARSTR_LONG ""), restab);
		    }
		    ;
		    /*^quasiblock */



#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-hooks.melt:1911:/ cppif.then");
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
	  /*_#MELT_NEED_DBG__L15*/ meltfnum[14] =
			/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
			( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
			0	/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
			;;
		      MELT_LOCATION ("warmelt-hooks.melt:1911:/ cond");
		      /*cond */ if ( /*_#MELT_NEED_DBG__L15*/ meltfnum[14])	/*then */
			{
			  /*^cond.then */
			  /*^block */
			  /*anyblock */
			  {

	    /*_#MELT_CALLCOUNT__L16*/ meltfnum[15] =
			      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			      meltcallcount	/* melt_callcount debugging */
#else
			      0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			      ;;

			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION ("warmelt-hooks.melt:1911:/ apply");
			    /*apply */
			    {
			      union meltparam_un argtab[7];
			      memset (&argtab, 0, sizeof (argtab));
			      /*^apply.arg */
			      argtab[0].meltbp_long =
				/*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
			      /*^apply.arg */
			      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
			      /*^apply.arg */
			      argtab[2].meltbp_long = 1911;
			      /*^apply.arg */
			      argtab[3].meltbp_cstring =
				"hook_simple_ipa_execute after call resexec=";
			      /*^apply.arg */
			      argtab[4].meltbp_aptr =
				(melt_ptr_t *) & /*_.RESEXEC__V9*/
				meltfptr[5];
			      /*^apply.arg */
			      argtab[5].meltbp_cstring = " flagexec=";
			      /*^apply.arg */
			      argtab[6].meltbp_long =
				/*_#FLAGEXEC__L14*/ meltfnum[10];
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
				melt_apply ((meltclosure_ptr_t)
					    (( /*~MELT_DEBUG_FUN */
					      meltfhook->tabval[4])),
					    (melt_ptr_t) (( /*nil */ NULL)),
					    (MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_PTR
					     MELTBPARSTR_CSTRING
					     MELTBPARSTR_LONG ""), argtab, "",
					    (union meltparam_un *) 0);
			    }
			    ;
			    /*_.IF___V11*/ meltfptr[10] =
			      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-hooks.melt:1911:/ clear");
		      /*clear *//*_#MELT_CALLCOUNT__L16*/ meltfnum[15] =
			      0;
			    /*^clear */
		      /*clear *//*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =
			      0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

	   /*_.IF___V11*/ meltfptr[10] = NULL;;
			}
		      ;
		      MELT_LOCATION ("warmelt-hooks.melt:1911:/ quasiblock");


		      /*_.PROGN___V13*/ meltfptr[11] =
			/*_.IF___V11*/ meltfptr[10];;
		      /*^compute */
		      /*_.IFCPP___V10*/ meltfptr[6] =
			/*_.PROGN___V13*/ meltfptr[11];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-hooks.melt:1911:/ clear");
		    /*clear *//*_#MELT_NEED_DBG__L15*/ meltfnum[14] = 0;
		      /*^clear */
		    /*clear *//*_.IF___V11*/ meltfptr[10] = 0;
		      /*^clear */
		    /*clear *//*_.PROGN___V13*/ meltfptr[11] = 0;
		    }

#else /*MELT_HAVE_DEBUG */
		    /*^cppif.else */
		    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		    ;

		    MELT_CHECK_SIGNAL ();
		    ;
		    /*^compute */
	/*_#NULL__L17*/ meltfnum[15] =
		      (( /*_.RESEXEC__V9*/ meltfptr[5]) == NULL);;
		    MELT_LOCATION ("warmelt-hooks.melt:1913:/ cond");
		    /*cond */ if ( /*_#NULL__L17*/ meltfnum[15])	/*then */
		      {
			/*^cond.then */
			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-hooks.melt:1914:/ compute");
			  /*_#OKRES__L4*/ meltfnum[3] =
			    /*_#SETQ___L19*/ meltfnum[18] =
			    /*_#FLAGEXEC__L14*/ meltfnum[10];;
			  /*_#IFELSE___L18*/ meltfnum[14] =
			    /*_#SETQ___L19*/ meltfnum[18];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:1913:/ clear");
		    /*clear *//*_#SETQ___L19*/ meltfnum[18] = 0;
			}
			;
		      }
		    else
		      {		/*^cond.else */

			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-hooks.melt:1915:/ compute");
			  /*_#OKRES__L4*/ meltfnum[3] =
			    /*_#SETQ___L20*/ meltfnum[18] = 1;;
			  /*_#IFELSE___L18*/ meltfnum[14] =
			    /*_#SETQ___L20*/ meltfnum[18];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-hooks.melt:1913:/ clear");
		    /*clear *//*_#SETQ___L20*/ meltfnum[18] = 0;
			}
			;
		      }
		    ;

		    MELT_CHECK_SIGNAL ();
		    ;
		    /*_#MULTI___L13*/ meltfnum[11] =
		      /*_#IFELSE___L18*/ meltfnum[14];;

		    MELT_LOCATION ("warmelt-hooks.melt:1908:/ clear");
		  /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
		    /*^clear */
		  /*clear *//*_#NULL__L17*/ meltfnum[15] = 0;
		    /*^clear */
		  /*clear *//*_#IFELSE___L18*/ meltfnum[14] = 0;

		    /*^clear */
		  /*clear *//*_#FLAGEXEC__L14*/ meltfnum[10] = 0;

		    {
		      MELT_LOCATION ("warmelt-hooks.melt:1917:/ locexp");
		      /*void */ (void) 0;
		    }
		    ;
		    MELT_LOCATION ("warmelt-hooks.melt:1905:/ quasiblock");


		    /*epilog */

		    MELT_LOCATION ("warmelt-hooks.melt:1899:/ clear");
		  /*clear *//*_.IFCPP___V5*/ meltfptr[4] = 0;
		    /*^clear */
		  /*clear *//*_#MULTI___L13*/ meltfnum[11] = 0;
		  }

		  meltgc_restore_dump_file (oldf);
		  oldf = NULL;
		  ;
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:1895:/ clear");
	      /*clear *//*_.EXECFUN__V4*/ meltfptr[3] = 0;
	  /*^clear */
	      /*clear *//*_?*/ meltfram__.loc_CSTRING__o0 = 0;
	  /*^clear */
	      /*clear *//*_#IS_CLOSURE__L10*/ meltfnum[9] = 0;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:1923:/ quasiblock");


    /*_#RESHOOK___L1*/ meltfnum[0] = /*_#OKRES__L4*/ meltfnum[3];;
    MELT_LOCATION ("warmelt-hooks.melt:1923:/ finalreturn");
    ;
    /*finalret */ goto meltlabend_rout;
    /*_#LET___L2*/ meltfnum[1] = /*_#RETURN___L21*/ meltfnum[18];;

    MELT_LOCATION ("warmelt-hooks.melt:1870:/ clear");
	    /*clear *//*_#HASMODEFLAG__L3*/ meltfnum[2] = 0;
    /*^clear */
	    /*clear *//*_#OKRES__L4*/ meltfnum[3] = 0;
    /*^clear */
	    /*clear *//*_.PASSDICT__V2*/ meltfptr[1] = 0;
    /*^clear */
	    /*clear *//*_.MYPASS__V3*/ meltfptr[2] = 0;
    /*^clear */
	    /*clear *//*_#PASSNUM__L5*/ meltfnum[4] = 0;
    /*^clear */
	    /*clear *//*_#NOT__L6*/ meltfnum[5] = 0;
    /*^clear */
	    /*clear *//*_#IF___L7*/ meltfnum[6] = 0;
    /*^clear */
	    /*clear *//*_#IS_A__L9*/ meltfnum[7] = 0;
    /*^clear */
	    /*clear *//*_#RETURN___L21*/ meltfnum[18] = 0;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:1867:/ clear");
	    /*clear *//*_#LET___L2*/ meltfnum[1] = 0;
  }
  ;
  /* epilogue of melthook_HOOK_SIMPLE_IPA_EXECUTE hook */
  goto meltlabend_rout;
meltlabend_rout:;

  /* final of melthook_HOOK_SIMPLE_IPA_EXECUTE hook */
  melt_topframe = meltfram__.mcfr_prev;
  return
   /*_#RESHOOK___L1*/ meltfnum[0];
}				/* end of melthook_HOOK_SIMPLE_IPA_EXECUTE hook */

#undef meltframe
#undef meltfram__
#undef meltfhook
#undef meltcallcount

/* define frame moving or marking routine for melthook_HOOK_SIMPLE_IPA_EXECUTEhook */
void
melthookmark_HOOK_SIMPLE_IPA_EXECUTE_rout (struct melt_callframe_st
					   *meltcfram, int meltmarking)
{

  melthookframe_HOOK_SIMPLE_IPA_EXECUTE_t *meltframptr_
    = (melthookframe_HOOK_SIMPLE_IPA_EXECUTE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_SIMPLE_IPA_EXECUTE good hash",
		  meltframptr_->mcfr_nbvar == -3863);
  if (meltmarking)
    {				/* marking hook HOOK_SIMPLE_IPA_EXECUTE */
      int meltix;
      for (meltix = 0; meltix < 13; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_SIMPLE_IPA_EXECUTE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 13; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_SIMPLE_IPA_EXECUTE */
}				/* end  melthookmark_HOOK_SIMPLE_IPA_EXECUTE_rout */



/*no syntax testing generated*/

/* the frame for initial routine */
struct melt_initial_frame_st
{
  int mcfr_nbvar;
#if MELT_HAVE_DEBUG
  const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
  const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
  void (*mcfr_initforwmarkrout) (struct melt_callframe_st *, int);
  struct excepth_melt_st *mcfr_exh;
  struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 363
  melt_ptr_t mcfr_varptr[363];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
  long _spare_;
}
 ;


static void
initialize_module_meltdata_warmelt_hooks (meltinitial_frame_t * iniframp__,
					  char meltpredefinited[])
{
#define meltfram__  (*iniframp__)

  /* curframe_cdat_struct */ struct meltcdata_st
  {
    void *meltcdata_inigap[2];	/* could help debugging */

    struct MELT_HOOK_STRUCT (3) dhook__1__HOOK_LOW_DEBUG_VALUE_AT;
    struct MELT_OBJECT_STRUCT (6) dobj_2__;
    struct MELT_OBJECT_STRUCT (3) dsym_3__VAL;
    struct MELT_STRING_STRUCT (3) dstr_4__VAL;
    struct MELT_OBJECT_STRUCT (2) dobj_5;
    struct MELT_OBJECT_STRUCT (3) dsym_6__FILENAME;
    struct MELT_STRING_STRUCT (8) dstr_7__FILENAME;
    struct MELT_OBJECT_STRUCT (2) dobj_8;
    struct MELT_OBJECT_STRUCT (3) dsym_9__LINENO;
    struct MELT_STRING_STRUCT (6) dstr_10__LINENO;
    struct MELT_OBJECT_STRUCT (2) dobj_11;
    struct MELT_OBJECT_STRUCT (3) dsym_12__MSG;
    struct MELT_STRING_STRUCT (3) dstr_13__MSG;
    struct MELT_OBJECT_STRUCT (2) dobj_14;
    struct MELT_OBJECT_STRUCT (3) dsym_15__COUNT;
    struct MELT_STRING_STRUCT (5) dstr_16__COUNT;
    struct MELT_OBJECT_STRUCT (2) dobj_17;
    struct MELT_MULTIPLE_STRUCT (5) dtup_18___HOOK_LOW_DEBUG_VALUE_AT;
    struct MELT_MULTIPLE_STRUCT (0) dtup_19___HOOK_LOW_DEBUG_VALUE_AT;
    struct MELT_OBJECT_STRUCT (3) dsym_20__MELT_DEBUG_FUN;
    struct MELT_STRING_STRUCT (14) dstr_21__MELT_DEBUG_FUN;
    struct MELT_OBJECT_STRUCT (3) dsym_22__OVERRIDE_GATE_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (27) dstr_23__OVERRIDE_GATE_DE;
    struct MELT_OBJECT_STRUCT (3) dsym_24__CLASS_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (19) dstr_25__CLASS_DELAYED_QU;
    struct MELT_OBJECT_STRUCT (5) dobj_26__OVERRIDE_GATE_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (27) dstr_27__override_gate_de;
    struct MELT_OBJECT_STRUCT (3) dsym_28__DISCR_LIST;
    struct MELT_STRING_STRUCT (10) dstr_29__DISCR_LIST;
    struct MELT_HOOK_STRUCT (9) dhook__30__HOOK_OVERRIDE_GATE;
    struct MELT_OBJECT_STRUCT (6) dobj_31__;
    struct MELT_OBJECT_STRUCT (3) dsym_32__BEFOREGATE;
    struct MELT_STRING_STRUCT (10) dstr_33__BEFOREGATE;
    struct MELT_OBJECT_STRUCT (2) dobj_34;
    struct MELT_MULTIPLE_STRUCT (1) dtup_35___HOOK_OVERRIDE_GATE;
    struct MELT_OBJECT_STRUCT (3) dsym_36__AFTERGATE;
    struct MELT_STRING_STRUCT (9) dstr_37__AFTERGATE;
    struct MELT_OBJECT_STRUCT (2) dobj_38;
    struct MELT_MULTIPLE_STRUCT (1) dtup_39___HOOK_OVERRIDE_GATE;
    struct MELT_OBJECT_STRUCT (3) dsym_40__TRUE;
    struct MELT_STRING_STRUCT (4) dstr_41__TRUE;
    struct MELT_OBJECT_STRUCT (3) dsym_42__CLASS_REFERENCE;
    struct MELT_STRING_STRUCT (15) dstr_43__CLASS_REFERENCE;
    struct MELT_ROUTINE_STRUCT (0) drout_44__ENABLE_OVERRIDE_GATE;
    struct MELT_CLOSURE_STRUCT (0) dclo_45__ENABLE_OVERRIDE_GATE;
    struct MELT_ROUTINE_STRUCT (2) drout_46__MAYBE_DISABLE_OVERRIDE_GATE;
    struct MELT_CLOSURE_STRUCT (0) dclo_47__MAYBE_DISABLE_OVERRIDE_GATE;
    struct MELT_ROUTINE_STRUCT (4) drout_48__REGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_49__REGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_ROUTINE_STRUCT (4) drout_50__REGISTER_OVERRIDE_GATE_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_51__REGISTER_OVERRIDE_GATE_LAST;
    struct MELT_ROUTINE_STRUCT (5) drout_52__UNREGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_53__UNREGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_ROUTINE_STRUCT (5) drout_54__UNREGISTER_OVERRIDE_GATE_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_55__UNREGISTER_OVERRIDE_GATE_LAST;
    struct MELT_OBJECT_STRUCT (3) dsym_56__START_UNIT_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (24) dstr_57__START_UNIT_DELAY;
    struct MELT_OBJECT_STRUCT (5) dobj_58__START_UNIT_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (24) dstr_59__start_unit_delay;
    struct MELT_HOOK_STRUCT (9) dhook__60__HOOK_START_UNIT;
    struct MELT_OBJECT_STRUCT (6) dobj_61__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_62___HOOK_START_UNIT;
    struct MELT_MULTIPLE_STRUCT (0) dtup_63___HOOK_START_UNIT;
    struct MELT_OBJECT_STRUCT (3) dsym_64__LIST_EVERY;
    struct MELT_STRING_STRUCT (10) dstr_65__LIST_EVERY;
    struct MELT_ROUTINE_STRUCT (1) drout_66__LAMBDA_;
    struct MELT_ROUTINE_STRUCT (0) drout_67__LAMBDA_;
    struct MELT_ROUTINE_STRUCT (1) drout_68__LAMBDA_;
    struct MELT_ROUTINE_STRUCT (0) drout_69__ENABLE_START_UNIT;
    struct MELT_CLOSURE_STRUCT (0) dclo_70__ENABLE_START_UNIT;
    struct MELT_ROUTINE_STRUCT (2) drout_71__AT_START_UNIT_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_72__AT_START_UNIT_FIRST;
    struct MELT_ROUTINE_STRUCT (2) drout_73__AT_START_UNIT_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_74__AT_START_UNIT_LAST;
    struct MELT_OBJECT_STRUCT (3) dsym_75__FINISH_UNIT_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (25) dstr_76__FINISH_UNIT_DELA;
    struct MELT_OBJECT_STRUCT (5) dobj_77__FINISH_UNIT_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (25) dstr_78__finish_unit_dela;
    struct MELT_HOOK_STRUCT (7) dhook__79__HOOK_FINISH_UNIT;
    struct MELT_OBJECT_STRUCT (6) dobj_80__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_81___HOOK_FINISH_UNIT;
    struct MELT_MULTIPLE_STRUCT (0) dtup_82___HOOK_FINISH_UNIT;
    struct MELT_ROUTINE_STRUCT (1) drout_83__LAMBDA_;
    struct MELT_ROUTINE_STRUCT (0) drout_84__LAMBDA_;
    struct MELT_ROUTINE_STRUCT (1) drout_85__LAMBDA_;
    struct MELT_ROUTINE_STRUCT (0) drout_86__ENABLE_FINISH_UNIT;
    struct MELT_CLOSURE_STRUCT (0) dclo_87__ENABLE_FINISH_UNIT;
    struct MELT_ROUTINE_STRUCT (2) drout_88__AT_FINISH_UNIT_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_89__AT_FINISH_UNIT_FIRST;
    struct MELT_ROUTINE_STRUCT (2) drout_90__AT_FINISH_UNIT_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_91__AT_FINISH_UNIT_LAST;
    struct MELT_OBJECT_STRUCT (3) dsym_92__ALL_PASSES_START_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (30) dstr_93__ALL_PASSES_START;
    struct MELT_OBJECT_STRUCT (5) dobj_94__ALL_PASSES_START_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (30) dstr_95__all_passes_start;
    struct MELT_HOOK_STRUCT (3) dhook__96__HOOK_ALL_PASSES_START;
    struct MELT_OBJECT_STRUCT (6) dobj_97__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_98___HOOK_ALL_PASSES_START;
    struct MELT_MULTIPLE_STRUCT (0) dtup_99___HOOK_ALL_PASSES_START;
    struct MELT_ROUTINE_STRUCT (0) drout_100__ENABLE_ALL_PASSES_START;
    struct MELT_CLOSURE_STRUCT (0) dclo_101__ENABLE_ALL_PASSES_START;
    struct MELT_ROUTINE_STRUCT (4) drout_102__REGISTER_ALL_PASSES_START_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_103__REGISTER_ALL_PASSES_START_FIRST;
    struct MELT_ROUTINE_STRUCT (4) drout_104__REGISTER_ALL_PASSES_START_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_105__REGISTER_ALL_PASSES_START_LAST;
    struct MELT_OBJECT_STRUCT (3) dsym_106__ALL_PASSES_END_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (28) dstr_107__ALL_PASSES_END_D;
    struct MELT_OBJECT_STRUCT (5) dobj_108__ALL_PASSES_END_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (28) dstr_109__all_passes_end_d;
    struct MELT_HOOK_STRUCT (3) dhook__110__HOOK_ALL_PASSES_END;
    struct MELT_OBJECT_STRUCT (6) dobj_111__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_112___HOOK_ALL_PASSES_END;
    struct MELT_MULTIPLE_STRUCT (0) dtup_113___HOOK_ALL_PASSES_END;
    struct MELT_ROUTINE_STRUCT (0) drout_114__ENABLE_ALL_PASSES_END;
    struct MELT_CLOSURE_STRUCT (0) dclo_115__ENABLE_ALL_PASSES_END;
    struct MELT_ROUTINE_STRUCT (4) drout_116__REGISTER_ALL_PASSES_END_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_117__REGISTER_ALL_PASSES_END_FIRST;
    struct MELT_ROUTINE_STRUCT (4) drout_118__REGISTER_ALL_PASSES_END_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_119__REGISTER_ALL_PASSES_END_LAST;
    struct MELT_OBJECT_STRUCT (3)
      dsym_120__ALL_IPA_PASSES_START_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (34) dstr_121__ALL_IPA_PASSES_S;
    struct MELT_OBJECT_STRUCT (5)
      dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (34) dstr_123__all_ipa_passes_s;
    struct MELT_HOOK_STRUCT (3) dhook__124__HOOK_ALL_IPA_PASSES_START;
    struct MELT_OBJECT_STRUCT (6) dobj_125__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_126___HOOK_ALL_IPA_PASSES_START;
    struct MELT_MULTIPLE_STRUCT (0) dtup_127___HOOK_ALL_IPA_PASSES_START;
    struct MELT_ROUTINE_STRUCT (0) drout_128__ENABLE_ALL_IPA_PASSES_START;
    struct MELT_CLOSURE_STRUCT (0) dclo_129__ENABLE_ALL_IPA_PASSES_START;
    struct MELT_ROUTINE_STRUCT (4)
      drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST;
    struct MELT_CLOSURE_STRUCT (0)
      dclo_131__REGISTER_ALL_IPA_PASSES_START_FIRST;
    struct MELT_ROUTINE_STRUCT (4)
      drout_132__REGISTER_ALL_IPA_PASSES_START_LAST;
    struct MELT_CLOSURE_STRUCT (0)
      dclo_133__REGISTER_ALL_IPA_PASSES_START_LAST;
    struct MELT_OBJECT_STRUCT (3) dsym_134__ALL_IPA_PASSES_END_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (32) dstr_135__ALL_IPA_PASSES_E;
    struct MELT_OBJECT_STRUCT (5) dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (32) dstr_137__all_ipa_passes_e;
    struct MELT_HOOK_STRUCT (3) dhook__138__HOOK_ALL_IPA_PASSES_END;
    struct MELT_OBJECT_STRUCT (6) dobj_139__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_140___HOOK_ALL_IPA_PASSES_END;
    struct MELT_MULTIPLE_STRUCT (0) dtup_141___HOOK_ALL_IPA_PASSES_END;
    struct MELT_ROUTINE_STRUCT (0) drout_142__ENABLE_ALL_IPA_PASSES_END;
    struct MELT_CLOSURE_STRUCT (0) dclo_143__ENABLE_ALL_IPA_PASSES_END;
    struct MELT_ROUTINE_STRUCT (4)
      drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST;
    struct MELT_CLOSURE_STRUCT (0)
      dclo_145__REGISTER_ALL_IPA_PASSES_END_FIRST;
    struct MELT_ROUTINE_STRUCT (4)
      drout_146__REGISTER_ALL_IPA_PASSES_END_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_147__REGISTER_ALL_IPA_PASSES_END_LAST;
    struct MELT_OBJECT_STRUCT (3)
      dsym_148__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (39) dstr_149__EARLY_GIMPLE_PAS;
    struct MELT_OBJECT_STRUCT (5)
      dobj_150__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (39) dstr_151__early_gimple_pas;
    struct MELT_HOOK_STRUCT (3) dhook__152__HOOK_EARLY_GIMPLE_PASSES_START;
    struct MELT_OBJECT_STRUCT (6) dobj_153__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_154___HOOK_EARLY_GIMPLE_PASSES_START;
    struct MELT_MULTIPLE_STRUCT (0) dtup_155___HOOK_EARLY_GIMPLE_PASSES_START;
    struct MELT_ROUTINE_STRUCT (0)
      drout_156__ENABLE_EARLY_GIMPLE_PASSES_START;
    struct MELT_CLOSURE_STRUCT (0) dclo_157__ENABLE_EARLY_GIMPLE_PASSES_START;
    struct MELT_ROUTINE_STRUCT (4)
      drout_158__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST;
    struct MELT_CLOSURE_STRUCT (0)
      dclo_159__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST;
    struct MELT_ROUTINE_STRUCT (4)
      drout_160__REGISTER_EARLY_GIMPLE_PASSES_START_LAST;
    struct MELT_CLOSURE_STRUCT (0)
      dclo_161__REGISTER_EARLY_GIMPLE_PASSES_START_LAST;
    struct MELT_OBJECT_STRUCT (3)
      dsym_162__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (37) dstr_163__EARLY_GIMPLE_PAS;
    struct MELT_OBJECT_STRUCT (5)
      dobj_164__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (37) dstr_165__early_gimple_pas;
    struct MELT_HOOK_STRUCT (3) dhook__166__HOOK_EARLY_GIMPLE_PASSES_END;
    struct MELT_OBJECT_STRUCT (6) dobj_167__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_168___HOOK_EARLY_GIMPLE_PASSES_END;
    struct MELT_MULTIPLE_STRUCT (0) dtup_169___HOOK_EARLY_GIMPLE_PASSES_END;
    struct MELT_ROUTINE_STRUCT (0) drout_170__ENABLE_EARLY_GIMPLE_PASSES_END;
    struct MELT_CLOSURE_STRUCT (0) dclo_171__ENABLE_EARLY_GIMPLE_PASSES_END;
    struct MELT_ROUTINE_STRUCT (4)
      drout_172__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST;
    struct MELT_CLOSURE_STRUCT (0)
      dclo_173__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST;
    struct MELT_ROUTINE_STRUCT (4)
      drout_174__REGISTER_EARLY_GIMPLE_PASSES_END_LAST;
    struct MELT_CLOSURE_STRUCT (0)
      dclo_175__REGISTER_EARLY_GIMPLE_PASSES_END_LAST;
    struct MELT_OBJECT_STRUCT (3) dsym_176__PREGENERICIZE_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (27) dstr_177__PREGENERICIZE_DE;
    struct MELT_OBJECT_STRUCT (5) dobj_178__PREGENERICIZE_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (27) dstr_179__pregenericize_de;
    struct MELT_ROUTINE_STRUCT (0) drout_180__ENABLE_PRE_GENERICIZE;
    struct MELT_CLOSURE_STRUCT (0) dclo_181__ENABLE_PRE_GENERICIZE;
    struct MELT_HOOK_STRUCT (3) dhook__182__HOOK_PRE_GENERICIZE;
    struct MELT_OBJECT_STRUCT (6) dobj_183__;
    struct MELT_OBJECT_STRUCT (3) dsym_184__TFNDECL;
    struct MELT_STRING_STRUCT (7) dstr_185__TFNDECL;
    struct MELT_OBJECT_STRUCT (2) dobj_186;
    struct MELT_MULTIPLE_STRUCT (1) dtup_187___HOOK_PRE_GENERICIZE;
    struct MELT_MULTIPLE_STRUCT (0) dtup_188___HOOK_PRE_GENERICIZE;
    struct MELT_ROUTINE_STRUCT (4) drout_189__REGISTER_PRE_GENERICIZE_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_190__REGISTER_PRE_GENERICIZE_FIRST;
    struct MELT_ROUTINE_STRUCT (4) drout_191__REGISTER_PRE_GENERICIZE_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_192__REGISTER_PRE_GENERICIZE_LAST;
    struct MELT_OBJECT_STRUCT (3) dsym_193__FINISHTYPE_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (24) dstr_194__FINISHTYPE_DELAY;
    struct MELT_OBJECT_STRUCT (5) dobj_195__FINISHTYPE_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (24) dstr_196__finishtype_delay;
    struct MELT_ROUTINE_STRUCT (0) drout_197__ENABLE_FINISH_TYPE;
    struct MELT_CLOSURE_STRUCT (0) dclo_198__ENABLE_FINISH_TYPE;
    struct MELT_HOOK_STRUCT (3) dhook__199__HOOK_FINISH_TYPE;
    struct MELT_OBJECT_STRUCT (6) dobj_200__;
    struct MELT_OBJECT_STRUCT (2) dobj_201;
    struct MELT_MULTIPLE_STRUCT (1) dtup_202___HOOK_FINISH_TYPE;
    struct MELT_MULTIPLE_STRUCT (0) dtup_203___HOOK_FINISH_TYPE;
    struct MELT_ROUTINE_STRUCT (4) drout_204__REGISTER_FINISH_TYPE_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_205__REGISTER_FINISH_TYPE_FIRST;
    struct MELT_ROUTINE_STRUCT (4) drout_206__REGISTER_FINISH_TYPE_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_207__REGISTER_FINISH_TYPE_LAST;
    struct MELT_OBJECT_STRUCT (3) dsym_208__PASS_EXECUTION_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (28) dstr_209__PASS_EXECUTION_D;
    struct MELT_OBJECT_STRUCT (5) dobj_210__PASS_EXECUTION_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (28) dstr_211__pass_execution_d;
    struct MELT_HOOK_STRUCT (3) dhook__212__HOOK_PASS_EXECUTION;
    struct MELT_OBJECT_STRUCT (6) dobj_213__;
    struct MELT_OBJECT_STRUCT (3) dsym_214__PASSNAME;
    struct MELT_STRING_STRUCT (8) dstr_215__PASSNAME;
    struct MELT_OBJECT_STRUCT (2) dobj_216;
    struct MELT_OBJECT_STRUCT (3) dsym_217__PASSNUM;
    struct MELT_STRING_STRUCT (7) dstr_218__PASSNUM;
    struct MELT_OBJECT_STRUCT (2) dobj_219;
    struct MELT_MULTIPLE_STRUCT (2) dtup_220___HOOK_PASS_EXECUTION;
    struct MELT_MULTIPLE_STRUCT (0) dtup_221___HOOK_PASS_EXECUTION;
    struct MELT_ROUTINE_STRUCT (3) drout_222__REGISTER_PASS_EXECUTION_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_223__REGISTER_PASS_EXECUTION_FIRST;
    struct MELT_ROUTINE_STRUCT (3) drout_224__REGISTER_PASS_EXECUTION_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_225__REGISTER_PASS_EXECUTION_LAST;
    struct MELT_HOOK_STRUCT (13) dhook__226__HOOK_GIMPLE_GATE;
    struct MELT_OBJECT_STRUCT (6) dobj_227__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_228___HOOK_GIMPLE_GATE;
    struct MELT_MULTIPLE_STRUCT (0) dtup_229___HOOK_GIMPLE_GATE;
    struct MELT_OBJECT_STRUCT (3) dsym_230__INITIAL_SYSTEM_DATA;
    struct MELT_STRING_STRUCT (19) dstr_231__INITIAL_SYSTEM_D;
    struct MELT_OBJECT_STRUCT (3) dsym_232__CLASS_SYSTEM_DATA;
    struct MELT_STRING_STRUCT (17) dstr_233__CLASS_SYSTEM_DAT;
    struct MELT_OBJECT_STRUCT (3) dsym_234__CLASS_GCC_GIMPLE_PASS;
    struct MELT_STRING_STRUCT (21) dstr_235__CLASS_GCC_GIMPLE;
    struct MELT_OBJECT_STRUCT (3) dsym_236__CLASS_GCC_PASS;
    struct MELT_STRING_STRUCT (14) dstr_237__CLASS_GCC_PASS;
    struct MELT_HOOK_STRUCT (5) dhook__238__HOOK_GIMPLE_EXECUTE;
    struct MELT_OBJECT_STRUCT (6) dobj_239__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_240___HOOK_GIMPLE_EXECUTE;
    struct MELT_MULTIPLE_STRUCT (0) dtup_241___HOOK_GIMPLE_EXECUTE;
    struct MELT_HOOK_STRUCT (7) dhook__242__HOOK_RTL_GATE;
    struct MELT_OBJECT_STRUCT (6) dobj_243__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_244___HOOK_RTL_GATE;
    struct MELT_MULTIPLE_STRUCT (0) dtup_245___HOOK_RTL_GATE;
    struct MELT_OBJECT_STRUCT (3) dsym_246__CLASS_GCC_RTL_PASS;
    struct MELT_STRING_STRUCT (18) dstr_247__CLASS_GCC_RTL_PA;
    struct MELT_HOOK_STRUCT (5) dhook__248__HOOK_RTL_EXECUTE;
    struct MELT_OBJECT_STRUCT (6) dobj_249__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_250___HOOK_RTL_EXECUTE;
    struct MELT_MULTIPLE_STRUCT (0) dtup_251___HOOK_RTL_EXECUTE;
    struct MELT_HOOK_STRUCT (7) dhook__252__HOOK_SIMPLE_IPA_GATE;
    struct MELT_OBJECT_STRUCT (6) dobj_253__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_254___HOOK_SIMPLE_IPA_GATE;
    struct MELT_MULTIPLE_STRUCT (0) dtup_255___HOOK_SIMPLE_IPA_GATE;
    struct MELT_OBJECT_STRUCT (3) dsym_256__CLASS_GCC_SIMPLE_IPA_PASS;
    struct MELT_STRING_STRUCT (25) dstr_257__CLASS_GCC_SIMPLE;
    struct MELT_HOOK_STRUCT (5) dhook__258__HOOK_SIMPLE_IPA_EXECUTE;
    struct MELT_OBJECT_STRUCT (6) dobj_259__;
    struct MELT_MULTIPLE_STRUCT (0) dtup_260___HOOK_SIMPLE_IPA_EXECUTE;
    struct MELT_MULTIPLE_STRUCT (0) dtup_261___HOOK_SIMPLE_IPA_EXECUTE;
    struct MELT_OBJECT_STRUCT (3) dsym_262__AT_FINISH_UNIT_FIRST;
    struct MELT_STRING_STRUCT (20) dstr_263__AT_FINISH_UNIT_F;
    struct MELT_OBJECT_STRUCT (3) dsym_264__AT_FINISH_UNIT_LAST;
    struct MELT_STRING_STRUCT (19) dstr_265__AT_FINISH_UNIT_L;
    struct MELT_OBJECT_STRUCT (3) dsym_266__AT_START_UNIT_FIRST;
    struct MELT_STRING_STRUCT (19) dstr_267__AT_START_UNIT_FI;
    struct MELT_OBJECT_STRUCT (3) dsym_268__AT_START_UNIT_LAST;
    struct MELT_STRING_STRUCT (18) dstr_269__AT_START_UNIT_LA;
    struct MELT_OBJECT_STRUCT (3) dsym_270__HOOK_LOW_DEBUG_VALUE_AT;
    struct MELT_STRING_STRUCT (23) dstr_271__HOOK_LOW_DEBUG_V;
    struct MELT_OBJECT_STRUCT (3) dsym_272__HOOK_OVERRIDE_GATE;
    struct MELT_STRING_STRUCT (18) dstr_273__HOOK_OVERRIDE_GA;
    struct MELT_OBJECT_STRUCT (3) dsym_274__REGISTER_ALL_IPA_PASSES_END_FIRST;
    struct MELT_STRING_STRUCT (33) dstr_275__REGISTER_ALL_IPA;
    struct MELT_OBJECT_STRUCT (3) dsym_276__REGISTER_ALL_IPA_PASSES_END_LAST;
    struct MELT_STRING_STRUCT (32) dstr_277__REGISTER_ALL_IPA;
    struct MELT_OBJECT_STRUCT (3)
      dsym_278__REGISTER_ALL_IPA_PASSES_START_FIRST;
    struct MELT_STRING_STRUCT (35) dstr_279__REGISTER_ALL_IPA;
    struct MELT_OBJECT_STRUCT (3)
      dsym_280__REGISTER_ALL_IPA_PASSES_START_LAST;
    struct MELT_STRING_STRUCT (34) dstr_281__REGISTER_ALL_IPA;
    struct MELT_OBJECT_STRUCT (3) dsym_282__REGISTER_ALL_PASSES_END_FIRST;
    struct MELT_STRING_STRUCT (29) dstr_283__REGISTER_ALL_PAS;
    struct MELT_OBJECT_STRUCT (3) dsym_284__REGISTER_ALL_PASSES_END_LAST;
    struct MELT_STRING_STRUCT (28) dstr_285__REGISTER_ALL_PAS;
    struct MELT_OBJECT_STRUCT (3) dsym_286__REGISTER_ALL_PASSES_START_FIRST;
    struct MELT_STRING_STRUCT (31) dstr_287__REGISTER_ALL_PAS;
    struct MELT_OBJECT_STRUCT (3) dsym_288__REGISTER_ALL_PASSES_START_LAST;
    struct MELT_STRING_STRUCT (30) dstr_289__REGISTER_ALL_PAS;
    struct MELT_OBJECT_STRUCT (3)
      dsym_290__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST;
    struct MELT_STRING_STRUCT (38) dstr_291__REGISTER_EARLY_G;
    struct MELT_OBJECT_STRUCT (3)
      dsym_292__REGISTER_EARLY_GIMPLE_PASSES_END_LAST;
    struct MELT_STRING_STRUCT (37) dstr_293__REGISTER_EARLY_G;
    struct MELT_OBJECT_STRUCT (3)
      dsym_294__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST;
    struct MELT_STRING_STRUCT (40) dstr_295__REGISTER_EARLY_G;
    struct MELT_OBJECT_STRUCT (3)
      dsym_296__REGISTER_EARLY_GIMPLE_PASSES_START_LAST;
    struct MELT_STRING_STRUCT (39) dstr_297__REGISTER_EARLY_G;
    struct MELT_OBJECT_STRUCT (3) dsym_298__REGISTER_FINISH_TYPE_FIRST;
    struct MELT_STRING_STRUCT (26) dstr_299__REGISTER_FINISH_;
    struct MELT_OBJECT_STRUCT (3) dsym_300__REGISTER_FINISH_TYPE_LAST;
    struct MELT_STRING_STRUCT (25) dstr_301__REGISTER_FINISH_;
    struct MELT_OBJECT_STRUCT (3) dsym_302__REGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_STRING_STRUCT (28) dstr_303__REGISTER_OVERRID;
    struct MELT_OBJECT_STRUCT (3) dsym_304__REGISTER_OVERRIDE_GATE_LAST;
    struct MELT_STRING_STRUCT (27) dstr_305__REGISTER_OVERRID;
    struct MELT_OBJECT_STRUCT (3) dsym_306__REGISTER_PASS_EXECUTION_FIRST;
    struct MELT_STRING_STRUCT (29) dstr_307__REGISTER_PASS_EX;
    struct MELT_OBJECT_STRUCT (3) dsym_308__REGISTER_PASS_EXECUTION_LAST;
    struct MELT_STRING_STRUCT (28) dstr_309__REGISTER_PASS_EX;
    struct MELT_OBJECT_STRUCT (3) dsym_310__REGISTER_PRE_GENERICIZE_FIRST;
    struct MELT_STRING_STRUCT (29) dstr_311__REGISTER_PRE_GEN;
    struct MELT_OBJECT_STRUCT (3) dsym_312__REGISTER_PRE_GENERICIZE_LAST;
    struct MELT_STRING_STRUCT (28) dstr_313__REGISTER_PRE_GEN;
    struct MELT_OBJECT_STRUCT (3) dsym_314__UNREGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_STRING_STRUCT (30) dstr_315__UNREGISTER_OVERR;
    struct MELT_OBJECT_STRUCT (3) dsym_316__UNREGISTER_OVERRIDE_GATE_LAST;
    struct MELT_STRING_STRUCT (29) dstr_317__UNREGISTER_OVERR;
    long spare_;
  } *meltcdat = NULL;
  dbgprintf ("start initialize_module_meltdata_warmelt_hooks iniframp__=%p",
	     (void *) iniframp__);
  (void) meltpredefinited;	/* avoid warning if non-used. */
  melt_assertmsg ("check module initial frame",
		  iniframp__->mcfr_nbvar == /*minihash */ -3343);
  /*curframe_cdat_fill */
  meltcdat = (struct meltcdata_st *) meltgc_allocate (sizeof (*meltcdat), 0);
  melt_prohibit_garbcoll = TRUE;
  /*initial routine predef */
  /*inipredefhook dhook__1__HOOK_LOW_DEBUG_VALUE_AT */
  if (!MELT_PREDEF (HOOK_LOW_DEBUG_VALUE_AT))
    MELT_STORE_PREDEF (HOOK_LOW_DEBUG_VALUE_AT,
		       (melt_ptr_t) & meltcdat->
		       dhook__1__HOOK_LOW_DEBUG_VALUE_AT);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_LOW_DEBUG_VALUE_AT) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_LOW_DEBUG_VALUE_AT", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__30__HOOK_OVERRIDE_GATE */
  if (!MELT_PREDEF (HOOK_OVERRIDE_GATE))
    MELT_STORE_PREDEF (HOOK_OVERRIDE_GATE,
		       (melt_ptr_t) & meltcdat->
		       dhook__30__HOOK_OVERRIDE_GATE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_OVERRIDE_GATE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_OVERRIDE_GATE", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__60__HOOK_START_UNIT */
  if (!MELT_PREDEF (HOOK_START_UNIT))
    MELT_STORE_PREDEF (HOOK_START_UNIT,
		       (melt_ptr_t) & meltcdat->dhook__60__HOOK_START_UNIT);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_START_UNIT) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_START_UNIT", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__79__HOOK_FINISH_UNIT */
  if (!MELT_PREDEF (HOOK_FINISH_UNIT))
    MELT_STORE_PREDEF (HOOK_FINISH_UNIT,
		       (melt_ptr_t) & meltcdat->dhook__79__HOOK_FINISH_UNIT);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_FINISH_UNIT) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_FINISH_UNIT", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__96__HOOK_ALL_PASSES_START */
  if (!MELT_PREDEF (HOOK_ALL_PASSES_START))
    MELT_STORE_PREDEF (HOOK_ALL_PASSES_START,
		       (melt_ptr_t) & meltcdat->
		       dhook__96__HOOK_ALL_PASSES_START);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_ALL_PASSES_START) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_ALL_PASSES_START", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__110__HOOK_ALL_PASSES_END */
  if (!MELT_PREDEF (HOOK_ALL_PASSES_END))
    MELT_STORE_PREDEF (HOOK_ALL_PASSES_END,
		       (melt_ptr_t) & meltcdat->
		       dhook__110__HOOK_ALL_PASSES_END);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_ALL_PASSES_END) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_ALL_PASSES_END", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__124__HOOK_ALL_IPA_PASSES_START */
  if (!MELT_PREDEF (HOOK_ALL_IPA_PASSES_START))
    MELT_STORE_PREDEF (HOOK_ALL_IPA_PASSES_START,
		       (melt_ptr_t) & meltcdat->
		       dhook__124__HOOK_ALL_IPA_PASSES_START);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_ALL_IPA_PASSES_START) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_ALL_IPA_PASSES_START", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__138__HOOK_ALL_IPA_PASSES_END */
  if (!MELT_PREDEF (HOOK_ALL_IPA_PASSES_END))
    MELT_STORE_PREDEF (HOOK_ALL_IPA_PASSES_END,
		       (melt_ptr_t) & meltcdat->
		       dhook__138__HOOK_ALL_IPA_PASSES_END);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_ALL_IPA_PASSES_END) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_ALL_IPA_PASSES_END", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__152__HOOK_EARLY_GIMPLE_PASSES_START */
  if (!MELT_PREDEF (HOOK_EARLY_GIMPLE_PASSES_START))
    MELT_STORE_PREDEF (HOOK_EARLY_GIMPLE_PASSES_START,
		       (melt_ptr_t) & meltcdat->
		       dhook__152__HOOK_EARLY_GIMPLE_PASSES_START);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_EARLY_GIMPLE_PASSES_START) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_EARLY_GIMPLE_PASSES_START", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__166__HOOK_EARLY_GIMPLE_PASSES_END */
  if (!MELT_PREDEF (HOOK_EARLY_GIMPLE_PASSES_END))
    MELT_STORE_PREDEF (HOOK_EARLY_GIMPLE_PASSES_END,
		       (melt_ptr_t) & meltcdat->
		       dhook__166__HOOK_EARLY_GIMPLE_PASSES_END);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_EARLY_GIMPLE_PASSES_END) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_EARLY_GIMPLE_PASSES_END", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__182__HOOK_PRE_GENERICIZE */
  if (!MELT_PREDEF (HOOK_PRE_GENERICIZE))
    MELT_STORE_PREDEF (HOOK_PRE_GENERICIZE,
		       (melt_ptr_t) & meltcdat->
		       dhook__182__HOOK_PRE_GENERICIZE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_PRE_GENERICIZE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_PRE_GENERICIZE", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__199__HOOK_FINISH_TYPE */
  if (!MELT_PREDEF (HOOK_FINISH_TYPE))
    MELT_STORE_PREDEF (HOOK_FINISH_TYPE,
		       (melt_ptr_t) & meltcdat->dhook__199__HOOK_FINISH_TYPE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_FINISH_TYPE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_FINISH_TYPE", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__212__HOOK_PASS_EXECUTION */
  if (!MELT_PREDEF (HOOK_PASS_EXECUTION))
    MELT_STORE_PREDEF (HOOK_PASS_EXECUTION,
		       (melt_ptr_t) & meltcdat->
		       dhook__212__HOOK_PASS_EXECUTION);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_PASS_EXECUTION) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_PASS_EXECUTION", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__226__HOOK_GIMPLE_GATE */
  if (!MELT_PREDEF (HOOK_GIMPLE_GATE))
    MELT_STORE_PREDEF (HOOK_GIMPLE_GATE,
		       (melt_ptr_t) & meltcdat->dhook__226__HOOK_GIMPLE_GATE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_GIMPLE_GATE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_GIMPLE_GATE", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__238__HOOK_GIMPLE_EXECUTE */
  if (!MELT_PREDEF (HOOK_GIMPLE_EXECUTE))
    MELT_STORE_PREDEF (HOOK_GIMPLE_EXECUTE,
		       (melt_ptr_t) & meltcdat->
		       dhook__238__HOOK_GIMPLE_EXECUTE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_GIMPLE_EXECUTE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_GIMPLE_EXECUTE", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__242__HOOK_RTL_GATE */
  if (!MELT_PREDEF (HOOK_RTL_GATE))
    MELT_STORE_PREDEF (HOOK_RTL_GATE,
		       (melt_ptr_t) & meltcdat->dhook__242__HOOK_RTL_GATE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_RTL_GATE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_RTL_GATE", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__248__HOOK_RTL_EXECUTE */
  if (!MELT_PREDEF (HOOK_RTL_EXECUTE))
    MELT_STORE_PREDEF (HOOK_RTL_EXECUTE,
		       (melt_ptr_t) & meltcdat->dhook__248__HOOK_RTL_EXECUTE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_RTL_EXECUTE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_RTL_EXECUTE", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__252__HOOK_SIMPLE_IPA_GATE */
  if (!MELT_PREDEF (HOOK_SIMPLE_IPA_GATE))
    MELT_STORE_PREDEF (HOOK_SIMPLE_IPA_GATE,
		       (melt_ptr_t) & meltcdat->
		       dhook__252__HOOK_SIMPLE_IPA_GATE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_SIMPLE_IPA_GATE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_SIMPLE_IPA_GATE", __FILE__, __LINE__);
    };

  /*^inipredefhook */
  /*inipredefhook dhook__258__HOOK_SIMPLE_IPA_EXECUTE */
  if (!MELT_PREDEF (HOOK_SIMPLE_IPA_EXECUTE))
    MELT_STORE_PREDEF (HOOK_SIMPLE_IPA_EXECUTE,
		       (melt_ptr_t) & meltcdat->
		       dhook__258__HOOK_SIMPLE_IPA_EXECUTE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_SIMPLE_IPA_EXECUTE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_SIMPLE_IPA_EXECUTE", __FILE__, __LINE__);
    };

  /*initial routine fill */


/*inihook dhook__1__HOOK_LOW_DEBUG_VALUE_AT */
  meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_LOW_DEBUG_VALUE_AT data */
  meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT,
			  melthook_HOOK_LOW_DEBUG_VALUE_AT);
  meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.hookdata =
    /*_.VALDATA___V21*/ meltfptr[20];
  strncpy (meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.hookname,
	   "HOOK_LOW_DEBUG_VALUE_AT",
	   sizeof (meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.hookname) - 1);
/*predefinedhook HOOK_LOW_DEBUG_VALUE_AT for dhook__1__HOOK_LOW_DEBUG_VALUE_AT*/
  if (!MELT_PREDEF (HOOK_LOW_DEBUG_VALUE_AT))
    MELT_STORE_PREDEF (HOOK_LOW_DEBUG_VALUE_AT,
		       (melt_ptr_t) & meltcdat->
		       dhook__1__HOOK_LOW_DEBUG_VALUE_AT);

  /*_.VALHOOK___V18*/ meltfptr[17] =
  /*inihooklocvar HOOK_LOW_DEBUG_VALUE_AT */
  (melt_ptr_t) & meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT;

  /*done inihook dhook__1__HOOK_LOW_DEBUG_VALUE_AT */

  MELT_LOCATION ("warmelt-hooks.melt:32:/ iniobj");

/*iniobj dobj_2__*/
/*_.VALDATA___V21*/ meltfptr[20] = (melt_ptr_t) & meltcdat->dobj_2__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_2__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_2__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_2__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_2__.obj_hash = 643112671;
  meltcdat->dobj_2__.obj_len = 6;


/*iniobj dsym_3__VAL*/
  /*uniqueobj*/ if (! /*_.VALDATA___V24*/ meltfptr[23])
/*_.VALDATA___V24*/ meltfptr[23] = (melt_ptr_t) & meltcdat->dsym_3__VAL;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_3__VAL",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_3__VAL",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_3__VAL.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_3__VAL.obj_hash = 122845685;
  meltcdat->dsym_3__VAL.obj_len = 3;


/*inistring dstr_4__VAL*/
 /*_.VALSTR___V25*/ meltfptr[24] = (melt_ptr_t) & meltcdat->dstr_4__VAL;
  meltcdat->dstr_4__VAL.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_4__VAL.val, "VAL",
				sizeof (meltcdat->dstr_4__VAL.val) - 1);
  meltcdat->dstr_4__VAL.val[3] = (char) 0;
  meltcdat->dstr_4__VAL.slen = 3;

  /*^iniobj */

/*iniobj dobj_5*/
/*_.VALDATA___V23*/ meltfptr[22] = (melt_ptr_t) & meltcdat->dobj_5;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_5",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_5",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_5.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_5.obj_num = 0;
  meltcdat->dobj_5.obj_hash = 556645664;
  meltcdat->dobj_5.obj_len = 2;


/*iniobj dsym_6__FILENAME*/
  /*uniqueobj*/ if (! /*_.VALDATA___V27*/ meltfptr[26])
/*_.VALDATA___V27*/ meltfptr[26] =
      (melt_ptr_t) & meltcdat->dsym_6__FILENAME;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_6__FILENAME",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_6__FILENAME",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_6__FILENAME.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_6__FILENAME.obj_hash = 109653219;
  meltcdat->dsym_6__FILENAME.obj_len = 3;


/*inistring dstr_7__FILENAME*/
 /*_.VALSTR___V28*/ meltfptr[27] = (melt_ptr_t) & meltcdat->dstr_7__FILENAME;
  meltcdat->dstr_7__FILENAME.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_7__FILENAME.val, "FILENAME",
				sizeof (meltcdat->dstr_7__FILENAME.val) - 1);
  meltcdat->dstr_7__FILENAME.val[8] = (char) 0;
  meltcdat->dstr_7__FILENAME.slen = 8;

  /*^iniobj */

/*iniobj dobj_8*/
/*_.VALDATA___V26*/ meltfptr[25] = (melt_ptr_t) & meltcdat->dobj_8;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_8",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_8",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_8.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_8.obj_num = 1;
  meltcdat->dobj_8.obj_hash = 761276875;
  meltcdat->dobj_8.obj_len = 2;


/*iniobj dsym_9__LINENO*/
  /*uniqueobj*/ if (! /*_.VALDATA___V30*/ meltfptr[29])
/*_.VALDATA___V30*/ meltfptr[29] =
      (melt_ptr_t) & meltcdat->dsym_9__LINENO;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_9__LINENO",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_9__LINENO",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_9__LINENO.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_9__LINENO.obj_hash = 590813330;
  meltcdat->dsym_9__LINENO.obj_len = 3;


/*inistring dstr_10__LINENO*/
 /*_.VALSTR___V31*/ meltfptr[30] = (melt_ptr_t) & meltcdat->dstr_10__LINENO;
  meltcdat->dstr_10__LINENO.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_10__LINENO.val, "LINENO",
				sizeof (meltcdat->dstr_10__LINENO.val) - 1);
  meltcdat->dstr_10__LINENO.val[6] = (char) 0;
  meltcdat->dstr_10__LINENO.slen = 6;

  /*^iniobj */

/*iniobj dobj_11*/
/*_.VALDATA___V29*/ meltfptr[28] = (melt_ptr_t) & meltcdat->dobj_11;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_11",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_11",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_11.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_11.obj_num = 2;
  meltcdat->dobj_11.obj_hash = 56419665;
  meltcdat->dobj_11.obj_len = 2;


/*iniobj dsym_12__MSG*/
  /*uniqueobj*/ if (! /*_.VALDATA___V33*/ meltfptr[32])
/*_.VALDATA___V33*/ meltfptr[32] = (melt_ptr_t) & meltcdat->dsym_12__MSG;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_12__MSG",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_12__MSG",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_12__MSG.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_12__MSG.obj_hash = 97118120;
  meltcdat->dsym_12__MSG.obj_len = 3;


/*inistring dstr_13__MSG*/
 /*_.VALSTR___V34*/ meltfptr[33] = (melt_ptr_t) & meltcdat->dstr_13__MSG;
  meltcdat->dstr_13__MSG.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_13__MSG.val, "MSG",
				sizeof (meltcdat->dstr_13__MSG.val) - 1);
  meltcdat->dstr_13__MSG.val[3] = (char) 0;
  meltcdat->dstr_13__MSG.slen = 3;

  /*^iniobj */

/*iniobj dobj_14*/
/*_.VALDATA___V32*/ meltfptr[31] = (melt_ptr_t) & meltcdat->dobj_14;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_14",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_14",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_14.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_14.obj_num = 3;
  meltcdat->dobj_14.obj_hash = 864839698;
  meltcdat->dobj_14.obj_len = 2;


/*iniobj dsym_15__COUNT*/
  /*uniqueobj*/ if (! /*_.VALDATA___V36*/ meltfptr[35])
/*_.VALDATA___V36*/ meltfptr[35] =
      (melt_ptr_t) & meltcdat->dsym_15__COUNT;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_15__COUNT",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_15__COUNT",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_15__COUNT.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_15__COUNT.obj_hash = 929826072;
  meltcdat->dsym_15__COUNT.obj_len = 3;


/*inistring dstr_16__COUNT*/
 /*_.VALSTR___V37*/ meltfptr[36] = (melt_ptr_t) & meltcdat->dstr_16__COUNT;
  meltcdat->dstr_16__COUNT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_16__COUNT.val, "COUNT",
				sizeof (meltcdat->dstr_16__COUNT.val) - 1);
  meltcdat->dstr_16__COUNT.val[5] = (char) 0;
  meltcdat->dstr_16__COUNT.slen = 5;

  /*^iniobj */

/*iniobj dobj_17*/
/*_.VALDATA___V35*/ meltfptr[34] = (melt_ptr_t) & meltcdat->dobj_17;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_17",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_17",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_17.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_17.obj_num = 4;
  meltcdat->dobj_17.obj_hash = 206007972;
  meltcdat->dobj_17.obj_len = 2;

  /*inimult dtup_18___HOOK_LOW_DEBUG_VALUE_AT */
 /*_.VALTUP___V38*/ meltfptr[37] =
    (melt_ptr_t) & meltcdat->dtup_18___HOOK_LOW_DEBUG_VALUE_AT;
  meltcdat->dtup_18___HOOK_LOW_DEBUG_VALUE_AT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_18___HOOK_LOW_DEBUG_VALUE_AT.nbval = 5;
  /*inimult dtup_19___HOOK_LOW_DEBUG_VALUE_AT */
 /*_.VALTUP___V39*/ meltfptr[38] =
    (melt_ptr_t) & meltcdat->dtup_19___HOOK_LOW_DEBUG_VALUE_AT;
  meltcdat->dtup_19___HOOK_LOW_DEBUG_VALUE_AT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_19___HOOK_LOW_DEBUG_VALUE_AT.nbval = 0;

/*iniobj dsym_20__MELT_DEBUG_FUN*/
  /*uniqueobj*/ if (! /*_.VALDATA___V19*/ meltfptr[18])
/*_.VALDATA___V19*/ meltfptr[18] =
      (melt_ptr_t) & meltcdat->dsym_20__MELT_DEBUG_FUN;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_20__MELT_DEBUG_FUN",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_20__MELT_DEBUG_FUN",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_20__MELT_DEBUG_FUN.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_20__MELT_DEBUG_FUN.obj_hash = 972123377;
  meltcdat->dsym_20__MELT_DEBUG_FUN.obj_len = 3;


/*inistring dstr_21__MELT_DEBUG_FUN*/
 /*_.VALSTR___V20*/ meltfptr[19] =
    (melt_ptr_t) & meltcdat->dstr_21__MELT_DEBUG_FUN;
  meltcdat->dstr_21__MELT_DEBUG_FUN.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_21__MELT_DEBUG_FUN.val,
				"MELT_DEBUG_FUN",
				sizeof (meltcdat->dstr_21__MELT_DEBUG_FUN.
					val) - 1);
  meltcdat->dstr_21__MELT_DEBUG_FUN.val[14] = (char) 0;
  meltcdat->dstr_21__MELT_DEBUG_FUN.slen = 14;


/*iniobj dsym_22__OVERRIDE_GATE_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V40*/ meltfptr[39])
/*_.VALDATA___V40*/ meltfptr[39] =
      (melt_ptr_t) & meltcdat->dsym_22__OVERRIDE_GATE_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_22__OVERRIDE_GATE_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_22__OVERRIDE_GATE_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_22__OVERRIDE_GATE_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_22__OVERRIDE_GATE_DELAYED_QUEUE.obj_hash = 225417214;
  meltcdat->dsym_22__OVERRIDE_GATE_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_23__OVERRIDE_GATE_DE*/
 /*_.VALSTR___V41*/ meltfptr[40] =
    (melt_ptr_t) & meltcdat->dstr_23__OVERRIDE_GATE_DE;
  meltcdat->dstr_23__OVERRIDE_GATE_DE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_23__OVERRIDE_GATE_DE.val,
				"OVERRIDE_GATE_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_23__OVERRIDE_GATE_DE.
					val) - 1);
  meltcdat->dstr_23__OVERRIDE_GATE_DE.val[27] = (char) 0;
  meltcdat->dstr_23__OVERRIDE_GATE_DE.slen = 27;


/*iniobj dsym_24__CLASS_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V42*/ meltfptr[41])
/*_.VALDATA___V42*/ meltfptr[41] =
      (melt_ptr_t) & meltcdat->dsym_24__CLASS_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_24__CLASS_DELAYED_QUEUE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_24__CLASS_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_24__CLASS_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_24__CLASS_DELAYED_QUEUE.obj_hash = 761132519;
  meltcdat->dsym_24__CLASS_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_25__CLASS_DELAYED_QU*/
 /*_.VALSTR___V43*/ meltfptr[42] =
    (melt_ptr_t) & meltcdat->dstr_25__CLASS_DELAYED_QU;
  meltcdat->dstr_25__CLASS_DELAYED_QU.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_25__CLASS_DELAYED_QU.val,
				"CLASS_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_25__CLASS_DELAYED_QU.
					val) - 1);
  meltcdat->dstr_25__CLASS_DELAYED_QU.val[19] = (char) 0;
  meltcdat->dstr_25__CLASS_DELAYED_QU.slen = 19;

  MELT_LOCATION ("warmelt-hooks.melt:43:/ iniobj");

/*iniobj dobj_26__OVERRIDE_GATE_DELAYED_QUEUE*/
/*_.VALDATA___V44*/ meltfptr[43] =
    (melt_ptr_t) & meltcdat->dobj_26__OVERRIDE_GATE_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_26__OVERRIDE_GATE_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_26__OVERRIDE_GATE_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_26__OVERRIDE_GATE_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_26__OVERRIDE_GATE_DELAYED_QUEUE.obj_hash = 448129967;
  meltcdat->dobj_26__OVERRIDE_GATE_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_27__override_gate_de*/
 /*_.VALSTR___V47*/ meltfptr[46] =
    (melt_ptr_t) & meltcdat->dstr_27__override_gate_de;
  meltcdat->dstr_27__override_gate_de.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_27__override_gate_de.val,
				"override_gate_delayed_queue",
				sizeof (meltcdat->dstr_27__override_gate_de.
					val) - 1);
  meltcdat->dstr_27__override_gate_de.val[27] = (char) 0;
  meltcdat->dstr_27__override_gate_de.slen = 27;


/*iniobj dsym_28__DISCR_LIST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V48*/ meltfptr[47])
/*_.VALDATA___V48*/ meltfptr[47] =
      (melt_ptr_t) & meltcdat->dsym_28__DISCR_LIST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_28__DISCR_LIST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_28__DISCR_LIST",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_28__DISCR_LIST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_28__DISCR_LIST.obj_hash = 98407634;
  meltcdat->dsym_28__DISCR_LIST.obj_len = 3;


/*inistring dstr_29__DISCR_LIST*/
 /*_.VALSTR___V49*/ meltfptr[48] =
    (melt_ptr_t) & meltcdat->dstr_29__DISCR_LIST;
  meltcdat->dstr_29__DISCR_LIST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_29__DISCR_LIST.val,
				"DISCR_LIST",
				sizeof (meltcdat->dstr_29__DISCR_LIST.val) -
				1);
  meltcdat->dstr_29__DISCR_LIST.val[10] = (char) 0;
  meltcdat->dstr_29__DISCR_LIST.slen = 10;


/*inihook dhook__30__HOOK_OVERRIDE_GATE */
  meltcdat->dhook__30__HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_OVERRIDE_GATE data */
  meltcdat->dhook__30__HOOK_OVERRIDE_GATE.nbval = 9;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__30__HOOK_OVERRIDE_GATE,
			  melthook_HOOK_OVERRIDE_GATE);
  meltcdat->dhook__30__HOOK_OVERRIDE_GATE.hookdata =
    /*_.VALDATA___V55*/ meltfptr[54];
  strncpy (meltcdat->dhook__30__HOOK_OVERRIDE_GATE.hookname,
	   "HOOK_OVERRIDE_GATE",
	   sizeof (meltcdat->dhook__30__HOOK_OVERRIDE_GATE.hookname) - 1);
/*predefinedhook HOOK_OVERRIDE_GATE for dhook__30__HOOK_OVERRIDE_GATE*/
  if (!MELT_PREDEF (HOOK_OVERRIDE_GATE))
    MELT_STORE_PREDEF (HOOK_OVERRIDE_GATE,
		       (melt_ptr_t) & meltcdat->
		       dhook__30__HOOK_OVERRIDE_GATE);

  /*_.VALHOOK___V50*/ meltfptr[49] =
  /*inihooklocvar HOOK_OVERRIDE_GATE */
  (melt_ptr_t) & meltcdat->dhook__30__HOOK_OVERRIDE_GATE;

  /*done inihook dhook__30__HOOK_OVERRIDE_GATE */

  MELT_LOCATION ("warmelt-hooks.melt:49:/ iniobj");

/*iniobj dobj_31__*/
/*_.VALDATA___V55*/ meltfptr[54] = (melt_ptr_t) & meltcdat->dobj_31__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_31__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_31__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_31__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_31__.obj_hash = 1066478506;
  meltcdat->dobj_31__.obj_len = 6;


/*iniobj dsym_32__BEFOREGATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V58*/ meltfptr[57])
/*_.VALDATA___V58*/ meltfptr[57] =
      (melt_ptr_t) & meltcdat->dsym_32__BEFOREGATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_32__BEFOREGATE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_32__BEFOREGATE",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_32__BEFOREGATE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_32__BEFOREGATE.obj_hash = 841081062;
  meltcdat->dsym_32__BEFOREGATE.obj_len = 3;


/*inistring dstr_33__BEFOREGATE*/
 /*_.VALSTR___V59*/ meltfptr[58] =
    (melt_ptr_t) & meltcdat->dstr_33__BEFOREGATE;
  meltcdat->dstr_33__BEFOREGATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_33__BEFOREGATE.val,
				"BEFOREGATE",
				sizeof (meltcdat->dstr_33__BEFOREGATE.val) -
				1);
  meltcdat->dstr_33__BEFOREGATE.val[10] = (char) 0;
  meltcdat->dstr_33__BEFOREGATE.slen = 10;

  /*^iniobj */

/*iniobj dobj_34*/
/*_.VALDATA___V57*/ meltfptr[56] = (melt_ptr_t) & meltcdat->dobj_34;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_34",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_34",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_34.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_34.obj_num = 0;
  meltcdat->dobj_34.obj_hash = 294979320;
  meltcdat->dobj_34.obj_len = 2;

  /*inimult dtup_35___HOOK_OVERRIDE_GATE */
 /*_.VALTUP___V60*/ meltfptr[59] =
    (melt_ptr_t) & meltcdat->dtup_35___HOOK_OVERRIDE_GATE;
  meltcdat->dtup_35___HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_35___HOOK_OVERRIDE_GATE.nbval = 1;

/*iniobj dsym_36__AFTERGATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V62*/ meltfptr[61])
/*_.VALDATA___V62*/ meltfptr[61] =
      (melt_ptr_t) & meltcdat->dsym_36__AFTERGATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_36__AFTERGATE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_36__AFTERGATE",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_36__AFTERGATE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_36__AFTERGATE.obj_hash = 561055110;
  meltcdat->dsym_36__AFTERGATE.obj_len = 3;


/*inistring dstr_37__AFTERGATE*/
 /*_.VALSTR___V63*/ meltfptr[62] =
    (melt_ptr_t) & meltcdat->dstr_37__AFTERGATE;
  meltcdat->dstr_37__AFTERGATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_37__AFTERGATE.val, "AFTERGATE",
				sizeof (meltcdat->dstr_37__AFTERGATE.val) -
				1);
  meltcdat->dstr_37__AFTERGATE.val[9] = (char) 0;
  meltcdat->dstr_37__AFTERGATE.slen = 9;

  /*^iniobj */

/*iniobj dobj_38*/
/*_.VALDATA___V61*/ meltfptr[60] = (melt_ptr_t) & meltcdat->dobj_38;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_38",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_38",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_38.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_38.obj_num = 0;
  meltcdat->dobj_38.obj_hash = 156789889;
  meltcdat->dobj_38.obj_len = 2;

  /*inimult dtup_39___HOOK_OVERRIDE_GATE */
 /*_.VALTUP___V64*/ meltfptr[63] =
    (melt_ptr_t) & meltcdat->dtup_39___HOOK_OVERRIDE_GATE;
  meltcdat->dtup_39___HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_39___HOOK_OVERRIDE_GATE.nbval = 1;

/*iniobj dsym_40__TRUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V51*/ meltfptr[50])
/*_.VALDATA___V51*/ meltfptr[50] = (melt_ptr_t) & meltcdat->dsym_40__TRUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_40__TRUE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_KEYWORD))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_40__TRUE",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_KEYWORD)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_40__TRUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_KEYWORD))));
  meltcdat->dsym_40__TRUE.obj_hash = 576485513;
  meltcdat->dsym_40__TRUE.obj_len = 3;


/*inistring dstr_41__TRUE*/
 /*_.VALSTR___V52*/ meltfptr[51] = (melt_ptr_t) & meltcdat->dstr_41__TRUE;
  meltcdat->dstr_41__TRUE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_41__TRUE.val, "TRUE",
				sizeof (meltcdat->dstr_41__TRUE.val) - 1);
  meltcdat->dstr_41__TRUE.val[4] = (char) 0;
  meltcdat->dstr_41__TRUE.slen = 4;


/*iniobj dsym_42__CLASS_REFERENCE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V53*/ meltfptr[52])
/*_.VALDATA___V53*/ meltfptr[52] =
      (melt_ptr_t) & meltcdat->dsym_42__CLASS_REFERENCE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_42__CLASS_REFERENCE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_42__CLASS_REFERENCE",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_42__CLASS_REFERENCE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_42__CLASS_REFERENCE.obj_hash = 896623907;
  meltcdat->dsym_42__CLASS_REFERENCE.obj_len = 3;


/*inistring dstr_43__CLASS_REFERENCE*/
 /*_.VALSTR___V54*/ meltfptr[53] =
    (melt_ptr_t) & meltcdat->dstr_43__CLASS_REFERENCE;
  meltcdat->dstr_43__CLASS_REFERENCE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_43__CLASS_REFERENCE.val,
				"CLASS_REFERENCE",
				sizeof (meltcdat->dstr_43__CLASS_REFERENCE.
					val) - 1);
  meltcdat->dstr_43__CLASS_REFERENCE.val[15] = (char) 0;
  meltcdat->dstr_43__CLASS_REFERENCE.slen = 15;

  /*inirout drout_44__ENABLE_OVERRIDE_GATE */
 /*_.VALROUT___V65*/ meltfptr[64] =
    (melt_ptr_t) & meltcdat->drout_44__ENABLE_OVERRIDE_GATE;
  meltcdat->drout_44__ENABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_44__ENABLE_OVERRIDE_GATE.routdescr,
	   "ENABLE_OVERRIDE_GATE @warmelt-hooks.melt:144",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_44__ENABLE_OVERRIDE_GATE.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_44__ENABLE_OVERRIDE_GATE,
			     meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE);

  /*iniclos dclo_45__ENABLE_OVERRIDE_GATE */
 /*_.VALCLO___V66*/ meltfptr[65] =
    (melt_ptr_t) & meltcdat->dclo_45__ENABLE_OVERRIDE_GATE;
  meltcdat->dclo_45__ENABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_45__ENABLE_OVERRIDE_GATE.nbval = 0;

  /*inirout drout_46__MAYBE_DISABLE_OVERRIDE_GATE */
 /*_.VALROUT___V67*/ meltfptr[66] =
    (melt_ptr_t) & meltcdat->drout_46__MAYBE_DISABLE_OVERRIDE_GATE;
  meltcdat->drout_46__MAYBE_DISABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_46__MAYBE_DISABLE_OVERRIDE_GATE.routdescr,
	   "MAYBE_DISABLE_OVERRIDE_GATE @warmelt-hooks.melt:159",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_46__MAYBE_DISABLE_OVERRIDE_GATE.nbval = 2;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_46__MAYBE_DISABLE_OVERRIDE_GATE,
			     meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE);

  /*iniclos dclo_47__MAYBE_DISABLE_OVERRIDE_GATE */
 /*_.VALCLO___V68*/ meltfptr[67] =
    (melt_ptr_t) & meltcdat->dclo_47__MAYBE_DISABLE_OVERRIDE_GATE;
  meltcdat->dclo_47__MAYBE_DISABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_47__MAYBE_DISABLE_OVERRIDE_GATE.nbval = 0;

  /*inirout drout_48__REGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALROUT___V69*/ meltfptr[68] =
    (melt_ptr_t) & meltcdat->drout_48__REGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->drout_48__REGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_48__REGISTER_OVERRIDE_GATE_FIRST.routdescr,
	   "REGISTER_OVERRIDE_GATE_FIRST @warmelt-hooks.melt:175",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_48__REGISTER_OVERRIDE_GATE_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_48__REGISTER_OVERRIDE_GATE_FIRST,
			     meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST);

  /*iniclos dclo_49__REGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALCLO___V70*/ meltfptr[69] =
    (melt_ptr_t) & meltcdat->dclo_49__REGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->dclo_49__REGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_49__REGISTER_OVERRIDE_GATE_FIRST.nbval = 0;

  /*inirout drout_50__REGISTER_OVERRIDE_GATE_LAST */
 /*_.VALROUT___V71*/ meltfptr[70] =
    (melt_ptr_t) & meltcdat->drout_50__REGISTER_OVERRIDE_GATE_LAST;
  meltcdat->drout_50__REGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_50__REGISTER_OVERRIDE_GATE_LAST.routdescr,
	   "REGISTER_OVERRIDE_GATE_LAST @warmelt-hooks.melt:183",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_50__REGISTER_OVERRIDE_GATE_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_50__REGISTER_OVERRIDE_GATE_LAST,
			     meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST);

  /*iniclos dclo_51__REGISTER_OVERRIDE_GATE_LAST */
 /*_.VALCLO___V72*/ meltfptr[71] =
    (melt_ptr_t) & meltcdat->dclo_51__REGISTER_OVERRIDE_GATE_LAST;
  meltcdat->dclo_51__REGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_51__REGISTER_OVERRIDE_GATE_LAST.nbval = 0;

  /*inirout drout_52__UNREGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALROUT___V73*/ meltfptr[72] =
    (melt_ptr_t) & meltcdat->drout_52__UNREGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->drout_52__UNREGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_52__UNREGISTER_OVERRIDE_GATE_FIRST.routdescr,
	   "UNREGISTER_OVERRIDE_GATE_FIRST @warmelt-hooks.melt:191",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_52__UNREGISTER_OVERRIDE_GATE_FIRST.nbval = 5;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_52__UNREGISTER_OVERRIDE_GATE_FIRST,
			     meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST);

  /*iniclos dclo_53__UNREGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALCLO___V74*/ meltfptr[73] =
    (melt_ptr_t) & meltcdat->dclo_53__UNREGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->dclo_53__UNREGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_53__UNREGISTER_OVERRIDE_GATE_FIRST.nbval = 0;

  /*inirout drout_54__UNREGISTER_OVERRIDE_GATE_LAST */
 /*_.VALROUT___V75*/ meltfptr[74] =
    (melt_ptr_t) & meltcdat->drout_54__UNREGISTER_OVERRIDE_GATE_LAST;
  meltcdat->drout_54__UNREGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_54__UNREGISTER_OVERRIDE_GATE_LAST.routdescr,
	   "UNREGISTER_OVERRIDE_GATE_LAST @warmelt-hooks.melt:214",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_54__UNREGISTER_OVERRIDE_GATE_LAST.nbval = 5;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_54__UNREGISTER_OVERRIDE_GATE_LAST,
			     meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST);

  /*iniclos dclo_55__UNREGISTER_OVERRIDE_GATE_LAST */
 /*_.VALCLO___V76*/ meltfptr[75] =
    (melt_ptr_t) & meltcdat->dclo_55__UNREGISTER_OVERRIDE_GATE_LAST;
  meltcdat->dclo_55__UNREGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_55__UNREGISTER_OVERRIDE_GATE_LAST.nbval = 0;


/*iniobj dsym_56__START_UNIT_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V77*/ meltfptr[76])
/*_.VALDATA___V77*/ meltfptr[76] =
      (melt_ptr_t) & meltcdat->dsym_56__START_UNIT_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_56__START_UNIT_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_56__START_UNIT_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_56__START_UNIT_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_56__START_UNIT_DELAYED_QUEUE.obj_hash = 335728873;
  meltcdat->dsym_56__START_UNIT_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_57__START_UNIT_DELAY*/
 /*_.VALSTR___V78*/ meltfptr[77] =
    (melt_ptr_t) & meltcdat->dstr_57__START_UNIT_DELAY;
  meltcdat->dstr_57__START_UNIT_DELAY.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_57__START_UNIT_DELAY.val,
				"START_UNIT_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_57__START_UNIT_DELAY.
					val) - 1);
  meltcdat->dstr_57__START_UNIT_DELAY.val[24] = (char) 0;
  meltcdat->dstr_57__START_UNIT_DELAY.slen = 24;

  MELT_LOCATION ("warmelt-hooks.melt:250:/ iniobj");

/*iniobj dobj_58__START_UNIT_DELAYED_QUEUE*/
/*_.VALDATA___V79*/ meltfptr[78] =
    (melt_ptr_t) & meltcdat->dobj_58__START_UNIT_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_58__START_UNIT_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_58__START_UNIT_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_58__START_UNIT_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_58__START_UNIT_DELAYED_QUEUE.obj_hash = 352060588;
  meltcdat->dobj_58__START_UNIT_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_59__start_unit_delay*/
 /*_.VALSTR___V82*/ meltfptr[81] =
    (melt_ptr_t) & meltcdat->dstr_59__start_unit_delay;
  meltcdat->dstr_59__start_unit_delay.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_59__start_unit_delay.val,
				"start_unit_delayed_queue",
				sizeof (meltcdat->dstr_59__start_unit_delay.
					val) - 1);
  meltcdat->dstr_59__start_unit_delay.val[24] = (char) 0;
  meltcdat->dstr_59__start_unit_delay.slen = 24;


/*inihook dhook__60__HOOK_START_UNIT */
  meltcdat->dhook__60__HOOK_START_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_START_UNIT data */
  meltcdat->dhook__60__HOOK_START_UNIT.nbval = 9;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__60__HOOK_START_UNIT,
			  melthook_HOOK_START_UNIT);
  meltcdat->dhook__60__HOOK_START_UNIT.hookdata =
    /*_.VALDATA___V89*/ meltfptr[88];
  strncpy (meltcdat->dhook__60__HOOK_START_UNIT.hookname, "HOOK_START_UNIT",
	   sizeof (meltcdat->dhook__60__HOOK_START_UNIT.hookname) - 1);
/*predefinedhook HOOK_START_UNIT for dhook__60__HOOK_START_UNIT*/
  if (!MELT_PREDEF (HOOK_START_UNIT))
    MELT_STORE_PREDEF (HOOK_START_UNIT,
		       (melt_ptr_t) & meltcdat->dhook__60__HOOK_START_UNIT);

  /*_.VALHOOK___V83*/ meltfptr[82] =
  /*inihooklocvar HOOK_START_UNIT */
  (melt_ptr_t) & meltcdat->dhook__60__HOOK_START_UNIT;

  /*done inihook dhook__60__HOOK_START_UNIT */

  MELT_LOCATION ("warmelt-hooks.melt:259:/ iniobj");

/*iniobj dobj_61__*/
/*_.VALDATA___V89*/ meltfptr[88] = (melt_ptr_t) & meltcdat->dobj_61__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_61__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_61__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_61__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_61__.obj_hash = 893121042;
  meltcdat->dobj_61__.obj_len = 6;

  /*inimult dtup_62___HOOK_START_UNIT */
 /*_.VALTUP___V91*/ meltfptr[90] =
    (melt_ptr_t) & meltcdat->dtup_62___HOOK_START_UNIT;
  meltcdat->dtup_62___HOOK_START_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_62___HOOK_START_UNIT.nbval = 0;
  /*inimult dtup_63___HOOK_START_UNIT */
 /*_.VALTUP___V92*/ meltfptr[91] =
    (melt_ptr_t) & meltcdat->dtup_63___HOOK_START_UNIT;
  meltcdat->dtup_63___HOOK_START_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_63___HOOK_START_UNIT.nbval = 0;

/*iniobj dsym_64__LIST_EVERY*/
  /*uniqueobj*/ if (! /*_.VALDATA___V84*/ meltfptr[83])
/*_.VALDATA___V84*/ meltfptr[83] =
      (melt_ptr_t) & meltcdat->dsym_64__LIST_EVERY;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_64__LIST_EVERY",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_64__LIST_EVERY",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_64__LIST_EVERY.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_64__LIST_EVERY.obj_hash = 942863429;
  meltcdat->dsym_64__LIST_EVERY.obj_len = 3;


/*inistring dstr_65__LIST_EVERY*/
 /*_.VALSTR___V85*/ meltfptr[84] =
    (melt_ptr_t) & meltcdat->dstr_65__LIST_EVERY;
  meltcdat->dstr_65__LIST_EVERY.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_65__LIST_EVERY.val,
				"LIST_EVERY",
				sizeof (meltcdat->dstr_65__LIST_EVERY.val) -
				1);
  meltcdat->dstr_65__LIST_EVERY.val[10] = (char) 0;
  meltcdat->dstr_65__LIST_EVERY.slen = 10;

  /*inirout drout_66__LAMBDA_ */
 /*_.VALROUT___V86*/ meltfptr[85] =
    (melt_ptr_t) & meltcdat->drout_66__LAMBDA_;
  meltcdat->drout_66__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_66__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:269", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_66__LAMBDA_.nbval = 1;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_66__LAMBDA_,
			     meltrout_9_warmelt_hooks_LAMBDA___1__);

  /*inirout drout_67__LAMBDA_ */
 /*_.VALROUT___V87*/ meltfptr[86] =
    (melt_ptr_t) & meltcdat->drout_67__LAMBDA_;
  meltcdat->drout_67__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_67__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:278", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_67__LAMBDA_.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_67__LAMBDA_,
			     meltrout_10_warmelt_hooks_LAMBDA___2__);

  /*inirout drout_68__LAMBDA_ */
 /*_.VALROUT___V88*/ meltfptr[87] =
    (melt_ptr_t) & meltcdat->drout_68__LAMBDA_;
  meltcdat->drout_68__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_68__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:283", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_68__LAMBDA_.nbval = 1;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_68__LAMBDA_,
			     meltrout_11_warmelt_hooks_LAMBDA___3__);

  /*inirout drout_69__ENABLE_START_UNIT */
 /*_.VALROUT___V93*/ meltfptr[92] =
    (melt_ptr_t) & meltcdat->drout_69__ENABLE_START_UNIT;
  meltcdat->drout_69__ENABLE_START_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_69__ENABLE_START_UNIT.routdescr,
	   "ENABLE_START_UNIT @warmelt-hooks.melt:307",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_69__ENABLE_START_UNIT.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_69__ENABLE_START_UNIT,
			     meltrout_13_warmelt_hooks_ENABLE_START_UNIT);

  /*iniclos dclo_70__ENABLE_START_UNIT */
 /*_.VALCLO___V94*/ meltfptr[93] =
    (melt_ptr_t) & meltcdat->dclo_70__ENABLE_START_UNIT;
  meltcdat->dclo_70__ENABLE_START_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_70__ENABLE_START_UNIT.nbval = 0;

  /*inirout drout_71__AT_START_UNIT_FIRST */
 /*_.VALROUT___V95*/ meltfptr[94] =
    (melt_ptr_t) & meltcdat->drout_71__AT_START_UNIT_FIRST;
  meltcdat->drout_71__AT_START_UNIT_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_71__AT_START_UNIT_FIRST.routdescr,
	   "AT_START_UNIT_FIRST @warmelt-hooks.melt:322",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_71__AT_START_UNIT_FIRST.nbval = 2;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_71__AT_START_UNIT_FIRST,
			     meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST);

  /*iniclos dclo_72__AT_START_UNIT_FIRST */
 /*_.VALCLO___V96*/ meltfptr[95] =
    (melt_ptr_t) & meltcdat->dclo_72__AT_START_UNIT_FIRST;
  meltcdat->dclo_72__AT_START_UNIT_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_72__AT_START_UNIT_FIRST.nbval = 0;

  /*inirout drout_73__AT_START_UNIT_LAST */
 /*_.VALROUT___V97*/ meltfptr[96] =
    (melt_ptr_t) & meltcdat->drout_73__AT_START_UNIT_LAST;
  meltcdat->drout_73__AT_START_UNIT_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_73__AT_START_UNIT_LAST.routdescr,
	   "AT_START_UNIT_LAST @warmelt-hooks.melt:331",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_73__AT_START_UNIT_LAST.nbval = 2;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_73__AT_START_UNIT_LAST,
			     meltrout_15_warmelt_hooks_AT_START_UNIT_LAST);

  /*iniclos dclo_74__AT_START_UNIT_LAST */
 /*_.VALCLO___V98*/ meltfptr[97] =
    (melt_ptr_t) & meltcdat->dclo_74__AT_START_UNIT_LAST;
  meltcdat->dclo_74__AT_START_UNIT_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_74__AT_START_UNIT_LAST.nbval = 0;


/*iniobj dsym_75__FINISH_UNIT_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V99*/ meltfptr[98])
/*_.VALDATA___V99*/ meltfptr[98] =
      (melt_ptr_t) & meltcdat->dsym_75__FINISH_UNIT_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_75__FINISH_UNIT_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_75__FINISH_UNIT_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_75__FINISH_UNIT_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_75__FINISH_UNIT_DELAYED_QUEUE.obj_hash = 456223898;
  meltcdat->dsym_75__FINISH_UNIT_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_76__FINISH_UNIT_DELA*/
 /*_.VALSTR___V100*/ meltfptr[99] =
    (melt_ptr_t) & meltcdat->dstr_76__FINISH_UNIT_DELA;
  meltcdat->dstr_76__FINISH_UNIT_DELA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_76__FINISH_UNIT_DELA.val,
				"FINISH_UNIT_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_76__FINISH_UNIT_DELA.
					val) - 1);
  meltcdat->dstr_76__FINISH_UNIT_DELA.val[25] = (char) 0;
  meltcdat->dstr_76__FINISH_UNIT_DELA.slen = 25;

  MELT_LOCATION ("warmelt-hooks.melt:347:/ iniobj");

/*iniobj dobj_77__FINISH_UNIT_DELAYED_QUEUE*/
/*_.VALDATA___V101*/ meltfptr[100] =
    (melt_ptr_t) & meltcdat->dobj_77__FINISH_UNIT_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_77__FINISH_UNIT_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_77__FINISH_UNIT_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_77__FINISH_UNIT_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_77__FINISH_UNIT_DELAYED_QUEUE.obj_hash = 530065885;
  meltcdat->dobj_77__FINISH_UNIT_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_78__finish_unit_dela*/
 /*_.VALSTR___V104*/ meltfptr[103] =
    (melt_ptr_t) & meltcdat->dstr_78__finish_unit_dela;
  meltcdat->dstr_78__finish_unit_dela.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_78__finish_unit_dela.val,
				"finish_unit_delayed_queue",
				sizeof (meltcdat->dstr_78__finish_unit_dela.
					val) - 1);
  meltcdat->dstr_78__finish_unit_dela.val[25] = (char) 0;
  meltcdat->dstr_78__finish_unit_dela.slen = 25;


/*inihook dhook__79__HOOK_FINISH_UNIT */
  meltcdat->dhook__79__HOOK_FINISH_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_FINISH_UNIT data */
  meltcdat->dhook__79__HOOK_FINISH_UNIT.nbval = 7;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__79__HOOK_FINISH_UNIT,
			  melthook_HOOK_FINISH_UNIT);
  meltcdat->dhook__79__HOOK_FINISH_UNIT.hookdata =
    /*_.VALDATA___V109*/ meltfptr[108];
  strncpy (meltcdat->dhook__79__HOOK_FINISH_UNIT.hookname, "HOOK_FINISH_UNIT",
	   sizeof (meltcdat->dhook__79__HOOK_FINISH_UNIT.hookname) - 1);
/*predefinedhook HOOK_FINISH_UNIT for dhook__79__HOOK_FINISH_UNIT*/
  if (!MELT_PREDEF (HOOK_FINISH_UNIT))
    MELT_STORE_PREDEF (HOOK_FINISH_UNIT,
		       (melt_ptr_t) & meltcdat->dhook__79__HOOK_FINISH_UNIT);

  /*_.VALHOOK___V105*/ meltfptr[104] =
  /*inihooklocvar HOOK_FINISH_UNIT */
  (melt_ptr_t) & meltcdat->dhook__79__HOOK_FINISH_UNIT;

  /*done inihook dhook__79__HOOK_FINISH_UNIT */

  MELT_LOCATION ("warmelt-hooks.melt:361:/ iniobj");

/*iniobj dobj_80__*/
/*_.VALDATA___V109*/ meltfptr[108] = (melt_ptr_t) & meltcdat->dobj_80__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_80__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_80__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_80__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_80__.obj_hash = 151143219;
  meltcdat->dobj_80__.obj_len = 6;

  /*inimult dtup_81___HOOK_FINISH_UNIT */
 /*_.VALTUP___V111*/ meltfptr[110] =
    (melt_ptr_t) & meltcdat->dtup_81___HOOK_FINISH_UNIT;
  meltcdat->dtup_81___HOOK_FINISH_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_81___HOOK_FINISH_UNIT.nbval = 0;
  /*inimult dtup_82___HOOK_FINISH_UNIT */
 /*_.VALTUP___V112*/ meltfptr[111] =
    (melt_ptr_t) & meltcdat->dtup_82___HOOK_FINISH_UNIT;
  meltcdat->dtup_82___HOOK_FINISH_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_82___HOOK_FINISH_UNIT.nbval = 0;
  /*inirout drout_83__LAMBDA_ */
 /*_.VALROUT___V106*/ meltfptr[105] =
    (melt_ptr_t) & meltcdat->drout_83__LAMBDA_;
  meltcdat->drout_83__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_83__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:371", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_83__LAMBDA_.nbval = 1;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_83__LAMBDA_,
			     meltrout_16_warmelt_hooks_LAMBDA___4__);

  /*inirout drout_84__LAMBDA_ */
 /*_.VALROUT___V107*/ meltfptr[106] =
    (melt_ptr_t) & meltcdat->drout_84__LAMBDA_;
  meltcdat->drout_84__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_84__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:380", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_84__LAMBDA_.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_84__LAMBDA_,
			     meltrout_17_warmelt_hooks_LAMBDA___5__);

  /*inirout drout_85__LAMBDA_ */
 /*_.VALROUT___V108*/ meltfptr[107] =
    (melt_ptr_t) & meltcdat->drout_85__LAMBDA_;
  meltcdat->drout_85__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_85__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:385", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_85__LAMBDA_.nbval = 1;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_85__LAMBDA_,
			     meltrout_18_warmelt_hooks_LAMBDA___6__);

  /*inirout drout_86__ENABLE_FINISH_UNIT */
 /*_.VALROUT___V113*/ meltfptr[112] =
    (melt_ptr_t) & meltcdat->drout_86__ENABLE_FINISH_UNIT;
  meltcdat->drout_86__ENABLE_FINISH_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_86__ENABLE_FINISH_UNIT.routdescr,
	   "ENABLE_FINISH_UNIT @warmelt-hooks.melt:413",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_86__ENABLE_FINISH_UNIT.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_86__ENABLE_FINISH_UNIT,
			     meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT);

  /*iniclos dclo_87__ENABLE_FINISH_UNIT */
 /*_.VALCLO___V114*/ meltfptr[113] =
    (melt_ptr_t) & meltcdat->dclo_87__ENABLE_FINISH_UNIT;
  meltcdat->dclo_87__ENABLE_FINISH_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_87__ENABLE_FINISH_UNIT.nbval = 0;

  /*inirout drout_88__AT_FINISH_UNIT_FIRST */
 /*_.VALROUT___V115*/ meltfptr[114] =
    (melt_ptr_t) & meltcdat->drout_88__AT_FINISH_UNIT_FIRST;
  meltcdat->drout_88__AT_FINISH_UNIT_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_88__AT_FINISH_UNIT_FIRST.routdescr,
	   "AT_FINISH_UNIT_FIRST @warmelt-hooks.melt:427",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_88__AT_FINISH_UNIT_FIRST.nbval = 2;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_88__AT_FINISH_UNIT_FIRST,
			     meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST);

  /*iniclos dclo_89__AT_FINISH_UNIT_FIRST */
 /*_.VALCLO___V116*/ meltfptr[115] =
    (melt_ptr_t) & meltcdat->dclo_89__AT_FINISH_UNIT_FIRST;
  meltcdat->dclo_89__AT_FINISH_UNIT_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_89__AT_FINISH_UNIT_FIRST.nbval = 0;

  /*inirout drout_90__AT_FINISH_UNIT_LAST */
 /*_.VALROUT___V117*/ meltfptr[116] =
    (melt_ptr_t) & meltcdat->drout_90__AT_FINISH_UNIT_LAST;
  meltcdat->drout_90__AT_FINISH_UNIT_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_90__AT_FINISH_UNIT_LAST.routdescr,
	   "AT_FINISH_UNIT_LAST @warmelt-hooks.melt:437",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_90__AT_FINISH_UNIT_LAST.nbval = 2;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_90__AT_FINISH_UNIT_LAST,
			     meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST);

  /*iniclos dclo_91__AT_FINISH_UNIT_LAST */
 /*_.VALCLO___V118*/ meltfptr[117] =
    (melt_ptr_t) & meltcdat->dclo_91__AT_FINISH_UNIT_LAST;
  meltcdat->dclo_91__AT_FINISH_UNIT_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_91__AT_FINISH_UNIT_LAST.nbval = 0;


/*iniobj dsym_92__ALL_PASSES_START_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V119*/ meltfptr[118])
/*_.VALDATA___V119*/ meltfptr[118] =
      (melt_ptr_t) & meltcdat->dsym_92__ALL_PASSES_START_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_92__ALL_PASSES_START_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_92__ALL_PASSES_START_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_92__ALL_PASSES_START_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_92__ALL_PASSES_START_DELAYED_QUEUE.obj_hash = 676258517;
  meltcdat->dsym_92__ALL_PASSES_START_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_93__ALL_PASSES_START*/
 /*_.VALSTR___V120*/ meltfptr[119] =
    (melt_ptr_t) & meltcdat->dstr_93__ALL_PASSES_START;
  meltcdat->dstr_93__ALL_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_93__ALL_PASSES_START.val,
				"ALL_PASSES_START_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_93__ALL_PASSES_START.
					val) - 1);
  meltcdat->dstr_93__ALL_PASSES_START.val[30] = (char) 0;
  meltcdat->dstr_93__ALL_PASSES_START.slen = 30;

  MELT_LOCATION ("warmelt-hooks.melt:458:/ iniobj");

/*iniobj dobj_94__ALL_PASSES_START_DELAYED_QUEUE*/
/*_.VALDATA___V121*/ meltfptr[120] =
    (melt_ptr_t) & meltcdat->dobj_94__ALL_PASSES_START_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_94__ALL_PASSES_START_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_94__ALL_PASSES_START_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_94__ALL_PASSES_START_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_94__ALL_PASSES_START_DELAYED_QUEUE.obj_hash = 1012479001;
  meltcdat->dobj_94__ALL_PASSES_START_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_95__all_passes_start*/
 /*_.VALSTR___V122*/ meltfptr[121] =
    (melt_ptr_t) & meltcdat->dstr_95__all_passes_start;
  meltcdat->dstr_95__all_passes_start.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_95__all_passes_start.val,
				"all_passes_start_delayed_queue",
				sizeof (meltcdat->dstr_95__all_passes_start.
					val) - 1);
  meltcdat->dstr_95__all_passes_start.val[30] = (char) 0;
  meltcdat->dstr_95__all_passes_start.slen = 30;


/*inihook dhook__96__HOOK_ALL_PASSES_START */
  meltcdat->dhook__96__HOOK_ALL_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_ALL_PASSES_START data */
  meltcdat->dhook__96__HOOK_ALL_PASSES_START.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__96__HOOK_ALL_PASSES_START,
			  melthook_HOOK_ALL_PASSES_START);
  meltcdat->dhook__96__HOOK_ALL_PASSES_START.hookdata =
    /*_.VALDATA___V124*/ meltfptr[123];
  strncpy (meltcdat->dhook__96__HOOK_ALL_PASSES_START.hookname,
	   "HOOK_ALL_PASSES_START",
	   sizeof (meltcdat->dhook__96__HOOK_ALL_PASSES_START.hookname) - 1);
/*predefinedhook HOOK_ALL_PASSES_START for dhook__96__HOOK_ALL_PASSES_START*/
  if (!MELT_PREDEF (HOOK_ALL_PASSES_START))
    MELT_STORE_PREDEF (HOOK_ALL_PASSES_START,
		       (melt_ptr_t) & meltcdat->
		       dhook__96__HOOK_ALL_PASSES_START);

  /*_.VALHOOK___V123*/ meltfptr[122] =
  /*inihooklocvar HOOK_ALL_PASSES_START */
  (melt_ptr_t) & meltcdat->dhook__96__HOOK_ALL_PASSES_START;

  /*done inihook dhook__96__HOOK_ALL_PASSES_START */

  MELT_LOCATION ("warmelt-hooks.melt:472:/ iniobj");

/*iniobj dobj_97__*/
/*_.VALDATA___V124*/ meltfptr[123] = (melt_ptr_t) & meltcdat->dobj_97__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_97__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_97__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_97__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_97__.obj_hash = 388126420;
  meltcdat->dobj_97__.obj_len = 6;

  /*inimult dtup_98___HOOK_ALL_PASSES_START */
 /*_.VALTUP___V126*/ meltfptr[125] =
    (melt_ptr_t) & meltcdat->dtup_98___HOOK_ALL_PASSES_START;
  meltcdat->dtup_98___HOOK_ALL_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_98___HOOK_ALL_PASSES_START.nbval = 0;
  /*inimult dtup_99___HOOK_ALL_PASSES_START */
 /*_.VALTUP___V127*/ meltfptr[126] =
    (melt_ptr_t) & meltcdat->dtup_99___HOOK_ALL_PASSES_START;
  meltcdat->dtup_99___HOOK_ALL_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_99___HOOK_ALL_PASSES_START.nbval = 0;
  /*inirout drout_100__ENABLE_ALL_PASSES_START */
 /*_.VALROUT___V128*/ meltfptr[127] =
    (melt_ptr_t) & meltcdat->drout_100__ENABLE_ALL_PASSES_START;
  meltcdat->drout_100__ENABLE_ALL_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_100__ENABLE_ALL_PASSES_START.routdescr,
	   "ENABLE_ALL_PASSES_START @warmelt-hooks.melt:513",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_100__ENABLE_ALL_PASSES_START.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_100__ENABLE_ALL_PASSES_START,
			     meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START);

  /*iniclos dclo_101__ENABLE_ALL_PASSES_START */
 /*_.VALCLO___V129*/ meltfptr[128] =
    (melt_ptr_t) & meltcdat->dclo_101__ENABLE_ALL_PASSES_START;
  meltcdat->dclo_101__ENABLE_ALL_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_101__ENABLE_ALL_PASSES_START.nbval = 0;

  /*inirout drout_102__REGISTER_ALL_PASSES_START_FIRST */
 /*_.VALROUT___V130*/ meltfptr[129] =
    (melt_ptr_t) & meltcdat->drout_102__REGISTER_ALL_PASSES_START_FIRST;
  meltcdat->drout_102__REGISTER_ALL_PASSES_START_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_102__REGISTER_ALL_PASSES_START_FIRST.routdescr,
	   "REGISTER_ALL_PASSES_START_FIRST @warmelt-hooks.melt:526",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_102__REGISTER_ALL_PASSES_START_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_102__REGISTER_ALL_PASSES_START_FIRST,
			     meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST);

  /*iniclos dclo_103__REGISTER_ALL_PASSES_START_FIRST */
 /*_.VALCLO___V131*/ meltfptr[130] =
    (melt_ptr_t) & meltcdat->dclo_103__REGISTER_ALL_PASSES_START_FIRST;
  meltcdat->dclo_103__REGISTER_ALL_PASSES_START_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_103__REGISTER_ALL_PASSES_START_FIRST.nbval = 0;

  /*inirout drout_104__REGISTER_ALL_PASSES_START_LAST */
 /*_.VALROUT___V132*/ meltfptr[131] =
    (melt_ptr_t) & meltcdat->drout_104__REGISTER_ALL_PASSES_START_LAST;
  meltcdat->drout_104__REGISTER_ALL_PASSES_START_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_104__REGISTER_ALL_PASSES_START_LAST.routdescr,
	   "REGISTER_ALL_PASSES_START_LAST @warmelt-hooks.melt:549",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_104__REGISTER_ALL_PASSES_START_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_104__REGISTER_ALL_PASSES_START_LAST,
			     meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST);

  /*iniclos dclo_105__REGISTER_ALL_PASSES_START_LAST */
 /*_.VALCLO___V133*/ meltfptr[132] =
    (melt_ptr_t) & meltcdat->dclo_105__REGISTER_ALL_PASSES_START_LAST;
  meltcdat->dclo_105__REGISTER_ALL_PASSES_START_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_105__REGISTER_ALL_PASSES_START_LAST.nbval = 0;


/*iniobj dsym_106__ALL_PASSES_END_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V134*/ meltfptr[133])
/*_.VALDATA___V134*/ meltfptr[133] =
      (melt_ptr_t) & meltcdat->dsym_106__ALL_PASSES_END_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_106__ALL_PASSES_END_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_106__ALL_PASSES_END_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_106__ALL_PASSES_END_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_106__ALL_PASSES_END_DELAYED_QUEUE.obj_hash = 856751701;
  meltcdat->dsym_106__ALL_PASSES_END_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_107__ALL_PASSES_END_D*/
 /*_.VALSTR___V135*/ meltfptr[134] =
    (melt_ptr_t) & meltcdat->dstr_107__ALL_PASSES_END_D;
  meltcdat->dstr_107__ALL_PASSES_END_D.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_107__ALL_PASSES_END_D.val,
				"ALL_PASSES_END_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_107__ALL_PASSES_END_D.
					val) - 1);
  meltcdat->dstr_107__ALL_PASSES_END_D.val[28] = (char) 0;
  meltcdat->dstr_107__ALL_PASSES_END_D.slen = 28;

  MELT_LOCATION ("warmelt-hooks.melt:589:/ iniobj");

/*iniobj dobj_108__ALL_PASSES_END_DELAYED_QUEUE*/
/*_.VALDATA___V136*/ meltfptr[135] =
    (melt_ptr_t) & meltcdat->dobj_108__ALL_PASSES_END_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_108__ALL_PASSES_END_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_108__ALL_PASSES_END_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_108__ALL_PASSES_END_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_108__ALL_PASSES_END_DELAYED_QUEUE.obj_hash = 985283572;
  meltcdat->dobj_108__ALL_PASSES_END_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_109__all_passes_end_d*/
 /*_.VALSTR___V137*/ meltfptr[136] =
    (melt_ptr_t) & meltcdat->dstr_109__all_passes_end_d;
  meltcdat->dstr_109__all_passes_end_d.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_109__all_passes_end_d.val,
				"all_passes_end_delayed_queue",
				sizeof (meltcdat->dstr_109__all_passes_end_d.
					val) - 1);
  meltcdat->dstr_109__all_passes_end_d.val[28] = (char) 0;
  meltcdat->dstr_109__all_passes_end_d.slen = 28;


/*inihook dhook__110__HOOK_ALL_PASSES_END */
  meltcdat->dhook__110__HOOK_ALL_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_ALL_PASSES_END data */
  meltcdat->dhook__110__HOOK_ALL_PASSES_END.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__110__HOOK_ALL_PASSES_END,
			  melthook_HOOK_ALL_PASSES_END);
  meltcdat->dhook__110__HOOK_ALL_PASSES_END.hookdata =
    /*_.VALDATA___V139*/ meltfptr[138];
  strncpy (meltcdat->dhook__110__HOOK_ALL_PASSES_END.hookname,
	   "HOOK_ALL_PASSES_END",
	   sizeof (meltcdat->dhook__110__HOOK_ALL_PASSES_END.hookname) - 1);
/*predefinedhook HOOK_ALL_PASSES_END for dhook__110__HOOK_ALL_PASSES_END*/
  if (!MELT_PREDEF (HOOK_ALL_PASSES_END))
    MELT_STORE_PREDEF (HOOK_ALL_PASSES_END,
		       (melt_ptr_t) & meltcdat->
		       dhook__110__HOOK_ALL_PASSES_END);

  /*_.VALHOOK___V138*/ meltfptr[137] =
  /*inihooklocvar HOOK_ALL_PASSES_END */
  (melt_ptr_t) & meltcdat->dhook__110__HOOK_ALL_PASSES_END;

  /*done inihook dhook__110__HOOK_ALL_PASSES_END */

  MELT_LOCATION ("warmelt-hooks.melt:596:/ iniobj");

/*iniobj dobj_111__*/
/*_.VALDATA___V139*/ meltfptr[138] = (melt_ptr_t) & meltcdat->dobj_111__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_111__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_111__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_111__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_111__.obj_hash = 500440698;
  meltcdat->dobj_111__.obj_len = 6;

  /*inimult dtup_112___HOOK_ALL_PASSES_END */
 /*_.VALTUP___V141*/ meltfptr[140] =
    (melt_ptr_t) & meltcdat->dtup_112___HOOK_ALL_PASSES_END;
  meltcdat->dtup_112___HOOK_ALL_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_112___HOOK_ALL_PASSES_END.nbval = 0;
  /*inimult dtup_113___HOOK_ALL_PASSES_END */
 /*_.VALTUP___V142*/ meltfptr[141] =
    (melt_ptr_t) & meltcdat->dtup_113___HOOK_ALL_PASSES_END;
  meltcdat->dtup_113___HOOK_ALL_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_113___HOOK_ALL_PASSES_END.nbval = 0;
  /*inirout drout_114__ENABLE_ALL_PASSES_END */
 /*_.VALROUT___V143*/ meltfptr[142] =
    (melt_ptr_t) & meltcdat->drout_114__ENABLE_ALL_PASSES_END;
  meltcdat->drout_114__ENABLE_ALL_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_114__ENABLE_ALL_PASSES_END.routdescr,
	   "ENABLE_ALL_PASSES_END @warmelt-hooks.melt:636",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_114__ENABLE_ALL_PASSES_END.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_114__ENABLE_ALL_PASSES_END,
			     meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END);

  /*iniclos dclo_115__ENABLE_ALL_PASSES_END */
 /*_.VALCLO___V144*/ meltfptr[143] =
    (melt_ptr_t) & meltcdat->dclo_115__ENABLE_ALL_PASSES_END;
  meltcdat->dclo_115__ENABLE_ALL_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_115__ENABLE_ALL_PASSES_END.nbval = 0;

  /*inirout drout_116__REGISTER_ALL_PASSES_END_FIRST */
 /*_.VALROUT___V145*/ meltfptr[144] =
    (melt_ptr_t) & meltcdat->drout_116__REGISTER_ALL_PASSES_END_FIRST;
  meltcdat->drout_116__REGISTER_ALL_PASSES_END_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_116__REGISTER_ALL_PASSES_END_FIRST.routdescr,
	   "REGISTER_ALL_PASSES_END_FIRST @warmelt-hooks.melt:649",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_116__REGISTER_ALL_PASSES_END_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_116__REGISTER_ALL_PASSES_END_FIRST,
			     meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST);

  /*iniclos dclo_117__REGISTER_ALL_PASSES_END_FIRST */
 /*_.VALCLO___V146*/ meltfptr[145] =
    (melt_ptr_t) & meltcdat->dclo_117__REGISTER_ALL_PASSES_END_FIRST;
  meltcdat->dclo_117__REGISTER_ALL_PASSES_END_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_117__REGISTER_ALL_PASSES_END_FIRST.nbval = 0;

  /*inirout drout_118__REGISTER_ALL_PASSES_END_LAST */
 /*_.VALROUT___V147*/ meltfptr[146] =
    (melt_ptr_t) & meltcdat->drout_118__REGISTER_ALL_PASSES_END_LAST;
  meltcdat->drout_118__REGISTER_ALL_PASSES_END_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_118__REGISTER_ALL_PASSES_END_LAST.routdescr,
	   "REGISTER_ALL_PASSES_END_LAST @warmelt-hooks.melt:672",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_118__REGISTER_ALL_PASSES_END_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_118__REGISTER_ALL_PASSES_END_LAST,
			     meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST);

  /*iniclos dclo_119__REGISTER_ALL_PASSES_END_LAST */
 /*_.VALCLO___V148*/ meltfptr[147] =
    (melt_ptr_t) & meltcdat->dclo_119__REGISTER_ALL_PASSES_END_LAST;
  meltcdat->dclo_119__REGISTER_ALL_PASSES_END_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_119__REGISTER_ALL_PASSES_END_LAST.nbval = 0;


/*iniobj dsym_120__ALL_IPA_PASSES_START_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V149*/ meltfptr[148])
/*_.VALDATA___V149*/ meltfptr[148] =
      (melt_ptr_t) & meltcdat->dsym_120__ALL_IPA_PASSES_START_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_120__ALL_IPA_PASSES_START_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_120__ALL_IPA_PASSES_START_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_120__ALL_IPA_PASSES_START_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_120__ALL_IPA_PASSES_START_DELAYED_QUEUE.obj_hash = 919668591;
  meltcdat->dsym_120__ALL_IPA_PASSES_START_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_121__ALL_IPA_PASSES_S*/
 /*_.VALSTR___V150*/ meltfptr[149] =
    (melt_ptr_t) & meltcdat->dstr_121__ALL_IPA_PASSES_S;
  meltcdat->dstr_121__ALL_IPA_PASSES_S.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_121__ALL_IPA_PASSES_S.val,
				"ALL_IPA_PASSES_START_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_121__ALL_IPA_PASSES_S.
					val) - 1);
  meltcdat->dstr_121__ALL_IPA_PASSES_S.val[34] = (char) 0;
  meltcdat->dstr_121__ALL_IPA_PASSES_S.slen = 34;

  MELT_LOCATION ("warmelt-hooks.melt:703:/ iniobj");

/*iniobj dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE*/
/*_.VALDATA___V151*/ meltfptr[150] =
    (melt_ptr_t) & meltcdat->dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE.obj_hash = 233335382;
  meltcdat->dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_123__all_ipa_passes_s*/
 /*_.VALSTR___V152*/ meltfptr[151] =
    (melt_ptr_t) & meltcdat->dstr_123__all_ipa_passes_s;
  meltcdat->dstr_123__all_ipa_passes_s.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_123__all_ipa_passes_s.val,
				"all_ipa_passes_start_delayed_queue",
				sizeof (meltcdat->dstr_123__all_ipa_passes_s.
					val) - 1);
  meltcdat->dstr_123__all_ipa_passes_s.val[34] = (char) 0;
  meltcdat->dstr_123__all_ipa_passes_s.slen = 34;


/*inihook dhook__124__HOOK_ALL_IPA_PASSES_START */
  meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_ALL_IPA_PASSES_START data */
  meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START,
			  melthook_HOOK_ALL_IPA_PASSES_START);
  meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START.hookdata =
    /*_.VALDATA___V154*/ meltfptr[153];
  strncpy (meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START.hookname,
	   "HOOK_ALL_IPA_PASSES_START",
	   sizeof (meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START.hookname) -
	   1);
/*predefinedhook HOOK_ALL_IPA_PASSES_START for dhook__124__HOOK_ALL_IPA_PASSES_START*/
  if (!MELT_PREDEF (HOOK_ALL_IPA_PASSES_START))
    MELT_STORE_PREDEF (HOOK_ALL_IPA_PASSES_START,
		       (melt_ptr_t) & meltcdat->
		       dhook__124__HOOK_ALL_IPA_PASSES_START);

  /*_.VALHOOK___V153*/ meltfptr[152] =
  /*inihooklocvar HOOK_ALL_IPA_PASSES_START */
  (melt_ptr_t) & meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START;

  /*done inihook dhook__124__HOOK_ALL_IPA_PASSES_START */

  MELT_LOCATION ("warmelt-hooks.melt:717:/ iniobj");

/*iniobj dobj_125__*/
/*_.VALDATA___V154*/ meltfptr[153] = (melt_ptr_t) & meltcdat->dobj_125__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_125__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_125__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_125__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_125__.obj_hash = 796060678;
  meltcdat->dobj_125__.obj_len = 6;

  /*inimult dtup_126___HOOK_ALL_IPA_PASSES_START */
 /*_.VALTUP___V156*/ meltfptr[155] =
    (melt_ptr_t) & meltcdat->dtup_126___HOOK_ALL_IPA_PASSES_START;
  meltcdat->dtup_126___HOOK_ALL_IPA_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_126___HOOK_ALL_IPA_PASSES_START.nbval = 0;
  /*inimult dtup_127___HOOK_ALL_IPA_PASSES_START */
 /*_.VALTUP___V157*/ meltfptr[156] =
    (melt_ptr_t) & meltcdat->dtup_127___HOOK_ALL_IPA_PASSES_START;
  meltcdat->dtup_127___HOOK_ALL_IPA_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_127___HOOK_ALL_IPA_PASSES_START.nbval = 0;
  /*inirout drout_128__ENABLE_ALL_IPA_PASSES_START */
 /*_.VALROUT___V158*/ meltfptr[157] =
    (melt_ptr_t) & meltcdat->drout_128__ENABLE_ALL_IPA_PASSES_START;
  meltcdat->drout_128__ENABLE_ALL_IPA_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_128__ENABLE_ALL_IPA_PASSES_START.routdescr,
	   "ENABLE_ALL_IPA_PASSES_START @warmelt-hooks.melt:758",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_128__ENABLE_ALL_IPA_PASSES_START.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_128__ENABLE_ALL_IPA_PASSES_START,
			     meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START);

  /*iniclos dclo_129__ENABLE_ALL_IPA_PASSES_START */
 /*_.VALCLO___V159*/ meltfptr[158] =
    (melt_ptr_t) & meltcdat->dclo_129__ENABLE_ALL_IPA_PASSES_START;
  meltcdat->dclo_129__ENABLE_ALL_IPA_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_129__ENABLE_ALL_IPA_PASSES_START.nbval = 0;

  /*inirout drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST */
 /*_.VALROUT___V160*/ meltfptr[159] =
    (melt_ptr_t) & meltcdat->drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST;
  meltcdat->drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST.routdescr,
	   "REGISTER_ALL_IPA_PASSES_START_FIRST @warmelt-hooks.melt:771",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST,
			     meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST);

  /*iniclos dclo_131__REGISTER_ALL_IPA_PASSES_START_FIRST */
 /*_.VALCLO___V161*/ meltfptr[160] =
    (melt_ptr_t) & meltcdat->dclo_131__REGISTER_ALL_IPA_PASSES_START_FIRST;
  meltcdat->dclo_131__REGISTER_ALL_IPA_PASSES_START_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_131__REGISTER_ALL_IPA_PASSES_START_FIRST.nbval = 0;

  /*inirout drout_132__REGISTER_ALL_IPA_PASSES_START_LAST */
 /*_.VALROUT___V162*/ meltfptr[161] =
    (melt_ptr_t) & meltcdat->drout_132__REGISTER_ALL_IPA_PASSES_START_LAST;
  meltcdat->drout_132__REGISTER_ALL_IPA_PASSES_START_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_132__REGISTER_ALL_IPA_PASSES_START_LAST.routdescr,
	   "REGISTER_ALL_IPA_PASSES_START_LAST @warmelt-hooks.melt:794",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_132__REGISTER_ALL_IPA_PASSES_START_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_132__REGISTER_ALL_IPA_PASSES_START_LAST,
			     meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST);

  /*iniclos dclo_133__REGISTER_ALL_IPA_PASSES_START_LAST */
 /*_.VALCLO___V163*/ meltfptr[162] =
    (melt_ptr_t) & meltcdat->dclo_133__REGISTER_ALL_IPA_PASSES_START_LAST;
  meltcdat->dclo_133__REGISTER_ALL_IPA_PASSES_START_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_133__REGISTER_ALL_IPA_PASSES_START_LAST.nbval = 0;


/*iniobj dsym_134__ALL_IPA_PASSES_END_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V164*/ meltfptr[163])
/*_.VALDATA___V164*/ meltfptr[163] =
      (melt_ptr_t) & meltcdat->dsym_134__ALL_IPA_PASSES_END_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_134__ALL_IPA_PASSES_END_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_134__ALL_IPA_PASSES_END_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_134__ALL_IPA_PASSES_END_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_134__ALL_IPA_PASSES_END_DELAYED_QUEUE.obj_hash = 887070733;
  meltcdat->dsym_134__ALL_IPA_PASSES_END_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_135__ALL_IPA_PASSES_E*/
 /*_.VALSTR___V165*/ meltfptr[164] =
    (melt_ptr_t) & meltcdat->dstr_135__ALL_IPA_PASSES_E;
  meltcdat->dstr_135__ALL_IPA_PASSES_E.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_135__ALL_IPA_PASSES_E.val,
				"ALL_IPA_PASSES_END_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_135__ALL_IPA_PASSES_E.
					val) - 1);
  meltcdat->dstr_135__ALL_IPA_PASSES_E.val[32] = (char) 0;
  meltcdat->dstr_135__ALL_IPA_PASSES_E.slen = 32;

  MELT_LOCATION ("warmelt-hooks.melt:834:/ iniobj");

/*iniobj dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE*/
/*_.VALDATA___V166*/ meltfptr[165] =
    (melt_ptr_t) & meltcdat->dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE.obj_hash = 813597501;
  meltcdat->dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_137__all_ipa_passes_e*/
 /*_.VALSTR___V167*/ meltfptr[166] =
    (melt_ptr_t) & meltcdat->dstr_137__all_ipa_passes_e;
  meltcdat->dstr_137__all_ipa_passes_e.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_137__all_ipa_passes_e.val,
				"all_ipa_passes_end_delayed_queue",
				sizeof (meltcdat->dstr_137__all_ipa_passes_e.
					val) - 1);
  meltcdat->dstr_137__all_ipa_passes_e.val[32] = (char) 0;
  meltcdat->dstr_137__all_ipa_passes_e.slen = 32;


/*inihook dhook__138__HOOK_ALL_IPA_PASSES_END */
  meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_ALL_IPA_PASSES_END data */
  meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END,
			  melthook_HOOK_ALL_IPA_PASSES_END);
  meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END.hookdata =
    /*_.VALDATA___V169*/ meltfptr[168];
  strncpy (meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END.hookname,
	   "HOOK_ALL_IPA_PASSES_END",
	   sizeof (meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END.hookname) -
	   1);
/*predefinedhook HOOK_ALL_IPA_PASSES_END for dhook__138__HOOK_ALL_IPA_PASSES_END*/
  if (!MELT_PREDEF (HOOK_ALL_IPA_PASSES_END))
    MELT_STORE_PREDEF (HOOK_ALL_IPA_PASSES_END,
		       (melt_ptr_t) & meltcdat->
		       dhook__138__HOOK_ALL_IPA_PASSES_END);

  /*_.VALHOOK___V168*/ meltfptr[167] =
  /*inihooklocvar HOOK_ALL_IPA_PASSES_END */
  (melt_ptr_t) & meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END;

  /*done inihook dhook__138__HOOK_ALL_IPA_PASSES_END */

  MELT_LOCATION ("warmelt-hooks.melt:841:/ iniobj");

/*iniobj dobj_139__*/
/*_.VALDATA___V169*/ meltfptr[168] = (melt_ptr_t) & meltcdat->dobj_139__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_139__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_139__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_139__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_139__.obj_hash = 361805378;
  meltcdat->dobj_139__.obj_len = 6;

  /*inimult dtup_140___HOOK_ALL_IPA_PASSES_END */
 /*_.VALTUP___V171*/ meltfptr[170] =
    (melt_ptr_t) & meltcdat->dtup_140___HOOK_ALL_IPA_PASSES_END;
  meltcdat->dtup_140___HOOK_ALL_IPA_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_140___HOOK_ALL_IPA_PASSES_END.nbval = 0;
  /*inimult dtup_141___HOOK_ALL_IPA_PASSES_END */
 /*_.VALTUP___V172*/ meltfptr[171] =
    (melt_ptr_t) & meltcdat->dtup_141___HOOK_ALL_IPA_PASSES_END;
  meltcdat->dtup_141___HOOK_ALL_IPA_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_141___HOOK_ALL_IPA_PASSES_END.nbval = 0;
  /*inirout drout_142__ENABLE_ALL_IPA_PASSES_END */
 /*_.VALROUT___V173*/ meltfptr[172] =
    (melt_ptr_t) & meltcdat->drout_142__ENABLE_ALL_IPA_PASSES_END;
  meltcdat->drout_142__ENABLE_ALL_IPA_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_142__ENABLE_ALL_IPA_PASSES_END.routdescr,
	   "ENABLE_ALL_IPA_PASSES_END @warmelt-hooks.melt:881",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_142__ENABLE_ALL_IPA_PASSES_END.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_142__ENABLE_ALL_IPA_PASSES_END,
			     meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END);

  /*iniclos dclo_143__ENABLE_ALL_IPA_PASSES_END */
 /*_.VALCLO___V174*/ meltfptr[173] =
    (melt_ptr_t) & meltcdat->dclo_143__ENABLE_ALL_IPA_PASSES_END;
  meltcdat->dclo_143__ENABLE_ALL_IPA_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_143__ENABLE_ALL_IPA_PASSES_END.nbval = 0;

  /*inirout drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST */
 /*_.VALROUT___V175*/ meltfptr[174] =
    (melt_ptr_t) & meltcdat->drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST;
  meltcdat->drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST.routdescr,
	   "REGISTER_ALL_IPA_PASSES_END_FIRST @warmelt-hooks.melt:894",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST,
			     meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST);

  /*iniclos dclo_145__REGISTER_ALL_IPA_PASSES_END_FIRST */
 /*_.VALCLO___V176*/ meltfptr[175] =
    (melt_ptr_t) & meltcdat->dclo_145__REGISTER_ALL_IPA_PASSES_END_FIRST;
  meltcdat->dclo_145__REGISTER_ALL_IPA_PASSES_END_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_145__REGISTER_ALL_IPA_PASSES_END_FIRST.nbval = 0;

  /*inirout drout_146__REGISTER_ALL_IPA_PASSES_END_LAST */
 /*_.VALROUT___V177*/ meltfptr[176] =
    (melt_ptr_t) & meltcdat->drout_146__REGISTER_ALL_IPA_PASSES_END_LAST;
  meltcdat->drout_146__REGISTER_ALL_IPA_PASSES_END_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_146__REGISTER_ALL_IPA_PASSES_END_LAST.routdescr,
	   "REGISTER_ALL_IPA_PASSES_END_LAST @warmelt-hooks.melt:917",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_146__REGISTER_ALL_IPA_PASSES_END_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_146__REGISTER_ALL_IPA_PASSES_END_LAST,
			     meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST);

  /*iniclos dclo_147__REGISTER_ALL_IPA_PASSES_END_LAST */
 /*_.VALCLO___V178*/ meltfptr[177] =
    (melt_ptr_t) & meltcdat->dclo_147__REGISTER_ALL_IPA_PASSES_END_LAST;
  meltcdat->dclo_147__REGISTER_ALL_IPA_PASSES_END_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_147__REGISTER_ALL_IPA_PASSES_END_LAST.nbval = 0;


/*iniobj dsym_148__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V179*/ meltfptr[178])
/*_.VALDATA___V179*/ meltfptr[178] =
      (melt_ptr_t) & meltcdat->
      dsym_148__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_148__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_148__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_148__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_148__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE.obj_hash =
    893744541;
  meltcdat->dsym_148__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_149__EARLY_GIMPLE_PAS*/
 /*_.VALSTR___V180*/ meltfptr[179] =
    (melt_ptr_t) & meltcdat->dstr_149__EARLY_GIMPLE_PAS;
  meltcdat->dstr_149__EARLY_GIMPLE_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_149__EARLY_GIMPLE_PAS.val,
				"EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_149__EARLY_GIMPLE_PAS.
					val) - 1);
  meltcdat->dstr_149__EARLY_GIMPLE_PAS.val[39] = (char) 0;
  meltcdat->dstr_149__EARLY_GIMPLE_PAS.slen = 39;

  MELT_LOCATION ("warmelt-hooks.melt:949:/ iniobj");

/*iniobj dobj_150__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE*/
/*_.VALDATA___V181*/ meltfptr[180] =
    (melt_ptr_t) & meltcdat->
    dobj_150__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_150__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_150__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_150__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_150__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE.obj_hash =
    755843179;
  meltcdat->dobj_150__EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_151__early_gimple_pas*/
 /*_.VALSTR___V182*/ meltfptr[181] =
    (melt_ptr_t) & meltcdat->dstr_151__early_gimple_pas;
  meltcdat->dstr_151__early_gimple_pas.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_151__early_gimple_pas.val,
				"early_gimple_passes_start_delayed_queue",
				sizeof (meltcdat->dstr_151__early_gimple_pas.
					val) - 1);
  meltcdat->dstr_151__early_gimple_pas.val[39] = (char) 0;
  meltcdat->dstr_151__early_gimple_pas.slen = 39;


/*inihook dhook__152__HOOK_EARLY_GIMPLE_PASSES_START */
  meltcdat->dhook__152__HOOK_EARLY_GIMPLE_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_EARLY_GIMPLE_PASSES_START data */
  meltcdat->dhook__152__HOOK_EARLY_GIMPLE_PASSES_START.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->
			  dhook__152__HOOK_EARLY_GIMPLE_PASSES_START,
			  melthook_HOOK_EARLY_GIMPLE_PASSES_START);
  meltcdat->dhook__152__HOOK_EARLY_GIMPLE_PASSES_START.hookdata =
    /*_.VALDATA___V184*/ meltfptr[183];
  strncpy (meltcdat->dhook__152__HOOK_EARLY_GIMPLE_PASSES_START.hookname,
	   "HOOK_EARLY_GIMPLE_PASSES_START",
	   sizeof (meltcdat->dhook__152__HOOK_EARLY_GIMPLE_PASSES_START.
		   hookname) - 1);
/*predefinedhook HOOK_EARLY_GIMPLE_PASSES_START for dhook__152__HOOK_EARLY_GIMPLE_PASSES_START*/
  if (!MELT_PREDEF (HOOK_EARLY_GIMPLE_PASSES_START))
    MELT_STORE_PREDEF (HOOK_EARLY_GIMPLE_PASSES_START,
		       (melt_ptr_t) & meltcdat->
		       dhook__152__HOOK_EARLY_GIMPLE_PASSES_START);

  /*_.VALHOOK___V183*/ meltfptr[182] =
  /*inihooklocvar HOOK_EARLY_GIMPLE_PASSES_START */
  (melt_ptr_t) & meltcdat->dhook__152__HOOK_EARLY_GIMPLE_PASSES_START;

  /*done inihook dhook__152__HOOK_EARLY_GIMPLE_PASSES_START */

  MELT_LOCATION ("warmelt-hooks.melt:963:/ iniobj");

/*iniobj dobj_153__*/
/*_.VALDATA___V184*/ meltfptr[183] = (melt_ptr_t) & meltcdat->dobj_153__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_153__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_153__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_153__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_153__.obj_hash = 484828793;
  meltcdat->dobj_153__.obj_len = 6;

  /*inimult dtup_154___HOOK_EARLY_GIMPLE_PASSES_START */
 /*_.VALTUP___V186*/ meltfptr[185] =
    (melt_ptr_t) & meltcdat->dtup_154___HOOK_EARLY_GIMPLE_PASSES_START;
  meltcdat->dtup_154___HOOK_EARLY_GIMPLE_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_154___HOOK_EARLY_GIMPLE_PASSES_START.nbval = 0;
  /*inimult dtup_155___HOOK_EARLY_GIMPLE_PASSES_START */
 /*_.VALTUP___V187*/ meltfptr[186] =
    (melt_ptr_t) & meltcdat->dtup_155___HOOK_EARLY_GIMPLE_PASSES_START;
  meltcdat->dtup_155___HOOK_EARLY_GIMPLE_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_155___HOOK_EARLY_GIMPLE_PASSES_START.nbval = 0;
  /*inirout drout_156__ENABLE_EARLY_GIMPLE_PASSES_START */
 /*_.VALROUT___V188*/ meltfptr[187] =
    (melt_ptr_t) & meltcdat->drout_156__ENABLE_EARLY_GIMPLE_PASSES_START;
  meltcdat->drout_156__ENABLE_EARLY_GIMPLE_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_156__ENABLE_EARLY_GIMPLE_PASSES_START.routdescr,
	   "ENABLE_EARLY_GIMPLE_PASSES_START @warmelt-hooks.melt:1004",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_156__ENABLE_EARLY_GIMPLE_PASSES_START.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_156__ENABLE_EARLY_GIMPLE_PASSES_START,
			     meltrout_40_warmelt_hooks_ENABLE_EARLY_GIMPLE_PASSES_START);

  /*iniclos dclo_157__ENABLE_EARLY_GIMPLE_PASSES_START */
 /*_.VALCLO___V189*/ meltfptr[188] =
    (melt_ptr_t) & meltcdat->dclo_157__ENABLE_EARLY_GIMPLE_PASSES_START;
  meltcdat->dclo_157__ENABLE_EARLY_GIMPLE_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_157__ENABLE_EARLY_GIMPLE_PASSES_START.nbval = 0;

  /*inirout drout_158__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST */
 /*_.VALROUT___V190*/ meltfptr[189] =
    (melt_ptr_t) & meltcdat->
    drout_158__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST;
  meltcdat->drout_158__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_158__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST.
	   routdescr,
	   "REGISTER_EARLY_GIMPLE_PASSES_START_FIRST @warmelt-hooks.melt:1017",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_158__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_158__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST,
			     meltrout_41_warmelt_hooks_REGISTER_EARLY_GIMPLE_PASSES_START_FIRST);

  /*iniclos dclo_159__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST */
 /*_.VALCLO___V191*/ meltfptr[190] =
    (melt_ptr_t) & meltcdat->
    dclo_159__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST;
  meltcdat->dclo_159__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_159__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST.nbval = 0;

  /*inirout drout_160__REGISTER_EARLY_GIMPLE_PASSES_START_LAST */
 /*_.VALROUT___V192*/ meltfptr[191] =
    (melt_ptr_t) & meltcdat->
    drout_160__REGISTER_EARLY_GIMPLE_PASSES_START_LAST;
  meltcdat->drout_160__REGISTER_EARLY_GIMPLE_PASSES_START_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_160__REGISTER_EARLY_GIMPLE_PASSES_START_LAST.
	   routdescr,
	   "REGISTER_EARLY_GIMPLE_PASSES_START_LAST @warmelt-hooks.melt:1040",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_160__REGISTER_EARLY_GIMPLE_PASSES_START_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_160__REGISTER_EARLY_GIMPLE_PASSES_START_LAST,
			     meltrout_42_warmelt_hooks_REGISTER_EARLY_GIMPLE_PASSES_START_LAST);

  /*iniclos dclo_161__REGISTER_EARLY_GIMPLE_PASSES_START_LAST */
 /*_.VALCLO___V193*/ meltfptr[192] =
    (melt_ptr_t) & meltcdat->
    dclo_161__REGISTER_EARLY_GIMPLE_PASSES_START_LAST;
  meltcdat->dclo_161__REGISTER_EARLY_GIMPLE_PASSES_START_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_161__REGISTER_EARLY_GIMPLE_PASSES_START_LAST.nbval = 0;


/*iniobj dsym_162__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V194*/ meltfptr[193])
/*_.VALDATA___V194*/ meltfptr[193] =
      (melt_ptr_t) & meltcdat->
      dsym_162__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_162__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_162__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_162__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_162__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE.obj_hash =
    232373921;
  meltcdat->dsym_162__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_163__EARLY_GIMPLE_PAS*/
 /*_.VALSTR___V195*/ meltfptr[194] =
    (melt_ptr_t) & meltcdat->dstr_163__EARLY_GIMPLE_PAS;
  meltcdat->dstr_163__EARLY_GIMPLE_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_163__EARLY_GIMPLE_PAS.val,
				"EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_163__EARLY_GIMPLE_PAS.
					val) - 1);
  meltcdat->dstr_163__EARLY_GIMPLE_PAS.val[37] = (char) 0;
  meltcdat->dstr_163__EARLY_GIMPLE_PAS.slen = 37;

  MELT_LOCATION ("warmelt-hooks.melt:1080:/ iniobj");

/*iniobj dobj_164__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/
/*_.VALDATA___V196*/ meltfptr[195] =
    (melt_ptr_t) & meltcdat->dobj_164__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_164__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_164__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_164__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_164__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE.obj_hash =
    77443544;
  meltcdat->dobj_164__EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_165__early_gimple_pas*/
 /*_.VALSTR___V197*/ meltfptr[196] =
    (melt_ptr_t) & meltcdat->dstr_165__early_gimple_pas;
  meltcdat->dstr_165__early_gimple_pas.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_165__early_gimple_pas.val,
				"early_gimple_passes_end_delayed_queue",
				sizeof (meltcdat->dstr_165__early_gimple_pas.
					val) - 1);
  meltcdat->dstr_165__early_gimple_pas.val[37] = (char) 0;
  meltcdat->dstr_165__early_gimple_pas.slen = 37;


/*inihook dhook__166__HOOK_EARLY_GIMPLE_PASSES_END */
  meltcdat->dhook__166__HOOK_EARLY_GIMPLE_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_EARLY_GIMPLE_PASSES_END data */
  meltcdat->dhook__166__HOOK_EARLY_GIMPLE_PASSES_END.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__166__HOOK_EARLY_GIMPLE_PASSES_END,
			  melthook_HOOK_EARLY_GIMPLE_PASSES_END);
  meltcdat->dhook__166__HOOK_EARLY_GIMPLE_PASSES_END.hookdata =
    /*_.VALDATA___V199*/ meltfptr[198];
  strncpy (meltcdat->dhook__166__HOOK_EARLY_GIMPLE_PASSES_END.hookname,
	   "HOOK_EARLY_GIMPLE_PASSES_END",
	   sizeof (meltcdat->dhook__166__HOOK_EARLY_GIMPLE_PASSES_END.
		   hookname) - 1);
/*predefinedhook HOOK_EARLY_GIMPLE_PASSES_END for dhook__166__HOOK_EARLY_GIMPLE_PASSES_END*/
  if (!MELT_PREDEF (HOOK_EARLY_GIMPLE_PASSES_END))
    MELT_STORE_PREDEF (HOOK_EARLY_GIMPLE_PASSES_END,
		       (melt_ptr_t) & meltcdat->
		       dhook__166__HOOK_EARLY_GIMPLE_PASSES_END);

  /*_.VALHOOK___V198*/ meltfptr[197] =
  /*inihooklocvar HOOK_EARLY_GIMPLE_PASSES_END */
  (melt_ptr_t) & meltcdat->dhook__166__HOOK_EARLY_GIMPLE_PASSES_END;

  /*done inihook dhook__166__HOOK_EARLY_GIMPLE_PASSES_END */

  MELT_LOCATION ("warmelt-hooks.melt:1087:/ iniobj");

/*iniobj dobj_167__*/
/*_.VALDATA___V199*/ meltfptr[198] = (melt_ptr_t) & meltcdat->dobj_167__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_167__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_167__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_167__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_167__.obj_hash = 421475679;
  meltcdat->dobj_167__.obj_len = 6;

  /*inimult dtup_168___HOOK_EARLY_GIMPLE_PASSES_END */
 /*_.VALTUP___V201*/ meltfptr[200] =
    (melt_ptr_t) & meltcdat->dtup_168___HOOK_EARLY_GIMPLE_PASSES_END;
  meltcdat->dtup_168___HOOK_EARLY_GIMPLE_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_168___HOOK_EARLY_GIMPLE_PASSES_END.nbval = 0;
  /*inimult dtup_169___HOOK_EARLY_GIMPLE_PASSES_END */
 /*_.VALTUP___V202*/ meltfptr[201] =
    (melt_ptr_t) & meltcdat->dtup_169___HOOK_EARLY_GIMPLE_PASSES_END;
  meltcdat->dtup_169___HOOK_EARLY_GIMPLE_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_169___HOOK_EARLY_GIMPLE_PASSES_END.nbval = 0;
  /*inirout drout_170__ENABLE_EARLY_GIMPLE_PASSES_END */
 /*_.VALROUT___V203*/ meltfptr[202] =
    (melt_ptr_t) & meltcdat->drout_170__ENABLE_EARLY_GIMPLE_PASSES_END;
  meltcdat->drout_170__ENABLE_EARLY_GIMPLE_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_170__ENABLE_EARLY_GIMPLE_PASSES_END.routdescr,
	   "ENABLE_EARLY_GIMPLE_PASSES_END @warmelt-hooks.melt:1127",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_170__ENABLE_EARLY_GIMPLE_PASSES_END.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_170__ENABLE_EARLY_GIMPLE_PASSES_END,
			     meltrout_44_warmelt_hooks_ENABLE_EARLY_GIMPLE_PASSES_END);

  /*iniclos dclo_171__ENABLE_EARLY_GIMPLE_PASSES_END */
 /*_.VALCLO___V204*/ meltfptr[203] =
    (melt_ptr_t) & meltcdat->dclo_171__ENABLE_EARLY_GIMPLE_PASSES_END;
  meltcdat->dclo_171__ENABLE_EARLY_GIMPLE_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_171__ENABLE_EARLY_GIMPLE_PASSES_END.nbval = 0;

  /*inirout drout_172__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST */
 /*_.VALROUT___V205*/ meltfptr[204] =
    (melt_ptr_t) & meltcdat->
    drout_172__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST;
  meltcdat->drout_172__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_172__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST.
	   routdescr,
	   "REGISTER_EARLY_GIMPLE_PASSES_END_FIRST @warmelt-hooks.melt:1140",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_172__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_172__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST,
			     meltrout_45_warmelt_hooks_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST);

  /*iniclos dclo_173__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST */
 /*_.VALCLO___V206*/ meltfptr[205] =
    (melt_ptr_t) & meltcdat->dclo_173__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST;
  meltcdat->dclo_173__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_173__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST.nbval = 0;

  /*inirout drout_174__REGISTER_EARLY_GIMPLE_PASSES_END_LAST */
 /*_.VALROUT___V207*/ meltfptr[206] =
    (melt_ptr_t) & meltcdat->drout_174__REGISTER_EARLY_GIMPLE_PASSES_END_LAST;
  meltcdat->drout_174__REGISTER_EARLY_GIMPLE_PASSES_END_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_174__REGISTER_EARLY_GIMPLE_PASSES_END_LAST.
	   routdescr,
	   "REGISTER_EARLY_GIMPLE_PASSES_END_LAST @warmelt-hooks.melt:1163",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_174__REGISTER_EARLY_GIMPLE_PASSES_END_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_174__REGISTER_EARLY_GIMPLE_PASSES_END_LAST,
			     meltrout_46_warmelt_hooks_REGISTER_EARLY_GIMPLE_PASSES_END_LAST);

  /*iniclos dclo_175__REGISTER_EARLY_GIMPLE_PASSES_END_LAST */
 /*_.VALCLO___V208*/ meltfptr[207] =
    (melt_ptr_t) & meltcdat->dclo_175__REGISTER_EARLY_GIMPLE_PASSES_END_LAST;
  meltcdat->dclo_175__REGISTER_EARLY_GIMPLE_PASSES_END_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_175__REGISTER_EARLY_GIMPLE_PASSES_END_LAST.nbval = 0;


/*iniobj dsym_176__PREGENERICIZE_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V209*/ meltfptr[208])
/*_.VALDATA___V209*/ meltfptr[208] =
      (melt_ptr_t) & meltcdat->dsym_176__PREGENERICIZE_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_176__PREGENERICIZE_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_176__PREGENERICIZE_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_176__PREGENERICIZE_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_176__PREGENERICIZE_DELAYED_QUEUE.obj_hash = 975490957;
  meltcdat->dsym_176__PREGENERICIZE_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_177__PREGENERICIZE_DE*/
 /*_.VALSTR___V210*/ meltfptr[209] =
    (melt_ptr_t) & meltcdat->dstr_177__PREGENERICIZE_DE;
  meltcdat->dstr_177__PREGENERICIZE_DE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_177__PREGENERICIZE_DE.val,
				"PREGENERICIZE_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_177__PREGENERICIZE_DE.
					val) - 1);
  meltcdat->dstr_177__PREGENERICIZE_DE.val[27] = (char) 0;
  meltcdat->dstr_177__PREGENERICIZE_DE.slen = 27;

  MELT_LOCATION ("warmelt-hooks.melt:1193:/ iniobj");

/*iniobj dobj_178__PREGENERICIZE_DELAYED_QUEUE*/
/*_.VALDATA___V211*/ meltfptr[210] =
    (melt_ptr_t) & meltcdat->dobj_178__PREGENERICIZE_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_178__PREGENERICIZE_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_178__PREGENERICIZE_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_178__PREGENERICIZE_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_178__PREGENERICIZE_DELAYED_QUEUE.obj_hash = 772221282;
  meltcdat->dobj_178__PREGENERICIZE_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_179__pregenericize_de*/
 /*_.VALSTR___V212*/ meltfptr[211] =
    (melt_ptr_t) & meltcdat->dstr_179__pregenericize_de;
  meltcdat->dstr_179__pregenericize_de.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_179__pregenericize_de.val,
				"pregenericize_delayed_queue",
				sizeof (meltcdat->dstr_179__pregenericize_de.
					val) - 1);
  meltcdat->dstr_179__pregenericize_de.val[27] = (char) 0;
  meltcdat->dstr_179__pregenericize_de.slen = 27;

  /*inirout drout_180__ENABLE_PRE_GENERICIZE */
 /*_.VALROUT___V213*/ meltfptr[212] =
    (melt_ptr_t) & meltcdat->drout_180__ENABLE_PRE_GENERICIZE;
  meltcdat->drout_180__ENABLE_PRE_GENERICIZE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_180__ENABLE_PRE_GENERICIZE.routdescr,
	   "ENABLE_PRE_GENERICIZE @warmelt-hooks.melt:1223",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_180__ENABLE_PRE_GENERICIZE.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_180__ENABLE_PRE_GENERICIZE,
			     meltrout_47_warmelt_hooks_ENABLE_PRE_GENERICIZE);

  /*iniclos dclo_181__ENABLE_PRE_GENERICIZE */
 /*_.VALCLO___V214*/ meltfptr[213] =
    (melt_ptr_t) & meltcdat->dclo_181__ENABLE_PRE_GENERICIZE;
  meltcdat->dclo_181__ENABLE_PRE_GENERICIZE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_181__ENABLE_PRE_GENERICIZE.nbval = 0;


/*inihook dhook__182__HOOK_PRE_GENERICIZE */
  meltcdat->dhook__182__HOOK_PRE_GENERICIZE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_PRE_GENERICIZE data */
  meltcdat->dhook__182__HOOK_PRE_GENERICIZE.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__182__HOOK_PRE_GENERICIZE,
			  melthook_HOOK_PRE_GENERICIZE);
  meltcdat->dhook__182__HOOK_PRE_GENERICIZE.hookdata =
    /*_.VALDATA___V216*/ meltfptr[215];
  strncpy (meltcdat->dhook__182__HOOK_PRE_GENERICIZE.hookname,
	   "HOOK_PRE_GENERICIZE",
	   sizeof (meltcdat->dhook__182__HOOK_PRE_GENERICIZE.hookname) - 1);
/*predefinedhook HOOK_PRE_GENERICIZE for dhook__182__HOOK_PRE_GENERICIZE*/
  if (!MELT_PREDEF (HOOK_PRE_GENERICIZE))
    MELT_STORE_PREDEF (HOOK_PRE_GENERICIZE,
		       (melt_ptr_t) & meltcdat->
		       dhook__182__HOOK_PRE_GENERICIZE);

  /*_.VALHOOK___V215*/ meltfptr[214] =
  /*inihooklocvar HOOK_PRE_GENERICIZE */
  (melt_ptr_t) & meltcdat->dhook__182__HOOK_PRE_GENERICIZE;

  /*done inihook dhook__182__HOOK_PRE_GENERICIZE */

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ iniobj");

/*iniobj dobj_183__*/
/*_.VALDATA___V216*/ meltfptr[215] = (melt_ptr_t) & meltcdat->dobj_183__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_183__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_183__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_183__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_183__.obj_hash = 485691946;
  meltcdat->dobj_183__.obj_len = 6;


/*iniobj dsym_184__TFNDECL*/
  /*uniqueobj*/ if (! /*_.VALDATA___V219*/ meltfptr[218])
/*_.VALDATA___V219*/ meltfptr[218] =
      (melt_ptr_t) & meltcdat->dsym_184__TFNDECL;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_184__TFNDECL",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_184__TFNDECL",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_184__TFNDECL.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_184__TFNDECL.obj_hash = 4800269;
  meltcdat->dsym_184__TFNDECL.obj_len = 3;


/*inistring dstr_185__TFNDECL*/
 /*_.VALSTR___V220*/ meltfptr[219] =
    (melt_ptr_t) & meltcdat->dstr_185__TFNDECL;
  meltcdat->dstr_185__TFNDECL.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_185__TFNDECL.val, "TFNDECL",
				sizeof (meltcdat->dstr_185__TFNDECL.val) - 1);
  meltcdat->dstr_185__TFNDECL.val[7] = (char) 0;
  meltcdat->dstr_185__TFNDECL.slen = 7;

  /*^iniobj */

/*iniobj dobj_186*/
/*_.VALDATA___V218*/ meltfptr[217] = (melt_ptr_t) & meltcdat->dobj_186;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_186",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_186",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_186.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_186.obj_num = 0;
  meltcdat->dobj_186.obj_hash = 720483980;
  meltcdat->dobj_186.obj_len = 2;

  /*inimult dtup_187___HOOK_PRE_GENERICIZE */
 /*_.VALTUP___V221*/ meltfptr[220] =
    (melt_ptr_t) & meltcdat->dtup_187___HOOK_PRE_GENERICIZE;
  meltcdat->dtup_187___HOOK_PRE_GENERICIZE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_187___HOOK_PRE_GENERICIZE.nbval = 1;
  /*inimult dtup_188___HOOK_PRE_GENERICIZE */
 /*_.VALTUP___V222*/ meltfptr[221] =
    (melt_ptr_t) & meltcdat->dtup_188___HOOK_PRE_GENERICIZE;
  meltcdat->dtup_188___HOOK_PRE_GENERICIZE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_188___HOOK_PRE_GENERICIZE.nbval = 0;
  /*inirout drout_189__REGISTER_PRE_GENERICIZE_FIRST */
 /*_.VALROUT___V223*/ meltfptr[222] =
    (melt_ptr_t) & meltcdat->drout_189__REGISTER_PRE_GENERICIZE_FIRST;
  meltcdat->drout_189__REGISTER_PRE_GENERICIZE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_189__REGISTER_PRE_GENERICIZE_FIRST.routdescr,
	   "REGISTER_PRE_GENERICIZE_FIRST @warmelt-hooks.melt:1268",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_189__REGISTER_PRE_GENERICIZE_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_189__REGISTER_PRE_GENERICIZE_FIRST,
			     meltrout_49_warmelt_hooks_REGISTER_PRE_GENERICIZE_FIRST);

  /*iniclos dclo_190__REGISTER_PRE_GENERICIZE_FIRST */
 /*_.VALCLO___V224*/ meltfptr[223] =
    (melt_ptr_t) & meltcdat->dclo_190__REGISTER_PRE_GENERICIZE_FIRST;
  meltcdat->dclo_190__REGISTER_PRE_GENERICIZE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_190__REGISTER_PRE_GENERICIZE_FIRST.nbval = 0;

  /*inirout drout_191__REGISTER_PRE_GENERICIZE_LAST */
 /*_.VALROUT___V225*/ meltfptr[224] =
    (melt_ptr_t) & meltcdat->drout_191__REGISTER_PRE_GENERICIZE_LAST;
  meltcdat->drout_191__REGISTER_PRE_GENERICIZE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_191__REGISTER_PRE_GENERICIZE_LAST.routdescr,
	   "REGISTER_PRE_GENERICIZE_LAST @warmelt-hooks.melt:1291",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_191__REGISTER_PRE_GENERICIZE_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_191__REGISTER_PRE_GENERICIZE_LAST,
			     meltrout_50_warmelt_hooks_REGISTER_PRE_GENERICIZE_LAST);

  /*iniclos dclo_192__REGISTER_PRE_GENERICIZE_LAST */
 /*_.VALCLO___V226*/ meltfptr[225] =
    (melt_ptr_t) & meltcdat->dclo_192__REGISTER_PRE_GENERICIZE_LAST;
  meltcdat->dclo_192__REGISTER_PRE_GENERICIZE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_192__REGISTER_PRE_GENERICIZE_LAST.nbval = 0;


/*iniobj dsym_193__FINISHTYPE_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V227*/ meltfptr[226])
/*_.VALDATA___V227*/ meltfptr[226] =
      (melt_ptr_t) & meltcdat->dsym_193__FINISHTYPE_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_193__FINISHTYPE_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_193__FINISHTYPE_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_193__FINISHTYPE_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_193__FINISHTYPE_DELAYED_QUEUE.obj_hash = 436015866;
  meltcdat->dsym_193__FINISHTYPE_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_194__FINISHTYPE_DELAY*/
 /*_.VALSTR___V228*/ meltfptr[227] =
    (melt_ptr_t) & meltcdat->dstr_194__FINISHTYPE_DELAY;
  meltcdat->dstr_194__FINISHTYPE_DELAY.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_194__FINISHTYPE_DELAY.val,
				"FINISHTYPE_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_194__FINISHTYPE_DELAY.
					val) - 1);
  meltcdat->dstr_194__FINISHTYPE_DELAY.val[24] = (char) 0;
  meltcdat->dstr_194__FINISHTYPE_DELAY.slen = 24;

  MELT_LOCATION ("warmelt-hooks.melt:1324:/ iniobj");

/*iniobj dobj_195__FINISHTYPE_DELAYED_QUEUE*/
/*_.VALDATA___V229*/ meltfptr[228] =
    (melt_ptr_t) & meltcdat->dobj_195__FINISHTYPE_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_195__FINISHTYPE_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_195__FINISHTYPE_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_195__FINISHTYPE_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_195__FINISHTYPE_DELAYED_QUEUE.obj_hash = 671053552;
  meltcdat->dobj_195__FINISHTYPE_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_196__finishtype_delay*/
 /*_.VALSTR___V230*/ meltfptr[229] =
    (melt_ptr_t) & meltcdat->dstr_196__finishtype_delay;
  meltcdat->dstr_196__finishtype_delay.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_196__finishtype_delay.val,
				"finishtype_delayed_queue",
				sizeof (meltcdat->dstr_196__finishtype_delay.
					val) - 1);
  meltcdat->dstr_196__finishtype_delay.val[24] = (char) 0;
  meltcdat->dstr_196__finishtype_delay.slen = 24;

  /*inirout drout_197__ENABLE_FINISH_TYPE */
 /*_.VALROUT___V231*/ meltfptr[230] =
    (melt_ptr_t) & meltcdat->drout_197__ENABLE_FINISH_TYPE;
  meltcdat->drout_197__ENABLE_FINISH_TYPE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_197__ENABLE_FINISH_TYPE.routdescr,
	   "ENABLE_FINISH_TYPE @warmelt-hooks.melt:1354",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_197__ENABLE_FINISH_TYPE.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_197__ENABLE_FINISH_TYPE,
			     meltrout_51_warmelt_hooks_ENABLE_FINISH_TYPE);

  /*iniclos dclo_198__ENABLE_FINISH_TYPE */
 /*_.VALCLO___V232*/ meltfptr[231] =
    (melt_ptr_t) & meltcdat->dclo_198__ENABLE_FINISH_TYPE;
  meltcdat->dclo_198__ENABLE_FINISH_TYPE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_198__ENABLE_FINISH_TYPE.nbval = 0;


/*inihook dhook__199__HOOK_FINISH_TYPE */
  meltcdat->dhook__199__HOOK_FINISH_TYPE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_FINISH_TYPE data */
  meltcdat->dhook__199__HOOK_FINISH_TYPE.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__199__HOOK_FINISH_TYPE,
			  melthook_HOOK_FINISH_TYPE);
  meltcdat->dhook__199__HOOK_FINISH_TYPE.hookdata =
    /*_.VALDATA___V234*/ meltfptr[233];
  strncpy (meltcdat->dhook__199__HOOK_FINISH_TYPE.hookname,
	   "HOOK_FINISH_TYPE",
	   sizeof (meltcdat->dhook__199__HOOK_FINISH_TYPE.hookname) - 1);
/*predefinedhook HOOK_FINISH_TYPE for dhook__199__HOOK_FINISH_TYPE*/
  if (!MELT_PREDEF (HOOK_FINISH_TYPE))
    MELT_STORE_PREDEF (HOOK_FINISH_TYPE,
		       (melt_ptr_t) & meltcdat->dhook__199__HOOK_FINISH_TYPE);

  /*_.VALHOOK___V233*/ meltfptr[232] =
  /*inihooklocvar HOOK_FINISH_TYPE */
  (melt_ptr_t) & meltcdat->dhook__199__HOOK_FINISH_TYPE;

  /*done inihook dhook__199__HOOK_FINISH_TYPE */

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ iniobj");

/*iniobj dobj_200__*/
/*_.VALDATA___V234*/ meltfptr[233] = (melt_ptr_t) & meltcdat->dobj_200__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_200__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_200__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_200__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_200__.obj_hash = 242438522;
  meltcdat->dobj_200__.obj_len = 6;

  /*^iniobj */

/*iniobj dobj_201*/
/*_.VALDATA___V236*/ meltfptr[235] = (melt_ptr_t) & meltcdat->dobj_201;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_201",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_201",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_201.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_201.obj_num = 0;
  meltcdat->dobj_201.obj_hash = 570578442;
  meltcdat->dobj_201.obj_len = 2;

  /*inimult dtup_202___HOOK_FINISH_TYPE */
 /*_.VALTUP___V237*/ meltfptr[236] =
    (melt_ptr_t) & meltcdat->dtup_202___HOOK_FINISH_TYPE;
  meltcdat->dtup_202___HOOK_FINISH_TYPE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_202___HOOK_FINISH_TYPE.nbval = 1;
  /*inimult dtup_203___HOOK_FINISH_TYPE */
 /*_.VALTUP___V238*/ meltfptr[237] =
    (melt_ptr_t) & meltcdat->dtup_203___HOOK_FINISH_TYPE;
  meltcdat->dtup_203___HOOK_FINISH_TYPE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_203___HOOK_FINISH_TYPE.nbval = 0;
  /*inirout drout_204__REGISTER_FINISH_TYPE_FIRST */
 /*_.VALROUT___V239*/ meltfptr[238] =
    (melt_ptr_t) & meltcdat->drout_204__REGISTER_FINISH_TYPE_FIRST;
  meltcdat->drout_204__REGISTER_FINISH_TYPE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_204__REGISTER_FINISH_TYPE_FIRST.routdescr,
	   "REGISTER_FINISH_TYPE_FIRST @warmelt-hooks.melt:1399",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_204__REGISTER_FINISH_TYPE_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_204__REGISTER_FINISH_TYPE_FIRST,
			     meltrout_53_warmelt_hooks_REGISTER_FINISH_TYPE_FIRST);

  /*iniclos dclo_205__REGISTER_FINISH_TYPE_FIRST */
 /*_.VALCLO___V240*/ meltfptr[239] =
    (melt_ptr_t) & meltcdat->dclo_205__REGISTER_FINISH_TYPE_FIRST;
  meltcdat->dclo_205__REGISTER_FINISH_TYPE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_205__REGISTER_FINISH_TYPE_FIRST.nbval = 0;

  /*inirout drout_206__REGISTER_FINISH_TYPE_LAST */
 /*_.VALROUT___V241*/ meltfptr[240] =
    (melt_ptr_t) & meltcdat->drout_206__REGISTER_FINISH_TYPE_LAST;
  meltcdat->drout_206__REGISTER_FINISH_TYPE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_206__REGISTER_FINISH_TYPE_LAST.routdescr,
	   "REGISTER_FINISH_TYPE_LAST @warmelt-hooks.melt:1422",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_206__REGISTER_FINISH_TYPE_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_206__REGISTER_FINISH_TYPE_LAST,
			     meltrout_54_warmelt_hooks_REGISTER_FINISH_TYPE_LAST);

  /*iniclos dclo_207__REGISTER_FINISH_TYPE_LAST */
 /*_.VALCLO___V242*/ meltfptr[241] =
    (melt_ptr_t) & meltcdat->dclo_207__REGISTER_FINISH_TYPE_LAST;
  meltcdat->dclo_207__REGISTER_FINISH_TYPE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_207__REGISTER_FINISH_TYPE_LAST.nbval = 0;


/*iniobj dsym_208__PASS_EXECUTION_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V243*/ meltfptr[242])
/*_.VALDATA___V243*/ meltfptr[242] =
      (melt_ptr_t) & meltcdat->dsym_208__PASS_EXECUTION_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_208__PASS_EXECUTION_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_208__PASS_EXECUTION_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_208__PASS_EXECUTION_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_208__PASS_EXECUTION_DELAYED_QUEUE.obj_hash = 721747703;
  meltcdat->dsym_208__PASS_EXECUTION_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_209__PASS_EXECUTION_D*/
 /*_.VALSTR___V244*/ meltfptr[243] =
    (melt_ptr_t) & meltcdat->dstr_209__PASS_EXECUTION_D;
  meltcdat->dstr_209__PASS_EXECUTION_D.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_209__PASS_EXECUTION_D.val,
				"PASS_EXECUTION_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_209__PASS_EXECUTION_D.
					val) - 1);
  meltcdat->dstr_209__PASS_EXECUTION_D.val[28] = (char) 0;
  meltcdat->dstr_209__PASS_EXECUTION_D.slen = 28;

  MELT_LOCATION ("warmelt-hooks.melt:1455:/ iniobj");

/*iniobj dobj_210__PASS_EXECUTION_DELAYED_QUEUE*/
/*_.VALDATA___V245*/ meltfptr[244] =
    (melt_ptr_t) & meltcdat->dobj_210__PASS_EXECUTION_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_210__PASS_EXECUTION_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_210__PASS_EXECUTION_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_210__PASS_EXECUTION_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);
  meltcdat->dobj_210__PASS_EXECUTION_DELAYED_QUEUE.obj_hash = 808914183;
  meltcdat->dobj_210__PASS_EXECUTION_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_211__pass_execution_d*/
 /*_.VALSTR___V246*/ meltfptr[245] =
    (melt_ptr_t) & meltcdat->dstr_211__pass_execution_d;
  meltcdat->dstr_211__pass_execution_d.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_211__pass_execution_d.val,
				"pass_execution_delayed_queue",
				sizeof (meltcdat->dstr_211__pass_execution_d.
					val) - 1);
  meltcdat->dstr_211__pass_execution_d.val[28] = (char) 0;
  meltcdat->dstr_211__pass_execution_d.slen = 28;


/*inihook dhook__212__HOOK_PASS_EXECUTION */
  meltcdat->dhook__212__HOOK_PASS_EXECUTION.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_PASS_EXECUTION data */
  meltcdat->dhook__212__HOOK_PASS_EXECUTION.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__212__HOOK_PASS_EXECUTION,
			  melthook_HOOK_PASS_EXECUTION);
  meltcdat->dhook__212__HOOK_PASS_EXECUTION.hookdata =
    /*_.VALDATA___V248*/ meltfptr[247];
  strncpy (meltcdat->dhook__212__HOOK_PASS_EXECUTION.hookname,
	   "HOOK_PASS_EXECUTION",
	   sizeof (meltcdat->dhook__212__HOOK_PASS_EXECUTION.hookname) - 1);
/*predefinedhook HOOK_PASS_EXECUTION for dhook__212__HOOK_PASS_EXECUTION*/
  if (!MELT_PREDEF (HOOK_PASS_EXECUTION))
    MELT_STORE_PREDEF (HOOK_PASS_EXECUTION,
		       (melt_ptr_t) & meltcdat->
		       dhook__212__HOOK_PASS_EXECUTION);

  /*_.VALHOOK___V247*/ meltfptr[246] =
  /*inihooklocvar HOOK_PASS_EXECUTION */
  (melt_ptr_t) & meltcdat->dhook__212__HOOK_PASS_EXECUTION;

  /*done inihook dhook__212__HOOK_PASS_EXECUTION */

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ iniobj");

/*iniobj dobj_213__*/
/*_.VALDATA___V248*/ meltfptr[247] = (melt_ptr_t) & meltcdat->dobj_213__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_213__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_213__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_213__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_213__.obj_hash = 727072914;
  meltcdat->dobj_213__.obj_len = 6;


/*iniobj dsym_214__PASSNAME*/
  /*uniqueobj*/ if (! /*_.VALDATA___V251*/ meltfptr[250])
/*_.VALDATA___V251*/ meltfptr[250] =
      (melt_ptr_t) & meltcdat->dsym_214__PASSNAME;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_214__PASSNAME",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_214__PASSNAME",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_214__PASSNAME.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_214__PASSNAME.obj_hash = 475697004;
  meltcdat->dsym_214__PASSNAME.obj_len = 3;


/*inistring dstr_215__PASSNAME*/
 /*_.VALSTR___V252*/ meltfptr[251] =
    (melt_ptr_t) & meltcdat->dstr_215__PASSNAME;
  meltcdat->dstr_215__PASSNAME.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_215__PASSNAME.val, "PASSNAME",
				sizeof (meltcdat->dstr_215__PASSNAME.val) -
				1);
  meltcdat->dstr_215__PASSNAME.val[8] = (char) 0;
  meltcdat->dstr_215__PASSNAME.slen = 8;

  /*^iniobj */

/*iniobj dobj_216*/
/*_.VALDATA___V250*/ meltfptr[249] = (melt_ptr_t) & meltcdat->dobj_216;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_216",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_216",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_216.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_216.obj_num = 0;
  meltcdat->dobj_216.obj_hash = 849288382;
  meltcdat->dobj_216.obj_len = 2;


/*iniobj dsym_217__PASSNUM*/
  /*uniqueobj*/ if (! /*_.VALDATA___V254*/ meltfptr[253])
/*_.VALDATA___V254*/ meltfptr[253] =
      (melt_ptr_t) & meltcdat->dsym_217__PASSNUM;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_217__PASSNUM",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_217__PASSNUM",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_217__PASSNUM.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_217__PASSNUM.obj_hash = 228918362;
  meltcdat->dsym_217__PASSNUM.obj_len = 3;


/*inistring dstr_218__PASSNUM*/
 /*_.VALSTR___V255*/ meltfptr[254] =
    (melt_ptr_t) & meltcdat->dstr_218__PASSNUM;
  meltcdat->dstr_218__PASSNUM.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_218__PASSNUM.val, "PASSNUM",
				sizeof (meltcdat->dstr_218__PASSNUM.val) - 1);
  meltcdat->dstr_218__PASSNUM.val[7] = (char) 0;
  meltcdat->dstr_218__PASSNUM.slen = 7;

  /*^iniobj */

/*iniobj dobj_219*/
/*_.VALDATA___V253*/ meltfptr[252] = (melt_ptr_t) & meltcdat->dobj_219;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_219",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_219",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_219.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_219.obj_num = 1;
  meltcdat->dobj_219.obj_hash = 247354383;
  meltcdat->dobj_219.obj_len = 2;

  /*inimult dtup_220___HOOK_PASS_EXECUTION */
 /*_.VALTUP___V256*/ meltfptr[255] =
    (melt_ptr_t) & meltcdat->dtup_220___HOOK_PASS_EXECUTION;
  meltcdat->dtup_220___HOOK_PASS_EXECUTION.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_220___HOOK_PASS_EXECUTION.nbval = 2;
  /*inimult dtup_221___HOOK_PASS_EXECUTION */
 /*_.VALTUP___V257*/ meltfptr[256] =
    (melt_ptr_t) & meltcdat->dtup_221___HOOK_PASS_EXECUTION;
  meltcdat->dtup_221___HOOK_PASS_EXECUTION.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_221___HOOK_PASS_EXECUTION.nbval = 0;
  /*inirout drout_222__REGISTER_PASS_EXECUTION_FIRST */
 /*_.VALROUT___V258*/ meltfptr[257] =
    (melt_ptr_t) & meltcdat->drout_222__REGISTER_PASS_EXECUTION_FIRST;
  meltcdat->drout_222__REGISTER_PASS_EXECUTION_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_222__REGISTER_PASS_EXECUTION_FIRST.routdescr,
	   "REGISTER_PASS_EXECUTION_FIRST @warmelt-hooks.melt:1534",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_222__REGISTER_PASS_EXECUTION_FIRST.nbval = 3;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_222__REGISTER_PASS_EXECUTION_FIRST,
			     meltrout_56_warmelt_hooks_REGISTER_PASS_EXECUTION_FIRST);

  /*iniclos dclo_223__REGISTER_PASS_EXECUTION_FIRST */
 /*_.VALCLO___V259*/ meltfptr[258] =
    (melt_ptr_t) & meltcdat->dclo_223__REGISTER_PASS_EXECUTION_FIRST;
  meltcdat->dclo_223__REGISTER_PASS_EXECUTION_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_223__REGISTER_PASS_EXECUTION_FIRST.nbval = 0;

  /*inirout drout_224__REGISTER_PASS_EXECUTION_LAST */
 /*_.VALROUT___V260*/ meltfptr[259] =
    (melt_ptr_t) & meltcdat->drout_224__REGISTER_PASS_EXECUTION_LAST;
  meltcdat->drout_224__REGISTER_PASS_EXECUTION_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_224__REGISTER_PASS_EXECUTION_LAST.routdescr,
	   "REGISTER_PASS_EXECUTION_LAST @warmelt-hooks.melt:1555",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_224__REGISTER_PASS_EXECUTION_LAST.nbval = 3;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_224__REGISTER_PASS_EXECUTION_LAST,
			     meltrout_57_warmelt_hooks_REGISTER_PASS_EXECUTION_LAST);

  /*iniclos dclo_225__REGISTER_PASS_EXECUTION_LAST */
 /*_.VALCLO___V261*/ meltfptr[260] =
    (melt_ptr_t) & meltcdat->dclo_225__REGISTER_PASS_EXECUTION_LAST;
  meltcdat->dclo_225__REGISTER_PASS_EXECUTION_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_225__REGISTER_PASS_EXECUTION_LAST.nbval = 0;


/*inihook dhook__226__HOOK_GIMPLE_GATE */
  meltcdat->dhook__226__HOOK_GIMPLE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_GIMPLE_GATE data */
  meltcdat->dhook__226__HOOK_GIMPLE_GATE.nbval = 13;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__226__HOOK_GIMPLE_GATE,
			  melthook_HOOK_GIMPLE_GATE);
  meltcdat->dhook__226__HOOK_GIMPLE_GATE.hookdata =
    /*_.VALDATA___V271*/ meltfptr[270];
  strncpy (meltcdat->dhook__226__HOOK_GIMPLE_GATE.hookname,
	   "HOOK_GIMPLE_GATE",
	   sizeof (meltcdat->dhook__226__HOOK_GIMPLE_GATE.hookname) - 1);
/*predefinedhook HOOK_GIMPLE_GATE for dhook__226__HOOK_GIMPLE_GATE*/
  if (!MELT_PREDEF (HOOK_GIMPLE_GATE))
    MELT_STORE_PREDEF (HOOK_GIMPLE_GATE,
		       (melt_ptr_t) & meltcdat->dhook__226__HOOK_GIMPLE_GATE);

  /*_.VALHOOK___V262*/ meltfptr[261] =
  /*inihooklocvar HOOK_GIMPLE_GATE */
  (melt_ptr_t) & meltcdat->dhook__226__HOOK_GIMPLE_GATE;

  /*done inihook dhook__226__HOOK_GIMPLE_GATE */

  MELT_LOCATION ("warmelt-hooks.melt:1579:/ iniobj");

/*iniobj dobj_227__*/
/*_.VALDATA___V271*/ meltfptr[270] = (melt_ptr_t) & meltcdat->dobj_227__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_227__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_227__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_227__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_227__.obj_hash = 330002013;
  meltcdat->dobj_227__.obj_len = 6;

  /*inimult dtup_228___HOOK_GIMPLE_GATE */
 /*_.VALTUP___V273*/ meltfptr[272] =
    (melt_ptr_t) & meltcdat->dtup_228___HOOK_GIMPLE_GATE;
  meltcdat->dtup_228___HOOK_GIMPLE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_228___HOOK_GIMPLE_GATE.nbval = 0;
  /*inimult dtup_229___HOOK_GIMPLE_GATE */
 /*_.VALTUP___V274*/ meltfptr[273] =
    (melt_ptr_t) & meltcdat->dtup_229___HOOK_GIMPLE_GATE;
  meltcdat->dtup_229___HOOK_GIMPLE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_229___HOOK_GIMPLE_GATE.nbval = 0;

/*iniobj dsym_230__INITIAL_SYSTEM_DATA*/
  /*uniqueobj*/ if (! /*_.VALDATA___V263*/ meltfptr[262])
/*_.VALDATA___V263*/ meltfptr[262] =
      (melt_ptr_t) & meltcdat->dsym_230__INITIAL_SYSTEM_DATA;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_230__INITIAL_SYSTEM_DATA",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_230__INITIAL_SYSTEM_DATA",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_230__INITIAL_SYSTEM_DATA.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_230__INITIAL_SYSTEM_DATA.obj_hash = 771349808;
  meltcdat->dsym_230__INITIAL_SYSTEM_DATA.obj_len = 3;


/*inistring dstr_231__INITIAL_SYSTEM_D*/
 /*_.VALSTR___V264*/ meltfptr[263] =
    (melt_ptr_t) & meltcdat->dstr_231__INITIAL_SYSTEM_D;
  meltcdat->dstr_231__INITIAL_SYSTEM_D.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_231__INITIAL_SYSTEM_D.val,
				"INITIAL_SYSTEM_DATA",
				sizeof (meltcdat->dstr_231__INITIAL_SYSTEM_D.
					val) - 1);
  meltcdat->dstr_231__INITIAL_SYSTEM_D.val[19] = (char) 0;
  meltcdat->dstr_231__INITIAL_SYSTEM_D.slen = 19;


/*iniobj dsym_232__CLASS_SYSTEM_DATA*/
  /*uniqueobj*/ if (! /*_.VALDATA___V265*/ meltfptr[264])
/*_.VALDATA___V265*/ meltfptr[264] =
      (melt_ptr_t) & meltcdat->dsym_232__CLASS_SYSTEM_DATA;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_232__CLASS_SYSTEM_DATA",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_232__CLASS_SYSTEM_DATA",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_232__CLASS_SYSTEM_DATA.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_232__CLASS_SYSTEM_DATA.obj_hash = 648451867;
  meltcdat->dsym_232__CLASS_SYSTEM_DATA.obj_len = 3;


/*inistring dstr_233__CLASS_SYSTEM_DAT*/
 /*_.VALSTR___V266*/ meltfptr[265] =
    (melt_ptr_t) & meltcdat->dstr_233__CLASS_SYSTEM_DAT;
  meltcdat->dstr_233__CLASS_SYSTEM_DAT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_233__CLASS_SYSTEM_DAT.val,
				"CLASS_SYSTEM_DATA",
				sizeof (meltcdat->dstr_233__CLASS_SYSTEM_DAT.
					val) - 1);
  meltcdat->dstr_233__CLASS_SYSTEM_DAT.val[17] = (char) 0;
  meltcdat->dstr_233__CLASS_SYSTEM_DAT.slen = 17;


/*iniobj dsym_234__CLASS_GCC_GIMPLE_PASS*/
  /*uniqueobj*/ if (! /*_.VALDATA___V267*/ meltfptr[266])
/*_.VALDATA___V267*/ meltfptr[266] =
      (melt_ptr_t) & meltcdat->dsym_234__CLASS_GCC_GIMPLE_PASS;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_234__CLASS_GCC_GIMPLE_PASS",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_234__CLASS_GCC_GIMPLE_PASS",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_234__CLASS_GCC_GIMPLE_PASS.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_234__CLASS_GCC_GIMPLE_PASS.obj_hash = 216040702;
  meltcdat->dsym_234__CLASS_GCC_GIMPLE_PASS.obj_len = 3;


/*inistring dstr_235__CLASS_GCC_GIMPLE*/
 /*_.VALSTR___V268*/ meltfptr[267] =
    (melt_ptr_t) & meltcdat->dstr_235__CLASS_GCC_GIMPLE;
  meltcdat->dstr_235__CLASS_GCC_GIMPLE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_235__CLASS_GCC_GIMPLE.val,
				"CLASS_GCC_GIMPLE_PASS",
				sizeof (meltcdat->dstr_235__CLASS_GCC_GIMPLE.
					val) - 1);
  meltcdat->dstr_235__CLASS_GCC_GIMPLE.val[21] = (char) 0;
  meltcdat->dstr_235__CLASS_GCC_GIMPLE.slen = 21;


/*iniobj dsym_236__CLASS_GCC_PASS*/
  /*uniqueobj*/ if (! /*_.VALDATA___V269*/ meltfptr[268])
/*_.VALDATA___V269*/ meltfptr[268] =
      (melt_ptr_t) & meltcdat->dsym_236__CLASS_GCC_PASS;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_236__CLASS_GCC_PASS",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_236__CLASS_GCC_PASS",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_236__CLASS_GCC_PASS.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_236__CLASS_GCC_PASS.obj_hash = 271520587;
  meltcdat->dsym_236__CLASS_GCC_PASS.obj_len = 3;


/*inistring dstr_237__CLASS_GCC_PASS*/
 /*_.VALSTR___V270*/ meltfptr[269] =
    (melt_ptr_t) & meltcdat->dstr_237__CLASS_GCC_PASS;
  meltcdat->dstr_237__CLASS_GCC_PASS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_237__CLASS_GCC_PASS.val,
				"CLASS_GCC_PASS",
				sizeof (meltcdat->dstr_237__CLASS_GCC_PASS.
					val) - 1);
  meltcdat->dstr_237__CLASS_GCC_PASS.val[14] = (char) 0;
  meltcdat->dstr_237__CLASS_GCC_PASS.slen = 14;


/*inihook dhook__238__HOOK_GIMPLE_EXECUTE */
  meltcdat->dhook__238__HOOK_GIMPLE_EXECUTE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_GIMPLE_EXECUTE data */
  meltcdat->dhook__238__HOOK_GIMPLE_EXECUTE.nbval = 5;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__238__HOOK_GIMPLE_EXECUTE,
			  melthook_HOOK_GIMPLE_EXECUTE);
  meltcdat->dhook__238__HOOK_GIMPLE_EXECUTE.hookdata =
    /*_.VALDATA___V276*/ meltfptr[275];
  strncpy (meltcdat->dhook__238__HOOK_GIMPLE_EXECUTE.hookname,
	   "HOOK_GIMPLE_EXECUTE",
	   sizeof (meltcdat->dhook__238__HOOK_GIMPLE_EXECUTE.hookname) - 1);
/*predefinedhook HOOK_GIMPLE_EXECUTE for dhook__238__HOOK_GIMPLE_EXECUTE*/
  if (!MELT_PREDEF (HOOK_GIMPLE_EXECUTE))
    MELT_STORE_PREDEF (HOOK_GIMPLE_EXECUTE,
		       (melt_ptr_t) & meltcdat->
		       dhook__238__HOOK_GIMPLE_EXECUTE);

  /*_.VALHOOK___V275*/ meltfptr[274] =
  /*inihooklocvar HOOK_GIMPLE_EXECUTE */
  (melt_ptr_t) & meltcdat->dhook__238__HOOK_GIMPLE_EXECUTE;

  /*done inihook dhook__238__HOOK_GIMPLE_EXECUTE */

  MELT_LOCATION ("warmelt-hooks.melt:1632:/ iniobj");

/*iniobj dobj_239__*/
/*_.VALDATA___V276*/ meltfptr[275] = (melt_ptr_t) & meltcdat->dobj_239__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_239__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_239__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_239__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_239__.obj_hash = 508397136;
  meltcdat->dobj_239__.obj_len = 6;

  /*inimult dtup_240___HOOK_GIMPLE_EXECUTE */
 /*_.VALTUP___V278*/ meltfptr[277] =
    (melt_ptr_t) & meltcdat->dtup_240___HOOK_GIMPLE_EXECUTE;
  meltcdat->dtup_240___HOOK_GIMPLE_EXECUTE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_240___HOOK_GIMPLE_EXECUTE.nbval = 0;
  /*inimult dtup_241___HOOK_GIMPLE_EXECUTE */
 /*_.VALTUP___V279*/ meltfptr[278] =
    (melt_ptr_t) & meltcdat->dtup_241___HOOK_GIMPLE_EXECUTE;
  meltcdat->dtup_241___HOOK_GIMPLE_EXECUTE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_241___HOOK_GIMPLE_EXECUTE.nbval = 0;

/*inihook dhook__242__HOOK_RTL_GATE */
  meltcdat->dhook__242__HOOK_RTL_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_RTL_GATE data */
  meltcdat->dhook__242__HOOK_RTL_GATE.nbval = 7;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__242__HOOK_RTL_GATE,
			  melthook_HOOK_RTL_GATE);
  meltcdat->dhook__242__HOOK_RTL_GATE.hookdata =
    /*_.VALDATA___V283*/ meltfptr[282];
  strncpy (meltcdat->dhook__242__HOOK_RTL_GATE.hookname, "HOOK_RTL_GATE",
	   sizeof (meltcdat->dhook__242__HOOK_RTL_GATE.hookname) - 1);
/*predefinedhook HOOK_RTL_GATE for dhook__242__HOOK_RTL_GATE*/
  if (!MELT_PREDEF (HOOK_RTL_GATE))
    MELT_STORE_PREDEF (HOOK_RTL_GATE,
		       (melt_ptr_t) & meltcdat->dhook__242__HOOK_RTL_GATE);

  /*_.VALHOOK___V280*/ meltfptr[279] =
  /*inihooklocvar HOOK_RTL_GATE */
  (melt_ptr_t) & meltcdat->dhook__242__HOOK_RTL_GATE;

  /*done inihook dhook__242__HOOK_RTL_GATE */

  MELT_LOCATION ("warmelt-hooks.melt:1695:/ iniobj");

/*iniobj dobj_243__*/
/*_.VALDATA___V283*/ meltfptr[282] = (melt_ptr_t) & meltcdat->dobj_243__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_243__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_243__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_243__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_243__.obj_hash = 259042824;
  meltcdat->dobj_243__.obj_len = 6;

  /*inimult dtup_244___HOOK_RTL_GATE */
 /*_.VALTUP___V285*/ meltfptr[284] =
    (melt_ptr_t) & meltcdat->dtup_244___HOOK_RTL_GATE;
  meltcdat->dtup_244___HOOK_RTL_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_244___HOOK_RTL_GATE.nbval = 0;
  /*inimult dtup_245___HOOK_RTL_GATE */
 /*_.VALTUP___V286*/ meltfptr[285] =
    (melt_ptr_t) & meltcdat->dtup_245___HOOK_RTL_GATE;
  meltcdat->dtup_245___HOOK_RTL_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_245___HOOK_RTL_GATE.nbval = 0;

/*iniobj dsym_246__CLASS_GCC_RTL_PASS*/
  /*uniqueobj*/ if (! /*_.VALDATA___V281*/ meltfptr[280])
/*_.VALDATA___V281*/ meltfptr[280] =
      (melt_ptr_t) & meltcdat->dsym_246__CLASS_GCC_RTL_PASS;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_246__CLASS_GCC_RTL_PASS",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_246__CLASS_GCC_RTL_PASS",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_246__CLASS_GCC_RTL_PASS.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_246__CLASS_GCC_RTL_PASS.obj_hash = 997542884;
  meltcdat->dsym_246__CLASS_GCC_RTL_PASS.obj_len = 3;


/*inistring dstr_247__CLASS_GCC_RTL_PA*/
 /*_.VALSTR___V282*/ meltfptr[281] =
    (melt_ptr_t) & meltcdat->dstr_247__CLASS_GCC_RTL_PA;
  meltcdat->dstr_247__CLASS_GCC_RTL_PA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_247__CLASS_GCC_RTL_PA.val,
				"CLASS_GCC_RTL_PASS",
				sizeof (meltcdat->dstr_247__CLASS_GCC_RTL_PA.
					val) - 1);
  meltcdat->dstr_247__CLASS_GCC_RTL_PA.val[18] = (char) 0;
  meltcdat->dstr_247__CLASS_GCC_RTL_PA.slen = 18;


/*inihook dhook__248__HOOK_RTL_EXECUTE */
  meltcdat->dhook__248__HOOK_RTL_EXECUTE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_RTL_EXECUTE data */
  meltcdat->dhook__248__HOOK_RTL_EXECUTE.nbval = 5;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__248__HOOK_RTL_EXECUTE,
			  melthook_HOOK_RTL_EXECUTE);
  meltcdat->dhook__248__HOOK_RTL_EXECUTE.hookdata =
    /*_.VALDATA___V288*/ meltfptr[287];
  strncpy (meltcdat->dhook__248__HOOK_RTL_EXECUTE.hookname,
	   "HOOK_RTL_EXECUTE",
	   sizeof (meltcdat->dhook__248__HOOK_RTL_EXECUTE.hookname) - 1);
/*predefinedhook HOOK_RTL_EXECUTE for dhook__248__HOOK_RTL_EXECUTE*/
  if (!MELT_PREDEF (HOOK_RTL_EXECUTE))
    MELT_STORE_PREDEF (HOOK_RTL_EXECUTE,
		       (melt_ptr_t) & meltcdat->dhook__248__HOOK_RTL_EXECUTE);

  /*_.VALHOOK___V287*/ meltfptr[286] =
  /*inihooklocvar HOOK_RTL_EXECUTE */
  (melt_ptr_t) & meltcdat->dhook__248__HOOK_RTL_EXECUTE;

  /*done inihook dhook__248__HOOK_RTL_EXECUTE */

  MELT_LOCATION ("warmelt-hooks.melt:1750:/ iniobj");

/*iniobj dobj_249__*/
/*_.VALDATA___V288*/ meltfptr[287] = (melt_ptr_t) & meltcdat->dobj_249__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_249__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_249__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_249__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_249__.obj_hash = 628023864;
  meltcdat->dobj_249__.obj_len = 6;

  /*inimult dtup_250___HOOK_RTL_EXECUTE */
 /*_.VALTUP___V290*/ meltfptr[289] =
    (melt_ptr_t) & meltcdat->dtup_250___HOOK_RTL_EXECUTE;
  meltcdat->dtup_250___HOOK_RTL_EXECUTE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_250___HOOK_RTL_EXECUTE.nbval = 0;
  /*inimult dtup_251___HOOK_RTL_EXECUTE */
 /*_.VALTUP___V291*/ meltfptr[290] =
    (melt_ptr_t) & meltcdat->dtup_251___HOOK_RTL_EXECUTE;
  meltcdat->dtup_251___HOOK_RTL_EXECUTE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_251___HOOK_RTL_EXECUTE.nbval = 0;

/*inihook dhook__252__HOOK_SIMPLE_IPA_GATE */
  meltcdat->dhook__252__HOOK_SIMPLE_IPA_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_SIMPLE_IPA_GATE data */
  meltcdat->dhook__252__HOOK_SIMPLE_IPA_GATE.nbval = 7;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__252__HOOK_SIMPLE_IPA_GATE,
			  melthook_HOOK_SIMPLE_IPA_GATE);
  meltcdat->dhook__252__HOOK_SIMPLE_IPA_GATE.hookdata =
    /*_.VALDATA___V295*/ meltfptr[294];
  strncpy (meltcdat->dhook__252__HOOK_SIMPLE_IPA_GATE.hookname,
	   "HOOK_SIMPLE_IPA_GATE",
	   sizeof (meltcdat->dhook__252__HOOK_SIMPLE_IPA_GATE.hookname) - 1);
/*predefinedhook HOOK_SIMPLE_IPA_GATE for dhook__252__HOOK_SIMPLE_IPA_GATE*/
  if (!MELT_PREDEF (HOOK_SIMPLE_IPA_GATE))
    MELT_STORE_PREDEF (HOOK_SIMPLE_IPA_GATE,
		       (melt_ptr_t) & meltcdat->
		       dhook__252__HOOK_SIMPLE_IPA_GATE);

  /*_.VALHOOK___V292*/ meltfptr[291] =
  /*inihooklocvar HOOK_SIMPLE_IPA_GATE */
  (melt_ptr_t) & meltcdat->dhook__252__HOOK_SIMPLE_IPA_GATE;

  /*done inihook dhook__252__HOOK_SIMPLE_IPA_GATE */

  MELT_LOCATION ("warmelt-hooks.melt:1812:/ iniobj");

/*iniobj dobj_253__*/
/*_.VALDATA___V295*/ meltfptr[294] = (melt_ptr_t) & meltcdat->dobj_253__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_253__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_253__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_253__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_253__.obj_hash = 52812016;
  meltcdat->dobj_253__.obj_len = 6;

  /*inimult dtup_254___HOOK_SIMPLE_IPA_GATE */
 /*_.VALTUP___V297*/ meltfptr[296] =
    (melt_ptr_t) & meltcdat->dtup_254___HOOK_SIMPLE_IPA_GATE;
  meltcdat->dtup_254___HOOK_SIMPLE_IPA_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_254___HOOK_SIMPLE_IPA_GATE.nbval = 0;
  /*inimult dtup_255___HOOK_SIMPLE_IPA_GATE */
 /*_.VALTUP___V298*/ meltfptr[297] =
    (melt_ptr_t) & meltcdat->dtup_255___HOOK_SIMPLE_IPA_GATE;
  meltcdat->dtup_255___HOOK_SIMPLE_IPA_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_255___HOOK_SIMPLE_IPA_GATE.nbval = 0;

/*iniobj dsym_256__CLASS_GCC_SIMPLE_IPA_PASS*/
  /*uniqueobj*/ if (! /*_.VALDATA___V293*/ meltfptr[292])
/*_.VALDATA___V293*/ meltfptr[292] =
      (melt_ptr_t) & meltcdat->dsym_256__CLASS_GCC_SIMPLE_IPA_PASS;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_256__CLASS_GCC_SIMPLE_IPA_PASS",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_256__CLASS_GCC_SIMPLE_IPA_PASS",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_256__CLASS_GCC_SIMPLE_IPA_PASS.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_256__CLASS_GCC_SIMPLE_IPA_PASS.obj_hash = 827993790;
  meltcdat->dsym_256__CLASS_GCC_SIMPLE_IPA_PASS.obj_len = 3;


/*inistring dstr_257__CLASS_GCC_SIMPLE*/
 /*_.VALSTR___V294*/ meltfptr[293] =
    (melt_ptr_t) & meltcdat->dstr_257__CLASS_GCC_SIMPLE;
  meltcdat->dstr_257__CLASS_GCC_SIMPLE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_257__CLASS_GCC_SIMPLE.val,
				"CLASS_GCC_SIMPLE_IPA_PASS",
				sizeof (meltcdat->dstr_257__CLASS_GCC_SIMPLE.
					val) - 1);
  meltcdat->dstr_257__CLASS_GCC_SIMPLE.val[25] = (char) 0;
  meltcdat->dstr_257__CLASS_GCC_SIMPLE.slen = 25;


/*inihook dhook__258__HOOK_SIMPLE_IPA_EXECUTE */
  meltcdat->dhook__258__HOOK_SIMPLE_IPA_EXECUTE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_SIMPLE_IPA_EXECUTE data */
  meltcdat->dhook__258__HOOK_SIMPLE_IPA_EXECUTE.nbval = 5;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__258__HOOK_SIMPLE_IPA_EXECUTE,
			  melthook_HOOK_SIMPLE_IPA_EXECUTE);
  meltcdat->dhook__258__HOOK_SIMPLE_IPA_EXECUTE.hookdata =
    /*_.VALDATA___V300*/ meltfptr[299];
  strncpy (meltcdat->dhook__258__HOOK_SIMPLE_IPA_EXECUTE.hookname,
	   "HOOK_SIMPLE_IPA_EXECUTE",
	   sizeof (meltcdat->dhook__258__HOOK_SIMPLE_IPA_EXECUTE.hookname) -
	   1);
/*predefinedhook HOOK_SIMPLE_IPA_EXECUTE for dhook__258__HOOK_SIMPLE_IPA_EXECUTE*/
  if (!MELT_PREDEF (HOOK_SIMPLE_IPA_EXECUTE))
    MELT_STORE_PREDEF (HOOK_SIMPLE_IPA_EXECUTE,
		       (melt_ptr_t) & meltcdat->
		       dhook__258__HOOK_SIMPLE_IPA_EXECUTE);

  /*_.VALHOOK___V299*/ meltfptr[298] =
  /*inihooklocvar HOOK_SIMPLE_IPA_EXECUTE */
  (melt_ptr_t) & meltcdat->dhook__258__HOOK_SIMPLE_IPA_EXECUTE;

  /*done inihook dhook__258__HOOK_SIMPLE_IPA_EXECUTE */

  MELT_LOCATION ("warmelt-hooks.melt:1867:/ iniobj");

/*iniobj dobj_259__*/
/*_.VALDATA___V300*/ meltfptr[299] = (melt_ptr_t) & meltcdat->dobj_259__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_259__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_259__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_259__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_259__.obj_hash = 12143643;
  meltcdat->dobj_259__.obj_len = 6;

  /*inimult dtup_260___HOOK_SIMPLE_IPA_EXECUTE */
 /*_.VALTUP___V302*/ meltfptr[301] =
    (melt_ptr_t) & meltcdat->dtup_260___HOOK_SIMPLE_IPA_EXECUTE;
  meltcdat->dtup_260___HOOK_SIMPLE_IPA_EXECUTE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_260___HOOK_SIMPLE_IPA_EXECUTE.nbval = 0;
  /*inimult dtup_261___HOOK_SIMPLE_IPA_EXECUTE */
 /*_.VALTUP___V303*/ meltfptr[302] =
    (melt_ptr_t) & meltcdat->dtup_261___HOOK_SIMPLE_IPA_EXECUTE;
  meltcdat->dtup_261___HOOK_SIMPLE_IPA_EXECUTE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_261___HOOK_SIMPLE_IPA_EXECUTE.nbval = 0;

/*iniobj dsym_262__AT_FINISH_UNIT_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V304*/ meltfptr[303])
/*_.VALDATA___V304*/ meltfptr[303] =
      (melt_ptr_t) & meltcdat->dsym_262__AT_FINISH_UNIT_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_262__AT_FINISH_UNIT_FIRST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_262__AT_FINISH_UNIT_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_262__AT_FINISH_UNIT_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_262__AT_FINISH_UNIT_FIRST.obj_hash = 390084996;
  meltcdat->dsym_262__AT_FINISH_UNIT_FIRST.obj_len = 3;


/*inistring dstr_263__AT_FINISH_UNIT_F*/
 /*_.VALSTR___V305*/ meltfptr[304] =
    (melt_ptr_t) & meltcdat->dstr_263__AT_FINISH_UNIT_F;
  meltcdat->dstr_263__AT_FINISH_UNIT_F.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_263__AT_FINISH_UNIT_F.val,
				"AT_FINISH_UNIT_FIRST",
				sizeof (meltcdat->dstr_263__AT_FINISH_UNIT_F.
					val) - 1);
  meltcdat->dstr_263__AT_FINISH_UNIT_F.val[20] = (char) 0;
  meltcdat->dstr_263__AT_FINISH_UNIT_F.slen = 20;


/*iniobj dsym_264__AT_FINISH_UNIT_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V306*/ meltfptr[305])
/*_.VALDATA___V306*/ meltfptr[305] =
      (melt_ptr_t) & meltcdat->dsym_264__AT_FINISH_UNIT_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_264__AT_FINISH_UNIT_LAST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_264__AT_FINISH_UNIT_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_264__AT_FINISH_UNIT_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_264__AT_FINISH_UNIT_LAST.obj_hash = 289829591;
  meltcdat->dsym_264__AT_FINISH_UNIT_LAST.obj_len = 3;


/*inistring dstr_265__AT_FINISH_UNIT_L*/
 /*_.VALSTR___V307*/ meltfptr[306] =
    (melt_ptr_t) & meltcdat->dstr_265__AT_FINISH_UNIT_L;
  meltcdat->dstr_265__AT_FINISH_UNIT_L.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_265__AT_FINISH_UNIT_L.val,
				"AT_FINISH_UNIT_LAST",
				sizeof (meltcdat->dstr_265__AT_FINISH_UNIT_L.
					val) - 1);
  meltcdat->dstr_265__AT_FINISH_UNIT_L.val[19] = (char) 0;
  meltcdat->dstr_265__AT_FINISH_UNIT_L.slen = 19;


/*iniobj dsym_266__AT_START_UNIT_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V308*/ meltfptr[307])
/*_.VALDATA___V308*/ meltfptr[307] =
      (melt_ptr_t) & meltcdat->dsym_266__AT_START_UNIT_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_266__AT_START_UNIT_FIRST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_266__AT_START_UNIT_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_266__AT_START_UNIT_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_266__AT_START_UNIT_FIRST.obj_hash = 536593345;
  meltcdat->dsym_266__AT_START_UNIT_FIRST.obj_len = 3;


/*inistring dstr_267__AT_START_UNIT_FI*/
 /*_.VALSTR___V309*/ meltfptr[308] =
    (melt_ptr_t) & meltcdat->dstr_267__AT_START_UNIT_FI;
  meltcdat->dstr_267__AT_START_UNIT_FI.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_267__AT_START_UNIT_FI.val,
				"AT_START_UNIT_FIRST",
				sizeof (meltcdat->dstr_267__AT_START_UNIT_FI.
					val) - 1);
  meltcdat->dstr_267__AT_START_UNIT_FI.val[19] = (char) 0;
  meltcdat->dstr_267__AT_START_UNIT_FI.slen = 19;


/*iniobj dsym_268__AT_START_UNIT_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V310*/ meltfptr[309])
/*_.VALDATA___V310*/ meltfptr[309] =
      (melt_ptr_t) & meltcdat->dsym_268__AT_START_UNIT_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_268__AT_START_UNIT_LAST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_268__AT_START_UNIT_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_268__AT_START_UNIT_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_268__AT_START_UNIT_LAST.obj_hash = 817460620;
  meltcdat->dsym_268__AT_START_UNIT_LAST.obj_len = 3;


/*inistring dstr_269__AT_START_UNIT_LA*/
 /*_.VALSTR___V311*/ meltfptr[310] =
    (melt_ptr_t) & meltcdat->dstr_269__AT_START_UNIT_LA;
  meltcdat->dstr_269__AT_START_UNIT_LA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_269__AT_START_UNIT_LA.val,
				"AT_START_UNIT_LAST",
				sizeof (meltcdat->dstr_269__AT_START_UNIT_LA.
					val) - 1);
  meltcdat->dstr_269__AT_START_UNIT_LA.val[18] = (char) 0;
  meltcdat->dstr_269__AT_START_UNIT_LA.slen = 18;


/*iniobj dsym_270__HOOK_LOW_DEBUG_VALUE_AT*/
  /*uniqueobj*/ if (! /*_.VALDATA___V312*/ meltfptr[311])
/*_.VALDATA___V312*/ meltfptr[311] =
      (melt_ptr_t) & meltcdat->dsym_270__HOOK_LOW_DEBUG_VALUE_AT;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_270__HOOK_LOW_DEBUG_VALUE_AT",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_270__HOOK_LOW_DEBUG_VALUE_AT",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_270__HOOK_LOW_DEBUG_VALUE_AT.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_270__HOOK_LOW_DEBUG_VALUE_AT.obj_hash = 1048470198;
  meltcdat->dsym_270__HOOK_LOW_DEBUG_VALUE_AT.obj_len = 3;


/*inistring dstr_271__HOOK_LOW_DEBUG_V*/
 /*_.VALSTR___V313*/ meltfptr[312] =
    (melt_ptr_t) & meltcdat->dstr_271__HOOK_LOW_DEBUG_V;
  meltcdat->dstr_271__HOOK_LOW_DEBUG_V.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_271__HOOK_LOW_DEBUG_V.val,
				"HOOK_LOW_DEBUG_VALUE_AT",
				sizeof (meltcdat->dstr_271__HOOK_LOW_DEBUG_V.
					val) - 1);
  meltcdat->dstr_271__HOOK_LOW_DEBUG_V.val[23] = (char) 0;
  meltcdat->dstr_271__HOOK_LOW_DEBUG_V.slen = 23;


/*iniobj dsym_272__HOOK_OVERRIDE_GATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V314*/ meltfptr[313])
/*_.VALDATA___V314*/ meltfptr[313] =
      (melt_ptr_t) & meltcdat->dsym_272__HOOK_OVERRIDE_GATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_272__HOOK_OVERRIDE_GATE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_272__HOOK_OVERRIDE_GATE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_272__HOOK_OVERRIDE_GATE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_272__HOOK_OVERRIDE_GATE.obj_hash = 995989956;
  meltcdat->dsym_272__HOOK_OVERRIDE_GATE.obj_len = 3;


/*inistring dstr_273__HOOK_OVERRIDE_GA*/
 /*_.VALSTR___V315*/ meltfptr[314] =
    (melt_ptr_t) & meltcdat->dstr_273__HOOK_OVERRIDE_GA;
  meltcdat->dstr_273__HOOK_OVERRIDE_GA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_273__HOOK_OVERRIDE_GA.val,
				"HOOK_OVERRIDE_GATE",
				sizeof (meltcdat->dstr_273__HOOK_OVERRIDE_GA.
					val) - 1);
  meltcdat->dstr_273__HOOK_OVERRIDE_GA.val[18] = (char) 0;
  meltcdat->dstr_273__HOOK_OVERRIDE_GA.slen = 18;


/*iniobj dsym_274__REGISTER_ALL_IPA_PASSES_END_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V316*/ meltfptr[315])
/*_.VALDATA___V316*/ meltfptr[315] =
      (melt_ptr_t) & meltcdat->dsym_274__REGISTER_ALL_IPA_PASSES_END_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_274__REGISTER_ALL_IPA_PASSES_END_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_274__REGISTER_ALL_IPA_PASSES_END_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_274__REGISTER_ALL_IPA_PASSES_END_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_274__REGISTER_ALL_IPA_PASSES_END_FIRST.obj_hash = 188504921;
  meltcdat->dsym_274__REGISTER_ALL_IPA_PASSES_END_FIRST.obj_len = 3;


/*inistring dstr_275__REGISTER_ALL_IPA*/
 /*_.VALSTR___V317*/ meltfptr[316] =
    (melt_ptr_t) & meltcdat->dstr_275__REGISTER_ALL_IPA;
  meltcdat->dstr_275__REGISTER_ALL_IPA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_275__REGISTER_ALL_IPA.val,
				"REGISTER_ALL_IPA_PASSES_END_FIRST",
				sizeof (meltcdat->dstr_275__REGISTER_ALL_IPA.
					val) - 1);
  meltcdat->dstr_275__REGISTER_ALL_IPA.val[33] = (char) 0;
  meltcdat->dstr_275__REGISTER_ALL_IPA.slen = 33;


/*iniobj dsym_276__REGISTER_ALL_IPA_PASSES_END_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V318*/ meltfptr[317])
/*_.VALDATA___V318*/ meltfptr[317] =
      (melt_ptr_t) & meltcdat->dsym_276__REGISTER_ALL_IPA_PASSES_END_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_276__REGISTER_ALL_IPA_PASSES_END_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_276__REGISTER_ALL_IPA_PASSES_END_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_276__REGISTER_ALL_IPA_PASSES_END_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_276__REGISTER_ALL_IPA_PASSES_END_LAST.obj_hash = 822070917;
  meltcdat->dsym_276__REGISTER_ALL_IPA_PASSES_END_LAST.obj_len = 3;


/*inistring dstr_277__REGISTER_ALL_IPA*/
 /*_.VALSTR___V319*/ meltfptr[318] =
    (melt_ptr_t) & meltcdat->dstr_277__REGISTER_ALL_IPA;
  meltcdat->dstr_277__REGISTER_ALL_IPA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_277__REGISTER_ALL_IPA.val,
				"REGISTER_ALL_IPA_PASSES_END_LAST",
				sizeof (meltcdat->dstr_277__REGISTER_ALL_IPA.
					val) - 1);
  meltcdat->dstr_277__REGISTER_ALL_IPA.val[32] = (char) 0;
  meltcdat->dstr_277__REGISTER_ALL_IPA.slen = 32;


/*iniobj dsym_278__REGISTER_ALL_IPA_PASSES_START_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V320*/ meltfptr[319])
/*_.VALDATA___V320*/ meltfptr[319] =
      (melt_ptr_t) & meltcdat->dsym_278__REGISTER_ALL_IPA_PASSES_START_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_278__REGISTER_ALL_IPA_PASSES_START_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_278__REGISTER_ALL_IPA_PASSES_START_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_278__REGISTER_ALL_IPA_PASSES_START_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_278__REGISTER_ALL_IPA_PASSES_START_FIRST.obj_hash = 87049035;
  meltcdat->dsym_278__REGISTER_ALL_IPA_PASSES_START_FIRST.obj_len = 3;


/*inistring dstr_279__REGISTER_ALL_IPA*/
 /*_.VALSTR___V321*/ meltfptr[320] =
    (melt_ptr_t) & meltcdat->dstr_279__REGISTER_ALL_IPA;
  meltcdat->dstr_279__REGISTER_ALL_IPA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_279__REGISTER_ALL_IPA.val,
				"REGISTER_ALL_IPA_PASSES_START_FIRST",
				sizeof (meltcdat->dstr_279__REGISTER_ALL_IPA.
					val) - 1);
  meltcdat->dstr_279__REGISTER_ALL_IPA.val[35] = (char) 0;
  meltcdat->dstr_279__REGISTER_ALL_IPA.slen = 35;


/*iniobj dsym_280__REGISTER_ALL_IPA_PASSES_START_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V322*/ meltfptr[321])
/*_.VALDATA___V322*/ meltfptr[321] =
      (melt_ptr_t) & meltcdat->dsym_280__REGISTER_ALL_IPA_PASSES_START_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_280__REGISTER_ALL_IPA_PASSES_START_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_280__REGISTER_ALL_IPA_PASSES_START_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_280__REGISTER_ALL_IPA_PASSES_START_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_280__REGISTER_ALL_IPA_PASSES_START_LAST.obj_hash = 169109429;
  meltcdat->dsym_280__REGISTER_ALL_IPA_PASSES_START_LAST.obj_len = 3;


/*inistring dstr_281__REGISTER_ALL_IPA*/
 /*_.VALSTR___V323*/ meltfptr[322] =
    (melt_ptr_t) & meltcdat->dstr_281__REGISTER_ALL_IPA;
  meltcdat->dstr_281__REGISTER_ALL_IPA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_281__REGISTER_ALL_IPA.val,
				"REGISTER_ALL_IPA_PASSES_START_LAST",
				sizeof (meltcdat->dstr_281__REGISTER_ALL_IPA.
					val) - 1);
  meltcdat->dstr_281__REGISTER_ALL_IPA.val[34] = (char) 0;
  meltcdat->dstr_281__REGISTER_ALL_IPA.slen = 34;


/*iniobj dsym_282__REGISTER_ALL_PASSES_END_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V324*/ meltfptr[323])
/*_.VALDATA___V324*/ meltfptr[323] =
      (melt_ptr_t) & meltcdat->dsym_282__REGISTER_ALL_PASSES_END_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_282__REGISTER_ALL_PASSES_END_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_282__REGISTER_ALL_PASSES_END_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_282__REGISTER_ALL_PASSES_END_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_282__REGISTER_ALL_PASSES_END_FIRST.obj_hash = 377865029;
  meltcdat->dsym_282__REGISTER_ALL_PASSES_END_FIRST.obj_len = 3;


/*inistring dstr_283__REGISTER_ALL_PAS*/
 /*_.VALSTR___V325*/ meltfptr[324] =
    (melt_ptr_t) & meltcdat->dstr_283__REGISTER_ALL_PAS;
  meltcdat->dstr_283__REGISTER_ALL_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_283__REGISTER_ALL_PAS.val,
				"REGISTER_ALL_PASSES_END_FIRST",
				sizeof (meltcdat->dstr_283__REGISTER_ALL_PAS.
					val) - 1);
  meltcdat->dstr_283__REGISTER_ALL_PAS.val[29] = (char) 0;
  meltcdat->dstr_283__REGISTER_ALL_PAS.slen = 29;


/*iniobj dsym_284__REGISTER_ALL_PASSES_END_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V326*/ meltfptr[325])
/*_.VALDATA___V326*/ meltfptr[325] =
      (melt_ptr_t) & meltcdat->dsym_284__REGISTER_ALL_PASSES_END_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_284__REGISTER_ALL_PASSES_END_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_284__REGISTER_ALL_PASSES_END_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_284__REGISTER_ALL_PASSES_END_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_284__REGISTER_ALL_PASSES_END_LAST.obj_hash = 469287204;
  meltcdat->dsym_284__REGISTER_ALL_PASSES_END_LAST.obj_len = 3;


/*inistring dstr_285__REGISTER_ALL_PAS*/
 /*_.VALSTR___V327*/ meltfptr[326] =
    (melt_ptr_t) & meltcdat->dstr_285__REGISTER_ALL_PAS;
  meltcdat->dstr_285__REGISTER_ALL_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_285__REGISTER_ALL_PAS.val,
				"REGISTER_ALL_PASSES_END_LAST",
				sizeof (meltcdat->dstr_285__REGISTER_ALL_PAS.
					val) - 1);
  meltcdat->dstr_285__REGISTER_ALL_PAS.val[28] = (char) 0;
  meltcdat->dstr_285__REGISTER_ALL_PAS.slen = 28;


/*iniobj dsym_286__REGISTER_ALL_PASSES_START_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V328*/ meltfptr[327])
/*_.VALDATA___V328*/ meltfptr[327] =
      (melt_ptr_t) & meltcdat->dsym_286__REGISTER_ALL_PASSES_START_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_286__REGISTER_ALL_PASSES_START_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_286__REGISTER_ALL_PASSES_START_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_286__REGISTER_ALL_PASSES_START_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_286__REGISTER_ALL_PASSES_START_FIRST.obj_hash = 861337342;
  meltcdat->dsym_286__REGISTER_ALL_PASSES_START_FIRST.obj_len = 3;


/*inistring dstr_287__REGISTER_ALL_PAS*/
 /*_.VALSTR___V329*/ meltfptr[328] =
    (melt_ptr_t) & meltcdat->dstr_287__REGISTER_ALL_PAS;
  meltcdat->dstr_287__REGISTER_ALL_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_287__REGISTER_ALL_PAS.val,
				"REGISTER_ALL_PASSES_START_FIRST",
				sizeof (meltcdat->dstr_287__REGISTER_ALL_PAS.
					val) - 1);
  meltcdat->dstr_287__REGISTER_ALL_PAS.val[31] = (char) 0;
  meltcdat->dstr_287__REGISTER_ALL_PAS.slen = 31;


/*iniobj dsym_288__REGISTER_ALL_PASSES_START_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V330*/ meltfptr[329])
/*_.VALDATA___V330*/ meltfptr[329] =
      (melt_ptr_t) & meltcdat->dsym_288__REGISTER_ALL_PASSES_START_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_288__REGISTER_ALL_PASSES_START_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_288__REGISTER_ALL_PASSES_START_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_288__REGISTER_ALL_PASSES_START_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_288__REGISTER_ALL_PASSES_START_LAST.obj_hash = 428184515;
  meltcdat->dsym_288__REGISTER_ALL_PASSES_START_LAST.obj_len = 3;


/*inistring dstr_289__REGISTER_ALL_PAS*/
 /*_.VALSTR___V331*/ meltfptr[330] =
    (melt_ptr_t) & meltcdat->dstr_289__REGISTER_ALL_PAS;
  meltcdat->dstr_289__REGISTER_ALL_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_289__REGISTER_ALL_PAS.val,
				"REGISTER_ALL_PASSES_START_LAST",
				sizeof (meltcdat->dstr_289__REGISTER_ALL_PAS.
					val) - 1);
  meltcdat->dstr_289__REGISTER_ALL_PAS.val[30] = (char) 0;
  meltcdat->dstr_289__REGISTER_ALL_PAS.slen = 30;


/*iniobj dsym_290__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V332*/ meltfptr[331])
/*_.VALDATA___V332*/ meltfptr[331] =
      (melt_ptr_t) & meltcdat->
      dsym_290__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_290__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_290__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_290__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_290__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST.obj_hash =
    155694900;
  meltcdat->dsym_290__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST.obj_len = 3;


/*inistring dstr_291__REGISTER_EARLY_G*/
 /*_.VALSTR___V333*/ meltfptr[332] =
    (melt_ptr_t) & meltcdat->dstr_291__REGISTER_EARLY_G;
  meltcdat->dstr_291__REGISTER_EARLY_G.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_291__REGISTER_EARLY_G.val,
				"REGISTER_EARLY_GIMPLE_PASSES_END_FIRST",
				sizeof (meltcdat->dstr_291__REGISTER_EARLY_G.
					val) - 1);
  meltcdat->dstr_291__REGISTER_EARLY_G.val[38] = (char) 0;
  meltcdat->dstr_291__REGISTER_EARLY_G.slen = 38;


/*iniobj dsym_292__REGISTER_EARLY_GIMPLE_PASSES_END_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V334*/ meltfptr[333])
/*_.VALDATA___V334*/ meltfptr[333] =
      (melt_ptr_t) & meltcdat->
      dsym_292__REGISTER_EARLY_GIMPLE_PASSES_END_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_292__REGISTER_EARLY_GIMPLE_PASSES_END_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_292__REGISTER_EARLY_GIMPLE_PASSES_END_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_292__REGISTER_EARLY_GIMPLE_PASSES_END_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_292__REGISTER_EARLY_GIMPLE_PASSES_END_LAST.obj_hash =
    523935070;
  meltcdat->dsym_292__REGISTER_EARLY_GIMPLE_PASSES_END_LAST.obj_len = 3;


/*inistring dstr_293__REGISTER_EARLY_G*/
 /*_.VALSTR___V335*/ meltfptr[334] =
    (melt_ptr_t) & meltcdat->dstr_293__REGISTER_EARLY_G;
  meltcdat->dstr_293__REGISTER_EARLY_G.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_293__REGISTER_EARLY_G.val,
				"REGISTER_EARLY_GIMPLE_PASSES_END_LAST",
				sizeof (meltcdat->dstr_293__REGISTER_EARLY_G.
					val) - 1);
  meltcdat->dstr_293__REGISTER_EARLY_G.val[37] = (char) 0;
  meltcdat->dstr_293__REGISTER_EARLY_G.slen = 37;


/*iniobj dsym_294__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V336*/ meltfptr[335])
/*_.VALDATA___V336*/ meltfptr[335] =
      (melt_ptr_t) & meltcdat->
      dsym_294__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_294__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_294__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_294__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_294__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST.obj_hash =
    62165641;
  meltcdat->dsym_294__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST.obj_len = 3;


/*inistring dstr_295__REGISTER_EARLY_G*/
 /*_.VALSTR___V337*/ meltfptr[336] =
    (melt_ptr_t) & meltcdat->dstr_295__REGISTER_EARLY_G;
  meltcdat->dstr_295__REGISTER_EARLY_G.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_295__REGISTER_EARLY_G.val,
				"REGISTER_EARLY_GIMPLE_PASSES_START_FIRST",
				sizeof (meltcdat->dstr_295__REGISTER_EARLY_G.
					val) - 1);
  meltcdat->dstr_295__REGISTER_EARLY_G.val[40] = (char) 0;
  meltcdat->dstr_295__REGISTER_EARLY_G.slen = 40;


/*iniobj dsym_296__REGISTER_EARLY_GIMPLE_PASSES_START_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V338*/ meltfptr[337])
/*_.VALDATA___V338*/ meltfptr[337] =
      (melt_ptr_t) & meltcdat->
      dsym_296__REGISTER_EARLY_GIMPLE_PASSES_START_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_296__REGISTER_EARLY_GIMPLE_PASSES_START_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_296__REGISTER_EARLY_GIMPLE_PASSES_START_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_296__REGISTER_EARLY_GIMPLE_PASSES_START_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_296__REGISTER_EARLY_GIMPLE_PASSES_START_LAST.obj_hash =
    1045063155;
  meltcdat->dsym_296__REGISTER_EARLY_GIMPLE_PASSES_START_LAST.obj_len = 3;


/*inistring dstr_297__REGISTER_EARLY_G*/
 /*_.VALSTR___V339*/ meltfptr[338] =
    (melt_ptr_t) & meltcdat->dstr_297__REGISTER_EARLY_G;
  meltcdat->dstr_297__REGISTER_EARLY_G.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_297__REGISTER_EARLY_G.val,
				"REGISTER_EARLY_GIMPLE_PASSES_START_LAST",
				sizeof (meltcdat->dstr_297__REGISTER_EARLY_G.
					val) - 1);
  meltcdat->dstr_297__REGISTER_EARLY_G.val[39] = (char) 0;
  meltcdat->dstr_297__REGISTER_EARLY_G.slen = 39;


/*iniobj dsym_298__REGISTER_FINISH_TYPE_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V340*/ meltfptr[339])
/*_.VALDATA___V340*/ meltfptr[339] =
      (melt_ptr_t) & meltcdat->dsym_298__REGISTER_FINISH_TYPE_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_298__REGISTER_FINISH_TYPE_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_298__REGISTER_FINISH_TYPE_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_298__REGISTER_FINISH_TYPE_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_298__REGISTER_FINISH_TYPE_FIRST.obj_hash = 801786105;
  meltcdat->dsym_298__REGISTER_FINISH_TYPE_FIRST.obj_len = 3;


/*inistring dstr_299__REGISTER_FINISH_*/
 /*_.VALSTR___V341*/ meltfptr[340] =
    (melt_ptr_t) & meltcdat->dstr_299__REGISTER_FINISH_;
  meltcdat->dstr_299__REGISTER_FINISH_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_299__REGISTER_FINISH_.val,
				"REGISTER_FINISH_TYPE_FIRST",
				sizeof (meltcdat->dstr_299__REGISTER_FINISH_.
					val) - 1);
  meltcdat->dstr_299__REGISTER_FINISH_.val[26] = (char) 0;
  meltcdat->dstr_299__REGISTER_FINISH_.slen = 26;


/*iniobj dsym_300__REGISTER_FINISH_TYPE_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V342*/ meltfptr[341])
/*_.VALDATA___V342*/ meltfptr[341] =
      (melt_ptr_t) & meltcdat->dsym_300__REGISTER_FINISH_TYPE_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_300__REGISTER_FINISH_TYPE_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_300__REGISTER_FINISH_TYPE_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_300__REGISTER_FINISH_TYPE_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_300__REGISTER_FINISH_TYPE_LAST.obj_hash = 1006242336;
  meltcdat->dsym_300__REGISTER_FINISH_TYPE_LAST.obj_len = 3;


/*inistring dstr_301__REGISTER_FINISH_*/
 /*_.VALSTR___V343*/ meltfptr[342] =
    (melt_ptr_t) & meltcdat->dstr_301__REGISTER_FINISH_;
  meltcdat->dstr_301__REGISTER_FINISH_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_301__REGISTER_FINISH_.val,
				"REGISTER_FINISH_TYPE_LAST",
				sizeof (meltcdat->dstr_301__REGISTER_FINISH_.
					val) - 1);
  meltcdat->dstr_301__REGISTER_FINISH_.val[25] = (char) 0;
  meltcdat->dstr_301__REGISTER_FINISH_.slen = 25;


/*iniobj dsym_302__REGISTER_OVERRIDE_GATE_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V344*/ meltfptr[343])
/*_.VALDATA___V344*/ meltfptr[343] =
      (melt_ptr_t) & meltcdat->dsym_302__REGISTER_OVERRIDE_GATE_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_302__REGISTER_OVERRIDE_GATE_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_302__REGISTER_OVERRIDE_GATE_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_302__REGISTER_OVERRIDE_GATE_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_302__REGISTER_OVERRIDE_GATE_FIRST.obj_hash = 929826072;
  meltcdat->dsym_302__REGISTER_OVERRIDE_GATE_FIRST.obj_len = 3;


/*inistring dstr_303__REGISTER_OVERRID*/
 /*_.VALSTR___V345*/ meltfptr[344] =
    (melt_ptr_t) & meltcdat->dstr_303__REGISTER_OVERRID;
  meltcdat->dstr_303__REGISTER_OVERRID.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_303__REGISTER_OVERRID.val,
				"REGISTER_OVERRIDE_GATE_FIRST",
				sizeof (meltcdat->dstr_303__REGISTER_OVERRID.
					val) - 1);
  meltcdat->dstr_303__REGISTER_OVERRID.val[28] = (char) 0;
  meltcdat->dstr_303__REGISTER_OVERRID.slen = 28;


/*iniobj dsym_304__REGISTER_OVERRIDE_GATE_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V346*/ meltfptr[345])
/*_.VALDATA___V346*/ meltfptr[345] =
      (melt_ptr_t) & meltcdat->dsym_304__REGISTER_OVERRIDE_GATE_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_304__REGISTER_OVERRIDE_GATE_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_304__REGISTER_OVERRIDE_GATE_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_304__REGISTER_OVERRIDE_GATE_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_304__REGISTER_OVERRIDE_GATE_LAST.obj_hash = 387249375;
  meltcdat->dsym_304__REGISTER_OVERRIDE_GATE_LAST.obj_len = 3;


/*inistring dstr_305__REGISTER_OVERRID*/
 /*_.VALSTR___V347*/ meltfptr[346] =
    (melt_ptr_t) & meltcdat->dstr_305__REGISTER_OVERRID;
  meltcdat->dstr_305__REGISTER_OVERRID.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_305__REGISTER_OVERRID.val,
				"REGISTER_OVERRIDE_GATE_LAST",
				sizeof (meltcdat->dstr_305__REGISTER_OVERRID.
					val) - 1);
  meltcdat->dstr_305__REGISTER_OVERRID.val[27] = (char) 0;
  meltcdat->dstr_305__REGISTER_OVERRID.slen = 27;


/*iniobj dsym_306__REGISTER_PASS_EXECUTION_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V348*/ meltfptr[347])
/*_.VALDATA___V348*/ meltfptr[347] =
      (melt_ptr_t) & meltcdat->dsym_306__REGISTER_PASS_EXECUTION_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_306__REGISTER_PASS_EXECUTION_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_306__REGISTER_PASS_EXECUTION_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_306__REGISTER_PASS_EXECUTION_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_306__REGISTER_PASS_EXECUTION_FIRST.obj_hash = 401907923;
  meltcdat->dsym_306__REGISTER_PASS_EXECUTION_FIRST.obj_len = 3;


/*inistring dstr_307__REGISTER_PASS_EX*/
 /*_.VALSTR___V349*/ meltfptr[348] =
    (melt_ptr_t) & meltcdat->dstr_307__REGISTER_PASS_EX;
  meltcdat->dstr_307__REGISTER_PASS_EX.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_307__REGISTER_PASS_EX.val,
				"REGISTER_PASS_EXECUTION_FIRST",
				sizeof (meltcdat->dstr_307__REGISTER_PASS_EX.
					val) - 1);
  meltcdat->dstr_307__REGISTER_PASS_EX.val[29] = (char) 0;
  meltcdat->dstr_307__REGISTER_PASS_EX.slen = 29;


/*iniobj dsym_308__REGISTER_PASS_EXECUTION_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V350*/ meltfptr[349])
/*_.VALDATA___V350*/ meltfptr[349] =
      (melt_ptr_t) & meltcdat->dsym_308__REGISTER_PASS_EXECUTION_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_308__REGISTER_PASS_EXECUTION_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_308__REGISTER_PASS_EXECUTION_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_308__REGISTER_PASS_EXECUTION_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_308__REGISTER_PASS_EXECUTION_LAST.obj_hash = 1006242336;
  meltcdat->dsym_308__REGISTER_PASS_EXECUTION_LAST.obj_len = 3;


/*inistring dstr_309__REGISTER_PASS_EX*/
 /*_.VALSTR___V351*/ meltfptr[350] =
    (melt_ptr_t) & meltcdat->dstr_309__REGISTER_PASS_EX;
  meltcdat->dstr_309__REGISTER_PASS_EX.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_309__REGISTER_PASS_EX.val,
				"REGISTER_PASS_EXECUTION_LAST",
				sizeof (meltcdat->dstr_309__REGISTER_PASS_EX.
					val) - 1);
  meltcdat->dstr_309__REGISTER_PASS_EX.val[28] = (char) 0;
  meltcdat->dstr_309__REGISTER_PASS_EX.slen = 28;


/*iniobj dsym_310__REGISTER_PRE_GENERICIZE_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V352*/ meltfptr[351])
/*_.VALDATA___V352*/ meltfptr[351] =
      (melt_ptr_t) & meltcdat->dsym_310__REGISTER_PRE_GENERICIZE_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_310__REGISTER_PRE_GENERICIZE_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_310__REGISTER_PRE_GENERICIZE_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_310__REGISTER_PRE_GENERICIZE_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_310__REGISTER_PRE_GENERICIZE_FIRST.obj_hash = 551171250;
  meltcdat->dsym_310__REGISTER_PRE_GENERICIZE_FIRST.obj_len = 3;


/*inistring dstr_311__REGISTER_PRE_GEN*/
 /*_.VALSTR___V353*/ meltfptr[352] =
    (melt_ptr_t) & meltcdat->dstr_311__REGISTER_PRE_GEN;
  meltcdat->dstr_311__REGISTER_PRE_GEN.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_311__REGISTER_PRE_GEN.val,
				"REGISTER_PRE_GENERICIZE_FIRST",
				sizeof (meltcdat->dstr_311__REGISTER_PRE_GEN.
					val) - 1);
  meltcdat->dstr_311__REGISTER_PRE_GEN.val[29] = (char) 0;
  meltcdat->dstr_311__REGISTER_PRE_GEN.slen = 29;


/*iniobj dsym_312__REGISTER_PRE_GENERICIZE_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V354*/ meltfptr[353])
/*_.VALDATA___V354*/ meltfptr[353] =
      (melt_ptr_t) & meltcdat->dsym_312__REGISTER_PRE_GENERICIZE_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_312__REGISTER_PRE_GENERICIZE_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_312__REGISTER_PRE_GENERICIZE_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_312__REGISTER_PRE_GENERICIZE_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_312__REGISTER_PRE_GENERICIZE_LAST.obj_hash = 943052200;
  meltcdat->dsym_312__REGISTER_PRE_GENERICIZE_LAST.obj_len = 3;


/*inistring dstr_313__REGISTER_PRE_GEN*/
 /*_.VALSTR___V355*/ meltfptr[354] =
    (melt_ptr_t) & meltcdat->dstr_313__REGISTER_PRE_GEN;
  meltcdat->dstr_313__REGISTER_PRE_GEN.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_313__REGISTER_PRE_GEN.val,
				"REGISTER_PRE_GENERICIZE_LAST",
				sizeof (meltcdat->dstr_313__REGISTER_PRE_GEN.
					val) - 1);
  meltcdat->dstr_313__REGISTER_PRE_GEN.val[28] = (char) 0;
  meltcdat->dstr_313__REGISTER_PRE_GEN.slen = 28;


/*iniobj dsym_314__UNREGISTER_OVERRIDE_GATE_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V356*/ meltfptr[355])
/*_.VALDATA___V356*/ meltfptr[355] =
      (melt_ptr_t) & meltcdat->dsym_314__UNREGISTER_OVERRIDE_GATE_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_314__UNREGISTER_OVERRIDE_GATE_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_314__UNREGISTER_OVERRIDE_GATE_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_314__UNREGISTER_OVERRIDE_GATE_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_314__UNREGISTER_OVERRIDE_GATE_FIRST.obj_hash = 604111710;
  meltcdat->dsym_314__UNREGISTER_OVERRIDE_GATE_FIRST.obj_len = 3;


/*inistring dstr_315__UNREGISTER_OVERR*/
 /*_.VALSTR___V357*/ meltfptr[356] =
    (melt_ptr_t) & meltcdat->dstr_315__UNREGISTER_OVERR;
  meltcdat->dstr_315__UNREGISTER_OVERR.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_315__UNREGISTER_OVERR.val,
				"UNREGISTER_OVERRIDE_GATE_FIRST",
				sizeof (meltcdat->dstr_315__UNREGISTER_OVERR.
					val) - 1);
  meltcdat->dstr_315__UNREGISTER_OVERR.val[30] = (char) 0;
  meltcdat->dstr_315__UNREGISTER_OVERR.slen = 30;


/*iniobj dsym_316__UNREGISTER_OVERRIDE_GATE_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V358*/ meltfptr[357])
/*_.VALDATA___V358*/ meltfptr[357] =
      (melt_ptr_t) & meltcdat->dsym_316__UNREGISTER_OVERRIDE_GATE_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_316__UNREGISTER_OVERRIDE_GATE_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_316__UNREGISTER_OVERRIDE_GATE_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_316__UNREGISTER_OVERRIDE_GATE_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_316__UNREGISTER_OVERRIDE_GATE_LAST.obj_hash = 458707543;
  meltcdat->dsym_316__UNREGISTER_OVERRIDE_GATE_LAST.obj_len = 3;


/*inistring dstr_317__UNREGISTER_OVERR*/
 /*_.VALSTR___V359*/ meltfptr[358] =
    (melt_ptr_t) & meltcdat->dstr_317__UNREGISTER_OVERR;
  meltcdat->dstr_317__UNREGISTER_OVERR.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_317__UNREGISTER_OVERR.val,
				"UNREGISTER_OVERRIDE_GATE_LAST",
				sizeof (meltcdat->dstr_317__UNREGISTER_OVERR.
					val) - 1);
  meltcdat->dstr_317__UNREGISTER_OVERR.val[29] = (char) 0;
  meltcdat->dstr_317__UNREGISTER_OVERR.slen = 29;
  meltcdat = NULL;
  melt_prohibit_garbcoll = FALSE;

#undef meltfram__
}				/*end initialize_module_meltdata_warmelt_hooks */


#if MELT_HAVE_DEBUG
const char meltmodule_warmelt_hooks__melt_have_debug_enabled[] =
  "MELT module warmelt-hooks have debug enabled";
#else /*!MELT_HAVE_DEBUG */
const char meltmodule_warmelt_hooks__melt_have_debug_disabled[] =
  "MELT module warmelt-hooks have debug disabled";
#endif /*MELT_HAVE_DEBUG */


void *
melt_start_this_module (void *modargp_)
{

  char meltpredefinited[MELTGLOB__LASTGLOB + 8];

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  melt_initial_frame_st meltfram__;	/*declfrastruct */
#define meltframe meltfram__

  /*melt_start_this_module is initial declstructinit */
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit initial routine melt_start_this_module minihash 3343*/

  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
/**initial routine prologue**/
  /* set initial frame marking */
  ((struct melt_callframe_st *) &meltfram__)->mcfr_nbvar =
    /*minihash */ -3343;
  ((struct melt_callframe_st *) &meltfram__)->mcfr_forwmarkrout =
    meltmod__warmelt_hooks__forward_or_mark_module_start_frame;
 /**COMMENT: get previous environment **/ ;

  /*^compute */
  /*_.PREVENV___V4*/ meltfptr[3] = /*getprevenv */ (melt_ptr_t) modargp_;
  /*^comment */
 /**COMMENT: compute boxloc **/ ;

  /*^cond */
  /*cond */ if ( /*_.CONTENV___V2*/ meltfptr[1] || melt_object_length ((melt_ptr_t) MELT_PREDEF (INITIAL_SYSTEM_DATA)) < MELTFIELD_SYSDATA_CONT_FRESH_ENV)	/*then */
    {
    }
  else
    {				/*^cond.else */

      /*^block */
      /*com.block:compute fresh module environment */
      {

	/*^comment */
   /**COMMENT: start computing boxloc **/ ;
	;
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (((melt_ptr_t) (MELT_PREDEF (INITIAL_SYSTEM_DATA))))
	    /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "SYSDATA_CONT_FRESH_ENV");
    /*_.FRESHENV___V3*/ meltfptr[2] = slot;
	};
	;
	/*^compute */

	/*checkfreshenv */ if (( /*_.PREVENV___V4*/ meltfptr[3])
			       &&
			       melt_magic_discr ((melt_ptr_t)
						 ( /*_.FRESHENV___V3*/
						  meltfptr[2])) !=
			       MELTOBMAG_CLOSURE)
	  warning (0,
		   "MELT corruption: bad FRESH_ENV @%p in system data <%s:%d>",
		   (void *) ( /*_.FRESHENV___V3*/ meltfptr[2]), __FILE__,
		   __LINE__);;;
	/*^apply */
	/*apply */
	{
	  /*_.CONTENV___V2*/ meltfptr[1] =
	    melt_apply ((meltclosure_ptr_t)
			( /*_.FRESHENV___V3*/ meltfptr[2]),
			(melt_ptr_t) ( /*_.PREVENV___V4*/ meltfptr[3]), (""),
			(union meltparam_un *) 0, "",
			(union meltparam_un *) 0);
	}
	;
      }				/*com.end block:compute fresh module environment */
      ;
    }

  /*^comment */
 /**COMMENT: get symbols & keywords for initial procedure **/ ;

  /*^getnamedsymbol */
  /*getnamedsym:VAL */

  if (NULL == /*_.VALDATA___V24*/ meltfptr[23])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol VAL without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_VAL =
	melthookproc_HOOK_NAMED_SYMBOL ("VAL", (long) MELT_GET);
   /*_.VALDATA___V24*/ meltfptr[23] = (melt_ptr_t) meltsy_VAL;
    }				/*endgetnamedsym VAL */


  /*^getnamedsymbol */
  /*getnamedsym:FILENAME */

  if (NULL == /*_.VALDATA___V27*/ meltfptr[26])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol FILENAME without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_FILENAME =
	melthookproc_HOOK_NAMED_SYMBOL ("FILENAME", (long) MELT_GET);
   /*_.VALDATA___V27*/ meltfptr[26] = (melt_ptr_t) meltsy_FILENAME;
    }				/*endgetnamedsym FILENAME */


  /*^getnamedsymbol */
  /*getnamedsym:LINENO */

  if (NULL == /*_.VALDATA___V30*/ meltfptr[29])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol LINENO without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_LINENO =
	melthookproc_HOOK_NAMED_SYMBOL ("LINENO", (long) MELT_GET);
   /*_.VALDATA___V30*/ meltfptr[29] = (melt_ptr_t) meltsy_LINENO;
    }				/*endgetnamedsym LINENO */


  /*^getnamedsymbol */
  /*getnamedsym:MSG */

  if (NULL == /*_.VALDATA___V33*/ meltfptr[32])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MSG without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_MSG =
	melthookproc_HOOK_NAMED_SYMBOL ("MSG", (long) MELT_GET);
   /*_.VALDATA___V33*/ meltfptr[32] = (melt_ptr_t) meltsy_MSG;
    }				/*endgetnamedsym MSG */


  /*^getnamedsymbol */
  /*getnamedsym:COUNT */

  if (NULL == /*_.VALDATA___V36*/ meltfptr[35])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol COUNT without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_COUNT =
	melthookproc_HOOK_NAMED_SYMBOL ("COUNT", (long) MELT_GET);
   /*_.VALDATA___V36*/ meltfptr[35] = (melt_ptr_t) meltsy_COUNT;
    }				/*endgetnamedsym COUNT */


  /*^getnamedsymbol */
  /*getnamedsym:MELT_DEBUG_FUN */

  if (NULL == /*_.VALDATA___V19*/ meltfptr[18])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MELT_DEBUG_FUN without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_MELT_DEBUG_FUN =
	melthookproc_HOOK_NAMED_SYMBOL ("MELT_DEBUG_FUN", (long) MELT_GET);
   /*_.VALDATA___V19*/ meltfptr[18] = (melt_ptr_t) meltsy_MELT_DEBUG_FUN;
    }				/*endgetnamedsym MELT_DEBUG_FUN */


  /*^getnamedsymbol */
  /*getnamedsym:OVERRIDE_GATE_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V40*/ meltfptr[39])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol OVERRIDE_GATE_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_OVERRIDE_GATE_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("OVERRIDE_GATE_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V40*/ meltfptr[39] =
	(melt_ptr_t) meltsy_OVERRIDE_GATE_DELAYED_QUEUE;
    }				/*endgetnamedsym OVERRIDE_GATE_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:CLASS_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V42*/ meltfptr[41])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_CLASS_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("CLASS_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V42*/ meltfptr[41] =
	(melt_ptr_t) meltsy_CLASS_DELAYED_QUEUE;
    }				/*endgetnamedsym CLASS_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:DISCR_LIST */

  if (NULL == /*_.VALDATA___V48*/ meltfptr[47])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol DISCR_LIST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_DISCR_LIST =
	melthookproc_HOOK_NAMED_SYMBOL ("DISCR_LIST", (long) MELT_GET);
   /*_.VALDATA___V48*/ meltfptr[47] = (melt_ptr_t) meltsy_DISCR_LIST;
    }				/*endgetnamedsym DISCR_LIST */


  /*^getnamedsymbol */
  /*getnamedsym:BEFOREGATE */

  if (NULL == /*_.VALDATA___V58*/ meltfptr[57])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol BEFOREGATE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_BEFOREGATE =
	melthookproc_HOOK_NAMED_SYMBOL ("BEFOREGATE", (long) MELT_GET);
   /*_.VALDATA___V58*/ meltfptr[57] = (melt_ptr_t) meltsy_BEFOREGATE;
    }				/*endgetnamedsym BEFOREGATE */


  /*^getnamedsymbol */
  /*getnamedsym:AFTERGATE */

  if (NULL == /*_.VALDATA___V62*/ meltfptr[61])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AFTERGATE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AFTERGATE =
	melthookproc_HOOK_NAMED_SYMBOL ("AFTERGATE", (long) MELT_GET);
   /*_.VALDATA___V62*/ meltfptr[61] = (melt_ptr_t) meltsy_AFTERGATE;
    }				/*endgetnamedsym AFTERGATE */


  /*^getnamedkeyword */
  /*getnamedkeyw:TRUE */
  {
    melt_ptr_t kw_TRUE =
      melthookproc_HOOK_NAMED_KEYWORD ("TRUE", (long) MELT_GET);
    if (kw_TRUE)
	       /*_.VALDATA___V51*/
      meltfptr[50] = (melt_ptr_t) kw_TRUE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:CLASS_REFERENCE */

  if (NULL == /*_.VALDATA___V53*/ meltfptr[52])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_REFERENCE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_CLASS_REFERENCE =
	melthookproc_HOOK_NAMED_SYMBOL ("CLASS_REFERENCE", (long) MELT_GET);
   /*_.VALDATA___V53*/ meltfptr[52] = (melt_ptr_t) meltsy_CLASS_REFERENCE;
    }				/*endgetnamedsym CLASS_REFERENCE */


  /*^getnamedsymbol */
  /*getnamedsym:START_UNIT_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V77*/ meltfptr[76])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol START_UNIT_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_START_UNIT_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("START_UNIT_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V77*/ meltfptr[76] =
	(melt_ptr_t) meltsy_START_UNIT_DELAYED_QUEUE;
    }				/*endgetnamedsym START_UNIT_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:LIST_EVERY */

  if (NULL == /*_.VALDATA___V84*/ meltfptr[83])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol LIST_EVERY without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_LIST_EVERY =
	melthookproc_HOOK_NAMED_SYMBOL ("LIST_EVERY", (long) MELT_GET);
   /*_.VALDATA___V84*/ meltfptr[83] = (melt_ptr_t) meltsy_LIST_EVERY;
    }				/*endgetnamedsym LIST_EVERY */


  /*^getnamedsymbol */
  /*getnamedsym:FINISH_UNIT_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V99*/ meltfptr[98])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol FINISH_UNIT_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_FINISH_UNIT_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("FINISH_UNIT_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V99*/ meltfptr[98] =
	(melt_ptr_t) meltsy_FINISH_UNIT_DELAYED_QUEUE;
    }				/*endgetnamedsym FINISH_UNIT_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:ALL_PASSES_START_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V119*/ meltfptr[118])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol ALL_PASSES_START_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_ALL_PASSES_START_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("ALL_PASSES_START_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V119*/ meltfptr[118] =
	(melt_ptr_t) meltsy_ALL_PASSES_START_DELAYED_QUEUE;
    }				/*endgetnamedsym ALL_PASSES_START_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:ALL_PASSES_END_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V134*/ meltfptr[133])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol ALL_PASSES_END_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_ALL_PASSES_END_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("ALL_PASSES_END_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V134*/ meltfptr[133] =
	(melt_ptr_t) meltsy_ALL_PASSES_END_DELAYED_QUEUE;
    }				/*endgetnamedsym ALL_PASSES_END_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:ALL_IPA_PASSES_START_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V149*/ meltfptr[148])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol ALL_IPA_PASSES_START_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_ALL_IPA_PASSES_START_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("ALL_IPA_PASSES_START_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V149*/ meltfptr[148] =
	(melt_ptr_t) meltsy_ALL_IPA_PASSES_START_DELAYED_QUEUE;
    }				/*endgetnamedsym ALL_IPA_PASSES_START_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:ALL_IPA_PASSES_END_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V164*/ meltfptr[163])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol ALL_IPA_PASSES_END_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_ALL_IPA_PASSES_END_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("ALL_IPA_PASSES_END_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V164*/ meltfptr[163] =
	(melt_ptr_t) meltsy_ALL_IPA_PASSES_END_DELAYED_QUEUE;
    }				/*endgetnamedsym ALL_IPA_PASSES_END_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V179*/ meltfptr[178])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL
	("EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE", (long) MELT_GET);
   /*_.VALDATA___V179*/ meltfptr[178] =
	(melt_ptr_t) meltsy_EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE;
    }				/*endgetnamedsym EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V194*/ meltfptr[193])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL
	("EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE", (long) MELT_GET);
   /*_.VALDATA___V194*/ meltfptr[193] =
	(melt_ptr_t) meltsy_EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE;
    }				/*endgetnamedsym EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:PREGENERICIZE_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V209*/ meltfptr[208])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol PREGENERICIZE_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_PREGENERICIZE_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("PREGENERICIZE_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V209*/ meltfptr[208] =
	(melt_ptr_t) meltsy_PREGENERICIZE_DELAYED_QUEUE;
    }				/*endgetnamedsym PREGENERICIZE_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:TFNDECL */

  if (NULL == /*_.VALDATA___V219*/ meltfptr[218])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol TFNDECL without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_TFNDECL =
	melthookproc_HOOK_NAMED_SYMBOL ("TFNDECL", (long) MELT_GET);
   /*_.VALDATA___V219*/ meltfptr[218] = (melt_ptr_t) meltsy_TFNDECL;
    }				/*endgetnamedsym TFNDECL */


  /*^getnamedsymbol */
  /*getnamedsym:FINISHTYPE_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V227*/ meltfptr[226])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol FINISHTYPE_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_FINISHTYPE_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("FINISHTYPE_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V227*/ meltfptr[226] =
	(melt_ptr_t) meltsy_FINISHTYPE_DELAYED_QUEUE;
    }				/*endgetnamedsym FINISHTYPE_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:PASS_EXECUTION_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V243*/ meltfptr[242])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol PASS_EXECUTION_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_PASS_EXECUTION_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("PASS_EXECUTION_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V243*/ meltfptr[242] =
	(melt_ptr_t) meltsy_PASS_EXECUTION_DELAYED_QUEUE;
    }				/*endgetnamedsym PASS_EXECUTION_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:PASSNAME */

  if (NULL == /*_.VALDATA___V251*/ meltfptr[250])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol PASSNAME without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_PASSNAME =
	melthookproc_HOOK_NAMED_SYMBOL ("PASSNAME", (long) MELT_GET);
   /*_.VALDATA___V251*/ meltfptr[250] = (melt_ptr_t) meltsy_PASSNAME;
    }				/*endgetnamedsym PASSNAME */


  /*^getnamedsymbol */
  /*getnamedsym:PASSNUM */

  if (NULL == /*_.VALDATA___V254*/ meltfptr[253])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol PASSNUM without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_PASSNUM =
	melthookproc_HOOK_NAMED_SYMBOL ("PASSNUM", (long) MELT_GET);
   /*_.VALDATA___V254*/ meltfptr[253] = (melt_ptr_t) meltsy_PASSNUM;
    }				/*endgetnamedsym PASSNUM */


  /*^getnamedsymbol */
  /*getnamedsym:INITIAL_SYSTEM_DATA */

  if (NULL == /*_.VALDATA___V263*/ meltfptr[262])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol INITIAL_SYSTEM_DATA without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_INITIAL_SYSTEM_DATA =
	melthookproc_HOOK_NAMED_SYMBOL ("INITIAL_SYSTEM_DATA",
					(long) MELT_GET);
   /*_.VALDATA___V263*/ meltfptr[262] =
	(melt_ptr_t) meltsy_INITIAL_SYSTEM_DATA;
    }				/*endgetnamedsym INITIAL_SYSTEM_DATA */


  /*^getnamedsymbol */
  /*getnamedsym:CLASS_SYSTEM_DATA */

  if (NULL == /*_.VALDATA___V265*/ meltfptr[264])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_SYSTEM_DATA without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_CLASS_SYSTEM_DATA =
	melthookproc_HOOK_NAMED_SYMBOL ("CLASS_SYSTEM_DATA", (long) MELT_GET);
   /*_.VALDATA___V265*/ meltfptr[264] =
	(melt_ptr_t) meltsy_CLASS_SYSTEM_DATA;
    }				/*endgetnamedsym CLASS_SYSTEM_DATA */


  /*^getnamedsymbol */
  /*getnamedsym:CLASS_GCC_GIMPLE_PASS */

  if (NULL == /*_.VALDATA___V267*/ meltfptr[266])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_GCC_GIMPLE_PASS without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_CLASS_GCC_GIMPLE_PASS =
	melthookproc_HOOK_NAMED_SYMBOL ("CLASS_GCC_GIMPLE_PASS",
					(long) MELT_GET);
   /*_.VALDATA___V267*/ meltfptr[266] =
	(melt_ptr_t) meltsy_CLASS_GCC_GIMPLE_PASS;
    }				/*endgetnamedsym CLASS_GCC_GIMPLE_PASS */


  /*^getnamedsymbol */
  /*getnamedsym:CLASS_GCC_PASS */

  if (NULL == /*_.VALDATA___V269*/ meltfptr[268])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_GCC_PASS without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_CLASS_GCC_PASS =
	melthookproc_HOOK_NAMED_SYMBOL ("CLASS_GCC_PASS", (long) MELT_GET);
   /*_.VALDATA___V269*/ meltfptr[268] = (melt_ptr_t) meltsy_CLASS_GCC_PASS;
    }				/*endgetnamedsym CLASS_GCC_PASS */


  /*^getnamedsymbol */
  /*getnamedsym:CLASS_GCC_RTL_PASS */

  if (NULL == /*_.VALDATA___V281*/ meltfptr[280])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_GCC_RTL_PASS without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_CLASS_GCC_RTL_PASS =
	melthookproc_HOOK_NAMED_SYMBOL ("CLASS_GCC_RTL_PASS",
					(long) MELT_GET);
   /*_.VALDATA___V281*/ meltfptr[280] =
	(melt_ptr_t) meltsy_CLASS_GCC_RTL_PASS;
    }				/*endgetnamedsym CLASS_GCC_RTL_PASS */


  /*^getnamedsymbol */
  /*getnamedsym:CLASS_GCC_SIMPLE_IPA_PASS */

  if (NULL == /*_.VALDATA___V293*/ meltfptr[292])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_GCC_SIMPLE_IPA_PASS without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_CLASS_GCC_SIMPLE_IPA_PASS =
	melthookproc_HOOK_NAMED_SYMBOL ("CLASS_GCC_SIMPLE_IPA_PASS",
					(long) MELT_GET);
   /*_.VALDATA___V293*/ meltfptr[292] =
	(melt_ptr_t) meltsy_CLASS_GCC_SIMPLE_IPA_PASS;
    }				/*endgetnamedsym CLASS_GCC_SIMPLE_IPA_PASS */


  /*^getnamedsymbol */
  /*getnamedsym:AT_FINISH_UNIT_FIRST */

  if (NULL == /*_.VALDATA___V304*/ meltfptr[303])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AT_FINISH_UNIT_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AT_FINISH_UNIT_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("AT_FINISH_UNIT_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V304*/ meltfptr[303] =
	(melt_ptr_t) meltsy_AT_FINISH_UNIT_FIRST;
    }				/*endgetnamedsym AT_FINISH_UNIT_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:AT_FINISH_UNIT_LAST */

  if (NULL == /*_.VALDATA___V306*/ meltfptr[305])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AT_FINISH_UNIT_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AT_FINISH_UNIT_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("AT_FINISH_UNIT_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V306*/ meltfptr[305] =
	(melt_ptr_t) meltsy_AT_FINISH_UNIT_LAST;
    }				/*endgetnamedsym AT_FINISH_UNIT_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:AT_START_UNIT_FIRST */

  if (NULL == /*_.VALDATA___V308*/ meltfptr[307])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AT_START_UNIT_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AT_START_UNIT_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("AT_START_UNIT_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V308*/ meltfptr[307] =
	(melt_ptr_t) meltsy_AT_START_UNIT_FIRST;
    }				/*endgetnamedsym AT_START_UNIT_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:AT_START_UNIT_LAST */

  if (NULL == /*_.VALDATA___V310*/ meltfptr[309])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AT_START_UNIT_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AT_START_UNIT_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("AT_START_UNIT_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V310*/ meltfptr[309] =
	(melt_ptr_t) meltsy_AT_START_UNIT_LAST;
    }				/*endgetnamedsym AT_START_UNIT_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:HOOK_LOW_DEBUG_VALUE_AT */

  if (NULL == /*_.VALDATA___V312*/ meltfptr[311])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol HOOK_LOW_DEBUG_VALUE_AT without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_HOOK_LOW_DEBUG_VALUE_AT =
	melthookproc_HOOK_NAMED_SYMBOL ("HOOK_LOW_DEBUG_VALUE_AT",
					(long) MELT_GET);
   /*_.VALDATA___V312*/ meltfptr[311] =
	(melt_ptr_t) meltsy_HOOK_LOW_DEBUG_VALUE_AT;
    }				/*endgetnamedsym HOOK_LOW_DEBUG_VALUE_AT */


  /*^getnamedsymbol */
  /*getnamedsym:HOOK_OVERRIDE_GATE */

  if (NULL == /*_.VALDATA___V314*/ meltfptr[313])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol HOOK_OVERRIDE_GATE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_HOOK_OVERRIDE_GATE =
	melthookproc_HOOK_NAMED_SYMBOL ("HOOK_OVERRIDE_GATE",
					(long) MELT_GET);
   /*_.VALDATA___V314*/ meltfptr[313] =
	(melt_ptr_t) meltsy_HOOK_OVERRIDE_GATE;
    }				/*endgetnamedsym HOOK_OVERRIDE_GATE */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_IPA_PASSES_END_FIRST */

  if (NULL == /*_.VALDATA___V316*/ meltfptr[315])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_IPA_PASSES_END_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_IPA_PASSES_END_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_IPA_PASSES_END_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V316*/ meltfptr[315] =
	(melt_ptr_t) meltsy_REGISTER_ALL_IPA_PASSES_END_FIRST;
    }				/*endgetnamedsym REGISTER_ALL_IPA_PASSES_END_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_IPA_PASSES_END_LAST */

  if (NULL == /*_.VALDATA___V318*/ meltfptr[317])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_IPA_PASSES_END_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_IPA_PASSES_END_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_IPA_PASSES_END_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V318*/ meltfptr[317] =
	(melt_ptr_t) meltsy_REGISTER_ALL_IPA_PASSES_END_LAST;
    }				/*endgetnamedsym REGISTER_ALL_IPA_PASSES_END_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_IPA_PASSES_START_FIRST */

  if (NULL == /*_.VALDATA___V320*/ meltfptr[319])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_IPA_PASSES_START_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_IPA_PASSES_START_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_IPA_PASSES_START_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V320*/ meltfptr[319] =
	(melt_ptr_t) meltsy_REGISTER_ALL_IPA_PASSES_START_FIRST;
    }				/*endgetnamedsym REGISTER_ALL_IPA_PASSES_START_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_IPA_PASSES_START_LAST */

  if (NULL == /*_.VALDATA___V322*/ meltfptr[321])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_IPA_PASSES_START_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_IPA_PASSES_START_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_IPA_PASSES_START_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V322*/ meltfptr[321] =
	(melt_ptr_t) meltsy_REGISTER_ALL_IPA_PASSES_START_LAST;
    }				/*endgetnamedsym REGISTER_ALL_IPA_PASSES_START_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_PASSES_END_FIRST */

  if (NULL == /*_.VALDATA___V324*/ meltfptr[323])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_PASSES_END_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_PASSES_END_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_PASSES_END_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V324*/ meltfptr[323] =
	(melt_ptr_t) meltsy_REGISTER_ALL_PASSES_END_FIRST;
    }				/*endgetnamedsym REGISTER_ALL_PASSES_END_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_PASSES_END_LAST */

  if (NULL == /*_.VALDATA___V326*/ meltfptr[325])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_PASSES_END_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_PASSES_END_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_PASSES_END_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V326*/ meltfptr[325] =
	(melt_ptr_t) meltsy_REGISTER_ALL_PASSES_END_LAST;
    }				/*endgetnamedsym REGISTER_ALL_PASSES_END_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_PASSES_START_FIRST */

  if (NULL == /*_.VALDATA___V328*/ meltfptr[327])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_PASSES_START_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_PASSES_START_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_PASSES_START_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V328*/ meltfptr[327] =
	(melt_ptr_t) meltsy_REGISTER_ALL_PASSES_START_FIRST;
    }				/*endgetnamedsym REGISTER_ALL_PASSES_START_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_PASSES_START_LAST */

  if (NULL == /*_.VALDATA___V330*/ meltfptr[329])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_PASSES_START_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_PASSES_START_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_PASSES_START_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V330*/ meltfptr[329] =
	(melt_ptr_t) meltsy_REGISTER_ALL_PASSES_START_LAST;
    }				/*endgetnamedsym REGISTER_ALL_PASSES_START_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_EARLY_GIMPLE_PASSES_END_FIRST */

  if (NULL == /*_.VALDATA___V332*/ meltfptr[331])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_EARLY_GIMPLE_PASSES_END_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL
	("REGISTER_EARLY_GIMPLE_PASSES_END_FIRST", (long) MELT_GET);
   /*_.VALDATA___V332*/ meltfptr[331] =
	(melt_ptr_t) meltsy_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST;
    }				/*endgetnamedsym REGISTER_EARLY_GIMPLE_PASSES_END_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_EARLY_GIMPLE_PASSES_END_LAST */

  if (NULL == /*_.VALDATA___V334*/ meltfptr[333])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_EARLY_GIMPLE_PASSES_END_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_EARLY_GIMPLE_PASSES_END_LAST =
	melthookproc_HOOK_NAMED_SYMBOL
	("REGISTER_EARLY_GIMPLE_PASSES_END_LAST", (long) MELT_GET);
   /*_.VALDATA___V334*/ meltfptr[333] =
	(melt_ptr_t) meltsy_REGISTER_EARLY_GIMPLE_PASSES_END_LAST;
    }				/*endgetnamedsym REGISTER_EARLY_GIMPLE_PASSES_END_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_EARLY_GIMPLE_PASSES_START_FIRST */

  if (NULL == /*_.VALDATA___V336*/ meltfptr[335])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_EARLY_GIMPLE_PASSES_START_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_EARLY_GIMPLE_PASSES_START_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL
	("REGISTER_EARLY_GIMPLE_PASSES_START_FIRST", (long) MELT_GET);
   /*_.VALDATA___V336*/ meltfptr[335] =
	(melt_ptr_t) meltsy_REGISTER_EARLY_GIMPLE_PASSES_START_FIRST;
    }				/*endgetnamedsym REGISTER_EARLY_GIMPLE_PASSES_START_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_EARLY_GIMPLE_PASSES_START_LAST */

  if (NULL == /*_.VALDATA___V338*/ meltfptr[337])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_EARLY_GIMPLE_PASSES_START_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_EARLY_GIMPLE_PASSES_START_LAST =
	melthookproc_HOOK_NAMED_SYMBOL
	("REGISTER_EARLY_GIMPLE_PASSES_START_LAST", (long) MELT_GET);
   /*_.VALDATA___V338*/ meltfptr[337] =
	(melt_ptr_t) meltsy_REGISTER_EARLY_GIMPLE_PASSES_START_LAST;
    }				/*endgetnamedsym REGISTER_EARLY_GIMPLE_PASSES_START_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_FINISH_TYPE_FIRST */

  if (NULL == /*_.VALDATA___V340*/ meltfptr[339])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_FINISH_TYPE_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_FINISH_TYPE_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_FINISH_TYPE_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V340*/ meltfptr[339] =
	(melt_ptr_t) meltsy_REGISTER_FINISH_TYPE_FIRST;
    }				/*endgetnamedsym REGISTER_FINISH_TYPE_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_FINISH_TYPE_LAST */

  if (NULL == /*_.VALDATA___V342*/ meltfptr[341])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_FINISH_TYPE_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_FINISH_TYPE_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_FINISH_TYPE_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V342*/ meltfptr[341] =
	(melt_ptr_t) meltsy_REGISTER_FINISH_TYPE_LAST;
    }				/*endgetnamedsym REGISTER_FINISH_TYPE_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_OVERRIDE_GATE_FIRST */

  if (NULL == /*_.VALDATA___V344*/ meltfptr[343])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_OVERRIDE_GATE_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_OVERRIDE_GATE_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_OVERRIDE_GATE_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V344*/ meltfptr[343] =
	(melt_ptr_t) meltsy_REGISTER_OVERRIDE_GATE_FIRST;
    }				/*endgetnamedsym REGISTER_OVERRIDE_GATE_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_OVERRIDE_GATE_LAST */

  if (NULL == /*_.VALDATA___V346*/ meltfptr[345])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_OVERRIDE_GATE_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_OVERRIDE_GATE_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_OVERRIDE_GATE_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V346*/ meltfptr[345] =
	(melt_ptr_t) meltsy_REGISTER_OVERRIDE_GATE_LAST;
    }				/*endgetnamedsym REGISTER_OVERRIDE_GATE_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_PASS_EXECUTION_FIRST */

  if (NULL == /*_.VALDATA___V348*/ meltfptr[347])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_PASS_EXECUTION_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_PASS_EXECUTION_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_PASS_EXECUTION_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V348*/ meltfptr[347] =
	(melt_ptr_t) meltsy_REGISTER_PASS_EXECUTION_FIRST;
    }				/*endgetnamedsym REGISTER_PASS_EXECUTION_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_PASS_EXECUTION_LAST */

  if (NULL == /*_.VALDATA___V350*/ meltfptr[349])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_PASS_EXECUTION_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_PASS_EXECUTION_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_PASS_EXECUTION_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V350*/ meltfptr[349] =
	(melt_ptr_t) meltsy_REGISTER_PASS_EXECUTION_LAST;
    }				/*endgetnamedsym REGISTER_PASS_EXECUTION_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_PRE_GENERICIZE_FIRST */

  if (NULL == /*_.VALDATA___V352*/ meltfptr[351])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_PRE_GENERICIZE_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_PRE_GENERICIZE_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_PRE_GENERICIZE_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V352*/ meltfptr[351] =
	(melt_ptr_t) meltsy_REGISTER_PRE_GENERICIZE_FIRST;
    }				/*endgetnamedsym REGISTER_PRE_GENERICIZE_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_PRE_GENERICIZE_LAST */

  if (NULL == /*_.VALDATA___V354*/ meltfptr[353])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_PRE_GENERICIZE_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_PRE_GENERICIZE_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_PRE_GENERICIZE_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V354*/ meltfptr[353] =
	(melt_ptr_t) meltsy_REGISTER_PRE_GENERICIZE_LAST;
    }				/*endgetnamedsym REGISTER_PRE_GENERICIZE_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:UNREGISTER_OVERRIDE_GATE_FIRST */

  if (NULL == /*_.VALDATA___V356*/ meltfptr[355])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol UNREGISTER_OVERRIDE_GATE_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_UNREGISTER_OVERRIDE_GATE_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("UNREGISTER_OVERRIDE_GATE_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V356*/ meltfptr[355] =
	(melt_ptr_t) meltsy_UNREGISTER_OVERRIDE_GATE_FIRST;
    }				/*endgetnamedsym UNREGISTER_OVERRIDE_GATE_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:UNREGISTER_OVERRIDE_GATE_LAST */

  if (NULL == /*_.VALDATA___V358*/ meltfptr[357])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol UNREGISTER_OVERRIDE_GATE_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_UNREGISTER_OVERRIDE_GATE_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("UNREGISTER_OVERRIDE_GATE_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V358*/ meltfptr[357] =
	(melt_ptr_t) meltsy_UNREGISTER_OVERRIDE_GATE_LAST;
    }				/*endgetnamedsym UNREGISTER_OVERRIDE_GATE_LAST */


  /*^comment */
 /**COMMENT: get the value importer **/ ;

  /*^getslot */
  {
    melt_ptr_t slot = NULL, obj = NULL;
    obj =
      (melt_ptr_t) (((melt_ptr_t) (MELT_PREDEF (INITIAL_SYSTEM_DATA))))
      /*=obj*/ ;
    melt_object_get_field (slot, obj, 8, "SYSDATA_VALUE_IMPORTER");
  /*_.VALIMPORT___V6*/ meltfptr[5] = slot;
  };

  /*^comment */
 /**COMMENT: before getting imported values **/ ;

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "MELT_DEBUG_FUN";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V19*/ meltfptr[18]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "CLASS_DELAYED_QUEUE";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V42*/ meltfptr[41]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "DISCR_LIST";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.DISCR_LIST__V9*/ meltfptr[8] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V48*/ meltfptr[47]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "CLASS_REFERENCE";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.CLASS_REFERENCE__V10*/ meltfptr[9] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V53*/ meltfptr[52]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "LIST_EVERY";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.LIST_EVERY__V11*/ meltfptr[10] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V84*/ meltfptr[83]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "INITIAL_SYSTEM_DATA";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.INITIAL_SYSTEM_DATA__V12*/ meltfptr[11] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V263*/ meltfptr[262]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "CLASS_SYSTEM_DATA";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.CLASS_SYSTEM_DATA__V13*/ meltfptr[12] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V265*/ meltfptr[264]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "CLASS_GCC_GIMPLE_PASS";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.CLASS_GCC_GIMPLE_PASS__V14*/ meltfptr[13] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V267*/ meltfptr[266]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "CLASS_GCC_PASS";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.CLASS_GCC_PASS__V15*/ meltfptr[14] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V269*/ meltfptr[268]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "CLASS_GCC_RTL_PASS";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.CLASS_GCC_RTL_PASS__V16*/ meltfptr[15] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V281*/ meltfptr[280]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^apply */
  /*apply */
  {
    union meltparam_un argtab[3];
    memset (&argtab, 0, sizeof (argtab));
    /*^apply.arg */
    argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.PREVENV___V4*/ meltfptr[3];
    /*^apply.arg */
    argtab[1].meltbp_cstring = "CLASS_GCC_SIMPLE_IPA_PASS";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.CLASS_GCC_SIMPLE_IPA_PASS__V17*/ meltfptr[16] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V293*/ meltfptr[292]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^comment */
 /**COMMENT: after getting imported values **/ ;

 /**initial routine cdata initializer**/

  memset (meltpredefinited, 0, sizeof (meltpredefinited));
  initialize_module_meltdata_warmelt_hooks (&meltfram__, meltpredefinited);
 /**initial routine body**/

  meltmod__warmelt_hooks__initialmeltchunk_0 (&meltfram__, meltpredefinited);
  meltmod__warmelt_hooks__initialmeltchunk_1 (&meltfram__, meltpredefinited);
  meltmod__warmelt_hooks__initialmeltchunk_2 (&meltfram__, meltpredefinited);
  meltmod__warmelt_hooks__initialmeltchunk_3 (&meltfram__, meltpredefinited);
  meltmod__warmelt_hooks__initialmeltchunk_4 (&meltfram__, meltpredefinited);
  meltmod__warmelt_hooks__initialmeltchunk_5 (&meltfram__, meltpredefinited);
  meltmod__warmelt_hooks__initialmeltchunk_6 (&meltfram__, meltpredefinited);;
  goto meltlabend_rout;
meltlabend_rout:;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
/* popped initial frame */
  {				/* clear initial frame & return */
    melt_ptr_t retval = /*_.RETINIT___V1*/ meltfptr[0];
    memset ((void *) &meltfram__, 0, sizeof (meltfram__));
    return retval;
  }
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR

}				/* end  */

void
meltmod__warmelt_hooks__initialmeltchunk_0 (meltinitial_frame_t *
					    meltmeltframptr__,
					    char meltpredefinited[])
{
#define meltfram__ (*meltmeltframptr__)
#undef meltcallcount
#define meltcallcount 0L
  (void) meltpredefinited;
/**COMMENT: start of oinibody **/ ;

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V19*/ meltfptr[18])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V19*/ meltfptr[18]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V19*/ meltfptr[18]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V20*/ meltfptr[19]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V19*/ meltfptr[18]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V19*/ meltfptr[18],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V21*/ meltfptr[20])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V21*/ meltfptr[20]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V21*/ meltfptr[20]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V22*/ meltfptr[21]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V24*/ meltfptr[23])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V24*/ meltfptr[23]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V24*/ meltfptr[23]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V25*/ meltfptr[24]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V24*/ meltfptr[23]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V24*/ meltfptr[23],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V23*/ meltfptr[22])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V23*/ meltfptr[22]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V23*/ meltfptr[22]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V24*/ meltfptr[23]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V23*/ meltfptr[22])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V23*/ meltfptr[22]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V23*/ meltfptr[22]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VALUE))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V23*/ meltfptr[22]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V23*/ meltfptr[22],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V27*/ meltfptr[26])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V27*/ meltfptr[26]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V27*/ meltfptr[26]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V28*/ meltfptr[27]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V27*/ meltfptr[26]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V27*/ meltfptr[26],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V26*/ meltfptr[25])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V26*/ meltfptr[25]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V26*/ meltfptr[25]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V27*/ meltfptr[26]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V26*/ meltfptr[25])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V26*/ meltfptr[25]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V26*/ meltfptr[25]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_CSTRING))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V26*/ meltfptr[25]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V26*/ meltfptr[25],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V30*/ meltfptr[29])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V30*/ meltfptr[29]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V30*/ meltfptr[29]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V31*/ meltfptr[30]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V30*/ meltfptr[29]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V30*/ meltfptr[29],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V29*/ meltfptr[28])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V29*/ meltfptr[28]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V29*/ meltfptr[28]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V30*/ meltfptr[29]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V29*/ meltfptr[28])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V29*/ meltfptr[28]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V29*/ meltfptr[28]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V29*/ meltfptr[28]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V29*/ meltfptr[28],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V33*/ meltfptr[32])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V33*/ meltfptr[32]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V33*/ meltfptr[32]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V34*/ meltfptr[33]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V33*/ meltfptr[32]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V33*/ meltfptr[32],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V32*/ meltfptr[31])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V32*/ meltfptr[31]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V32*/ meltfptr[31]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V33*/ meltfptr[32]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V32*/ meltfptr[31])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V32*/ meltfptr[31]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V32*/ meltfptr[31]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_CSTRING))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V32*/ meltfptr[31]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V32*/ meltfptr[31],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V36*/ meltfptr[35])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V36*/ meltfptr[35]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V36*/ meltfptr[35]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V37*/ meltfptr[36]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V36*/ meltfptr[35]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V36*/ meltfptr[35],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V35*/ meltfptr[34])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V35*/ meltfptr[34]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V35*/ meltfptr[34]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V36*/ meltfptr[35]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V35*/ meltfptr[34])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V35*/ meltfptr[34]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V35*/ meltfptr[34]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V35*/ meltfptr[34]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V35*/ meltfptr[34],
				"new static instance");

  /*putupl#1 */
  melt_assertmsg ("putupl #1 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V38*/ meltfptr[37])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #1 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V38*/
					  meltfptr[37]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V38*/ meltfptr[37]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V23*/ meltfptr[22]);

  /*putupl#2 */
  melt_assertmsg ("putupl #2 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V38*/ meltfptr[37])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #2 checkoff",
		  (1 >= 0
		   && 1 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V38*/
					  meltfptr[37]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V38*/ meltfptr[37]))->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V26*/ meltfptr[25]);

  /*putupl#3 */
  melt_assertmsg ("putupl #3 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V38*/ meltfptr[37])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #3 checkoff",
		  (2 >= 0
		   && 2 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V38*/
					  meltfptr[37]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V38*/ meltfptr[37]))->tabval[2] =
    (melt_ptr_t) ( /*_.VALDATA___V29*/ meltfptr[28]);

  /*putupl#4 */
  melt_assertmsg ("putupl #4 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V38*/ meltfptr[37])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #4 checkoff",
		  (3 >= 0
		   && 3 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V38*/
					  meltfptr[37]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V38*/ meltfptr[37]))->tabval[3] =
    (melt_ptr_t) ( /*_.VALDATA___V32*/ meltfptr[31]);

  /*putupl#5 */
  melt_assertmsg ("putupl #5 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V38*/ meltfptr[37])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #5 checkoff",
		  (4 >= 0
		   && 4 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V38*/
					  meltfptr[37]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V38*/ meltfptr[37]))->tabval[4] =
    (melt_ptr_t) ( /*_.VALDATA___V35*/ meltfptr[34]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V38*/ meltfptr[37]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V21*/ meltfptr[20])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V21*/ meltfptr[20]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V21*/ meltfptr[20]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V38*/ meltfptr[37]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V39*/ meltfptr[38]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V21*/ meltfptr[20])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V21*/ meltfptr[20]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V21*/ meltfptr[20]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V39*/ meltfptr[38]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V21*/ meltfptr[20])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V21*/ meltfptr[20]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V21*/ meltfptr[20]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V21*/ meltfptr[20]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V21*/ meltfptr[20],
				"new static instance");

  /*put data in hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookdata checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V18*/ meltfptr[17])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V18*/ meltfptr[17])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V21*/ meltfptr[20]);

  /*put inside hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookconst checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V18*/
				  meltfptr[17]));
  ((melthook_ptr_t) /*_.VALHOOK___V18*/ meltfptr[17])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*put inside hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookconst checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V18*/
				  meltfptr[17]));
  ((melthook_ptr_t) /*_.VALHOOK___V18*/ meltfptr[17])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V19*/ meltfptr[18]);

  /*put inside hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookconst checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V18*/
				  meltfptr[17]));
  ((melthook_ptr_t) /*_.VALHOOK___V18*/ meltfptr[17])->tabval[2] =
    (melt_ptr_t) ( /*_.VALSTR___V20*/ meltfptr[19]);

  /*touch:HOOK_LOW_DEBUG_VALUE_AT */
  meltgc_touch ( /*_.VALHOOK___V18*/ meltfptr[17]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V40*/ meltfptr[39])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V40*/ meltfptr[39]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V40*/ meltfptr[39]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V41*/ meltfptr[40]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V40*/ meltfptr[39]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V40*/ meltfptr[39],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V42*/ meltfptr[41])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V42*/ meltfptr[41]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V42*/ meltfptr[41]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V43*/ meltfptr[42]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V42*/ meltfptr[41]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V42*/ meltfptr[41],
				"new static instance");

 /*_.MAKE_LIST__V45*/ meltfptr[44] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
 /*_.MAKE_LIST__V46*/ meltfptr[45] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V44*/ meltfptr[43])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V44*/ meltfptr[43]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V47*/ meltfptr[46]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V44*/ meltfptr[43])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V44*/ meltfptr[43]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V45*/ meltfptr[44]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V44*/ meltfptr[43])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V44*/ meltfptr[43]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V46*/ meltfptr[45]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V44*/ meltfptr[43]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V44*/ meltfptr[43],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V48*/ meltfptr[47])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V48*/ meltfptr[47]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V48*/ meltfptr[47]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V49*/ meltfptr[48]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V48*/ meltfptr[47]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V48*/ meltfptr[47],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V51*/ meltfptr[50])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V51*/ meltfptr[50]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V51*/ meltfptr[50]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V52*/ meltfptr[51]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V51*/ meltfptr[50]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V51*/ meltfptr[50],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V53*/ meltfptr[52])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V53*/ meltfptr[52]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V53*/ meltfptr[52]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V54*/ meltfptr[53]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V53*/ meltfptr[52]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V53*/ meltfptr[52],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V55*/ meltfptr[54])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V55*/ meltfptr[54]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V55*/ meltfptr[54]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V56*/ meltfptr[55]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V58*/ meltfptr[57])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V58*/ meltfptr[57]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V58*/ meltfptr[57]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V59*/ meltfptr[58]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V58*/ meltfptr[57]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V58*/ meltfptr[57],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V57*/ meltfptr[56])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V57*/ meltfptr[56]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V57*/ meltfptr[56]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V58*/ meltfptr[57]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V57*/ meltfptr[56])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V57*/ meltfptr[56]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V57*/ meltfptr[56]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V57*/ meltfptr[56]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V57*/ meltfptr[56],
				"new static instance");

  /*putupl#6 */
  melt_assertmsg ("putupl #6 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V60*/ meltfptr[59])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #6 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V60*/
					  meltfptr[59]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V60*/ meltfptr[59]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V57*/ meltfptr[56]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V60*/ meltfptr[59]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V55*/ meltfptr[54])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V55*/ meltfptr[54]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V55*/ meltfptr[54]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V60*/ meltfptr[59]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V62*/ meltfptr[61])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V62*/ meltfptr[61]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V62*/ meltfptr[61]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V63*/ meltfptr[62]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V62*/ meltfptr[61]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V62*/ meltfptr[61],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V61*/ meltfptr[60])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V61*/ meltfptr[60]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V61*/ meltfptr[60]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V62*/ meltfptr[61]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V61*/ meltfptr[60])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V61*/ meltfptr[60]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V61*/ meltfptr[60]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V61*/ meltfptr[60]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V61*/ meltfptr[60],
				"new static instance");

  /*putupl#7 */
  melt_assertmsg ("putupl #7 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V64*/ meltfptr[63])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #7 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V64*/
					  meltfptr[63]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V64*/ meltfptr[63]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V61*/ meltfptr[60]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V64*/ meltfptr[63]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V55*/ meltfptr[54])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V55*/ meltfptr[54]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V55*/ meltfptr[54]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V64*/ meltfptr[63]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V55*/ meltfptr[54])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V55*/ meltfptr[54]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V55*/ meltfptr[54]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V55*/ meltfptr[54]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V55*/ meltfptr[54],
				"new static instance");

  /*put data in hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookdata checkhook HOOK_OVERRIDE_GATE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V50*/ meltfptr[49])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V55*/ meltfptr[54]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V50*/
				  meltfptr[49]));
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V50*/
				  meltfptr[49]));
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V50*/
				  meltfptr[49]));
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V50*/
				  meltfptr[49]));
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->tabval[3] =
    (melt_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V50*/
				  meltfptr[49]));
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->tabval[4] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V50*/
				  meltfptr[49]));
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->tabval[5] =
    (melt_ptr_t) ( /*_.VALDATA___V51*/ meltfptr[50]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V50*/
				  meltfptr[49]));
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->tabval[6] =
    (melt_ptr_t) ( /*_.VALSTR___V52*/ meltfptr[51]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (7) >= 0
		  && (7) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V50*/
				  meltfptr[49]));
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->tabval[7] =
    (melt_ptr_t) ( /*_.VALDATA___V53*/ meltfptr[52]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (8) >= 0
		  && (8) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V50*/
				  meltfptr[49]));
  ((melthook_ptr_t) /*_.VALHOOK___V50*/ meltfptr[49])->tabval[8] =
    (melt_ptr_t) ( /*_.VALSTR___V54*/ meltfptr[53]);

  /*touch:HOOK_OVERRIDE_GATE */
  meltgc_touch ( /*_.VALHOOK___V50*/ meltfptr[49]);

  /*putclosurout#1 */
  melt_assertmsg ("putclosrout#1 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V66*/ meltfptr[65])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#1 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V66*/ meltfptr[65])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V65*/ meltfptr[64]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V66*/ meltfptr[65]);

  MELT_LOCATION ("warmelt-hooks.melt:159:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V44*/ meltfptr[43]);
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]);

  MELT_LOCATION ("warmelt-hooks.melt:159:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_46__MAYBE_DISABLE_OVERRIDE_GATE#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:159:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V67*/ meltfptr[66]);

  /*putclosurout#2 */
  melt_assertmsg ("putclosrout#2 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V68*/ meltfptr[67])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#2 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V68*/ meltfptr[67])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V67*/ meltfptr[66]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V68*/ meltfptr[67]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_48__REGISTER_OVERRIDE_GATE_FIRST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V44*/ meltfptr[43]);
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_48__REGISTER_OVERRIDE_GATE_FIRST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V66*/ meltfptr[65]);
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V66*/ meltfptr[65]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V69*/ meltfptr[68]);

  /*putclosurout#3 */
  melt_assertmsg ("putclosrout#3 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V70*/ meltfptr[69])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#3 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V70*/ meltfptr[69])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V69*/ meltfptr[68]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V70*/ meltfptr[69]);


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_0 */

void
meltmod__warmelt_hooks__initialmeltchunk_1 (meltinitial_frame_t *
					    meltmeltframptr__,
					    char meltpredefinited[])
{
#define meltfram__ (*meltmeltframptr__)
#undef meltcallcount
#define meltcallcount 0L
  (void) meltpredefinited;
  MELT_LOCATION ("warmelt-hooks.melt:183:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_50__REGISTER_OVERRIDE_GATE_LAST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V71*/ meltfptr[70])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:183:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V44*/ meltfptr[43]);
  ((meltroutine_ptr_t) /*_.VALROUT___V71*/ meltfptr[70])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]);

  MELT_LOCATION ("warmelt-hooks.melt:183:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_50__REGISTER_OVERRIDE_GATE_LAST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V71*/ meltfptr[70])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:183:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V66*/ meltfptr[65]);
  ((meltroutine_ptr_t) /*_.VALROUT___V71*/ meltfptr[70])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V66*/ meltfptr[65]);

  MELT_LOCATION ("warmelt-hooks.melt:183:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V71*/ meltfptr[70]);

  /*putclosurout#4 */
  melt_assertmsg ("putclosrout#4 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V72*/ meltfptr[71])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#4 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V72*/ meltfptr[71])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V71*/ meltfptr[70]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V72*/ meltfptr[71]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V73*/ meltfptr[72])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_52__UNREGISTER_OVERRIDE_GATE_FIRST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V73*/ meltfptr[72])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V73*/ meltfptr[72])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V44*/ meltfptr[43]);
  ((meltroutine_ptr_t) /*_.VALROUT___V73*/ meltfptr[72])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V73*/ meltfptr[72])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_52__UNREGISTER_OVERRIDE_GATE_FIRST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V73*/ meltfptr[72])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V73*/ meltfptr[72])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_52__UNREGISTER_OVERRIDE_GATE_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V73*/ meltfptr[72])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V73*/ meltfptr[72])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V68*/ meltfptr[67]);
  ((meltroutine_ptr_t) /*_.VALROUT___V73*/ meltfptr[72])->tabval[4] =
    (melt_ptr_t) ( /*_.VALCLO___V68*/ meltfptr[67]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V73*/ meltfptr[72]);

  /*putclosurout#5 */
  melt_assertmsg ("putclosrout#5 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V74*/ meltfptr[73])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#5 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V73*/ meltfptr[72])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V74*/ meltfptr[73])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V73*/ meltfptr[72]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V74*/ meltfptr[73]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V75*/ meltfptr[74])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_54__UNREGISTER_OVERRIDE_GATE_LAST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V75*/ meltfptr[74])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V75*/ meltfptr[74])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V44*/ meltfptr[43]);
  ((meltroutine_ptr_t) /*_.VALROUT___V75*/ meltfptr[74])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V75*/ meltfptr[74])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_54__UNREGISTER_OVERRIDE_GATE_LAST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V75*/ meltfptr[74])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V75*/ meltfptr[74])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_54__UNREGISTER_OVERRIDE_GATE_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V75*/ meltfptr[74])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V75*/ meltfptr[74])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V68*/ meltfptr[67]);
  ((meltroutine_ptr_t) /*_.VALROUT___V75*/ meltfptr[74])->tabval[4] =
    (melt_ptr_t) ( /*_.VALCLO___V68*/ meltfptr[67]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V75*/ meltfptr[74]);

  /*putclosurout#6 */
  melt_assertmsg ("putclosrout#6 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V76*/ meltfptr[75])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#6 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V75*/ meltfptr[74])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V76*/ meltfptr[75])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V75*/ meltfptr[74]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V76*/ meltfptr[75]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V77*/ meltfptr[76])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V77*/ meltfptr[76]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V77*/ meltfptr[76]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V78*/ meltfptr[77]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V77*/ meltfptr[76]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V77*/ meltfptr[76],
				"new static instance");

 /*_.MAKE_LIST__V80*/ meltfptr[79] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
 /*_.MAKE_LIST__V81*/ meltfptr[80] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V79*/ meltfptr[78])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V79*/ meltfptr[78]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V82*/ meltfptr[81]);

  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V79*/ meltfptr[78])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V79*/ meltfptr[78]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V80*/ meltfptr[79]);

  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V79*/ meltfptr[78])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V79*/ meltfptr[78]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V81*/ meltfptr[80]);

  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V79*/ meltfptr[78]);

  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V79*/ meltfptr[78],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V84*/ meltfptr[83])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V84*/ meltfptr[83]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V84*/ meltfptr[83]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V85*/ meltfptr[84]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V84*/ meltfptr[83]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V84*/ meltfptr[83],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:269:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V86*/ meltfptr[85])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V79*/ meltfptr[78]);
  ((meltroutine_ptr_t) /*_.VALROUT___V86*/ meltfptr[85])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]);

  MELT_LOCATION ("warmelt-hooks.melt:269:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V86*/ meltfptr[85]);

  MELT_LOCATION ("warmelt-hooks.melt:283:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V88*/ meltfptr[87])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V79*/ meltfptr[78]);
  ((meltroutine_ptr_t) /*_.VALROUT___V88*/ meltfptr[87])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]);

  MELT_LOCATION ("warmelt-hooks.melt:283:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V88*/ meltfptr[87]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V89*/ meltfptr[88])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V89*/ meltfptr[88]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V89*/ meltfptr[88]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V90*/ meltfptr[89]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V91*/ meltfptr[90]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V89*/ meltfptr[88])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V89*/ meltfptr[88]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V89*/ meltfptr[88]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V91*/ meltfptr[90]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V92*/ meltfptr[91]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V89*/ meltfptr[88])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V89*/ meltfptr[88]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V89*/ meltfptr[88]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V92*/ meltfptr[91]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V89*/ meltfptr[88])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V89*/ meltfptr[88]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V89*/ meltfptr[88]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V89*/ meltfptr[88]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V89*/ meltfptr[88],
				"new static instance");

  /*put data in hook HOOK_START_UNIT */
  melt_assertmsg ("puthookdata checkhook HOOK_START_UNIT",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V83*/ meltfptr[82])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V89*/ meltfptr[88]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V83*/
				  meltfptr[82]));
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V83*/
				  meltfptr[82]));
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->tabval[1] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V83*/
				  meltfptr[82]));
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V83*/
				  meltfptr[82]));
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->tabval[3] =
    (melt_ptr_t) ( /*_.LIST_EVERY__V11*/ meltfptr[10]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V83*/
				  meltfptr[82]));
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->tabval[4] =
    (melt_ptr_t) ( /*_.VALDATA___V84*/ meltfptr[83]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V83*/
				  meltfptr[82]));
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->tabval[5] =
    (melt_ptr_t) ( /*_.VALSTR___V85*/ meltfptr[84]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V83*/
				  meltfptr[82]));
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->tabval[6] =
    (melt_ptr_t) ( /*_.VALROUT___V86*/ meltfptr[85]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (7) >= 0
		  && (7) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V83*/
				  meltfptr[82]));
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->tabval[7] =
    (melt_ptr_t) ( /*_.VALROUT___V87*/ meltfptr[86]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (8) >= 0
		  && (8) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V83*/
				  meltfptr[82]));
  ((melthook_ptr_t) /*_.VALHOOK___V83*/ meltfptr[82])->tabval[8] =
    (melt_ptr_t) ( /*_.VALROUT___V88*/ meltfptr[87]);

  /*touch:HOOK_START_UNIT */
  meltgc_touch ( /*_.VALHOOK___V83*/ meltfptr[82]);

  /*putclosurout#7 */
  melt_assertmsg ("putclosrout#7 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V94*/ meltfptr[93])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#7 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V93*/ meltfptr[92])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V94*/ meltfptr[93])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V93*/ meltfptr[92]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V94*/ meltfptr[93]);

  MELT_LOCATION ("warmelt-hooks.melt:322:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V95*/ meltfptr[94])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V79*/ meltfptr[78]);
  ((meltroutine_ptr_t) /*_.VALROUT___V95*/ meltfptr[94])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]);

  MELT_LOCATION ("warmelt-hooks.melt:322:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V95*/ meltfptr[94])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V94*/ meltfptr[93]);
  ((meltroutine_ptr_t) /*_.VALROUT___V95*/ meltfptr[94])->tabval[1] =
    (melt_ptr_t) ( /*_.VALCLO___V94*/ meltfptr[93]);

  MELT_LOCATION ("warmelt-hooks.melt:322:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V95*/ meltfptr[94]);

  /*putclosurout#8 */
  melt_assertmsg ("putclosrout#8 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V96*/ meltfptr[95])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#8 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V95*/ meltfptr[94])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V96*/ meltfptr[95])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V95*/ meltfptr[94]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V96*/ meltfptr[95]);

  MELT_LOCATION ("warmelt-hooks.melt:331:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V97*/ meltfptr[96])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V79*/ meltfptr[78]);
  ((meltroutine_ptr_t) /*_.VALROUT___V97*/ meltfptr[96])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]);

  MELT_LOCATION ("warmelt-hooks.melt:331:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V97*/ meltfptr[96])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V94*/ meltfptr[93]);
  ((meltroutine_ptr_t) /*_.VALROUT___V97*/ meltfptr[96])->tabval[1] =
    (melt_ptr_t) ( /*_.VALCLO___V94*/ meltfptr[93]);

  MELT_LOCATION ("warmelt-hooks.melt:331:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V97*/ meltfptr[96]);

  /*putclosurout#9 */
  melt_assertmsg ("putclosrout#9 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V98*/ meltfptr[97])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#9 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V97*/ meltfptr[96])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V98*/ meltfptr[97])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V97*/ meltfptr[96]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V98*/ meltfptr[97]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V99*/ meltfptr[98])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V99*/ meltfptr[98]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V99*/ meltfptr[98]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V100*/ meltfptr[99]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V99*/ meltfptr[98]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V99*/ meltfptr[98],
				"new static instance");

 /*_.MAKE_LIST__V102*/ meltfptr[101] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
 /*_.MAKE_LIST__V103*/ meltfptr[102] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V101*/ meltfptr[100])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V101*/
					meltfptr[100]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V101*/ meltfptr[100]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V104*/ meltfptr[103]);

  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V101*/ meltfptr[100])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V101*/
					meltfptr[100]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V101*/ meltfptr[100]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V102*/ meltfptr[101]);

  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V101*/ meltfptr[100])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V101*/
					meltfptr[100]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V101*/ meltfptr[100]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V103*/ meltfptr[102]);

  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V101*/ meltfptr[100]);

  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V101*/ meltfptr[100],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:371:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V106*/ meltfptr[105])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V101*/ meltfptr[100]);
  ((meltroutine_ptr_t) /*_.VALROUT___V106*/ meltfptr[105])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V101*/ meltfptr[100]);

  MELT_LOCATION ("warmelt-hooks.melt:371:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V106*/ meltfptr[105]);

  MELT_LOCATION ("warmelt-hooks.melt:385:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V108*/ meltfptr[107])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V101*/ meltfptr[100]);
  ((meltroutine_ptr_t) /*_.VALROUT___V108*/ meltfptr[107])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V101*/ meltfptr[100]);

  MELT_LOCATION ("warmelt-hooks.melt:385:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V108*/ meltfptr[107]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V109*/ meltfptr[108])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V109*/
					meltfptr[108]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V109*/ meltfptr[108]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V110*/ meltfptr[109]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V111*/ meltfptr[110]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V109*/ meltfptr[108])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V109*/
					meltfptr[108]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V109*/ meltfptr[108]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V111*/ meltfptr[110]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V112*/ meltfptr[111]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V109*/ meltfptr[108])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V109*/
					meltfptr[108]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V109*/ meltfptr[108]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V112*/ meltfptr[111]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V109*/ meltfptr[108])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V109*/
					meltfptr[108]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V109*/ meltfptr[108]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V109*/ meltfptr[108]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V109*/ meltfptr[108],
				"new static instance");

  /*put data in hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookdata checkhook HOOK_FINISH_UNIT",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V105*/ meltfptr[104])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V105*/ meltfptr[104])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V109*/ meltfptr[108]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V105*/
				  meltfptr[104]));
  ((melthook_ptr_t) /*_.VALHOOK___V105*/ meltfptr[104])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V101*/ meltfptr[100]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V105*/
				  meltfptr[104]));
  ((melthook_ptr_t) /*_.VALHOOK___V105*/ meltfptr[104])->tabval[1] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V105*/
				  meltfptr[104]));
  ((melthook_ptr_t) /*_.VALHOOK___V105*/ meltfptr[104])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V105*/
				  meltfptr[104]));
  ((melthook_ptr_t) /*_.VALHOOK___V105*/ meltfptr[104])->tabval[3] =
    (melt_ptr_t) ( /*_.LIST_EVERY__V11*/ meltfptr[10]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V105*/
				  meltfptr[104]));
  ((melthook_ptr_t) /*_.VALHOOK___V105*/ meltfptr[104])->tabval[4] =
    (melt_ptr_t) ( /*_.VALROUT___V106*/ meltfptr[105]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V105*/
				  meltfptr[104]));
  ((melthook_ptr_t) /*_.VALHOOK___V105*/ meltfptr[104])->tabval[5] =
    (melt_ptr_t) ( /*_.VALROUT___V107*/ meltfptr[106]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V105*/
				  meltfptr[104]));
  ((melthook_ptr_t) /*_.VALHOOK___V105*/ meltfptr[104])->tabval[6] =
    (melt_ptr_t) ( /*_.VALROUT___V108*/ meltfptr[107]);

  /*touch:HOOK_FINISH_UNIT */
  meltgc_touch ( /*_.VALHOOK___V105*/ meltfptr[104]);

  /*putclosurout#10 */
  melt_assertmsg ("putclosrout#10 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V114*/ meltfptr[113])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#10 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V113*/ meltfptr[112])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V114*/ meltfptr[113])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V113*/ meltfptr[112]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V114*/ meltfptr[113]);

  MELT_LOCATION ("warmelt-hooks.melt:427:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V115*/ meltfptr[114])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V101*/ meltfptr[100]);
  ((meltroutine_ptr_t) /*_.VALROUT___V115*/ meltfptr[114])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V101*/ meltfptr[100]);

  MELT_LOCATION ("warmelt-hooks.melt:427:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V115*/ meltfptr[114])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V114*/ meltfptr[113]);
  ((meltroutine_ptr_t) /*_.VALROUT___V115*/ meltfptr[114])->tabval[1] =
    (melt_ptr_t) ( /*_.VALCLO___V114*/ meltfptr[113]);

  MELT_LOCATION ("warmelt-hooks.melt:427:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V115*/ meltfptr[114]);

  /*putclosurout#11 */
  melt_assertmsg ("putclosrout#11 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V116*/ meltfptr[115])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#11 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V115*/ meltfptr[114])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V116*/ meltfptr[115])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V115*/ meltfptr[114]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V116*/ meltfptr[115]);

  MELT_LOCATION ("warmelt-hooks.melt:437:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V117*/ meltfptr[116])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V101*/ meltfptr[100]);
  ((meltroutine_ptr_t) /*_.VALROUT___V117*/ meltfptr[116])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V101*/ meltfptr[100]);

  MELT_LOCATION ("warmelt-hooks.melt:437:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V117*/ meltfptr[116])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V114*/ meltfptr[113]);
  ((meltroutine_ptr_t) /*_.VALROUT___V117*/ meltfptr[116])->tabval[1] =
    (melt_ptr_t) ( /*_.VALCLO___V114*/ meltfptr[113]);

  MELT_LOCATION ("warmelt-hooks.melt:437:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V117*/ meltfptr[116]);

  /*putclosurout#12 */
  melt_assertmsg ("putclosrout#12 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V118*/ meltfptr[117])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#12 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V117*/ meltfptr[116])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V118*/ meltfptr[117])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V117*/ meltfptr[116]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V118*/ meltfptr[117]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V119*/ meltfptr[118])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V119*/
					meltfptr[118]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V119*/ meltfptr[118]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V120*/ meltfptr[119]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V119*/ meltfptr[118]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V119*/ meltfptr[118],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V121*/ meltfptr[120])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V121*/
					meltfptr[120]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V121*/ meltfptr[120]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V122*/ meltfptr[121]);

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V121*/ meltfptr[120])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V121*/
					meltfptr[120]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V121*/ meltfptr[120]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V121*/ meltfptr[120])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V121*/
					meltfptr[120]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V121*/ meltfptr[120]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V121*/ meltfptr[120]);

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V121*/ meltfptr[120],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V124*/ meltfptr[123])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V124*/
					meltfptr[123]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V124*/ meltfptr[123]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V125*/ meltfptr[124]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V126*/ meltfptr[125]);

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V124*/ meltfptr[123])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V124*/
					meltfptr[123]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V124*/ meltfptr[123]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V126*/ meltfptr[125]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V127*/ meltfptr[126]);

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V124*/ meltfptr[123])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V124*/
					meltfptr[123]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V124*/ meltfptr[123]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V127*/ meltfptr[126]);

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V124*/ meltfptr[123])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V124*/
					meltfptr[123]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V124*/ meltfptr[123]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_1 */

void
meltmod__warmelt_hooks__initialmeltchunk_2 (meltinitial_frame_t *
					    meltmeltframptr__,
					    char meltpredefinited[])
{
#define meltfram__ (*meltmeltframptr__)
#undef meltcallcount
#define meltcallcount 0L
  (void) meltpredefinited;
  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V124*/ meltfptr[123]);

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V124*/ meltfptr[123],
				"new static instance");

  /*put data in hook HOOK_ALL_PASSES_START */
  melt_assertmsg ("puthookdata checkhook HOOK_ALL_PASSES_START",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V123*/ meltfptr[122])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V123*/ meltfptr[122])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V124*/ meltfptr[123]);

  /*put inside hook HOOK_ALL_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_START",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V123*/
				  meltfptr[122]));
  ((melthook_ptr_t) /*_.VALHOOK___V123*/ meltfptr[122])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V121*/ meltfptr[120]);

  /*put inside hook HOOK_ALL_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_START",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V123*/
				  meltfptr[122]));
  ((melthook_ptr_t) /*_.VALHOOK___V123*/ meltfptr[122])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_ALL_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_START",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V123*/
				  meltfptr[122]));
  ((melthook_ptr_t) /*_.VALHOOK___V123*/ meltfptr[122])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_ALL_PASSES_START */
  meltgc_touch ( /*_.VALHOOK___V123*/ meltfptr[122]);

  /*putclosurout#13 */
  melt_assertmsg ("putclosrout#13 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V129*/ meltfptr[128])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#13 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V128*/ meltfptr[127])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V129*/ meltfptr[128])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V128*/ meltfptr[127]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V129*/ meltfptr[128]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V130*/ meltfptr[129])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V121*/ meltfptr[120]);
  ((meltroutine_ptr_t) /*_.VALROUT___V130*/ meltfptr[129])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V121*/ meltfptr[120]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V130*/ meltfptr[129]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_102__REGISTER_ALL_PASSES_START_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V130*/ meltfptr[129])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V130*/ meltfptr[129])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V129*/ meltfptr[128]);
  ((meltroutine_ptr_t) /*_.VALROUT___V130*/ meltfptr[129])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V129*/ meltfptr[128]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V130*/ meltfptr[129]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_102__REGISTER_ALL_PASSES_START_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V130*/ meltfptr[129])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V130*/ meltfptr[129]);

  /*putclosurout#14 */
  melt_assertmsg ("putclosrout#14 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V131*/ meltfptr[130])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#14 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V130*/ meltfptr[129])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V131*/ meltfptr[130])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V130*/ meltfptr[129]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V131*/ meltfptr[130]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V132*/ meltfptr[131])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V121*/ meltfptr[120]);
  ((meltroutine_ptr_t) /*_.VALROUT___V132*/ meltfptr[131])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V121*/ meltfptr[120]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V132*/ meltfptr[131]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_104__REGISTER_ALL_PASSES_START_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V132*/ meltfptr[131])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V132*/ meltfptr[131])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V129*/ meltfptr[128]);
  ((meltroutine_ptr_t) /*_.VALROUT___V132*/ meltfptr[131])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V129*/ meltfptr[128]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V132*/ meltfptr[131]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_104__REGISTER_ALL_PASSES_START_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V132*/ meltfptr[131])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V132*/ meltfptr[131]);

  /*putclosurout#15 */
  melt_assertmsg ("putclosrout#15 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V133*/ meltfptr[132])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#15 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V132*/ meltfptr[131])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V133*/ meltfptr[132])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V132*/ meltfptr[131]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V133*/ meltfptr[132]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V134*/ meltfptr[133])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V134*/
					meltfptr[133]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V134*/ meltfptr[133]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V135*/ meltfptr[134]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V134*/ meltfptr[133]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V134*/ meltfptr[133],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V136*/ meltfptr[135])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V136*/
					meltfptr[135]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V136*/ meltfptr[135]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V137*/ meltfptr[136]);

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V136*/ meltfptr[135])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V136*/
					meltfptr[135]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V136*/ meltfptr[135]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V136*/ meltfptr[135])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V136*/
					meltfptr[135]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V136*/ meltfptr[135]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V136*/ meltfptr[135]);

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V136*/ meltfptr[135],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V139*/ meltfptr[138])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V139*/
					meltfptr[138]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V139*/ meltfptr[138]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V140*/ meltfptr[139]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V141*/ meltfptr[140]);

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V139*/ meltfptr[138])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V139*/
					meltfptr[138]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V139*/ meltfptr[138]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V141*/ meltfptr[140]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V142*/ meltfptr[141]);

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V139*/ meltfptr[138])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V139*/
					meltfptr[138]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V139*/ meltfptr[138]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V142*/ meltfptr[141]);

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V139*/ meltfptr[138])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V139*/
					meltfptr[138]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V139*/ meltfptr[138]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V139*/ meltfptr[138]);

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V139*/ meltfptr[138],
				"new static instance");

  /*put data in hook HOOK_ALL_PASSES_END */
  melt_assertmsg ("puthookdata checkhook HOOK_ALL_PASSES_END",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V138*/ meltfptr[137])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V138*/ meltfptr[137])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V139*/ meltfptr[138]);

  /*put inside hook HOOK_ALL_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_END",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V138*/
				  meltfptr[137]));
  ((melthook_ptr_t) /*_.VALHOOK___V138*/ meltfptr[137])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V136*/ meltfptr[135]);

  /*put inside hook HOOK_ALL_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_END",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V138*/
				  meltfptr[137]));
  ((melthook_ptr_t) /*_.VALHOOK___V138*/ meltfptr[137])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_ALL_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_END",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V138*/
				  meltfptr[137]));
  ((melthook_ptr_t) /*_.VALHOOK___V138*/ meltfptr[137])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_ALL_PASSES_END */
  meltgc_touch ( /*_.VALHOOK___V138*/ meltfptr[137]);

  /*putclosurout#16 */
  melt_assertmsg ("putclosrout#16 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V144*/ meltfptr[143])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#16 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V143*/ meltfptr[142])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V144*/ meltfptr[143])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V143*/ meltfptr[142]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V144*/ meltfptr[143]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V145*/ meltfptr[144])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V136*/ meltfptr[135]);
  ((meltroutine_ptr_t) /*_.VALROUT___V145*/ meltfptr[144])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V136*/ meltfptr[135]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V145*/ meltfptr[144]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_116__REGISTER_ALL_PASSES_END_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V145*/ meltfptr[144])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V145*/ meltfptr[144])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V144*/ meltfptr[143]);
  ((meltroutine_ptr_t) /*_.VALROUT___V145*/ meltfptr[144])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V144*/ meltfptr[143]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V145*/ meltfptr[144]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_116__REGISTER_ALL_PASSES_END_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V145*/ meltfptr[144])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V145*/ meltfptr[144]);

  /*putclosurout#17 */
  melt_assertmsg ("putclosrout#17 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V146*/ meltfptr[145])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#17 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V145*/ meltfptr[144])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V146*/ meltfptr[145])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V145*/ meltfptr[144]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V146*/ meltfptr[145]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V147*/ meltfptr[146])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V136*/ meltfptr[135]);
  ((meltroutine_ptr_t) /*_.VALROUT___V147*/ meltfptr[146])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V136*/ meltfptr[135]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V147*/ meltfptr[146]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_118__REGISTER_ALL_PASSES_END_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V147*/ meltfptr[146])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V147*/ meltfptr[146])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V144*/ meltfptr[143]);
  ((meltroutine_ptr_t) /*_.VALROUT___V147*/ meltfptr[146])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V144*/ meltfptr[143]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V147*/ meltfptr[146]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_118__REGISTER_ALL_PASSES_END_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V147*/ meltfptr[146])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V147*/ meltfptr[146]);

  /*putclosurout#18 */
  melt_assertmsg ("putclosrout#18 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V148*/ meltfptr[147])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#18 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V147*/ meltfptr[146])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V148*/ meltfptr[147])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V147*/ meltfptr[146]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V148*/ meltfptr[147]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V149*/ meltfptr[148])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V149*/
					meltfptr[148]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V149*/ meltfptr[148]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V150*/ meltfptr[149]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V149*/ meltfptr[148]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V149*/ meltfptr[148],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V151*/ meltfptr[150])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V151*/
					meltfptr[150]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V151*/ meltfptr[150]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V152*/ meltfptr[151]);

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V151*/ meltfptr[150])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V151*/
					meltfptr[150]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V151*/ meltfptr[150]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V151*/ meltfptr[150])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V151*/
					meltfptr[150]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V151*/ meltfptr[150]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V151*/ meltfptr[150]);

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V151*/ meltfptr[150],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V154*/ meltfptr[153])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V154*/
					meltfptr[153]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V154*/ meltfptr[153]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V155*/ meltfptr[154]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V156*/ meltfptr[155]);

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V154*/ meltfptr[153])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V154*/
					meltfptr[153]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V154*/ meltfptr[153]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V156*/ meltfptr[155]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V157*/ meltfptr[156]);

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V154*/ meltfptr[153])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V154*/
					meltfptr[153]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V154*/ meltfptr[153]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V157*/ meltfptr[156]);

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V154*/ meltfptr[153])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V154*/
					meltfptr[153]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V154*/ meltfptr[153]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V154*/ meltfptr[153]);

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V154*/ meltfptr[153],
				"new static instance");

  /*put data in hook HOOK_ALL_IPA_PASSES_START */
  melt_assertmsg ("puthookdata checkhook HOOK_ALL_IPA_PASSES_START",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V153*/ meltfptr[152])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V153*/ meltfptr[152])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V154*/ meltfptr[153]);

  /*put inside hook HOOK_ALL_IPA_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_START",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V153*/
				  meltfptr[152]));
  ((melthook_ptr_t) /*_.VALHOOK___V153*/ meltfptr[152])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V151*/ meltfptr[150]);

  /*put inside hook HOOK_ALL_IPA_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_START",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V153*/
				  meltfptr[152]));
  ((melthook_ptr_t) /*_.VALHOOK___V153*/ meltfptr[152])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_ALL_IPA_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_START",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V153*/
				  meltfptr[152]));
  ((melthook_ptr_t) /*_.VALHOOK___V153*/ meltfptr[152])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_ALL_IPA_PASSES_START */
  meltgc_touch ( /*_.VALHOOK___V153*/ meltfptr[152]);

  /*putclosurout#19 */
  melt_assertmsg ("putclosrout#19 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V159*/ meltfptr[158])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#19 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V158*/ meltfptr[157])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V159*/ meltfptr[158])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V158*/ meltfptr[157]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V159*/ meltfptr[158]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V160*/ meltfptr[159])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V151*/ meltfptr[150]);
  ((meltroutine_ptr_t) /*_.VALROUT___V160*/ meltfptr[159])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V151*/ meltfptr[150]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V160*/ meltfptr[159]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V160*/ meltfptr[159])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V160*/ meltfptr[159])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V159*/ meltfptr[158]);
  ((meltroutine_ptr_t) /*_.VALROUT___V160*/ meltfptr[159])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V159*/ meltfptr[158]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V160*/ meltfptr[159]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V160*/ meltfptr[159])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V160*/ meltfptr[159]);

  /*putclosurout#20 */
  melt_assertmsg ("putclosrout#20 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V161*/ meltfptr[160])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#20 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V160*/ meltfptr[159])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V161*/ meltfptr[160])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V160*/ meltfptr[159]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V161*/ meltfptr[160]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V162*/ meltfptr[161])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V151*/ meltfptr[150]);
  ((meltroutine_ptr_t) /*_.VALROUT___V162*/ meltfptr[161])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V151*/ meltfptr[150]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V162*/ meltfptr[161]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_132__REGISTER_ALL_IPA_PASSES_START_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V162*/ meltfptr[161])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V162*/ meltfptr[161])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V159*/ meltfptr[158]);
  ((meltroutine_ptr_t) /*_.VALROUT___V162*/ meltfptr[161])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V159*/ meltfptr[158]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V162*/ meltfptr[161]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_132__REGISTER_ALL_IPA_PASSES_START_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V162*/ meltfptr[161])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V162*/ meltfptr[161]);

  /*putclosurout#21 */
  melt_assertmsg ("putclosrout#21 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V163*/ meltfptr[162])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#21 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V162*/ meltfptr[161])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V163*/ meltfptr[162])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V162*/ meltfptr[161]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V163*/ meltfptr[162]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V164*/ meltfptr[163])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V164*/
					meltfptr[163]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V164*/ meltfptr[163]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V165*/ meltfptr[164]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V164*/ meltfptr[163]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V164*/ meltfptr[163],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V166*/ meltfptr[165])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V166*/
					meltfptr[165]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V166*/ meltfptr[165]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V167*/ meltfptr[166]);

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V166*/ meltfptr[165])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V166*/
					meltfptr[165]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V166*/ meltfptr[165]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V166*/ meltfptr[165])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V166*/
					meltfptr[165]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V166*/ meltfptr[165]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V166*/ meltfptr[165]);

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V166*/ meltfptr[165],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V169*/ meltfptr[168])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V169*/
					meltfptr[168]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V169*/ meltfptr[168]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V170*/ meltfptr[169]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V171*/ meltfptr[170]);

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V169*/ meltfptr[168])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V169*/
					meltfptr[168]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V169*/ meltfptr[168]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V171*/ meltfptr[170]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V172*/ meltfptr[171]);

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V169*/ meltfptr[168])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V169*/
					meltfptr[168]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V169*/ meltfptr[168]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V172*/ meltfptr[171]);

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V169*/ meltfptr[168])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V169*/
					meltfptr[168]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V169*/ meltfptr[168]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V169*/ meltfptr[168]);

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V169*/ meltfptr[168],
				"new static instance");

  /*put data in hook HOOK_ALL_IPA_PASSES_END */
  melt_assertmsg ("puthookdata checkhook HOOK_ALL_IPA_PASSES_END",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V168*/ meltfptr[167])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V168*/ meltfptr[167])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V169*/ meltfptr[168]);

  /*put inside hook HOOK_ALL_IPA_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_END",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V168*/
				  meltfptr[167]));
  ((melthook_ptr_t) /*_.VALHOOK___V168*/ meltfptr[167])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V166*/ meltfptr[165]);

  /*put inside hook HOOK_ALL_IPA_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_END",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V168*/
				  meltfptr[167]));
  ((melthook_ptr_t) /*_.VALHOOK___V168*/ meltfptr[167])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_ALL_IPA_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_END",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V168*/
				  meltfptr[167]));
  ((melthook_ptr_t) /*_.VALHOOK___V168*/ meltfptr[167])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_ALL_IPA_PASSES_END */
  meltgc_touch ( /*_.VALHOOK___V168*/ meltfptr[167]);

  /*putclosurout#22 */
  melt_assertmsg ("putclosrout#22 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V174*/ meltfptr[173])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#22 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V173*/ meltfptr[172])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V174*/ meltfptr[173])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V173*/ meltfptr[172]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V174*/ meltfptr[173]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V175*/ meltfptr[174])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V166*/ meltfptr[165]);
  ((meltroutine_ptr_t) /*_.VALROUT___V175*/ meltfptr[174])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V166*/ meltfptr[165]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V175*/ meltfptr[174]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V175*/ meltfptr[174])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V175*/ meltfptr[174])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V174*/ meltfptr[173]);
  ((meltroutine_ptr_t) /*_.VALROUT___V175*/ meltfptr[174])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V174*/ meltfptr[173]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V175*/ meltfptr[174]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V175*/ meltfptr[174])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V175*/ meltfptr[174]);

  /*putclosurout#23 */
  melt_assertmsg ("putclosrout#23 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V176*/ meltfptr[175])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#23 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V175*/ meltfptr[174])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V176*/ meltfptr[175])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V175*/ meltfptr[174]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V176*/ meltfptr[175]);

  MELT_LOCATION ("warmelt-hooks.melt:917:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V177*/ meltfptr[176])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V166*/ meltfptr[165]);
  ((meltroutine_ptr_t) /*_.VALROUT___V177*/ meltfptr[176])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V166*/ meltfptr[165]);


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_2 */

void
meltmod__warmelt_hooks__initialmeltchunk_3 (meltinitial_frame_t *
					    meltmeltframptr__,
					    char meltpredefinited[])
{
#define meltfram__ (*meltmeltframptr__)
#undef meltcallcount
#define meltcallcount 0L
  (void) meltpredefinited;
  MELT_LOCATION ("warmelt-hooks.melt:917:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V177*/ meltfptr[176]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_146__REGISTER_ALL_IPA_PASSES_END_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V177*/ meltfptr[176])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:917:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V177*/ meltfptr[176])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V174*/ meltfptr[173]);
  ((meltroutine_ptr_t) /*_.VALROUT___V177*/ meltfptr[176])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V174*/ meltfptr[173]);

  MELT_LOCATION ("warmelt-hooks.melt:917:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V177*/ meltfptr[176]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_146__REGISTER_ALL_IPA_PASSES_END_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V177*/ meltfptr[176])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:917:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V177*/ meltfptr[176]);

  /*putclosurout#24 */
  melt_assertmsg ("putclosrout#24 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V178*/ meltfptr[177])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#24 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V177*/ meltfptr[176])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V178*/ meltfptr[177])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V177*/ meltfptr[176]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V178*/ meltfptr[177]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V179*/ meltfptr[178])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V179*/
					meltfptr[178]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V179*/ meltfptr[178]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V180*/ meltfptr[179]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V179*/ meltfptr[178]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V179*/ meltfptr[178],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:949:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V181*/ meltfptr[180])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V181*/
					meltfptr[180]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V181*/ meltfptr[180]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V182*/ meltfptr[181]);

  MELT_LOCATION ("warmelt-hooks.melt:949:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V181*/ meltfptr[180])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V181*/
					meltfptr[180]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V181*/ meltfptr[180]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:949:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V181*/ meltfptr[180])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V181*/
					meltfptr[180]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V181*/ meltfptr[180]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:949:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V181*/ meltfptr[180]);

  MELT_LOCATION ("warmelt-hooks.melt:949:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V181*/ meltfptr[180],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:963:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V184*/ meltfptr[183])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V184*/
					meltfptr[183]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V184*/ meltfptr[183]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V185*/ meltfptr[184]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V186*/ meltfptr[185]);

  MELT_LOCATION ("warmelt-hooks.melt:963:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V184*/ meltfptr[183])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V184*/
					meltfptr[183]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V184*/ meltfptr[183]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V186*/ meltfptr[185]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V187*/ meltfptr[186]);

  MELT_LOCATION ("warmelt-hooks.melt:963:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V184*/ meltfptr[183])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V184*/
					meltfptr[183]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V184*/ meltfptr[183]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V187*/ meltfptr[186]);

  MELT_LOCATION ("warmelt-hooks.melt:963:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V184*/ meltfptr[183])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V184*/
					meltfptr[183]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V184*/ meltfptr[183]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:963:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V184*/ meltfptr[183]);

  MELT_LOCATION ("warmelt-hooks.melt:963:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V184*/ meltfptr[183],
				"new static instance");

  /*put data in hook HOOK_EARLY_GIMPLE_PASSES_START */
  melt_assertmsg ("puthookdata checkhook HOOK_EARLY_GIMPLE_PASSES_START",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V183*/ meltfptr[182])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V183*/ meltfptr[182])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V184*/ meltfptr[183]);

  /*put inside hook HOOK_EARLY_GIMPLE_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_EARLY_GIMPLE_PASSES_START",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V183*/
				  meltfptr[182]));
  ((melthook_ptr_t) /*_.VALHOOK___V183*/ meltfptr[182])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V181*/ meltfptr[180]);

  /*put inside hook HOOK_EARLY_GIMPLE_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_EARLY_GIMPLE_PASSES_START",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V183*/
				  meltfptr[182]));
  ((melthook_ptr_t) /*_.VALHOOK___V183*/ meltfptr[182])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_EARLY_GIMPLE_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_EARLY_GIMPLE_PASSES_START",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V183*/
				  meltfptr[182]));
  ((melthook_ptr_t) /*_.VALHOOK___V183*/ meltfptr[182])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_EARLY_GIMPLE_PASSES_START */
  meltgc_touch ( /*_.VALHOOK___V183*/ meltfptr[182]);

  /*putclosurout#25 */
  melt_assertmsg ("putclosrout#25 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V189*/ meltfptr[188])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#25 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V188*/ meltfptr[187])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V189*/ meltfptr[188])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V188*/ meltfptr[187]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V189*/ meltfptr[188]);

  MELT_LOCATION ("warmelt-hooks.melt:1017:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V190*/ meltfptr[189])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V181*/ meltfptr[180]);
  ((meltroutine_ptr_t) /*_.VALROUT___V190*/ meltfptr[189])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V181*/ meltfptr[180]);

  MELT_LOCATION ("warmelt-hooks.melt:1017:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V190*/ meltfptr[189]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_158__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V190*/ meltfptr[189])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1017:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V190*/ meltfptr[189])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V189*/ meltfptr[188]);
  ((meltroutine_ptr_t) /*_.VALROUT___V190*/ meltfptr[189])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V189*/ meltfptr[188]);

  MELT_LOCATION ("warmelt-hooks.melt:1017:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V190*/ meltfptr[189]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_158__REGISTER_EARLY_GIMPLE_PASSES_START_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V190*/ meltfptr[189])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1017:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V190*/ meltfptr[189]);

  /*putclosurout#26 */
  melt_assertmsg ("putclosrout#26 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V191*/ meltfptr[190])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#26 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V190*/ meltfptr[189])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V191*/ meltfptr[190])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V190*/ meltfptr[189]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V191*/ meltfptr[190]);

  MELT_LOCATION ("warmelt-hooks.melt:1040:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V192*/ meltfptr[191])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V181*/ meltfptr[180]);
  ((meltroutine_ptr_t) /*_.VALROUT___V192*/ meltfptr[191])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V181*/ meltfptr[180]);

  MELT_LOCATION ("warmelt-hooks.melt:1040:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V192*/ meltfptr[191]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_160__REGISTER_EARLY_GIMPLE_PASSES_START_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V192*/ meltfptr[191])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1040:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V192*/ meltfptr[191])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V189*/ meltfptr[188]);
  ((meltroutine_ptr_t) /*_.VALROUT___V192*/ meltfptr[191])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V189*/ meltfptr[188]);

  MELT_LOCATION ("warmelt-hooks.melt:1040:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V192*/ meltfptr[191]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_160__REGISTER_EARLY_GIMPLE_PASSES_START_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V192*/ meltfptr[191])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1040:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V192*/ meltfptr[191]);

  /*putclosurout#27 */
  melt_assertmsg ("putclosrout#27 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V193*/ meltfptr[192])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#27 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V192*/ meltfptr[191])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V193*/ meltfptr[192])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V192*/ meltfptr[191]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V193*/ meltfptr[192]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V194*/ meltfptr[193])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V194*/
					meltfptr[193]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V194*/ meltfptr[193]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V195*/ meltfptr[194]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V194*/ meltfptr[193]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V194*/ meltfptr[193],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1080:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V196*/ meltfptr[195])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V196*/
					meltfptr[195]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V196*/ meltfptr[195]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V197*/ meltfptr[196]);

  MELT_LOCATION ("warmelt-hooks.melt:1080:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V196*/ meltfptr[195])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V196*/
					meltfptr[195]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V196*/ meltfptr[195]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:1080:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V196*/ meltfptr[195])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V196*/
					meltfptr[195]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V196*/ meltfptr[195]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:1080:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V196*/ meltfptr[195]);

  MELT_LOCATION ("warmelt-hooks.melt:1080:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V196*/ meltfptr[195],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1087:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V199*/ meltfptr[198])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V199*/
					meltfptr[198]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V199*/ meltfptr[198]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V200*/ meltfptr[199]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V201*/ meltfptr[200]);

  MELT_LOCATION ("warmelt-hooks.melt:1087:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V199*/ meltfptr[198])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V199*/
					meltfptr[198]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V199*/ meltfptr[198]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V201*/ meltfptr[200]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V202*/ meltfptr[201]);

  MELT_LOCATION ("warmelt-hooks.melt:1087:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V199*/ meltfptr[198])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V199*/
					meltfptr[198]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V199*/ meltfptr[198]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V202*/ meltfptr[201]);

  MELT_LOCATION ("warmelt-hooks.melt:1087:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V199*/ meltfptr[198])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V199*/
					meltfptr[198]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V199*/ meltfptr[198]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:1087:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V199*/ meltfptr[198]);

  MELT_LOCATION ("warmelt-hooks.melt:1087:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V199*/ meltfptr[198],
				"new static instance");

  /*put data in hook HOOK_EARLY_GIMPLE_PASSES_END */
  melt_assertmsg ("puthookdata checkhook HOOK_EARLY_GIMPLE_PASSES_END",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V198*/ meltfptr[197])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V198*/ meltfptr[197])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V199*/ meltfptr[198]);

  /*put inside hook HOOK_EARLY_GIMPLE_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_EARLY_GIMPLE_PASSES_END",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V198*/
				  meltfptr[197]));
  ((melthook_ptr_t) /*_.VALHOOK___V198*/ meltfptr[197])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V196*/ meltfptr[195]);

  /*put inside hook HOOK_EARLY_GIMPLE_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_EARLY_GIMPLE_PASSES_END",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V198*/
				  meltfptr[197]));
  ((melthook_ptr_t) /*_.VALHOOK___V198*/ meltfptr[197])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_EARLY_GIMPLE_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_EARLY_GIMPLE_PASSES_END",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V198*/
				  meltfptr[197]));
  ((melthook_ptr_t) /*_.VALHOOK___V198*/ meltfptr[197])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_EARLY_GIMPLE_PASSES_END */
  meltgc_touch ( /*_.VALHOOK___V198*/ meltfptr[197]);

  /*putclosurout#28 */
  melt_assertmsg ("putclosrout#28 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V204*/ meltfptr[203])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#28 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V203*/ meltfptr[202])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V204*/ meltfptr[203])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V203*/ meltfptr[202]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V204*/ meltfptr[203]);

  MELT_LOCATION ("warmelt-hooks.melt:1140:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V205*/ meltfptr[204])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V196*/ meltfptr[195]);
  ((meltroutine_ptr_t) /*_.VALROUT___V205*/ meltfptr[204])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V196*/ meltfptr[195]);

  MELT_LOCATION ("warmelt-hooks.melt:1140:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V205*/ meltfptr[204]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_172__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V205*/ meltfptr[204])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1140:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V205*/ meltfptr[204])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V204*/ meltfptr[203]);
  ((meltroutine_ptr_t) /*_.VALROUT___V205*/ meltfptr[204])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V204*/ meltfptr[203]);

  MELT_LOCATION ("warmelt-hooks.melt:1140:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V205*/ meltfptr[204]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_172__REGISTER_EARLY_GIMPLE_PASSES_END_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V205*/ meltfptr[204])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1140:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V205*/ meltfptr[204]);

  /*putclosurout#29 */
  melt_assertmsg ("putclosrout#29 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V206*/ meltfptr[205])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#29 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V205*/ meltfptr[204])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V206*/ meltfptr[205])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V205*/ meltfptr[204]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V206*/ meltfptr[205]);

  MELT_LOCATION ("warmelt-hooks.melt:1163:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V207*/ meltfptr[206])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V196*/ meltfptr[195]);
  ((meltroutine_ptr_t) /*_.VALROUT___V207*/ meltfptr[206])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V196*/ meltfptr[195]);

  MELT_LOCATION ("warmelt-hooks.melt:1163:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V207*/ meltfptr[206]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_174__REGISTER_EARLY_GIMPLE_PASSES_END_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V207*/ meltfptr[206])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1163:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V207*/ meltfptr[206])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V204*/ meltfptr[203]);
  ((meltroutine_ptr_t) /*_.VALROUT___V207*/ meltfptr[206])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V204*/ meltfptr[203]);

  MELT_LOCATION ("warmelt-hooks.melt:1163:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V207*/ meltfptr[206]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_174__REGISTER_EARLY_GIMPLE_PASSES_END_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V207*/ meltfptr[206])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1163:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V207*/ meltfptr[206]);

  /*putclosurout#30 */
  melt_assertmsg ("putclosrout#30 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V208*/ meltfptr[207])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#30 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V207*/ meltfptr[206])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V208*/ meltfptr[207])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V207*/ meltfptr[206]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V208*/ meltfptr[207]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V209*/ meltfptr[208])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V209*/
					meltfptr[208]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V209*/ meltfptr[208]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V210*/ meltfptr[209]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V209*/ meltfptr[208]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V209*/ meltfptr[208],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1193:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V211*/ meltfptr[210])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V211*/
					meltfptr[210]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V211*/ meltfptr[210]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V212*/ meltfptr[211]);

  MELT_LOCATION ("warmelt-hooks.melt:1193:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V211*/ meltfptr[210])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V211*/
					meltfptr[210]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V211*/ meltfptr[210]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:1193:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V211*/ meltfptr[210])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V211*/
					meltfptr[210]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V211*/ meltfptr[210]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:1193:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V211*/ meltfptr[210]);

  MELT_LOCATION ("warmelt-hooks.melt:1193:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V211*/ meltfptr[210],
				"new static instance");

  /*putclosurout#31 */
  melt_assertmsg ("putclosrout#31 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V214*/ meltfptr[213])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#31 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V213*/ meltfptr[212])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V214*/ meltfptr[213])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V213*/ meltfptr[212]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V214*/ meltfptr[213]);

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V216*/ meltfptr[215])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V216*/
					meltfptr[215]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V216*/ meltfptr[215]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V217*/ meltfptr[216]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V219*/ meltfptr[218])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V219*/
					meltfptr[218]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V219*/ meltfptr[218]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V220*/ meltfptr[219]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V219*/ meltfptr[218]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V219*/ meltfptr[218],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V218*/ meltfptr[217])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V218*/
					meltfptr[217]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V218*/ meltfptr[217]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V219*/ meltfptr[218]);

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V218*/ meltfptr[217])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V218*/
					meltfptr[217]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V218*/ meltfptr[217]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_TREE))));

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V218*/ meltfptr[217]);

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V218*/ meltfptr[217],
				"new static instance");

  /*putupl#8 */
  melt_assertmsg ("putupl #8 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V221*/ meltfptr[220])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #8 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V221*/
					  meltfptr[220]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V221*/ meltfptr[220]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V218*/ meltfptr[217]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V221*/ meltfptr[220]);

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V216*/ meltfptr[215])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V216*/
					meltfptr[215]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V216*/ meltfptr[215]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V221*/ meltfptr[220]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V222*/ meltfptr[221]);

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V216*/ meltfptr[215])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V216*/
					meltfptr[215]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V216*/ meltfptr[215]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V222*/ meltfptr[221]);

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V216*/ meltfptr[215])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V216*/
					meltfptr[215]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V216*/ meltfptr[215]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V216*/ meltfptr[215]);

  MELT_LOCATION ("warmelt-hooks.melt:1237:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V216*/ meltfptr[215],
				"new static instance");

  /*put data in hook HOOK_PRE_GENERICIZE */
  melt_assertmsg ("puthookdata checkhook HOOK_PRE_GENERICIZE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V215*/ meltfptr[214])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V215*/ meltfptr[214])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V216*/ meltfptr[215]);

  /*put inside hook HOOK_PRE_GENERICIZE */
  melt_assertmsg ("puthookconst checkhook HOOK_PRE_GENERICIZE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V215*/
				  meltfptr[214]));
  ((melthook_ptr_t) /*_.VALHOOK___V215*/ meltfptr[214])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V211*/ meltfptr[210]);

  /*put inside hook HOOK_PRE_GENERICIZE */
  melt_assertmsg ("puthookconst checkhook HOOK_PRE_GENERICIZE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V215*/
				  meltfptr[214]));
  ((melthook_ptr_t) /*_.VALHOOK___V215*/ meltfptr[214])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_PRE_GENERICIZE */
  melt_assertmsg ("puthookconst checkhook HOOK_PRE_GENERICIZE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V215*/
				  meltfptr[214]));
  ((melthook_ptr_t) /*_.VALHOOK___V215*/ meltfptr[214])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_PRE_GENERICIZE */
  meltgc_touch ( /*_.VALHOOK___V215*/ meltfptr[214]);

  MELT_LOCATION ("warmelt-hooks.melt:1268:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V223*/ meltfptr[222])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V211*/ meltfptr[210]);
  ((meltroutine_ptr_t) /*_.VALROUT___V223*/ meltfptr[222])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V211*/ meltfptr[210]);

  MELT_LOCATION ("warmelt-hooks.melt:1268:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V223*/ meltfptr[222]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_189__REGISTER_PRE_GENERICIZE_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V223*/ meltfptr[222])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1268:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V223*/ meltfptr[222]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_189__REGISTER_PRE_GENERICIZE_FIRST#2",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V223*/ meltfptr[222])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1268:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V223*/ meltfptr[222])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V214*/ meltfptr[213]);
  ((meltroutine_ptr_t) /*_.VALROUT___V223*/ meltfptr[222])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V214*/ meltfptr[213]);

  MELT_LOCATION ("warmelt-hooks.melt:1268:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V223*/ meltfptr[222]);

  /*putclosurout#32 */
  melt_assertmsg ("putclosrout#32 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V224*/ meltfptr[223])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#32 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V223*/ meltfptr[222])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V224*/ meltfptr[223])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V223*/ meltfptr[222]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V224*/ meltfptr[223]);

  MELT_LOCATION ("warmelt-hooks.melt:1291:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V225*/ meltfptr[224])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V211*/ meltfptr[210]);
  ((meltroutine_ptr_t) /*_.VALROUT___V225*/ meltfptr[224])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V211*/ meltfptr[210]);

  MELT_LOCATION ("warmelt-hooks.melt:1291:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V225*/ meltfptr[224]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_191__REGISTER_PRE_GENERICIZE_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V225*/ meltfptr[224])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1291:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V225*/ meltfptr[224]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_191__REGISTER_PRE_GENERICIZE_LAST#2",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V225*/ meltfptr[224])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1291:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V225*/ meltfptr[224])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V214*/ meltfptr[213]);
  ((meltroutine_ptr_t) /*_.VALROUT___V225*/ meltfptr[224])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V214*/ meltfptr[213]);

  MELT_LOCATION ("warmelt-hooks.melt:1291:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V225*/ meltfptr[224]);

  /*putclosurout#33 */
  melt_assertmsg ("putclosrout#33 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V226*/ meltfptr[225])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#33 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V225*/ meltfptr[224])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V226*/ meltfptr[225])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V225*/ meltfptr[224]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V226*/ meltfptr[225]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V227*/ meltfptr[226])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V227*/
					meltfptr[226]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V227*/ meltfptr[226]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V228*/ meltfptr[227]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V227*/ meltfptr[226]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V227*/ meltfptr[226],
				"new static instance");


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_3 */

void
meltmod__warmelt_hooks__initialmeltchunk_4 (meltinitial_frame_t *
					    meltmeltframptr__,
					    char meltpredefinited[])
{
#define meltfram__ (*meltmeltframptr__)
#undef meltcallcount
#define meltcallcount 0L
  (void) meltpredefinited;
  MELT_LOCATION ("warmelt-hooks.melt:1324:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V229*/ meltfptr[228])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V229*/
					meltfptr[228]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V229*/ meltfptr[228]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V230*/ meltfptr[229]);

  MELT_LOCATION ("warmelt-hooks.melt:1324:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V229*/ meltfptr[228])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V229*/
					meltfptr[228]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V229*/ meltfptr[228]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:1324:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V229*/ meltfptr[228])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V229*/
					meltfptr[228]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V229*/ meltfptr[228]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:1324:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V229*/ meltfptr[228]);

  MELT_LOCATION ("warmelt-hooks.melt:1324:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V229*/ meltfptr[228],
				"new static instance");

  /*putclosurout#34 */
  melt_assertmsg ("putclosrout#34 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V232*/ meltfptr[231])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#34 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V231*/ meltfptr[230])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V232*/ meltfptr[231])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V231*/ meltfptr[230]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V232*/ meltfptr[231]);

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V234*/ meltfptr[233])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V234*/
					meltfptr[233]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V234*/ meltfptr[233]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V235*/ meltfptr[234]);

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V236*/ meltfptr[235])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V236*/
					meltfptr[235]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V236*/ meltfptr[235]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V219*/ meltfptr[218]);

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V236*/ meltfptr[235])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V236*/
					meltfptr[235]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V236*/ meltfptr[235]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_TREE))));

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V236*/ meltfptr[235]);

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V236*/ meltfptr[235],
				"new static instance");

  /*putupl#9 */
  melt_assertmsg ("putupl #9 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V237*/ meltfptr[236])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #9 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V237*/
					  meltfptr[236]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V237*/ meltfptr[236]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V236*/ meltfptr[235]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V237*/ meltfptr[236]);

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V234*/ meltfptr[233])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V234*/
					meltfptr[233]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V234*/ meltfptr[233]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V237*/ meltfptr[236]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V238*/ meltfptr[237]);

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V234*/ meltfptr[233])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V234*/
					meltfptr[233]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V234*/ meltfptr[233]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V238*/ meltfptr[237]);

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V234*/ meltfptr[233])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V234*/
					meltfptr[233]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V234*/ meltfptr[233]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V234*/ meltfptr[233]);

  MELT_LOCATION ("warmelt-hooks.melt:1368:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V234*/ meltfptr[233],
				"new static instance");

  /*put data in hook HOOK_FINISH_TYPE */
  melt_assertmsg ("puthookdata checkhook HOOK_FINISH_TYPE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V233*/ meltfptr[232])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V233*/ meltfptr[232])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V234*/ meltfptr[233]);

  /*put inside hook HOOK_FINISH_TYPE */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_TYPE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V233*/
				  meltfptr[232]));
  ((melthook_ptr_t) /*_.VALHOOK___V233*/ meltfptr[232])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V229*/ meltfptr[228]);

  /*put inside hook HOOK_FINISH_TYPE */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_TYPE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V233*/
				  meltfptr[232]));
  ((melthook_ptr_t) /*_.VALHOOK___V233*/ meltfptr[232])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_FINISH_TYPE */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_TYPE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V233*/
				  meltfptr[232]));
  ((melthook_ptr_t) /*_.VALHOOK___V233*/ meltfptr[232])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_FINISH_TYPE */
  meltgc_touch ( /*_.VALHOOK___V233*/ meltfptr[232]);

  MELT_LOCATION ("warmelt-hooks.melt:1399:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V239*/ meltfptr[238])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V229*/ meltfptr[228]);
  ((meltroutine_ptr_t) /*_.VALROUT___V239*/ meltfptr[238])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V229*/ meltfptr[228]);

  MELT_LOCATION ("warmelt-hooks.melt:1399:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V239*/ meltfptr[238]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_204__REGISTER_FINISH_TYPE_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V239*/ meltfptr[238])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1399:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V239*/ meltfptr[238]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_204__REGISTER_FINISH_TYPE_FIRST#2",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V239*/ meltfptr[238])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1399:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V239*/ meltfptr[238])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V232*/ meltfptr[231]);
  ((meltroutine_ptr_t) /*_.VALROUT___V239*/ meltfptr[238])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V232*/ meltfptr[231]);

  MELT_LOCATION ("warmelt-hooks.melt:1399:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V239*/ meltfptr[238]);

  /*putclosurout#35 */
  melt_assertmsg ("putclosrout#35 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V240*/ meltfptr[239])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#35 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V239*/ meltfptr[238])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V240*/ meltfptr[239])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V239*/ meltfptr[238]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V240*/ meltfptr[239]);

  MELT_LOCATION ("warmelt-hooks.melt:1422:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V241*/ meltfptr[240])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V229*/ meltfptr[228]);
  ((meltroutine_ptr_t) /*_.VALROUT___V241*/ meltfptr[240])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V229*/ meltfptr[228]);

  MELT_LOCATION ("warmelt-hooks.melt:1422:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V241*/ meltfptr[240]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_206__REGISTER_FINISH_TYPE_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V241*/ meltfptr[240])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1422:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V241*/ meltfptr[240]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_206__REGISTER_FINISH_TYPE_LAST#2",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V241*/ meltfptr[240])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1422:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V241*/ meltfptr[240])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V232*/ meltfptr[231]);
  ((meltroutine_ptr_t) /*_.VALROUT___V241*/ meltfptr[240])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V232*/ meltfptr[231]);

  MELT_LOCATION ("warmelt-hooks.melt:1422:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V241*/ meltfptr[240]);

  /*putclosurout#36 */
  melt_assertmsg ("putclosrout#36 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V242*/ meltfptr[241])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#36 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V241*/ meltfptr[240])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V242*/ meltfptr[241])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V241*/ meltfptr[240]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V242*/ meltfptr[241]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V243*/ meltfptr[242])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V243*/
					meltfptr[242]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V243*/ meltfptr[242]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V244*/ meltfptr[243]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V243*/ meltfptr[242]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V243*/ meltfptr[242],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1455:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V245*/ meltfptr[244])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V245*/
					meltfptr[244]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V245*/ meltfptr[244]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V246*/ meltfptr[245]);

  MELT_LOCATION ("warmelt-hooks.melt:1455:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V245*/ meltfptr[244])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V245*/
					meltfptr[244]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V245*/ meltfptr[244]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:1455:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V245*/ meltfptr[244])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V245*/
					meltfptr[244]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V245*/ meltfptr[244]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:1455:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V245*/ meltfptr[244]);

  MELT_LOCATION ("warmelt-hooks.melt:1455:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V245*/ meltfptr[244],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V248*/ meltfptr[247])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V248*/
					meltfptr[247]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V248*/ meltfptr[247]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V249*/ meltfptr[248]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V251*/ meltfptr[250])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V251*/
					meltfptr[250]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V251*/ meltfptr[250]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V252*/ meltfptr[251]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V251*/ meltfptr[250]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V251*/ meltfptr[250],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V250*/ meltfptr[249])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V250*/
					meltfptr[249]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V250*/ meltfptr[249]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V251*/ meltfptr[250]);

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V250*/ meltfptr[249])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V250*/
					meltfptr[249]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V250*/ meltfptr[249]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_CSTRING))));

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V250*/ meltfptr[249]);

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V250*/ meltfptr[249],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V254*/ meltfptr[253])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V254*/
					meltfptr[253]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V254*/ meltfptr[253]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V255*/ meltfptr[254]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V254*/ meltfptr[253]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V254*/ meltfptr[253],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V253*/ meltfptr[252])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V253*/
					meltfptr[252]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V253*/ meltfptr[252]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V254*/ meltfptr[253]);

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V253*/ meltfptr[252])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V253*/
					meltfptr[252]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V253*/ meltfptr[252]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V253*/ meltfptr[252]);

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V253*/ meltfptr[252],
				"new static instance");

  /*putupl#10 */
  melt_assertmsg ("putupl #10 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V256*/ meltfptr[255])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #10 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V256*/
					  meltfptr[255]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V256*/ meltfptr[255]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V250*/ meltfptr[249]);

  /*putupl#11 */
  melt_assertmsg ("putupl #11 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V256*/ meltfptr[255])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #11 checkoff",
		  (1 >= 0
		   && 1 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V256*/
					  meltfptr[255]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V256*/ meltfptr[255]))->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V253*/ meltfptr[252]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V256*/ meltfptr[255]);

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V248*/ meltfptr[247])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V248*/
					meltfptr[247]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V248*/ meltfptr[247]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V256*/ meltfptr[255]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V257*/ meltfptr[256]);

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V248*/ meltfptr[247])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V248*/
					meltfptr[247]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V248*/ meltfptr[247]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V257*/ meltfptr[256]);

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V248*/ meltfptr[247])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V248*/
					meltfptr[247]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V248*/ meltfptr[247]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V248*/ meltfptr[247]);

  MELT_LOCATION ("warmelt-hooks.melt:1494:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V248*/ meltfptr[247],
				"new static instance");

  /*put data in hook HOOK_PASS_EXECUTION */
  melt_assertmsg ("puthookdata checkhook HOOK_PASS_EXECUTION",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V247*/ meltfptr[246])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V247*/ meltfptr[246])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V248*/ meltfptr[247]);

  /*put inside hook HOOK_PASS_EXECUTION */
  melt_assertmsg ("puthookconst checkhook HOOK_PASS_EXECUTION",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V247*/
				  meltfptr[246]));
  ((melthook_ptr_t) /*_.VALHOOK___V247*/ meltfptr[246])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V245*/ meltfptr[244]);

  /*put inside hook HOOK_PASS_EXECUTION */
  melt_assertmsg ("puthookconst checkhook HOOK_PASS_EXECUTION",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V247*/
				  meltfptr[246]));
  ((melthook_ptr_t) /*_.VALHOOK___V247*/ meltfptr[246])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_PASS_EXECUTION */
  melt_assertmsg ("puthookconst checkhook HOOK_PASS_EXECUTION",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V247*/
				  meltfptr[246]));
  ((melthook_ptr_t) /*_.VALHOOK___V247*/ meltfptr[246])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_PASS_EXECUTION */
  meltgc_touch ( /*_.VALHOOK___V247*/ meltfptr[246]);

  MELT_LOCATION ("warmelt-hooks.melt:1534:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V258*/ meltfptr[257])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V245*/ meltfptr[244]);
  ((meltroutine_ptr_t) /*_.VALROUT___V258*/ meltfptr[257])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V245*/ meltfptr[244]);

  MELT_LOCATION ("warmelt-hooks.melt:1534:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V258*/ meltfptr[257]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_222__REGISTER_PASS_EXECUTION_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V258*/ meltfptr[257])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1534:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V258*/ meltfptr[257]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_222__REGISTER_PASS_EXECUTION_FIRST#2",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V258*/ meltfptr[257])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1534:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V258*/ meltfptr[257]);

  /*putclosurout#37 */
  melt_assertmsg ("putclosrout#37 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V259*/ meltfptr[258])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#37 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V258*/ meltfptr[257])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V259*/ meltfptr[258])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V258*/ meltfptr[257]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V259*/ meltfptr[258]);

  MELT_LOCATION ("warmelt-hooks.melt:1555:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V260*/ meltfptr[259])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V245*/ meltfptr[244]);
  ((meltroutine_ptr_t) /*_.VALROUT___V260*/ meltfptr[259])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V245*/ meltfptr[244]);

  MELT_LOCATION ("warmelt-hooks.melt:1555:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V260*/ meltfptr[259]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_224__REGISTER_PASS_EXECUTION_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V260*/ meltfptr[259])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:1555:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V260*/ meltfptr[259]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_224__REGISTER_PASS_EXECUTION_LAST#2",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V260*/ meltfptr[259])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:1555:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V260*/ meltfptr[259]);

  /*putclosurout#38 */
  melt_assertmsg ("putclosrout#38 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V261*/ meltfptr[260])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#38 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V260*/ meltfptr[259])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V261*/ meltfptr[260])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V260*/ meltfptr[259]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V261*/ meltfptr[260]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V263*/ meltfptr[262])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V263*/
					meltfptr[262]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V263*/ meltfptr[262]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V264*/ meltfptr[263]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V263*/ meltfptr[262]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V263*/ meltfptr[262],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V265*/ meltfptr[264])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V265*/
					meltfptr[264]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V265*/ meltfptr[264]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V266*/ meltfptr[265]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V265*/ meltfptr[264]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V265*/ meltfptr[264],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V267*/ meltfptr[266])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V267*/
					meltfptr[266]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V267*/ meltfptr[266]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V268*/ meltfptr[267]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V267*/ meltfptr[266]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V267*/ meltfptr[266],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V269*/ meltfptr[268])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V269*/
					meltfptr[268]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V269*/ meltfptr[268]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V270*/ meltfptr[269]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V269*/ meltfptr[268]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V269*/ meltfptr[268],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1579:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V271*/ meltfptr[270])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V271*/
					meltfptr[270]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V271*/ meltfptr[270]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V272*/ meltfptr[271]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V273*/ meltfptr[272]);

  MELT_LOCATION ("warmelt-hooks.melt:1579:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V271*/ meltfptr[270])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V271*/
					meltfptr[270]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V271*/ meltfptr[270]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V273*/ meltfptr[272]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V274*/ meltfptr[273]);

  MELT_LOCATION ("warmelt-hooks.melt:1579:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V271*/ meltfptr[270])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V271*/
					meltfptr[270]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V271*/ meltfptr[270]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V274*/ meltfptr[273]);

  MELT_LOCATION ("warmelt-hooks.melt:1579:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V271*/ meltfptr[270])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V271*/
					meltfptr[270]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V271*/ meltfptr[270]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:1579:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V271*/ meltfptr[270]);

  MELT_LOCATION ("warmelt-hooks.melt:1579:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V271*/ meltfptr[270],
				"new static instance");

  /*put data in hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookdata checkhook HOOK_GIMPLE_GATE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V262*/ meltfptr[261])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V271*/ meltfptr[270]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[0] =
    (melt_ptr_t) ( /*_.INITIAL_SYSTEM_DATA__V12*/ meltfptr[11]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_SYSTEM_DATA__V13*/ meltfptr[12]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_GCC_GIMPLE_PASS__V14*/ meltfptr[13]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[3] =
    (melt_ptr_t) ( /*_.CLASS_GCC_PASS__V15*/ meltfptr[14]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[4] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[5] =
    (melt_ptr_t) ( /*_.VALDATA___V263*/ meltfptr[262]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[6] =
    (melt_ptr_t) ( /*_.VALSTR___V264*/ meltfptr[263]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (7) >= 0
		  && (7) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[7] =
    (melt_ptr_t) ( /*_.VALDATA___V265*/ meltfptr[264]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (8) >= 0
		  && (8) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[8] =
    (melt_ptr_t) ( /*_.VALSTR___V266*/ meltfptr[265]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (9) >= 0
		  && (9) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[9] =
    (melt_ptr_t) ( /*_.VALDATA___V267*/ meltfptr[266]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (10) >= 0
		  && (10) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[10] =
    (melt_ptr_t) ( /*_.VALSTR___V268*/ meltfptr[267]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (11) >= 0
		  && (11) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[11] =
    (melt_ptr_t) ( /*_.VALDATA___V269*/ meltfptr[268]);

  /*put inside hook HOOK_GIMPLE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_GATE",
		  (12) >= 0
		  && (12) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V262*/
				  meltfptr[261]));
  ((melthook_ptr_t) /*_.VALHOOK___V262*/ meltfptr[261])->tabval[12] =
    (melt_ptr_t) ( /*_.VALSTR___V270*/ meltfptr[269]);

  /*touch:HOOK_GIMPLE_GATE */
  meltgc_touch ( /*_.VALHOOK___V262*/ meltfptr[261]);

  MELT_LOCATION ("warmelt-hooks.melt:1632:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V276*/ meltfptr[275])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V276*/
					meltfptr[275]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V276*/ meltfptr[275]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V277*/ meltfptr[276]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V278*/ meltfptr[277]);

  MELT_LOCATION ("warmelt-hooks.melt:1632:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V276*/ meltfptr[275])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V276*/
					meltfptr[275]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V276*/ meltfptr[275]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V278*/ meltfptr[277]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V279*/ meltfptr[278]);

  MELT_LOCATION ("warmelt-hooks.melt:1632:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V276*/ meltfptr[275])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V276*/
					meltfptr[275]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V276*/ meltfptr[275]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V279*/ meltfptr[278]);


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_4 */

void
meltmod__warmelt_hooks__initialmeltchunk_5 (meltinitial_frame_t *
					    meltmeltframptr__,
					    char meltpredefinited[])
{
#define meltfram__ (*meltmeltframptr__)
#undef meltcallcount
#define meltcallcount 0L
  (void) meltpredefinited;
  MELT_LOCATION ("warmelt-hooks.melt:1632:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V276*/ meltfptr[275])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V276*/
					meltfptr[275]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V276*/ meltfptr[275]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:1632:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V276*/ meltfptr[275]);

  MELT_LOCATION ("warmelt-hooks.melt:1632:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V276*/ meltfptr[275],
				"new static instance");

  /*put data in hook HOOK_GIMPLE_EXECUTE */
  melt_assertmsg ("puthookdata checkhook HOOK_GIMPLE_EXECUTE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V275*/ meltfptr[274])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V275*/ meltfptr[274])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V276*/ meltfptr[275]);

  /*put inside hook HOOK_GIMPLE_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_EXECUTE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V275*/
				  meltfptr[274]));
  ((melthook_ptr_t) /*_.VALHOOK___V275*/ meltfptr[274])->tabval[0] =
    (melt_ptr_t) ( /*_.INITIAL_SYSTEM_DATA__V12*/ meltfptr[11]);

  /*put inside hook HOOK_GIMPLE_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_EXECUTE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V275*/
				  meltfptr[274]));
  ((melthook_ptr_t) /*_.VALHOOK___V275*/ meltfptr[274])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_SYSTEM_DATA__V13*/ meltfptr[12]);

  /*put inside hook HOOK_GIMPLE_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_EXECUTE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V275*/
				  meltfptr[274]));
  ((melthook_ptr_t) /*_.VALHOOK___V275*/ meltfptr[274])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_GCC_GIMPLE_PASS__V14*/ meltfptr[13]);

  /*put inside hook HOOK_GIMPLE_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_EXECUTE",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V275*/
				  meltfptr[274]));
  ((melthook_ptr_t) /*_.VALHOOK___V275*/ meltfptr[274])->tabval[3] =
    (melt_ptr_t) ( /*_.CLASS_GCC_PASS__V15*/ meltfptr[14]);

  /*put inside hook HOOK_GIMPLE_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_GIMPLE_EXECUTE",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V275*/
				  meltfptr[274]));
  ((melthook_ptr_t) /*_.VALHOOK___V275*/ meltfptr[274])->tabval[4] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*touch:HOOK_GIMPLE_EXECUTE */
  meltgc_touch ( /*_.VALHOOK___V275*/ meltfptr[274]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V281*/ meltfptr[280])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V281*/
					meltfptr[280]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V281*/ meltfptr[280]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V282*/ meltfptr[281]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V281*/ meltfptr[280]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V281*/ meltfptr[280],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1695:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V283*/ meltfptr[282])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V283*/
					meltfptr[282]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V283*/ meltfptr[282]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V284*/ meltfptr[283]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V285*/ meltfptr[284]);

  MELT_LOCATION ("warmelt-hooks.melt:1695:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V283*/ meltfptr[282])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V283*/
					meltfptr[282]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V283*/ meltfptr[282]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V285*/ meltfptr[284]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V286*/ meltfptr[285]);

  MELT_LOCATION ("warmelt-hooks.melt:1695:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V283*/ meltfptr[282])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V283*/
					meltfptr[282]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V283*/ meltfptr[282]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V286*/ meltfptr[285]);

  MELT_LOCATION ("warmelt-hooks.melt:1695:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V283*/ meltfptr[282])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V283*/
					meltfptr[282]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V283*/ meltfptr[282]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:1695:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V283*/ meltfptr[282]);

  MELT_LOCATION ("warmelt-hooks.melt:1695:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V283*/ meltfptr[282],
				"new static instance");

  /*put data in hook HOOK_RTL_GATE */
  melt_assertmsg ("puthookdata checkhook HOOK_RTL_GATE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V280*/ meltfptr[279])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V280*/ meltfptr[279])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V283*/ meltfptr[282]);

  /*put inside hook HOOK_RTL_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_GATE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V280*/
				  meltfptr[279]));
  ((melthook_ptr_t) /*_.VALHOOK___V280*/ meltfptr[279])->tabval[0] =
    (melt_ptr_t) ( /*_.INITIAL_SYSTEM_DATA__V12*/ meltfptr[11]);

  /*put inside hook HOOK_RTL_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_GATE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V280*/
				  meltfptr[279]));
  ((melthook_ptr_t) /*_.VALHOOK___V280*/ meltfptr[279])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_SYSTEM_DATA__V13*/ meltfptr[12]);

  /*put inside hook HOOK_RTL_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_GATE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V280*/
				  meltfptr[279]));
  ((melthook_ptr_t) /*_.VALHOOK___V280*/ meltfptr[279])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_GCC_RTL_PASS__V16*/ meltfptr[15]);

  /*put inside hook HOOK_RTL_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_GATE",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V280*/
				  meltfptr[279]));
  ((melthook_ptr_t) /*_.VALHOOK___V280*/ meltfptr[279])->tabval[3] =
    (melt_ptr_t) ( /*_.CLASS_GCC_PASS__V15*/ meltfptr[14]);

  /*put inside hook HOOK_RTL_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_GATE",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V280*/
				  meltfptr[279]));
  ((melthook_ptr_t) /*_.VALHOOK___V280*/ meltfptr[279])->tabval[4] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*put inside hook HOOK_RTL_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_GATE",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V280*/
				  meltfptr[279]));
  ((melthook_ptr_t) /*_.VALHOOK___V280*/ meltfptr[279])->tabval[5] =
    (melt_ptr_t) ( /*_.VALDATA___V281*/ meltfptr[280]);

  /*put inside hook HOOK_RTL_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_GATE",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V280*/
				  meltfptr[279]));
  ((melthook_ptr_t) /*_.VALHOOK___V280*/ meltfptr[279])->tabval[6] =
    (melt_ptr_t) ( /*_.VALSTR___V282*/ meltfptr[281]);

  /*touch:HOOK_RTL_GATE */
  meltgc_touch ( /*_.VALHOOK___V280*/ meltfptr[279]);

  MELT_LOCATION ("warmelt-hooks.melt:1750:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V288*/ meltfptr[287])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V288*/
					meltfptr[287]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V288*/ meltfptr[287]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V289*/ meltfptr[288]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V290*/ meltfptr[289]);

  MELT_LOCATION ("warmelt-hooks.melt:1750:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V288*/ meltfptr[287])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V288*/
					meltfptr[287]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V288*/ meltfptr[287]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V290*/ meltfptr[289]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V291*/ meltfptr[290]);

  MELT_LOCATION ("warmelt-hooks.melt:1750:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V288*/ meltfptr[287])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V288*/
					meltfptr[287]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V288*/ meltfptr[287]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V291*/ meltfptr[290]);

  MELT_LOCATION ("warmelt-hooks.melt:1750:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V288*/ meltfptr[287])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V288*/
					meltfptr[287]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V288*/ meltfptr[287]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:1750:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V288*/ meltfptr[287]);

  MELT_LOCATION ("warmelt-hooks.melt:1750:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V288*/ meltfptr[287],
				"new static instance");

  /*put data in hook HOOK_RTL_EXECUTE */
  melt_assertmsg ("puthookdata checkhook HOOK_RTL_EXECUTE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V287*/ meltfptr[286])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V287*/ meltfptr[286])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V288*/ meltfptr[287]);

  /*put inside hook HOOK_RTL_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_EXECUTE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V287*/
				  meltfptr[286]));
  ((melthook_ptr_t) /*_.VALHOOK___V287*/ meltfptr[286])->tabval[0] =
    (melt_ptr_t) ( /*_.INITIAL_SYSTEM_DATA__V12*/ meltfptr[11]);

  /*put inside hook HOOK_RTL_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_EXECUTE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V287*/
				  meltfptr[286]));
  ((melthook_ptr_t) /*_.VALHOOK___V287*/ meltfptr[286])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_SYSTEM_DATA__V13*/ meltfptr[12]);

  /*put inside hook HOOK_RTL_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_EXECUTE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V287*/
				  meltfptr[286]));
  ((melthook_ptr_t) /*_.VALHOOK___V287*/ meltfptr[286])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_GCC_RTL_PASS__V16*/ meltfptr[15]);

  /*put inside hook HOOK_RTL_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_EXECUTE",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V287*/
				  meltfptr[286]));
  ((melthook_ptr_t) /*_.VALHOOK___V287*/ meltfptr[286])->tabval[3] =
    (melt_ptr_t) ( /*_.CLASS_GCC_PASS__V15*/ meltfptr[14]);

  /*put inside hook HOOK_RTL_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_RTL_EXECUTE",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V287*/
				  meltfptr[286]));
  ((melthook_ptr_t) /*_.VALHOOK___V287*/ meltfptr[286])->tabval[4] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*touch:HOOK_RTL_EXECUTE */
  meltgc_touch ( /*_.VALHOOK___V287*/ meltfptr[286]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V293*/ meltfptr[292])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V293*/
					meltfptr[292]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V293*/ meltfptr[292]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V294*/ meltfptr[293]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V293*/ meltfptr[292]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V293*/ meltfptr[292],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:1812:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V295*/ meltfptr[294])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V295*/
					meltfptr[294]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V295*/ meltfptr[294]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V296*/ meltfptr[295]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V297*/ meltfptr[296]);

  MELT_LOCATION ("warmelt-hooks.melt:1812:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V295*/ meltfptr[294])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V295*/
					meltfptr[294]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V295*/ meltfptr[294]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V297*/ meltfptr[296]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V298*/ meltfptr[297]);

  MELT_LOCATION ("warmelt-hooks.melt:1812:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V295*/ meltfptr[294])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V295*/
					meltfptr[294]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V295*/ meltfptr[294]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V298*/ meltfptr[297]);

  MELT_LOCATION ("warmelt-hooks.melt:1812:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V295*/ meltfptr[294])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V295*/
					meltfptr[294]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V295*/ meltfptr[294]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:1812:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V295*/ meltfptr[294]);

  MELT_LOCATION ("warmelt-hooks.melt:1812:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V295*/ meltfptr[294],
				"new static instance");

  /*put data in hook HOOK_SIMPLE_IPA_GATE */
  melt_assertmsg ("puthookdata checkhook HOOK_SIMPLE_IPA_GATE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V292*/ meltfptr[291])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V292*/ meltfptr[291])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V295*/ meltfptr[294]);

  /*put inside hook HOOK_SIMPLE_IPA_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_GATE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V292*/
				  meltfptr[291]));
  ((melthook_ptr_t) /*_.VALHOOK___V292*/ meltfptr[291])->tabval[0] =
    (melt_ptr_t) ( /*_.INITIAL_SYSTEM_DATA__V12*/ meltfptr[11]);

  /*put inside hook HOOK_SIMPLE_IPA_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_GATE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V292*/
				  meltfptr[291]));
  ((melthook_ptr_t) /*_.VALHOOK___V292*/ meltfptr[291])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_SYSTEM_DATA__V13*/ meltfptr[12]);

  /*put inside hook HOOK_SIMPLE_IPA_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_GATE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V292*/
				  meltfptr[291]));
  ((melthook_ptr_t) /*_.VALHOOK___V292*/ meltfptr[291])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_GCC_SIMPLE_IPA_PASS__V17*/ meltfptr[16]);

  /*put inside hook HOOK_SIMPLE_IPA_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_GATE",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V292*/
				  meltfptr[291]));
  ((melthook_ptr_t) /*_.VALHOOK___V292*/ meltfptr[291])->tabval[3] =
    (melt_ptr_t) ( /*_.CLASS_GCC_PASS__V15*/ meltfptr[14]);

  /*put inside hook HOOK_SIMPLE_IPA_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_GATE",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V292*/
				  meltfptr[291]));
  ((melthook_ptr_t) /*_.VALHOOK___V292*/ meltfptr[291])->tabval[4] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*put inside hook HOOK_SIMPLE_IPA_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_GATE",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V292*/
				  meltfptr[291]));
  ((melthook_ptr_t) /*_.VALHOOK___V292*/ meltfptr[291])->tabval[5] =
    (melt_ptr_t) ( /*_.VALDATA___V293*/ meltfptr[292]);

  /*put inside hook HOOK_SIMPLE_IPA_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_GATE",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V292*/
				  meltfptr[291]));
  ((melthook_ptr_t) /*_.VALHOOK___V292*/ meltfptr[291])->tabval[6] =
    (melt_ptr_t) ( /*_.VALSTR___V294*/ meltfptr[293]);

  /*touch:HOOK_SIMPLE_IPA_GATE */
  meltgc_touch ( /*_.VALHOOK___V292*/ meltfptr[291]);

  MELT_LOCATION ("warmelt-hooks.melt:1867:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V300*/ meltfptr[299])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V300*/
					meltfptr[299]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V300*/ meltfptr[299]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V301*/ meltfptr[300]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V302*/ meltfptr[301]);

  MELT_LOCATION ("warmelt-hooks.melt:1867:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V300*/ meltfptr[299])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V300*/
					meltfptr[299]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V300*/ meltfptr[299]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V302*/ meltfptr[301]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V303*/ meltfptr[302]);

  MELT_LOCATION ("warmelt-hooks.melt:1867:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V300*/ meltfptr[299])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V300*/
					meltfptr[299]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V300*/ meltfptr[299]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V303*/ meltfptr[302]);

  MELT_LOCATION ("warmelt-hooks.melt:1867:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V300*/ meltfptr[299])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V300*/
					meltfptr[299]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V300*/ meltfptr[299]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:1867:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V300*/ meltfptr[299]);

  MELT_LOCATION ("warmelt-hooks.melt:1867:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V300*/ meltfptr[299],
				"new static instance");

  /*put data in hook HOOK_SIMPLE_IPA_EXECUTE */
  melt_assertmsg ("puthookdata checkhook HOOK_SIMPLE_IPA_EXECUTE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V299*/ meltfptr[298])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V299*/ meltfptr[298])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V300*/ meltfptr[299]);

  /*put inside hook HOOK_SIMPLE_IPA_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_EXECUTE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V299*/
				  meltfptr[298]));
  ((melthook_ptr_t) /*_.VALHOOK___V299*/ meltfptr[298])->tabval[0] =
    (melt_ptr_t) ( /*_.INITIAL_SYSTEM_DATA__V12*/ meltfptr[11]);

  /*put inside hook HOOK_SIMPLE_IPA_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_EXECUTE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V299*/
				  meltfptr[298]));
  ((melthook_ptr_t) /*_.VALHOOK___V299*/ meltfptr[298])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_SYSTEM_DATA__V13*/ meltfptr[12]);

  /*put inside hook HOOK_SIMPLE_IPA_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_EXECUTE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V299*/
				  meltfptr[298]));
  ((melthook_ptr_t) /*_.VALHOOK___V299*/ meltfptr[298])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_GCC_SIMPLE_IPA_PASS__V17*/ meltfptr[16]);

  /*put inside hook HOOK_SIMPLE_IPA_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_EXECUTE",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V299*/
				  meltfptr[298]));
  ((melthook_ptr_t) /*_.VALHOOK___V299*/ meltfptr[298])->tabval[3] =
    (melt_ptr_t) ( /*_.CLASS_GCC_PASS__V15*/ meltfptr[14]);

  /*put inside hook HOOK_SIMPLE_IPA_EXECUTE */
  melt_assertmsg ("puthookconst checkhook HOOK_SIMPLE_IPA_EXECUTE",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V299*/
				  meltfptr[298]));
  ((melthook_ptr_t) /*_.VALHOOK___V299*/ meltfptr[298])->tabval[4] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*touch:HOOK_SIMPLE_IPA_EXECUTE */
  meltgc_touch ( /*_.VALHOOK___V299*/ meltfptr[298]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V304*/ meltfptr[303])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V304*/
					meltfptr[303]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V304*/ meltfptr[303]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V305*/ meltfptr[304]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V304*/ meltfptr[303]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V304*/ meltfptr[303],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V306*/ meltfptr[305])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V306*/
					meltfptr[305]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V306*/ meltfptr[305]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V307*/ meltfptr[306]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V306*/ meltfptr[305]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V306*/ meltfptr[305],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V308*/ meltfptr[307])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V308*/
					meltfptr[307]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V308*/ meltfptr[307]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V309*/ meltfptr[308]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V308*/ meltfptr[307]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V308*/ meltfptr[307],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V310*/ meltfptr[309])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V310*/
					meltfptr[309]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V310*/ meltfptr[309]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V311*/ meltfptr[310]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V310*/ meltfptr[309]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V310*/ meltfptr[309],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V312*/ meltfptr[311])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V312*/
					meltfptr[311]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V312*/ meltfptr[311]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V313*/ meltfptr[312]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V312*/ meltfptr[311]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V312*/ meltfptr[311],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V314*/ meltfptr[313])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V314*/
					meltfptr[313]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V314*/ meltfptr[313]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V315*/ meltfptr[314]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V314*/ meltfptr[313]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V314*/ meltfptr[313],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V316*/ meltfptr[315])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V316*/
					meltfptr[315]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V316*/ meltfptr[315]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V317*/ meltfptr[316]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V316*/ meltfptr[315]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V316*/ meltfptr[315],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V318*/ meltfptr[317])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V318*/
					meltfptr[317]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V318*/ meltfptr[317]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V319*/ meltfptr[318]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V318*/ meltfptr[317]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V318*/ meltfptr[317],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V320*/ meltfptr[319])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V320*/
					meltfptr[319]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V320*/ meltfptr[319]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V321*/ meltfptr[320]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V320*/ meltfptr[319]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V320*/ meltfptr[319],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V322*/ meltfptr[321])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V322*/
					meltfptr[321]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V322*/ meltfptr[321]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V323*/ meltfptr[322]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V322*/ meltfptr[321]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V322*/ meltfptr[321],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V324*/ meltfptr[323])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V324*/
					meltfptr[323]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V324*/ meltfptr[323]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V325*/ meltfptr[324]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V324*/ meltfptr[323]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V324*/ meltfptr[323],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V326*/ meltfptr[325])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V326*/
					meltfptr[325]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V326*/ meltfptr[325]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V327*/ meltfptr[326]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V326*/ meltfptr[325]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V326*/ meltfptr[325],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V328*/ meltfptr[327])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V328*/
					meltfptr[327]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V328*/ meltfptr[327]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V329*/ meltfptr[328]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V328*/ meltfptr[327]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V328*/ meltfptr[327],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V330*/ meltfptr[329])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V330*/
					meltfptr[329]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V330*/ meltfptr[329]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V331*/ meltfptr[330]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V330*/ meltfptr[329]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V330*/ meltfptr[329],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V332*/ meltfptr[331])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V332*/
					meltfptr[331]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V332*/ meltfptr[331]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V333*/ meltfptr[332]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V332*/ meltfptr[331]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V332*/ meltfptr[331],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V334*/ meltfptr[333])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V334*/
					meltfptr[333]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V334*/ meltfptr[333]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V335*/ meltfptr[334]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V334*/ meltfptr[333]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V334*/ meltfptr[333],
				"new static instance");


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_5 */

void
meltmod__warmelt_hooks__initialmeltchunk_6 (meltinitial_frame_t *
					    meltmeltframptr__,
					    char meltpredefinited[])
{
#define meltfram__ (*meltmeltframptr__)
#undef meltcallcount
#define meltcallcount 0L
  (void) meltpredefinited;
/*putslot*/
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V336*/ meltfptr[335])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V336*/
					meltfptr[335]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V336*/ meltfptr[335]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V337*/ meltfptr[336]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V336*/ meltfptr[335]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V336*/ meltfptr[335],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V338*/ meltfptr[337])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V338*/
					meltfptr[337]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V338*/ meltfptr[337]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V339*/ meltfptr[338]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V338*/ meltfptr[337]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V338*/ meltfptr[337],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V340*/ meltfptr[339])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V340*/
					meltfptr[339]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V340*/ meltfptr[339]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V341*/ meltfptr[340]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V340*/ meltfptr[339]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V340*/ meltfptr[339],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V342*/ meltfptr[341])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V342*/
					meltfptr[341]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V342*/ meltfptr[341]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V343*/ meltfptr[342]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V342*/ meltfptr[341]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V342*/ meltfptr[341],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V344*/ meltfptr[343])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V344*/
					meltfptr[343]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V344*/ meltfptr[343]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V345*/ meltfptr[344]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V344*/ meltfptr[343]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V344*/ meltfptr[343],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V346*/ meltfptr[345])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V346*/
					meltfptr[345]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V346*/ meltfptr[345]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V347*/ meltfptr[346]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V346*/ meltfptr[345]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V346*/ meltfptr[345],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V348*/ meltfptr[347])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V348*/
					meltfptr[347]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V348*/ meltfptr[347]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V349*/ meltfptr[348]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V348*/ meltfptr[347]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V348*/ meltfptr[347],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V350*/ meltfptr[349])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V350*/
					meltfptr[349]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V350*/ meltfptr[349]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V351*/ meltfptr[350]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V350*/ meltfptr[349]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V350*/ meltfptr[349],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V352*/ meltfptr[351])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V352*/
					meltfptr[351]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V352*/ meltfptr[351]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V353*/ meltfptr[352]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V352*/ meltfptr[351]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V352*/ meltfptr[351],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V354*/ meltfptr[353])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V354*/
					meltfptr[353]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V354*/ meltfptr[353]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V355*/ meltfptr[354]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V354*/ meltfptr[353]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V354*/ meltfptr[353],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V356*/ meltfptr[355])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V356*/
					meltfptr[355]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V356*/ meltfptr[355]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V357*/ meltfptr[356]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V356*/ meltfptr[355]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V356*/ meltfptr[355],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V358*/ meltfptr[357])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V358*/
					meltfptr[357]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V358*/ meltfptr[357]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V359*/ meltfptr[358]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V358*/ meltfptr[357]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V358*/ meltfptr[357],
				"new static instance");

 /**COMMENT: before toplevel body **/ ;

  MELT_LOCATION ("warmelt-hooks.melt:4:/ initchunk");
  /*^block */
  /*anyblock */
  {

    /*^cond */
    /*cond */ if (
		   /*quasi.cur.mod.env.cont cur.mod.env.cont : at very start */
							/*_.CONTENV___V2*/ meltfptr[1])
							/*then */
      {
	/*^cond.then */
  /*_.OR___V360*/ meltfptr[359] =
	  /*quasi.cur.mod.env.cont cur.mod.env.cont : at very start */
/*_.CONTENV___V2*/ meltfptr[1];;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:4:/ cond.else");

	/*^block */
	/*anyblock */
	{

	  /*^getslot */
	  {
	    melt_ptr_t slot = NULL, obj = NULL;
	    obj =
	      (melt_ptr_t) (((melt_ptr_t)
			     (MELT_PREDEF (INITIAL_SYSTEM_DATA)))) /*=obj*/ ;
	    melt_object_get_field (slot, obj, 3, "SYSDATA_CONT_FRESH_ENV");
     /*_.SYSDATA_CONT_FRESH_ENV__V361*/ meltfptr[360] = slot;
	  };
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^apply */
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_cstring = "warmelt-hooks";
	    /*_.FUN___V362*/ meltfptr[361] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.SYSDATA_CONT_FRESH_ENV__V361*/ meltfptr[360]),
			  (melt_ptr_t) ( /*_.PREVENV___V4*/ meltfptr[3]),
			  (MELTBPARSTR_CSTRING ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*_.OR___V360*/ meltfptr[359] = /*_.FUN___V362*/ meltfptr[361];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:4:/ clear");
	      /*clear *//*_.SYSDATA_CONT_FRESH_ENV__V361*/ meltfptr[360] = 0;
	  /*^clear */
	      /*clear *//*_.FUN___V362*/ meltfptr[361] = 0;
	}
	;
      }
    ;
    /*^block */
    /*com.block:upd.cur.mod.env.cont : at very start */
    {
    }				/*com.end block:upd.cur.mod.env.cont : at very start */
    ;
    /*epilog */

    /*^clear */
	    /*clear *//*_.OR___V360*/ meltfptr[359] = 0;
  }

  MELT_LOCATION ("warmelt-hooks.melt:1487:/ initchunk");
  /*^block */
  /*anyblock */
  {


    {
      /*^locexp */
      /* we always REGISTER_PASS_EXECUTION_HOOK_CHK__1 */
      register_callback (melt_plugin_name, PLUGIN_PASS_EXECUTION,
			 melt_pass_execution_hook_callback, NULL);
      ;
    }
    ;
    /*epilog */
  }

  MELT_LOCATION ("warmelt-hooks.melt:1933:/ initchunk");
  /*^block */
  /*anyblock */
  {

    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (((melt_ptr_t) (MELT_PREDEF (INITIAL_SYSTEM_DATA))))
	/*=obj*/ ;
      melt_object_get_field (slot, obj, 4, "SYSDATA_VALUE_EXPORTER");
   /*_.VALUEXPORTER___V363*/ meltfptr[360] = slot;
    };
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V116*/ meltfptr[115];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : AT_FINISH_UNIT_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V304*/ meltfptr[303]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V118*/ meltfptr[117];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : AT_FINISH_UNIT_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V306*/ meltfptr[305]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V96*/ meltfptr[95];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : AT_START_UNIT_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V308*/ meltfptr[307]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V98*/ meltfptr[97];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : AT_START_UNIT_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V310*/ meltfptr[309]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALHOOK___V18*/ meltfptr[17];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : HOOK_LOW_DEBUG_VALUE_AT */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V312*/ meltfptr[311]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALHOOK___V50*/ meltfptr[49];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : HOOK_OVERRIDE_GATE */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V314*/ meltfptr[313]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V176*/ meltfptr[175];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_IPA_PASSES_END_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V316*/ meltfptr[315]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V178*/ meltfptr[177];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_IPA_PASSES_END_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V318*/ meltfptr[317]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V161*/ meltfptr[160];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_IPA_PASSES_START_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V320*/ meltfptr[319]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V163*/ meltfptr[162];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_IPA_PASSES_START_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V322*/ meltfptr[321]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V146*/ meltfptr[145];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_PASSES_END_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V324*/ meltfptr[323]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V148*/ meltfptr[147];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_PASSES_END_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V326*/ meltfptr[325]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V131*/ meltfptr[130];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_PASSES_START_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V328*/ meltfptr[327]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V133*/ meltfptr[132];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_PASSES_START_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V330*/ meltfptr[329]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V206*/ meltfptr[205];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_EARLY_GIMPLE_PASSES_END_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V332*/ meltfptr[331]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V208*/ meltfptr[207];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_EARLY_GIMPLE_PASSES_END_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V334*/ meltfptr[333]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V191*/ meltfptr[190];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_EARLY_GIMPLE_PASSES_START_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V336*/ meltfptr[335]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V193*/ meltfptr[192];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_EARLY_GIMPLE_PASSES_START_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V338*/ meltfptr[337]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V240*/ meltfptr[239];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_FINISH_TYPE_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V340*/ meltfptr[339]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V242*/ meltfptr[241];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_FINISH_TYPE_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V342*/ meltfptr[341]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V70*/ meltfptr[69];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_OVERRIDE_GATE_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V344*/ meltfptr[343]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V72*/ meltfptr[71];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_OVERRIDE_GATE_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V346*/ meltfptr[345]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V259*/ meltfptr[258];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_PASS_EXECUTION_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V348*/ meltfptr[347]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V261*/ meltfptr[260];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_PASS_EXECUTION_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V350*/ meltfptr[349]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V224*/ meltfptr[223];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_PRE_GENERICIZE_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V352*/ meltfptr[351]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V226*/ meltfptr[225];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_PRE_GENERICIZE_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V354*/ meltfptr[353]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V74*/ meltfptr[73];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : UNREGISTER_OVERRIDE_GATE_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V356*/ meltfptr[355]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V76*/ meltfptr[75];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : UNREGISTER_OVERRIDE_GATE_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V363*/ meltfptr[360]),
		  (melt_ptr_t) ( /*_.VALDATA___V358*/ meltfptr[357]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*epilog */

    /*^clear */
	    /*clear *//*_.VALUEXPORTER___V363*/ meltfptr[360] = 0;
  }

 /**COMMENT: after toplevel body **/ ;

 /**COMMENT: compute boxloc again **/ ;

  /*cond */ if ( /*_.CONTENV___V2*/ meltfptr[1] || melt_object_length ((melt_ptr_t) MELT_PREDEF (INITIAL_SYSTEM_DATA)) < MELTFIELD_SYSDATA_CONT_FRESH_ENV)	/*then */
    {
    }
  else
    {				/*^cond.else */

      /*^block */
      /*com.block:compute fresh module environment */
      {

	/*^comment */
   /**COMMENT: start computing boxloc **/ ;
	;
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (((melt_ptr_t) (MELT_PREDEF (INITIAL_SYSTEM_DATA))))
	    /*=obj*/ ;
	  melt_object_get_field (slot, obj, 3, "SYSDATA_CONT_FRESH_ENV");
    /*_.FRESHENV___V3*/ meltfptr[2] = slot;
	};
	;
	/*^compute */

	/*checkfreshenv */ if (( /*_.PREVENV___V4*/ meltfptr[3])
			       &&
			       melt_magic_discr ((melt_ptr_t)
						 ( /*_.FRESHENV___V3*/
						  meltfptr[2])) !=
			       MELTOBMAG_CLOSURE)
	  warning (0,
		   "MELT corruption: bad FRESH_ENV @%p in system data <%s:%d>",
		   (void *) ( /*_.FRESHENV___V3*/ meltfptr[2]), __FILE__,
		   __LINE__);;;
	/*^apply */
	/*apply */
	{
	  /*_.CONTENV___V2*/ meltfptr[1] =
	    melt_apply ((meltclosure_ptr_t)
			( /*_.FRESHENV___V3*/ meltfptr[2]),
			(melt_ptr_t) ( /*_.PREVENV___V4*/ meltfptr[3]), (""),
			(union meltparam_un *) 0, "",
			(union meltparam_un *) 0);
	}
	;
      }				/*com.end block:compute fresh module environment */
      ;
    }

 /**COMMENT: intern symbols **/ ;

  /*internsym:VAL */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V24*/ meltfptr[23]));

  /*internsym:FILENAME */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V27*/ meltfptr[26]));

  /*internsym:LINENO */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V30*/ meltfptr[29]));

  /*internsym:MSG */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V33*/ meltfptr[32]));

  /*internsym:COUNT */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V36*/ meltfptr[35]));

  /*internsym:MELT_DEBUG_FUN */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V19*/ meltfptr[18]));

  /*internsym:OVERRIDE_GATE_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V40*/ meltfptr[39]));

  /*internsym:CLASS_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V42*/ meltfptr[41]));

  /*internsym:DISCR_LIST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V48*/ meltfptr[47]));

  /*internsym:BEFOREGATE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V58*/ meltfptr[57]));

  /*internsym:AFTERGATE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V62*/ meltfptr[61]));

  /*internkeyw:TRUE */
  (void)
    melthookproc_HOOK_INTERN_KEYWORD ((melt_ptr_t)
				      ( /*_.VALDATA___V51*/ meltfptr[50]));

  /*internsym:CLASS_REFERENCE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V53*/ meltfptr[52]));

  /*internsym:START_UNIT_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V77*/ meltfptr[76]));

  /*internsym:LIST_EVERY */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V84*/ meltfptr[83]));

  /*internsym:FINISH_UNIT_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V99*/ meltfptr[98]));

  /*internsym:ALL_PASSES_START_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V119*/ meltfptr[118]));

  /*internsym:ALL_PASSES_END_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V134*/ meltfptr[133]));

  /*internsym:ALL_IPA_PASSES_START_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V149*/ meltfptr[148]));

  /*internsym:ALL_IPA_PASSES_END_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V164*/ meltfptr[163]));

  /*internsym:EARLY_GIMPLE_PASSES_START_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V179*/ meltfptr[178]));

  /*internsym:EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V194*/ meltfptr[193]));

  /*internsym:PREGENERICIZE_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V209*/ meltfptr[208]));

  /*internsym:TFNDECL */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V219*/ meltfptr[218]));

  /*internsym:FINISHTYPE_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V227*/ meltfptr[226]));

  /*internsym:PASS_EXECUTION_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V243*/ meltfptr[242]));

  /*internsym:PASSNAME */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V251*/ meltfptr[250]));

  /*internsym:PASSNUM */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V254*/ meltfptr[253]));

  /*internsym:INITIAL_SYSTEM_DATA */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V263*/ meltfptr[262]));

  /*internsym:CLASS_SYSTEM_DATA */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V265*/ meltfptr[264]));

  /*internsym:CLASS_GCC_GIMPLE_PASS */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V267*/ meltfptr[266]));

  /*internsym:CLASS_GCC_PASS */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V269*/ meltfptr[268]));

  /*internsym:CLASS_GCC_RTL_PASS */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V281*/ meltfptr[280]));

  /*internsym:CLASS_GCC_SIMPLE_IPA_PASS */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V293*/ meltfptr[292]));

  /*internsym:AT_FINISH_UNIT_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V304*/ meltfptr[303]));

  /*internsym:AT_FINISH_UNIT_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V306*/ meltfptr[305]));

  /*internsym:AT_START_UNIT_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V308*/ meltfptr[307]));

  /*internsym:AT_START_UNIT_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V310*/ meltfptr[309]));

  /*internsym:HOOK_LOW_DEBUG_VALUE_AT */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V312*/ meltfptr[311]));

  /*internsym:HOOK_OVERRIDE_GATE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V314*/ meltfptr[313]));

  /*internsym:REGISTER_ALL_IPA_PASSES_END_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V316*/ meltfptr[315]));

  /*internsym:REGISTER_ALL_IPA_PASSES_END_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V318*/ meltfptr[317]));

  /*internsym:REGISTER_ALL_IPA_PASSES_START_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V320*/ meltfptr[319]));

  /*internsym:REGISTER_ALL_IPA_PASSES_START_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V322*/ meltfptr[321]));

  /*internsym:REGISTER_ALL_PASSES_END_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V324*/ meltfptr[323]));

  /*internsym:REGISTER_ALL_PASSES_END_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V326*/ meltfptr[325]));

  /*internsym:REGISTER_ALL_PASSES_START_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V328*/ meltfptr[327]));

  /*internsym:REGISTER_ALL_PASSES_START_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V330*/ meltfptr[329]));

  /*internsym:REGISTER_EARLY_GIMPLE_PASSES_END_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V332*/ meltfptr[331]));

  /*internsym:REGISTER_EARLY_GIMPLE_PASSES_END_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V334*/ meltfptr[333]));

  /*internsym:REGISTER_EARLY_GIMPLE_PASSES_START_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V336*/ meltfptr[335]));

  /*internsym:REGISTER_EARLY_GIMPLE_PASSES_START_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V338*/ meltfptr[337]));

  /*internsym:REGISTER_FINISH_TYPE_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V340*/ meltfptr[339]));

  /*internsym:REGISTER_FINISH_TYPE_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V342*/ meltfptr[341]));

  /*internsym:REGISTER_OVERRIDE_GATE_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V344*/ meltfptr[343]));

  /*internsym:REGISTER_OVERRIDE_GATE_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V346*/ meltfptr[345]));

  /*internsym:REGISTER_PASS_EXECUTION_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V348*/ meltfptr[347]));

  /*internsym:REGISTER_PASS_EXECUTION_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V350*/ meltfptr[349]));

  /*internsym:REGISTER_PRE_GENERICIZE_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V352*/ meltfptr[351]));

  /*internsym:REGISTER_PRE_GENERICIZE_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V354*/ meltfptr[353]));

  /*internsym:UNREGISTER_OVERRIDE_GATE_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V356*/ meltfptr[355]));

  /*internsym:UNREGISTER_OVERRIDE_GATE_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V358*/ meltfptr[357]));

 /**COMMENT: set retinit from boxloc **/ ;

 /*_.RETINIT___V1*/ meltfptr[0] =
    /* finalsetretinit */
    melt_reference_value ((melt_ptr_t) ( /*_.CONTENV___V2*/ meltfptr[1]));
 /**COMMENT: end the initproc **/ ;


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_6 */

void
meltmod__warmelt_hooks__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking)
{
  int meltix = 0;
  melt_initial_frame_st *meltframptr_ = (melt_initial_frame_st *) fp;
  melt_assertmsg ("check module frame",
		  meltframptr_->mcfr_nbvar == /*minihash */ -3343);
  if (!marking && melt_is_forwarding)
    {
      dbgprintf
	("forward_or_mark_module_start_frame_warmelt_hooks forwarding %d pointers in frame %p",
	 363, (void *) meltframptr_);
      for (meltix = 0; meltix < 363; meltix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltix]);
      return;
    }				/*end forwarding */
  dbgprintf
    ("forward_or_mark_module_start_frame_warmelt_hooks marking in frame %p",
     (void *) meltframptr_);
  for (meltix = 0; meltix < 363; meltix++)
    if (meltframptr_->mcfr_varptr[meltix])
      gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);

}				/* end meltmod__warmelt_hooks__forward_or_mark_module_start_frame */



/* exported 0 field offsets */

/* exported 0 class lengths */



/**** end of warmelt-hooks ****/
