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


/** declaration of hook melthook_HOOK_LOW_DEBUG_VALUE_AT */
MELT_EXTERN
  void melthook_HOOK_LOW_DEBUG_VALUE_AT (melt_ptr_t melthookdata,
					 melt_ptr_t meltinp0_VAL,
					 const char *meltinp1_FILENAME,
					 long meltinp2_LINENO,
					 const char *meltinp3_MSG,
					 long meltinp4_COUNT);


/* declare hook frame marking routine for melthook_HOOK_LOW_DEBUG_VALUE_AT hook */
static void melthookmark_HOOK_LOW_DEBUG_VALUE_AT_rout (struct
						       melt_callframe_st *,
						       int);




/** declaration of hook melthook_HOOK_OVERRIDE_GATE */
MELT_EXTERN
  void melthook_HOOK_OVERRIDE_GATE (melt_ptr_t melthookdata,
				    long meltinp0_BEFOREGATE,
				    long *meltoutp0_AFTERGATE);


/* declare hook frame marking routine for melthook_HOOK_OVERRIDE_GATE hook */
static void melthookmark_HOOK_OVERRIDE_GATE_rout (struct melt_callframe_st *,
						  int);







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
meltmod__warmelt_hooks__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking);



/**** warmelt-hooks implementations ****/
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
  meltframe.mcfr_nbvar = -1661 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -1661);
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
  meltframe.mcfr_nbvar = -2185 /*hook minihash */ ;
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
		  meltframptr_->mcfr_nbvar == -2185);
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
#define MELTFRAM_NBVARPTR 2
    melt_ptr_t mcfr_varptr[2];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
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
      for (meltix = 0; meltix < 2; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE nbval 2*/
  meltfram__.mcfr_nbvar = 2 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ENABLE_OVERRIDE_GATE", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:137:/ block");
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:138:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!REGISTERED_FLAG_OVERRIDE_GATE */
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
	    (melt_ptr_t) (( /*!REGISTERED_FLAG_OVERRIDE_GATE */ meltfrout->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
   /*_.REFERENCED_VALUE__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.REFERENCED_VALUE__V2*/ meltfptr[1] = NULL;;
      }
    ;
    /*^compute */
 /*_#NULL__L1*/ meltfnum[0] =
      (( /*_.REFERENCED_VALUE__V2*/ meltfptr[1]) == NULL);;
    MELT_LOCATION ("warmelt-hooks.melt:138:/ cond");
    /*cond */ if ( /*_#NULL__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:139:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!REGISTERED_FLAG_OVERRIDE_GATE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[1])))
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
						  (( /*!REGISTERED_FLAG_OVERRIDE_GATE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		melt_putfield_object ((( /*!REGISTERED_FLAG_OVERRIDE_GATE */
					meltfrout->tabval[0])), (0),
				      (( /*!konst_2_TRUE */ meltfrout->
					tabval[2])), "REFERENCED_VALUE");
		;
		/*^touch */
		meltgc_touch (( /*!REGISTERED_FLAG_OVERRIDE_GATE */
			       meltfrout->tabval[0]));
		;
		/*^touchobj */

		melt_dbgtrace_written_object (( /*!REGISTERED_FLAG_OVERRIDE_GATE */ meltfrout->tabval[0]), "put-fields");
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:140:/ locexp");
	    /* enable_override_gate REGPLUGIN_CHK__1 */
	    register_callback (melt_plugin_name, PLUGIN_OVERRIDE_GATE,
			       melt_override_gate_callback, NULL);
	    ;
	  }
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:138:/ quasiblock");


	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:137:/ clear");
	   /*clear *//*_.REFERENCED_VALUE__V2*/ meltfptr[1] = 0;
    /*^clear */
	   /*clear *//*_#NULL__L1*/ meltfnum[0] = 0;
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
#define MELTFRAM_NBVARNUM 4
    long mcfr_varnum[4];
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
  MELT_LOCATION ("warmelt-hooks.melt:150:/ block");
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:151:/ cond");
    /*cond */ if (( /*!REGISTERED_FLAG_OVERRIDE_GATE */ meltfrout->tabval[0]))	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:152:/ cond");
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
     /*_.DELQU_FIRST__V2*/ meltfptr[1] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.DELQU_FIRST__V2*/ meltfptr[1] = NULL;;
	    }
	  ;
	  /*^compute */
   /*_.LIST_FIRST__V3*/ meltfptr[2] =
	    (melt_list_first
	     ((melt_ptr_t) ( /*_.DELQU_FIRST__V2*/ meltfptr[1])));;
	  /*^compute */
   /*_#NULL__L2*/ meltfnum[1] =
	    (( /*_.LIST_FIRST__V3*/ meltfptr[2]) == NULL);;
	  MELT_LOCATION ("warmelt-hooks.melt:151:/ cond");
	  /*cond */ if ( /*_#NULL__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-hooks.melt:153:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!OVERRIDE_GATE_DELAYED_QUEUE */ meltfrout->tabval[1])),
						    (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[2])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^getslot */
		    {
		      melt_ptr_t slot = NULL, obj = NULL;
		      obj =
			(melt_ptr_t) (( /*!OVERRIDE_GATE_DELAYED_QUEUE */
				       meltfrout->tabval[1])) /*=obj*/ ;
		      melt_object_get_field (slot, obj, 3, "DELQU_LAST");
       /*_.DELQU_LAST__V4*/ meltfptr[3] = slot;
		    };
		    ;
		  }
		else
		  {		/*^cond.else */

      /*_.DELQU_LAST__V4*/ meltfptr[3] = NULL;;
		  }
		;
		/*^compute */
     /*_.LIST_FIRST__V5*/ meltfptr[4] =
		  (melt_list_first
		   ((melt_ptr_t) ( /*_.DELQU_LAST__V4*/ meltfptr[3])));;
		/*^compute */
     /*_#NULL__L4*/ meltfnum[3] =
		  (( /*_.LIST_FIRST__V5*/ meltfptr[4]) == NULL);;
		/*^compute */
		/*_#IF___L3*/ meltfnum[2] = /*_#NULL__L4*/ meltfnum[3];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:151:/ clear");
	       /*clear *//*_.DELQU_LAST__V4*/ meltfptr[3] = 0;
		/*^clear */
	       /*clear *//*_.LIST_FIRST__V5*/ meltfptr[4] = 0;
		/*^clear */
	       /*clear *//*_#NULL__L4*/ meltfnum[3] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_#IF___L3*/ meltfnum[2] = 0;;
	    }
	  ;
	  /*^compute */
	  /*_#IF___L1*/ meltfnum[0] = /*_#IF___L3*/ meltfnum[2];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:151:/ clear");
	     /*clear *//*_.DELQU_FIRST__V2*/ meltfptr[1] = 0;
	  /*^clear */
	     /*clear *//*_.LIST_FIRST__V3*/ meltfptr[2] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L2*/ meltfnum[1] = 0;
	  /*^clear */
	     /*clear *//*_#IF___L3*/ meltfnum[2] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_#IF___L1*/ meltfnum[0] = 0;;
      }
    ;
    MELT_LOCATION ("warmelt-hooks.melt:151:/ cond");
    /*cond */ if ( /*_#IF___L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:154:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!REGISTERED_FLAG_OVERRIDE_GATE */ meltfrout->tabval[0])),
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
						  (( /*!REGISTERED_FLAG_OVERRIDE_GATE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		melt_putfield_object ((( /*!REGISTERED_FLAG_OVERRIDE_GATE */
					meltfrout->tabval[0])), (0),
				      (( /*nil */ NULL)), "REFERENCED_VALUE");
		;
		/*^touch */
		meltgc_touch (( /*!REGISTERED_FLAG_OVERRIDE_GATE */
			       meltfrout->tabval[0]));
		;
		/*^touchobj */

		melt_dbgtrace_written_object (( /*!REGISTERED_FLAG_OVERRIDE_GATE */ meltfrout->tabval[0]), "put-fields");
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:155:/ locexp");
	    /* disable_override_gate UNREGPLUGIN_CHK__1 */
	    unregister_callback (melt_plugin_name, PLUGIN_OVERRIDE_GATE);
	    ;
	  }
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:151:/ quasiblock");


	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:150:/ clear");
	   /*clear *//*_#IF___L1*/ meltfnum[0] = 0;
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
  MELT_LOCATION ("warmelt-hooks.melt:163:/ getarg");
 /*_.CLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:166:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:166:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:166:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 166;
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

	    MELT_LOCATION ("warmelt-hooks.melt:166:/ clear");
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
      MELT_LOCATION ("warmelt-hooks.melt:166:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_.IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:166:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:167:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:168:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:168:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.DELQU_FIRST__V8*/ meltfptr[4]),
				(melt_ptr_t) ( /*_.CLO__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:169:/ apply");
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
	  MELT_LOCATION ("warmelt-hooks.melt:167:/ quasiblock");


	  /*_.PROGN___V10*/ meltfptr[9] =
	    /*_.ENABLE_OVERRIDE_GATE__V9*/ meltfptr[8];;
	  /*^compute */
	  /*_.IF___V7*/ meltfptr[3] = /*_.PROGN___V10*/ meltfptr[9];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:167:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:163:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = /*_.IF___V7*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:163:/ locexp");
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
  MELT_LOCATION ("warmelt-hooks.melt:171:/ getarg");
 /*_.CLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:174:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:174:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:174:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 174;
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

	    MELT_LOCATION ("warmelt-hooks.melt:174:/ clear");
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
      MELT_LOCATION ("warmelt-hooks.melt:174:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_.IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:174:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:175:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:176:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:176:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.DELQU_LAST__V8*/ meltfptr[4]),
				(melt_ptr_t) ( /*_.CLO__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:177:/ apply");
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
	  MELT_LOCATION ("warmelt-hooks.melt:175:/ quasiblock");


	  /*_.PROGN___V10*/ meltfptr[9] =
	    /*_.ENABLE_OVERRIDE_GATE__V9*/ meltfptr[8];;
	  /*^compute */
	  /*_.IF___V7*/ meltfptr[3] = /*_.PROGN___V10*/ meltfptr[9];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:175:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:171:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = /*_.IF___V7*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:171:/ locexp");
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
  MELT_LOCATION ("warmelt-hooks.melt:179:/ getarg");
 /*_.CLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:183:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:183:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:183:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 183;
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

	    MELT_LOCATION ("warmelt-hooks.melt:183:/ clear");
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
      MELT_LOCATION ("warmelt-hooks.melt:183:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_.IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:183:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:184:/ quasiblock");


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
	  MELT_LOCATION ("warmelt-hooks.melt:191:/ cond");
	  /*cond */ if ( /*_#__L4*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

    /*_#I__L6*/ meltfnum[5] =
		  (( /*_#COUNT__L3*/ meltfnum[1]) + (1));;
		MELT_LOCATION ("warmelt-hooks.melt:192:/ compute");
		/*_#COUNT__L3*/ meltfnum[1] = /*_#SETQ___L7*/ meltfnum[6] =
		  /*_#I__L6*/ meltfnum[5];;
		/*_#IFELSE___L5*/ meltfnum[4] = /*_#SETQ___L7*/ meltfnum[6];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:191:/ clear");
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
		  MELT_LOCATION ("warmelt-hooks.melt:193:/ locexp");
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

      MELT_LOCATION ("warmelt-hooks.melt:188:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:194:/ cond");
    /*cond */ if ( /*_#COUNT__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:195:/ cond");
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
	  MELT_LOCATION ("warmelt-hooks.melt:196:/ apply");
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
	  MELT_LOCATION ("warmelt-hooks.melt:197:/ cppif.then");
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
	    MELT_LOCATION ("warmelt-hooks.melt:197:/ cond");
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
		  MELT_LOCATION ("warmelt-hooks.melt:197:/ apply");
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
		    argtab[2].meltbp_long = 197;
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

		  MELT_LOCATION ("warmelt-hooks.melt:197:/ clear");
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
	    MELT_LOCATION ("warmelt-hooks.melt:197:/ quasiblock");


	    /*_.PROGN___V17*/ meltfptr[15] = /*_.IF___V15*/ meltfptr[14];;
	    /*^compute */
	    /*_.IFCPP___V14*/ meltfptr[13] = /*_.PROGN___V17*/ meltfptr[15];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:197:/ clear");
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
	  MELT_LOCATION ("warmelt-hooks.melt:198:/ quasiblock");


	  /*_.RETVAL___V1*/ meltfptr[0] = /*_.BOX__V18*/ meltfptr[14];;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:198:/ locexp");
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
	  MELT_LOCATION ("warmelt-hooks.melt:194:/ quasiblock");


	  /*_.PROGN___V20*/ meltfptr[19] = /*_.RETURN___V19*/ meltfptr[15];;
	  /*^compute */
	  /*_.IF___V12*/ meltfptr[11] = /*_.PROGN___V20*/ meltfptr[19];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:194:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:199:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:199:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:199:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[4];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[5];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 199;
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

	    MELT_LOCATION ("warmelt-hooks.melt:199:/ clear");
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
      MELT_LOCATION ("warmelt-hooks.melt:199:/ quasiblock");


      /*_.PROGN___V24*/ meltfptr[15] = /*_.IF___V22*/ meltfptr[13];;
      /*^compute */
      /*_.IFCPP___V21*/ meltfptr[12] = /*_.PROGN___V24*/ meltfptr[15];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:199:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:200:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = ( /*nil */ NULL);;

    {
      MELT_LOCATION ("warmelt-hooks.melt:200:/ locexp");
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

    MELT_LOCATION ("warmelt-hooks.melt:184:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:179:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = /*_.LET___V7*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:179:/ locexp");
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
  MELT_LOCATION ("warmelt-hooks.melt:202:/ getarg");
 /*_.CLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-hooks.melt:206:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:206:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:206:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 206;
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

	    MELT_LOCATION ("warmelt-hooks.melt:206:/ clear");
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
      MELT_LOCATION ("warmelt-hooks.melt:206:/ quasiblock");


      /*_.PROGN___V6*/ meltfptr[4] = /*_.IF___V4*/ meltfptr[3];;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_.PROGN___V6*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:206:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:207:/ quasiblock");


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
	  MELT_LOCATION ("warmelt-hooks.melt:214:/ cond");
	  /*cond */ if ( /*_#__L4*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

    /*_#I__L6*/ meltfnum[5] =
		  (( /*_#COUNT__L3*/ meltfnum[1]) + (1));;
		MELT_LOCATION ("warmelt-hooks.melt:215:/ compute");
		/*_#COUNT__L3*/ meltfnum[1] = /*_#SETQ___L7*/ meltfnum[6] =
		  /*_#I__L6*/ meltfnum[5];;
		/*_#IFELSE___L5*/ meltfnum[4] = /*_#SETQ___L7*/ meltfnum[6];;
		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:214:/ clear");
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
		  MELT_LOCATION ("warmelt-hooks.melt:216:/ locexp");
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

      MELT_LOCATION ("warmelt-hooks.melt:211:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:217:/ cond");
    /*cond */ if ( /*_#COUNT__L3*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:218:/ cond");
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
	  MELT_LOCATION ("warmelt-hooks.melt:219:/ apply");
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
	  MELT_LOCATION ("warmelt-hooks.melt:220:/ cppif.then");
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
	    MELT_LOCATION ("warmelt-hooks.melt:220:/ cond");
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
		  MELT_LOCATION ("warmelt-hooks.melt:220:/ apply");
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
		    argtab[2].meltbp_long = 220;
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

		  MELT_LOCATION ("warmelt-hooks.melt:220:/ clear");
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
	    MELT_LOCATION ("warmelt-hooks.melt:220:/ quasiblock");


	    /*_.PROGN___V17*/ meltfptr[15] = /*_.IF___V15*/ meltfptr[14];;
	    /*^compute */
	    /*_.IFCPP___V14*/ meltfptr[13] = /*_.PROGN___V17*/ meltfptr[15];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-hooks.melt:220:/ clear");
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
	  MELT_LOCATION ("warmelt-hooks.melt:221:/ quasiblock");


	  /*_.RETVAL___V1*/ meltfptr[0] = /*_.BOX__V18*/ meltfptr[14];;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:221:/ locexp");
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
	  MELT_LOCATION ("warmelt-hooks.melt:217:/ quasiblock");


	  /*_.PROGN___V20*/ meltfptr[19] = /*_.RETURN___V19*/ meltfptr[15];;
	  /*^compute */
	  /*_.IF___V12*/ meltfptr[11] = /*_.PROGN___V20*/ meltfptr[19];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:217:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:222:/ cppif.then");
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
      MELT_LOCATION ("warmelt-hooks.melt:222:/ cond");
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
	    MELT_LOCATION ("warmelt-hooks.melt:222:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[4];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[5];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-hooks.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 222;
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

	    MELT_LOCATION ("warmelt-hooks.melt:222:/ clear");
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
      MELT_LOCATION ("warmelt-hooks.melt:222:/ quasiblock");


      /*_.PROGN___V24*/ meltfptr[15] = /*_.IF___V22*/ meltfptr[13];;
      /*^compute */
      /*_.IFCPP___V21*/ meltfptr[12] = /*_.PROGN___V24*/ meltfptr[15];;
      /*epilog */

      MELT_LOCATION ("warmelt-hooks.melt:222:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:223:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = ( /*nil */ NULL);;

    {
      MELT_LOCATION ("warmelt-hooks.melt:223:/ locexp");
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

    MELT_LOCATION ("warmelt-hooks.melt:207:/ clear");
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
    MELT_LOCATION ("warmelt-hooks.melt:202:/ quasiblock");


    /*_.RETVAL___V1*/ meltfptr[0] = /*_.LET___V7*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-hooks.melt:202:/ locexp");
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
#define MELTFRAM_NBVARPTR 88
  melt_ptr_t mcfr_varptr[88];
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
    struct MELT_OBJECT_STRUCT (3) dsym_44__REGISTERED_FLAG_OVERRIDE_GATE;
    struct MELT_STRING_STRUCT (29) dstr_45__REGISTERED_FLAG_;
    struct MELT_OBJECT_STRUCT (1) dobj_46__REGISTERED_FLAG_OVERRIDE_GATE;
    struct MELT_ROUTINE_STRUCT (3) drout_47__ENABLE_OVERRIDE_GATE;
    struct MELT_CLOSURE_STRUCT (0) dclo_48__ENABLE_OVERRIDE_GATE;
    struct MELT_ROUTINE_STRUCT (4) drout_49__MAYBE_DISABLE_OVERRIDE_GATE;
    struct MELT_CLOSURE_STRUCT (0) dclo_50__MAYBE_DISABLE_OVERRIDE_GATE;
    struct MELT_ROUTINE_STRUCT (4) drout_51__REGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_52__REGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_ROUTINE_STRUCT (4) drout_53__REGISTER_OVERRIDE_GATE_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_54__REGISTER_OVERRIDE_GATE_LAST;
    struct MELT_ROUTINE_STRUCT (5) drout_55__UNREGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_CLOSURE_STRUCT (0) dclo_56__UNREGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_ROUTINE_STRUCT (5) drout_57__UNREGISTER_OVERRIDE_GATE_LAST;
    struct MELT_CLOSURE_STRUCT (0) dclo_58__UNREGISTER_OVERRIDE_GATE_LAST;
    struct MELT_OBJECT_STRUCT (3) dsym_59__HOOK_OVERRIDE_GATE;
    struct MELT_STRING_STRUCT (18) dstr_60__HOOK_OVERRIDE_GA;
    struct MELT_OBJECT_STRUCT (3) dsym_61__HOOK_LOW_DEBUG_VALUE_AT;
    struct MELT_STRING_STRUCT (23) dstr_62__HOOK_LOW_DEBUG_V;
    struct MELT_OBJECT_STRUCT (3) dsym_63__REGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_STRING_STRUCT (28) dstr_64__REGISTER_OVERRID;
    struct MELT_OBJECT_STRUCT (3) dsym_65__REGISTER_OVERRIDE_GATE_LAST;
    struct MELT_STRING_STRUCT (27) dstr_66__REGISTER_OVERRID;
    struct MELT_OBJECT_STRUCT (3) dsym_67__UNREGISTER_OVERRIDE_GATE_FIRST;
    struct MELT_STRING_STRUCT (30) dstr_68__UNREGISTER_OVERR;
    struct MELT_OBJECT_STRUCT (3) dsym_69__UNREGISTER_OVERRIDE_GATE_LAST;
    struct MELT_STRING_STRUCT (29) dstr_70__UNREGISTER_OVERR;
    long spare_;
  } *meltcdat = NULL;
  dbgprintf ("start initialize_module_meltdata_warmelt_hooks iniframp__=%p",
	     (void *) iniframp__);
  (void) meltpredefinited;	/* avoid warning if non-used. */
  melt_assertmsg ("check module initial frame",
		  iniframp__->mcfr_nbvar == /*minihash */ -1637);
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

  /*initial routine fill */


/*inihook dhook__1__HOOK_LOW_DEBUG_VALUE_AT */
  meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_HOOK))));
  /*inihook HOOK_LOW_DEBUG_VALUE_AT data */
  meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.nbval = 3;
  MELT_HOOK_SET_ROUTCODE (&meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT,
			  melthook_HOOK_LOW_DEBUG_VALUE_AT);
  meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.hookdata =
    /*_.VALDATA___V14*/ meltfptr[13];
  strncpy (meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.hookname,
	   "HOOK_LOW_DEBUG_VALUE_AT",
	   sizeof (meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT.hookname) - 1);
/*predefinedhook HOOK_LOW_DEBUG_VALUE_AT for dhook__1__HOOK_LOW_DEBUG_VALUE_AT*/
  /*_.VALHOOK___V11*/ meltfptr[10] =
  /*inihooklocvar HOOK_LOW_DEBUG_VALUE_AT */
  (melt_ptr_t) & meltcdat->dhook__1__HOOK_LOW_DEBUG_VALUE_AT;

  /*done inihook dhook__1__HOOK_LOW_DEBUG_VALUE_AT */

  MELT_LOCATION ("warmelt-hooks.melt:32:/ iniobj");

/*iniobj dobj_2__*/
/*_.VALDATA___V14*/ meltfptr[13] = (melt_ptr_t) & meltcdat->dobj_2__;
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
  meltcdat->dobj_2__.obj_hash = 372954826;
  meltcdat->dobj_2__.obj_len = 6;


/*iniobj dsym_3__VAL*/
  /*uniqueobj*/ if (! /*_.VALDATA___V17*/ meltfptr[16])
/*_.VALDATA___V17*/ meltfptr[16] = (melt_ptr_t) & meltcdat->dsym_3__VAL;
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
 /*_.VALSTR___V18*/ meltfptr[17] = (melt_ptr_t) & meltcdat->dstr_4__VAL;
  meltcdat->dstr_4__VAL.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_4__VAL.val, "VAL",
				sizeof (meltcdat->dstr_4__VAL.val) - 1);
  meltcdat->dstr_4__VAL.val[3] = (char) 0;
  meltcdat->dstr_4__VAL.slen = 3;

  /*^iniobj */

/*iniobj dobj_5*/
/*_.VALDATA___V16*/ meltfptr[15] = (melt_ptr_t) & meltcdat->dobj_5;
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
  meltcdat->dobj_5.obj_hash = 87049035;
  meltcdat->dobj_5.obj_len = 2;


/*iniobj dsym_6__FILENAME*/
  /*uniqueobj*/ if (! /*_.VALDATA___V20*/ meltfptr[19])
/*_.VALDATA___V20*/ meltfptr[19] =
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
 /*_.VALSTR___V21*/ meltfptr[20] = (melt_ptr_t) & meltcdat->dstr_7__FILENAME;
  meltcdat->dstr_7__FILENAME.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_7__FILENAME.val, "FILENAME",
				sizeof (meltcdat->dstr_7__FILENAME.val) - 1);
  meltcdat->dstr_7__FILENAME.val[8] = (char) 0;
  meltcdat->dstr_7__FILENAME.slen = 8;

  /*^iniobj */

/*iniobj dobj_8*/
/*_.VALDATA___V19*/ meltfptr[18] = (melt_ptr_t) & meltcdat->dobj_8;
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
  meltcdat->dobj_8.obj_hash = 941124219;
  meltcdat->dobj_8.obj_len = 2;


/*iniobj dsym_9__LINENO*/
  /*uniqueobj*/ if (! /*_.VALDATA___V23*/ meltfptr[22])
/*_.VALDATA___V23*/ meltfptr[22] =
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
 /*_.VALSTR___V24*/ meltfptr[23] = (melt_ptr_t) & meltcdat->dstr_10__LINENO;
  meltcdat->dstr_10__LINENO.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_10__LINENO.val, "LINENO",
				sizeof (meltcdat->dstr_10__LINENO.val) - 1);
  meltcdat->dstr_10__LINENO.val[6] = (char) 0;
  meltcdat->dstr_10__LINENO.slen = 6;

  /*^iniobj */

/*iniobj dobj_11*/
/*_.VALDATA___V22*/ meltfptr[21] = (melt_ptr_t) & meltcdat->dobj_11;
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
  meltcdat->dobj_11.obj_hash = 414566119;
  meltcdat->dobj_11.obj_len = 2;


/*iniobj dsym_12__MSG*/
  /*uniqueobj*/ if (! /*_.VALDATA___V26*/ meltfptr[25])
/*_.VALDATA___V26*/ meltfptr[25] = (melt_ptr_t) & meltcdat->dsym_12__MSG;
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
 /*_.VALSTR___V27*/ meltfptr[26] = (melt_ptr_t) & meltcdat->dstr_13__MSG;
  meltcdat->dstr_13__MSG.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_13__MSG.val, "MSG",
				sizeof (meltcdat->dstr_13__MSG.val) - 1);
  meltcdat->dstr_13__MSG.val[3] = (char) 0;
  meltcdat->dstr_13__MSG.slen = 3;

  /*^iniobj */

/*iniobj dobj_14*/
/*_.VALDATA___V25*/ meltfptr[24] = (melt_ptr_t) & meltcdat->dobj_14;
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
  meltcdat->dobj_14.obj_hash = 385368606;
  meltcdat->dobj_14.obj_len = 2;


/*iniobj dsym_15__COUNT*/
  /*uniqueobj*/ if (! /*_.VALDATA___V29*/ meltfptr[28])
/*_.VALDATA___V29*/ meltfptr[28] =
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
 /*_.VALSTR___V30*/ meltfptr[29] = (melt_ptr_t) & meltcdat->dstr_16__COUNT;
  meltcdat->dstr_16__COUNT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_16__COUNT.val, "COUNT",
				sizeof (meltcdat->dstr_16__COUNT.val) - 1);
  meltcdat->dstr_16__COUNT.val[5] = (char) 0;
  meltcdat->dstr_16__COUNT.slen = 5;

  /*^iniobj */

