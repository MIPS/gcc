/* GCC MELT GENERATED FILE warmelt-hooks.c - DO NOT EDIT */

/***+ warmelt-hooks without options +***/
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt[] =
  MELT_RUN_HASHMD5 /* from melt-run.h */ ;


/**** warmelt-hooks declarations ****/


/***************************************************
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
****************************************************/

/* ordinary MELT module */
#define MELT_HAS_INITIAL_ENVIRONMENT 1	/*usual */

struct melt_callframe_st;	/*defined in melt-runtime.h */



/***** 1 extra C headers *****/


/** header #0: **/


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



/***** end of 1 extra C headers *****/


/** declaration of hook melthook_HOOK_OVERRIDE_GATE */
MELT_EXTERN
  void melthook_HOOK_OVERRIDE_GATE (melt_ptr_t melthookdata,
				    long meltinp0_BEFOREGATE,
				    long *meltoutp0_AFTERGATE);


/* declare hook frame marking routine for melthook_HOOK_OVERRIDE_GATE hook */
static void melthookmark_HOOK_OVERRIDE_GATE_rout (struct melt_callframe_st *,
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
meltmod__warmelt_hooks__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking);



/**** warmelt-hooks implementations ****/
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
#define MELTFRAM_NBVARPTR 1
  melt_ptr_t mcfr_varptr[1];
#define MELTFRAM_NBVARNUM 2
  long mcfr_varnum[2];
/*others*/
  long _spare_;
}
melthookframe_HOOK_OVERRIDE_GATE_t;	/* end of hook frame melthook_HOOK_OVERRIDE_GATE */


