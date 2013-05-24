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


/*** 14 extra MELT c-headers ***/


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

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 2 : **/

/* flag for PLUGIN_OVERRIDE_GATE */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_override_gate_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 3 : **/

/* flag for PLUGIN_START_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_start_unit_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 4 : **/


void MELT_MODULE_VISIBILITY
melt_startunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			     void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 5 : **/

/* flag for PLUGIN_FINISH_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_finish_unit_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 6 : **/


void MELT_MODULE_VISIBILITY
melt_finishunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			      void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 7 : **/

/* flag for PLUGIN_ALL_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_passes_start_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 8 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				     void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 9 : **/

/* flag for PLUGIN_ALL_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_all_passes_end_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 10 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 11 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_ipa_passes_start_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 12 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					 void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 13 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_ipa_passes_end_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 14 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				       void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/

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
meltmod__warmelt_hooks__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking);



/**** warmelt-hooks implementations ****/
/**** MELT GENERATED IMPLEMENTATIONS for warmelt-hooks ** DO NOT EDIT ; see gcc-melt.org ****/
/*** 13 extra MELT implementations ***/


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
#warning should call melthookproc_HOOK_ALL_IPA_PASSES_START ();
}



/** MELT extra c-implementation 12 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_END define */
bool melthk_all_ipa_passes_end_registered_flag;



/** MELT extra c-implementation 13 : **/


void
melt_all_ipa_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				       void *user_data ATTRIBUTE_UNUSED)
{
#warning should call    melthookproc_HOOK_ALL_IPA_PASSES_END ();
}

/*** end of 13 extra MELT c-implementations ***/
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
  meltframe.mcfr_nbvar = -1212 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -1212);
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
  meltframe.mcfr_nbvar = -3064 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -3064);
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
  meltframe.mcfr_nbvar = -2488 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -2488);
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
  meltframe.mcfr_nbvar = -2379 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -2379);
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
  meltframe.mcfr_nbvar = -13 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -13);
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
  meltframe.mcfr_nbvar = -2873 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -2873);
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
  meltframe.mcfr_nbvar = -512 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -512);
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
  meltframe.mcfr_nbvar = -1567 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -1567);
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
#define MELTFRAM_NBVARPTR 212
  melt_ptr_t mcfr_varptr[212];
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
    struct MELT_OBJECT_STRUCT (3) dsym_148__AT_FINISH_UNIT_FIRST;
    struct MELT_STRING_STRUCT (20) dstr_149__AT_FINISH_UNIT_F;
    struct MELT_OBJECT_STRUCT (3) dsym_150__AT_FINISH_UNIT_LAST;
    struct MELT_STRING_STRUCT (19) dstr_151__AT_FINISH_UNIT_L;
    struct MELT_OBJECT_STRUCT (3) dsym_152__AT_START_UNIT_FIRST;
    struct MELT_STRING_STRUCT (19) dstr_153__AT_START_UNIT_FI;
    struct MELT_OBJECT_STRUCT (3) dsym_154__AT_START_UNIT_LAST;
    struct MELT_STRING_STRUCT (18) dstr_155__AT_START_UNIT_LA;
    struct MELT_OBJECT_STRUCT (3) dsym_156__HOOK_OVERRIDE_GATE;
    struct MELT_STRING_STRUCT (18) dstr_157__HOOK_OVERRIDE_GA;
    struct MELT_OBJECT_STRUCT (3) dsym_158__HOOK_LOW_DEBUG_VALUE_AT;
    struct MELT_STRING_STRUCT (23) dstr_159__HOOK_LOW_DEBUG_V;
    struct MELT_OBJECT_STRUCT (3) dsym_160__REGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_STRING_STRUCT (28) dstr_161__REGISTER_OVERRID;
    struct MELT_OBJECT_STRUCT (3) dsym_162__REGISTER_OVERRIDE_GATE_LAST;
    struct MELT_STRING_STRUCT (27) dstr_163__REGISTER_OVERRID;
    struct MELT_OBJECT_STRUCT (3) dsym_164__REGISTER_ALL_IPA_PASSES_END_FIRST;
    struct MELT_STRING_STRUCT (33) dstr_165__REGISTER_ALL_IPA;
    struct MELT_OBJECT_STRUCT (3) dsym_166__REGISTER_ALL_IPA_PASSES_END_LAST;
    struct MELT_STRING_STRUCT (32) dstr_167__REGISTER_ALL_IPA;
    struct MELT_OBJECT_STRUCT (3)
      dsym_168__REGISTER_ALL_IPA_PASSES_START_FIRST;
    struct MELT_STRING_STRUCT (35) dstr_169__REGISTER_ALL_IPA;
    struct MELT_OBJECT_STRUCT (3)
      dsym_170__REGISTER_ALL_IPA_PASSES_START_LAST;
    struct MELT_STRING_STRUCT (34) dstr_171__REGISTER_ALL_IPA;
    struct MELT_OBJECT_STRUCT (3) dsym_172__REGISTER_ALL_PASSES_END_FIRST;
    struct MELT_STRING_STRUCT (29) dstr_173__REGISTER_ALL_PAS;
    struct MELT_OBJECT_STRUCT (3) dsym_174__REGISTER_ALL_PASSES_END_LAST;
    struct MELT_STRING_STRUCT (28) dstr_175__REGISTER_ALL_PAS;
    struct MELT_OBJECT_STRUCT (3) dsym_176__REGISTER_ALL_PASSES_START_FIRST;
    struct MELT_STRING_STRUCT (31) dstr_177__REGISTER_ALL_PAS;
    struct MELT_OBJECT_STRUCT (3) dsym_178__REGISTER_ALL_PASSES_START_LAST;
    struct MELT_STRING_STRUCT (30) dstr_179__REGISTER_ALL_PAS;
    struct MELT_OBJECT_STRUCT (3) dsym_180__UNREGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_STRING_STRUCT (30) dstr_181__UNREGISTER_OVERR;
    struct MELT_OBJECT_STRUCT (3) dsym_182__UNREGISTER_OVERRIDE_GATE_LAST;
    struct MELT_STRING_STRUCT (29) dstr_183__UNREGISTER_OVERR;
    long spare_;
  } *meltcdat = NULL;
  dbgprintf ("start initialize_module_meltdata_warmelt_hooks iniframp__=%p",
	     (void *) iniframp__);
  (void) meltpredefinited;	/* avoid warning if non-used. */
  melt_assertmsg ("check module initial frame",
		  iniframp__->mcfr_nbvar == /*minihash */ -2218);
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

  /*initial routine fill */


/*inihook dhook__1__HOOK_LOW_DEBUG_VALUE_AT */
  meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_LOW_DEBUG_VALUE_AT data */
  meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT,
			  melthook_HOOK_LOW_DEBUG_VALUE_AT);
  meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.hookdata =
    /*_.VALDATA___V15*/ meltfptr[14];
  strncpy (meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.hookname,
	   "HOOK_LOW_DEBUG_VALUE_AT",
	   sizeof (meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.hookname) - 1);
/*predefinedhook HOOK_LOW_DEBUG_VALUE_AT for dhook__1__HOOK_LOW_DEBUG_VALUE_AT*/
  if (!MELT_PREDEF (HOOK_LOW_DEBUG_VALUE_AT))
    MELT_STORE_PREDEF (HOOK_LOW_DEBUG_VALUE_AT,
		       (melt_ptr_t) & meltcdat->
		       dhook__1__HOOK_LOW_DEBUG_VALUE_AT);

  /*_.VALHOOK___V12*/ meltfptr[11] =
  /*inihooklocvar HOOK_LOW_DEBUG_VALUE_AT */
  (melt_ptr_t) & meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT;

  /*done inihook dhook__1__HOOK_LOW_DEBUG_VALUE_AT */

  MELT_LOCATION ("warmelt-hooks.melt:32:/ iniobj");

/*iniobj dobj_2__*/
/*_.VALDATA___V15*/ meltfptr[14] = (melt_ptr_t) & meltcdat->dobj_2__;
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
  meltcdat->dobj_2__.obj_hash = 36867523;
  meltcdat->dobj_2__.obj_len = 6;


/*iniobj dsym_3__VAL*/
  /*uniqueobj*/ if (! /*_.VALDATA___V18*/ meltfptr[17])
/*_.VALDATA___V18*/ meltfptr[17] = (melt_ptr_t) & meltcdat->dsym_3__VAL;
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
 /*_.VALSTR___V19*/ meltfptr[18] = (melt_ptr_t) & meltcdat->dstr_4__VAL;
  meltcdat->dstr_4__VAL.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_4__VAL.val, "VAL",
				sizeof (meltcdat->dstr_4__VAL.val) - 1);
  meltcdat->dstr_4__VAL.val[3] = (char) 0;
  meltcdat->dstr_4__VAL.slen = 3;

  /*^iniobj */

/*iniobj dobj_5*/
/*_.VALDATA___V17*/ meltfptr[16] = (melt_ptr_t) & meltcdat->dobj_5;
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
  meltcdat->dobj_5.obj_hash = 97884006;
  meltcdat->dobj_5.obj_len = 2;


/*iniobj dsym_6__FILENAME*/
  /*uniqueobj*/ if (! /*_.VALDATA___V21*/ meltfptr[20])
/*_.VALDATA___V21*/ meltfptr[20] =
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
 /*_.VALSTR___V22*/ meltfptr[21] = (melt_ptr_t) & meltcdat->dstr_7__FILENAME;
  meltcdat->dstr_7__FILENAME.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_7__FILENAME.val, "FILENAME",
				sizeof (meltcdat->dstr_7__FILENAME.val) - 1);
  meltcdat->dstr_7__FILENAME.val[8] = (char) 0;
  meltcdat->dstr_7__FILENAME.slen = 8;

  /*^iniobj */

/*iniobj dobj_8*/
/*_.VALDATA___V20*/ meltfptr[19] = (melt_ptr_t) & meltcdat->dobj_8;
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
  meltcdat->dobj_8.obj_hash = 528158789;
  meltcdat->dobj_8.obj_len = 2;


/*iniobj dsym_9__LINENO*/
  /*uniqueobj*/ if (! /*_.VALDATA___V24*/ meltfptr[23])
/*_.VALDATA___V24*/ meltfptr[23] =
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
 /*_.VALSTR___V25*/ meltfptr[24] = (melt_ptr_t) & meltcdat->dstr_10__LINENO;
  meltcdat->dstr_10__LINENO.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_10__LINENO.val, "LINENO",
				sizeof (meltcdat->dstr_10__LINENO.val) - 1);
  meltcdat->dstr_10__LINENO.val[6] = (char) 0;
  meltcdat->dstr_10__LINENO.slen = 6;

  /*^iniobj */

/*iniobj dobj_11*/
/*_.VALDATA___V23*/ meltfptr[22] = (melt_ptr_t) & meltcdat->dobj_11;
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
  meltcdat->dobj_11.obj_hash = 810466935;
  meltcdat->dobj_11.obj_len = 2;


/*iniobj dsym_12__MSG*/
  /*uniqueobj*/ if (! /*_.VALDATA___V27*/ meltfptr[26])
/*_.VALDATA___V27*/ meltfptr[26] = (melt_ptr_t) & meltcdat->dsym_12__MSG;
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
 /*_.VALSTR___V28*/ meltfptr[27] = (melt_ptr_t) & meltcdat->dstr_13__MSG;
  meltcdat->dstr_13__MSG.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_13__MSG.val, "MSG",
				sizeof (meltcdat->dstr_13__MSG.val) - 1);
  meltcdat->dstr_13__MSG.val[3] = (char) 0;
  meltcdat->dstr_13__MSG.slen = 3;

  /*^iniobj */

/*iniobj dobj_14*/
/*_.VALDATA___V26*/ meltfptr[25] = (melt_ptr_t) & meltcdat->dobj_14;
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
  meltcdat->dobj_14.obj_hash = 213972843;
  meltcdat->dobj_14.obj_len = 2;


/*iniobj dsym_15__COUNT*/
  /*uniqueobj*/ if (! /*_.VALDATA___V30*/ meltfptr[29])
/*_.VALDATA___V30*/ meltfptr[29] =
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
 /*_.VALSTR___V31*/ meltfptr[30] = (melt_ptr_t) & meltcdat->dstr_16__COUNT;
  meltcdat->dstr_16__COUNT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_16__COUNT.val, "COUNT",
				sizeof (meltcdat->dstr_16__COUNT.val) - 1);
  meltcdat->dstr_16__COUNT.val[5] = (char) 0;
  meltcdat->dstr_16__COUNT.slen = 5;

  /*^iniobj */

/*iniobj dobj_17*/
/*_.VALDATA___V29*/ meltfptr[28] = (melt_ptr_t) & meltcdat->dobj_17;
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
  meltcdat->dobj_17.obj_hash = 404975914;
  meltcdat->dobj_17.obj_len = 2;

  /*inimult dtup_18___HOOK_LOW_DEBUG_VALUE_AT */
 /*_.VALTUP___V32*/ meltfptr[31] =
    (melt_ptr_t) & meltcdat->dtup_18___HOOK_LOW_DEBUG_VALUE_AT;
  meltcdat->dtup_18___HOOK_LOW_DEBUG_VALUE_AT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_18___HOOK_LOW_DEBUG_VALUE_AT.nbval = 5;
  /*inimult dtup_19___HOOK_LOW_DEBUG_VALUE_AT */
 /*_.VALTUP___V33*/ meltfptr[32] =
    (melt_ptr_t) & meltcdat->dtup_19___HOOK_LOW_DEBUG_VALUE_AT;
  meltcdat->dtup_19___HOOK_LOW_DEBUG_VALUE_AT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_19___HOOK_LOW_DEBUG_VALUE_AT.nbval = 0;

/*iniobj dsym_20__MELT_DEBUG_FUN*/
  /*uniqueobj*/ if (! /*_.VALDATA___V13*/ meltfptr[12])
/*_.VALDATA___V13*/ meltfptr[12] =
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
 /*_.VALSTR___V14*/ meltfptr[13] =
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
  /*uniqueobj*/ if (! /*_.VALDATA___V34*/ meltfptr[33])
/*_.VALDATA___V34*/ meltfptr[33] =
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
 /*_.VALSTR___V35*/ meltfptr[34] =
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
  /*uniqueobj*/ if (! /*_.VALDATA___V36*/ meltfptr[35])
/*_.VALDATA___V36*/ meltfptr[35] =
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
 /*_.VALSTR___V37*/ meltfptr[36] =
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
/*_.VALDATA___V38*/ meltfptr[37] =
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
  meltcdat->dobj_26__OVERRIDE_GATE_DELAYED_QUEUE.obj_hash = 61218423;
  meltcdat->dobj_26__OVERRIDE_GATE_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_27__override_gate_de*/
 /*_.VALSTR___V41*/ meltfptr[40] =
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
  /*uniqueobj*/ if (! /*_.VALDATA___V42*/ meltfptr[41])
/*_.VALDATA___V42*/ meltfptr[41] =
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
 /*_.VALSTR___V43*/ meltfptr[42] =
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
    /*_.VALDATA___V49*/ meltfptr[48];
  strncpy (meltcdat->dhook__30__HOOK_OVERRIDE_GATE.hookname,
	   "HOOK_OVERRIDE_GATE",
	   sizeof (meltcdat->dhook__30__HOOK_OVERRIDE_GATE.hookname) - 1);
/*predefinedhook HOOK_OVERRIDE_GATE for dhook__30__HOOK_OVERRIDE_GATE*/
  if (!MELT_PREDEF (HOOK_OVERRIDE_GATE))
    MELT_STORE_PREDEF (HOOK_OVERRIDE_GATE,
		       (melt_ptr_t) & meltcdat->
		       dhook__30__HOOK_OVERRIDE_GATE);

  /*_.VALHOOK___V44*/ meltfptr[43] =
  /*inihooklocvar HOOK_OVERRIDE_GATE */
  (melt_ptr_t) & meltcdat->dhook__30__HOOK_OVERRIDE_GATE;

  /*done inihook dhook__30__HOOK_OVERRIDE_GATE */

  MELT_LOCATION ("warmelt-hooks.melt:49:/ iniobj");

/*iniobj dobj_31__*/
/*_.VALDATA___V49*/ meltfptr[48] = (melt_ptr_t) & meltcdat->dobj_31__;
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
  meltcdat->dobj_31__.obj_hash = 985216815;
  meltcdat->dobj_31__.obj_len = 6;


/*iniobj dsym_32__BEFOREGATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V52*/ meltfptr[51])
/*_.VALDATA___V52*/ meltfptr[51] =
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
 /*_.VALSTR___V53*/ meltfptr[52] =
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
/*_.VALDATA___V51*/ meltfptr[50] = (melt_ptr_t) & meltcdat->dobj_34;
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
  meltcdat->dobj_34.obj_hash = 639945006;
  meltcdat->dobj_34.obj_len = 2;

  /*inimult dtup_35___HOOK_OVERRIDE_GATE */
 /*_.VALTUP___V54*/ meltfptr[53] =
    (melt_ptr_t) & meltcdat->dtup_35___HOOK_OVERRIDE_GATE;
  meltcdat->dtup_35___HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_35___HOOK_OVERRIDE_GATE.nbval = 1;

/*iniobj dsym_36__AFTERGATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V56*/ meltfptr[55])
/*_.VALDATA___V56*/ meltfptr[55] =
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
 /*_.VALSTR___V57*/ meltfptr[56] =
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
/*_.VALDATA___V55*/ meltfptr[54] = (melt_ptr_t) & meltcdat->dobj_38;
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
  meltcdat->dobj_38.obj_hash = 589292111;
  meltcdat->dobj_38.obj_len = 2;

  /*inimult dtup_39___HOOK_OVERRIDE_GATE */
 /*_.VALTUP___V58*/ meltfptr[57] =
    (melt_ptr_t) & meltcdat->dtup_39___HOOK_OVERRIDE_GATE;
  meltcdat->dtup_39___HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_39___HOOK_OVERRIDE_GATE.nbval = 1;

/*iniobj dsym_40__TRUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V45*/ meltfptr[44])
/*_.VALDATA___V45*/ meltfptr[44] = (melt_ptr_t) & meltcdat->dsym_40__TRUE;
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
 /*_.VALSTR___V46*/ meltfptr[45] = (melt_ptr_t) & meltcdat->dstr_41__TRUE;
  meltcdat->dstr_41__TRUE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_41__TRUE.val, "TRUE",
				sizeof (meltcdat->dstr_41__TRUE.val) - 1);
  meltcdat->dstr_41__TRUE.val[4] = (char) 0;
  meltcdat->dstr_41__TRUE.slen = 4;


