/* GCC MELT GENERATED C++ FILE warmelt-moremacro.cc - DO NOT EDIT - see http://gcc-melt.org/ */

/***+ warmelt-moremacro without options +***/
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-moremacro declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-moremacro ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2013 Free Software Foundation, Inc.
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


/** ordinary MELT module meltbuild-sources/warmelt-moremacro**/
#define MELT_HAS_INITIAL_ENVIRONMENT 1 /*usualmodule*/



class Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110; // forward declaration fromline 6541
typedef Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110 Melt_InitialFrame;
/**** no MELT module variables ****/

/*** 4 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_MACRO_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*declare MELT called hook #2 HOOK_MELT_MAKE_LOCATION **/
MELT_EXTERN melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION (melt_ptr_t melthookdatap, const char* meltinp0_FILENAME, long meltinp1_LINENO);

/*declare MELT called hook #3 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*** end of 4 MELT called hook declarations ***/

/*** no extra MELT c-headers ***/




#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::procdecl::**/
#endif /*MELTGCC_NOLINENUMBERING*/
MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/


/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110 /*opaqueinitialclassy*/ meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiMOREMACRO__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMOREMACRO__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiMOREMACRO__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMOREMACRO__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMOREMACRO__initialmeltchunk_0 (meltinitial_frame_t*, char*);


/**** warmelt-moremacro implementations ****/
/**** MELT GENERATED IMPLEMENTATIONS for warmelt-moremacro ** DO NOT EDIT ; see gcc-melt.org ****/
/*** no extra MELT implementations ***/

#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::proc::**/
#endif /*MELTGCC_NOLINENUMBERING*/



melt_ptr_t MELT_MODULE_VISIBILITY 
meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
      const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
  long current_blocklevel_signals_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST_melt =  melt_blocklevel_signals;
 
#if MELT_HAVE_DEBUG
 static long melt_call_counter__;
 long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1505

/* start of frame for routine meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST fromline 1719 */

/** start of frame for meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST of CLASS_PROCROUTINEOBJ from 1522**/

 /*curframdeclclassy*/ class MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST// fromline 1535
  : public Melt_CallFrameWithValues<75> {
 public: /* fromline 1539*/
 long mcfr_varnum[28];
 /*classy others*/
 long _meltsparel;
 void melt_mark_stuff (void) {
  } /*end melt_mark_stuff*/
 virtual void melt_mark_ggc_data (void) {
  melt_mark_values ();
  melt_mark_stuff ();
 }; /*end melt_mark_ggc_data*/
 MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST(meltclosure_ptr_t clos) //the closure constructor fromline 1597
   : Melt_CallFrameWithValues<75> (
#if ENABLE_CHECKING /*fromline 1601*/
    __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1605*/
   sizeof(MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST), clos) {};
 MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST() //the constructor fromline 1609
   : Melt_CallFrameWithValues<75> (
#if ENABLE_CHECKING /*fromline 1613*/
    __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1617*/
   sizeof(MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST)) {};
#if ENABLE_CHECKING /*fromline 1621*/
  MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST(const char*fil, int lin) //the constructor fromline 1623
   : Melt_CallFrameWithValues<75> (fil,lin, sizeof(MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST)) {};
  MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1628
   : Melt_CallFrameWithValues<75> (fil,lin, sizeof(MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST), clos) {};
#endif /* ENABLE_CHECKING fromline 1633*/
  
}; // end  class MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST


/** end of frame for meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST fromline 1664**/

/* end of frame for routine meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST fromline 1723 */

/* classy proc frame meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST */ MeltFrame_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST 
    meltfram__ /*declfrastruct fromline 1747*/
/*classyprocarg meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST fromline 1752*/
#if ENABLE_CHECKING
  (__FILE__, __LINE__, meltclosp_) /* fromline 1756*/
#else /*ENABLE_CHECKING */
  (meltclosp_) /* fromline 1760*/
#endif /*ENABLE_CHECKING */

;
 #define meltframe meltfram__

melt_trace_start("EACH_COMPONENT_IN_LIST", meltcallcount);
 /*use arguments*/ (void) meltclosp_; (void)meltfirstargp_; (void)meltxargdescr_;
   (void)meltxargtab_; (void)meltxresdescr_; (void)meltxrestab_;
/*getargs*/

 /*getarg#0*/
 MELT_LOCATION("warmelt-moremacro.melt:39:/ getarg");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::getarg::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.SEXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
 
 /*getarg#1*/
 /*^getarg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
 gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);
 
 
 /*getarg#2*/
 /*^getarg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
 /*_.MEXPANDER__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
 gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);
 
 
 /*getarg#3*/
 /*^getarg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
 /*_.MODCTX__V5*/ meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
 gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);
 
 ;
 goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
/*anyblock*/{
 
 
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:45:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:45:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:45:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[8];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
      argtab[2].meltbp_long = 45;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list macro sexp=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXP__V2*/ meltfptr[1];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
      argtab[5].meltbp_cstring =  "\n env=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
      argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
      argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
      /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V6*/ meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:45:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
   
    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:45:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
   /*clear*/ /*_._IF___V6*/ meltfptr[5] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 MELT_LOCATION("warmelt-moremacro.melt:46:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 46   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 MELT_LOCATION("warmelt-moremacro.melt:47:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 47   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*cond*/ if (
#ifndef MELTGCC_NOLINENUMBERING
#line 47   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]), 
                                                                       (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[2])))
				      ) /*then*/ {
 /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 47
#endif
 /*^getslot*/
#ifndef MELTGCC_NOLINENUMBERING
#line 47
#endif
  { melt_ptr_t slot=NULL, obj=NULL;
   obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
   melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
   /*_.SLOC__V9*/ meltfptr[5] = slot; };
  ;
 } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 47
#endif
 
  /*_.SLOC__V9*/ meltfptr[5] =  /*reallynil*/ NULL ;;
  }
 ;
 MELT_LOCATION("warmelt-moremacro.melt:48:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 48   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*cond*/ if (
#ifndef MELTGCC_NOLINENUMBERING
#line 48   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]), 
                                                                       (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[3])))
				      ) /*then*/ {
 /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 48
#endif
 /*^getslot*/
#ifndef MELTGCC_NOLINENUMBERING
#line 48
#endif
  { melt_ptr_t slot=NULL, obj=NULL;
   obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
   melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
   /*_.SEXCONT__V10*/ meltfptr[9] = slot; };
  ;
 } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 48