/** implementation of hook melthook_HOOK_OVERRIDE_GATE */
void
melthook_HOOK_OVERRIDE_GATE (melt_ptr_t melthookdata,
			     long meltinp0_BEFOREGATE,
			     long *meltoutp0_AFTERGATE)
{				/* body of hook melthook_HOOK_OVERRIDE_GATE */
/* declare and initialize frame for melthook_HOOK_OVERRIDE_GATE hook */
#undef meltcallcount
#if MELT_HAVE_DEBUG
  static long melthook_call_counter_HOOK_OVERRIDE_GATE;
  long melt_thishookcall_counter_HOOK_OVERRIDE_GATE ATTRIBUTE_UNUSED =
    melthook_call_counter_HOOK_OVERRIDE_GATE++;
#define meltcallcount melt_thishookcall_counter_HOOK_OVERRIDE_GATE
#else
#define meltcallcount 0L
#endif
  melthookframe_HOOK_OVERRIDE_GATE_t melthook_HOOK_OVERRIDE_GATE_frame_;
  /* clear and push the new call frame for hook melthook_HOOK_OVERRIDE_GATE */
  memset (&melthook_HOOK_OVERRIDE_GATE_frame_, 0,
	  sizeof (melthook_HOOK_OVERRIDE_GATE_frame_));
#define meltfram__ melthook_HOOK_OVERRIDE_GATE_frame_
#define meltframe meltfram__
  meltframe.mcfr_nbvar = -314 /*hook minihash */ ;
  ((struct melt_callframe_st *) &meltframe)->mcfr_forwmarkrout
    = melthookmark_HOOK_OVERRIDE_GATE_rout;
  meltframe.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe =
    (struct melt_callframe_st *) &melthook_HOOK_OVERRIDE_GATE_frame_;
#define meltfhook /*hook data of HOOK_OVERRIDE_GATE*/ ((melthook_ptr_t) (/*_.HOOKDATA___V1*/ meltfptr[0]))
  /* body of melthook_HOOK_OVERRIDE_GATE hook */
  MELT_LOCATION ("warmelt-hooks.melt:35:/ compute");
  /*_.HOOKDATA___V1*/ meltfptr[0] = /*hookgetdata */ melthookdata;;
  /*^compute */
  /*_#BEFOREGATE__L1*/ meltfnum[0] = /*gethookinput */ meltinp0_BEFOREGATE;;
  NULL;
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

/* define frame moving or marking routine for melthook_HOOK_OVERRIDE_GATEhook */
void
melthookmark_HOOK_OVERRIDE_GATE_rout (struct melt_callframe_st *meltcfram,
				      int meltmarking)
{

  melthookframe_HOOK_OVERRIDE_GATE_t *meltframptr_
    = (melthookframe_HOOK_OVERRIDE_GATE_t *) meltcfram;
  melt_assertmsg ("hook frame HOOK_OVERRIDE_GATE good hash",
		  meltframptr_->mcfr_nbvar == -314);
  if (meltmarking)
    {				/* marking hook HOOK_OVERRIDE_GATE */
      int meltix;
      for (meltix = 0; meltix < 1; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
    }
  else /* copying hook HOOK_OVERRIDE_GATE */ if (melt_is_forwarding)
    {
      int meltvix;
      for (meltvix = 0; meltvix < 1; meltvix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltvix]);
    }				/* end copying hook HOOK_OVERRIDE_GATE */
}				/* end  melthookmark_HOOK_OVERRIDE_GATE_rout */



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
#define MELTFRAM_NBVARPTR 35
  melt_ptr_t mcfr_varptr[35];
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
    struct MELT_OBJECT_STRUCT (3) dsym_1__OVERRIDE_GATE_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (27) dstr_2__OVERRIDE_GATE_DE;
    struct MELT_OBJECT_STRUCT (3) dsym_3__CLASS_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (19) dstr_4__CLASS_DELAYED_QU;
    struct MELT_OBJECT_STRUCT (5) dobj_5__OVERRIDE_GATE_DELAYED_QUEUE;
    struct MELT_STRING_STRUCT (27) dstr_6__override_gate_de;
    struct MELT_OBJECT_STRUCT (3) dsym_7__DISCR_LIST;
    struct MELT_STRING_STRUCT (10) dstr_8__DISCR_LIST;
    struct MELT_HOOK_STRUCT (0) dhook__9__HOOK_OVERRIDE_GATE;
    struct MELT_OBJECT_STRUCT (6) dobj_10__;
    struct MELT_OBJECT_STRUCT (3) dsym_11__BEFOREGATE;
    struct MELT_STRING_STRUCT (10) dstr_12__BEFOREGATE;
    struct MELT_OBJECT_STRUCT (2) dobj_13;
    struct MELT_MULTIPLE_STRUCT (1) dtup_14___HOOK_OVERRIDE_GATE;
    struct MELT_OBJECT_STRUCT (3) dsym_15__AFTERGATE;
    struct MELT_STRING_STRUCT (9) dstr_16__AFTERGATE;
    struct MELT_OBJECT_STRUCT (2) dobj_17;
    struct MELT_MULTIPLE_STRUCT (1) dtup_18___HOOK_OVERRIDE_GATE;
    struct MELT_OBJECT_STRUCT (3) dsym_19__HOOK_OVERRIDE_GATE;
    struct MELT_STRING_STRUCT (18) dstr_20__HOOK_OVERRIDE_GA;
    long spare_;
  } *meltcdat = NULL;
  dbgprintf ("start initialize_module_meltdata_warmelt_hooks iniframp__=%p",
	     (void *) iniframp__);
  (void) meltpredefinited;	/* avoid warning if non-used. */
  melt_assertmsg ("check module initial frame",
		  iniframp__->mcfr_nbvar == /*minihash */ -1797);
  /*curframe_cdat_fill */
  meltcdat = (struct meltcdata_st *) meltgc_allocate (sizeof (*meltcdat), 0);
  melt_prohibit_garbcoll = TRUE;
  /*initial routine predef */
  /*inipredefhook dhook__9__HOOK_OVERRIDE_GATE */
  if (!MELT_PREDEF (HOOK_OVERRIDE_GATE))
    MELT_STORE_PREDEF (HOOK_OVERRIDE_GATE,
		       (melt_ptr_t) & meltcdat->dhook__9__HOOK_OVERRIDE_GATE);
  else
    {
      MELTPREDEFIX (meltpredefinited, HOOK_OVERRIDE_GATE) = 1;
      fnotice (stderr, "MELT: predefined hook %s already defined <%s:%d>\n",
	       "HOOK_OVERRIDE_GATE", __FILE__, __LINE__);
    };

  /*initial routine fill */