/*iniobj dsym_42__CLASS_REFERENCE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V47*/ meltfptr[46])
/*_.VALDATA___V47*/ meltfptr[46] =
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
 /*_.VALSTR___V48*/ meltfptr[47] =
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
 /*_.VALROUT___V59*/ meltfptr[58] =
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
 /*_.VALCLO___V60*/ meltfptr[59] =
    (melt_ptr_t) & meltcdat->dclo_45__ENABLE_OVERRIDE_GATE;
  meltcdat->dclo_45__ENABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_45__ENABLE_OVERRIDE_GATE.nbval = 0;

  /*inirout drout_46__MAYBE_DISABLE_OVERRIDE_GATE */
 /*_.VALROUT___V61*/ meltfptr[60] =
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
 /*_.VALCLO___V62*/ meltfptr[61] =
    (melt_ptr_t) & meltcdat->dclo_47__MAYBE_DISABLE_OVERRIDE_GATE;
  meltcdat->dclo_47__MAYBE_DISABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_47__MAYBE_DISABLE_OVERRIDE_GATE.nbval = 0;

  /*inirout drout_48__REGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALROUT___V63*/ meltfptr[62] =
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
 /*_.VALCLO___V64*/ meltfptr[63] =
    (melt_ptr_t) & meltcdat->dclo_49__REGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->dclo_49__REGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_49__REGISTER_OVERRIDE_GATE_FIRST.nbval = 0;

  /*inirout drout_50__REGISTER_OVERRIDE_GATE_LAST */
 /*_.VALROUT___V65*/ meltfptr[64] =
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
 /*_.VALCLO___V66*/ meltfptr[65] =
    (melt_ptr_t) & meltcdat->dclo_51__REGISTER_OVERRIDE_GATE_LAST;
  meltcdat->dclo_51__REGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_51__REGISTER_OVERRIDE_GATE_LAST.nbval = 0;

  /*inirout drout_52__UNREGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALROUT___V67*/ meltfptr[66] =
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
 /*_.VALCLO___V68*/ meltfptr[67] =
    (melt_ptr_t) & meltcdat->dclo_53__UNREGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->dclo_53__UNREGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_53__UNREGISTER_OVERRIDE_GATE_FIRST.nbval = 0;

  /*inirout drout_54__UNREGISTER_OVERRIDE_GATE_LAST */
 /*_.VALROUT___V69*/ meltfptr[68] =
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
 /*_.VALCLO___V70*/ meltfptr[69] =
    (melt_ptr_t) & meltcdat->dclo_55__UNREGISTER_OVERRIDE_GATE_LAST;
  meltcdat->dclo_55__UNREGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_55__UNREGISTER_OVERRIDE_GATE_LAST.nbval = 0;


/*iniobj dsym_56__START_UNIT_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V71*/ meltfptr[70])
/*_.VALDATA___V71*/ meltfptr[70] =
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
 /*_.VALSTR___V72*/ meltfptr[71] =
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
/*_.VALDATA___V73*/ meltfptr[72] =
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
  meltcdat->dobj_58__START_UNIT_DELAYED_QUEUE.obj_hash = 989971911;
  meltcdat->dobj_58__START_UNIT_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_59__start_unit_delay*/
 /*_.VALSTR___V76*/ meltfptr[75] =
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
    /*_.VALDATA___V83*/ meltfptr[82];
  strncpy (meltcdat->dhook__60__HOOK_START_UNIT.hookname, "HOOK_START_UNIT",
	   sizeof (meltcdat->dhook__60__HOOK_START_UNIT.hookname) - 1);
/*predefinedhook HOOK_START_UNIT for dhook__60__HOOK_START_UNIT*/
  if (!MELT_PREDEF (HOOK_START_UNIT))
    MELT_STORE_PREDEF (HOOK_START_UNIT,
		       (melt_ptr_t) & meltcdat->dhook__60__HOOK_START_UNIT);

  /*_.VALHOOK___V77*/ meltfptr[76] =
  /*inihooklocvar HOOK_START_UNIT */
  (melt_ptr_t) & meltcdat->dhook__60__HOOK_START_UNIT;

  /*done inihook dhook__60__HOOK_START_UNIT */

  MELT_LOCATION ("warmelt-hooks.melt:259:/ iniobj");

/*iniobj dobj_61__*/
/*_.VALDATA___V83*/ meltfptr[82] = (melt_ptr_t) & meltcdat->dobj_61__;
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
  meltcdat->dobj_61__.obj_hash = 303872640;
  meltcdat->dobj_61__.obj_len = 6;

  /*inimult dtup_62___HOOK_START_UNIT */
 /*_.VALTUP___V85*/ meltfptr[84] =
    (melt_ptr_t) & meltcdat->dtup_62___HOOK_START_UNIT;
  meltcdat->dtup_62___HOOK_START_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_62___HOOK_START_UNIT.nbval = 0;
  /*inimult dtup_63___HOOK_START_UNIT */
 /*_.VALTUP___V86*/ meltfptr[85] =
    (melt_ptr_t) & meltcdat->dtup_63___HOOK_START_UNIT;
  meltcdat->dtup_63___HOOK_START_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_63___HOOK_START_UNIT.nbval = 0;

/*iniobj dsym_64__LIST_EVERY*/
  /*uniqueobj*/ if (! /*_.VALDATA___V78*/ meltfptr[77])
/*_.VALDATA___V78*/ meltfptr[77] =
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
 /*_.VALSTR___V79*/ meltfptr[78] =
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
 /*_.VALROUT___V80*/ meltfptr[79] =
    (melt_ptr_t) & meltcdat->drout_66__LAMBDA_;
  meltcdat->drout_66__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_66__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:269", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_66__LAMBDA_.nbval = 1;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_66__LAMBDA_,
			     meltrout_9_warmelt_hooks_LAMBDA___1__);

  /*inirout drout_67__LAMBDA_ */
 /*_.VALROUT___V81*/ meltfptr[80] =
    (melt_ptr_t) & meltcdat->drout_67__LAMBDA_;
  meltcdat->drout_67__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_67__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:278", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_67__LAMBDA_.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_67__LAMBDA_,
			     meltrout_10_warmelt_hooks_LAMBDA___2__);

  /*inirout drout_68__LAMBDA_ */
 /*_.VALROUT___V82*/ meltfptr[81] =
    (melt_ptr_t) & meltcdat->drout_68__LAMBDA_;
  meltcdat->drout_68__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_68__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:283", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_68__LAMBDA_.nbval = 1;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_68__LAMBDA_,
			     meltrout_11_warmelt_hooks_LAMBDA___3__);

  /*inirout drout_69__ENABLE_START_UNIT */
 /*_.VALROUT___V87*/ meltfptr[86] =
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
 /*_.VALCLO___V88*/ meltfptr[87] =
    (melt_ptr_t) & meltcdat->dclo_70__ENABLE_START_UNIT;
  meltcdat->dclo_70__ENABLE_START_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_70__ENABLE_START_UNIT.nbval = 0;

  /*inirout drout_71__AT_START_UNIT_FIRST */
 /*_.VALROUT___V89*/ meltfptr[88] =
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
 /*_.VALCLO___V90*/ meltfptr[89] =
    (melt_ptr_t) & meltcdat->dclo_72__AT_START_UNIT_FIRST;
  meltcdat->dclo_72__AT_START_UNIT_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_72__AT_START_UNIT_FIRST.nbval = 0;

  /*inirout drout_73__AT_START_UNIT_LAST */
 /*_.VALROUT___V91*/ meltfptr[90] =
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
 /*_.VALCLO___V92*/ meltfptr[91] =
    (melt_ptr_t) & meltcdat->dclo_74__AT_START_UNIT_LAST;
  meltcdat->dclo_74__AT_START_UNIT_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_74__AT_START_UNIT_LAST.nbval = 0;


/*iniobj dsym_75__FINISH_UNIT_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V93*/ meltfptr[92])
/*_.VALDATA___V93*/ meltfptr[92] =
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
 /*_.VALSTR___V94*/ meltfptr[93] =
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
/*_.VALDATA___V95*/ meltfptr[94] =
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
  meltcdat->dobj_77__FINISH_UNIT_DELAYED_QUEUE.obj_hash = 642443728;
  meltcdat->dobj_77__FINISH_UNIT_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_78__finish_unit_dela*/
 /*_.VALSTR___V98*/ meltfptr[97] =
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
    /*_.VALDATA___V103*/ meltfptr[102];
  strncpy (meltcdat->dhook__79__HOOK_FINISH_UNIT.hookname, "HOOK_FINISH_UNIT",
	   sizeof (meltcdat->dhook__79__HOOK_FINISH_UNIT.hookname) - 1);
/*predefinedhook HOOK_FINISH_UNIT for dhook__79__HOOK_FINISH_UNIT*/
  if (!MELT_PREDEF (HOOK_FINISH_UNIT))
    MELT_STORE_PREDEF (HOOK_FINISH_UNIT,
		       (melt_ptr_t) & meltcdat->dhook__79__HOOK_FINISH_UNIT);

  /*_.VALHOOK___V99*/ meltfptr[98] =
  /*inihooklocvar HOOK_FINISH_UNIT */
  (melt_ptr_t) & meltcdat->dhook__79__HOOK_FINISH_UNIT;

  /*done inihook dhook__79__HOOK_FINISH_UNIT */

  MELT_LOCATION ("warmelt-hooks.melt:361:/ iniobj");

/*iniobj dobj_80__*/
/*_.VALDATA___V103*/ meltfptr[102] = (melt_ptr_t) & meltcdat->dobj_80__;
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
  meltcdat->dobj_80__.obj_hash = 813312488;
  meltcdat->dobj_80__.obj_len = 6;

  /*inimult dtup_81___HOOK_FINISH_UNIT */
 /*_.VALTUP___V105*/ meltfptr[104] =
    (melt_ptr_t) & meltcdat->dtup_81___HOOK_FINISH_UNIT;
  meltcdat->dtup_81___HOOK_FINISH_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_81___HOOK_FINISH_UNIT.nbval = 0;
  /*inimult dtup_82___HOOK_FINISH_UNIT */
 /*_.VALTUP___V106*/ meltfptr[105] =
    (melt_ptr_t) & meltcdat->dtup_82___HOOK_FINISH_UNIT;
  meltcdat->dtup_82___HOOK_FINISH_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_82___HOOK_FINISH_UNIT.nbval = 0;
  /*inirout drout_83__LAMBDA_ */
 /*_.VALROUT___V100*/ meltfptr[99] =
    (melt_ptr_t) & meltcdat->drout_83__LAMBDA_;
  meltcdat->drout_83__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_83__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:371", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_83__LAMBDA_.nbval = 1;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_83__LAMBDA_,
			     meltrout_16_warmelt_hooks_LAMBDA___4__);

  /*inirout drout_84__LAMBDA_ */
 /*_.VALROUT___V101*/ meltfptr[100] =
    (melt_ptr_t) & meltcdat->drout_84__LAMBDA_;
  meltcdat->drout_84__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_84__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:380", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_84__LAMBDA_.nbval = 0;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_84__LAMBDA_,
			     meltrout_17_warmelt_hooks_LAMBDA___5__);

  /*inirout drout_85__LAMBDA_ */
 /*_.VALROUT___V102*/ meltfptr[101] =
    (melt_ptr_t) & meltcdat->drout_85__LAMBDA_;
  meltcdat->drout_85__LAMBDA_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_85__LAMBDA_.routdescr,
	   "LAMBDA_ @warmelt-hooks.melt:385", MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_85__LAMBDA_.nbval = 1;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_85__LAMBDA_,
			     meltrout_18_warmelt_hooks_LAMBDA___6__);

  /*inirout drout_86__ENABLE_FINISH_UNIT */
 /*_.VALROUT___V107*/ meltfptr[106] =
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
 /*_.VALCLO___V108*/ meltfptr[107] =
    (melt_ptr_t) & meltcdat->dclo_87__ENABLE_FINISH_UNIT;
  meltcdat->dclo_87__ENABLE_FINISH_UNIT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_87__ENABLE_FINISH_UNIT.nbval = 0;

  /*inirout drout_88__AT_FINISH_UNIT_FIRST */
 /*_.VALROUT___V109*/ meltfptr[108] =
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
 /*_.VALCLO___V110*/ meltfptr[109] =
    (melt_ptr_t) & meltcdat->dclo_89__AT_FINISH_UNIT_FIRST;
  meltcdat->dclo_89__AT_FINISH_UNIT_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_89__AT_FINISH_UNIT_FIRST.nbval = 0;

  /*inirout drout_90__AT_FINISH_UNIT_LAST */
 /*_.VALROUT___V111*/ meltfptr[110] =
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
 /*_.VALCLO___V112*/ meltfptr[111] =
    (melt_ptr_t) & meltcdat->dclo_91__AT_FINISH_UNIT_LAST;
  meltcdat->dclo_91__AT_FINISH_UNIT_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_91__AT_FINISH_UNIT_LAST.nbval = 0;


/*iniobj dsym_92__ALL_PASSES_START_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V113*/ meltfptr[112])
/*_.VALDATA___V113*/ meltfptr[112] =
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
 /*_.VALSTR___V114*/ meltfptr[113] =
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
/*_.VALDATA___V115*/ meltfptr[114] =
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
  meltcdat->dobj_94__ALL_PASSES_START_DELAYED_QUEUE.obj_hash = 421742374;
  meltcdat->dobj_94__ALL_PASSES_START_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_95__all_passes_start*/
 /*_.VALSTR___V116*/ meltfptr[115] =
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
    /*_.VALDATA___V118*/ meltfptr[117];
  strncpy (meltcdat->dhook__96__HOOK_ALL_PASSES_START.hookname,
	   "HOOK_ALL_PASSES_START",
	   sizeof (meltcdat->dhook__96__HOOK_ALL_PASSES_START.hookname) - 1);
/*predefinedhook HOOK_ALL_PASSES_START for dhook__96__HOOK_ALL_PASSES_START*/
  if (!MELT_PREDEF (HOOK_ALL_PASSES_START))
    MELT_STORE_PREDEF (HOOK_ALL_PASSES_START,
		       (melt_ptr_t) & meltcdat->
		       dhook__96__HOOK_ALL_PASSES_START);

  /*_.VALHOOK___V117*/ meltfptr[116] =
  /*inihooklocvar HOOK_ALL_PASSES_START */
  (melt_ptr_t) & meltcdat->dhook__96__HOOK_ALL_PASSES_START;

  /*done inihook dhook__96__HOOK_ALL_PASSES_START */

  MELT_LOCATION ("warmelt-hooks.melt:472:/ iniobj");

/*iniobj dobj_97__*/
/*_.VALDATA___V118*/ meltfptr[117] = (melt_ptr_t) & meltcdat->dobj_97__;
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
  meltcdat->dobj_97__.obj_hash = 858965529;
  meltcdat->dobj_97__.obj_len = 6;

  /*inimult dtup_98___HOOK_ALL_PASSES_START */
 /*_.VALTUP___V120*/ meltfptr[119] =
    (melt_ptr_t) & meltcdat->dtup_98___HOOK_ALL_PASSES_START;
  meltcdat->dtup_98___HOOK_ALL_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_98___HOOK_ALL_PASSES_START.nbval = 0;
  /*inimult dtup_99___HOOK_ALL_PASSES_START */
 /*_.VALTUP___V121*/ meltfptr[120] =
    (melt_ptr_t) & meltcdat->dtup_99___HOOK_ALL_PASSES_START;
  meltcdat->dtup_99___HOOK_ALL_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_99___HOOK_ALL_PASSES_START.nbval = 0;
  /*inirout drout_100__ENABLE_ALL_PASSES_START */
 /*_.VALROUT___V122*/ meltfptr[121] =
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
 /*_.VALCLO___V123*/ meltfptr[122] =
    (melt_ptr_t) & meltcdat->dclo_101__ENABLE_ALL_PASSES_START;
  meltcdat->dclo_101__ENABLE_ALL_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_101__ENABLE_ALL_PASSES_START.nbval = 0;

  /*inirout drout_102__REGISTER_ALL_PASSES_START_FIRST */
 /*_.VALROUT___V124*/ meltfptr[123] =
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
 /*_.VALCLO___V125*/ meltfptr[124] =
    (melt_ptr_t) & meltcdat->dclo_103__REGISTER_ALL_PASSES_START_FIRST;
  meltcdat->dclo_103__REGISTER_ALL_PASSES_START_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_103__REGISTER_ALL_PASSES_START_FIRST.nbval = 0;

  /*inirout drout_104__REGISTER_ALL_PASSES_START_LAST */
 /*_.VALROUT___V126*/ meltfptr[125] =
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
 /*_.VALCLO___V127*/ meltfptr[126] =
    (melt_ptr_t) & meltcdat->dclo_105__REGISTER_ALL_PASSES_START_LAST;
  meltcdat->dclo_105__REGISTER_ALL_PASSES_START_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_105__REGISTER_ALL_PASSES_START_LAST.nbval = 0;


/*iniobj dsym_106__ALL_PASSES_END_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V128*/ meltfptr[127])
/*_.VALDATA___V128*/ meltfptr[127] =
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
 /*_.VALSTR___V129*/ meltfptr[128] =
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
/*_.VALDATA___V130*/ meltfptr[129] =
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
  meltcdat->dobj_108__ALL_PASSES_END_DELAYED_QUEUE.obj_hash = 607618752;
  meltcdat->dobj_108__ALL_PASSES_END_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_109__all_passes_end_d*/
 /*_.VALSTR___V131*/ meltfptr[130] =
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
    /*_.VALDATA___V133*/ meltfptr[132];
  strncpy (meltcdat->dhook__110__HOOK_ALL_PASSES_END.hookname,
	   "HOOK_ALL_PASSES_END",
	   sizeof (meltcdat->dhook__110__HOOK_ALL_PASSES_END.hookname) - 1);