#endif
 
  /*_.SEXCONT__V10*/ meltfptr[9] =  /*reallynil*/ NULL ;;
  }
 ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:50:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L3*/ meltfnum[1] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:50:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L3*/ meltfnum[1]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L4*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:50:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[8];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[0];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
      argtab[2].meltbp_long = 50;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list sloc=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
      argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V9*/ meltfptr[5];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
      argtab[6].meltbp_cstring =  " sexcont=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
      argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V10*/ meltfptr[9];
      /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V11*/ meltfptr[10] = /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:50:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L4*/ meltfnum[0] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
   
    /*_._IF___V11*/ meltfptr[10] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:50:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 50   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L3*/ meltfnum[1] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
   /*clear*/ /*_._IF___V11*/ meltfptr[10] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 50
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:51:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
  /*anyblock*/{
   
   
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#IS_LIST__L5*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    (melt_magic_discr((melt_ptr_t)(/*_.SEXCONT__V10*/ meltfptr[9])) == MELTOBMAG_LIST);;
   MELT_LOCATION("warmelt-moremacro.melt:51:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#IS_LIST__L5*/ meltfnum[0]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
   /*_._IFELSE___V14*/ meltfptr[10] = (/*nil*/NULL);;
   } else {MELT_LOCATION("warmelt-moremacro.melt:51:/ cond.else");
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::cond.else::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   
    /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
    /*anyblock*/{
     
     
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     /*^apply*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
     /*apply*/{
      union meltparam_un argtab[4];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
      argtab[0].meltbp_cstring =  "check sexcont";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
      argtab[2].meltbp_long = 51;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
      argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V10*/ meltfptr[9];
      /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IFELSE___V14*/ meltfptr[10] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:51:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;}
    ;
    }
   ;
   /*_.IFCPP___V13*/ meltfptr[11] = /*_._IFELSE___V14*/ meltfptr[10];;
   /*epilog*/
   
   MELT_LOCATION("warmelt-moremacro.melt:51:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*clear*/ /*_#IS_LIST__L5*/ meltfnum[0] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
   /*clear*/ /*_._IFELSE___V14*/ meltfptr[10] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 51
#endif
 /*_.IFCPP___V13*/ meltfptr[11] = (/*nil*/NULL);
  #endif /*MELT_HAVE_DEBUG*/
  ;
 MELT_LOCATION("warmelt-moremacro.melt:52:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 52   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 
#ifndef MELTGCC_NOLINENUMBERING
#line 53   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 MELT_LOCATION("warmelt-moremacro.melt:53:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 53   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*apply*/{
  union meltparam_un argtab[1];
  memset(&argtab, 0, sizeof(argtab));
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 53
#endif
  argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[6]);
  /*_.TCONT__V17*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SEXCONT__V10*/ meltfptr[9]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
  }
 ;
 /*_#TCONTLEN__L6*/ meltfnum[1] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 54   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  (melt_multiple_length((melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10])));;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:56:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L7*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:56:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L7*/ meltfnum[0]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:56:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[7];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
      argtab[2].meltbp_long = 56;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list tcont=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V17*/ meltfptr[10];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
      argtab[5].meltbp_cstring =  "; tcontlen=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
      argtab[6].meltbp_long = /*_#TCONTLEN__L6*/ meltfnum[1];
      /*_.MELT_DEBUG_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V18*/ meltfptr[17] = /*_.MELT_DEBUG_FUN__V19*/ meltfptr[18];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:56:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V19*/ meltfptr[18] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
   
    /*_._IF___V18*/ meltfptr[17] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:56:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 56   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L7*/ meltfnum[0] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
   /*clear*/ /*_._IF___V18*/ meltfptr[17] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 56
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 57   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:57:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 57   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
  /*anyblock*/{
   
   
#ifndef MELTGCC_NOLINENUMBERING
#line 57   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#IS_MULTIPLE__L9*/ meltfnum[7] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 57   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    (melt_magic_discr((melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10])) == MELTOBMAG_MULTIPLE);;
   MELT_LOCATION("warmelt-moremacro.melt:57:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 57   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#IS_MULTIPLE__L9*/ meltfnum[7]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
   /*_._IFELSE___V21*/ meltfptr[17] = (/*nil*/NULL);;
   } else {MELT_LOCATION("warmelt-moremacro.melt:57:/ cond.else");
#ifndef MELTGCC_NOLINENUMBERING
#line 57   "warmelt-moremacro.melt" /**::cond.else::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   
    /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
    /*anyblock*/{
     
     
#ifndef MELTGCC_NOLINENUMBERING
#line 57   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     /*^apply*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
     /*apply*/{
      union meltparam_un argtab[5];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
      argtab[0].meltbp_cstring =  "check tcont";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
      argtab[2].meltbp_long = 57;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
      argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V17*/ meltfptr[10];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V10*/ meltfptr[9];
      /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IFELSE___V21*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:57:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 57   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] = 0 ;}
    ;
    }
   ;
   /*_.IFCPP___V20*/ meltfptr[18] = /*_._IFELSE___V21*/ meltfptr[17];;
   /*epilog*/
   
   MELT_LOCATION("warmelt-moremacro.melt:57:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 57   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*clear*/ /*_#IS_MULTIPLE__L9*/ meltfnum[7] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
   /*clear*/ /*_._IFELSE___V21*/ meltfptr[17] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 57
#endif
 /*_.IFCPP___V20*/ meltfptr[18] = (/*nil*/NULL);
  #endif /*MELT_HAVE_DEBUG*/
  ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 58   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:58:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 58   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
  /*anyblock*/{
   
   
#ifndef MELTGCC_NOLINENUMBERING
#line 58   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#gtI__L10*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 58   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    ((/*_#TCONTLEN__L6*/ meltfnum[1]) > (0));;
   MELT_LOCATION("warmelt-moremacro.melt:58:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 58   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#gtI__L10*/ meltfnum[0]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
   /*_._IFELSE___V24*/ meltfptr[17] = (/*nil*/NULL);;
   } else {MELT_LOCATION("warmelt-moremacro.melt:58:/ cond.else");
#ifndef MELTGCC_NOLINENUMBERING
#line 58   "warmelt-moremacro.melt" /**::cond.else::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   
    /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
    /*anyblock*/{
     
     
#ifndef MELTGCC_NOLINENUMBERING
#line 58   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     /*^apply*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
     /*apply*/{
      union meltparam_un argtab[5];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
      argtab[0].meltbp_cstring =  "check tcontlen";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
      argtab[2].meltbp_long = 58;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
      argtab[3].meltbp_long = /*_#TCONTLEN__L6*/ meltfnum[1];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V17*/ meltfptr[10];
      /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IFELSE___V24*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:58:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 58   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;}
    ;
    }
   ;
   /*_.IFCPP___V23*/ meltfptr[21] = /*_._IFELSE___V24*/ meltfptr[17];;
   /*epilog*/
   
   MELT_LOCATION("warmelt-moremacro.melt:58:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 58   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*clear*/ /*_#gtI__L10*/ meltfnum[0] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
   /*clear*/ /*_._IFELSE___V24*/ meltfptr[17] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 58
#endif
 /*_.IFCPP___V23*/ meltfptr[21] = (/*nil*/NULL);
  #endif /*MELT_HAVE_DEBUG*/
  ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 59   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*_#ltI__L11*/ meltfnum[7] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 59   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  ((/*_#TCONTLEN__L6*/ meltfnum[1]) < (4));;
 MELT_LOCATION("warmelt-moremacro.melt:59:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 59   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*cond*/ if (/*_#ltI__L11*/ meltfnum[7]) /*then*/ {
 /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 59
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 59
#endif
  /*anyblock*/{
   
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:60:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 60   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /* error_plain */ melt_error_str((melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]), ( "EACH_COMPONENT_IN_LIST <expr-list> <var-comp> too short"), (melt_ptr_t)0);}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 61   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   MELT_LOCATION("warmelt-moremacro.melt:61:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 61   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   
   
   /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;
   MELT_LOCATION("warmelt-moremacro.melt:61:/ putxtraresult");
#ifndef MELTGCC_NOLINENUMBERING
#line 61   "warmelt-moremacro.melt" /**::putxtraresult::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
   if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
   if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
   ;
   /*^finalreturn*/
#ifndef MELTGCC_NOLINENUMBERING
#line 61
#endif
   ;
   /*finalret*/ goto meltlabend_rout ;
   MELT_LOCATION("warmelt-moremacro.melt:59:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 59   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   
   
   /*_.PROGN___V28*/ meltfptr[27] = /*_.RETURN___V27*/ meltfptr[17];;
   /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
   /*_._IF___V26*/ meltfptr[24] = /*_.PROGN___V28*/ meltfptr[27];;
   /*epilog*/
   
   MELT_LOCATION("warmelt-moremacro.melt:59:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 59   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*clear*/ /*_.RETURN___V27*/ meltfptr[17] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 59
#endif
   /*clear*/ /*_.PROGN___V28*/ meltfptr[27] = 0 ;}
  ;
 } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 59
#endif
 
  /*_._IF___V26*/ meltfptr[24] =  /*reallynil*/ NULL ;;
  }
 ;
 MELT_LOCATION("warmelt-moremacro.melt:62:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 62   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 MELT_LOCATION("warmelt-moremacro.melt:63:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 63   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 /*_.SE__V31*/ meltfptr[30] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 63   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10]), (1)));;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:65:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L12*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:65:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L12*/ meltfnum[0]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L13*/ meltfnum[12] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:65:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[5];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
      argtab[2].meltbp_long = 65;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list sexplist=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SE__V31*/ meltfptr[30];
      /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V32*/ meltfptr[31] = /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:65:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L13*/ meltfnum[12] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
   
    /*_._IF___V32*/ meltfptr[31] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:65:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 65   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L12*/ meltfnum[0] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
   /*clear*/ /*_._IF___V32*/ meltfptr[31] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 65
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 /*_.LET___V30*/ meltfptr[27] = /*_.SE__V31*/ meltfptr[30];;
 
 MELT_LOCATION("warmelt-moremacro.melt:63:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 63   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.SE__V31*/ meltfptr[30] = 0 ;
 /*_.SEXPLIST__V34*/ meltfptr[32] = /*_.LET___V30*/ meltfptr[27];;
 MELT_LOCATION("warmelt-moremacro.melt:67:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 67   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 /*_.SV__V36*/ meltfptr[30] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 67   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10]), (2)));;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:69:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L14*/ meltfnum[12] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:69:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L14*/ meltfnum[12]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L15*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:69:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[5];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[0];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
      argtab[2].meltbp_long = 69;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list svarcomp=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SV__V36*/ meltfptr[30];
      /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V37*/ meltfptr[36] = /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:69:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L15*/ meltfnum[0] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
   
    /*_._IF___V37*/ meltfptr[36] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:69:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 69   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L14*/ meltfnum[12] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
   /*clear*/ /*_._IF___V37*/ meltfptr[36] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 69
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 /*_.LET___V35*/ meltfptr[31] = /*_.SV__V36*/ meltfptr[30];;
 
 MELT_LOCATION("warmelt-moremacro.melt:67:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 67   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.SV__V36*/ meltfptr[30] = 0 ;
 /*_.SVARCOMP__V39*/ meltfptr[37] = /*_.LET___V35*/ meltfptr[31];;
 MELT_LOCATION("warmelt-moremacro.melt:71:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 71   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 
#ifndef MELTGCC_NOLINENUMBERING
#line 71   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 /*^apply*/
#ifndef MELTGCC_NOLINENUMBERING
#line 71
#endif
 /*apply*/{
  union meltparam_un argtab[3];
  memset(&argtab, 0, sizeof(argtab));
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 71
#endif
  argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 71
#endif
  argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 71
#endif
  argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
  /*_.ME__V41*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SEXPLIST__V34*/ meltfptr[32]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
  }
 ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:73:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L16*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:73:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L16*/ meltfnum[0]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L17*/ meltfnum[12] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:73:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[5];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[12];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
      argtab[2].meltbp_long = 73;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list mlistexp=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ME__V41*/ meltfptr[30];
      /*_.MELT_DEBUG_FUN__V43*/ meltfptr[42] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V42*/ meltfptr[41] = /*_.MELT_DEBUG_FUN__V43*/ meltfptr[42];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:73:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L17*/ meltfnum[12] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V43*/ meltfptr[42] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
   
    /*_._IF___V42*/ meltfptr[41] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:73:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 73   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L16*/ meltfnum[0] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
   /*clear*/ /*_._IF___V42*/ meltfptr[41] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 73
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 /*_.LET___V40*/ meltfptr[36] = /*_.ME__V41*/ meltfptr[30];;
 
 MELT_LOCATION("warmelt-moremacro.melt:71:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 71   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.ME__V41*/ meltfptr[30] = 0 ;
 /*_.MLISTEXP__V44*/ meltfptr[42] = /*_.LET___V40*/ meltfptr[36];;
 MELT_LOCATION("warmelt-moremacro.melt:75:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 75   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 
#ifndef MELTGCC_NOLINENUMBERING
#line 75   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 /*^apply*/
#ifndef MELTGCC_NOLINENUMBERING
#line 75
#endif
 /*apply*/{
  union meltparam_un argtab[3];
  memset(&argtab, 0, sizeof(argtab));
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 75
#endif
  argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 75
#endif
  argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 75
#endif
  argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
  /*_.MV__V46*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SVARCOMP__V39*/ meltfptr[37]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
  }
 ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:77:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L18*/ meltfnum[12] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:77:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L18*/ meltfnum[12]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L19*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:77:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[5];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L19*/ meltfnum[0];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
      argtab[2].meltbp_long = 77;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list mvarcomp=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MV__V46*/ meltfptr[30];
      /*_.MELT_DEBUG_FUN__V48*/ meltfptr[47] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V47*/ meltfptr[46] = /*_.MELT_DEBUG_FUN__V48*/ meltfptr[47];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:77:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L19*/ meltfnum[0] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V48*/ meltfptr[47] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
   
    /*_._IF___V47*/ meltfptr[46] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:77:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 77   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L18*/ meltfnum[12] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
   /*clear*/ /*_._IF___V47*/ meltfptr[46] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 77
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 /*_.LET___V45*/ meltfptr[41] = /*_.MV__V46*/ meltfptr[30];;
 
 MELT_LOCATION("warmelt-moremacro.melt:75:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 75   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.MV__V46*/ meltfptr[30] = 0 ;
 /*_.MVARCOMP__V49*/ meltfptr[47] = /*_.LET___V45*/ meltfptr[41];;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:80:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L20*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:80:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L20*/ meltfnum[0]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L21*/ meltfnum[12] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:80:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[7];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L21*/ meltfnum[12];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
      argtab[2].meltbp_long = 80;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list tcont=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V17*/ meltfptr[10];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
      argtab[5].meltbp_cstring =  "\n.. before expand_tuple_slice_as_tuple=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
      argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!EXPAND_TUPLE_SLICE_AS_TUPLE*/ meltfrout->tabval[7]);
      /*_.MELT_DEBUG_FUN__V51*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V50*/ meltfptr[46] = /*_.MELT_DEBUG_FUN__V51*/ meltfptr[30];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:80:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L21*/ meltfnum[12] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V51*/ meltfptr[30] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
   
    /*_._IF___V50*/ meltfptr[46] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:80:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L20*/ meltfnum[0] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
   /*clear*/ /*_._IF___V50*/ meltfptr[46] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 80
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 MELT_LOCATION("warmelt-moremacro.melt:82:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 82   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 
#ifndef MELTGCC_NOLINENUMBERING
#line 82   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 /*^apply*/
#ifndef MELTGCC_NOLINENUMBERING
#line 82
#endif
 /*apply*/{
  union meltparam_un argtab[5];
  memset(&argtab, 0, sizeof(argtab));
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 82
#endif
  argtab[0].meltbp_long = 3;
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 82
#endif
  argtab[1].meltbp_long = -1;
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 82
#endif
  argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 82
#endif
  argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 82
#endif
  argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
  /*_.BE__V53*/ meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_TUPLE_SLICE_AS_TUPLE*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10]), (MELTBPARSTR_LONG MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
  }
 ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:84:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L22*/ meltfnum[12] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:84:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L22*/ meltfnum[12]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L23*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:84:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[5];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L23*/ meltfnum[0];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
      argtab[2].meltbp_long = 84;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list bodyexp=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BE__V53*/ meltfptr[46];
      /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V54*/ meltfptr[53] = /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:84:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L23*/ meltfnum[0] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
   
    /*_._IF___V54*/ meltfptr[53] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:84:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 84   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L22*/ meltfnum[12] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
   /*clear*/ /*_._IF___V54*/ meltfptr[53] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 84
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 /*_.LET___V52*/ meltfptr[30] = /*_.BE__V53*/ meltfptr[46];;
 
 MELT_LOCATION("warmelt-moremacro.melt:82:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 82   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.BE__V53*/ meltfptr[46] = 0 ;
 MELT_LOCATION("warmelt-moremacro.melt:79:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 79   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 /*_.BODYEXP__V56*/ meltfptr[54] = /*_.LET___V52*/ meltfptr[30];;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 87   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*_#IS_NOT_A__L24*/ meltfnum[0] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 87   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  !melt_is_instance_of((melt_ptr_t)(/*_.MVARCOMP__V49*/ meltfptr[47]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[8])));;
 MELT_LOCATION("warmelt-moremacro.melt:87:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 87   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*cond*/ if (/*_#IS_NOT_A__L24*/ meltfnum[0]) /*then*/ {
 /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 87
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 87
#endif
  /*anyblock*/{
   
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:88:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 88   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /* error_plain */ melt_error_str((melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]), ( "EACH_COMPONENT_IN_LIST <expr-list> <var-comp> bad second argument for\
 component variable"), (melt_ptr_t)0);}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 89   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   MELT_LOCATION("warmelt-moremacro.melt:89:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 89   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   
   
   /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;
   MELT_LOCATION("warmelt-moremacro.melt:89:/ putxtraresult");
#ifndef MELTGCC_NOLINENUMBERING
#line 89   "warmelt-moremacro.melt" /**::putxtraresult::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
   if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
   if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
   ;
   /*^finalreturn*/
#ifndef MELTGCC_NOLINENUMBERING
#line 89
#endif
   ;
   /*finalret*/ goto meltlabend_rout ;
   MELT_LOCATION("warmelt-moremacro.melt:87:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 87   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   
   
   /*_.PROGN___V59*/ meltfptr[58] = /*_.RETURN___V58*/ meltfptr[46];;
   /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
   /*_._IF___V57*/ meltfptr[53] = /*_.PROGN___V59*/ meltfptr[58];;
   /*epilog*/
   
   MELT_LOCATION("warmelt-moremacro.melt:87:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 87   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*clear*/ /*_.RETURN___V58*/ meltfptr[46] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 87
#endif
   /*clear*/ /*_.PROGN___V59*/ meltfptr[58] = 0 ;}
  ;
 } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 87
#endif
 
  /*_._IF___V57*/ meltfptr[53] =  /*reallynil*/ NULL ;;
  }
 ;
 MELT_LOCATION("warmelt-moremacro.melt:90:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 90   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 
#ifndef MELTGCC_NOLINENUMBERING
#line 91   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 MELT_LOCATION("warmelt-moremacro.melt:91:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 91   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*apply*/{
  /*_.CURPAIRINLISTSYMB__V61*/ meltfptr[58] =  melt_apply ((meltclosure_ptr_t)((/*!CLONE_SYMBOL*/ meltfrout->tabval[9])), (melt_ptr_t)((/*!konst_10_CURPAIRINLISTSYMB*/ meltfrout->tabval[10])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
  }
 ;
 MELT_LOCATION("warmelt-moremacro.melt:92:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 92   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 /*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 93   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[12]),  "warmelt-moremacro.melt", 93) ;;
 /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*_.HOOK_MELT_MAKE_LOCATION__V64*/ meltfptr[63] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 94   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[12]),  "warmelt-moremacro.melt", 94) ;;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 94   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 MELT_LOCATION("warmelt-moremacro.melt:94:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 94   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*apply*/{
  union meltparam_un argtab[1];
  memset(&argtab, 0, sizeof(argtab));
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 94
#endif
  argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MLISTEXP__V44*/ meltfptr[42];
  /*_.MELT_MAKE_SEXPR__V65*/ meltfptr[64] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V64*/ meltfptr[63]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
  }
 ;
 /*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 95   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[12]),  "warmelt-moremacro.melt", 95) ;;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 95   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 MELT_LOCATION("warmelt-moremacro.melt:95:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 95   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*apply*/{
  union meltparam_un argtab[2];
  memset(&argtab, 0, sizeof(argtab));
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 95
#endif
  argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURPAIRINLISTSYMB__V61*/ meltfptr[58];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 95
#endif
  argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MVARCOMP__V49*/ meltfptr[47];
  /*_.MELT_MAKE_SEXPR__V67*/ meltfptr[66] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
  }
 ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 93   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 MELT_LOCATION("warmelt-moremacro.melt:93:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 93   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*apply*/{
  union meltparam_un argtab[4];
  memset(&argtab, 0, sizeof(argtab));
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 93
#endif
  argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_13_FOREACH_PAIR_COMPONENT_IN_LIST*/ meltfrout->tabval[13]);
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 93
#endif
  argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V65*/ meltfptr[64];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 93
#endif
  argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V67*/ meltfptr[66];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 93
#endif
  argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.BODYEXP__V56*/ meltfptr[54];
  /*_.RESEXP__V68*/ meltfptr[67] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
  }
 ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:98:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L25*/ meltfnum[12] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:98:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L25*/ meltfnum[12]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L26*/ meltfnum[25] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:98:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[5];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L26*/ meltfnum[25];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
      argtab[2].meltbp_long = 98;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list resexp=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESEXP__V68*/ meltfptr[67];
      /*_.MELT_DEBUG_FUN__V70*/ meltfptr[69] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V69*/ meltfptr[68] = /*_.MELT_DEBUG_FUN__V70*/ meltfptr[69];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:98:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L26*/ meltfnum[25] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V70*/ meltfptr[69] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
   
    /*_._IF___V69*/ meltfptr[68] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:98:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 98   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L25*/ meltfnum[12] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
   /*clear*/ /*_._IF___V69*/ meltfptr[68] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 98
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 MELT_LOCATION("warmelt-moremacro.melt:99:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 99   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 
#ifndef MELTGCC_NOLINENUMBERING
#line 99   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 /*^apply*/
#ifndef MELTGCC_NOLINENUMBERING
#line 99
#endif
 /*apply*/{
  union meltparam_un argtab[3];
  memset(&argtab, 0, sizeof(argtab));
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 99
#endif
  argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 99
#endif
  argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
  /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 99
#endif
  argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
  /*_.MEXP__V72*/ meltfptr[68] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.RESEXP__V68*/ meltfptr[67]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
  }
 ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 101   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 MELT_LOCATION("warmelt-moremacro.melt:101:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 101   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*cond*/ if (
#ifndef MELTGCC_NOLINENUMBERING
#line 101   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.MEXP__V72*/ meltfptr[68]), 
                                                                       (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[2])))
				      ) /*then*/ {
 /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 101
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 101
#endif
  /*anyblock*/{
   
   /*^putslot*/
#ifndef MELTGCC_NOLINENUMBERING
#line 101
#endif
   /*putslot*/
   melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.MEXP__V72*/ meltfptr[68])) == MELTOBMAG_OBJECT);
   melt_putfield_object((/*_.MEXP__V72*/ meltfptr[68]), (1), (/*_.SLOC__V9*/ meltfptr[5]), "LOCA_LOCATION");
   ;
   /*^touch*/
#ifndef MELTGCC_NOLINENUMBERING
#line 101
#endif
   meltgc_touch(/*_.MEXP__V72*/ meltfptr[68]);
   ;
   /*^touchobj*/
#ifndef MELTGCC_NOLINENUMBERING
#line 101
#endif
   
   melt_dbgtrace_written_object (/*_.MEXP__V72*/ meltfptr[68], "put-fields");
   ;
   /*epilog*/
   }
  ;
 } /*noelse*/
 ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::cppif::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 #if MELT_HAVE_DEBUG
  MELT_LOCATION("warmelt-moremacro.melt:102:/ cppif.then");
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::cppif.then::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
   /*melt_increment_dbgcounter*/ 
#if MELT_HAVE_DEBUG
melt_dbgcounter++;
#endif
;}
   ;
   
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    MELT_CHECK_SIGNAL();
   ;
   /*_#MELT_NEED_DBG__L27*/ meltfnum[25] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
 (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
 0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
;;
   MELT_LOCATION("warmelt-moremacro.melt:102:/ cond");
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::cond::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*cond*/ if (/*_#MELT_NEED_DBG__L27*/ meltfnum[25]) /*then*/ {
   /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
    /*anyblock*/{
     
     /*_#MELT_CALLCOUNT__L28*/ meltfnum[12] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
  meltcallcount  /* melt_callcount debugging */
#else
  0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
;;
     
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      MELT_CHECK_SIGNAL();
     ;
     MELT_LOCATION("warmelt-moremacro.melt:102:/ apply");
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::apply::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*apply*/{
      union meltparam_un argtab[5];
      memset(&argtab, 0, sizeof(argtab));
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L28*/ meltfnum[12];
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
      argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
      argtab[2].meltbp_long = 102;
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
      argtab[3].meltbp_cstring =  "each_component_in_list final mexp=";
      /*^apply.arg*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
      argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXP__V72*/ meltfptr[68];
      /*_.MELT_DEBUG_FUN__V74*/ meltfptr[73] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
      }
     ;
     /*_._IF___V73*/ meltfptr[72] = /*_.MELT_DEBUG_FUN__V74*/ meltfptr[73];;
     /*epilog*/
     
     MELT_LOCATION("warmelt-moremacro.melt:102:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
     /*clear*/ /*_#MELT_CALLCOUNT__L28*/ meltfnum[12] = 0 ;
     /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
     /*clear*/ /*_.MELT_DEBUG_FUN__V74*/ meltfptr[73] = 0 ;}
    ;
   } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
   
    /*_._IF___V73*/ meltfptr[72] =  /*reallynil*/ NULL ;;
    }
   ;
   
   {
   MELT_LOCATION("warmelt-moremacro.melt:102:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 102   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*void*/(void)0;}
   ;
   /*^quasiblock*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
   
   
   /*epilog*/
   
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
   /*clear*/ /*_#MELT_NEED_DBG__L27*/ meltfnum[25] = 0 ;
   /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
   /*clear*/ /*_._IF___V73*/ meltfptr[72] = 0 ;}
  
  #else /*MELT_HAVE_DEBUG*/
  /*^cppif.else*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
  /*anyblock*/{
   
   
   {
   /*^locexp*/
#ifndef MELTGCC_NOLINENUMBERING
#line 102
#endif
   /*void*/(void)0;}
   ;
   /*epilog*/
   }
  
  #endif /*MELT_HAVE_DEBUG*/
  ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 103   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 MELT_LOCATION("warmelt-moremacro.melt:103:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 103   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 /*_._RETVAL___V1*/ meltfptr[0] = /*_.MEXP__V72*/ meltfptr[68];;
 
 {
 MELT_LOCATION("warmelt-moremacro.melt:103:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 103   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
 if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
    melt_warn_for_no_expected_secondary_results();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
;}
 ;
 /*^finalreturn*/
#ifndef MELTGCC_NOLINENUMBERING
#line 103
#endif
 ;
 /*finalret*/ goto meltlabend_rout ;
 /*_.LET___V71*/ meltfptr[69] = /*_.RETURN___V75*/ meltfptr[73];;
 
 MELT_LOCATION("warmelt-moremacro.melt:99:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 99   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.MEXP__V72*/ meltfptr[68] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 99
#endif
 /*clear*/ /*_.RETURN___V75*/ meltfptr[73] = 0 ;
 /*_.LET___V62*/ meltfptr[61] = /*_.LET___V71*/ meltfptr[69];;
 
 MELT_LOCATION("warmelt-moremacro.melt:92:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 92   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 92
#endif
 /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V64*/ meltfptr[63] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 92
#endif
 /*clear*/ /*_.MELT_MAKE_SEXPR__V65*/ meltfptr[64] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 92
#endif
 /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 92
#endif
 /*clear*/ /*_.MELT_MAKE_SEXPR__V67*/ meltfptr[66] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 92
#endif
 /*clear*/ /*_.RESEXP__V68*/ meltfptr[67] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 92
#endif
 /*clear*/ /*_.LET___V71*/ meltfptr[69] = 0 ;
 /*_.LET___V60*/ meltfptr[46] = /*_.LET___V62*/ meltfptr[61];;
 
 MELT_LOCATION("warmelt-moremacro.melt:90:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 90   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.CURPAIRINLISTSYMB__V61*/ meltfptr[58] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 90
#endif
 /*clear*/ /*_.LET___V62*/ meltfptr[61] = 0 ;
 /*_.LET___V29*/ meltfptr[17] = /*_.LET___V60*/ meltfptr[46];;
 
 MELT_LOCATION("warmelt-moremacro.melt:62:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 62   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.LET___V30*/ meltfptr[27] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.SEXPLIST__V34*/ meltfptr[32] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.LET___V35*/ meltfptr[31] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.SVARCOMP__V39*/ meltfptr[37] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.LET___V40*/ meltfptr[36] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.MLISTEXP__V44*/ meltfptr[42] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.LET___V45*/ meltfptr[41] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.MVARCOMP__V49*/ meltfptr[47] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.LET___V52*/ meltfptr[30] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.BODYEXP__V56*/ meltfptr[54] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_#IS_NOT_A__L24*/ meltfnum[0] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_._IF___V57*/ meltfptr[53] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 62
#endif
 /*clear*/ /*_.LET___V60*/ meltfptr[46] = 0 ;
 /*_.LET___V16*/ meltfptr[14] = /*_.LET___V29*/ meltfptr[17];;
 
 MELT_LOCATION("warmelt-moremacro.melt:52:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 52   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.TCONT__V17*/ meltfptr[10] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 52
#endif
 /*clear*/ /*_#TCONTLEN__L6*/ meltfnum[1] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 52
#endif
 /*clear*/ /*_.IFCPP___V20*/ meltfptr[18] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 52
#endif
 /*clear*/ /*_.IFCPP___V23*/ meltfptr[21] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 52
#endif
 /*clear*/ /*_#ltI__L11*/ meltfnum[7] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 52
#endif
 /*clear*/ /*_._IF___V26*/ meltfptr[24] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 52
#endif
 /*clear*/ /*_.LET___V29*/ meltfptr[17] = 0 ;
 /*_.LET___V8*/ meltfptr[6] = /*_.LET___V16*/ meltfptr[14];;
 
 MELT_LOCATION("warmelt-moremacro.melt:46:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 46   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*clear*/ /*_.SLOC__V9*/ meltfptr[5] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 46
#endif
 /*clear*/ /*_.SEXCONT__V10*/ meltfptr[9] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 46
#endif
 /*clear*/ /*_.IFCPP___V13*/ meltfptr[11] = 0 ;
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 46
#endif
 /*clear*/ /*_.LET___V16*/ meltfptr[14] = 0 ;
 
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::checksignal::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  MELT_CHECK_SIGNAL();
 ;
 MELT_LOCATION("warmelt-moremacro.melt:39:/ quasiblock");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::quasiblock::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 
 
 /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;
 
 {
 MELT_LOCATION("warmelt-moremacro.melt:39:/ locexp");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::locexp::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
 if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
    melt_warn_for_no_expected_secondary_results();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
;}
 ;
 /*^finalreturn*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 ;
 /*finalret*/ goto meltlabend_rout ;
 /*epilog*/
 
 /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;}

;
 goto meltlabend_rout;
meltlabend_rout:
melt_trace_end("EACH_COMPONENT_IN_LIST", meltcallcount);
  melt_blocklevel_signals = current_blocklevel_signals_meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST_melt;
 return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST*/



/*no syntax testing generated*/

/* the frame for initial routine */
/** start of frame for melt_start_this_module of CLASS_INITIAL_MODULE_ROUTINEOBJ from 1522**/

 /*curframdeclclassy*/ class Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110// fromline 1535
  : public Melt_CallFrameWithValues<53> {
 public: /* fromline 1539*/
 // no classy longs
 /*classy others*/
 long _meltsparel;
 void melt_mark_stuff (void) {
  } /*end melt_mark_stuff*/
 virtual void melt_mark_ggc_data (void) {
  melt_mark_values ();
  melt_mark_stuff ();
 }; /*end melt_mark_ggc_data*/
 Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110(meltclosure_ptr_t clos) //the closure constructor fromline 1597
   : Melt_CallFrameWithValues<53> (
#if ENABLE_CHECKING /*fromline 1601*/
    __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1605*/
   sizeof(Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110), clos) {};
 Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110() //the constructor fromline 1609
   : Melt_CallFrameWithValues<53> (
#if ENABLE_CHECKING /*fromline 1613*/
    __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1617*/
   sizeof(Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110)) {};
#if ENABLE_CHECKING /*fromline 1621*/
  Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110(const char*fil, int lin) //the constructor fromline 1623
   : Melt_CallFrameWithValues<53> (fil,lin, sizeof(Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110)) {};
#endif /* ENABLE_CHECKING fromline 1633*/
  
}; // end  class Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h209339110


/** end of frame for melt_start_this_module fromline 1664**/



static void melt_initialize_module_meltdata_WARMELTmiMOREMACRO (meltinitial_frame_t *iniframp__, char meltpredefinited[])
{
#define meltfram__  (*iniframp__)
 
 /* curframe_cdat_struct */ struct meltcdata_st {
 void* meltcdata_inigap[2]; /* could help debugging */
 
  struct MELT_OBJECT_STRUCT(3) dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER;
  struct MELT_STRING_STRUCT(38) dstr_2__HOOK_FRESH_ENVIRONMENT_R;
  struct MELT_ROUTINE_STRUCT(14) drout_3__EACH_COMPONENT_IN_LIST;
  struct MELT_CLOSURE_STRUCT(0) dclo_4__EACH_COMPONENT_IN_LIST;
  struct MELT_OBJECT_STRUCT(3) dsym_5__MELT_DEBUG_FUN;
  struct MELT_STRING_STRUCT(14) dstr_6__MELT_DEBUG_FUN;
  struct MELT_OBJECT_STRUCT(3) dsym_7__DEBUG_LESS;
  struct MELT_STRING_STRUCT(10) dstr_8__DEBUG_LESS;
  struct MELT_OBJECT_STRUCT(3) dsym_9__CLASS_LOCATED;
  struct MELT_STRING_STRUCT(13) dstr_10__CLASS_LOCATED;
  struct MELT_OBJECT_STRUCT(3) dsym_11__CLASS_SEXPR;
  struct MELT_STRING_STRUCT(11) dstr_12__CLASS_SEXPR;
  struct MELT_OBJECT_STRUCT(3) dsym_13__MELT_ASSERT_FAILURE_FUN;
  struct MELT_STRING_STRUCT(23) dstr_14__MELT_ASSERT_FAILURE_FUN;
  struct MELT_OBJECT_STRUCT(3) dsym_15__LIST_TO_MULTIPLE;
  struct MELT_STRING_STRUCT(16) dstr_16__LIST_TO_MULTIPLE;
  struct MELT_OBJECT_STRUCT(3) dsym_17__DISCR_MULTIPLE;
  struct MELT_STRING_STRUCT(14) dstr_18__DISCR_MULTIPLE;
  struct MELT_OBJECT_STRUCT(3) dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE;
  struct MELT_STRING_STRUCT(27) dstr_20__EXPAND_TUPLE_SLICE_AS_TU;
  struct MELT_OBJECT_STRUCT(3) dsym_21__CLASS_SYMBOL;
  struct MELT_STRING_STRUCT(12) dstr_22__CLASS_SYMBOL;
  struct MELT_OBJECT_STRUCT(3) dsym_23__CLONE_SYMBOL;
  struct MELT_STRING_STRUCT(12) dstr_24__CLONE_SYMBOL;
  struct MELT_OBJECT_STRUCT(3) dsym_25__CURPAIRINLISTSYMB;
  struct MELT_STRING_STRUCT(17) dstr_26__CURPAIRINLISTSYMB;
  struct MELT_OBJECT_STRUCT(3) dsym_27__MELT_MAKE_SEXPR;
  struct MELT_STRING_STRUCT(15) dstr_28__MELT_MAKE_SEXPR;
  struct MELT_OBJECT_STRUCT(3) dsym_29__HOOK_MELT_MAKE_LOCATION;
  struct MELT_STRING_STRUCT(23) dstr_30__HOOK_MELT_MAKE_LOCATION;
  struct MELT_OBJECT_STRUCT(3) dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST;
  struct MELT_STRING_STRUCT(30) dstr_32__FOREACH_PAIR_COMPONENT_I;
  struct MELT_OBJECT_STRUCT(3) dsym_33__EACH_COMPONENT_IN_LIST;
  struct MELT_STRING_STRUCT(22) dstr_34__EACH_COMPONENT_IN_LIST;
 long spare_;
} *meltcdat = NULL;
 dbgprintf ("start melt_initialize_module_meltdata_WARMELTmiMOREMACRO iniframp__=%p", (void*) iniframp__);
 (void) meltpredefinited; /* avoid warning if non-used. */
  /*curframe_cdat_fill */ 
  meltcdat = (struct meltcdata_st*) meltgc_allocate(sizeof(*meltcdat),0);
  melt_prohibit_garbcoll = TRUE;
 /*initial routine predef*/
 /*initial routine fill*/
 
 
/*iniobj dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER*/
/*uniqueobj*/ if (!/*_._VALDATA___V18*/ meltfptr[17]) 
/*_._VALDATA___V18*/ meltfptr[17] = (melt_ptr_t )&meltcdat->dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER.obj_hash = 824058733;
 meltcdat->dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER.obj_len = 3;

 
/*inistring dstr_2__HOOK_FRESH_ENVIRONMENT_R*/
 /*_._VALSTR___V19*/ meltfptr[18] = (melt_ptr_t) &meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R;
  meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.val, "HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER", sizeof (meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.val)-1);
 meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.val[38] = (char)0 ; 
 meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.slen = 38 ; 
 
 /*inirout drout_3__EACH_COMPONENT_IN_LIST*/
 /*iniroutlocvar*/ /*_._VALROUT___V20*/ meltfptr[19] = (melt_ptr_t) &meltcdat->drout_3__EACH_COMPONENT_IN_LIST;
  meltcdat->drout_3__EACH_COMPONENT_IN_LIST.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_ROUTINE))));
  strncpy (meltcdat->drout_3__EACH_COMPONENT_IN_LIST.routdescr, "EACH_COMPONENT_IN_LIST @warmelt-moremacro.melt:39",  MELT_ROUTDESCR_LEN - 1);
  meltcdat->drout_3__EACH_COMPONENT_IN_LIST.nbval = 14;
 MELT_ROUTINE_SET_ROUTCODE(&meltcdat->drout_3__EACH_COMPONENT_IN_LIST, meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST);
 
 /*iniclos dclo_4__EACH_COMPONENT_IN_LIST*/
 /*_._VALCLO___V25*/ meltfptr[24] = (melt_ptr_t) &meltcdat->dclo_4__EACH_COMPONENT_IN_LIST;
  meltcdat->dclo_4__EACH_COMPONENT_IN_LIST.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MACRO_CLOSURE))));
  meltcdat->dclo_4__EACH_COMPONENT_IN_LIST.nbval = 0;
 
 
/*iniobj dsym_5__MELT_DEBUG_FUN*/
/*uniqueobj*/ if (!/*_._VALDATA___V26*/ meltfptr[25]) 
/*_._VALDATA___V26*/ meltfptr[25] = (melt_ptr_t )&meltcdat->dsym_5__MELT_DEBUG_FUN;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_5__MELT_DEBUG_FUN", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_5__MELT_DEBUG_FUN", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_5__MELT_DEBUG_FUN.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_5__MELT_DEBUG_FUN.obj_hash = 972123377;
 meltcdat->dsym_5__MELT_DEBUG_FUN.obj_len = 3;

 
/*inistring dstr_6__MELT_DEBUG_FUN*/
 /*_._VALSTR___V27*/ meltfptr[26] = (melt_ptr_t) &meltcdat->dstr_6__MELT_DEBUG_FUN;
  meltcdat->dstr_6__MELT_DEBUG_FUN.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_6__MELT_DEBUG_FUN.val, "MELT_DEBUG_FUN", sizeof (meltcdat->dstr_6__MELT_DEBUG_FUN.val)-1);
 meltcdat->dstr_6__MELT_DEBUG_FUN.val[14] = (char)0 ; 
 meltcdat->dstr_6__MELT_DEBUG_FUN.slen = 14 ; 
 
 
/*iniobj dsym_7__DEBUG_LESS*/
/*uniqueobj*/ if (!/*_._VALDATA___V28*/ meltfptr[27]) 
/*_._VALDATA___V28*/ meltfptr[27] = (melt_ptr_t )&meltcdat->dsym_7__DEBUG_LESS;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_7__DEBUG_LESS", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_7__DEBUG_LESS", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_7__DEBUG_LESS.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_7__DEBUG_LESS.obj_hash = 977096719;
 meltcdat->dsym_7__DEBUG_LESS.obj_len = 3;

 
/*inistring dstr_8__DEBUG_LESS*/
 /*_._VALSTR___V29*/ meltfptr[28] = (melt_ptr_t) &meltcdat->dstr_8__DEBUG_LESS;
  meltcdat->dstr_8__DEBUG_LESS.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_8__DEBUG_LESS.val, "DEBUG_LESS", sizeof (meltcdat->dstr_8__DEBUG_LESS.val)-1);
 meltcdat->dstr_8__DEBUG_LESS.val[10] = (char)0 ; 
 meltcdat->dstr_8__DEBUG_LESS.slen = 10 ; 
 
 
/*iniobj dsym_9__CLASS_LOCATED*/
/*uniqueobj*/ if (!/*_._VALDATA___V30*/ meltfptr[29]) 
/*_._VALDATA___V30*/ meltfptr[29] = (melt_ptr_t )&meltcdat->dsym_9__CLASS_LOCATED;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_9__CLASS_LOCATED", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_9__CLASS_LOCATED", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_9__CLASS_LOCATED.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_9__CLASS_LOCATED.obj_hash = 979003;
 meltcdat->dsym_9__CLASS_LOCATED.obj_len = 3;

 
/*inistring dstr_10__CLASS_LOCATED*/
 /*_._VALSTR___V31*/ meltfptr[30] = (melt_ptr_t) &meltcdat->dstr_10__CLASS_LOCATED;
  meltcdat->dstr_10__CLASS_LOCATED.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_10__CLASS_LOCATED.val, "CLASS_LOCATED", sizeof (meltcdat->dstr_10__CLASS_LOCATED.val)-1);
 meltcdat->dstr_10__CLASS_LOCATED.val[13] = (char)0 ; 
 meltcdat->dstr_10__CLASS_LOCATED.slen = 13 ; 
 
 
/*iniobj dsym_11__CLASS_SEXPR*/
/*uniqueobj*/ if (!/*_._VALDATA___V32*/ meltfptr[31]) 
/*_._VALDATA___V32*/ meltfptr[31] = (melt_ptr_t )&meltcdat->dsym_11__CLASS_SEXPR;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_11__CLASS_SEXPR", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_11__CLASS_SEXPR", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_11__CLASS_SEXPR.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_11__CLASS_SEXPR.obj_hash = 53411612;
 meltcdat->dsym_11__CLASS_SEXPR.obj_len = 3;

 
/*inistring dstr_12__CLASS_SEXPR*/
 /*_._VALSTR___V33*/ meltfptr[32] = (melt_ptr_t) &meltcdat->dstr_12__CLASS_SEXPR;
  meltcdat->dstr_12__CLASS_SEXPR.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_12__CLASS_SEXPR.val, "CLASS_SEXPR", sizeof (meltcdat->dstr_12__CLASS_SEXPR.val)-1);
 meltcdat->dstr_12__CLASS_SEXPR.val[11] = (char)0 ; 
 meltcdat->dstr_12__CLASS_SEXPR.slen = 11 ; 
 
 
/*iniobj dsym_13__MELT_ASSERT_FAILURE_FUN*/
/*uniqueobj*/ if (!/*_._VALDATA___V34*/ meltfptr[33]) 
/*_._VALDATA___V34*/ meltfptr[33] = (melt_ptr_t )&meltcdat->dsym_13__MELT_ASSERT_FAILURE_FUN;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_13__MELT_ASSERT_FAILURE_FUN", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_13__MELT_ASSERT_FAILURE_FUN", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_13__MELT_ASSERT_FAILURE_FUN.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_13__MELT_ASSERT_FAILURE_FUN.obj_hash = 215547874;
 meltcdat->dsym_13__MELT_ASSERT_FAILURE_FUN.obj_len = 3;

 
/*inistring dstr_14__MELT_ASSERT_FAILURE_FUN*/
 /*_._VALSTR___V35*/ meltfptr[34] = (melt_ptr_t) &meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN;
  meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.val, "MELT_ASSERT_FAILURE_FUN", sizeof (meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.val)-1);
 meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.val[23] = (char)0 ; 
 meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.slen = 23 ; 
 
 
/*iniobj dsym_15__LIST_TO_MULTIPLE*/
/*uniqueobj*/ if (!/*_._VALDATA___V36*/ meltfptr[35]) 
/*_._VALDATA___V36*/ meltfptr[35] = (melt_ptr_t )&meltcdat->dsym_15__LIST_TO_MULTIPLE;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_15__LIST_TO_MULTIPLE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_15__LIST_TO_MULTIPLE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_15__LIST_TO_MULTIPLE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_15__LIST_TO_MULTIPLE.obj_hash = 987171205;
 meltcdat->dsym_15__LIST_TO_MULTIPLE.obj_len = 3;

 
/*inistring dstr_16__LIST_TO_MULTIPLE*/
 /*_._VALSTR___V37*/ meltfptr[36] = (melt_ptr_t) &meltcdat->dstr_16__LIST_TO_MULTIPLE;
  meltcdat->dstr_16__LIST_TO_MULTIPLE.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_16__LIST_TO_MULTIPLE.val, "LIST_TO_MULTIPLE", sizeof (meltcdat->dstr_16__LIST_TO_MULTIPLE.val)-1);
 meltcdat->dstr_16__LIST_TO_MULTIPLE.val[16] = (char)0 ; 
 meltcdat->dstr_16__LIST_TO_MULTIPLE.slen = 16 ; 
 
 
/*iniobj dsym_17__DISCR_MULTIPLE*/
/*uniqueobj*/ if (!/*_._VALDATA___V38*/ meltfptr[37]) 
/*_._VALDATA___V38*/ meltfptr[37] = (melt_ptr_t )&meltcdat->dsym_17__DISCR_MULTIPLE;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_17__DISCR_MULTIPLE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_17__DISCR_MULTIPLE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_17__DISCR_MULTIPLE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_17__DISCR_MULTIPLE.obj_hash = 91864583;
 meltcdat->dsym_17__DISCR_MULTIPLE.obj_len = 3;

 
/*inistring dstr_18__DISCR_MULTIPLE*/
 /*_._VALSTR___V39*/ meltfptr[38] = (melt_ptr_t) &meltcdat->dstr_18__DISCR_MULTIPLE;
  meltcdat->dstr_18__DISCR_MULTIPLE.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_18__DISCR_MULTIPLE.val, "DISCR_MULTIPLE", sizeof (meltcdat->dstr_18__DISCR_MULTIPLE.val)-1);
 meltcdat->dstr_18__DISCR_MULTIPLE.val[14] = (char)0 ; 
 meltcdat->dstr_18__DISCR_MULTIPLE.slen = 14 ; 
 
 