/*iniobj dobj_17*/
/*_.VALDATA___V28*/ meltfptr[27] = (melt_ptr_t) & meltcdat->dobj_17;
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
  meltcdat->dobj_17.obj_hash = 803689199;
  meltcdat->dobj_17.obj_len = 2;

  /*inimult dtup_18___HOOK_LOW_DEBUG_VALUE_AT */
 /*_.VALTUP___V31*/ meltfptr[30] =
    (melt_ptr_t) & meltcdat->dtup_18___HOOK_LOW_DEBUG_VALUE_AT;
  meltcdat->dtup_18___HOOK_LOW_DEBUG_VALUE_AT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_18___HOOK_LOW_DEBUG_VALUE_AT.nbval = 5;
  /*inimult dtup_19___HOOK_LOW_DEBUG_VALUE_AT */
 /*_.VALTUP___V32*/ meltfptr[31] =
    (melt_ptr_t) & meltcdat->dtup_19___HOOK_LOW_DEBUG_VALUE_AT;
  meltcdat->dtup_19___HOOK_LOW_DEBUG_VALUE_AT.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_19___HOOK_LOW_DEBUG_VALUE_AT.nbval = 0;

/*iniobj dsym_20__MELT_DEBUG_FUN*/
  /*uniqueobj*/ if (! /*_.VALDATA___V12*/ meltfptr[11])