/*iniobj dsym_1__OVERRIDE_GATE_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V9*/ meltfptr[8])
/*_.VALDATA___V9*/ meltfptr[8] =
      (melt_ptr_t) & meltcdat->dsym_1__OVERRIDE_GATE_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_1__OVERRIDE_GATE_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_1__OVERRIDE_GATE_DELAYED_QUEUE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_1__OVERRIDE_GATE_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_1__OVERRIDE_GATE_DELAYED_QUEUE.obj_hash = 225417214;
  meltcdat->dsym_1__OVERRIDE_GATE_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_2__OVERRIDE_GATE_DE*/
 /*_.VALSTR___V10*/ meltfptr[9] =
    (melt_ptr_t) & meltcdat->dstr_2__OVERRIDE_GATE_DE;
  meltcdat->dstr_2__OVERRIDE_GATE_DE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_2__OVERRIDE_GATE_DE.val,
				"OVERRIDE_GATE_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_2__OVERRIDE_GATE_DE.
					val) - 1);
  meltcdat->dstr_2__OVERRIDE_GATE_DE.val[27] = (char) 0;
  meltcdat->dstr_2__OVERRIDE_GATE_DE.slen = 27;


/*iniobj dsym_3__CLASS_DELAYED_QUEUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V11*/ meltfptr[10])
/*_.VALDATA___V11*/ meltfptr[10] =
      (melt_ptr_t) & meltcdat->dsym_3__CLASS_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_3__CLASS_DELAYED_QUEUE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_3__CLASS_DELAYED_QUEUE",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_3__CLASS_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_3__CLASS_DELAYED_QUEUE.obj_hash = 761132519;
  meltcdat->dsym_3__CLASS_DELAYED_QUEUE.obj_len = 3;


/*inistring dstr_4__CLASS_DELAYED_QU*/
 /*_.VALSTR___V12*/ meltfptr[11] =
    (melt_ptr_t) & meltcdat->dstr_4__CLASS_DELAYED_QU;
  meltcdat->dstr_4__CLASS_DELAYED_QU.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_4__CLASS_DELAYED_QU.val,
				"CLASS_DELAYED_QUEUE",
				sizeof (meltcdat->dstr_4__CLASS_DELAYED_QU.
					val) - 1);
  meltcdat->dstr_4__CLASS_DELAYED_QU.val[19] = (char) 0;
  meltcdat->dstr_4__CLASS_DELAYED_QU.slen = 19;

  MELT_LOCATION ("warmelt-hooks.melt:29:/ iniobj");

/*iniobj dobj_5__OVERRIDE_GATE_DELAYED_QUEUE*/
/*_.VALDATA___V13*/ meltfptr[12] =
    (melt_ptr_t) & meltcdat->dobj_5__OVERRIDE_GATE_DELAYED_QUEUE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_5__OVERRIDE_GATE_DELAYED_QUEUE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_DELAYED_QUEUE__V7*/ meltfptr[6])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_5__OVERRIDE_GATE_DELAYED_QUEUE",
       ((meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V7*/ meltfptr[6]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_5__OVERRIDE_GATE_DELAYED_QUEUE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V7*/ meltfptr[6]);
  meltcdat->dobj_5__OVERRIDE_GATE_DELAYED_QUEUE.obj_hash = 517542139;
  meltcdat->dobj_5__OVERRIDE_GATE_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_6__override_gate_de*/
 /*_.VALSTR___V16*/ meltfptr[15] =
    (melt_ptr_t) & meltcdat->dstr_6__override_gate_de;
  meltcdat->dstr_6__override_gate_de.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_6__override_gate_de.val,
				"override_gate_delayed_queue",
				sizeof (meltcdat->dstr_6__override_gate_de.
					val) - 1);
  meltcdat->dstr_6__override_gate_de.val[27] = (char) 0;
  meltcdat->dstr_6__override_gate_de.slen = 27;