/*iniobj dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE*/
/*uniqueobj*/ if (!/*_._VALDATA___V40*/ meltfptr[39]) 
/*_._VALDATA___V40*/ meltfptr[39] = (melt_ptr_t )&meltcdat->dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE.obj_hash = 746513982;
 meltcdat->dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE.obj_len = 3;

 
/*inistring dstr_20__EXPAND_TUPLE_SLICE_AS_TU*/
 /*_._VALSTR___V41*/ meltfptr[40] = (melt_ptr_t) &meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU;
  meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.val, "EXPAND_TUPLE_SLICE_AS_TUPLE", sizeof (meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.val)-1);
 meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.val[27] = (char)0 ; 
 meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.slen = 27 ; 
 
 
/*iniobj dsym_21__CLASS_SYMBOL*/
/*uniqueobj*/ if (!/*_._VALDATA___V42*/ meltfptr[41]) 
/*_._VALDATA___V42*/ meltfptr[41] = (melt_ptr_t )&meltcdat->dsym_21__CLASS_SYMBOL;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_21__CLASS_SYMBOL", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_21__CLASS_SYMBOL", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_21__CLASS_SYMBOL.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_21__CLASS_SYMBOL.obj_hash = 53524182;
 meltcdat->dsym_21__CLASS_SYMBOL.obj_len = 3;

 