/*_.VALDATA___V12*/ meltfptr[11] =
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
 /*_.VALSTR___V13*/ meltfptr[12] =
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
  /*uniqueobj*/ if (! /*_.VALDATA___V33*/ meltfptr[32])
/*_.VALDATA___V33*/ meltfptr[32] =
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
 /*_.VALSTR___V34*/ meltfptr[33] =
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
  /*uniqueobj*/ if (! /*_.VALDATA___V35*/ meltfptr[34])
/*_.VALDATA___V35*/ meltfptr[34] =
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
 /*_.VALSTR___V36*/ meltfptr[35] =
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
/*_.VALDATA___V37*/ meltfptr[36] =
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
  meltcdat->dobj_26__OVERRIDE_GATE_DELAYED_QUEUE.obj_hash = 517408791;
  meltcdat->dobj_26__OVERRIDE_GATE_DELAYED_QUEUE.obj_len = 5;


/*inistring dstr_27__override_gate_de*/
 /*_.VALSTR___V40*/ meltfptr[39] =
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
  /*uniqueobj*/ if (! /*_.VALDATA___V41*/ meltfptr[40])
/*_.VALDATA___V41*/ meltfptr[40] =
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
 /*_.VALSTR___V42*/ meltfptr[41] =
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
    /*_.VALDATA___V48*/ meltfptr[47];
  strncpy (meltcdat->dhook__30__HOOK_OVERRIDE_GATE.hookname,
	   "HOOK_OVERRIDE_GATE",
	   sizeof (meltcdat->dhook__30__HOOK_OVERRIDE_GATE.hookname) - 1);
/*predefinedhook HOOK_OVERRIDE_GATE for dhook__30__HOOK_OVERRIDE_GATE*/
  /*_.VALHOOK___V43*/ meltfptr[42] =
  /*inihooklocvar HOOK_OVERRIDE_GATE */
  (melt_ptr_t) & meltcdat->dhook__30__HOOK_OVERRIDE_GATE;

  /*done inihook dhook__30__HOOK_OVERRIDE_GATE */

  MELT_LOCATION ("warmelt-hooks.melt:49:/ iniobj");

/*iniobj dobj_31__*/
/*_.VALDATA___V48*/ meltfptr[47] = (melt_ptr_t) & meltcdat->dobj_31__;
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
  meltcdat->dobj_31__.obj_hash = 224148143;
  meltcdat->dobj_31__.obj_len = 6;


/*iniobj dsym_32__BEFOREGATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V51*/ meltfptr[50])
/*_.VALDATA___V51*/ meltfptr[50] =
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
 /*_.VALSTR___V52*/ meltfptr[51] =
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
/*_.VALDATA___V50*/ meltfptr[49] = (melt_ptr_t) & meltcdat->dobj_34;
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
  meltcdat->dobj_34.obj_hash = 822070917;
  meltcdat->dobj_34.obj_len = 2;

  /*inimult dtup_35___HOOK_OVERRIDE_GATE */
 /*_.VALTUP___V53*/ meltfptr[52] =
    (melt_ptr_t) & meltcdat->dtup_35___HOOK_OVERRIDE_GATE;
  meltcdat->dtup_35___HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_35___HOOK_OVERRIDE_GATE.nbval = 1;

/*iniobj dsym_36__AFTERGATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V55*/ meltfptr[54])
/*_.VALDATA___V55*/ meltfptr[54] =
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
 /*_.VALSTR___V56*/ meltfptr[55] =
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
/*_.VALDATA___V54*/ meltfptr[53] = (melt_ptr_t) & meltcdat->dobj_38;
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
  meltcdat->dobj_38.obj_hash = 893744541;
  meltcdat->dobj_38.obj_len = 2;

  /*inimult dtup_39___HOOK_OVERRIDE_GATE */
 /*_.VALTUP___V57*/ meltfptr[56] =
    (melt_ptr_t) & meltcdat->dtup_39___HOOK_OVERRIDE_GATE;
  meltcdat->dtup_39___HOOK_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
  meltcdat->dtup_39___HOOK_OVERRIDE_GATE.nbval = 1;

/*iniobj dsym_40__TRUE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V44*/ meltfptr[43])
/*_.VALDATA___V44*/ meltfptr[43] = (melt_ptr_t) & meltcdat->dsym_40__TRUE;
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
 /*_.VALSTR___V45*/ meltfptr[44] = (melt_ptr_t) & meltcdat->dstr_41__TRUE;
  meltcdat->dstr_41__TRUE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_41__TRUE.val, "TRUE",
				sizeof (meltcdat->dstr_41__TRUE.val) - 1);
  meltcdat->dstr_41__TRUE.val[4] = (char) 0;
  meltcdat->dstr_41__TRUE.slen = 4;


/*iniobj dsym_42__CLASS_REFERENCE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V46*/ meltfptr[45])
/*_.VALDATA___V46*/ meltfptr[45] =
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
 /*_.VALSTR___V47*/ meltfptr[46] =
    (melt_ptr_t) & meltcdat->dstr_43__CLASS_REFERENCE;
  meltcdat->dstr_43__CLASS_REFERENCE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_43__CLASS_REFERENCE.val,
				"CLASS_REFERENCE",
				sizeof (meltcdat->dstr_43__CLASS_REFERENCE.
					val) - 1);
  meltcdat->dstr_43__CLASS_REFERENCE.val[15] = (char) 0;
  meltcdat->dstr_43__CLASS_REFERENCE.slen = 15;


/*iniobj dsym_44__REGISTERED_FLAG_OVERRIDE_GATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V58*/ meltfptr[57])
/*_.VALDATA___V58*/ meltfptr[57] =
      (melt_ptr_t) & meltcdat->dsym_44__REGISTERED_FLAG_OVERRIDE_GATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_44__REGISTERED_FLAG_OVERRIDE_GATE",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_44__REGISTERED_FLAG_OVERRIDE_GATE",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_44__REGISTERED_FLAG_OVERRIDE_GATE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_44__REGISTERED_FLAG_OVERRIDE_GATE.obj_hash = 1055582817;
  meltcdat->dsym_44__REGISTERED_FLAG_OVERRIDE_GATE.obj_len = 3;