/*iniobj dsym_7__DISCR_LIST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V17*/ meltfptr[16])
/*_.VALDATA___V17*/ meltfptr[16] =
      (melt_ptr_t) & meltcdat->dsym_7__DISCR_LIST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_7__DISCR_LIST",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_7__DISCR_LIST",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_7__DISCR_LIST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_7__DISCR_LIST.obj_hash = 98407634;
  meltcdat->dsym_7__DISCR_LIST.obj_len = 3;


/*inistring dstr_8__DISCR_LIST*/
 /*_.VALSTR___V18*/ meltfptr[17] =
    (melt_ptr_t) & meltcdat->dstr_8__DISCR_LIST;
  meltcdat->dstr_8__DISCR_LIST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_8__DISCR_LIST.val,
				"DISCR_LIST",
				sizeof (meltcdat->dstr_8__DISCR_LIST.val) -
				1);
  meltcdat->dstr_8__DISCR_LIST.val[10] = (char) 0;
  meltcdat->dstr_8__DISCR_LIST.slen = 10;


/*inihook dhook__9__HOOK_OVERRIDE_GATE */
 /*_.VALHOOK___V19*/ meltfptr[18] =
    (melt_ptr_t) & meltcdat->dhook__9__HOOK_OVERRIDE_GATE;

  meltcdat->dhook__9__HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  meltcdat->dhook__9__HOOK_OVERRIDE_GATE.nbval = 0;
  MELT_HOOK_SET_ROUTCODE (meltcdat, melthook_HOOK_OVERRIDE_GATE);
  meltcdat->dhook__9__HOOK_OVERRIDE_GATE.hookdata =
    /*_.VALDATA___V20*/ meltfptr[19];
  strncpy (meltcdat->dhook__9__HOOK_OVERRIDE_GATE.hookname,
	   "HOOK_OVERRIDE_GATE",
	   sizeof (meltcdat->dhook__9__HOOK_OVERRIDE_GATE.hookname) - 1);
/*predefinedhook HOOK_OVERRIDE_GATE for dhook__9__HOOK_OVERRIDE_GATE*/
/*done inihook dhook__9__HOOK_OVERRIDE_GATE */

  MELT_LOCATION ("warmelt-hooks.melt:35:/ iniobj");

/*iniobj dobj_10__*/
/*_.VALDATA___V20*/ meltfptr[19] = (melt_ptr_t) & meltcdat->dobj_10__;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_10__",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_HOOK_DESCRIPTOR))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_10__",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_10__.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_HOOK_DESCRIPTOR))));
  meltcdat->dobj_10__.obj_hash = 1013919855;
  meltcdat->dobj_10__.obj_len = 6;


/*iniobj dsym_11__BEFOREGATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V23*/ meltfptr[22])
/*_.VALDATA___V23*/ meltfptr[22] =
      (melt_ptr_t) & meltcdat->dsym_11__BEFOREGATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_11__BEFOREGATE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_11__BEFOREGATE",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_11__BEFOREGATE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_11__BEFOREGATE.obj_hash = 841081062;
  meltcdat->dsym_11__BEFOREGATE.obj_len = 3;


/*inistring dstr_12__BEFOREGATE*/
 /*_.VALSTR___V24*/ meltfptr[23] =
    (melt_ptr_t) & meltcdat->dstr_12__BEFOREGATE;
  meltcdat->dstr_12__BEFOREGATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_12__BEFOREGATE.val,
				"BEFOREGATE",
				sizeof (meltcdat->dstr_12__BEFOREGATE.val) -
				1);
  meltcdat->dstr_12__BEFOREGATE.val[10] = (char) 0;
  meltcdat->dstr_12__BEFOREGATE.slen = 10;

  /*^iniobj */