/*inistring dstr_22__CLASS_SYMBOL*/
 /*_._VALSTR___V43*/ meltfptr[42] = (melt_ptr_t) &meltcdat->dstr_22__CLASS_SYMBOL;
  meltcdat->dstr_22__CLASS_SYMBOL.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_22__CLASS_SYMBOL.val, "CLASS_SYMBOL", sizeof (meltcdat->dstr_22__CLASS_SYMBOL.val)-1);
 meltcdat->dstr_22__CLASS_SYMBOL.val[12] = (char)0 ; 
 meltcdat->dstr_22__CLASS_SYMBOL.slen = 12 ; 
 
 
/*iniobj dsym_23__CLONE_SYMBOL*/
/*uniqueobj*/ if (!/*_._VALDATA___V44*/ meltfptr[43]) 
/*_._VALDATA___V44*/ meltfptr[43] = (melt_ptr_t )&meltcdat->dsym_23__CLONE_SYMBOL;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_23__CLONE_SYMBOL", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_23__CLONE_SYMBOL", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_23__CLONE_SYMBOL.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_23__CLONE_SYMBOL.obj_hash = 213258240;
 meltcdat->dsym_23__CLONE_SYMBOL.obj_len = 3;

 
/*inistring dstr_24__CLONE_SYMBOL*/
 /*_._VALSTR___V45*/ meltfptr[44] = (melt_ptr_t) &meltcdat->dstr_24__CLONE_SYMBOL;
  meltcdat->dstr_24__CLONE_SYMBOL.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_24__CLONE_SYMBOL.val, "CLONE_SYMBOL", sizeof (meltcdat->dstr_24__CLONE_SYMBOL.val)-1);
 meltcdat->dstr_24__CLONE_SYMBOL.val[12] = (char)0 ; 
 meltcdat->dstr_24__CLONE_SYMBOL.slen = 12 ; 
 
 