/*inistring dstr_45__REGISTERED_FLAG_*/
 /*_.VALSTR___V59*/ meltfptr[58] =
    (melt_ptr_t) & meltcdat->dstr_45__REGISTERED_FLAG_;
  meltcdat->dstr_45__REGISTERED_FLAG_.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_45__REGISTERED_FLAG_.val,
				"REGISTERED_FLAG_OVERRIDE_GATE",
				sizeof (meltcdat->dstr_45__REGISTERED_FLAG_.
					val) - 1);
  meltcdat->dstr_45__REGISTERED_FLAG_.val[29] = (char) 0;
  meltcdat->dstr_45__REGISTERED_FLAG_.slen = 29;

  MELT_LOCATION ("warmelt-hooks.melt:134:/ iniobj");

/*iniobj dobj_46__REGISTERED_FLAG_OVERRIDE_GATE*/
/*_.VALDATA___V60*/ meltfptr[59] =
    (melt_ptr_t) & meltcdat->dobj_46__REGISTERED_FLAG_OVERRIDE_GATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dobj_46__REGISTERED_FLAG_OVERRIDE_GATE",
       melt_magic_discr ((melt_ptr_t)
			 ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9])) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dobj_46__REGISTERED_FLAG_OVERRIDE_GATE",
       ((meltobject_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dobj_46__REGISTERED_FLAG_OVERRIDE_GATE.meltobj_class =
    (meltobject_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);
  meltcdat->dobj_46__REGISTERED_FLAG_OVERRIDE_GATE.obj_hash = 385908311;
  meltcdat->dobj_46__REGISTERED_FLAG_OVERRIDE_GATE.obj_len = 1;

  /*inirout drout_47__ENABLE_OVERRIDE_GATE */
 /*_.VALROUT___V61*/ meltfptr[60] =
    (melt_ptr_t) & meltcdat->drout_47__ENABLE_OVERRIDE_GATE;
  meltcdat->drout_47__ENABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_47__ENABLE_OVERRIDE_GATE.routdescr,
	   "ENABLE_OVERRIDE_GATE @warmelt-hooks.melt:137",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_47__ENABLE_OVERRIDE_GATE.nbval = 3;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_47__ENABLE_OVERRIDE_GATE,
			     meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE);

  /*iniclos dclo_48__ENABLE_OVERRIDE_GATE */
 /*_.VALCLO___V62*/ meltfptr[61] =
    (melt_ptr_t) & meltcdat->dclo_48__ENABLE_OVERRIDE_GATE;
  meltcdat->dclo_48__ENABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_48__ENABLE_OVERRIDE_GATE.nbval = 0;

  /*inirout drout_49__MAYBE_DISABLE_OVERRIDE_GATE */
 /*_.VALROUT___V63*/ meltfptr[62] =
    (melt_ptr_t) & meltcdat->drout_49__MAYBE_DISABLE_OVERRIDE_GATE;
  meltcdat->drout_49__MAYBE_DISABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_49__MAYBE_DISABLE_OVERRIDE_GATE.routdescr,
	   "MAYBE_DISABLE_OVERRIDE_GATE @warmelt-hooks.melt:150",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_49__MAYBE_DISABLE_OVERRIDE_GATE.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_49__MAYBE_DISABLE_OVERRIDE_GATE,
			     meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE);

  /*iniclos dclo_50__MAYBE_DISABLE_OVERRIDE_GATE */
 /*_.VALCLO___V64*/ meltfptr[63] =
    (melt_ptr_t) & meltcdat->dclo_50__MAYBE_DISABLE_OVERRIDE_GATE;
  meltcdat->dclo_50__MAYBE_DISABLE_OVERRIDE_GATE.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_50__MAYBE_DISABLE_OVERRIDE_GATE.nbval = 0;

  /*inirout drout_51__REGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALROUT___V65*/ meltfptr[64] =
    (melt_ptr_t) & meltcdat->drout_51__REGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->drout_51__REGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_51__REGISTER_OVERRIDE_GATE_FIRST.routdescr,
	   "REGISTER_OVERRIDE_GATE_FIRST @warmelt-hooks.melt:163",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_51__REGISTER_OVERRIDE_GATE_FIRST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_51__REGISTER_OVERRIDE_GATE_FIRST,
			     meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST);

  /*iniclos dclo_52__REGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALCLO___V66*/ meltfptr[65] =
    (melt_ptr_t) & meltcdat->dclo_52__REGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->dclo_52__REGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_52__REGISTER_OVERRIDE_GATE_FIRST.nbval = 0;

  /*inirout drout_53__REGISTER_OVERRIDE_GATE_LAST */
 /*_.VALROUT___V67*/ meltfptr[66] =
    (melt_ptr_t) & meltcdat->drout_53__REGISTER_OVERRIDE_GATE_LAST;
  meltcdat->drout_53__REGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_53__REGISTER_OVERRIDE_GATE_LAST.routdescr,
	   "REGISTER_OVERRIDE_GATE_LAST @warmelt-hooks.melt:171",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_53__REGISTER_OVERRIDE_GATE_LAST.nbval = 4;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->drout_53__REGISTER_OVERRIDE_GATE_LAST,
			     meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST);

  /*iniclos dclo_54__REGISTER_OVERRIDE_GATE_LAST */
 /*_.VALCLO___V68*/ meltfptr[67] =
    (melt_ptr_t) & meltcdat->dclo_54__REGISTER_OVERRIDE_GATE_LAST;
  meltcdat->dclo_54__REGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_54__REGISTER_OVERRIDE_GATE_LAST.nbval = 0;

  /*inirout drout_55__UNREGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALROUT___V69*/ meltfptr[68] =
    (melt_ptr_t) & meltcdat->drout_55__UNREGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->drout_55__UNREGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_55__UNREGISTER_OVERRIDE_GATE_FIRST.routdescr,
	   "UNREGISTER_OVERRIDE_GATE_FIRST @warmelt-hooks.melt:179",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_55__UNREGISTER_OVERRIDE_GATE_FIRST.nbval = 5;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_55__UNREGISTER_OVERRIDE_GATE_FIRST,
			     meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST);

  /*iniclos dclo_56__UNREGISTER_OVERRIDE_GATE_FIRST */
 /*_.VALCLO___V70*/ meltfptr[69] =
    (melt_ptr_t) & meltcdat->dclo_56__UNREGISTER_OVERRIDE_GATE_FIRST;
  meltcdat->dclo_56__UNREGISTER_OVERRIDE_GATE_FIRST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_56__UNREGISTER_OVERRIDE_GATE_FIRST.nbval = 0;

  /*inirout drout_57__UNREGISTER_OVERRIDE_GATE_LAST */
 /*_.VALROUT___V71*/ meltfptr[70] =
    (melt_ptr_t) & meltcdat->drout_57__UNREGISTER_OVERRIDE_GATE_LAST;
  meltcdat->drout_57__UNREGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_ROUTINE))));
  strncpy (meltcdat->drout_57__UNREGISTER_OVERRIDE_GATE_LAST.routdescr,
	   "UNREGISTER_OVERRIDE_GATE_LAST @warmelt-hooks.melt:202",
	   MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_57__UNREGISTER_OVERRIDE_GATE_LAST.nbval = 5;
  MELT_ROUTINE_SET_ROUTCODE (&meltcdat->
			     drout_57__UNREGISTER_OVERRIDE_GATE_LAST,
			     meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST);

  /*iniclos dclo_58__UNREGISTER_OVERRIDE_GATE_LAST */
 /*_.VALCLO___V72*/ meltfptr[71] =
    (melt_ptr_t) & meltcdat->dclo_58__UNREGISTER_OVERRIDE_GATE_LAST;
  meltcdat->dclo_58__UNREGISTER_OVERRIDE_GATE_LAST.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_CLOSURE))));
  meltcdat->dclo_58__UNREGISTER_OVERRIDE_GATE_LAST.nbval = 0;


/*iniobj dsym_59__HOOK_OVERRIDE_GATE*/
  /*uniqueobj*/ if (! /*_.VALDATA___V73*/ meltfptr[72])
/*_.VALDATA___V73*/ meltfptr[72] =
      (melt_ptr_t) & meltcdat->dsym_59__HOOK_OVERRIDE_GATE;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr isobj dsym_59__HOOK_OVERRIDE_GATE",
		    melt_magic_discr ((melt_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (CLASS_SYMBOL))))) ==
		    MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg ("iniobj check.discr objmagic dsym_59__HOOK_OVERRIDE_GATE",
		    ((meltobject_ptr_t)
		     (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
		    meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_59__HOOK_OVERRIDE_GATE.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_59__HOOK_OVERRIDE_GATE.obj_hash = 995989956;
  meltcdat->dsym_59__HOOK_OVERRIDE_GATE.obj_len = 3;


/*inistring dstr_60__HOOK_OVERRIDE_GA*/
 /*_.VALSTR___V74*/ meltfptr[73] =
    (melt_ptr_t) & meltcdat->dstr_60__HOOK_OVERRIDE_GA;
  meltcdat->dstr_60__HOOK_OVERRIDE_GA.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_60__HOOK_OVERRIDE_GA.val,
				"HOOK_OVERRIDE_GATE",
				sizeof (meltcdat->dstr_60__HOOK_OVERRIDE_GA.
					val) - 1);
  meltcdat->dstr_60__HOOK_OVERRIDE_GA.val[18] = (char) 0;
  meltcdat->dstr_60__HOOK_OVERRIDE_GA.slen = 18;


/*iniobj dsym_61__HOOK_LOW_DEBUG_VALUE_AT*/
  /*uniqueobj*/ if (! /*_.VALDATA___V75*/ meltfptr[74])
/*_.VALDATA___V75*/ meltfptr[74] =
      (melt_ptr_t) & meltcdat->dsym_61__HOOK_LOW_DEBUG_VALUE_AT;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_61__HOOK_LOW_DEBUG_VALUE_AT",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_61__HOOK_LOW_DEBUG_VALUE_AT",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_61__HOOK_LOW_DEBUG_VALUE_AT.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_61__HOOK_LOW_DEBUG_VALUE_AT.obj_hash = 1048470198;
  meltcdat->dsym_61__HOOK_LOW_DEBUG_VALUE_AT.obj_len = 3;


/*inistring dstr_62__HOOK_LOW_DEBUG_V*/
 /*_.VALSTR___V76*/ meltfptr[75] =
    (melt_ptr_t) & meltcdat->dstr_62__HOOK_LOW_DEBUG_V;
  meltcdat->dstr_62__HOOK_LOW_DEBUG_V.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_62__HOOK_LOW_DEBUG_V.val,
				"HOOK_LOW_DEBUG_VALUE_AT",
				sizeof (meltcdat->dstr_62__HOOK_LOW_DEBUG_V.
					val) - 1);
  meltcdat->dstr_62__HOOK_LOW_DEBUG_V.val[23] = (char) 0;
  meltcdat->dstr_62__HOOK_LOW_DEBUG_V.slen = 23;


/*iniobj dsym_63__REGISTER_OVERRIDE_GATE_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V77*/ meltfptr[76])
/*_.VALDATA___V77*/ meltfptr[76] =
      (melt_ptr_t) & meltcdat->dsym_63__REGISTER_OVERRIDE_GATE_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_63__REGISTER_OVERRIDE_GATE_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_63__REGISTER_OVERRIDE_GATE_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_63__REGISTER_OVERRIDE_GATE_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_63__REGISTER_OVERRIDE_GATE_FIRST.obj_hash = 929826072;
  meltcdat->dsym_63__REGISTER_OVERRIDE_GATE_FIRST.obj_len = 3;