/*iniobj dobj_13*/
/*_.VALDATA___V22*/ meltfptr[21] = (melt_ptr_t) & meltcdat->dobj_13;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dobj_13",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF
					 (CLASS_FORMAL_BINDING))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dobj_13",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_13.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_FORMAL_BINDING))));
  meltcdat->dobj_13.obj_num = 0;
  meltcdat->dobj_13.obj_hash = 604111710;
  meltcdat->dobj_13.obj_len = 2;

  /*inimult dtup_14___HOOK_OVERRIDE_GATE */
 /*_.VALTUP___V25*/ meltfptr[24] =
    (melt_ptr_t) & meltcdat->dtup_14___HOOK_OVERRIDE_GATE;
  meltcdat->dtup_14___HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_14___HOOK_OVERRIDE_GATE.nbval = 1;

/*iniobj dsym_15__AFTERGATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V27*/ meltfptr[26])
/*_.VALDATA___V27*/ meltfptr[26] =
      (melt_ptr_t) & meltcdat->dsym_15__AFTERGATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_15__AFTERGATE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_15__AFTERGATE",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_15__AFTERGATE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_15__AFTERGATE.obj_hash = 561055110;
  meltcdat->dsym_15__AFTERGATE.obj_len = 3;


/*inistring dstr_16__AFTERGATE*/
 /*_.VALSTR___V28*/ meltfptr[27] =
    (melt_ptr_t) & meltcdat->dstr_16__AFTERGATE;
  meltcdat->dstr_16__AFTERGATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_16__AFTERGATE.val, "AFTERGATE",
				sizeof (meltcdat->dstr_16__AFTERGATE.val) -
				1);
  meltcdat->dstr_16__AFTERGATE.val[9] = (char) 0;
  meltcdat->dstr_16__AFTERGATE.slen = 9;

  /*^iniobj */

/*iniobj dobj_17*/
/*_.VALDATA___V26*/ meltfptr[25] = (melt_ptr_t) & meltcdat->dobj_17;
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
  meltcdat->dobj_17.obj_num = 0;
  meltcdat->dobj_17.obj_hash = 719812899;
  meltcdat->dobj_17.obj_len = 2;

  /*inimult dtup_18___HOOK_OVERRIDE_GATE */
 /*_.VALTUP___V29*/ meltfptr[28] =
    (melt_ptr_t) & meltcdat->dtup_18___HOOK_OVERRIDE_GATE;
  meltcdat->dtup_18___HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_18___HOOK_OVERRIDE_GATE.nbval = 1;

/*iniobj dsym_19__HOOK_OVERRIDE_GATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V30*/ meltfptr[29])
/*_.VALDATA___V30*/ meltfptr[29] =
      (melt_ptr_t) & meltcdat->dsym_19__HOOK_OVERRIDE_GATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_19__HOOK_OVERRIDE_GATE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_19__HOOK_OVERRIDE_GATE",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_19__HOOK_OVERRIDE_GATE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_19__HOOK_OVERRIDE_GATE.obj_hash = 995989956;
  meltcdat->dsym_19__HOOK_OVERRIDE_GATE.obj_len = 3;


/*inistring dstr_20__HOOK_OVERRIDE_GA*/
 /*_.VALSTR___V31*/ meltfptr[30] =
    (melt_ptr_t) & meltcdat->dstr_20__HOOK_OVERRIDE_GA;
  meltcdat->dstr_20__HOOK_OVERRIDE_GA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_20__HOOK_OVERRIDE_GA.val,
				"HOOK_OVERRIDE_GATE",
				sizeof (meltcdat->dstr_20__HOOK_OVERRIDE_GA.
					val) - 1);
  meltcdat->dstr_20__HOOK_OVERRIDE_GA.val[18] = (char) 0;
  meltcdat->dstr_20__HOOK_OVERRIDE_GA.slen = 18;
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
  static long call_counter__;
  long thiscallcounter__ ATTRIBUTE_UNUSED = ++call_counter__;