/*iniobj dsym_25__CURPAIRINLISTSYMB*/
/*uniqueobj*/ if (!/*_._VALDATA___V21*/ meltfptr[20]) 
/*_._VALDATA___V21*/ meltfptr[20] = (melt_ptr_t )&meltcdat->dsym_25__CURPAIRINLISTSYMB;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_25__CURPAIRINLISTSYMB", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_25__CURPAIRINLISTSYMB", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_25__CURPAIRINLISTSYMB.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_25__CURPAIRINLISTSYMB.obj_hash = 580598334;
 meltcdat->dsym_25__CURPAIRINLISTSYMB.obj_len = 3;

 
/*inistring dstr_26__CURPAIRINLISTSYMB*/
 /*_._VALSTR___V22*/ meltfptr[21] = (melt_ptr_t) &meltcdat->dstr_26__CURPAIRINLISTSYMB;
  meltcdat->dstr_26__CURPAIRINLISTSYMB.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_26__CURPAIRINLISTSYMB.val, "CURPAIRINLISTSYMB", sizeof (meltcdat->dstr_26__CURPAIRINLISTSYMB.val)-1);
 meltcdat->dstr_26__CURPAIRINLISTSYMB.val[17] = (char)0 ; 
 meltcdat->dstr_26__CURPAIRINLISTSYMB.slen = 17 ; 
 
 