/*inistring dstr_64__REGISTER_OVERRID*/
 /*_.VALSTR___V78*/ meltfptr[77] =
    (melt_ptr_t) & meltcdat->dstr_64__REGISTER_OVERRID;
  meltcdat->dstr_64__REGISTER_OVERRID.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_64__REGISTER_OVERRID.val,
				"REGISTER_OVERRIDE_GATE_FIRST",
				sizeof (meltcdat->dstr_64__REGISTER_OVERRID.
					val) - 1);
  meltcdat->dstr_64__REGISTER_OVERRID.val[28] = (char) 0;
  meltcdat->dstr_64__REGISTER_OVERRID.slen = 28;


/*iniobj dsym_65__REGISTER_OVERRIDE_GATE_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V79*/ meltfptr[78])
/*_.VALDATA___V79*/ meltfptr[78] =
      (melt_ptr_t) & meltcdat->dsym_65__REGISTER_OVERRIDE_GATE_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_65__REGISTER_OVERRIDE_GATE_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_65__REGISTER_OVERRIDE_GATE_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_65__REGISTER_OVERRIDE_GATE_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_65__REGISTER_OVERRIDE_GATE_LAST.obj_hash = 387249375;
  meltcdat->dsym_65__REGISTER_OVERRIDE_GATE_LAST.obj_len = 3;


/*inistring dstr_66__REGISTER_OVERRID*/
 /*_.VALSTR___V80*/ meltfptr[79] =
    (melt_ptr_t) & meltcdat->dstr_66__REGISTER_OVERRID;
  meltcdat->dstr_66__REGISTER_OVERRID.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_66__REGISTER_OVERRID.val,
				"REGISTER_OVERRIDE_GATE_LAST",
				sizeof (meltcdat->dstr_66__REGISTER_OVERRID.
					val) - 1);
  meltcdat->dstr_66__REGISTER_OVERRID.val[27] = (char) 0;
  meltcdat->dstr_66__REGISTER_OVERRID.slen = 27;


/*iniobj dsym_67__UNREGISTER_OVERRIDE_GATE_FIRST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V81*/ meltfptr[80])
/*_.VALDATA___V81*/ meltfptr[80] =
      (melt_ptr_t) & meltcdat->dsym_67__UNREGISTER_OVERRIDE_GATE_FIRST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_67__UNREGISTER_OVERRIDE_GATE_FIRST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_67__UNREGISTER_OVERRIDE_GATE_FIRST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_67__UNREGISTER_OVERRIDE_GATE_FIRST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_67__UNREGISTER_OVERRIDE_GATE_FIRST.obj_hash = 604111710;
  meltcdat->dsym_67__UNREGISTER_OVERRIDE_GATE_FIRST.obj_len = 3;


/*inistring dstr_68__UNREGISTER_OVERR*/
 /*_.VALSTR___V82*/ meltfptr[81] =
    (melt_ptr_t) & meltcdat->dstr_68__UNREGISTER_OVERR;
  meltcdat->dstr_68__UNREGISTER_OVERR.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_68__UNREGISTER_OVERR.val,
				"UNREGISTER_OVERRIDE_GATE_FIRST",
				sizeof (meltcdat->dstr_68__UNREGISTER_OVERR.
					val) - 1);
  meltcdat->dstr_68__UNREGISTER_OVERR.val[30] = (char) 0;
  meltcdat->dstr_68__UNREGISTER_OVERR.slen = 30;


/*iniobj dsym_69__UNREGISTER_OVERRIDE_GATE_LAST*/
  /*uniqueobj*/ if (! /*_.VALDATA___V83*/ meltfptr[82])
/*_.VALDATA___V83*/ meltfptr[82] =
      (melt_ptr_t) & meltcdat->dsym_69__UNREGISTER_OVERRIDE_GATE_LAST;
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr isobj dsym_69__UNREGISTER_OVERRIDE_GATE_LAST",
       melt_magic_discr ((melt_ptr_t)
			 (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))))) ==
       MELTOBMAG_OBJECT);
  if (MELT_LIKELY (!melt_prohibit_garbcoll))
    melt_assertmsg
      ("iniobj check.discr objmagic dsym_69__UNREGISTER_OVERRIDE_GATE_LAST",
       ((meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL)))))->
       meltobj_magic == MELTOBMAG_OBJECT);
  meltcdat->dsym_69__UNREGISTER_OVERRIDE_GATE_LAST.meltobj_class =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (CLASS_SYMBOL))));
  meltcdat->dsym_69__UNREGISTER_OVERRIDE_GATE_LAST.obj_hash = 458707543;
  meltcdat->dsym_69__UNREGISTER_OVERRIDE_GATE_LAST.obj_len = 3;


/*inistring dstr_70__UNREGISTER_OVERR*/
 /*_.VALSTR___V84*/ meltfptr[83] =
    (melt_ptr_t) & meltcdat->dstr_70__UNREGISTER_OVERR;
  meltcdat->dstr_70__UNREGISTER_OVERR.discr =
    (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_STRING))));
  /*small inistring */ strncpy (meltcdat->dstr_70__UNREGISTER_OVERR.val,
				"UNREGISTER_OVERRIDE_GATE_LAST",
				sizeof (meltcdat->dstr_70__UNREGISTER_OVERR.
					val) - 1);
  meltcdat->dstr_70__UNREGISTER_OVERR.val[29] = (char) 0;
  meltcdat->dstr_70__UNREGISTER_OVERR.slen = 29;
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
/* declstructinit initial routine melt_start_this_module minihash 1637*/

  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