#undef meltcallcount
#define meltcallcount thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  melt_initial_frame_st meltfram__;	/*declfrastruct */
#define meltframe meltfram__

  /*melt_start_this_module is initial declstructinit */
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit initial routine melt_start_this_module minihash 1797*/

  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
/**initial routine prologue**/
  /* set initial frame marking */
  ((struct melt_callframe_st *) &meltfram__)->mcfr_nbvar =
    /*minihash */ -1797;
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
 /**COMMENT: get symbols & keywords **/ ;

  /*^getnamedsymbol */
  /*getnamedsym:OVERRIDE_GATE_DELAYED_QUEUE */
  {
    melt_ptr_t sy_OVERRIDE_GATE_DELAYED_QUEUE =
      meltgc_named_symbol ("OVERRIDE_GATE_DELAYED_QUEUE", MELT_GET);
    if (sy_OVERRIDE_GATE_DELAYED_QUEUE
	&& NULL == /*_.VALDATA___V9*/ meltfptr[8])
  /*_.VALDATA___V9*/ meltfptr[8] =
	(melt_ptr_t) sy_OVERRIDE_GATE_DELAYED_QUEUE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:CLASS_DELAYED_QUEUE */
  {
    melt_ptr_t sy_CLASS_DELAYED_QUEUE =
      meltgc_named_symbol ("CLASS_DELAYED_QUEUE", MELT_GET);
    if (sy_CLASS_DELAYED_QUEUE && NULL == /*_.VALDATA___V11*/ meltfptr[10])
  /*_.VALDATA___V11*/ meltfptr[10] = (melt_ptr_t) sy_CLASS_DELAYED_QUEUE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:DISCR_LIST */
  {
    melt_ptr_t sy_DISCR_LIST = meltgc_named_symbol ("DISCR_LIST", MELT_GET);
    if (sy_DISCR_LIST && NULL == /*_.VALDATA___V17*/ meltfptr[16])
  /*_.VALDATA___V17*/ meltfptr[16] = (melt_ptr_t) sy_DISCR_LIST;
  }

  /*^getnamedsymbol */
  /*getnamedsym:BEFOREGATE */
  {
    melt_ptr_t sy_BEFOREGATE = meltgc_named_symbol ("BEFOREGATE", MELT_GET);
    if (sy_BEFOREGATE && NULL == /*_.VALDATA___V23*/ meltfptr[22])
  /*_.VALDATA___V23*/ meltfptr[22] = (melt_ptr_t) sy_BEFOREGATE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:AFTERGATE */
  {
    melt_ptr_t sy_AFTERGATE = meltgc_named_symbol ("AFTERGATE", MELT_GET);
    if (sy_AFTERGATE && NULL == /*_.VALDATA___V27*/ meltfptr[26])
  /*_.VALDATA___V27*/ meltfptr[26] = (melt_ptr_t) sy_AFTERGATE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:HOOK_OVERRIDE_GATE */
  {
    melt_ptr_t sy_HOOK_OVERRIDE_GATE =
      meltgc_named_symbol ("HOOK_OVERRIDE_GATE", MELT_GET);
    if (sy_HOOK_OVERRIDE_GATE && NULL == /*_.VALDATA___V30*/ meltfptr[29])
  /*_.VALDATA___V30*/ meltfptr[29] = (melt_ptr_t) sy_HOOK_OVERRIDE_GATE;
  }

  /*^comment */
 /**COMMENT: get the value importer **/ ;

  /*^getslot */
  {
    melt_ptr_t slot = NULL, obj = NULL;
    obj =
      (melt_ptr_t) (((melt_ptr_t) (MELT_PREDEF (INITIAL_SYSTEM_DATA))))
      /*=obj*/ ;
    melt_object_get_field (slot, obj, 12, "SYSDATA_VALUE_IMPORTER");
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
    argtab[1].meltbp_cstring = "CLASS_DELAYED_QUEUE";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.CLASS_DELAYED_QUEUE__V7*/ meltfptr[6] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V11*/ meltfptr[10]),
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
    /*_.DISCR_LIST__V8*/ meltfptr[7] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V17*/ meltfptr[16]),
		  (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING
		   ""), argtab, "", (union meltparam_un *) 0);
  }

  /*^comment */
 /**COMMENT: after getting imported values **/ ;

 /**initial routine cdata initializer**/

  memset (meltpredefinited, 0, sizeof (meltpredefinited));
  initialize_module_meltdata_warmelt_hooks (&meltfram__, meltpredefinited);
 /**initial routine body**/

  meltmod__warmelt_hooks__initialmeltchunk_0 (&meltfram__, meltpredefinited);;
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
				    ( /*_.VALDATA___V9*/ meltfptr[8])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V9*/ meltfptr[8]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V9*/ meltfptr[8]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V10*/ meltfptr[9]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V9*/ meltfptr[8]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V9*/ meltfptr[8],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V11*/ meltfptr[10])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V11*/ meltfptr[10]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V11*/ meltfptr[10]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V12*/ meltfptr[11]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V11*/ meltfptr[10]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V11*/ meltfptr[10],
				"new static instance");

 /*_.MAKE_LIST__V14*/ meltfptr[13] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V8*/ meltfptr[7])));
 /*_.MAKE_LIST__V15*/ meltfptr[14] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V8*/ meltfptr[7])));
  MELT_LOCATION ("warmelt-hooks.melt:29:/ initchunk");
  /*^putslot */
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
  /*_.VALSTR___V16*/ meltfptr[15]);

  MELT_LOCATION ("warmelt-hooks.melt:29:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V13*/ meltfptr[12])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V13*/ meltfptr[12]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V13*/ meltfptr[12]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V14*/ meltfptr[13]);

  MELT_LOCATION ("warmelt-hooks.melt:29:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V13*/ meltfptr[12])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V13*/ meltfptr[12]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V13*/ meltfptr[12]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V15*/ meltfptr[14]);

  MELT_LOCATION ("warmelt-hooks.melt:29:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V13*/ meltfptr[12]);

  MELT_LOCATION ("warmelt-hooks.melt:29:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V13*/ meltfptr[12],
				"new static instance");

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
  /*_.VALSTR___V18*/ meltfptr[17]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V17*/ meltfptr[16]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V17*/ meltfptr[16],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
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
  /*_.VALSTR___V21*/ meltfptr[20]);

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
  /*_.VALSTR___V24*/ meltfptr[23]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V23*/ meltfptr[22]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V23*/ meltfptr[22],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V22*/ meltfptr[21])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V22*/ meltfptr[21]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V22*/ meltfptr[21]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V23*/ meltfptr[22]);

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V22*/ meltfptr[21])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V22*/ meltfptr[21]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V22*/ meltfptr[21]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V22*/ meltfptr[21]);

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V22*/ meltfptr[21],
				"new static instance");

  /*putupl#1 */
  melt_assertmsg ("putupl #1 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V25*/ meltfptr[24])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #1 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V25*/
					  meltfptr[24]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V25*/ meltfptr[24]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V22*/ meltfptr[21]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V25*/ meltfptr[24]);

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V20*/ meltfptr[19])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V20*/ meltfptr[19]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V20*/ meltfptr[19]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V25*/ meltfptr[24]);

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

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
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

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
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
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V26*/ meltfptr[25]);

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V26*/ meltfptr[25],
				"new static instance");

  /*putupl#2 */
  melt_assertmsg ("putupl #2 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V29*/ meltfptr[28])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #2 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V29*/
					  meltfptr[28]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V29*/ meltfptr[28]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V26*/ meltfptr[25]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V29*/ meltfptr[28]);

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V20*/ meltfptr[19])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V20*/ meltfptr[19]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V20*/ meltfptr[19]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V29*/ meltfptr[28]);

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V20*/ meltfptr[19])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V20*/ meltfptr[19]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V20*/ meltfptr[19]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V20*/ meltfptr[19]);

  MELT_LOCATION ("warmelt-hooks.melt:35:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V20*/ meltfptr[19],
				"new static instance");

  /*put data in hook HOOK_OVERRIDE_GATE */
  ((melthook_ptr_t) /*_.VALHOOK___V19*/ meltfptr[18])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V20*/ meltfptr[19]);

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
  /*_.OR___V32*/ meltfptr[31] =
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
     /*_.SYSDATA_CONT_FRESH_ENV__V33*/ meltfptr[32] = slot;
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
	    /*_.FUN___V34*/ meltfptr[33] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.SYSDATA_CONT_FRESH_ENV__V33*/ meltfptr[32]),
			  (melt_ptr_t) ( /*_.PREVENV___V4*/ meltfptr[3]),
			  (MELTBPARSTR_CSTRING ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*_.OR___V32*/ meltfptr[31] = /*_.FUN___V34*/ meltfptr[33];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:4:/ clear");
	      /*clear *//*_.SYSDATA_CONT_FRESH_ENV__V33*/ meltfptr[32] = 0;
	  /*^clear */
	      /*clear *//*_.FUN___V34*/ meltfptr[33] = 0;
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
	    /*clear *//*_.OR___V32*/ meltfptr[31] = 0;
  }

  MELT_LOCATION ("warmelt-hooks.melt:119:/ initchunk");
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
   /*_.VALUEXPORTER___V35*/ meltfptr[32] = slot;
    };
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALHOOK___V19*/ meltfptr[18];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : HOOK_OVERRIDE_GATE */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V35*/ meltfptr[32]),
		  (melt_ptr_t) ( /*_.VALDATA___V30*/ meltfptr[29]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*epilog */

    /*^clear */
	    /*clear *//*_.VALUEXPORTER___V35*/ meltfptr[32] = 0;
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

  /*internsym:OVERRIDE_GATE_DELAYED_QUEUE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V9*/ meltfptr[8]));

  /*internsym:CLASS_DELAYED_QUEUE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V11*/ meltfptr[10]));

  /*internsym:DISCR_LIST */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V17*/ meltfptr[16]));

  /*internsym:BEFOREGATE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V23*/ meltfptr[22]));

  /*internsym:AFTERGATE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V27*/ meltfptr[26]));

  /*internsym:HOOK_OVERRIDE_GATE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V30*/ meltfptr[29]));

 /**COMMENT: set retinit from boxloc **/ ;

 /*_.RETINIT___V1*/ meltfptr[0] =
    /* finalsetretinit */
    melt_reference_value ((melt_ptr_t) ( /*_.CONTENV___V2*/ meltfptr[1]));
 /**COMMENT: end the initproc **/ ;


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_0 */

void
meltmod__warmelt_hooks__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking)
{
  int meltix = 0;
  melt_initial_frame_st *meltframptr_ = (melt_initial_frame_st *) fp;
  melt_assertmsg ("check module frame",
		  meltframptr_->mcfr_nbvar == /*minihash */ -1797);
  if (!marking && melt_is_forwarding)
    {
      dbgprintf
	("forward_or_mark_module_start_frame_warmelt_hooks forwarding %d pointers in frame %p",
	 35, (void *) meltframptr_);
      for (meltix = 0; meltix < 35; meltix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltix]);
      return;
    }				/*end forwarding */
  dbgprintf
    ("forward_or_mark_module_start_frame_warmelt_hooks marking in frame %p",
     (void *) meltframptr_);
  for (meltix = 0; meltix < 35; meltix++)
    if (meltframptr_->mcfr_varptr[meltix])
      gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);

}				/* end meltmod__warmelt_hooks__forward_or_mark_module_start_frame */



/* exported 0 field offsets */

/* exported 0 class lengths */



/**** end of warmelt-hooks ****/