/*iniobj dsym_27__MELT_MAKE_SEXPR*/
/*uniqueobj*/ if (!/*_._VALDATA___V46*/ meltfptr[45]) 
/*_._VALDATA___V46*/ meltfptr[45] = (melt_ptr_t )&meltcdat->dsym_27__MELT_MAKE_SEXPR;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_27__MELT_MAKE_SEXPR", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_27__MELT_MAKE_SEXPR", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_27__MELT_MAKE_SEXPR.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_27__MELT_MAKE_SEXPR.obj_hash = 383667794;
 meltcdat->dsym_27__MELT_MAKE_SEXPR.obj_len = 3;

 
/*inistring dstr_28__MELT_MAKE_SEXPR*/
 /*_._VALSTR___V47*/ meltfptr[46] = (melt_ptr_t) &meltcdat->dstr_28__MELT_MAKE_SEXPR;
  meltcdat->dstr_28__MELT_MAKE_SEXPR.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_28__MELT_MAKE_SEXPR.val, "MELT_MAKE_SEXPR", sizeof (meltcdat->dstr_28__MELT_MAKE_SEXPR.val)-1);
 meltcdat->dstr_28__MELT_MAKE_SEXPR.val[15] = (char)0 ; 
 meltcdat->dstr_28__MELT_MAKE_SEXPR.slen = 15 ; 
 
 
/*iniobj dsym_29__HOOK_MELT_MAKE_LOCATION*/
/*uniqueobj*/ if (!/*_._VALDATA___V48*/ meltfptr[47]) 
/*_._VALDATA___V48*/ meltfptr[47] = (melt_ptr_t )&meltcdat->dsym_29__HOOK_MELT_MAKE_LOCATION;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_29__HOOK_MELT_MAKE_LOCATION", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_29__HOOK_MELT_MAKE_LOCATION", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_29__HOOK_MELT_MAKE_LOCATION.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_29__HOOK_MELT_MAKE_LOCATION.obj_hash = 753739361;
 meltcdat->dsym_29__HOOK_MELT_MAKE_LOCATION.obj_len = 3;

 
/*inistring dstr_30__HOOK_MELT_MAKE_LOCATION*/
 /*_._VALSTR___V49*/ meltfptr[48] = (melt_ptr_t) &meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION;
  meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.val, "HOOK_MELT_MAKE_LOCATION", sizeof (meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.val)-1);
 meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.val[23] = (char)0 ; 
 meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.slen = 23 ; 
 
 
/*iniobj dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST*/
/*uniqueobj*/ if (!/*_._VALDATA___V23*/ meltfptr[22]) 
/*_._VALDATA___V23*/ meltfptr[22] = (melt_ptr_t )&meltcdat->dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST.obj_hash = 535752972;
 meltcdat->dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST.obj_len = 3;

 
/*inistring dstr_32__FOREACH_PAIR_COMPONENT_I*/
 /*_._VALSTR___V24*/ meltfptr[23] = (melt_ptr_t) &meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I;
  meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.val, "FOREACH_PAIR_COMPONENT_IN_LIST", sizeof (meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.val)-1);
 meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.val[30] = (char)0 ; 
 meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.slen = 30 ; 
 
 
/*iniobj dsym_33__EACH_COMPONENT_IN_LIST*/
/*uniqueobj*/ if (!/*_._VALDATA___V50*/ meltfptr[49]) 
/*_._VALDATA___V50*/ meltfptr[49] = (melt_ptr_t )&meltcdat->dsym_33__EACH_COMPONENT_IN_LIST;
 if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_33__EACH_COMPONENT_IN_LIST", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
  if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_33__EACH_COMPONENT_IN_LIST", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
 meltcdat->dsym_33__EACH_COMPONENT_IN_LIST.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
 meltcdat->dsym_33__EACH_COMPONENT_IN_LIST.obj_hash = 396791142;
 meltcdat->dsym_33__EACH_COMPONENT_IN_LIST.obj_len = 3;

 
/*inistring dstr_34__EACH_COMPONENT_IN_LIST*/
 /*_._VALSTR___V51*/ meltfptr[50] = (melt_ptr_t) &meltcdat->dstr_34__EACH_COMPONENT_IN_LIST;
  meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
 /*small inistring*/ strncpy(meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.val, "EACH_COMPONENT_IN_LIST", sizeof (meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.val)-1);
 meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.val[22] = (char)0 ; 
 meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.slen = 22 ; 
  meltcdat = NULL;
 melt_prohibit_garbcoll = FALSE;

#undef meltfram__
} /*end melt_initialize_module_meltdata_WARMELTmiMOREMACRO*/


#if MELT_HAVE_DEBUG
const char meltmodule_WARMELTmiMOREMACRO__melt_have_debug_enabled[] = "MELT module warmelt-moremacro have debug enabled";
#else /*!MELT_HAVE_DEBUG*/
const char meltmodule_WARMELTmiMOREMACRO__melt_have_debug_disabled[] = "MELT module warmelt-moremacro have debug disabled";
#endif /*MELT_HAVE_DEBUG*/