/**initial routine prologue**/
  /* set initial frame marking */
  ((struct melt_callframe_st *) &meltfram__)->mcfr_nbvar =
    /*minihash */ -1637;
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
  /*getnamedsym:VAL */
  {
    melt_ptr_t sy_VAL = meltgc_named_symbol ("VAL", MELT_GET);
    if (sy_VAL && NULL == /*_.VALDATA___V17*/ meltfptr[16])
  /*_.VALDATA___V17*/ meltfptr[16] = (melt_ptr_t) sy_VAL;
  }

  /*^getnamedsymbol */
  /*getnamedsym:FILENAME */
  {
    melt_ptr_t sy_FILENAME = meltgc_named_symbol ("FILENAME", MELT_GET);
    if (sy_FILENAME && NULL == /*_.VALDATA___V20*/ meltfptr[19])
  /*_.VALDATA___V20*/ meltfptr[19] = (melt_ptr_t) sy_FILENAME;
  }

  /*^getnamedsymbol */
  /*getnamedsym:LINENO */
  {
    melt_ptr_t sy_LINENO = meltgc_named_symbol ("LINENO", MELT_GET);
    if (sy_LINENO && NULL == /*_.VALDATA___V23*/ meltfptr[22])
  /*_.VALDATA___V23*/ meltfptr[22] = (melt_ptr_t) sy_LINENO;
  }

  /*^getnamedsymbol */
  /*getnamedsym:MSG */
  {
    melt_ptr_t sy_MSG = meltgc_named_symbol ("MSG", MELT_GET);
    if (sy_MSG && NULL == /*_.VALDATA___V26*/ meltfptr[25])
  /*_.VALDATA___V26*/ meltfptr[25] = (melt_ptr_t) sy_MSG;
  }

  /*^getnamedsymbol */
  /*getnamedsym:COUNT */
  {
    melt_ptr_t sy_COUNT = meltgc_named_symbol ("COUNT", MELT_GET);
    if (sy_COUNT && NULL == /*_.VALDATA___V29*/ meltfptr[28])
  /*_.VALDATA___V29*/ meltfptr[28] = (melt_ptr_t) sy_COUNT;
  }

  /*^getnamedsymbol */
  /*getnamedsym:MELT_DEBUG_FUN */
  {
    melt_ptr_t sy_MELT_DEBUG_FUN =
      meltgc_named_symbol ("MELT_DEBUG_FUN", MELT_GET);
    if (sy_MELT_DEBUG_FUN && NULL == /*_.VALDATA___V12*/ meltfptr[11])
  /*_.VALDATA___V12*/ meltfptr[11] = (melt_ptr_t) sy_MELT_DEBUG_FUN;
  }

  /*^getnamedsymbol */
  /*getnamedsym:OVERRIDE_GATE_DELAYED_QUEUE */
  {
    melt_ptr_t sy_OVERRIDE_GATE_DELAYED_QUEUE =
      meltgc_named_symbol ("OVERRIDE_GATE_DELAYED_QUEUE", MELT_GET);
    if (sy_OVERRIDE_GATE_DELAYED_QUEUE
	&& NULL == /*_.VALDATA___V33*/ meltfptr[32])
  /*_.VALDATA___V33*/ meltfptr[32] =
	(melt_ptr_t) sy_OVERRIDE_GATE_DELAYED_QUEUE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:CLASS_DELAYED_QUEUE */
  {
    melt_ptr_t sy_CLASS_DELAYED_QUEUE =
      meltgc_named_symbol ("CLASS_DELAYED_QUEUE", MELT_GET);
    if (sy_CLASS_DELAYED_QUEUE && NULL == /*_.VALDATA___V35*/ meltfptr[34])
  /*_.VALDATA___V35*/ meltfptr[34] = (melt_ptr_t) sy_CLASS_DELAYED_QUEUE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:DISCR_LIST */
  {
    melt_ptr_t sy_DISCR_LIST = meltgc_named_symbol ("DISCR_LIST", MELT_GET);
    if (sy_DISCR_LIST && NULL == /*_.VALDATA___V41*/ meltfptr[40])
  /*_.VALDATA___V41*/ meltfptr[40] = (melt_ptr_t) sy_DISCR_LIST;
  }

  /*^getnamedsymbol */
  /*getnamedsym:BEFOREGATE */
  {
    melt_ptr_t sy_BEFOREGATE = meltgc_named_symbol ("BEFOREGATE", MELT_GET);
    if (sy_BEFOREGATE && NULL == /*_.VALDATA___V51*/ meltfptr[50])
  /*_.VALDATA___V51*/ meltfptr[50] = (melt_ptr_t) sy_BEFOREGATE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:AFTERGATE */
  {
    melt_ptr_t sy_AFTERGATE = meltgc_named_symbol ("AFTERGATE", MELT_GET);
    if (sy_AFTERGATE && NULL == /*_.VALDATA___V55*/ meltfptr[54])
  /*_.VALDATA___V55*/ meltfptr[54] = (melt_ptr_t) sy_AFTERGATE;
  }

  /*^getnamedkeyword */
  /*getnamedkeyw:TRUE */
  {
    melt_ptr_t kw_TRUE = meltgc_named_keyword ("TRUE", MELT_GET);
    if (kw_TRUE)
	       /*_.VALDATA___V44*/
      meltfptr[43] = (melt_ptr_t) kw_TRUE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:CLASS_REFERENCE */
  {
    melt_ptr_t sy_CLASS_REFERENCE =
      meltgc_named_symbol ("CLASS_REFERENCE", MELT_GET);
    if (sy_CLASS_REFERENCE && NULL == /*_.VALDATA___V46*/ meltfptr[45])
  /*_.VALDATA___V46*/ meltfptr[45] = (melt_ptr_t) sy_CLASS_REFERENCE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:REGISTERED_FLAG_OVERRIDE_GATE */
  {
    melt_ptr_t sy_REGISTERED_FLAG_OVERRIDE_GATE =
      meltgc_named_symbol ("REGISTERED_FLAG_OVERRIDE_GATE", MELT_GET);
    if (sy_REGISTERED_FLAG_OVERRIDE_GATE
	&& NULL == /*_.VALDATA___V58*/ meltfptr[57])
  /*_.VALDATA___V58*/ meltfptr[57] =
	(melt_ptr_t) sy_REGISTERED_FLAG_OVERRIDE_GATE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:HOOK_OVERRIDE_GATE */
  {
    melt_ptr_t sy_HOOK_OVERRIDE_GATE =
      meltgc_named_symbol ("HOOK_OVERRIDE_GATE", MELT_GET);
    if (sy_HOOK_OVERRIDE_GATE && NULL == /*_.VALDATA___V73*/ meltfptr[72])
  /*_.VALDATA___V73*/ meltfptr[72] = (melt_ptr_t) sy_HOOK_OVERRIDE_GATE;
  }

  /*^getnamedsymbol */
  /*getnamedsym:HOOK_LOW_DEBUG_VALUE_AT */
  {
    melt_ptr_t sy_HOOK_LOW_DEBUG_VALUE_AT =
      meltgc_named_symbol ("HOOK_LOW_DEBUG_VALUE_AT", MELT_GET);
    if (sy_HOOK_LOW_DEBUG_VALUE_AT
	&& NULL == /*_.VALDATA___V75*/ meltfptr[74])
  /*_.VALDATA___V75*/ meltfptr[74] =
	(melt_ptr_t) sy_HOOK_LOW_DEBUG_VALUE_AT;
  }

  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_OVERRIDE_GATE_FIRST */
  {
    melt_ptr_t sy_REGISTER_OVERRIDE_GATE_FIRST =
      meltgc_named_symbol ("REGISTER_OVERRIDE_GATE_FIRST", MELT_GET);
    if (sy_REGISTER_OVERRIDE_GATE_FIRST
	&& NULL == /*_.VALDATA___V77*/ meltfptr[76])
  /*_.VALDATA___V77*/ meltfptr[76] =
	(melt_ptr_t) sy_REGISTER_OVERRIDE_GATE_FIRST;
  }

  /*^getnamedsymbol */
  /*getnamedsym:REGISTER_OVERRIDE_GATE_LAST */
  {
    melt_ptr_t sy_REGISTER_OVERRIDE_GATE_LAST =
      meltgc_named_symbol ("REGISTER_OVERRIDE_GATE_LAST", MELT_GET);
    if (sy_REGISTER_OVERRIDE_GATE_LAST
	&& NULL == /*_.VALDATA___V79*/ meltfptr[78])
  /*_.VALDATA___V79*/ meltfptr[78] =
	(melt_ptr_t) sy_REGISTER_OVERRIDE_GATE_LAST;
  }

  /*^getnamedsymbol */
  /*getnamedsym:UNREGISTER_OVERRIDE_GATE_FIRST */
  {
    melt_ptr_t sy_UNREGISTER_OVERRIDE_GATE_FIRST =
      meltgc_named_symbol ("UNREGISTER_OVERRIDE_GATE_FIRST", MELT_GET);
    if (sy_UNREGISTER_OVERRIDE_GATE_FIRST
	&& NULL == /*_.VALDATA___V81*/ meltfptr[80])
  /*_.VALDATA___V81*/ meltfptr[80] =
	(melt_ptr_t) sy_UNREGISTER_OVERRIDE_GATE_FIRST;
  }

  /*^getnamedsymbol */
  /*getnamedsym:UNREGISTER_OVERRIDE_GATE_LAST */
  {
    melt_ptr_t sy_UNREGISTER_OVERRIDE_GATE_LAST =
      meltgc_named_symbol ("UNREGISTER_OVERRIDE_GATE_LAST", MELT_GET);
    if (sy_UNREGISTER_OVERRIDE_GATE_LAST
	&& NULL == /*_.VALDATA___V83*/ meltfptr[82])
  /*_.VALDATA___V83*/ meltfptr[82] =
	(melt_ptr_t) sy_UNREGISTER_OVERRIDE_GATE_LAST;
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
    argtab[1].meltbp_cstring = "MELT_DEBUG_FUN";
    /*^apply.arg */
    argtab[2].meltbp_cstring = "warmelt-hooks";
    /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =
      melt_apply ((meltclosure_ptr_t) ( /*_.VALIMPORT___V6*/ meltfptr[5]),
		  (melt_ptr_t) ( /*_.VALDATA___V12*/ meltfptr[11]),
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
		  (melt_ptr_t) ( /*_.VALDATA___V35*/ meltfptr[34]),
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
		  (melt_ptr_t) ( /*_.VALDATA___V41*/ meltfptr[40]),
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
		  (melt_ptr_t) ( /*_.VALDATA___V46*/ meltfptr[45]),
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
  meltmod__warmelt_hooks__initialmeltchunk_1 (&meltfram__, meltpredefinited);;
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
				    ( /*_.VALDATA___V12*/ meltfptr[11])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V12*/ meltfptr[11]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V12*/ meltfptr[11]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V13*/ meltfptr[12]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V12*/ meltfptr[11]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V12*/ meltfptr[11],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V14*/ meltfptr[13])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V14*/ meltfptr[13]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V14*/ meltfptr[13]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V15*/ meltfptr[14]);

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

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V16*/ meltfptr[15])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V16*/ meltfptr[15]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V16*/ meltfptr[15]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V17*/ meltfptr[16]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V16*/ meltfptr[15])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V16*/ meltfptr[15]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V16*/ meltfptr[15]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VALUE))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V16*/ meltfptr[15]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V16*/ meltfptr[15],
				"new static instance");

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

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V20*/ meltfptr[19]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V20*/ meltfptr[19],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V19*/ meltfptr[18])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V19*/ meltfptr[18]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V19*/ meltfptr[18]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V20*/ meltfptr[19]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
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
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_CSTRING))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V19*/ meltfptr[18]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V19*/ meltfptr[18],
				"new static instance");

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

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
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

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
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

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V22*/ meltfptr[21]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V22*/ meltfptr[21],
				"new static instance");

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
  /*_.VALSTR___V27*/ meltfptr[26]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V26*/ meltfptr[25]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V26*/ meltfptr[25],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V25*/ meltfptr[24])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V25*/ meltfptr[24]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V25*/ meltfptr[24]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V26*/ meltfptr[25]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V25*/ meltfptr[24])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V25*/ meltfptr[24]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V25*/ meltfptr[24]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_CSTRING))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V25*/ meltfptr[24]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V25*/ meltfptr[24],
				"new static instance");

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
  /*_.VALSTR___V30*/ meltfptr[29]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V29*/ meltfptr[28]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V29*/ meltfptr[28],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V28*/ meltfptr[27])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V28*/ meltfptr[27]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V28*/ meltfptr[27]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V29*/ meltfptr[28]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V28*/ meltfptr[27])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V28*/ meltfptr[27]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V28*/ meltfptr[27]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V28*/ meltfptr[27]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V28*/ meltfptr[27],
				"new static instance");

  /*putupl#1 */
  melt_assertmsg ("putupl #1 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V31*/ meltfptr[30])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #1 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V31*/
					  meltfptr[30]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V31*/ meltfptr[30]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V16*/ meltfptr[15]);

  /*putupl#2 */
  melt_assertmsg ("putupl #2 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V31*/ meltfptr[30])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #2 checkoff",
		  (1 >= 0
		   && 1 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V31*/
					  meltfptr[30]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V31*/ meltfptr[30]))->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V19*/ meltfptr[18]);

  /*putupl#3 */
  melt_assertmsg ("putupl #3 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V31*/ meltfptr[30])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #3 checkoff",
		  (2 >= 0
		   && 2 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V31*/
					  meltfptr[30]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V31*/ meltfptr[30]))->tabval[2] =
    (melt_ptr_t) ( /*_.VALDATA___V22*/ meltfptr[21]);

  /*putupl#4 */
  melt_assertmsg ("putupl #4 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V31*/ meltfptr[30])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #4 checkoff",
		  (3 >= 0
		   && 3 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V31*/
					  meltfptr[30]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V31*/ meltfptr[30]))->tabval[3] =
    (melt_ptr_t) ( /*_.VALDATA___V25*/ meltfptr[24]);

  /*putupl#5 */
  melt_assertmsg ("putupl #5 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V31*/ meltfptr[30])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #5 checkoff",
		  (4 >= 0
		   && 4 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V31*/
					  meltfptr[30]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V31*/ meltfptr[30]))->tabval[4] =
    (melt_ptr_t) ( /*_.VALDATA___V28*/ meltfptr[27]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V31*/ meltfptr[30]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V14*/ meltfptr[13])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V14*/ meltfptr[13]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V14*/ meltfptr[13]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V31*/ meltfptr[30]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V32*/ meltfptr[31]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V14*/ meltfptr[13])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V14*/ meltfptr[13]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V14*/ meltfptr[13]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V32*/ meltfptr[31]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V14*/ meltfptr[13])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V14*/ meltfptr[13]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V14*/ meltfptr[13]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V14*/ meltfptr[13]);

  MELT_LOCATION ("warmelt-hooks.melt:32:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V14*/ meltfptr[13],
				"new static instance");

  /*put data in hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookdata checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V11*/ meltfptr[10])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V11*/ meltfptr[10])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V14*/ meltfptr[13]);

  /*put inside hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookconst checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V11*/
				  meltfptr[10]));
  ((melthook_ptr_t) /*_.VALHOOK___V11*/ meltfptr[10])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*put inside hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookconst checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V11*/
				  meltfptr[10]));
  ((melthook_ptr_t) /*_.VALHOOK___V11*/ meltfptr[10])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V12*/ meltfptr[11]);

  /*put inside hook HOOK_LOW_DEBUG_VALUE_AT */
  melt_assertmsg ("puthookconst checkhook HOOK_LOW_DEBUG_VALUE_AT",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V11*/
				  meltfptr[10]));
  ((melthook_ptr_t) /*_.VALHOOK___V11*/ meltfptr[10])->tabval[2] =
    (melt_ptr_t) ( /*_.VALSTR___V13*/ meltfptr[12]);

  /*touch:HOOK_LOW_DEBUG_VALUE_AT */
  meltgc_touch ( /*_.VALHOOK___V11*/ meltfptr[10]);

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
  /*_.VALSTR___V36*/ meltfptr[35]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V35*/ meltfptr[34]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V35*/ meltfptr[34],
				"new static instance");

 /*_.MAKE_LIST__V38*/ meltfptr[37] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
 /*_.MAKE_LIST__V39*/ meltfptr[38] =
    (meltgc_new_list
     ((meltobject_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8])));
  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V37*/ meltfptr[36])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V37*/ meltfptr[36]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V37*/ meltfptr[36]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V40*/ meltfptr[39]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V37*/ meltfptr[36])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V37*/ meltfptr[36]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V37*/ meltfptr[36]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V38*/ meltfptr[37]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V37*/ meltfptr[36])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V37*/ meltfptr[36]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V37*/ meltfptr[36]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.MAKE_LIST__V39*/ meltfptr[38]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V37*/ meltfptr[36]);

  MELT_LOCATION ("warmelt-hooks.melt:43:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V37*/ meltfptr[36],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V41*/ meltfptr[40])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V41*/ meltfptr[40]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V41*/ meltfptr[40]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V42*/ meltfptr[41]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V41*/ meltfptr[40]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V41*/ meltfptr[40],
				"new static instance");

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
  /*_.VALSTR___V45*/ meltfptr[44]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V44*/ meltfptr[43]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V44*/ meltfptr[43],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V46*/ meltfptr[45])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V46*/ meltfptr[45]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V46*/ meltfptr[45]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V47*/ meltfptr[46]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V46*/ meltfptr[45]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V46*/ meltfptr[45],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
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

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V50*/ meltfptr[49])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V50*/ meltfptr[49]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V50*/ meltfptr[49]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V51*/ meltfptr[50]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V50*/ meltfptr[49])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V50*/ meltfptr[49]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V50*/ meltfptr[49]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V50*/ meltfptr[49]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V50*/ meltfptr[49],
				"new static instance");

  /*putupl#6 */
  melt_assertmsg ("putupl #6 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V53*/ meltfptr[52])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #6 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V53*/
					  meltfptr[52]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V53*/ meltfptr[52]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V50*/ meltfptr[49]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V53*/ meltfptr[52]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V48*/ meltfptr[47])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (2 >= 0
		   && 2 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V48*/ meltfptr[47]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V48*/ meltfptr[47]))->obj_vartab[2] =
    (melt_ptr_t) (
  /*_.VALTUP___V53*/ meltfptr[52]);

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

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V55*/ meltfptr[54]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V55*/ meltfptr[54],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V54*/ meltfptr[53])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (0 >= 0
		   && 0 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V54*/ meltfptr[53]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V54*/ meltfptr[53]))->obj_vartab[0] =
    (melt_ptr_t) (
  /*_.VALDATA___V55*/ meltfptr[54]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V54*/ meltfptr[53])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V54*/ meltfptr[53]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V54*/ meltfptr[53]))->obj_vartab[1] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_LONG))));

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V54*/ meltfptr[53]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V54*/ meltfptr[53],
				"new static instance");

  /*putupl#7 */
  melt_assertmsg ("putupl #7 checktup",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALTUP___V57*/ meltfptr[56])) ==
		  MELTOBMAG_MULTIPLE);
  melt_assertmsg ("putupl #7 checkoff",
		  (0 >= 0
		   && 0 <
		   melt_multiple_length ((melt_ptr_t)
					 ( /*_.VALTUP___V57*/
					  meltfptr[56]))));
  ((meltmultiple_ptr_t) ( /*_.VALTUP___V57*/ meltfptr[56]))->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V54*/ meltfptr[53]);

  /*touch:INITUP_ */
  meltgc_touch ( /*_.VALTUP___V57*/ meltfptr[56]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V48*/ meltfptr[47])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (3 >= 0
		   && 3 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V48*/ meltfptr[47]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V48*/ meltfptr[47]))->obj_vartab[3] =
    (melt_ptr_t) (
  /*_.VALTUP___V57*/ meltfptr[56]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^putslot */
  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V48*/ meltfptr[47])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (4 >= 0
		   && 4 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V48*/ meltfptr[47]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V48*/ meltfptr[47]))->obj_vartab[4] =
    (melt_ptr_t) (
		    ((melt_ptr_t) (MELT_PREDEF (CTYPE_VOID))));

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V48*/ meltfptr[47]);

  MELT_LOCATION ("warmelt-hooks.melt:49:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V48*/ meltfptr[47],
				"new static instance");

  /*put data in hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookdata checkhook HOOK_OVERRIDE_GATE",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALHOOK___V43*/ meltfptr[42])) ==
		  MELTOBMAG_HOOK);
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->hookdata =
    (melt_ptr_t) ( /*_.VALDATA___V48*/ meltfptr[47]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (0) >= 0
		  && (0) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V43*/
				  meltfptr[42]));
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (1) >= 0
		  && (1) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V43*/
				  meltfptr[42]));
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (2) >= 0
		  && (2) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V43*/
				  meltfptr[42]));
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->tabval[2] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (3) >= 0
		  && (3) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V43*/
				  meltfptr[42]));
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->tabval[3] =
    (melt_ptr_t) ( /*_.VALDATA___V37*/ meltfptr[36]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (4) >= 0
		  && (4) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V43*/
				  meltfptr[42]));
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->tabval[4] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (5) >= 0
		  && (5) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V43*/
				  meltfptr[42]));
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->tabval[5] =
    (melt_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (6) >= 0
		  && (6) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V43*/
				  meltfptr[42]));
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->tabval[6] =
    (melt_ptr_t) ( /*_.VALSTR___V45*/ meltfptr[44]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (7) >= 0
		  && (7) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V43*/
				  meltfptr[42]));
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->tabval[7] =
    (melt_ptr_t) ( /*_.VALDATA___V46*/ meltfptr[45]);

  /*put inside hook HOOK_OVERRIDE_GATE */
  melt_assertmsg ("puthookconst checkhook HOOK_OVERRIDE_GATE",
		  (8) >= 0
		  && (8) <
		  melt_hook_size ((melt_ptr_t) /*_.VALHOOK___V43*/
				  meltfptr[42]));
  ((melthook_ptr_t) /*_.VALHOOK___V43*/ meltfptr[42])->tabval[8] =
    (melt_ptr_t) ( /*_.VALSTR___V47*/ meltfptr[46]);

  /*touch:HOOK_OVERRIDE_GATE */
  meltgc_touch ( /*_.VALHOOK___V43*/ meltfptr[42]);

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

  MELT_LOCATION ("warmelt-hooks.melt:134:/ initchunk");
  /*^touch */
  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V60*/ meltfptr[59]);

  MELT_LOCATION ("warmelt-hooks.melt:134:/ initchunk");
  /*^touchobj */

  melt_dbgtrace_written_object ( /*_.VALDATA___V60*/ meltfptr[59],
				"new static instance");

  MELT_LOCATION ("warmelt-hooks.melt:137:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V61*/ meltfptr[60])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V60*/ meltfptr[59]);
  ((meltroutine_ptr_t) /*_.VALROUT___V61*/ meltfptr[60])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V60*/ meltfptr[59]);

  MELT_LOCATION ("warmelt-hooks.melt:137:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V61*/ meltfptr[60])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg ("putroutconst constnull.drout_47__ENABLE_OVERRIDE_GATE#1",
		   NULL != ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]));
  ((meltroutine_ptr_t) /*_.VALROUT___V61*/ meltfptr[60])->tabval[1] =
    (melt_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);

  MELT_LOCATION ("warmelt-hooks.melt:137:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V61*/ meltfptr[60])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg ("putroutconst constnull.drout_47__ENABLE_OVERRIDE_GATE#2",
		   NULL != ( /*_.VALDATA___V44*/ meltfptr[43]));
  ((meltroutine_ptr_t) /*_.VALROUT___V61*/ meltfptr[60])->tabval[2] =
    (melt_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]);

  MELT_LOCATION ("warmelt-hooks.melt:137:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V61*/ meltfptr[60]);

  /*putclosurout#1 */
  melt_assertmsg ("putclosrout#1 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V62*/ meltfptr[61])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#1 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V61*/ meltfptr[60])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V62*/ meltfptr[61])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V61*/ meltfptr[60]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V62*/ meltfptr[61]);

  MELT_LOCATION ("warmelt-hooks.melt:150:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V60*/ meltfptr[59]);
  ((meltroutine_ptr_t) /*_.VALROUT___V63*/ meltfptr[62])->tabval[0] =
    (melt_ptr_t) ( /*_.VALDATA___V60*/ meltfptr[59]);

  MELT_LOCATION ("warmelt-hooks.melt:150:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V37*/ meltfptr[36]);
  ((meltroutine_ptr_t) /*_.VALROUT___V63*/ meltfptr[62])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V37*/ meltfptr[36]);

  MELT_LOCATION ("warmelt-hooks.melt:150:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_49__MAYBE_DISABLE_OVERRIDE_GATE#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V63*/ meltfptr[62])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);


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
  MELT_LOCATION ("warmelt-hooks.melt:150:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_49__MAYBE_DISABLE_OVERRIDE_GATE#3",
       NULL != ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]));
  ((meltroutine_ptr_t) /*_.VALROUT___V63*/ meltfptr[62])->tabval[3] =
    (melt_ptr_t) ( /*_.CLASS_REFERENCE__V10*/ meltfptr[9]);

  MELT_LOCATION ("warmelt-hooks.melt:150:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V63*/ meltfptr[62]);

  /*putclosurout#2 */
  melt_assertmsg ("putclosrout#2 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V64*/ meltfptr[63])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#2 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V63*/ meltfptr[62])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V64*/ meltfptr[63])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V63*/ meltfptr[62]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V64*/ meltfptr[63]);

  MELT_LOCATION ("warmelt-hooks.melt:163:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_51__REGISTER_OVERRIDE_GATE_FIRST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V65*/ meltfptr[64])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:163:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V37*/ meltfptr[36]);
  ((meltroutine_ptr_t) /*_.VALROUT___V65*/ meltfptr[64])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V37*/ meltfptr[36]);

  MELT_LOCATION ("warmelt-hooks.melt:163:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_51__REGISTER_OVERRIDE_GATE_FIRST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V65*/ meltfptr[64])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:163:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V62*/ meltfptr[61]);
  ((meltroutine_ptr_t) /*_.VALROUT___V65*/ meltfptr[64])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V62*/ meltfptr[61]);

  MELT_LOCATION ("warmelt-hooks.melt:163:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V65*/ meltfptr[64]);

  /*putclosurout#3 */
  melt_assertmsg ("putclosrout#3 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V66*/ meltfptr[65])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#3 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V65*/ meltfptr[64])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V66*/ meltfptr[65])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V65*/ meltfptr[64]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V66*/ meltfptr[65]);

  MELT_LOCATION ("warmelt-hooks.melt:171:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_53__REGISTER_OVERRIDE_GATE_LAST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:171:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V37*/ meltfptr[36]);
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V37*/ meltfptr[36]);

  MELT_LOCATION ("warmelt-hooks.melt:171:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_53__REGISTER_OVERRIDE_GATE_LAST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:171:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V62*/ meltfptr[61]);
  ((meltroutine_ptr_t) /*_.VALROUT___V67*/ meltfptr[66])->tabval[3] =
    (melt_ptr_t) ( /*_.VALCLO___V62*/ meltfptr[61]);

  MELT_LOCATION ("warmelt-hooks.melt:171:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V67*/ meltfptr[66]);

  /*putclosurout#4 */
  melt_assertmsg ("putclosrout#4 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V68*/ meltfptr[67])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#4 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V67*/ meltfptr[66])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V68*/ meltfptr[67])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V67*/ meltfptr[66]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V68*/ meltfptr[67]);

  MELT_LOCATION ("warmelt-hooks.melt:179:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_55__UNREGISTER_OVERRIDE_GATE_FIRST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:179:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V37*/ meltfptr[36]);
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V37*/ meltfptr[36]);

  MELT_LOCATION ("warmelt-hooks.melt:179:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_55__UNREGISTER_OVERRIDE_GATE_FIRST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:179:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_55__UNREGISTER_OVERRIDE_GATE_FIRST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:179:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V64*/ meltfptr[63]);
  ((meltroutine_ptr_t) /*_.VALROUT___V69*/ meltfptr[68])->tabval[4] =
    (melt_ptr_t) ( /*_.VALCLO___V64*/ meltfptr[63]);

  MELT_LOCATION ("warmelt-hooks.melt:179:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V69*/ meltfptr[68]);

  /*putclosurout#5 */
  melt_assertmsg ("putclosrout#5 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V70*/ meltfptr[69])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#5 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V69*/ meltfptr[68])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V70*/ meltfptr[69])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V69*/ meltfptr[68]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V70*/ meltfptr[69]);

  MELT_LOCATION ("warmelt-hooks.melt:202:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_57__UNREGISTER_OVERRIDE_GATE_LAST#0",
       NULL != ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]));
  ((meltroutine_ptr_t) /*_.VALROUT___V71*/ meltfptr[70])->tabval[0] =
    (melt_ptr_t) ( /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6]);

  MELT_LOCATION ("warmelt-hooks.melt:202:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALDATA___V37*/ meltfptr[36]);
  ((meltroutine_ptr_t) /*_.VALROUT___V71*/ meltfptr[70])->tabval[1] =
    (melt_ptr_t) ( /*_.VALDATA___V37*/ meltfptr[36]);

  MELT_LOCATION ("warmelt-hooks.melt:202:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_57__UNREGISTER_OVERRIDE_GATE_LAST#2",
       NULL != ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]));
  ((meltroutine_ptr_t) /*_.VALROUT___V71*/ meltfptr[70])->tabval[2] =
    (melt_ptr_t) ( /*_.CLASS_DELAYED_QUEUE__V8*/ meltfptr[7]);

  MELT_LOCATION ("warmelt-hooks.melt:202:/ initchunk");
  /*^putroutconst */
  /*putroutconst */
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_assertmsg ("putroutconst checkrout",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		    MELTOBMAG_ROUTINE);
  if (MELT_HAS_INITIAL_ENVIRONMENT)
    melt_checkmsg
      ("putroutconst constnull.drout_57__UNREGISTER_OVERRIDE_GATE_LAST#3",
       NULL != ( /*_.DISCR_LIST__V9*/ meltfptr[8]));
  ((meltroutine_ptr_t) /*_.VALROUT___V71*/ meltfptr[70])->tabval[3] =
    (melt_ptr_t) ( /*_.DISCR_LIST__V9*/ meltfptr[8]);

  MELT_LOCATION ("warmelt-hooks.melt:202:/ initchunk");
  /*^putroutconstnotnull */
  /*putroutconstnotnull */
  melt_assertmsg ("putroutconstnotnull checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		  MELTOBMAG_ROUTINE);
  melt_assertmsg ("putroutconstnotnull notnullconst",
		  NULL != /*_.VALCLO___V64*/ meltfptr[63]);
  ((meltroutine_ptr_t) /*_.VALROUT___V71*/ meltfptr[70])->tabval[4] =
    (melt_ptr_t) ( /*_.VALCLO___V64*/ meltfptr[63]);

  MELT_LOCATION ("warmelt-hooks.melt:202:/ initchunk");
  /*^touch */
  /*touch:IROUTVAL_ */
  meltgc_touch ( /*_.VALROUT___V71*/ meltfptr[70]);

  /*putclosurout#6 */
  melt_assertmsg ("putclosrout#6 checkclo",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALCLO___V72*/ meltfptr[71])) ==
		  MELTOBMAG_CLOSURE);
  melt_assertmsg ("putclosrout#6 checkrout",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALROUT___V71*/ meltfptr[70])) ==
		  MELTOBMAG_ROUTINE);
  ((meltclosure_ptr_t) /*_.VALCLO___V72*/ meltfptr[71])->rout =
    (meltroutine_ptr_t) ( /*_.VALROUT___V71*/ meltfptr[70]);

  /*touch:DATACLOSURE_ */
  meltgc_touch ( /*_.VALCLO___V72*/ meltfptr[71]);

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
  /*_.VALSTR___V74*/ meltfptr[73]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V73*/ meltfptr[72]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V73*/ meltfptr[72],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V75*/ meltfptr[74])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V75*/ meltfptr[74]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V75*/ meltfptr[74]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V76*/ meltfptr[75]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V75*/ meltfptr[74]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V75*/ meltfptr[74],
				"new static instance");

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
  /*_.VALSTR___V80*/ meltfptr[79]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V79*/ meltfptr[78]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V79*/ meltfptr[78],
				"new static instance");

  /*putslot */
  melt_assertmsg ("putslot checkobj",
		  melt_magic_discr ((melt_ptr_t)
				    ( /*_.VALDATA___V81*/ meltfptr[80])) ==
		  MELTOBMAG_OBJECT);

  melt_assertmsg ("putslot checkoff",
		  (1 >= 0
		   && 1 <
		   melt_object_length ((melt_ptr_t)
				       ( /*_.VALDATA___V81*/ meltfptr[80]))));
  ((meltobject_ptr_t) ( /*_.VALDATA___V81*/ meltfptr[80]))->obj_vartab[1] =
    (melt_ptr_t) (
  /*_.VALSTR___V82*/ meltfptr[81]);

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V81*/ meltfptr[80]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V81*/ meltfptr[80],
				"new static instance");

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

  /*touch:VALUEDATA_ */
  meltgc_touch ( /*_.VALDATA___V83*/ meltfptr[82]);


  melt_dbgtrace_written_object ( /*_.VALDATA___V83*/ meltfptr[82],
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
  /*_.OR___V85*/ meltfptr[84] =
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
     /*_.SYSDATA_CONT_FRESH_ENV__V86*/ meltfptr[85] = slot;
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
	    /*_.FUN___V87*/ meltfptr[86] =
	      melt_apply ((meltclosure_ptr_t)
			  ( /*_.SYSDATA_CONT_FRESH_ENV__V86*/ meltfptr[85]),
			  (melt_ptr_t) ( /*_.PREVENV___V4*/ meltfptr[3]),
			  (MELTBPARSTR_CSTRING ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*_.OR___V85*/ meltfptr[84] = /*_.FUN___V87*/ meltfptr[86];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-hooks.melt:4:/ clear");
	      /*clear *//*_.SYSDATA_CONT_FRESH_ENV__V86*/ meltfptr[85] = 0;
	  /*^clear */
	      /*clear *//*_.FUN___V87*/ meltfptr[86] = 0;
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
	    /*clear *//*_.OR___V85*/ meltfptr[84] = 0;
  }

  MELT_LOCATION ("warmelt-hooks.melt:227:/ initchunk");
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
   /*_.VALUEXPORTER___V88*/ meltfptr[85] = slot;
    };
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.VALHOOK___V43*/ meltfptr[42];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : HOOK_OVERRIDE_GATE */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V88*/ meltfptr[85]),
		  (melt_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]),
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
	(melt_ptr_t *) & /*_.VALHOOK___V11*/ meltfptr[10];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) &
	/*quasi.cur.mod.env.cont norm.exp.val : HOOK_LOW_DEBUG_VALUE_AT */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V88*/ meltfptr[85]),
		  (melt_ptr_t) ( /*_.VALDATA___V75*/ meltfptr[74]),
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
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_OVERRIDE_GATE_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V88*/ meltfptr[85]),
		  (melt_ptr_t) ( /*_.VALDATA___V77*/ meltfptr[76]),
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
	/*quasi.cur.mod.env.cont norm.exp.val : REGISTER_OVERRIDE_GATE_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V88*/ meltfptr[85]),
		  (melt_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]),
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
	/*quasi.cur.mod.env.cont norm.exp.val : UNREGISTER_OVERRIDE_GATE_FIRST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V88*/ meltfptr[85]),
		  (melt_ptr_t) ( /*_.VALDATA___V81*/ meltfptr[80]),
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
	/*quasi.cur.mod.env.cont norm.exp.val : UNREGISTER_OVERRIDE_GATE_LAST */