/*predefinedhook HOOK_ALL_PASSES_END for dhook__110__HOOK_ALL_PASSES_END*/
  if (!MELT_PREDEF (HOOK_ALL_PASSES_END))
    MELT_STORE_PREDEF (HOOK_ALL_PASSES_END,
		       (melt_ptr_t) & meltcdat->
		       dhook__110__HOOK_ALL_PASSES_END);

  /*_.VALHOOK___V132*/ meltfptr[131] =
  /*inihooklocvar HOOK_ALL_PASSES_END */
  (melt_ptr_t) & meltcdat->dhook__110__HOOK_ALL_PASSES_END;

  /*done inihook dhook__110__HOOK_ALL_PASSES_END */

  MELT_LOCATION ("warmelt-hooks.melt:596:/ iniobj");

/*iniobj dobj_111__*/
/*_.VALDATA___V133*/ meltfptr[132] = (melt_ptr_t) & meltcdat->dobj_111__;
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
  meltcdat->dobj_111__.obj_hash = 321423601;
  meltcdat->dobj_111__.obj_len = 6;

  /*inimult dtup_112___HOOK_ALL_PASSES_END */
 /*_.VALTUP___V135*/ meltfptr[134] =
    (melt_ptr_t) & meltcdat->dtup_112___HOOK_ALL_PASSES_END;
  meltcdat->dtup_112___HOOK_ALL_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_112___HOOK_ALL_PASSES_END.nbval = 0;
  /*inimult dtup_113___HOOK_ALL_PASSES_END */
 /*_.VALTUP___V136*/ meltfptr[135] =
    (melt_ptr_t) & meltcdat->dtup_113___HOOK_ALL_PASSES_END;
  meltcdat->dtup_113___HOOK_ALL_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_113___HOOK_ALL_PASSES_END.nbval = 0;
  /*inirout drout_114__ENABLE_ALL_PASSES_END */
 /*_.VALROUT___V137*/ meltfptr[136] =
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
 /*_.VALCLO___V138*/ meltfptr[137] =
    (melt_ptr_t) & meltcdat->dclo_115__ENABLE_ALL_PASSES_END;
  meltcdat->dclo_115__ENABLE_ALL_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_115__ENABLE_ALL_PASSES_END.nbval = 0;

  /*inirout drout_116__REGISTER_ALL_PASSES_END_FIRST */
 /*_.VALROUT___V139*/ meltfptr[138] =
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
 /*_.VALCLO___V140*/ meltfptr[139] =
    (melt_ptr_t) & meltcdat->dclo_117__REGISTER_ALL_PASSES_END_FIRST;
  meltcdat->dclo_117__REGISTER_ALL_PASSES_END_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_117__REGISTER_ALL_PASSES_END_FIRST.nbval = 0;

  /*inirout drout_118__REGISTER_ALL_PASSES_END_LAST */
 /*_.VALROUT___V141*/ meltfptr[140] =
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
 /*_.VALCLO___V142*/ meltfptr[141] =
    (melt_ptr_t) & meltcdat->dclo_119__REGISTER_ALL_PASSES_END_LAST;
  meltcdat->dclo_119__REGISTER_ALL_PASSES_END_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_119__REGISTER_ALL_PASSES_END_LAST.nbval = 0;


/*iniobj dsym_120__ALL_IPA_PASSES_START_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V143*/ meltfptr[142])
/*_.VALDATA___V143*/ meltfptr[142] =
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
 /*_.VALSTR___V144*/ meltfptr[143] =
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
/*_.VALDATA___V145*/ meltfptr[144] =
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
  meltcdat->dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE.obj_hash = 712459047;
  meltcdat->dobj_122__ALL_IPA_PASSES_START_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_123__all_ipa_passes_s*/
 /*_.VALSTR___V146*/ meltfptr[145] =
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
    /*_.VALDATA___V148*/ meltfptr[147];
  strncpy (meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START.hookname,
	   "HOOK_ALL_IPA_PASSES_START",
	   sizeof (meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START.hookname) -
	   1);
/*predefinedhook HOOK_ALL_IPA_PASSES_START for dhook__124__HOOK_ALL_IPA_PASSES_START*/
  if (!MELT_PREDEF (HOOK_ALL_IPA_PASSES_START))
    MELT_STORE_PREDEF (HOOK_ALL_IPA_PASSES_START,
		       (melt_ptr_t) & meltcdat->
		       dhook__124__HOOK_ALL_IPA_PASSES_START);

  /*_.VALHOOK___V147*/ meltfptr[146] =
  /*inihooklocvar HOOK_ALL_IPA_PASSES_START */
  (melt_ptr_t) & meltcdat->dhook__124__HOOK_ALL_IPA_PASSES_START;

  /*done inihook dhook__124__HOOK_ALL_IPA_PASSES_START */

  MELT_LOCATION ("warmelt-hooks.melt:717:/ iniobj");

/*iniobj dobj_125__*/
/*_.VALDATA___V148*/ meltfptr[147] = (melt_ptr_t) & meltcdat->dobj_125__;
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
  meltcdat->dobj_125__.obj_hash = 1065504139;
  meltcdat->dobj_125__.obj_len = 6;

  /*inimult dtup_126___HOOK_ALL_IPA_PASSES_START */
 /*_.VALTUP___V150*/ meltfptr[149] =
    (melt_ptr_t) & meltcdat->dtup_126___HOOK_ALL_IPA_PASSES_START;
  meltcdat->dtup_126___HOOK_ALL_IPA_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_126___HOOK_ALL_IPA_PASSES_START.nbval = 0;
  /*inimult dtup_127___HOOK_ALL_IPA_PASSES_START */
 /*_.VALTUP___V151*/ meltfptr[150] =
    (melt_ptr_t) & meltcdat->dtup_127___HOOK_ALL_IPA_PASSES_START;
  meltcdat->dtup_127___HOOK_ALL_IPA_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_127___HOOK_ALL_IPA_PASSES_START.nbval = 0;
  /*inirout drout_128__ENABLE_ALL_IPA_PASSES_START */
 /*_.VALROUT___V152*/ meltfptr[151] =
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
 /*_.VALCLO___V153*/ meltfptr[152] =
    (melt_ptr_t) & meltcdat->dclo_129__ENABLE_ALL_IPA_PASSES_START;
  meltcdat->dclo_129__ENABLE_ALL_IPA_PASSES_START.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_129__ENABLE_ALL_IPA_PASSES_START.nbval = 0;

  /*inirout drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST */
 /*_.VALROUT___V154*/ meltfptr[153] =
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
 /*_.VALCLO___V155*/ meltfptr[154] =
    (melt_ptr_t) & meltcdat->dclo_131__REGISTER_ALL_IPA_PASSES_START_FIRST;
  meltcdat->dclo_131__REGISTER_ALL_IPA_PASSES_START_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_131__REGISTER_ALL_IPA_PASSES_START_FIRST.nbval = 0;

  /*inirout drout_132__REGISTER_ALL_IPA_PASSES_START_LAST */
 /*_.VALROUT___V156*/ meltfptr[155] =
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
 /*_.VALCLO___V157*/ meltfptr[156] =
    (melt_ptr_t) & meltcdat->dclo_133__REGISTER_ALL_IPA_PASSES_START_LAST;
  meltcdat->dclo_133__REGISTER_ALL_IPA_PASSES_START_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_133__REGISTER_ALL_IPA_PASSES_START_LAST.nbval = 0;


/*iniobj dsym_134__ALL_IPA_PASSES_END_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V158*/ meltfptr[157])
/*_.VALDATA___V158*/ meltfptr[157] =
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
 /*_.VALSTR___V159*/ meltfptr[158] =
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
/*_.VALDATA___V160*/ meltfptr[159] =
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
  meltcdat->dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE.obj_hash = 609622823;
  meltcdat->dobj_136__ALL_IPA_PASSES_END_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_137__all_ipa_passes_e*/
 /*_.VALSTR___V161*/ meltfptr[160] =
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
    /*_.VALDATA___V163*/ meltfptr[162];
  strncpy (meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END.hookname,
	   "HOOK_ALL_IPA_PASSES_END",
	   sizeof (meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END.hookname) -
	   1);
/*predefinedhook HOOK_ALL_IPA_PASSES_END for dhook__138__HOOK_ALL_IPA_PASSES_END*/
  if (!MELT_PREDEF (HOOK_ALL_IPA_PASSES_END))
    MELT_STORE_PREDEF (HOOK_ALL_IPA_PASSES_END,
		       (melt_ptr_t) & meltcdat->
		       dhook__138__HOOK_ALL_IPA_PASSES_END);

  /*_.VALHOOK___V162*/ meltfptr[161] =
  /*inihooklocvar HOOK_ALL_IPA_PASSES_END */
  (melt_ptr_t) & meltcdat->dhook__138__HOOK_ALL_IPA_PASSES_END;

  /*done inihook dhook__138__HOOK_ALL_IPA_PASSES_END */

  MELT_LOCATION ("warmelt-hooks.melt:841:/ iniobj");

/*iniobj dobj_139__*/
/*_.VALDATA___V163*/ meltfptr[162] = (melt_ptr_t) & meltcdat->dobj_139__;
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
  meltcdat->dobj_139__.obj_hash = 625466690;
  meltcdat->dobj_139__.obj_len = 6;

  /*inimult dtup_140___HOOK_ALL_IPA_PASSES_END */
 /*_.VALTUP___V165*/ meltfptr[164] =
    (melt_ptr_t) & meltcdat->dtup_140___HOOK_ALL_IPA_PASSES_END;
  meltcdat->dtup_140___HOOK_ALL_IPA_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_140___HOOK_ALL_IPA_PASSES_END.nbval = 0;
  /*inimult dtup_141___HOOK_ALL_IPA_PASSES_END */
 /*_.VALTUP___V166*/ meltfptr[165] =
    (melt_ptr_t) & meltcdat->dtup_141___HOOK_ALL_IPA_PASSES_END;
  meltcdat->dtup_141___HOOK_ALL_IPA_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_141___HOOK_ALL_IPA_PASSES_END.nbval = 0;
  /*inirout drout_142__ENABLE_ALL_IPA_PASSES_END */
 /*_.VALROUT___V167*/ meltfptr[166] =
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
 /*_.VALCLO___V168*/ meltfptr[167] =
    (melt_ptr_t) & meltcdat->dclo_143__ENABLE_ALL_IPA_PASSES_END;
  meltcdat->dclo_143__ENABLE_ALL_IPA_PASSES_END.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_143__ENABLE_ALL_IPA_PASSES_END.nbval = 0;

  /*inirout drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST */
 /*_.VALROUT___V169*/ meltfptr[168] =
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
 /*_.VALCLO___V170*/ meltfptr[169] =
    (melt_ptr_t) & meltcdat->dclo_145__REGISTER_ALL_IPA_PASSES_END_FIRST;
  meltcdat->dclo_145__REGISTER_ALL_IPA_PASSES_END_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_145__REGISTER_ALL_IPA_PASSES_END_FIRST.nbval = 0;

  /*inirout drout_146__REGISTER_ALL_IPA_PASSES_END_LAST */
 /*_.VALROUT___V171*/ meltfptr[170] =
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
 /*_.VALCLO___V172*/ meltfptr[171] =
    (melt_ptr_t) & meltcdat->dclo_147__REGISTER_ALL_IPA_PASSES_END_LAST;
  meltcdat->dclo_147__REGISTER_ALL_IPA_PASSES_END_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_147__REGISTER_ALL_IPA_PASSES_END_LAST.nbval = 0;


/*iniobj dsym_148__AT_FINISH_UNIT_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V173*/ meltfptr[172])
/*_.VALDATA___V173*/ meltfptr[172] =
      (melt_ptr_t) & meltcdat->dsym_148__AT_FINISH_UNIT_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_148__AT_FINISH_UNIT_FIRST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_148__AT_FINISH_UNIT_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_148__AT_FINISH_UNIT_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_148__AT_FINISH_UNIT_FIRST.obj_hash = 390084996;
  meltcdat->dsym_148__AT_FINISH_UNIT_FIRST.obj_len = 3;


/*inistring dstr_149__AT_FINISH_UNIT_F*/
 /*_.VALSTR___V174*/ meltfptr[173] =
    (melt_ptr_t) & meltcdat->dstr_149__AT_FINISH_UNIT_F;
  meltcdat->dstr_149__AT_FINISH_UNIT_F.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_149__AT_FINISH_UNIT_F.val,
				"AT_FINISH_UNIT_FIRST",
				sizeof (meltcdat->dstr_149__AT_FINISH_UNIT_F.
					val) - 1);
  meltcdat->dstr_149__AT_FINISH_UNIT_F.val[20] = (char) 0;
  meltcdat->dstr_149__AT_FINISH_UNIT_F.slen = 20;


/*iniobj dsym_150__AT_FINISH_UNIT_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V175*/ meltfptr[174])
/*_.VALDATA___V175*/ meltfptr[174] =
      (melt_ptr_t) & meltcdat->dsym_150__AT_FINISH_UNIT_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_150__AT_FINISH_UNIT_LAST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_150__AT_FINISH_UNIT_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_150__AT_FINISH_UNIT_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_150__AT_FINISH_UNIT_LAST.obj_hash = 289829591;
  meltcdat->dsym_150__AT_FINISH_UNIT_LAST.obj_len = 3;


/*inistring dstr_151__AT_FINISH_UNIT_L*/
 /*_.VALSTR___V176*/ meltfptr[175] =
    (melt_ptr_t) & meltcdat->dstr_151__AT_FINISH_UNIT_L;
  meltcdat->dstr_151__AT_FINISH_UNIT_L.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_151__AT_FINISH_UNIT_L.val,
				"AT_FINISH_UNIT_LAST",
				sizeof (meltcdat->dstr_151__AT_FINISH_UNIT_L.
					val) - 1);
  meltcdat->dstr_151__AT_FINISH_UNIT_L.val[19] = (char) 0;
  meltcdat->dstr_151__AT_FINISH_UNIT_L.slen = 19;


/*iniobj dsym_152__AT_START_UNIT_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V177*/ meltfptr[176])
/*_.VALDATA___V177*/ meltfptr[176] =
      (melt_ptr_t) & meltcdat->dsym_152__AT_START_UNIT_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_152__AT_START_UNIT_FIRST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_152__AT_START_UNIT_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_152__AT_START_UNIT_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_152__AT_START_UNIT_FIRST.obj_hash = 536593345;
  meltcdat->dsym_152__AT_START_UNIT_FIRST.obj_len = 3;


/*inistring dstr_153__AT_START_UNIT_FI*/
 /*_.VALSTR___V178*/ meltfptr[177] =
    (melt_ptr_t) & meltcdat->dstr_153__AT_START_UNIT_FI;
  meltcdat->dstr_153__AT_START_UNIT_FI.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_153__AT_START_UNIT_FI.val,
				"AT_START_UNIT_FIRST",
				sizeof (meltcdat->dstr_153__AT_START_UNIT_FI.
					val) - 1);
  meltcdat->dstr_153__AT_START_UNIT_FI.val[19] = (char) 0;
  meltcdat->dstr_153__AT_START_UNIT_FI.slen = 19;


/*iniobj dsym_154__AT_START_UNIT_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V179*/ meltfptr[178])
/*_.VALDATA___V179*/ meltfptr[178] =
      (melt_ptr_t) & meltcdat->dsym_154__AT_START_UNIT_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_154__AT_START_UNIT_LAST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_154__AT_START_UNIT_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_154__AT_START_UNIT_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_154__AT_START_UNIT_LAST.obj_hash = 817460620;
  meltcdat->dsym_154__AT_START_UNIT_LAST.obj_len = 3;


/*inistring dstr_155__AT_START_UNIT_LA*/
 /*_.VALSTR___V180*/ meltfptr[179] =
    (melt_ptr_t) & meltcdat->dstr_155__AT_START_UNIT_LA;
  meltcdat->dstr_155__AT_START_UNIT_LA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_155__AT_START_UNIT_LA.val,
				"AT_START_UNIT_LAST",
				sizeof (meltcdat->dstr_155__AT_START_UNIT_LA.
					val) - 1);
  meltcdat->dstr_155__AT_START_UNIT_LA.val[18] = (char) 0;
  meltcdat->dstr_155__AT_START_UNIT_LA.slen = 18;


/*iniobj dsym_156__HOOK_OVERRIDE_GATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V181*/ meltfptr[180])
/*_.VALDATA___V181*/ meltfptr[180] =
      (melt_ptr_t) & meltcdat->dsym_156__HOOK_OVERRIDE_GATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_156__HOOK_OVERRIDE_GATE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_156__HOOK_OVERRIDE_GATE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_156__HOOK_OVERRIDE_GATE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_156__HOOK_OVERRIDE_GATE.obj_hash = 995989956;
  meltcdat->dsym_156__HOOK_OVERRIDE_GATE.obj_len = 3;


/*inistring dstr_157__HOOK_OVERRIDE_GA*/
 /*_.VALSTR___V182*/ meltfptr[181] =
    (melt_ptr_t) & meltcdat->dstr_157__HOOK_OVERRIDE_GA;
  meltcdat->dstr_157__HOOK_OVERRIDE_GA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_157__HOOK_OVERRIDE_GA.val,
				"HOOK_OVERRIDE_GATE",
				sizeof (meltcdat->dstr_157__HOOK_OVERRIDE_GA.
					val) - 1);
  meltcdat->dstr_157__HOOK_OVERRIDE_GA.val[18] = (char) 0;
  meltcdat->dstr_157__HOOK_OVERRIDE_GA.slen = 18;


/*iniobj dsym_158__HOOK_LOW_DEBUG_VALUE_AT*/
  /*uniqueobj*/ if (! /*_.VALDATA___V183*/ meltfptr[182])
/*_.VALDATA___V183*/ meltfptr[182] =
      (melt_ptr_t) & meltcdat->dsym_158__HOOK_LOW_DEBUG_VALUE_AT;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_158__HOOK_LOW_DEBUG_VALUE_AT",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_158__HOOK_LOW_DEBUG_VALUE_AT",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_158__HOOK_LOW_DEBUG_VALUE_AT.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_158__HOOK_LOW_DEBUG_VALUE_AT.obj_hash = 1048470198;
  meltcdat->dsym_158__HOOK_LOW_DEBUG_VALUE_AT.obj_len = 3;


/*inistring dstr_159__HOOK_LOW_DEBUG_V*/
 /*_.VALSTR___V184*/ meltfptr[183] =
    (melt_ptr_t) & meltcdat->dstr_159__HOOK_LOW_DEBUG_V;
  meltcdat->dstr_159__HOOK_LOW_DEBUG_V.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_159__HOOK_LOW_DEBUG_V.val,
				"HOOK_LOW_DEBUG_VALUE_AT",
				sizeof (meltcdat->dstr_159__HOOK_LOW_DEBUG_V.
					val) - 1);
  meltcdat->dstr_159__HOOK_LOW_DEBUG_V.val[23] = (char) 0;
  meltcdat->dstr_159__HOOK_LOW_DEBUG_V.slen = 23;