void* melt_start_this_module (void* modargp_) {

 char meltpredefinited[MELTGLOB__LASTGLOB+8];
 
#if MELT_HAVE_DEBUG
 static long melt_call_counter__;
 long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using LAMBDA_ @warmelt-outobj.melt:2189

/* start of frame for routine melt_start_this_module fromline 1719 */

/*startiniframdecl*/

/* end of frame for routine melt_start_this_module fromline 1723 */


 debugeprintf("sizeof Melt_InitialFrame in melt_start_this_module=%d", (int) sizeof(Melt_InitialFrame));
 /* classy initial frame melt_start_this_module fromline 1740*/ Melt_InitialFrame 
    meltfram__ /*declfrastruct fromline 1747*/
/*classyinitroutinearg melt_start_this_module fromline 1786*/
#if ENABLE_CHECKING
 /*classyinitroutfram fromline 1790:*/ (__FILE__, __LINE__)
#endif /*ENABLE_CHECKING fromline1792*/

;
 #define meltframe meltfram__

/**initial routine prologue**/
 /**COMMENT: get previous environment **/;
 
 /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*_._PREVENV___V4*/ meltfptr[3] = /*getprevenv*/ (melt_ptr_t) modargp_;
 /*^comment*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /**COMMENT: compute boxloc **/;
 
 /*^cond*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*cond*/ if (/*_._CONTENV___V2*/ meltfptr[1]) /*then*/ {
 } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 
  /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
  
    /* compile2obj_initproc emitted initialization of environment reference for warmelt-moremacro */
	      if (/*_._PREVENV___V4*/ meltfptr[3]) 
	        {
	          /*_._CONTENV___V2*/ meltfptr[1] = melthookproc_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER 
	                         ((melt_ptr_t) /*_._PREVENV___V4*/ meltfptr[3], "warmelt-moremacro") ;
	        }
	      else /* no prevenv */
	        {
#if MELT_HAS_INITIAL_ENVIRONMENT>0
	      if (!melt_flag_bootstrapping)
	        warning(0, "MELT is not creating a fresh environment reference for warmelt-moremacro without parent env.") ;
#endif /* MELT_HAS_INITIAL_ENVIRONMENT>0 */
	      }
	      ;;
  }
 
 /*^comment*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /**COMMENT: get symbols & keywords for initial procedure **/;
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER*/
 
  if (NULL == /*_._VALDATA___V18*/ meltfptr[17]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER = melthookproc_HOOK_NAMED_SYMBOL ("HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER", (long) MELT_GET);
   /*_._VALDATA___V18*/ meltfptr[17] = (melt_ptr_t) meltsy_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER;
 } /*endgetnamedsym HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:MELT_DEBUG_FUN*/
 
  if (NULL == /*_._VALDATA___V26*/ meltfptr[25]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MELT_DEBUG_FUN without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_MELT_DEBUG_FUN = melthookproc_HOOK_NAMED_SYMBOL ("MELT_DEBUG_FUN", (long) MELT_GET);
   /*_._VALDATA___V26*/ meltfptr[25] = (melt_ptr_t) meltsy_MELT_DEBUG_FUN;
 } /*endgetnamedsym MELT_DEBUG_FUN*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:DEBUG_LESS*/
 
  if (NULL == /*_._VALDATA___V28*/ meltfptr[27]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol DEBUG_LESS without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_DEBUG_LESS = melthookproc_HOOK_NAMED_SYMBOL ("DEBUG_LESS", (long) MELT_GET);
   /*_._VALDATA___V28*/ meltfptr[27] = (melt_ptr_t) meltsy_DEBUG_LESS;
 } /*endgetnamedsym DEBUG_LESS*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:CLASS_LOCATED*/
 
  if (NULL == /*_._VALDATA___V30*/ meltfptr[29]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_LOCATED without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_CLASS_LOCATED = melthookproc_HOOK_NAMED_SYMBOL ("CLASS_LOCATED", (long) MELT_GET);
   /*_._VALDATA___V30*/ meltfptr[29] = (melt_ptr_t) meltsy_CLASS_LOCATED;
 } /*endgetnamedsym CLASS_LOCATED*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:CLASS_SEXPR*/
 
  if (NULL == /*_._VALDATA___V32*/ meltfptr[31]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_SEXPR without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_CLASS_SEXPR = melthookproc_HOOK_NAMED_SYMBOL ("CLASS_SEXPR", (long) MELT_GET);
   /*_._VALDATA___V32*/ meltfptr[31] = (melt_ptr_t) meltsy_CLASS_SEXPR;
 } /*endgetnamedsym CLASS_SEXPR*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:MELT_ASSERT_FAILURE_FUN*/
 
  if (NULL == /*_._VALDATA___V34*/ meltfptr[33]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MELT_ASSERT_FAILURE_FUN without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_MELT_ASSERT_FAILURE_FUN = melthookproc_HOOK_NAMED_SYMBOL ("MELT_ASSERT_FAILURE_FUN", (long) MELT_GET);
   /*_._VALDATA___V34*/ meltfptr[33] = (melt_ptr_t) meltsy_MELT_ASSERT_FAILURE_FUN;
 } /*endgetnamedsym MELT_ASSERT_FAILURE_FUN*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:LIST_TO_MULTIPLE*/
 
  if (NULL == /*_._VALDATA___V36*/ meltfptr[35]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol LIST_TO_MULTIPLE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_LIST_TO_MULTIPLE = melthookproc_HOOK_NAMED_SYMBOL ("LIST_TO_MULTIPLE", (long) MELT_GET);
   /*_._VALDATA___V36*/ meltfptr[35] = (melt_ptr_t) meltsy_LIST_TO_MULTIPLE;
 } /*endgetnamedsym LIST_TO_MULTIPLE*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:DISCR_MULTIPLE*/
 
  if (NULL == /*_._VALDATA___V38*/ meltfptr[37]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol DISCR_MULTIPLE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_DISCR_MULTIPLE = melthookproc_HOOK_NAMED_SYMBOL ("DISCR_MULTIPLE", (long) MELT_GET);
   /*_._VALDATA___V38*/ meltfptr[37] = (melt_ptr_t) meltsy_DISCR_MULTIPLE;
 } /*endgetnamedsym DISCR_MULTIPLE*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:EXPAND_TUPLE_SLICE_AS_TUPLE*/
 
  if (NULL == /*_._VALDATA___V40*/ meltfptr[39]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol EXPAND_TUPLE_SLICE_AS_TUPLE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_EXPAND_TUPLE_SLICE_AS_TUPLE = melthookproc_HOOK_NAMED_SYMBOL ("EXPAND_TUPLE_SLICE_AS_TUPLE", (long) MELT_GET);
   /*_._VALDATA___V40*/ meltfptr[39] = (melt_ptr_t) meltsy_EXPAND_TUPLE_SLICE_AS_TUPLE;
 } /*endgetnamedsym EXPAND_TUPLE_SLICE_AS_TUPLE*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:CLASS_SYMBOL*/
 
  if (NULL == /*_._VALDATA___V42*/ meltfptr[41]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_SYMBOL without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_CLASS_SYMBOL = melthookproc_HOOK_NAMED_SYMBOL ("CLASS_SYMBOL", (long) MELT_GET);
   /*_._VALDATA___V42*/ meltfptr[41] = (melt_ptr_t) meltsy_CLASS_SYMBOL;
 } /*endgetnamedsym CLASS_SYMBOL*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:CLONE_SYMBOL*/
 
  if (NULL == /*_._VALDATA___V44*/ meltfptr[43]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLONE_SYMBOL without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_CLONE_SYMBOL = melthookproc_HOOK_NAMED_SYMBOL ("CLONE_SYMBOL", (long) MELT_GET);
   /*_._VALDATA___V44*/ meltfptr[43] = (melt_ptr_t) meltsy_CLONE_SYMBOL;
 } /*endgetnamedsym CLONE_SYMBOL*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:CURPAIRINLISTSYMB*/
 
  if (NULL == /*_._VALDATA___V21*/ meltfptr[20]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CURPAIRINLISTSYMB without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_CURPAIRINLISTSYMB = melthookproc_HOOK_NAMED_SYMBOL ("CURPAIRINLISTSYMB", (long) MELT_GET);
   /*_._VALDATA___V21*/ meltfptr[20] = (melt_ptr_t) meltsy_CURPAIRINLISTSYMB;
 } /*endgetnamedsym CURPAIRINLISTSYMB*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:MELT_MAKE_SEXPR*/
 
  if (NULL == /*_._VALDATA___V46*/ meltfptr[45]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MELT_MAKE_SEXPR without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_MELT_MAKE_SEXPR = melthookproc_HOOK_NAMED_SYMBOL ("MELT_MAKE_SEXPR", (long) MELT_GET);
   /*_._VALDATA___V46*/ meltfptr[45] = (melt_ptr_t) meltsy_MELT_MAKE_SEXPR;
 } /*endgetnamedsym MELT_MAKE_SEXPR*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:HOOK_MELT_MAKE_LOCATION*/
 
  if (NULL == /*_._VALDATA___V48*/ meltfptr[47]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol HOOK_MELT_MAKE_LOCATION without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_HOOK_MELT_MAKE_LOCATION = melthookproc_HOOK_NAMED_SYMBOL ("HOOK_MELT_MAKE_LOCATION", (long) MELT_GET);
   /*_._VALDATA___V48*/ meltfptr[47] = (melt_ptr_t) meltsy_HOOK_MELT_MAKE_LOCATION;
 } /*endgetnamedsym HOOK_MELT_MAKE_LOCATION*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:FOREACH_PAIR_COMPONENT_IN_LIST*/
 
  if (NULL == /*_._VALDATA___V23*/ meltfptr[22]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol FOREACH_PAIR_COMPONENT_IN_LIST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_FOREACH_PAIR_COMPONENT_IN_LIST = melthookproc_HOOK_NAMED_SYMBOL ("FOREACH_PAIR_COMPONENT_IN_LIST", (long) MELT_GET);
   /*_._VALDATA___V23*/ meltfptr[22] = (melt_ptr_t) meltsy_FOREACH_PAIR_COMPONENT_IN_LIST;
 } /*endgetnamedsym FOREACH_PAIR_COMPONENT_IN_LIST*/
 
 
 /*^getnamedsymbol*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /*getnamedsym:EACH_COMPONENT_IN_LIST*/
 
  if (NULL == /*_._VALDATA___V50*/ meltfptr[49]) {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol EACH_COMPONENT_IN_LIST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
    melt_ptr_t meltsy_EACH_COMPONENT_IN_LIST = melthookproc_HOOK_NAMED_SYMBOL ("EACH_COMPONENT_IN_LIST", (long) MELT_GET);
   /*_._VALDATA___V50*/ meltfptr[49] = (melt_ptr_t) meltsy_EACH_COMPONENT_IN_LIST;
 } /*endgetnamedsym EACH_COMPONENT_IN_LIST*/
 
 
 /*^comment*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 /**COMMENT: before getting imported values thru hook **/;
 
 MELT_LOCATION("warmelt-moremacro.melt:3:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 3   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V5*/ meltfptr[4] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 3   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 MELT_LOCATION("warmelt-moremacro.melt:45:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "MELT_DEBUG_FUN",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING
#line 45
#endif
 /*_.DEBUG_LESS__V7*/ meltfptr[6] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 45   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "DEBUG_LESS",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 MELT_LOCATION("warmelt-moremacro.melt:47:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 47   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.CLASS_LOCATED__V8*/ meltfptr[7] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 47   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "CLASS_LOCATED",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 MELT_LOCATION("warmelt-moremacro.melt:48:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 48   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.CLASS_SEXPR__V9*/ meltfptr[8] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 48   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "CLASS_SEXPR",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 MELT_LOCATION("warmelt-moremacro.melt:51:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 51   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "MELT_ASSERT_FAILURE_FUN",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 MELT_LOCATION("warmelt-moremacro.melt:53:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 53   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.LIST_TO_MULTIPLE__V11*/ meltfptr[10] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 53   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "LIST_TO_MULTIPLE",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING
#line 53
#endif
 /*_.DISCR_MULTIPLE__V12*/ meltfptr[11] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 53   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "DISCR_MULTIPLE",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 MELT_LOCATION("warmelt-moremacro.melt:80:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.EXPAND_TUPLE_SLICE_AS_TUPLE__V13*/ meltfptr[12] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 80   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "EXPAND_TUPLE_SLICE_AS_TUPLE",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 MELT_LOCATION("warmelt-moremacro.melt:87:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 87   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.CLASS_SYMBOL__V14*/ meltfptr[13] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 87   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "CLASS_SYMBOL",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 MELT_LOCATION("warmelt-moremacro.melt:91:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 91   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.CLONE_SYMBOL__V15*/ meltfptr[14] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 91   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "CLONE_SYMBOL",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 MELT_LOCATION("warmelt-moremacro.melt:93:/ compute");
#ifndef MELTGCC_NOLINENUMBERING
#line 93   "warmelt-moremacro.melt" /**::compute::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*_.MELT_MAKE_SEXPR__V16*/ meltfptr[15] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 93   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "MELT_MAKE_SEXPR",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING
#line 93
#endif
 /*_.HOOK_MELT_MAKE_LOCATION__V17*/ meltfptr[16] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 93   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "HOOK_MELT_MAKE_LOCATION",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
 /**COMMENT: after getting imported values **/;
 
 /**initial routine cdata initializer**/

 memset(meltpredefinited, 0, sizeof(meltpredefinited));
 melt_initialize_module_meltdata_WARMELTmiMOREMACRO (&meltfram__, meltpredefinited);
 /**initial routine body**/

 debugeprintf (" in initialization of warmelt-moremacro thru 1 chunk routines with frame @%p of %d bytes",
 /*fromline 2274*/ (void*)&meltfram__, (int) sizeof(meltfram__));
 melt_debuggc_eprintf ("initialization of warmelt-moremacro thru 1 chunks, frame @%p of %d bytes",
   (void*)&meltfram__, (int) sizeof(meltfram__));
 
 meltmod__WARMELTmiMOREMACRO__initialmeltchunk_0 (&meltfram__, meltpredefinited);;
 goto meltlabend_rout;
meltlabend_rout:;
{ /* return out of initial fromline 2306 */ melt_ptr_t retval = /*_._RETINIT___V1*/ meltfptr[0];
 debugeprintf ("returning @%p from initial routine of warmelt-moremacro", (void*)retval); /*fromline 2314*/
 return retval;}
 #undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR

} /* end fromline 2327 */

void meltmod__WARMELTmiMOREMACRO__initialmeltchunk_0 (meltinitial_frame_t* meltframptr__, char meltpredefinited[]) {
#define meltfram__ (*meltframptr__)
#undef meltcallcount
#define meltcallcount 0L
(void) meltpredefinited;
/**COMMENT: start of oinibody **/;
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V18*/ meltfptr[17])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V18*/ meltfptr[17]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V18*/ meltfptr[17]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V19*/ meltfptr[18]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V18*/ meltfptr[17]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V18*/ meltfptr[17], "new static instance");
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#0", NULL != (/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[0] = (melt_ptr_t)(/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#1", NULL != (/*_.DEBUG_LESS__V7*/ meltfptr[6]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[1] = (melt_ptr_t)(/*_.DEBUG_LESS__V7*/ meltfptr[6]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#2", NULL != (/*_.CLASS_LOCATED__V8*/ meltfptr[7]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[2] = (melt_ptr_t)(/*_.CLASS_LOCATED__V8*/ meltfptr[7]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#3", NULL != (/*_.CLASS_SEXPR__V9*/ meltfptr[8]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[3] = (melt_ptr_t)(/*_.CLASS_SEXPR__V9*/ meltfptr[8]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#4", NULL != (/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[4] = (melt_ptr_t)(/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#5", NULL != (/*_.LIST_TO_MULTIPLE__V11*/ meltfptr[10]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[5] = (melt_ptr_t)(/*_.LIST_TO_MULTIPLE__V11*/ meltfptr[10]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#6", NULL != (/*_.DISCR_MULTIPLE__V12*/ meltfptr[11]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[6] = (melt_ptr_t)(/*_.DISCR_MULTIPLE__V12*/ meltfptr[11]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#7", NULL != (/*_.EXPAND_TUPLE_SLICE_AS_TUPLE__V13*/ meltfptr[12]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[7] = (melt_ptr_t)(/*_.EXPAND_TUPLE_SLICE_AS_TUPLE__V13*/ meltfptr[12]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#8", NULL != (/*_.CLASS_SYMBOL__V14*/ meltfptr[13]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[8] = (melt_ptr_t)(/*_.CLASS_SYMBOL__V14*/ meltfptr[13]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#9", NULL != (/*_.CLONE_SYMBOL__V15*/ meltfptr[14]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[9] = (melt_ptr_t)(/*_.CLONE_SYMBOL__V15*/ meltfptr[14]);
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_25__CURPAIRINLISTSYMB @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V21*/ meltfptr[20])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_25__CURPAIRINLISTSYMB @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V21*/ meltfptr[20]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V21*/ meltfptr[20]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V22*/ meltfptr[21]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V21*/ meltfptr[20]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V21*/ meltfptr[20], "new static instance");
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#10", NULL != (/*_._VALDATA___V21*/ meltfptr[20]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[10] = (melt_ptr_t)(/*_._VALDATA___V21*/ meltfptr[20]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#11", NULL != (/*_.MELT_MAKE_SEXPR__V16*/ meltfptr[15]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[11] = (melt_ptr_t)(/*_.MELT_MAKE_SEXPR__V16*/ meltfptr[15]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#12", NULL != (/*_.HOOK_MELT_MAKE_LOCATION__V17*/ meltfptr[16]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[12] = (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V17*/ meltfptr[16]);
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V23*/ meltfptr[22])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V23*/ meltfptr[22]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V23*/ meltfptr[22]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V24*/ meltfptr[23]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V23*/ meltfptr[22]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V23*/ meltfptr[22], "new static instance");
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^putroutconst*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*putroutconst*/
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#13", NULL != (/*_._VALDATA___V23*/ meltfptr[22]));
 ((meltroutine_ptr_t)/*_._VALROUT___V20*/ meltfptr[19])->tabval[13] = (melt_ptr_t)(/*_._VALDATA___V23*/ meltfptr[22]);
 
 MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 39   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^touch*/