/*_.CONTENV___V2*/ meltfptr[1];
      melt_apply ((meltclosure_ptr_t)
		  ( /*_.VALUEXPORTER___V88*/ meltfptr[85]),
		  (melt_ptr_t) ( /*_.VALDATA___V83*/ meltfptr[82]),
		  (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		  (union meltparam_un *) 0);
    }
    ;
    /*epilog */

    /*^clear */
	    /*clear *//*_.VALUEXPORTER___V88*/ meltfptr[85] = 0;
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
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V17*/ meltfptr[16]));

  /*internsym:FILENAME */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V20*/ meltfptr[19]));

  /*internsym:LINENO */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V23*/ meltfptr[22]));

  /*internsym:MSG */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V26*/ meltfptr[25]));

  /*internsym:COUNT */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V29*/ meltfptr[28]));

  /*internsym:MELT_DEBUG_FUN */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V12*/ meltfptr[11]));

  /*internsym:OVERRIDE_GATE_DELAYED_QUEUE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V33*/ meltfptr[32]));

  /*internsym:CLASS_DELAYED_QUEUE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V35*/ meltfptr[34]));

  /*internsym:DISCR_LIST */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V41*/ meltfptr[40]));

  /*internsym:BEFOREGATE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V51*/ meltfptr[50]));

  /*internsym:AFTERGATE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V55*/ meltfptr[54]));

  /*internkeyw:TRUE */
  (void)
    meltgc_intern_keyword ((melt_ptr_t) ( /*_.VALDATA___V44*/ meltfptr[43]));

  /*internsym:CLASS_REFERENCE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V46*/ meltfptr[45]));

  /*internsym:REGISTERED_FLAG_OVERRIDE_GATE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V58*/ meltfptr[57]));

  /*internsym:HOOK_OVERRIDE_GATE */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V73*/ meltfptr[72]));

  /*internsym:HOOK_LOW_DEBUG_VALUE_AT */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V75*/ meltfptr[74]));

  /*internsym:REGISTER_OVERRIDE_GATE_FIRST */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V77*/ meltfptr[76]));

  /*internsym:REGISTER_OVERRIDE_GATE_LAST */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V79*/ meltfptr[78]));

  /*internsym:UNREGISTER_OVERRIDE_GATE_FIRST */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V81*/ meltfptr[80]));

  /*internsym:UNREGISTER_OVERRIDE_GATE_LAST */
  (void)
    meltgc_intern_symbol ((melt_ptr_t) ( /*_.VALDATA___V83*/ meltfptr[82]));

 /**COMMENT: set retinit from boxloc **/ ;

 /*_.RETINIT___V1*/ meltfptr[0] =
    /* finalsetretinit */
    melt_reference_value ((melt_ptr_t) ( /*_.CONTENV___V2*/ meltfptr[1]));
 /**COMMENT: end the initproc **/ ;


#undef meltfram__
}				/*end of meltmod__warmelt_hooks__initialmeltchunk_1 */

void
meltmod__warmelt_hooks__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking)
{
  int meltix = 0;
  melt_initial_frame_st *meltframptr_ = (melt_initial_frame_st *) fp;
  melt_assertmsg ("check module frame",
		  meltframptr_->mcfr_nbvar == /*minihash */ -1637);
  if (!marking && melt_is_forwarding)
    {
      dbgprintf
	("forward_or_mark_module_start_frame_warmelt_hooks forwarding %d pointers in frame %p",
	 88, (void *) meltframptr_);
      for (meltix = 0; meltix < 88; meltix++)
	MELT_FORWARDED (meltframptr_->mcfr_varptr[meltix]);
      return;
    }				/*end forwarding */
  dbgprintf
    ("forward_or_mark_module_start_frame_warmelt_hooks marking in frame %p",
     (void *) meltframptr_);
  for (meltix = 0; meltix < 88; meltix++)
    if (meltframptr_->mcfr_varptr[meltix])
      gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);

}				/* end meltmod__warmelt_hooks__forward_or_mark_module_start_frame */



/* exported 0 field offsets */

/* exported 0 class lengths */



/**** end of warmelt-hooks ****/