/*iniobj dsym_160__REGISTER_OVERRIDE_GATE_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V185*/ meltfptr[184])
/*_.VALDATA___V185*/ meltfptr[184] =
      (melt_ptr_t) & meltcdat->dsym_160__REGISTER_OVERRIDE_GATE_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_160__REGISTER_OVERRIDE_GATE_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_160__REGISTER_OVERRIDE_GATE_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_160__REGISTER_OVERRIDE_GATE_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_160__REGISTER_OVERRIDE_GATE_FIRST.obj_hash = 929826072;
  meltcdat->dsym_160__REGISTER_OVERRIDE_GATE_FIRST.obj_len = 3;


/*inistring dstr_161__REGISTER_OVERRID*/
 /*_.VALSTR___V186*/ meltfptr[185] =
    (melt_ptr_t) & meltcdat->dstr_161__REGISTER_OVERRID;
  meltcdat->dstr_161__REGISTER_OVERRID.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_161__REGISTER_OVERRID.val,
				"REGISTER_OVERRIDE_GATE_FIRST",
				sizeof (meltcdat->dstr_161__REGISTER_OVERRID.
					val) - 1);
  meltcdat->dstr_161__REGISTER_OVERRID.val[28] = (char) 0;
  meltcdat->dstr_161__REGISTER_OVERRID.slen = 28;


/*iniobj dsym_162__REGISTER_OVERRIDE_GATE_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V187*/ meltfptr[186])
/*_.VALDATA___V187*/ meltfptr[186] =
      (melt_ptr_t) & meltcdat->dsym_162__REGISTER_OVERRIDE_GATE_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_162__REGISTER_OVERRIDE_GATE_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_162__REGISTER_OVERRIDE_GATE_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_162__REGISTER_OVERRIDE_GATE_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_162__REGISTER_OVERRIDE_GATE_LAST.obj_hash = 387249375;
  meltcdat->dsym_162__REGISTER_OVERRIDE_GATE_LAST.obj_len = 3;


/*inistring dstr_163__REGISTER_OVERRID*/
 /*_.VALSTR___V188*/ meltfptr[187] =
    (melt_ptr_t) & meltcdat->dstr_163__REGISTER_OVERRID;
  meltcdat->dstr_163__REGISTER_OVERRID.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_163__REGISTER_OVERRID.val,
				"REGISTER_OVERRIDE_GATE_LAST",
				sizeof (meltcdat->dstr_163__REGISTER_OVERRID.
					val) - 1);
  meltcdat->dstr_163__REGISTER_OVERRID.val[27] = (char) 0;
  meltcdat->dstr_163__REGISTER_OVERRID.slen = 27;


/*iniobj dsym_164__REGISTER_ALL_IPA_PASSES_END_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V189*/ meltfptr[188])
/*_.VALDATA___V189*/ meltfptr[188] =
      (melt_ptr_t) & meltcdat->dsym_164__REGISTER_ALL_IPA_PASSES_END_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_164__REGISTER_ALL_IPA_PASSES_END_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_164__REGISTER_ALL_IPA_PASSES_END_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_164__REGISTER_ALL_IPA_PASSES_END_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_164__REGISTER_ALL_IPA_PASSES_END_FIRST.obj_hash = 188504921;
  meltcdat->dsym_164__REGISTER_ALL_IPA_PASSES_END_FIRST.obj_len = 3;


/*inistring dstr_165__REGISTER_ALL_IPA*/
 /*_.VALSTR___V190*/ meltfptr[189] =
    (melt_ptr_t) & meltcdat->dstr_165__REGISTER_ALL_IPA;
  meltcdat->dstr_165__REGISTER_ALL_IPA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_165__REGISTER_ALL_IPA.val,
				"REGISTER_ALL_IPA_PASSES_END_FIRST",
				sizeof (meltcdat->dstr_165__REGISTER_ALL_IPA.
					val) - 1);
  meltcdat->dstr_165__REGISTER_ALL_IPA.val[33] = (char) 0;
  meltcdat->dstr_165__REGISTER_ALL_IPA.slen = 33;


/*iniobj dsym_166__REGISTER_ALL_IPA_PASSES_END_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V191*/ meltfptr[190])
/*_.VALDATA___V191*/ meltfptr[190] =
      (melt_ptr_t) & meltcdat->dsym_166__REGISTER_ALL_IPA_PASSES_END_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_166__REGISTER_ALL_IPA_PASSES_END_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_166__REGISTER_ALL_IPA_PASSES_END_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_166__REGISTER_ALL_IPA_PASSES_END_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_166__REGISTER_ALL_IPA_PASSES_END_LAST.obj_hash = 822070917;
  meltcdat->dsym_166__REGISTER_ALL_IPA_PASSES_END_LAST.obj_len = 3;


/*inistring dstr_167__REGISTER_ALL_IPA*/
 /*_.VALSTR___V192*/ meltfptr[191] =
    (melt_ptr_t) & meltcdat->dstr_167__REGISTER_ALL_IPA;
  meltcdat->dstr_167__REGISTER_ALL_IPA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_167__REGISTER_ALL_IPA.val,
				"REGISTER_ALL_IPA_PASSES_END_LAST",
				sizeof (meltcdat->dstr_167__REGISTER_ALL_IPA.
					val) - 1);
  meltcdat->dstr_167__REGISTER_ALL_IPA.val[32] = (char) 0;
  meltcdat->dstr_167__REGISTER_ALL_IPA.slen = 32;


/*iniobj dsym_168__REGISTER_ALL_IPA_PASSES_START_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V193*/ meltfptr[192])
/*_.VALDATA___V193*/ meltfptr[192] =
      (melt_ptr_t) & meltcdat->dsym_168__REGISTER_ALL_IPA_PASSES_START_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_168__REGISTER_ALL_IPA_PASSES_START_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_168__REGISTER_ALL_IPA_PASSES_START_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_168__REGISTER_ALL_IPA_PASSES_START_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_168__REGISTER_ALL_IPA_PASSES_START_FIRST.obj_hash = 87049035;
  meltcdat->dsym_168__REGISTER_ALL_IPA_PASSES_START_FIRST.obj_len = 3;


/*inistring dstr_169__REGISTER_ALL_IPA*/
 /*_.VALSTR___V194*/ meltfptr[193] =
    (melt_ptr_t) & meltcdat->dstr_169__REGISTER_ALL_IPA;
  meltcdat->dstr_169__REGISTER_ALL_IPA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_169__REGISTER_ALL_IPA.val,
				"REGISTER_ALL_IPA_PASSES_START_FIRST",
				sizeof (meltcdat->dstr_169__REGISTER_ALL_IPA.
					val) - 1);
  meltcdat->dstr_169__REGISTER_ALL_IPA.val[35] = (char) 0;
  meltcdat->dstr_169__REGISTER_ALL_IPA.slen = 35;


/*iniobj dsym_170__REGISTER_ALL_IPA_PASSES_START_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V195*/ meltfptr[194])
/*_.VALDATA___V195*/ meltfptr[194] =
      (melt_ptr_t) & meltcdat->dsym_170__REGISTER_ALL_IPA_PASSES_START_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_170__REGISTER_ALL_IPA_PASSES_START_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_170__REGISTER_ALL_IPA_PASSES_START_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_170__REGISTER_ALL_IPA_PASSES_START_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_170__REGISTER_ALL_IPA_PASSES_START_LAST.obj_hash = 169109429;
  meltcdat->dsym_170__REGISTER_ALL_IPA_PASSES_START_LAST.obj_len = 3;


/*inistring dstr_171__REGISTER_ALL_IPA*/
 /*_.VALSTR___V196*/ meltfptr[195] =
    (melt_ptr_t) & meltcdat->dstr_171__REGISTER_ALL_IPA;
  meltcdat->dstr_171__REGISTER_ALL_IPA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_171__REGISTER_ALL_IPA.val,
				"REGISTER_ALL_IPA_PASSES_START_LAST",
				sizeof (meltcdat->dstr_171__REGISTER_ALL_IPA.
					val) - 1);
  meltcdat->dstr_171__REGISTER_ALL_IPA.val[34] = (char) 0;
  meltcdat->dstr_171__REGISTER_ALL_IPA.slen = 34;


/*iniobj dsym_172__REGISTER_ALL_PASSES_END_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V197*/ meltfptr[196])
/*_.VALDATA___V197*/ meltfptr[196] =
      (melt_ptr_t) & meltcdat->dsym_172__REGISTER_ALL_PASSES_END_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_172__REGISTER_ALL_PASSES_END_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_172__REGISTER_ALL_PASSES_END_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_172__REGISTER_ALL_PASSES_END_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_172__REGISTER_ALL_PASSES_END_FIRST.obj_hash = 377865029;
  meltcdat->dsym_172__REGISTER_ALL_PASSES_END_FIRST.obj_len = 3;


/*inistring dstr_173__REGISTER_ALL_PAS*/
 /*_.VALSTR___V198*/ meltfptr[197] =
    (melt_ptr_t) & meltcdat->dstr_173__REGISTER_ALL_PAS;
  meltcdat->dstr_173__REGISTER_ALL_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_173__REGISTER_ALL_PAS.val,
				"REGISTER_ALL_PASSES_END_FIRST",
				sizeof (meltcdat->dstr_173__REGISTER_ALL_PAS.
					val) - 1);
  meltcdat->dstr_173__REGISTER_ALL_PAS.val[29] = (char) 0;
  meltcdat->dstr_173__REGISTER_ALL_PAS.slen = 29;


/*iniobj dsym_174__REGISTER_ALL_PASSES_END_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V199*/ meltfptr[198])
/*_.VALDATA___V199*/ meltfptr[198] =
      (melt_ptr_t) & meltcdat->dsym_174__REGISTER_ALL_PASSES_END_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_174__REGISTER_ALL_PASSES_END_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_174__REGISTER_ALL_PASSES_END_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_174__REGISTER_ALL_PASSES_END_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_174__REGISTER_ALL_PASSES_END_LAST.obj_hash = 469287204;
  meltcdat->dsym_174__REGISTER_ALL_PASSES_END_LAST.obj_len = 3;


/*inistring dstr_175__REGISTER_ALL_PAS*/
 /*_.VALSTR___V200*/ meltfptr[199] =
    (melt_ptr_t) & meltcdat->dstr_175__REGISTER_ALL_PAS;
  meltcdat->dstr_175__REGISTER_ALL_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_175__REGISTER_ALL_PAS.val,
				"REGISTER_ALL_PASSES_END_LAST",
				sizeof (meltcdat->dstr_175__REGISTER_ALL_PAS.
					val) - 1);
  meltcdat->dstr_175__REGISTER_ALL_PAS.val[28] = (char) 0;
  meltcdat->dstr_175__REGISTER_ALL_PAS.slen = 28;


/*iniobj dsym_176__REGISTER_ALL_PASSES_START_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V201*/ meltfptr[200])
/*_.VALDATA___V201*/ meltfptr[200] =
      (melt_ptr_t) & meltcdat->dsym_176__REGISTER_ALL_PASSES_START_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_176__REGISTER_ALL_PASSES_START_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_176__REGISTER_ALL_PASSES_START_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_176__REGISTER_ALL_PASSES_START_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_176__REGISTER_ALL_PASSES_START_FIRST.obj_hash = 861337342;
  meltcdat->dsym_176__REGISTER_ALL_PASSES_START_FIRST.obj_len = 3;


/*inistring dstr_177__REGISTER_ALL_PAS*/
 /*_.VALSTR___V202*/ meltfptr[201] =
    (melt_ptr_t) & meltcdat->dstr_177__REGISTER_ALL_PAS;
  meltcdat->dstr_177__REGISTER_ALL_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_177__REGISTER_ALL_PAS.val,
				"REGISTER_ALL_PASSES_START_FIRST",
				sizeof (meltcdat->dstr_177__REGISTER_ALL_PAS.
					val) - 1);
  meltcdat->dstr_177__REGISTER_ALL_PAS.val[31] = (char) 0;
  meltcdat->dstr_177__REGISTER_ALL_PAS.slen = 31;


/*iniobj dsym_178__REGISTER_ALL_PASSES_START_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V203*/ meltfptr[202])
/*_.VALDATA___V203*/ meltfptr[202] =
      (melt_ptr_t) & meltcdat->dsym_178__REGISTER_ALL_PASSES_START_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_178__REGISTER_ALL_PASSES_START_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_178__REGISTER_ALL_PASSES_START_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_178__REGISTER_ALL_PASSES_START_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_178__REGISTER_ALL_PASSES_START_LAST.obj_hash = 428184515;
  meltcdat->dsym_178__REGISTER_ALL_PASSES_START_LAST.obj_len = 3;


/*inistring dstr_179__REGISTER_ALL_PAS*/
 /*_.VALSTR___V204*/ meltfptr[203] =
    (melt_ptr_t) & meltcdat->dstr_179__REGISTER_ALL_PAS;
  meltcdat->dstr_179__REGISTER_ALL_PAS.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_179__REGISTER_ALL_PAS.val,
				"REGISTER_ALL_PASSES_START_LAST",
				sizeof (meltcdat->dstr_179__REGISTER_ALL_PAS.
					val) - 1);
  meltcdat->dstr_179__REGISTER_ALL_PAS.val[30] = (char) 0;
  meltcdat->dstr_179__REGISTER_ALL_PAS.slen = 30;


/*iniobj dsym_180__UNREGISTER_OVERRIDE_GATE_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V205*/ meltfptr[204])
/*_.VALDATA___V205*/ meltfptr[204] =
      (melt_ptr_t) & meltcdat->dsym_180__UNREGISTER_OVERRIDE_GATE_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_180__UNREGISTER_OVERRIDE_GATE_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_180__UNREGISTER_OVERRIDE_GATE_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_180__UNREGISTER_OVERRIDE_GATE_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_180__UNREGISTER_OVERRIDE_GATE_FIRST.obj_hash = 604111710;
  meltcdat->dsym_180__UNREGISTER_OVERRIDE_GATE_FIRST.obj_len = 3;


/*inistring dstr_181__UNREGISTER_OVERR*/
 /*_.VALSTR___V206*/ meltfptr[205] =
    (melt_ptr_t) & meltcdat->dstr_181__UNREGISTER_OVERR;
  meltcdat->dstr_181__UNREGISTER_OVERR.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_181__UNREGISTER_OVERR.val,
				"UNREGISTER_OVERRIDE_GATE_FIRST",
				sizeof (meltcdat->dstr_181__UNREGISTER_OVERR.
					val) - 1);
  meltcdat->dstr_181__UNREGISTER_OVERR.val[30] = (char) 0;
  meltcdat->dstr_181__UNREGISTER_OVERR.slen = 30;


/*iniobj dsym_182__UNREGISTER_OVERRIDE_GATE_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V207*/ meltfptr[206])
/*_.VALDATA___V207*/ meltfptr[206] =
      (melt_ptr_t) & meltcdat->dsym_182__UNREGISTER_OVERRIDE_GATE_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_182__UNREGISTER_OVERRIDE_GATE_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_182__UNREGISTER_OVERRIDE_GATE_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_182__UNREGISTER_OVERRIDE_GATE_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_182__UNREGISTER_OVERRIDE_GATE_LAST.obj_hash = 458707543;
  meltcdat->dsym_182__UNREGISTER_OVERRIDE_GATE_LAST.obj_len = 3;


/*inistring dstr_183__UNREGISTER_OVERR*/
 /*_.VALSTR___V208*/ meltfptr[207] =
    (melt_ptr_t) & meltcdat->dstr_183__UNREGISTER_OVERR;
  meltcdat->dstr_183__UNREGISTER_OVERR.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_183__UNREGISTER_OVERR.val,
				"UNREGISTER_OVERRIDE_GATE_LAST",
				sizeof (meltcdat->dstr_183__UNREGISTER_OVERR.
					val) - 1);
  meltcdat->dstr_183__UNREGISTER_OVERR.val[29] = (char) 0;
  meltcdat->dstr_183__UNREGISTER_OVERR.slen = 29;
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
/* declstructinit initial routine melt_start_this_module minihash 2218*/

  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