#ifndef MELTGCC_NOLINENUMBERING
#line 39
#endif
 /*touch:_IROUTVAL_*/
 meltgc_touch(/*_._VALROUT___V20*/ meltfptr[19]);
 
 /*putclosurout#1*/
 melt_assertmsg("putclosrout#1 checkclo", melt_magic_discr((melt_ptr_t)(/*_._VALCLO___V25*/ meltfptr[24])) == MELTOBMAG_CLOSURE);
 melt_assertmsg("putclosrout#1 checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V20*/ meltfptr[19])) == MELTOBMAG_ROUTINE);
 ((meltclosure_ptr_t)/*_._VALCLO___V25*/ meltfptr[24])->rout = (meltroutine_ptr_t) (/*_._VALROUT___V20*/ meltfptr[19]);
 
 /*touch:_DATACLOSURE_*/
 meltgc_touch(/*_._VALCLO___V25*/ meltfptr[24]);
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_5__MELT_DEBUG_FUN @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V26*/ meltfptr[25])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_5__MELT_DEBUG_FUN @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V26*/ meltfptr[25]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V26*/ meltfptr[25]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V27*/ meltfptr[26]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V26*/ meltfptr[25]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V26*/ meltfptr[25], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_7__DEBUG_LESS @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V28*/ meltfptr[27])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_7__DEBUG_LESS @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V28*/ meltfptr[27]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V28*/ meltfptr[27]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V29*/ meltfptr[28]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V28*/ meltfptr[27]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V28*/ meltfptr[27], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_9__CLASS_LOCATED @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V30*/ meltfptr[29])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_9__CLASS_LOCATED @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V30*/ meltfptr[29]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V30*/ meltfptr[29]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V31*/ meltfptr[30]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V30*/ meltfptr[29]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V30*/ meltfptr[29], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_11__CLASS_SEXPR @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V32*/ meltfptr[31])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_11__CLASS_SEXPR @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V32*/ meltfptr[31]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V32*/ meltfptr[31]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V33*/ meltfptr[32]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V32*/ meltfptr[31]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V32*/ meltfptr[31], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_13__MELT_ASSERT_FAILURE_FUN @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_13__MELT_ASSERT_FAILURE_FUN @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V34*/ meltfptr[33]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V34*/ meltfptr[33]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V35*/ meltfptr[34]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V34*/ meltfptr[33]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V34*/ meltfptr[33], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_15__LIST_TO_MULTIPLE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_15__LIST_TO_MULTIPLE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V36*/ meltfptr[35]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V36*/ meltfptr[35]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V37*/ meltfptr[36]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V36*/ meltfptr[35]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V36*/ meltfptr[35], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_17__DISCR_MULTIPLE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V38*/ meltfptr[37])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_17__DISCR_MULTIPLE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V38*/ meltfptr[37]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V38*/ meltfptr[37]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V39*/ meltfptr[38]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V38*/ meltfptr[37]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V38*/ meltfptr[37], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V40*/ meltfptr[39])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V40*/ meltfptr[39]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V40*/ meltfptr[39]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V41*/ meltfptr[40]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V40*/ meltfptr[39]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V40*/ meltfptr[39], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_21__CLASS_SYMBOL @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_21__CLASS_SYMBOL @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V42*/ meltfptr[41]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V42*/ meltfptr[41]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V43*/ meltfptr[42]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V42*/ meltfptr[41]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V42*/ meltfptr[41], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_23__CLONE_SYMBOL @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V44*/ meltfptr[43])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_23__CLONE_SYMBOL @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V44*/ meltfptr[43]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V44*/ meltfptr[43]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V45*/ meltfptr[44]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V44*/ meltfptr[43]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V44*/ meltfptr[43], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_27__MELT_MAKE_SEXPR @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V46*/ meltfptr[45])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_27__MELT_MAKE_SEXPR @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V46*/ meltfptr[45]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V46*/ meltfptr[45]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V47*/ meltfptr[46]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V46*/ meltfptr[45]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V46*/ meltfptr[45], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_29__HOOK_MELT_MAKE_LOCATION @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V48*/ meltfptr[47])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_29__HOOK_MELT_MAKE_LOCATION @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V48*/ meltfptr[47]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V48*/ meltfptr[47]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V49*/ meltfptr[48]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V48*/ meltfptr[47]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V48*/ meltfptr[47], "new static instance");
 
 /*putslot*/
 melt_assertmsg("checkobj putslot  ~dsym_33__EACH_COMPONENT_IN_LIST @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V50*/ meltfptr[49])) == MELTOBMAG_OBJECT);
 
 melt_assertmsg("checkoff putslot  ~dsym_33__EACH_COMPONENT_IN_LIST @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V50*/ meltfptr[49]))));
 ((meltobject_ptr_t)(/*_._VALDATA___V50*/ meltfptr[49]))->obj_vartab[1] = (melt_ptr_t)(
  /*_._VALSTR___V51*/ meltfptr[50]);
 
 /*touch:_VALUEDATA_*/
 meltgc_touch(/*_._VALDATA___V50*/ meltfptr[49]);
 
 
 melt_dbgtrace_written_object (/*_._VALDATA___V50*/ meltfptr[49], "new static instance");
 
 /**COMMENT: before toplevel body **/;
 
 MELT_LOCATION("warmelt-moremacro.melt:3:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 3   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 3
#endif
 /*anyblock*/{
  
  /*^cond*/
#ifndef MELTGCC_NOLINENUMBERING
#line 3
#endif
  /*cond*/ if (
#ifndef MELTGCC_NOLINENUMBERING
#line 3   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*quasi.cur.mod.env.ref cur.mod.env.cont : at very start*/
 /* check quasi.cur.mod.env.ref 767622405 */ 
                  (/*_._CONTENV___V2*/ meltfptr[1])
		  ) /*then*/ {
  /*^cond.then*/
#ifndef MELTGCC_NOLINENUMBERING
#line 3
#endif
  /*_.OR___V52*/ meltfptr[51] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 3   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*quasi.cur.mod.env.ref cur.mod.env.cont : at very start*/
 /* check quasi.cur.mod.env.ref 767622405 */ 
                  (/*_._CONTENV___V2*/ meltfptr[1])
		  ;;
  } else {MELT_LOCATION("warmelt-moremacro.melt:3:/ cond.else");
#ifndef MELTGCC_NOLINENUMBERING
#line 3   "warmelt-moremacro.melt" /**::cond.else::**/
#endif /*MELTGCC_NOLINENUMBERING*/
  
   /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 3
#endif
   /*anyblock*/{
    
    /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V53*/ meltfptr[52] = 
#ifndef MELTGCC_NOLINENUMBERING
#line 3   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
      /*hookcall*/ melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER ((melt_ptr_t) /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V5*/ meltfptr[4], /*_._PREVENV___V4*/ meltfptr[3],  "warmelt-moremacro") ;;
    /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
    /*_.OR___V52*/ meltfptr[51] = /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V53*/ meltfptr[52];;
    /*epilog*/
    
    MELT_LOCATION("warmelt-moremacro.melt:3:/ clear");
#ifndef MELTGCC_NOLINENUMBERING
#line 3   "warmelt-moremacro.melt" /**::clear::**/
#endif /*MELTGCC_NOLINENUMBERING*/
    /*clear*/ /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V53*/ meltfptr[52] = 0 ;}
   ;
   }
  ;
  /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 3
#endif
  /*com.block:upd.cur.mod.env.cont : at very start*/{
   }/*com.end block:upd.cur.mod.env.cont : at very start*/
  ;
  /*epilog*/
  
  /*^clear*/
#ifndef MELTGCC_NOLINENUMBERING
#line 3
#endif
  /*clear*/ /*_.OR___V52*/ meltfptr[51] = 0 ;}
 
 MELT_LOCATION("warmelt-moremacro.melt:107:/ initchunk");
#ifndef MELTGCC_NOLINENUMBERING
#line 107   "warmelt-moremacro.melt" /**::initchunk::**/
#endif /*MELTGCC_NOLINENUMBERING*/
 /*^block*/
#ifndef MELTGCC_NOLINENUMBERING
#line 107
#endif
 /*anyblock*/{
  
  
  {
   /*hookcall*/ melthook_HOOK_MACRO_EXPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_MACRO_EXPORTER))), /*_._VALDATA___V50*/ meltfptr[49], /*_._VALCLO___V25*/ meltfptr[24], 
#ifndef MELTGCC_NOLINENUMBERING
#line 107   "warmelt-moremacro.melt" /**::expr::**/
#endif /*MELTGCC_NOLINENUMBERING*/
   /*quasi.cur.mod.env.ref norm.exp.val : EACH_COMPONENT_IN_LIST*/
 /* check quasi.cur.mod.env.ref 930230801 */ 
                  (/*_._CONTENV___V2*/ meltfptr[1])
		  ) ;}
  ;
  /*epilog*/
  }
 
 /**COMMENT: after toplevel body **/;
 
 /**COMMENT: compute boxloc again **/;
 
 /*cond*/ if (/*_._CONTENV___V2*/ meltfptr[1]) /*then*/ {
 } else {/*^cond.else*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
 
  /*^compute*/
#ifndef MELTGCC_NOLINENUMBERING

#endif
  
    /* compile2obj_initproc emitted initialization of environment reference for warmelt-moremacro */
	      if (/*_._PREVENV___V4*/ meltfptr[3]) 
	        {
	          /*_._CONTENV___V2*/ meltfptr[1] = melthookproc_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER 
	                         ((melt_ptr_t) /*_._PREVENV___V4*/ meltfptr[3], "warmelt-moremacro") ;
	        }
	      else /* no prevenv */
	        {
#if MELT_HAS_INITIAL_ENVIRONMENT>0
	      if (!melt_flag_bootstrapping)
	        warning(0, "MELT is not creating a fresh environment reference for warmelt-moremacro without parent env.") ;
#endif /* MELT_HAS_INITIAL_ENVIRONMENT>0 */
	      }
	      ;;
  }
 
 /**COMMENT: intern symbols **/;
 
 /*internsym:HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V18*/ meltfptr[17]));
 
 /*internsym:MELT_DEBUG_FUN*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V26*/ meltfptr[25]));
 
 /*internsym:DEBUG_LESS*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V28*/ meltfptr[27]));
 
 /*internsym:CLASS_LOCATED*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V30*/ meltfptr[29]));
 
 /*internsym:CLASS_SEXPR*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V32*/ meltfptr[31]));
 
 /*internsym:MELT_ASSERT_FAILURE_FUN*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V34*/ meltfptr[33]));
 
 /*internsym:LIST_TO_MULTIPLE*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V36*/ meltfptr[35]));
 
 /*internsym:DISCR_MULTIPLE*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V38*/ meltfptr[37]));
 
 /*internsym:EXPAND_TUPLE_SLICE_AS_TUPLE*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V40*/ meltfptr[39]));
 
 /*internsym:CLASS_SYMBOL*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V42*/ meltfptr[41]));
 
 /*internsym:CLONE_SYMBOL*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V44*/ meltfptr[43]));
 
 /*internsym:CURPAIRINLISTSYMB*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V21*/ meltfptr[20]));
 
 /*internsym:MELT_MAKE_SEXPR*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V46*/ meltfptr[45]));
 
 /*internsym:HOOK_MELT_MAKE_LOCATION*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V48*/ meltfptr[47]));
 
 /*internsym:FOREACH_PAIR_COMPONENT_IN_LIST*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V23*/ meltfptr[22]));
 
 /*internsym:EACH_COMPONENT_IN_LIST*/
 (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V50*/ meltfptr[49]));
 
 /**COMMENT: set retinit from boxloc **/;
 
 /*_._RETINIT___V1*/ meltfptr[0] = 
  /* finalsetretinit */ melt_reference_value((melt_ptr_t)(/*_._CONTENV___V2*/ meltfptr[1]));
 /**COMMENT: end the initproc **/;
 
 
#undef meltfram__
} /*end of meltmod__WARMELTmiMOREMACRO__initialmeltchunk_0*/




/* exported 0 field offsets */

/* exported 0 class lengths */



/**** end of warmelt-moremacro ****/