/**initial routine prologue**/
  /* set initial frame marking */
  ((struct melt_callframe_st *) &meltfram__)->mcfr_nbvar =
    /*minihash */ -2218;
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

  if (NULL == /*_.VALDATA___V18*/ meltfptr[17])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol VAL without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_VAL =
	melthookproc_HOOK_NAMED_SYMBOL ("VAL", (long) MELT_GET);
   /*_.VALDATA___V18*/ meltfptr[17] = (melt_ptr_t) meltsy_VAL;
    }				/*endgetnamedsym VAL */


  /*^getnamedsymbol */
  /*getnamedsym:FILENAME */

  if (NULL == /*_.VALDATA___V21*/ meltfptr[20])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol FILENAME without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_FILENAME =
	melthookproc_HOOK_NAMED_SYMBOL ("FILENAME", (long) MELT_GET);
   /*_.VALDATA___V21*/ meltfptr[20] = (melt_ptr_t) meltsy_FILENAME;
    }				/*endgetnamedsym FILENAME */


  /*^getnamedsymbol */
  /*getnamedsym:LINENO */

  if (NULL == /*_.VALDATA___V24*/ meltfptr[23])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol LINENO without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_LINENO =
	melthookproc_HOOK_NAMED_SYMBOL ("LINENO", (long) MELT_GET);
   /*_.VALDATA___V24*/ meltfptr[23] = (melt_ptr_t) meltsy_LINENO;
    }				/*endgetnamedsym LINENO */


  /*^getnamedsymbol */
  /*getnamedsym:MSG */

  if (NULL == /*_.VALDATA___V27*/ meltfptr[26])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MSG without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_MSG =
	melthookproc_HOOK_NAMED_SYMBOL ("MSG", (long) MELT_GET);
   /*_.VALDATA___V27*/ meltfptr[26] = (melt_ptr_t) meltsy_MSG;
    }				/*endgetnamedsym MSG */


  /*^getnamedsymbol */
  /*getnamedsym:COUNT */

  if (NULL == /*_.VALDATA___V30*/ meltfptr[29])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol COUNT without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_COUNT =
	melthookproc_HOOK_NAMED_SYMBOL ("COUNT", (long) MELT_GET);
   /*_.VALDATA___V30*/ meltfptr[29] = (melt_ptr_t) meltsy_COUNT;
    }				/*endgetnamedsym COUNT */


  /*^getnamedsymbol */
  /*getnamedsym:MELT_DEBUG_FUN */

  if (NULL == /*_.VALDATA___V13*/ meltfptr[12])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MELT_DEBUG_FUN without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_MELT_DEBUG_FUN =
	melthookproc_HOOK_NAMED_SYMBOL ("MELT_DEBUG_FUN", (long) MELT_GET);
   /*_.VALDATA___V13*/ meltfptr[12] = (melt_ptr_t) meltsy_MELT_DEBUG_FUN;
    }				/*endgetnamedsym MELT_DEBUG_FUN */


  /*^getnamedsymbol */
  /*getnamedsym:OVERRIDE_GATE_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V34*/ meltfptr[33])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol OVERRIDE_GATE_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_OVERRIDE_GATE_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("OVERRIDE_GATE_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V34*/ meltfptr[33] =
	(melt_ptr_t) meltsy_OVERRIDE_GATE_DELAYED_QUEUE;
    }				/*endgetnamedsym OVERRIDE_GATE_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:CLASS_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V36*/ meltfptr[35])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_CLASS_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("CLASS_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V36*/ meltfptr[35] =
	(melt_ptr_t) meltsy_CLASS_DELAYED_QUEUE;
    }				/*endgetnamedsym CLASS_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:DISCR_LIST */

  if (NULL == /*_.VALDATA___V42*/ meltfptr[41])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol DISCR_LIST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_DISCR_LIST =
	melthookproc_HOOK_NAMED_SYMBOL ("DISCR_LIST", (long) MELT_GET);
   /*_.VALDATA___V42*/ meltfptr[41] = (melt_ptr_t) meltsy_DISCR_LIST;
    }				/*endgetnamedsym DISCR_LIST */


  /*^getnamedsymbol */
  /*getnamedsym:BEFOREGATE */

  if (NULL == /*_.VALDATA___V52*/ meltfptr[51])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol BEFOREGATE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_BEFOREGATE =
	melthookproc_HOOK_NAMED_SYMBOL ("BEFOREGATE", (long) MELT_GET);
   /*_.VALDATA___V52*/ meltfptr[51] = (melt_ptr_t) meltsy_BEFOREGATE;
    }				/*endgetnamedsym BEFOREGATE */


  /*^getnamedsymbol */
  /*getnamedsym:AFTERGATE */

  if (NULL == /*_.VALDATA___V56*/ meltfptr[55])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AFTERGATE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AFTERGATE =
	melthookproc_HOOK_NAMED_SYMBOL ("AFTERGATE", (long) MELT_GET);
   /*_.VALDATA___V56*/ meltfptr[55] = (melt_ptr_t) meltsy_AFTERGATE;
    }				/*endgetnamedsym AFTERGATE */


  /*^getnamedkeyword */
  /*getnamedkeyw:TRUE */
  {
    melt_ptr_t kw_TRUE =
      melthookproc_HOOK_NAMED_KEYWORD ("TRUE", (long) MELT_GET);
    if (kw_TRUE)
	       /*_.VALDATA___V45*/
      meltfptr[44] = (melt_ptr_t) kw_TRUE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:CLASS_REFERENCE */

  if (NULL == /*_.VALDATA___V47*/ meltfptr[46])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_REFERENCE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_CLASS_REFERENCE =
	melthookproc_HOOK_NAMED_SYMBOL ("CLASS_REFERENCE", (long) MELT_GET);
   /*_.VALDATA___V47*/ meltfptr[46] = (melt_ptr_t) meltsy_CLASS_REFERENCE;
    }				/*endgetnamedsym CLASS_REFERENCE */


  /*^getnamedsymbol */
  /*getnamedsym:START_UNIT_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V71*/ meltfptr[70])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol START_UNIT_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_START_UNIT_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("START_UNIT_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V71*/ meltfptr[70] =
	(melt_ptr_t) meltsy_START_UNIT_DELAYED_QUEUE;
    }				/*endgetnamedsym START_UNIT_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:LIST_EVERY */

  if (NULL == /*_.VALDATA___V78*/ meltfptr[77])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol LIST_EVERY without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_LIST_EVERY =
	melthookproc_HOOK_NAMED_SYMBOL ("LIST_EVERY", (long) MELT_GET);
   /*_.VALDATA___V78*/ meltfptr[77] = (melt_ptr_t) meltsy_LIST_EVERY;
    }				/*endgetnamedsym LIST_EVERY */


  /*^getnamedsymbol */
  /*getnamedsym:FINISH_UNIT_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V93*/ meltfptr[92])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol FINISH_UNIT_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_FINISH_UNIT_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("FINISH_UNIT_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V93*/ meltfptr[92] =
	(melt_ptr_t) meltsy_FINISH_UNIT_DELAYED_QUEUE;
    }				/*endgetnamedsym FINISH_UNIT_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:ALL_PASSES_START_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V113*/ meltfptr[112])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol ALL_PASSES_START_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_ALL_PASSES_START_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("ALL_PASSES_START_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V113*/ meltfptr[112] =
	(melt_ptr_t) meltsy_ALL_PASSES_START_DELAYED_QUEUE;
    }				/*endgetnamedsym ALL_PASSES_START_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:ALL_PASSES_END_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V128*/ meltfptr[127])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol ALL_PASSES_END_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_ALL_PASSES_END_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("ALL_PASSES_END_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V128*/ meltfptr[127] =
	(melt_ptr_t) meltsy_ALL_PASSES_END_DELAYED_QUEUE;
    }				/*endgetnamedsym ALL_PASSES_END_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:ALL_IPA_PASSES_START_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V143*/ meltfptr[142])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol ALL_IPA_PASSES_START_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_ALL_IPA_PASSES_START_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("ALL_IPA_PASSES_START_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V143*/ meltfptr[142] =
	(melt_ptr_t) meltsy_ALL_IPA_PASSES_START_DELAYED_QUEUE;
    }				/*endgetnamedsym ALL_IPA_PASSES_START_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:ALL_IPA_PASSES_END_DELAYED_QUEUE */

  if (NULL == /*_.VALDATA___V158*/ meltfptr[157])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol ALL_IPA_PASSES_END_DELAYED_QUEUE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_ALL_IPA_PASSES_END_DELAYED_QUEUE =
	melthookproc_HOOK_NAMED_SYMBOL ("ALL_IPA_PASSES_END_DELAYED_QUEUE",
					(long) MELT_GET);
   /*_.VALDATA___V158*/ meltfptr[157] =
	(melt_ptr_t) meltsy_ALL_IPA_PASSES_END_DELAYED_QUEUE;
    }				/*endgetnamedsym ALL_IPA_PASSES_END_DELAYED_QUEUE */


  /*^getnamedsymbol */
  /*getnamedsym:AT_FINISH_UNIT_FIRST */

  if (NULL == /*_.VALDATA___V173*/ meltfptr[172])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AT_FINISH_UNIT_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AT_FINISH_UNIT_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("AT_FINISH_UNIT_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V173*/ meltfptr[172] =
	(melt_ptr_t) meltsy_AT_FINISH_UNIT_FIRST;
    }				/*endgetnamedsym AT_FINISH_UNIT_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:AT_FINISH_UNIT_LAST */

  if (NULL == /*_.VALDATA___V175*/ meltfptr[174])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AT_FINISH_UNIT_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AT_FINISH_UNIT_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("AT_FINISH_UNIT_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V175*/ meltfptr[174] =
	(melt_ptr_t) meltsy_AT_FINISH_UNIT_LAST;
    }				/*endgetnamedsym AT_FINISH_UNIT_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:AT_START_UNIT_FIRST */

  if (NULL == /*_.VALDATA___V177*/ meltfptr[176])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AT_START_UNIT_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AT_START_UNIT_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("AT_START_UNIT_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V177*/ meltfptr[176] =
	(melt_ptr_t) meltsy_AT_START_UNIT_FIRST;
    }				/*endgetnamedsym AT_START_UNIT_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:AT_START_UNIT_LAST */

  if (NULL == /*_.VALDATA___V179*/ meltfptr[178])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol AT_START_UNIT_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_AT_START_UNIT_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("AT_START_UNIT_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V179*/ meltfptr[178] =
	(melt_ptr_t) meltsy_AT_START_UNIT_LAST;
    }				/*endgetnamedsym AT_START_UNIT_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:HOOK_OVERRIDE_GATE */

  if (NULL == /*_.VALDATA___V181*/ meltfptr[180])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol HOOK_OVERRIDE_GATE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_HOOK_OVERRIDE_GATE =
	melthookproc_HOOK_NAMED_SYMBOL ("HOOK_OVERRIDE_GATE",
					(long) MELT_GET);
   /*_.VALDATA___V181*/ meltfptr[180] =
	(melt_ptr_t) meltsy_HOOK_OVERRIDE_GATE;
    }				/*endgetnamedsym HOOK_OVERRIDE_GATE */


  /*^getnamedsymbol */
  /*getnamedsym:HOOK_LOW_DEBUG_VALUE_AT */

  if (NULL == /*_.VALDATA___V183*/ meltfptr[182])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol HOOK_LOW_DEBUG_VALUE_AT without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_HOOK_LOW_DEBUG_VALUE_AT =
	melthookproc_HOOK_NAMED_SYMBOL ("HOOK_LOW_DEBUG_VALUE_AT",
					(long) MELT_GET);
   /*_.VALDATA___V183*/ meltfptr[182] =
	(melt_ptr_t) meltsy_HOOK_LOW_DEBUG_VALUE_AT;
    }				/*endgetnamedsym HOOK_LOW_DEBUG_VALUE_AT */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_OVERRIDE_GATE_FIRST */

  if (NULL == /*_.VALDATA___V185*/ meltfptr[184])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_OVERRIDE_GATE_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_OVERRIDE_GATE_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_OVERRIDE_GATE_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V185*/ meltfptr[184] =
	(melt_ptr_t) meltsy_REGISTER_OVERRIDE_GATE_FIRST;
    }				/*endgetnamedsym REGISTER_OVERRIDE_GATE_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_OVERRIDE_GATE_LAST */

  if (NULL == /*_.VALDATA___V187*/ meltfptr[186])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_OVERRIDE_GATE_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_OVERRIDE_GATE_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_OVERRIDE_GATE_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V187*/ meltfptr[186] =
	(melt_ptr_t) meltsy_REGISTER_OVERRIDE_GATE_LAST;
    }				/*endgetnamedsym REGISTER_OVERRIDE_GATE_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_IPA_PASSES_END_FIRST */

  if (NULL == /*_.VALDATA___V189*/ meltfptr[188])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_IPA_PASSES_END_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_IPA_PASSES_END_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_IPA_PASSES_END_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V189*/ meltfptr[188] =
	(melt_ptr_t) meltsy_REGISTER_ALL_IPA_PASSES_END_FIRST;
    }				/*endgetnamedsym REGISTER_ALL_IPA_PASSES_END_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_IPA_PASSES_END_LAST */

  if (NULL == /*_.VALDATA___V191*/ meltfptr[190])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_IPA_PASSES_END_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_IPA_PASSES_END_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_IPA_PASSES_END_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V191*/ meltfptr[190] =
	(melt_ptr_t) meltsy_REGISTER_ALL_IPA_PASSES_END_LAST;
    }				/*endgetnamedsym REGISTER_ALL_IPA_PASSES_END_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_IPA_PASSES_START_FIRST */

  if (NULL == /*_.VALDATA___V193*/ meltfptr[192])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_IPA_PASSES_START_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_IPA_PASSES_START_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_IPA_PASSES_START_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V193*/ meltfptr[192] =
	(melt_ptr_t) meltsy_REGISTER_ALL_IPA_PASSES_START_FIRST;
    }				/*endgetnamedsym REGISTER_ALL_IPA_PASSES_START_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_IPA_PASSES_START_LAST */

  if (NULL == /*_.VALDATA___V195*/ meltfptr[194])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_IPA_PASSES_START_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_IPA_PASSES_START_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_IPA_PASSES_START_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V195*/ meltfptr[194] =
	(melt_ptr_t) meltsy_REGISTER_ALL_IPA_PASSES_START_LAST;
    }				/*endgetnamedsym REGISTER_ALL_IPA_PASSES_START_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_PASSES_END_FIRST */

  if (NULL == /*_.VALDATA___V197*/ meltfptr[196])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_PASSES_END_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_PASSES_END_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_PASSES_END_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V197*/ meltfptr[196] =
	(melt_ptr_t) meltsy_REGISTER_ALL_PASSES_END_FIRST;
    }				/*endgetnamedsym REGISTER_ALL_PASSES_END_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_PASSES_END_LAST */

  if (NULL == /*_.VALDATA___V199*/ meltfptr[198])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_PASSES_END_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_PASSES_END_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_PASSES_END_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V199*/ meltfptr[198] =
	(melt_ptr_t) meltsy_REGISTER_ALL_PASSES_END_LAST;
    }				/*endgetnamedsym REGISTER_ALL_PASSES_END_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_PASSES_START_FIRST */

  if (NULL == /*_.VALDATA___V201*/ meltfptr[200])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_PASSES_START_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_PASSES_START_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_PASSES_START_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V201*/ meltfptr[200] =
	(melt_ptr_t) meltsy_REGISTER_ALL_PASSES_START_FIRST;
    }				/*endgetnamedsym REGISTER_ALL_PASSES_START_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_ALL_PASSES_START_LAST */

  if (NULL == /*_.VALDATA___V203*/ meltfptr[202])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol REGISTER_ALL_PASSES_START_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_REGISTER_ALL_PASSES_START_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("REGISTER_ALL_PASSES_START_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V203*/ meltfptr[202] =
	(melt_ptr_t) meltsy_REGISTER_ALL_PASSES_START_LAST;
    }				/*endgetnamedsym REGISTER_ALL_PASSES_START_LAST */


  /*^getnamedsymbol */
  /*getnamedsym:UNREGISTER_OVERRIDE_GATE_FIRST */

  if (NULL == /*_.VALDATA___V205*/ meltfptr[204])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol UNREGISTER_OVERRIDE_GATE_FIRST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_UNREGISTER_OVERRIDE_GATE_FIRST =
	melthookproc_HOOK_NAMED_SYMBOL ("UNREGISTER_OVERRIDE_GATE_FIRST",
					(long) MELT_GET);
   /*_.VALDATA___V205*/ meltfptr[204] =
	(melt_ptr_t) meltsy_UNREGISTER_OVERRIDE_GATE_FIRST;
    }				/*endgetnamedsym UNREGISTER_OVERRIDE_GATE_FIRST */


  /*^getnamedsymbol */
  /*getnamedsym:UNREGISTER_OVERRIDE_GATE_LAST */

  if (NULL == /*_.VALDATA___V207*/ meltfptr[206])
    {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol UNREGISTER_OVERRIDE_GATE_LAST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT */
      melt_ptr_t meltsy_UNREGISTER_OVERRIDE_GATE_LAST =
	melthookproc_HOOK_NAMED_SYMBOL ("UNREGISTER_OVERRIDE_GATE_LAST",
					(long) MELT_GET);
   /*_.VALDATA___V207*/ meltfptr[206] =
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
		  (melt_ptr_t) ( /*_.VALDATA___V13*/ meltfptr[12]),
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
		  (melt_ptr_t) ( /*_.VALDATA___V36*/ meltfptr[35]),
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
    argtab[1].meltbp_cstring = "CLASS_REFERENCE";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.CLASS_REFERENCE__V10*/ meltfptr[9] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V47*/ meltfptr[46]),
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
		  (melt_ptr_t) ( /*_.VALDATA___V78*/ meltfptr[77]),
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
  meltmod__warmelt_hooks__initialmeltchunk_3 (&meltfram__, meltpredefinited);;
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
				    ( /*_.VALDATA___V13*/ meltfptr[12])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V13*/ meltfptr[12]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V13*/ meltfptr[12]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V14*/ meltfptr[13]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V13*/ meltfptr[12]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V13*/ meltfptr[12],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V15*/ meltfptr[14])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V15*/ meltfptr[14]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V15*/ meltfptr[14]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V16*/ meltfptr[15]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V18*/ meltfptr[17])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V18*/ meltfptr[17]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V18*/ meltfptr[17]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V19*/ meltfptr[18]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V18*/ meltfptr[17]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V18*/ meltfptr[17],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V17*/ meltfptr[16])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V17*/ meltfptr[16]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V17*/ meltfptr[16]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V18*/ meltfptr[17]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V17*/ meltfptr[16])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V17*/ meltfptr[16]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V17*/ meltfptr[16]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VALUE))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V17*/ meltfptr[16]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V17*/ meltfptr[16],
				"new static instance");

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

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V21*/ meltfptr[20]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V21*/ meltfptr[20],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V20*/ meltfptr[19])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V20*/ meltfptr[19]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V20*/ meltfptr[19]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V21*/ meltfptr[20]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V20*/ meltfptr[19])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V20*/ meltfptr[19]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V20*/ meltfptr[19]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_CSTRING))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V20*/ meltfptr[19]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V20*/ meltfptr[19],
				"new static instance");

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
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

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

  /*putupl#1 */
  melt_assertmsg ("putupl #1 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V32*/ meltfptr[31])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #1 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V32*/
					  meltfptr[31]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V32*/ meltfptr[31]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V17*/ meltfptr[16]);

  /*putupl#2 */
  melt_assertmsg ("putupl #2 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V32*/ meltfptr[31])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #2 checkoff",
		  (1 >= 0
		   && 1 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V32*/
					  meltfptr[31]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V32*/ meltfptr[31]))->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V20*/ meltfptr[19]);

  /*putupl#3 */
  melt_assertmsg ("putupl #3 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V32*/ meltfptr[31])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #3 checkoff",
		  (2 >= 0
		   && 2 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V32*/
					  meltfptr[31]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V32*/ meltfptr[31]))->tabval[2] =
    (melt_ptr_t) ( /*_.VALDATA___V23*/ meltfptr[22]);

  /*putupl#4 */
  melt_assertmsg ("putupl #4 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V32*/ meltfptr[31])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #4 checkoff",
		  (3 >= 0
		   && 3 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V32*/
					  meltfptr[31]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V32*/ meltfptr[31]))->tabval[3] =
    (melt_ptr_t) ( /*_.VALDATA___V26*/ meltfptr[25]);

  /*putupl#5 */
  melt_assertmsg ("putupl #5 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V32*/ meltfptr[31])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #5 checkoff",
		  (4 >= 0
		   && 4 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V32*/
					  meltfptr[31]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V32*/ meltfptr[31]))->tabval[4] =
    (melt_ptr_t) ( /*_.VALDATA___V29*/ meltfptr[28]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V32*/ meltfptr[31]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V15*/ meltfptr[14])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V15*/ meltfptr[14]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V15*/ meltfptr[14]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V32*/ meltfptr[31]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V33*/ meltfptr[32]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V15*/ meltfptr[14])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V15*/ meltfptr[14]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V15*/ meltfptr[14]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V33*/ meltfptr[32]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V15*/ meltfptr[14])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V15*/ meltfptr[14]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V15*/ meltfptr[14]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V15*/ meltfptr[14]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V15*/ meltfptr[14],
				"new static instance");

  /*put data in hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookdata checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V12*/ meltfptr[11])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V12*/ meltfptr[11])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V15*/ meltfptr[14]);

  /*put inside hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookconst checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V12*/
				  meltfptr[11]));
  ((melthook_ptr_t) /*_.VALHOOK___V12*/ meltfptr[11])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*put inside hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookconst checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V12*/
				  meltfptr[11]));
  ((melthook_ptr_t) /*_.VALHOOK___V12*/ meltfptr[11])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V13*/ meltfptr[12]);

  /*put inside hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookconst checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V12*/
				  meltfptr[11]));
  ((melthook_ptr_t) /*_.VALHOOK___V12*/ meltfptr[11])->tabval[2] =
    (melt_ptr_t) ( /*_.VALSTR___V14*/ meltfptr[13]);

  /*touch:HOOK_LOW_DEBUG_VALUE_AT */
  meltgc_touch ( /*_.VALHOOK___V12*/ meltfptr[11]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V34*/ meltfptr[33])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V34*/ meltfptr[33]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V34*/ meltfptr[33]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V35*/ meltfptr[34]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V34*/ meltfptr[33]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V34*/ meltfptr[33],
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

 /*_.MAKE_LIST__V39*/ meltfptr[38] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
 /*_.MAKE_LIST__V40*/ meltfptr[39] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V38*/ meltfptr[37])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V38*/ meltfptr[37]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V38*/ meltfptr[37]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V41*/ meltfptr[40]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V38*/ meltfptr[37])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V38*/ meltfptr[37]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V38*/ meltfptr[37]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V39*/ meltfptr[38]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V38*/ meltfptr[37])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V38*/ meltfptr[37]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V38*/ meltfptr[37]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V40*/ meltfptr[39]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V38*/ meltfptr[37]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V38*/ meltfptr[37],
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

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V45*/ meltfptr[44])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V45*/ meltfptr[44]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V45*/ meltfptr[44]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V46*/ meltfptr[45]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V45*/ meltfptr[44]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V45*/ meltfptr[44],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V47*/ meltfptr[46])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V47*/ meltfptr[46]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V47*/ meltfptr[46]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V48*/ meltfptr[47]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V47*/ meltfptr[46]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V47*/ meltfptr[46],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V49*/ meltfptr[48])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V49*/ meltfptr[48]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V49*/ meltfptr[48]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V50*/ meltfptr[49]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V52*/ meltfptr[51])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V52*/ meltfptr[51]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V52*/ meltfptr[51]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V53*/ meltfptr[52]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V52*/ meltfptr[51]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V52*/ meltfptr[51],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V51*/ meltfptr[50])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V51*/ meltfptr[50]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V51*/ meltfptr[50]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V52*/ meltfptr[51]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
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
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V51*/ meltfptr[50]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V51*/ meltfptr[50],
				"new static instance");

  /*putupl#6 */
  melt_assertmsg ("putupl #6 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V54*/ meltfptr[53])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #6 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V54*/
					  meltfptr[53]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V54*/ meltfptr[53]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V51*/ meltfptr[50]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V54*/ meltfptr[53]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V49*/ meltfptr[48])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V49*/ meltfptr[48]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V49*/ meltfptr[48]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V54*/ meltfptr[53]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V56*/ meltfptr[55])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V56*/ meltfptr[55]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V56*/ meltfptr[55]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V57*/ meltfptr[56]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V56*/ meltfptr[55]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V56*/ meltfptr[55],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V55*/ meltfptr[54])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V55*/ meltfptr[54]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V55*/ meltfptr[54]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V56*/ meltfptr[55]);

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
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V55*/ meltfptr[54]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V55*/ meltfptr[54],
				"new static instance");

  /*putupl#7 */
  melt_assertmsg ("putupl #7 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V58*/ meltfptr[57])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #7 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V58*/
					  meltfptr[57]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V58*/ meltfptr[57]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V55*/ meltfptr[54]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V58*/ meltfptr[57]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V49*/ meltfptr[48])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V49*/ meltfptr[48]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V49*/ meltfptr[48]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V58*/ meltfptr[57]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V49*/ meltfptr[48])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V49*/ meltfptr[48]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V49*/ meltfptr[48]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V49*/ meltfptr[48]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V49*/ meltfptr[48],
				"new static instance");

  /*put data in hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookdata checkhook HOOK_OVERRIDE_GATE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V44*/ meltfptr[43])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V49*/ meltfptr[48]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V44*/
				  meltfptr[43]));
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V44*/
				  meltfptr[43]));
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V44*/
				  meltfptr[43]));
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V44*/
				  meltfptr[43]));
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->tabval[3] =
    (melt_ptr_t) ( /*_.VALDATA___V38*/ meltfptr[37]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V44*/
				  meltfptr[43]));
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->tabval[4] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V44*/
				  meltfptr[43]));
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->tabval[5] =
    (melt_ptr_t) ( /*_.VALDATA___V45*/ meltfptr[44]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V44*/
				  meltfptr[43]));
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->tabval[6] =
    (melt_ptr_t) ( /*_.VALSTR___V46*/ meltfptr[45]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (7) >= 0
		  && (7) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V44*/
				  meltfptr[43]));
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->tabval[7] =
    (melt_ptr_t) ( /*_.VALDATA___V47*/ meltfptr[46]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (8) >= 0
		  && (8) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V44*/
				  meltfptr[43]));
  ((melthook_ptr_t) /*_.VALHOOK___V44*/ meltfptr[43])->tabval[8] =
    (melt_ptr_t) ( /*_.VALSTR___V48*/ meltfptr[47]);

  /*touch:HOOK_OVERRIDE_GATE */
  meltgc_touch ( /*_.VALHOOK___V44*/ meltfptr[43]);

  /*putclosurout#1 */
  melt_assertmsg ("putclosrout#1 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V60*/ meltfptr[59])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#1 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V59*/ meltfptr[58])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V60*/ meltfptr[59])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V59*/ meltfptr[58]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V60*/ meltfptr[59]);

  MELT_LOCATION ("warmelt-hooks.melt:159:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V61*/ meltfptr[60])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V38*/ meltfptr[37]);
  ((meltroutine_ptr_t) /*_.VALROUT___V61*/ meltfptr[60])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V38*/ meltfptr[37]);

  MELT_LOCATION ("warmelt-hooks.melt:159:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V61*/ meltfptr[60])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_46__MAYBE_DISABLE_OVERRIDE_GATE#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V61*/ meltfptr[60])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:159:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V61*/ meltfptr[60]);

  /*putclosurout#2 */
  melt_assertmsg ("putclosrout#2 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V62*/ meltfptr[61])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#2 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V61*/ meltfptr[60])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V62*/ meltfptr[61])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V61*/ meltfptr[60]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V62*/ meltfptr[61]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_48__REGISTER_OVERRIDE_GATE_FIRST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V63*/ meltfptr[62])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V38*/ meltfptr[37]);
  ((meltroutine_ptr_t) /*_.VALROUT___V63*/ meltfptr[62])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V38*/ meltfptr[37]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_48__REGISTER_OVERRIDE_GATE_FIRST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V63*/ meltfptr[62])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V60*/ meltfptr[59]);
  ((meltroutine_ptr_t) /*_.VALROUT___V63*/ meltfptr[62])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V60*/ meltfptr[59]);

  MELT_LOCATION ("warmelt-hooks.melt:175:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V63*/ meltfptr[62]);

  /*putclosurout#3 */
  melt_assertmsg ("putclosrout#3 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V64*/ meltfptr[63])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#3 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V64*/ meltfptr[63])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V63*/ meltfptr[62]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V64*/ meltfptr[63]);


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
				      ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_50__REGISTER_OVERRIDE_GATE_LAST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V65*/ meltfptr[64])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:183:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V38*/ meltfptr[37]);
  ((meltroutine_ptr_t) /*_.VALROUT___V65*/ meltfptr[64])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V38*/ meltfptr[37]);

  MELT_LOCATION ("warmelt-hooks.melt:183:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_50__REGISTER_OVERRIDE_GATE_LAST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V65*/ meltfptr[64])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:183:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V60*/ meltfptr[59]);
  ((meltroutine_ptr_t) /*_.VALROUT___V65*/ meltfptr[64])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V60*/ meltfptr[59]);

  MELT_LOCATION ("warmelt-hooks.melt:183:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V65*/ meltfptr[64]);

  /*putclosurout#4 */
  melt_assertmsg ("putclosrout#4 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V66*/ meltfptr[65])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#4 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V66*/ meltfptr[65])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V65*/ meltfptr[64]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V66*/ meltfptr[65]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_52__UNREGISTER_OVERRIDE_GATE_FIRST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V38*/ meltfptr[37]);
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V38*/ meltfptr[37]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_52__UNREGISTER_OVERRIDE_GATE_FIRST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_52__UNREGISTER_OVERRIDE_GATE_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V62*/ meltfptr[61]);
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[4] =
    (melt_ptr_t) ( /*_.VALCLO___V62*/ meltfptr[61]);

  MELT_LOCATION ("warmelt-hooks.melt:191:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V67*/ meltfptr[66]);

  /*putclosurout#5 */
  melt_assertmsg ("putclosrout#5 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V68*/ meltfptr[67])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#5 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V68*/ meltfptr[67])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V67*/ meltfptr[66]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V68*/ meltfptr[67]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_54__UNREGISTER_OVERRIDE_GATE_LAST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V38*/ meltfptr[37]);
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V38*/ meltfptr[37]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_54__UNREGISTER_OVERRIDE_GATE_LAST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_54__UNREGISTER_OVERRIDE_GATE_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V62*/ meltfptr[61]);
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[4] =
    (melt_ptr_t) ( /*_.VALCLO___V62*/ meltfptr[61]);

  MELT_LOCATION ("warmelt-hooks.melt:214:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V69*/ meltfptr[68]);

  /*putclosurout#6 */
  melt_assertmsg ("putclosrout#6 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V70*/ meltfptr[69])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#6 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V70*/ meltfptr[69])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V69*/ meltfptr[68]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V70*/ meltfptr[69]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V71*/ meltfptr[70])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V71*/ meltfptr[70]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V71*/ meltfptr[70]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V72*/ meltfptr[71]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V71*/ meltfptr[70]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V71*/ meltfptr[70],
				"new static instance");

 /*_.MAKE_LIST__V74*/ meltfptr[73] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
 /*_.MAKE_LIST__V75*/ meltfptr[74] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V73*/ meltfptr[72])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V73*/ meltfptr[72]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V76*/ meltfptr[75]);

  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V73*/ meltfptr[72])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V73*/ meltfptr[72]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V74*/ meltfptr[73]);

  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V73*/ meltfptr[72])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V73*/ meltfptr[72]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V75*/ meltfptr[74]);

  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V73*/ meltfptr[72]);

  MELT_LOCATION ("warmelt-hooks.melt:250:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V73*/ meltfptr[72],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V78*/ meltfptr[77])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V78*/ meltfptr[77]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V78*/ meltfptr[77]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V79*/ meltfptr[78]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V78*/ meltfptr[77]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V78*/ meltfptr[77],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:269:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V80*/ meltfptr[79])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V73*/ meltfptr[72]);
  ((meltroutine_ptr_t) /*_.VALROUT___V80*/ meltfptr[79])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]);

  MELT_LOCATION ("warmelt-hooks.melt:269:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V80*/ meltfptr[79]);

  MELT_LOCATION ("warmelt-hooks.melt:283:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V82*/ meltfptr[81])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V73*/ meltfptr[72]);
  ((meltroutine_ptr_t) /*_.VALROUT___V82*/ meltfptr[81])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]);

  MELT_LOCATION ("warmelt-hooks.melt:283:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V82*/ meltfptr[81]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V83*/ meltfptr[82])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V83*/ meltfptr[82]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V83*/ meltfptr[82]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V84*/ meltfptr[83]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V85*/ meltfptr[84]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V83*/ meltfptr[82])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V83*/ meltfptr[82]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V83*/ meltfptr[82]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V85*/ meltfptr[84]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V86*/ meltfptr[85]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V83*/ meltfptr[82])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V83*/ meltfptr[82]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V83*/ meltfptr[82]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V86*/ meltfptr[85]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V83*/ meltfptr[82])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V83*/ meltfptr[82]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V83*/ meltfptr[82]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V83*/ meltfptr[82]);

  MELT_LOCATION ("warmelt-hooks.melt:259:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V83*/ meltfptr[82],
				"new static instance");

  /*put data in hook HOOK_START_UNIT */
  melt_assertmsg ("puthookdata checkhook HOOK_START_UNIT",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V77*/ meltfptr[76])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V83*/ meltfptr[82]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V77*/
				  meltfptr[76]));
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V77*/
				  meltfptr[76]));
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->tabval[1] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V77*/
				  meltfptr[76]));
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V77*/
				  meltfptr[76]));
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->tabval[3] =
    (melt_ptr_t) ( /*_.LIST_EVERY__V11*/ meltfptr[10]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V77*/
				  meltfptr[76]));
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->tabval[4] =
    (melt_ptr_t) ( /*_.VALDATA___V78*/ meltfptr[77]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V77*/
				  meltfptr[76]));
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->tabval[5] =
    (melt_ptr_t) ( /*_.VALSTR___V79*/ meltfptr[78]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V77*/
				  meltfptr[76]));
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->tabval[6] =
    (melt_ptr_t) ( /*_.VALROUT___V80*/ meltfptr[79]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (7) >= 0
		  && (7) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V77*/
				  meltfptr[76]));
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->tabval[7] =
    (melt_ptr_t) ( /*_.VALROUT___V81*/ meltfptr[80]);

  /*put inside hook HOOK_START_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_START_UNIT",
		  (8) >= 0
		  && (8) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V77*/
				  meltfptr[76]));
  ((melthook_ptr_t) /*_.VALHOOK___V77*/ meltfptr[76])->tabval[8] =
    (melt_ptr_t) ( /*_.VALROUT___V82*/ meltfptr[81]);

  /*touch:HOOK_START_UNIT */
  meltgc_touch ( /*_.VALHOOK___V77*/ meltfptr[76]);

  /*putclosurout#7 */
  melt_assertmsg ("putclosrout#7 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V88*/ meltfptr[87])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#7 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V87*/ meltfptr[86])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V88*/ meltfptr[87])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V87*/ meltfptr[86]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V88*/ meltfptr[87]);

  MELT_LOCATION ("warmelt-hooks.melt:322:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V89*/ meltfptr[88])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V73*/ meltfptr[72]);
  ((meltroutine_ptr_t) /*_.VALROUT___V89*/ meltfptr[88])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]);

  MELT_LOCATION ("warmelt-hooks.melt:322:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V89*/ meltfptr[88])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V88*/ meltfptr[87]);
  ((meltroutine_ptr_t) /*_.VALROUT___V89*/ meltfptr[88])->tabval[1] =
    (melt_ptr_t) ( /*_.VALCLO___V88*/ meltfptr[87]);

  MELT_LOCATION ("warmelt-hooks.melt:322:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V89*/ meltfptr[88]);

  /*putclosurout#8 */
  melt_assertmsg ("putclosrout#8 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V90*/ meltfptr[89])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#8 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V89*/ meltfptr[88])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V90*/ meltfptr[89])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V89*/ meltfptr[88]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V90*/ meltfptr[89]);

  MELT_LOCATION ("warmelt-hooks.melt:331:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V91*/ meltfptr[90])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V73*/ meltfptr[72]);
  ((meltroutine_ptr_t) /*_.VALROUT___V91*/ meltfptr[90])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]);

  MELT_LOCATION ("warmelt-hooks.melt:331:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V91*/ meltfptr[90])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V88*/ meltfptr[87]);
  ((meltroutine_ptr_t) /*_.VALROUT___V91*/ meltfptr[90])->tabval[1] =
    (melt_ptr_t) ( /*_.VALCLO___V88*/ meltfptr[87]);

  MELT_LOCATION ("warmelt-hooks.melt:331:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V91*/ meltfptr[90]);

  /*putclosurout#9 */
  melt_assertmsg ("putclosrout#9 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V92*/ meltfptr[91])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#9 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V91*/ meltfptr[90])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V92*/ meltfptr[91])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V91*/ meltfptr[90]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V92*/ meltfptr[91]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V93*/ meltfptr[92])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V93*/ meltfptr[92]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V93*/ meltfptr[92]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V94*/ meltfptr[93]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V93*/ meltfptr[92]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V93*/ meltfptr[92],
				"new static instance");

 /*_.MAKE_LIST__V96*/ meltfptr[95] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
 /*_.MAKE_LIST__V97*/ meltfptr[96] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V95*/ meltfptr[94])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V95*/ meltfptr[94]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V95*/ meltfptr[94]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V98*/ meltfptr[97]);

  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V95*/ meltfptr[94])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V95*/ meltfptr[94]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V95*/ meltfptr[94]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V96*/ meltfptr[95]);

  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V95*/ meltfptr[94])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V95*/ meltfptr[94]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V95*/ meltfptr[94]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V97*/ meltfptr[96]);

  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V95*/ meltfptr[94]);

  MELT_LOCATION ("warmelt-hooks.melt:347:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V95*/ meltfptr[94],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:371:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V100*/ meltfptr[99])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V95*/ meltfptr[94]);
  ((meltroutine_ptr_t) /*_.VALROUT___V100*/ meltfptr[99])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V95*/ meltfptr[94]);

  MELT_LOCATION ("warmelt-hooks.melt:371:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V100*/ meltfptr[99]);

  MELT_LOCATION ("warmelt-hooks.melt:385:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V102*/ meltfptr[101])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V95*/ meltfptr[94]);
  ((meltroutine_ptr_t) /*_.VALROUT___V102*/ meltfptr[101])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V95*/ meltfptr[94]);

  MELT_LOCATION ("warmelt-hooks.melt:385:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V102*/ meltfptr[101]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V103*/ meltfptr[102])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V103*/
					meltfptr[102]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V103*/ meltfptr[102]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V104*/ meltfptr[103]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V105*/ meltfptr[104]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V103*/ meltfptr[102])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V103*/
					meltfptr[102]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V103*/ meltfptr[102]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V105*/ meltfptr[104]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V106*/ meltfptr[105]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V103*/ meltfptr[102])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V103*/
					meltfptr[102]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V103*/ meltfptr[102]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V106*/ meltfptr[105]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V103*/ meltfptr[102])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V103*/
					meltfptr[102]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V103*/ meltfptr[102]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V103*/ meltfptr[102]);

  MELT_LOCATION ("warmelt-hooks.melt:361:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V103*/ meltfptr[102],
				"new static instance");

  /*put data in hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookdata checkhook HOOK_FINISH_UNIT",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V99*/ meltfptr[98])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V99*/ meltfptr[98])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V103*/ meltfptr[102]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V99*/
				  meltfptr[98]));
  ((melthook_ptr_t) /*_.VALHOOK___V99*/ meltfptr[98])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V95*/ meltfptr[94]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V99*/
				  meltfptr[98]));
  ((melthook_ptr_t) /*_.VALHOOK___V99*/ meltfptr[98])->tabval[1] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V99*/
				  meltfptr[98]));
  ((melthook_ptr_t) /*_.VALHOOK___V99*/ meltfptr[98])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V99*/
				  meltfptr[98]));
  ((melthook_ptr_t) /*_.VALHOOK___V99*/ meltfptr[98])->tabval[3] =
    (melt_ptr_t) ( /*_.LIST_EVERY__V11*/ meltfptr[10]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V99*/
				  meltfptr[98]));
  ((melthook_ptr_t) /*_.VALHOOK___V99*/ meltfptr[98])->tabval[4] =
    (melt_ptr_t) ( /*_.VALROUT___V100*/ meltfptr[99]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V99*/
				  meltfptr[98]));
  ((melthook_ptr_t) /*_.VALHOOK___V99*/ meltfptr[98])->tabval[5] =
    (melt_ptr_t) ( /*_.VALROUT___V101*/ meltfptr[100]);

  /*put inside hook HOOK_FINISH_UNIT */
  melt_assertmsg ("puthookconst checkhook HOOK_FINISH_UNIT",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V99*/
				  meltfptr[98]));
  ((melthook_ptr_t) /*_.VALHOOK___V99*/ meltfptr[98])->tabval[6] =
    (melt_ptr_t) ( /*_.VALROUT___V102*/ meltfptr[101]);

  /*touch:HOOK_FINISH_UNIT */
  meltgc_touch ( /*_.VALHOOK___V99*/ meltfptr[98]);

  /*putclosurout#10 */
  melt_assertmsg ("putclosrout#10 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V108*/ meltfptr[107])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#10 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V107*/ meltfptr[106])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V108*/ meltfptr[107])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V107*/ meltfptr[106]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V108*/ meltfptr[107]);

  MELT_LOCATION ("warmelt-hooks.melt:427:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V109*/ meltfptr[108])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V95*/ meltfptr[94]);
  ((meltroutine_ptr_t) /*_.VALROUT___V109*/ meltfptr[108])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V95*/ meltfptr[94]);

  MELT_LOCATION ("warmelt-hooks.melt:427:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V109*/ meltfptr[108])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V108*/ meltfptr[107]);
  ((meltroutine_ptr_t) /*_.VALROUT___V109*/ meltfptr[108])->tabval[1] =
    (melt_ptr_t) ( /*_.VALCLO___V108*/ meltfptr[107]);

  MELT_LOCATION ("warmelt-hooks.melt:427:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V109*/ meltfptr[108]);

  /*putclosurout#11 */
  melt_assertmsg ("putclosrout#11 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V110*/ meltfptr[109])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#11 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V109*/ meltfptr[108])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V110*/ meltfptr[109])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V109*/ meltfptr[108]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V110*/ meltfptr[109]);

  MELT_LOCATION ("warmelt-hooks.melt:437:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V111*/ meltfptr[110])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V95*/ meltfptr[94]);
  ((meltroutine_ptr_t) /*_.VALROUT___V111*/ meltfptr[110])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V95*/ meltfptr[94]);

  MELT_LOCATION ("warmelt-hooks.melt:437:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V111*/ meltfptr[110])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V108*/ meltfptr[107]);
  ((meltroutine_ptr_t) /*_.VALROUT___V111*/ meltfptr[110])->tabval[1] =
    (melt_ptr_t) ( /*_.VALCLO___V108*/ meltfptr[107]);

  MELT_LOCATION ("warmelt-hooks.melt:437:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V111*/ meltfptr[110]);

  /*putclosurout#12 */
  melt_assertmsg ("putclosrout#12 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V112*/ meltfptr[111])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#12 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V111*/ meltfptr[110])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V112*/ meltfptr[111])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V111*/ meltfptr[110]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V112*/ meltfptr[111]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V113*/ meltfptr[112])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V113*/
					meltfptr[112]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V113*/ meltfptr[112]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V114*/ meltfptr[113]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V113*/ meltfptr[112]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V113*/ meltfptr[112],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V115*/ meltfptr[114])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V115*/
					meltfptr[114]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V115*/ meltfptr[114]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V116*/ meltfptr[115]);

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V115*/ meltfptr[114])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V115*/
					meltfptr[114]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V115*/ meltfptr[114]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V115*/ meltfptr[114])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V115*/
					meltfptr[114]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V115*/ meltfptr[114]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V115*/ meltfptr[114]);

  MELT_LOCATION ("warmelt-hooks.melt:458:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V115*/ meltfptr[114],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V118*/ meltfptr[117])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V118*/
					meltfptr[117]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V118*/ meltfptr[117]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V119*/ meltfptr[118]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V120*/ meltfptr[119]);

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V118*/ meltfptr[117])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V118*/
					meltfptr[117]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V118*/ meltfptr[117]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V120*/ meltfptr[119]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V121*/ meltfptr[120]);

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V118*/ meltfptr[117])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V118*/
					meltfptr[117]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V118*/ meltfptr[117]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V121*/ meltfptr[120]);

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V118*/ meltfptr[117])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V118*/
					meltfptr[117]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V118*/ meltfptr[117]))->obj_vartab[4] =
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
  meltgc_touch ( /*_.VALDATA___V118*/ meltfptr[117]);

  MELT_LOCATION ("warmelt-hooks.melt:472:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V118*/ meltfptr[117],
				"new static instance");

  /*put data in hook HOOK_ALL_PASSES_START */
  melt_assertmsg ("puthookdata checkhook HOOK_ALL_PASSES_START",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V117*/ meltfptr[116])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V117*/ meltfptr[116])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V118*/ meltfptr[117]);

  /*put inside hook HOOK_ALL_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_START",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V117*/
				  meltfptr[116]));
  ((melthook_ptr_t) /*_.VALHOOK___V117*/ meltfptr[116])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V115*/ meltfptr[114]);

  /*put inside hook HOOK_ALL_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_START",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V117*/
				  meltfptr[116]));
  ((melthook_ptr_t) /*_.VALHOOK___V117*/ meltfptr[116])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_ALL_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_START",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V117*/
				  meltfptr[116]));
  ((melthook_ptr_t) /*_.VALHOOK___V117*/ meltfptr[116])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_ALL_PASSES_START */
  meltgc_touch ( /*_.VALHOOK___V117*/ meltfptr[116]);

  /*putclosurout#13 */
  melt_assertmsg ("putclosrout#13 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V123*/ meltfptr[122])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#13 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V122*/ meltfptr[121])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V123*/ meltfptr[122])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V122*/ meltfptr[121]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V123*/ meltfptr[122]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V124*/ meltfptr[123])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V115*/ meltfptr[114]);
  ((meltroutine_ptr_t) /*_.VALROUT___V124*/ meltfptr[123])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V115*/ meltfptr[114]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V124*/ meltfptr[123]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_102__REGISTER_ALL_PASSES_START_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V124*/ meltfptr[123])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V124*/ meltfptr[123])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V123*/ meltfptr[122]);
  ((meltroutine_ptr_t) /*_.VALROUT___V124*/ meltfptr[123])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V123*/ meltfptr[122]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V124*/ meltfptr[123]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_102__REGISTER_ALL_PASSES_START_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V124*/ meltfptr[123])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:526:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V124*/ meltfptr[123]);

  /*putclosurout#14 */
  melt_assertmsg ("putclosrout#14 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V125*/ meltfptr[124])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#14 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V124*/ meltfptr[123])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V125*/ meltfptr[124])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V124*/ meltfptr[123]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V125*/ meltfptr[124]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V126*/ meltfptr[125])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V115*/ meltfptr[114]);
  ((meltroutine_ptr_t) /*_.VALROUT___V126*/ meltfptr[125])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V115*/ meltfptr[114]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V126*/ meltfptr[125]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_104__REGISTER_ALL_PASSES_START_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V126*/ meltfptr[125])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V126*/ meltfptr[125])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V123*/ meltfptr[122]);
  ((meltroutine_ptr_t) /*_.VALROUT___V126*/ meltfptr[125])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V123*/ meltfptr[122]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V126*/ meltfptr[125]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_104__REGISTER_ALL_PASSES_START_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V126*/ meltfptr[125])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:549:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V126*/ meltfptr[125]);

  /*putclosurout#15 */
  melt_assertmsg ("putclosrout#15 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V127*/ meltfptr[126])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#15 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V126*/ meltfptr[125])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V127*/ meltfptr[126])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V126*/ meltfptr[125]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V127*/ meltfptr[126]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V128*/ meltfptr[127])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V128*/
					meltfptr[127]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V128*/ meltfptr[127]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V129*/ meltfptr[128]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V128*/ meltfptr[127]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V128*/ meltfptr[127],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V130*/ meltfptr[129])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V130*/
					meltfptr[129]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V130*/ meltfptr[129]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V131*/ meltfptr[130]);

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V130*/ meltfptr[129])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V130*/
					meltfptr[129]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V130*/ meltfptr[129]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V130*/ meltfptr[129])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V130*/
					meltfptr[129]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V130*/ meltfptr[129]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V130*/ meltfptr[129]);

  MELT_LOCATION ("warmelt-hooks.melt:589:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V130*/ meltfptr[129],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V133*/ meltfptr[132])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V133*/
					meltfptr[132]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V133*/ meltfptr[132]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V134*/ meltfptr[133]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V135*/ meltfptr[134]);

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V133*/ meltfptr[132])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V133*/
					meltfptr[132]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V133*/ meltfptr[132]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V135*/ meltfptr[134]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V136*/ meltfptr[135]);

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V133*/ meltfptr[132])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V133*/
					meltfptr[132]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V133*/ meltfptr[132]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V136*/ meltfptr[135]);

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V133*/ meltfptr[132])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V133*/
					meltfptr[132]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V133*/ meltfptr[132]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V133*/ meltfptr[132]);

  MELT_LOCATION ("warmelt-hooks.melt:596:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V133*/ meltfptr[132],
				"new static instance");

  /*put data in hook HOOK_ALL_PASSES_END */
  melt_assertmsg ("puthookdata checkhook HOOK_ALL_PASSES_END",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V132*/ meltfptr[131])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V132*/ meltfptr[131])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V133*/ meltfptr[132]);

  /*put inside hook HOOK_ALL_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_END",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V132*/
				  meltfptr[131]));
  ((melthook_ptr_t) /*_.VALHOOK___V132*/ meltfptr[131])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V130*/ meltfptr[129]);

  /*put inside hook HOOK_ALL_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_END",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V132*/
				  meltfptr[131]));
  ((melthook_ptr_t) /*_.VALHOOK___V132*/ meltfptr[131])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_ALL_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_PASSES_END",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V132*/
				  meltfptr[131]));
  ((melthook_ptr_t) /*_.VALHOOK___V132*/ meltfptr[131])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_ALL_PASSES_END */
  meltgc_touch ( /*_.VALHOOK___V132*/ meltfptr[131]);

  /*putclosurout#16 */
  melt_assertmsg ("putclosrout#16 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V138*/ meltfptr[137])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#16 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V137*/ meltfptr[136])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V138*/ meltfptr[137])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V137*/ meltfptr[136]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V138*/ meltfptr[137]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V139*/ meltfptr[138])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V130*/ meltfptr[129]);
  ((meltroutine_ptr_t) /*_.VALROUT___V139*/ meltfptr[138])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V130*/ meltfptr[129]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V139*/ meltfptr[138]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_116__REGISTER_ALL_PASSES_END_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V139*/ meltfptr[138])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V139*/ meltfptr[138])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V138*/ meltfptr[137]);
  ((meltroutine_ptr_t) /*_.VALROUT___V139*/ meltfptr[138])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V138*/ meltfptr[137]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V139*/ meltfptr[138]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_116__REGISTER_ALL_PASSES_END_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V139*/ meltfptr[138])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:649:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V139*/ meltfptr[138]);

  /*putclosurout#17 */
  melt_assertmsg ("putclosrout#17 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V140*/ meltfptr[139])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#17 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V139*/ meltfptr[138])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V140*/ meltfptr[139])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V139*/ meltfptr[138]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V140*/ meltfptr[139]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V141*/ meltfptr[140])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V130*/ meltfptr[129]);
  ((meltroutine_ptr_t) /*_.VALROUT___V141*/ meltfptr[140])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V130*/ meltfptr[129]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V141*/ meltfptr[140]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_118__REGISTER_ALL_PASSES_END_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V141*/ meltfptr[140])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V141*/ meltfptr[140])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V138*/ meltfptr[137]);
  ((meltroutine_ptr_t) /*_.VALROUT___V141*/ meltfptr[140])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V138*/ meltfptr[137]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V141*/ meltfptr[140]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_118__REGISTER_ALL_PASSES_END_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V141*/ meltfptr[140])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:672:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V141*/ meltfptr[140]);

  /*putclosurout#18 */
  melt_assertmsg ("putclosrout#18 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V142*/ meltfptr[141])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#18 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V141*/ meltfptr[140])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V142*/ meltfptr[141])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V141*/ meltfptr[140]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V142*/ meltfptr[141]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V143*/ meltfptr[142])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V143*/
					meltfptr[142]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V143*/ meltfptr[142]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V144*/ meltfptr[143]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V143*/ meltfptr[142]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V143*/ meltfptr[142],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V145*/ meltfptr[144])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V145*/
					meltfptr[144]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V145*/ meltfptr[144]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V146*/ meltfptr[145]);

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V145*/ meltfptr[144])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V145*/
					meltfptr[144]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V145*/ meltfptr[144]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V145*/ meltfptr[144])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V145*/
					meltfptr[144]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V145*/ meltfptr[144]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V145*/ meltfptr[144]);

  MELT_LOCATION ("warmelt-hooks.melt:703:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V145*/ meltfptr[144],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V148*/ meltfptr[147])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V148*/
					meltfptr[147]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V148*/ meltfptr[147]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V149*/ meltfptr[148]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V150*/ meltfptr[149]);

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V148*/ meltfptr[147])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V148*/
					meltfptr[147]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V148*/ meltfptr[147]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V150*/ meltfptr[149]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V151*/ meltfptr[150]);

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V148*/ meltfptr[147])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V148*/
					meltfptr[147]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V148*/ meltfptr[147]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V151*/ meltfptr[150]);

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V148*/ meltfptr[147])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V148*/
					meltfptr[147]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V148*/ meltfptr[147]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V148*/ meltfptr[147]);

  MELT_LOCATION ("warmelt-hooks.melt:717:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V148*/ meltfptr[147],
				"new static instance");

  /*put data in hook HOOK_ALL_IPA_PASSES_START */
  melt_assertmsg ("puthookdata checkhook HOOK_ALL_IPA_PASSES_START",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V147*/ meltfptr[146])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V147*/ meltfptr[146])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V148*/ meltfptr[147]);

  /*put inside hook HOOK_ALL_IPA_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_START",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V147*/
				  meltfptr[146]));
  ((melthook_ptr_t) /*_.VALHOOK___V147*/ meltfptr[146])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V145*/ meltfptr[144]);

  /*put inside hook HOOK_ALL_IPA_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_START",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V147*/
				  meltfptr[146]));
  ((melthook_ptr_t) /*_.VALHOOK___V147*/ meltfptr[146])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_ALL_IPA_PASSES_START */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_START",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V147*/
				  meltfptr[146]));
  ((melthook_ptr_t) /*_.VALHOOK___V147*/ meltfptr[146])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_ALL_IPA_PASSES_START */
  meltgc_touch ( /*_.VALHOOK___V147*/ meltfptr[146]);

  /*putclosurout#19 */
  melt_assertmsg ("putclosrout#19 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V153*/ meltfptr[152])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#19 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V152*/ meltfptr[151])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V153*/ meltfptr[152])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V152*/ meltfptr[151]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V153*/ meltfptr[152]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V154*/ meltfptr[153])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V145*/ meltfptr[144]);
  ((meltroutine_ptr_t) /*_.VALROUT___V154*/ meltfptr[153])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V145*/ meltfptr[144]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V154*/ meltfptr[153]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V154*/ meltfptr[153])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V154*/ meltfptr[153])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V153*/ meltfptr[152]);
  ((meltroutine_ptr_t) /*_.VALROUT___V154*/ meltfptr[153])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V153*/ meltfptr[152]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V154*/ meltfptr[153]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_130__REGISTER_ALL_IPA_PASSES_START_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V154*/ meltfptr[153])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:771:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V154*/ meltfptr[153]);

  /*putclosurout#20 */
  melt_assertmsg ("putclosrout#20 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V155*/ meltfptr[154])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#20 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V154*/ meltfptr[153])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V155*/ meltfptr[154])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V154*/ meltfptr[153]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V155*/ meltfptr[154]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V156*/ meltfptr[155])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V145*/ meltfptr[144]);
  ((meltroutine_ptr_t) /*_.VALROUT___V156*/ meltfptr[155])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V145*/ meltfptr[144]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V156*/ meltfptr[155]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_132__REGISTER_ALL_IPA_PASSES_START_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V156*/ meltfptr[155])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V156*/ meltfptr[155])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V153*/ meltfptr[152]);
  ((meltroutine_ptr_t) /*_.VALROUT___V156*/ meltfptr[155])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V153*/ meltfptr[152]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V156*/ meltfptr[155]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_132__REGISTER_ALL_IPA_PASSES_START_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V156*/ meltfptr[155])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:794:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V156*/ meltfptr[155]);

  /*putclosurout#21 */
  melt_assertmsg ("putclosrout#21 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V157*/ meltfptr[156])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#21 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V156*/ meltfptr[155])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V157*/ meltfptr[156])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V156*/ meltfptr[155]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V157*/ meltfptr[156]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V158*/ meltfptr[157])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V158*/
					meltfptr[157]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V158*/ meltfptr[157]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V159*/ meltfptr[158]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V158*/ meltfptr[157]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V158*/ meltfptr[157],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V160*/ meltfptr[159])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V160*/
					meltfptr[159]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V160*/ meltfptr[159]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V161*/ meltfptr[160]);

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V160*/ meltfptr[159])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V160*/
					meltfptr[159]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V160*/ meltfptr[159]))->obj_vartab[2] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V160*/ meltfptr[159])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V160*/
					meltfptr[159]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V160*/ meltfptr[159]))->obj_vartab[3] =
    (melt_ptr_t) (
		    ( /*nil */ NULL));

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V160*/ meltfptr[159]);

  MELT_LOCATION ("warmelt-hooks.melt:834:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V160*/ meltfptr[159],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V163*/ meltfptr[162])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V163*/
					meltfptr[162]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V163*/ meltfptr[162]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V164*/ meltfptr[163]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V165*/ meltfptr[164]);

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V163*/ meltfptr[162])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V163*/
					meltfptr[162]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V163*/ meltfptr[162]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V165*/ meltfptr[164]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V166*/ meltfptr[165]);

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V163*/ meltfptr[162])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V163*/
					meltfptr[162]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V163*/ meltfptr[162]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V166*/ meltfptr[165]);

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V163*/ meltfptr[162])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V163*/
					meltfptr[162]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V163*/ meltfptr[162]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V163*/ meltfptr[162]);

  MELT_LOCATION ("warmelt-hooks.melt:841:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V163*/ meltfptr[162],
				"new static instance");

  /*put data in hook HOOK_ALL_IPA_PASSES_END */
  melt_assertmsg ("puthookdata checkhook HOOK_ALL_IPA_PASSES_END",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V162*/ meltfptr[161])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V162*/ meltfptr[161])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V163*/ meltfptr[162]);

  /*put inside hook HOOK_ALL_IPA_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_END",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V162*/
				  meltfptr[161]));
  ((melthook_ptr_t) /*_.VALHOOK___V162*/ meltfptr[161])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V160*/ meltfptr[159]);

  /*put inside hook HOOK_ALL_IPA_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_END",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V162*/
				  meltfptr[161]));
  ((melthook_ptr_t) /*_.VALHOOK___V162*/ meltfptr[161])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_ALL_IPA_PASSES_END */
  melt_assertmsg ("puthookconst checkhook HOOK_ALL_IPA_PASSES_END",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V162*/
				  meltfptr[161]));
  ((melthook_ptr_t) /*_.VALHOOK___V162*/ meltfptr[161])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*touch:HOOK_ALL_IPA_PASSES_END */
  meltgc_touch ( /*_.VALHOOK___V162*/ meltfptr[161]);

  /*putclosurout#22 */
  melt_assertmsg ("putclosrout#22 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V168*/ meltfptr[167])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#22 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V167*/ meltfptr[166])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V168*/ meltfptr[167])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V167*/ meltfptr[166]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V168*/ meltfptr[167]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V169*/ meltfptr[168])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V160*/ meltfptr[159]);
  ((meltroutine_ptr_t) /*_.VALROUT___V169*/ meltfptr[168])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V160*/ meltfptr[159]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V169*/ meltfptr[168]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V169*/ meltfptr[168])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V169*/ meltfptr[168])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V168*/ meltfptr[167]);
  ((meltroutine_ptr_t) /*_.VALROUT___V169*/ meltfptr[168])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V168*/ meltfptr[167]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V169*/ meltfptr[168]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_144__REGISTER_ALL_IPA_PASSES_END_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V169*/ meltfptr[168])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:894:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V169*/ meltfptr[168]);

  /*putclosurout#23 */
  melt_assertmsg ("putclosrout#23 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V170*/ meltfptr[169])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#23 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V169*/ meltfptr[168])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V170*/ meltfptr[169])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V169*/ meltfptr[168]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V170*/ meltfptr[169]);

  MELT_LOCATION ("warmelt-hooks.melt:917:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V171*/ meltfptr[170])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V160*/ meltfptr[159]);
  ((meltroutine_ptr_t) /*_.VALROUT___V171*/ meltfptr[170])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V160*/ meltfptr[159]);


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
				      ( /*_.VALROUT___V171*/ meltfptr[170]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_146__REGISTER_ALL_IPA_PASSES_END_LAST#1",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V171*/ meltfptr[170])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:917:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V171*/ meltfptr[170])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V168*/ meltfptr[167]);
  ((meltroutine_ptr_t) /*_.VALROUT___V171*/ meltfptr[170])->tabval[2] =
    (melt_ptr_t) ( /*_.VALCLO___V168*/ meltfptr[167]);

  MELT_LOCATION ("warmelt-hooks.melt:917:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V171*/ meltfptr[170]))
		    == MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_146__REGISTER_ALL_IPA_PASSES_END_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V171*/ meltfptr[170])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:917:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V171*/ meltfptr[170]);

  /*putclosurout#24 */
  melt_assertmsg ("putclosrout#24 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V172*/ meltfptr[171])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#24 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V171*/ meltfptr[170])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V172*/ meltfptr[171])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V171*/ meltfptr[170]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V172*/ meltfptr[171]);

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V173*/ meltfptr[172])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V173*/
					meltfptr[172]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V173*/ meltfptr[172]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V174*/ meltfptr[173]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V173*/ meltfptr[172]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V173*/ meltfptr[172],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V175*/ meltfptr[174])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V175*/
					meltfptr[174]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V175*/ meltfptr[174]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V176*/ meltfptr[175]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V175*/ meltfptr[174]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V175*/ meltfptr[174],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V177*/ meltfptr[176])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V177*/
					meltfptr[176]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V177*/ meltfptr[176]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V178*/ meltfptr[177]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V177*/ meltfptr[176]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V177*/ meltfptr[176],
				"new static instance");

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

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V181*/ meltfptr[180]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V181*/ meltfptr[180],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V183*/ meltfptr[182])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V183*/
					meltfptr[182]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V183*/ meltfptr[182]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V184*/ meltfptr[183]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V183*/ meltfptr[182]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V183*/ meltfptr[182],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V185*/ meltfptr[184])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V185*/
					meltfptr[184]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V185*/ meltfptr[184]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V186*/ meltfptr[185]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V185*/ meltfptr[184]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V185*/ meltfptr[184],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V187*/ meltfptr[186])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V187*/
					meltfptr[186]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V187*/ meltfptr[186]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V188*/ meltfptr[187]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V187*/ meltfptr[186]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V187*/ meltfptr[186],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V189*/ meltfptr[188])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V189*/
					meltfptr[188]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V189*/ meltfptr[188]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V190*/ meltfptr[189]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V189*/ meltfptr[188]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V189*/ meltfptr[188],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V191*/ meltfptr[190])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V191*/
					meltfptr[190]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V191*/ meltfptr[190]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V192*/ meltfptr[191]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V191*/ meltfptr[190]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V191*/ meltfptr[190],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V193*/ meltfptr[192])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V193*/
					meltfptr[192]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V193*/ meltfptr[192]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V194*/ meltfptr[193]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V193*/ meltfptr[192]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V193*/ meltfptr[192],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V195*/ meltfptr[194])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V195*/
					meltfptr[194]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V195*/ meltfptr[194]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V196*/ meltfptr[195]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V195*/ meltfptr[194]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V195*/ meltfptr[194],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V197*/ meltfptr[196])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V197*/
					meltfptr[196]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V197*/ meltfptr[196]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V198*/ meltfptr[197]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V197*/ meltfptr[196]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V197*/ meltfptr[196],
				"new static instance");

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

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V199*/ meltfptr[198]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V199*/ meltfptr[198],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V201*/ meltfptr[200])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V201*/
					meltfptr[200]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V201*/ meltfptr[200]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V202*/ meltfptr[201]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V201*/ meltfptr[200]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V201*/ meltfptr[200],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V203*/ meltfptr[202])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V203*/
					meltfptr[202]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V203*/ meltfptr[202]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V204*/ meltfptr[203]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V203*/ meltfptr[202]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V203*/ meltfptr[202],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V205*/ meltfptr[204])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V205*/
					meltfptr[204]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V205*/ meltfptr[204]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V206*/ meltfptr[205]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V205*/ meltfptr[204]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V205*/ meltfptr[204],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V207*/ meltfptr[206])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V207*/
					meltfptr[206]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V207*/ meltfptr[206]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V208*/ meltfptr[207]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V207*/ meltfptr[206]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V207*/ meltfptr[206],
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
  /*_.OR___V209*/ meltfptr[208] =
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
     /*_.SYSDATA_CONT_FRESH_ENV__V210*/ meltfptr[209] = slot;
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
	    /*_.FUN___V211*/ meltfptr[210] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.SYSDATA_CONT_FRESH_ENV__V210*/ meltfptr[209]),
			  (melt_ptr_t) ( /*_.PREVENV___V4*/ meltfptr[3]),
			  (MELTBPARSTR_CSTRING ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*_.OR___V209*/ meltfptr[208] = /*_.FUN___V211*/ meltfptr[210];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:4:/ clear");
	      /*clear *//*_.SYSDATA_CONT_FRESH_ENV__V210*/ meltfptr[209] = 0;
	  /*^clear */
	      /*clear *//*_.FUN___V211*/ meltfptr[210] = 0;
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
	    /*clear *//*_.OR___V209*/ meltfptr[208] = 0;
  }

  MELT_LOCATION ("warmelt-hooks.melt:948:/ initchunk");
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
   /*_.VALUEXPORTER___V212*/ meltfptr[209] = slot;
    };
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALCLO___V110*/ meltfptr[109];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : AT_FINISH_UNIT_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V173*/ meltfptr[172]),
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
	(melt_ptr_t *) & /*_.VALCLO___V112*/ meltfptr[111];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : AT_FINISH_UNIT_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V175*/ meltfptr[174]),
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
	(melt_ptr_t *) & /*_.VALCLO___V90*/ meltfptr[89];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : AT_START_UNIT_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V177*/ meltfptr[176]),
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
	(melt_ptr_t *) & /*_.VALCLO___V92*/ meltfptr[91];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : AT_START_UNIT_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V179*/ meltfptr[178]),
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
	(melt_ptr_t *) & /*_.VALHOOK___V44*/ meltfptr[43];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : HOOK_OVERRIDE_GATE */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V181*/ meltfptr[180]),
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
	(melt_ptr_t *) & /*_.VALHOOK___V12*/ meltfptr[11];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : HOOK_LOW_DEBUG_VALUE_AT */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V183*/ meltfptr[182]),
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
	(melt_ptr_t *) & /*_.VALCLO___V64*/ meltfptr[63];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_OVERRIDE_GATE_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V185*/ meltfptr[184]),
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
	(melt_ptr_t *) & /*_.VALCLO___V66*/ meltfptr[65];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_OVERRIDE_GATE_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V187*/ meltfptr[186]),
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
	(melt_ptr_t *) & /*_.VALCLO___V170*/ meltfptr[169];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_IPA_PASSES_END_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V189*/ meltfptr[188]),
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
	(melt_ptr_t *) & /*_.VALCLO___V172*/ meltfptr[171];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_IPA_PASSES_END_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V191*/ meltfptr[190]),
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
	(melt_ptr_t *) & /*_.VALCLO___V155*/ meltfptr[154];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_IPA_PASSES_START_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V193*/ meltfptr[192]),
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
	(melt_ptr_t *) & /*_.VALCLO___V157*/ meltfptr[156];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_IPA_PASSES_START_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V195*/ meltfptr[194]),
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
	(melt_ptr_t *) & /*_.VALCLO___V140*/ meltfptr[139];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_PASSES_END_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V197*/ meltfptr[196]),
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
	(melt_ptr_t *) & /*_.VALCLO___V142*/ meltfptr[141];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_PASSES_END_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V199*/ meltfptr[198]),
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
	(melt_ptr_t *) & /*_.VALCLO___V125*/ meltfptr[124];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_PASSES_START_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V201*/ meltfptr[200]),
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
	(melt_ptr_t *) & /*_.VALCLO___V127*/ meltfptr[126];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_ALL_PASSES_START_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V203*/ meltfptr[202]),
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
	(melt_ptr_t *) & /*_.VALCLO___V68*/ meltfptr[67];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : UNREGISTER_OVERRIDE_GATE_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V205*/ meltfptr[204]),
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
	/*quasi.cur.mod.env.cont norm.exp.val : UNREGISTER_OVERRIDE_GATE_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V212*/ meltfptr[209]),
		  (melt_ptr_t) ( /*_.VALDATA___V207*/ meltfptr[206]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*epilog */

    /*^clear */
	    /*clear *//*_.VALUEXPORTER___V212*/ meltfptr[209] = 0;
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
				     ( /*_.VALDATA___V18*/ meltfptr[17]));

  /*internsym:FILENAME */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V21*/ meltfptr[20]));

  /*internsym:LINENO */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V24*/ meltfptr[23]));

  /*internsym:MSG */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V27*/ meltfptr[26]));

  /*internsym:COUNT */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V30*/ meltfptr[29]));

  /*internsym:MELT_DEBUG_FUN */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V13*/ meltfptr[12]));

  /*internsym:OVERRIDE_GATE_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V34*/ meltfptr[33]));

  /*internsym:CLASS_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V36*/ meltfptr[35]));

  /*internsym:DISCR_LIST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V42*/ meltfptr[41]));

  /*internsym:BEFOREGATE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V52*/ meltfptr[51]));

  /*internsym:AFTERGATE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V56*/ meltfptr[55]));

  /*internkeyw:TRUE */
  (void)
    melthookproc_HOOK_INTERN_KEYWORD ((melt_ptr_t)
				      ( /*_.VALDATA___V45*/ meltfptr[44]));

  /*internsym:CLASS_REFERENCE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V47*/ meltfptr[46]));

  /*internsym:START_UNIT_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V71*/ meltfptr[70]));

  /*internsym:LIST_EVERY */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V78*/ meltfptr[77]));

  /*internsym:FINISH_UNIT_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V93*/ meltfptr[92]));

  /*internsym:ALL_PASSES_START_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V113*/ meltfptr[112]));

  /*internsym:ALL_PASSES_END_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V128*/ meltfptr[127]));

  /*internsym:ALL_IPA_PASSES_START_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V143*/ meltfptr[142]));

  /*internsym:ALL_IPA_PASSES_END_DELAYED_QUEUE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V158*/ meltfptr[157]));

  /*internsym:AT_FINISH_UNIT_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V173*/ meltfptr[172]));

  /*internsym:AT_FINISH_UNIT_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V175*/ meltfptr[174]));

  /*internsym:AT_START_UNIT_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V177*/ meltfptr[176]));

  /*internsym:AT_START_UNIT_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V179*/ meltfptr[178]));

  /*internsym:HOOK_OVERRIDE_GATE */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V181*/ meltfptr[180]));

  /*internsym:HOOK_LOW_DEBUG_VALUE_AT */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V183*/ meltfptr[182]));

  /*internsym:REGISTER_OVERRIDE_GATE_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V185*/ meltfptr[184]));

  /*internsym:REGISTER_OVERRIDE_GATE_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V187*/ meltfptr[186]));

  /*internsym:REGISTER_ALL_IPA_PASSES_END_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V189*/ meltfptr[188]));

  /*internsym:REGISTER_ALL_IPA_PASSES_END_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V191*/ meltfptr[190]));

  /*internsym:REGISTER_ALL_IPA_PASSES_START_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V193*/ meltfptr[192]));

  /*internsym:REGISTER_ALL_IPA_PASSES_START_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V195*/ meltfptr[194]));

  /*internsym:REGISTER_ALL_PASSES_END_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V197*/ meltfptr[196]));

  /*internsym:REGISTER_ALL_PASSES_END_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V199*/ meltfptr[198]));

  /*internsym:REGISTER_ALL_PASSES_START_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V201*/ meltfptr[200]));

  /*internsym:REGISTER_ALL_PASSES_START_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V203*/ meltfptr[202]));

  /*internsym:UNREGISTER_OVERRIDE_GATE_FIRST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V205*/ meltfptr[204]));

  /*internsym:UNREGISTER_OVERRIDE_GATE_LAST */
  (void)
    melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)
				     ( /*_.VALDATA___V207*/ meltfptr[206]));

 /**COMMENT: set retinit from boxloc **/ ;

 /*_.RETINIT___V1*/ meltfptr[0] =
    /* finalsetretinit */
    melt_reference_value ((melt_ptr_t) ( /*_.CONTENV___V2*/ meltfptr[1]));
 /**COMMENT: end the initproc **/ ;


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_3 */

void
meltmod__warmelt_hooks__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking)
{
  int meltix = 0;
  melt_initial_frame_st *meltframptr_ = (melt_initial_frame_st *) fp;
  melt_assertmsg ("check module frame",
		  meltframptr_->mcfr_nbvar == /*minihash */ -2218);
  if (!marking && melt_is_forwarding)
    {
      dbgprintf
	("forward_or_mark_module_start_frame_warmelt_hooks forwarding %d pointers in frame %p",
	 212, (void *) meltframptr_);
      for (meltix = 0; meltix < 212; meltix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltix]);
      return;
    }				/*end forwarding */
  dbgprintf
    ("forward_or_mark_module_start_frame_warmelt_hooks marking in frame %p",
     (void *) meltframptr_);
  for (meltix = 0; meltix < 212; meltix++)
    if (meltframptr_->mcfr_varptr[meltix])
      gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);

}				/* end meltmod__warmelt_hooks__forward_or_mark_module_start_frame */



/* exported 0 field offsets */

/* exported 0 class lengths */



/**** end of warmelt-hooks ****/
