/* GCC MELT GENERATED C++ FILE warmelt-moremacro.cc - DO NOT EDIT - see http://gcc-melt.org/ */

/***+ warmelt-moremacro without options +***/
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-moremacro declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-moremacro ** DO NOT EDIT ; see gcc-melt.org ****/

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


/** ordinary MELT module meltbuild-sources/warmelt-moremacro**/
#define MELT_HAS_INITIAL_ENVIRONMENT 1 /*usualmodule*/



class Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166; // forward declaration fromline 6541
typedef Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166 Melt_InitialFrame;
/**** no MELT module variables ****/

/*** 5 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_MACRO_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*declare MELT called hook #2 HOOK_MELT_MAKE_LOCATION **/
MELT_EXTERN melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION (melt_ptr_t melthookdatap, const char* meltinp0_FILENAME, long meltinp1_LINENO);

/*declare MELT called hook #3 HOOK_PATMACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_PATMACRO_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_MACVAL, melt_ptr_t meltinp2_PATVAL, melt_ptr_t meltinp3_CONTENV);

/*declare MELT called hook #4 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*** end of 5 MELT called hook declarations ***/

/*** no extra MELT c-headers ***/




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/


/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166 /*opaqueinitialclassy*/ meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiMOREMACRO__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMOREMACRO__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiMOREMACRO__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMOREMACRO__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMOREMACRO__initialmeltchunk_0 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMOREMACRO__initialmeltchunk_1 (meltinitial_frame_t*, char*);


/**** warmelt-moremacro implementations ****/
/**** MELT GENERATED IMPLEMENTATIONS for warmelt-moremacro ** DO NOT EDIT ; see gcc-melt.org ****/
/*** no extra MELT implementations ***/




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
        : public Melt_CallFrameWithValues<75>
    {
    public: /* fromline 1539*/
        long mcfr_varnum[28];
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
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-moremacro.melt:39:/ getarg");
    /*_.SEXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/ meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
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
        MELT_LOCATION("warmelt-moremacro.melt:45:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:45:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-moremacro.melt:45:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 45;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list macro sexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXP__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n env=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/ meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:45:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:45:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/ meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-moremacro.melt:46:/ quasiblock");


        MELT_LOCATION("warmelt-moremacro.melt:47:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[2])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                    /*_.SLOC__V9*/ meltfptr[5] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SLOC__V9*/ meltfptr[5] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-moremacro.melt:48:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                    /*_.SEXCONT__V10*/ meltfptr[9] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SEXCONT__V10*/ meltfptr[9] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:50:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L3*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:50:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L4*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-moremacro.melt:50:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 50;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list sloc=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V9*/ meltfptr[5];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  " sexcont=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V10*/ meltfptr[9];
                            /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V11*/ meltfptr[10] = /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:50:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L4*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V11*/ meltfptr[10] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:50:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V11*/ meltfptr[10] = 0 ;
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
        MELT_LOCATION("warmelt-moremacro.melt:51:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L5*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.SEXCONT__V10*/ meltfptr[9])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-moremacro.melt:51:/ cond");
            /*cond*/ if (/*_#IS_LIST__L5*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-moremacro.melt:51:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check sexcont";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 51;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V10*/ meltfptr[9];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[10] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:51:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[11] = /*_._IFELSE___V14*/ meltfptr[10];;
            /*epilog*/

            MELT_LOCATION("warmelt-moremacro.melt:51:/ clear");
            /*clear*/ /*_#IS_LIST__L5*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[10] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[11] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-moremacro.melt:52:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:53:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[6]);
            /*_.TCONT__V17*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SEXCONT__V10*/ meltfptr[9]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#TCONTLEN__L6*/ meltfnum[1] =
            (melt_multiple_length((melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:56:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L7*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:56:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L7*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-moremacro.melt:56:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 56;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list tcont=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V17*/ meltfptr[10];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "; tcontlen=";
                            /*^apply.arg*/
                            argtab[6].meltbp_long = /*_#TCONTLEN__L6*/ meltfnum[1];
                            /*_.MELT_DEBUG_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V18*/ meltfptr[17] = /*_.MELT_DEBUG_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:56:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V19*/ meltfptr[18] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V18*/ meltfptr[17] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:56:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L7*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V18*/ meltfptr[17] = 0 ;
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
        MELT_LOCATION("warmelt-moremacro.melt:57:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE__L9*/ meltfnum[7] =
                (melt_magic_discr((melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10])) == MELTOBMAG_MULTIPLE);;
            MELT_LOCATION("warmelt-moremacro.melt:57:/ cond");
            /*cond*/ if (/*_#IS_MULTIPLE__L9*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V21*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-moremacro.melt:57:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check tcont";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 57;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V17*/ meltfptr[10];
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V10*/ meltfptr[9];
                            /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V21*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:57:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V20*/ meltfptr[18] = /*_._IFELSE___V21*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-moremacro.melt:57:/ clear");
            /*clear*/ /*_#IS_MULTIPLE__L9*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V21*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V20*/ meltfptr[18] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:58:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#gtI__L10*/ meltfnum[0] =
                ((/*_#TCONTLEN__L6*/ meltfnum[1]) > (0));;
            MELT_LOCATION("warmelt-moremacro.melt:58:/ cond");
            /*cond*/ if (/*_#gtI__L10*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V24*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-moremacro.melt:58:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check tcontlen";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 58;
                            /*^apply.arg*/
                            argtab[3].meltbp_long = /*_#TCONTLEN__L6*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V17*/ meltfptr[10];
                            /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V24*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:58:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V23*/ meltfptr[21] = /*_._IFELSE___V24*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-moremacro.melt:58:/ clear");
            /*clear*/ /*_#gtI__L10*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V24*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V23*/ meltfptr[21] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#ltI__L11*/ meltfnum[7] =
            ((/*_#TCONTLEN__L6*/ meltfnum[1]) < (4));;
        MELT_LOCATION("warmelt-moremacro.melt:59:/ cond");
        /*cond*/ if (/*_#ltI__L11*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-moremacro.melt:60:/ locexp");
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]), ( "EACH_COMPONENT_IN_LIST <expr-list> <var-comp> too short"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-moremacro.melt:61:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;
                    MELT_LOCATION("warmelt-moremacro.melt:61:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-moremacro.melt:59:/ quasiblock");


                    /*_.PROGN___V28*/ meltfptr[27] = /*_.RETURN___V27*/ meltfptr[17];;
                    /*^compute*/
                    /*_._IF___V26*/ meltfptr[24] = /*_.PROGN___V28*/ meltfptr[27];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-moremacro.melt:59:/ clear");
                    /*clear*/ /*_.RETURN___V27*/ meltfptr[17] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V28*/ meltfptr[27] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V26*/ meltfptr[24] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-moremacro.melt:62:/ quasiblock");


        MELT_LOCATION("warmelt-moremacro.melt:63:/ quasiblock");


        /*_.SE__V31*/ meltfptr[30] =
            (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10]), (1)));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:65:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L12*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:65:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L12*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-moremacro.melt:65:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 65;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list sexplist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SE__V31*/ meltfptr[30];
                            /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V32*/ meltfptr[31] = /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:65:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L13*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V32*/ meltfptr[31] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:65:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L12*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V32*/ meltfptr[31] = 0 ;
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
        /*_.LET___V30*/ meltfptr[27] = /*_.SE__V31*/ meltfptr[30];;

        MELT_LOCATION("warmelt-moremacro.melt:63:/ clear");
        /*clear*/ /*_.SE__V31*/ meltfptr[30] = 0 ;
        /*_.SEXPLIST__V34*/ meltfptr[32] = /*_.LET___V30*/ meltfptr[27];;
        MELT_LOCATION("warmelt-moremacro.melt:67:/ quasiblock");


        /*_.SV__V36*/ meltfptr[30] =
            (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10]), (2)));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:69:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L14*/ meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:69:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L14*/ meltfnum[12]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L15*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-moremacro.melt:69:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 69;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list svarcomp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SV__V36*/ meltfptr[30];
                            /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V37*/ meltfptr[36] = /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:69:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L15*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V37*/ meltfptr[36] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:69:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L14*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V37*/ meltfptr[36] = 0 ;
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
        /*_.LET___V35*/ meltfptr[31] = /*_.SV__V36*/ meltfptr[30];;

        MELT_LOCATION("warmelt-moremacro.melt:67:/ clear");
        /*clear*/ /*_.SV__V36*/ meltfptr[30] = 0 ;
        /*_.SVARCOMP__V39*/ meltfptr[37] = /*_.LET___V35*/ meltfptr[31];;
        MELT_LOCATION("warmelt-moremacro.melt:71:/ quasiblock");



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
            /*_.ME__V41*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SEXPLIST__V34*/ meltfptr[32]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:73:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L16*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:73:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L16*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-moremacro.melt:73:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 73;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list mlistexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ME__V41*/ meltfptr[30];
                            /*_.MELT_DEBUG_FUN__V43*/ meltfptr[42] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V42*/ meltfptr[41] = /*_.MELT_DEBUG_FUN__V43*/ meltfptr[42];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:73:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L17*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V43*/ meltfptr[42] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V42*/ meltfptr[41] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:73:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L16*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V42*/ meltfptr[41] = 0 ;
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
        /*_.LET___V40*/ meltfptr[36] = /*_.ME__V41*/ meltfptr[30];;

        MELT_LOCATION("warmelt-moremacro.melt:71:/ clear");
        /*clear*/ /*_.ME__V41*/ meltfptr[30] = 0 ;
        /*_.MLISTEXP__V44*/ meltfptr[42] = /*_.LET___V40*/ meltfptr[36];;
        MELT_LOCATION("warmelt-moremacro.melt:75:/ quasiblock");



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
            /*_.MV__V46*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SVARCOMP__V39*/ meltfptr[37]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:77:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L18*/ meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:77:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L18*/ meltfnum[12]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L19*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-moremacro.melt:77:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L19*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 77;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list mvarcomp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MV__V46*/ meltfptr[30];
                            /*_.MELT_DEBUG_FUN__V48*/ meltfptr[47] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V47*/ meltfptr[46] = /*_.MELT_DEBUG_FUN__V48*/ meltfptr[47];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:77:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L19*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V48*/ meltfptr[47] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V47*/ meltfptr[46] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:77:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L18*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V47*/ meltfptr[46] = 0 ;
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
        /*_.LET___V45*/ meltfptr[41] = /*_.MV__V46*/ meltfptr[30];;

        MELT_LOCATION("warmelt-moremacro.melt:75:/ clear");
        /*clear*/ /*_.MV__V46*/ meltfptr[30] = 0 ;
        /*_.MVARCOMP__V49*/ meltfptr[47] = /*_.LET___V45*/ meltfptr[41];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:80:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L20*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:80:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L20*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L21*/ meltfnum[12] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-moremacro.melt:80:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L21*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 80;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list tcont=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V17*/ meltfptr[10];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. before expand_tuple_slice_as_tuple=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!EXPAND_TUPLE_SLICE_AS_TUPLE*/ meltfrout->tabval[7]);
                            /*_.MELT_DEBUG_FUN__V51*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V50*/ meltfptr[46] = /*_.MELT_DEBUG_FUN__V51*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:80:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L21*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V51*/ meltfptr[30] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V50*/ meltfptr[46] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:80:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L20*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V50*/ meltfptr[46] = 0 ;
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
        MELT_LOCATION("warmelt-moremacro.melt:82:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_long = 3;
            /*^apply.arg*/
            argtab[1].meltbp_long = -1;
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.BE__V53*/ meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_TUPLE_SLICE_AS_TUPLE*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.TCONT__V17*/ meltfptr[10]), (MELTBPARSTR_LONG MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:84:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L22*/ meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:84:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L22*/ meltfnum[12]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L23*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-moremacro.melt:84:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L23*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 84;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list bodyexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BE__V53*/ meltfptr[46];
                            /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V54*/ meltfptr[53] = /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:84:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L23*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V54*/ meltfptr[53] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:84:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L22*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V54*/ meltfptr[53] = 0 ;
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
        /*_.LET___V52*/ meltfptr[30] = /*_.BE__V53*/ meltfptr[46];;

        MELT_LOCATION("warmelt-moremacro.melt:82:/ clear");
        /*clear*/ /*_.BE__V53*/ meltfptr[46] = 0 ;
        MELT_LOCATION("warmelt-moremacro.melt:79:/ quasiblock");


        /*_.BODYEXP__V56*/ meltfptr[54] = /*_.LET___V52*/ meltfptr[30];;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L24*/ meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.MVARCOMP__V49*/ meltfptr[47]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[8])));;
        MELT_LOCATION("warmelt-moremacro.melt:87:/ cond");
        /*cond*/ if (/*_#IS_NOT_A__L24*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-moremacro.melt:88:/ locexp");
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]), ( "EACH_COMPONENT_IN_LIST <expr-list> <var-comp> bad second argument for\
 component variable"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-moremacro.melt:89:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;
                    MELT_LOCATION("warmelt-moremacro.melt:89:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-moremacro.melt:87:/ quasiblock");


                    /*_.PROGN___V59*/ meltfptr[58] = /*_.RETURN___V58*/ meltfptr[46];;
                    /*^compute*/
                    /*_._IF___V57*/ meltfptr[53] = /*_.PROGN___V59*/ meltfptr[58];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-moremacro.melt:87:/ clear");
                    /*clear*/ /*_.RETURN___V58*/ meltfptr[46] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V59*/ meltfptr[58] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V57*/ meltfptr[53] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-moremacro.melt:90:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:91:/ apply");
        /*apply*/
        {
            /*_.CURPAIRINLISTSYMB__V61*/ meltfptr[58] =  melt_apply ((meltclosure_ptr_t)((/*!CLONE_SYMBOL*/ meltfrout->tabval[9])), (melt_ptr_t)((/*!konst_10_CURPAIRINLISTSYMB*/ meltfrout->tabval[10])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-moremacro.melt:92:/ quasiblock");


        /*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[12]),  "warmelt-moremacro.melt", 93) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V64*/ meltfptr[63] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[12]),  "warmelt-moremacro.melt", 94) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:94:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MLISTEXP__V44*/ meltfptr[42];
            /*_.MELT_MAKE_SEXPR__V65*/ meltfptr[64] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V64*/ meltfptr[63]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[12]),  "warmelt-moremacro.melt", 95) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:95:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURPAIRINLISTSYMB__V61*/ meltfptr[58];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MVARCOMP__V49*/ meltfptr[47];
            /*_.MELT_MAKE_SEXPR__V67*/ meltfptr[66] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:93:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_13_FOREACH_PAIR_COMPONENT_IN_LIST*/ meltfrout->tabval[13]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V65*/ meltfptr[64];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V67*/ meltfptr[66];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.BODYEXP__V56*/ meltfptr[54];
            /*_.RESEXP__V68*/ meltfptr[67] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:98:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L25*/ meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:98:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L25*/ meltfnum[12]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L26*/ meltfnum[25] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-moremacro.melt:98:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L26*/ meltfnum[25];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 98;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list resexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESEXP__V68*/ meltfptr[67];
                            /*_.MELT_DEBUG_FUN__V70*/ meltfptr[69] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V69*/ meltfptr[68] = /*_.MELT_DEBUG_FUN__V70*/ meltfptr[69];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:98:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L26*/ meltfnum[25] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V70*/ meltfptr[69] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V69*/ meltfptr[68] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:98:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L25*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V69*/ meltfptr[68] = 0 ;
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
        MELT_LOCATION("warmelt-moremacro.melt:99:/ quasiblock");



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
            /*_.MEXP__V72*/ meltfptr[68] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.RESEXP__V68*/ meltfptr[67]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:101:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.MEXP__V72*/ meltfptr[68]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[2])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.MEXP__V72*/ meltfptr[68])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.MEXP__V72*/ meltfptr[68]), (1), (/*_.SLOC__V9*/ meltfptr[5]), "LOCA_LOCATION");
                    ;
                    /*^touch*/
                    meltgc_touch(/*_.MEXP__V72*/ meltfptr[68]);
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.MEXP__V72*/ meltfptr[68], "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:102:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L27*/ meltfnum[25] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:102:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L27*/ meltfnum[25]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L28*/ meltfnum[12] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-moremacro.melt:102:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L28*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 102;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "each_component_in_list final mexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXP__V72*/ meltfptr[68];
                            /*_.MELT_DEBUG_FUN__V74*/ meltfptr[73] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V73*/ meltfptr[72] = /*_.MELT_DEBUG_FUN__V74*/ meltfptr[73];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:102:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L28*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V74*/ meltfptr[73] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V73*/ meltfptr[72] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:102:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L27*/ meltfnum[25] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V73*/ meltfptr[72] = 0 ;
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
        MELT_LOCATION("warmelt-moremacro.melt:103:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.MEXP__V72*/ meltfptr[68];;

        {
            MELT_LOCATION("warmelt-moremacro.melt:103:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*_.LET___V71*/ meltfptr[69] = /*_.RETURN___V75*/ meltfptr[73];;

        MELT_LOCATION("warmelt-moremacro.melt:99:/ clear");
        /*clear*/ /*_.MEXP__V72*/ meltfptr[68] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V75*/ meltfptr[73] = 0 ;
        /*_.LET___V62*/ meltfptr[61] = /*_.LET___V71*/ meltfptr[69];;

        MELT_LOCATION("warmelt-moremacro.melt:92:/ clear");
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V64*/ meltfptr[63] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V65*/ meltfptr[64] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V67*/ meltfptr[66] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RESEXP__V68*/ meltfptr[67] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V71*/ meltfptr[69] = 0 ;
        /*_.LET___V60*/ meltfptr[46] = /*_.LET___V62*/ meltfptr[61];;

        MELT_LOCATION("warmelt-moremacro.melt:90:/ clear");
        /*clear*/ /*_.CURPAIRINLISTSYMB__V61*/ meltfptr[58] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V62*/ meltfptr[61] = 0 ;
        /*_.LET___V29*/ meltfptr[17] = /*_.LET___V60*/ meltfptr[46];;

        MELT_LOCATION("warmelt-moremacro.melt:62:/ clear");
        /*clear*/ /*_.LET___V30*/ meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SEXPLIST__V34*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V35*/ meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SVARCOMP__V39*/ meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V40*/ meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MLISTEXP__V44*/ meltfptr[42] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V45*/ meltfptr[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MVARCOMP__V49*/ meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V52*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYEXP__V56*/ meltfptr[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L24*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V57*/ meltfptr[53] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V60*/ meltfptr[46] = 0 ;
        /*_.LET___V16*/ meltfptr[14] = /*_.LET___V29*/ meltfptr[17];;

        MELT_LOCATION("warmelt-moremacro.melt:52:/ clear");
        /*clear*/ /*_.TCONT__V17*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#TCONTLEN__L6*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V20*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V23*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#ltI__L11*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V26*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V29*/ meltfptr[17] = 0 ;
        /*_.LET___V8*/ meltfptr[6] = /*_.LET___V16*/ meltfptr[14];;

        MELT_LOCATION("warmelt-moremacro.melt:46:/ clear");
        /*clear*/ /*_.SLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SEXCONT__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/ meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:39:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-moremacro.melt:39:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

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





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION fromline 1719 */

    /** start of frame for meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION of CLASS_PROCROUTINEOBJ from 1522**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION// fromline 1535
        : public Melt_CallFrameWithValues<8>
    {
    public: /* fromline 1539*/
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
        MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION(meltclosure_ptr_t clos) //the closure constructor fromline 1597
            : Melt_CallFrameWithValues<8> (
#if ENABLE_CHECKING /*fromline 1601*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1605*/
                sizeof(MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION), clos) {};
        MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION() //the constructor fromline 1609
            : Melt_CallFrameWithValues<8> (
#if ENABLE_CHECKING /*fromline 1613*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1617*/
                sizeof(MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION)) {};
#if ENABLE_CHECKING /*fromline 1621*/
        MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION(const char*fil, int lin) //the constructor fromline 1623
            : Melt_CallFrameWithValues<8> (fil,lin, sizeof(MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION)) {};
        MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1628
            : Melt_CallFrameWithValues<8> (fil,lin, sizeof(MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION), clos) {};
#endif /* ENABLE_CHECKING fromline 1633*/

    }; // end  class MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION


    /** end of frame for meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION fromline 1664**/

    /* end of frame for routine meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION fromline 1723 */

    /* classy proc frame meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION */ MeltFrame_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION
    meltfram__ /*declfrastruct fromline 1747*/
    /*classyprocarg meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION fromline 1752*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1756*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1760*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("FILTERGCCVERSION", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-moremacro.melt:112:/ getarg");
    /*_.VERSIONSTR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:113:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L1*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VERSIONSTR__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-moremacro.melt:113:/ cond");
            /*cond*/ if (/*_#IS_STRING__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V4*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-moremacro.melt:113:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check versionstr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 113;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VERSIONSTR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V4*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:113:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V5*/ meltfptr[4] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V3*/ meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-moremacro.melt:113:/ clear");
            /*clear*/ /*_#IS_STRING__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V4*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V3*/ meltfptr[2] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-moremacro.melt:114:/ quasiblock");


        /*_#VERSIONLEN__L2*/ meltfnum[0] =
            melt_string_length((melt_ptr_t)(/*_.VERSIONSTR__V2*/ meltfptr[1]));;
        /*^compute*/
        /*_.RES__V7*/ meltfptr[3] = (/*nil*/NULL);;

        {
            MELT_LOCATION("warmelt-moremacro.melt:117:/ locexp");
            if (/*_#VERSIONLEN__L2*/ meltfnum[0]>0 /*FILTERGCC__1*/
                                     && !strncmp (melt_string_str((melt_ptr_t)/*_.VERSIONSTR__V2*/ meltfptr[1]),
                                                  melt_gccversionstr,
                                                  /*_#VERSIONLEN__L2*/ meltfnum[0]))
                /*_.RES__V7*/ meltfptr[3] = /*_.VERSIONSTR__V2*/ meltfptr[1];
            ;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:125:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V7*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-moremacro.melt:125:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*_.LET___V6*/ meltfptr[4] = /*_.RETURN___V8*/ meltfptr[7];;

        MELT_LOCATION("warmelt-moremacro.melt:114:/ clear");
        /*clear*/ /*_#VERSIONLEN__L2*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V7*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V8*/ meltfptr[7] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:112:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V6*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-moremacro.melt:112:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V6*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("FILTERGCCVERSION", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF fromline 1719 */

    /** start of frame for meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF of CLASS_PROCROUTINEOBJ from 1522**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF// fromline 1535
        : public Melt_CallFrameWithValues<60>
    {
    public: /* fromline 1539*/
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
        MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF(meltclosure_ptr_t clos) //the closure constructor fromline 1597
            : Melt_CallFrameWithValues<60> (
#if ENABLE_CHECKING /*fromline 1601*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1605*/
                sizeof(MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF), clos) {};
        MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF() //the constructor fromline 1609
            : Melt_CallFrameWithValues<60> (
#if ENABLE_CHECKING /*fromline 1613*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1617*/
                sizeof(MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF)) {};
#if ENABLE_CHECKING /*fromline 1621*/
        MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF(const char*fil, int lin) //the constructor fromline 1623
            : Melt_CallFrameWithValues<60> (fil,lin, sizeof(MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF)) {};
        MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1628
            : Melt_CallFrameWithValues<60> (fil,lin, sizeof(MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF), clos) {};
#endif /* ENABLE_CHECKING fromline 1633*/

    }; // end  class MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF


    /** end of frame for meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF fromline 1664**/

    /* end of frame for routine meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF fromline 1723 */

    /* classy proc frame meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF */ MeltFrame_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF
    meltfram__ /*declfrastruct fromline 1747*/
    /*classyprocarg meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF fromline 1752*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1756*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1760*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_GCCIF", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-moremacro.melt:127:/ getarg");
    /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/ meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
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
        MELT_LOCATION("warmelt-moremacro.melt:128:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/ meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-moremacro.melt:128:/ cond");
            /*cond*/ if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-moremacro.melt:128:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 128;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/ meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:128:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-moremacro.melt:128:/ clear");
            /*clear*/ /*_#IS_A__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/ meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:129:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-moremacro.melt:129:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-moremacro.melt:129:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 129;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/ meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:129:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/ meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-moremacro.melt:129:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/ meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:130:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L3*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:130:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L3*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-moremacro.melt:130:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 130;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_gccif sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V13*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V12*/ meltfptr[10] = /*_.MELT_DEBUG_FUN__V13*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:130:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L4*/ meltfnum[3] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V13*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V12*/ meltfptr[10] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:130:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L3*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V12*/ meltfptr[10] = 0 ;
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
        MELT_LOCATION("warmelt-moremacro.melt:131:/ quasiblock");


        MELT_LOCATION("warmelt-moremacro.melt:132:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V15*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-moremacro.melt:133:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.SLOC__V16*/ meltfptr[15] = slot;
        };
        ;
        /*_.LIST_FIRST__V17*/ meltfptr[16] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V15*/ meltfptr[10])));;
        /*^compute*/
        /*_.CURPAIR__V18*/ meltfptr[17] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V17*/ meltfptr[16])));;
        /*^compute*/
        /*_.CURIF__V19*/ meltfptr[18] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        /*^compute*/
        /*_.RESTPAIR__V20*/ meltfptr[19] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L5*/ meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.CURIF__V19*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-moremacro.melt:138:/ cond");
        /*cond*/ if (/*_#IS_A__L5*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-moremacro.melt:139:/ quasiblock");


                    /*^cond*/
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURIF__V19*/ meltfptr[18]),
                                                      (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURIF__V19*/ meltfptr[18]) /*=obj*/;
                                melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                /*_.XCURIF__V23*/ meltfptr[22] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.XCURIF__V23*/ meltfptr[22] =  /*reallynil*/ NULL ;;
                        }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-moremacro.melt:141:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L6*/ meltfnum[0] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-moremacro.melt:141:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L6*/ meltfnum[0]) /*then*/
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
                                    MELT_LOCATION("warmelt-moremacro.melt:141:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[6];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 141;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_gccif xcurif=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XCURIF__V23*/ meltfptr[22];
                                        /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V24*/ meltfptr[23] = /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-moremacro.melt:141:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L7*/ meltfnum[6] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V24*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-moremacro.melt:141:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L6*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V24*/ meltfptr[23] = 0 ;
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
                    MELT_LOCATION("warmelt-moremacro.melt:142:/ compute");
                    /*_.CURIF__V19*/ meltfptr[18] = /*_.SETQ___V26*/ meltfptr[24] = /*_.XCURIF__V23*/ meltfptr[22];;
                    /*_.LET___V22*/ meltfptr[21] = /*_.SETQ___V26*/ meltfptr[24];;

                    MELT_LOCATION("warmelt-moremacro.melt:139:/ clear");
                    /*clear*/ /*_.XCURIF__V23*/ meltfptr[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V26*/ meltfptr[24] = 0 ;
                    /*_._IF___V21*/ meltfptr[20] = /*_.LET___V22*/ meltfptr[21];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-moremacro.melt:138:/ clear");
                    /*clear*/ /*_.LET___V22*/ meltfptr[21] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V21*/ meltfptr[20] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-moremacro.melt:144:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L8*/ meltfnum[6] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:144:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L8*/ meltfnum[6]) /*then*/
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
                        MELT_LOCATION("warmelt-moremacro.melt:144:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 144;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_gccif curif";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CURIF__V19*/ meltfptr[18];
                            /*_.MELT_DEBUG_FUN__V28*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V27*/ meltfptr[23] = /*_.MELT_DEBUG_FUN__V28*/ meltfptr[22];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:144:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V28*/ meltfptr[22] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V27*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:144:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L8*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V27*/ meltfptr[23] = 0 ;
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
        /*_#IS_STRING__L10*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURIF__V19*/ meltfptr[18])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-moremacro.melt:146:/ cond");
        /*cond*/ if (/*_#IS_STRING__L10*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-moremacro.melt:147:/ apply");
                    /*apply*/
                    {
                        /*_.FILTERGCCVERSION__V30*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!FILTERGCCVERSION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.CURIF__V19*/ meltfptr[18]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    /*^cond*/
                    /*cond*/ if (/*_.FILTERGCCVERSION__V30*/ meltfptr[21]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-moremacro.melt:148:/ quasiblock");



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
                                    /*_.EXPREST__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_PAIRLIST_AS_TUPLE*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.RESTPAIR__V20*/ meltfptr[19]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-moremacro.melt:150:/ cppif.then");
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
                                    /*_#MELT_NEED_DBG__L11*/ meltfnum[6] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-moremacro.melt:150:/ cond");
                                    /*cond*/ if (/*_#MELT_NEED_DBG__L11*/ meltfnum[6]) /*then*/
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
                                                MELT_LOCATION("warmelt-moremacro.melt:150:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 150;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "mexpand_gccif return exprest";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.EXPREST__V33*/ meltfptr[32];
                                                    /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V34*/ meltfptr[33] = /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-moremacro.melt:150:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L12*/ meltfnum[11] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V34*/ meltfptr[33] =  /*reallynil*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-moremacro.melt:150:/ locexp");
                                        /*void*/(void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L11*/ meltfnum[6] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V34*/ meltfptr[33] = 0 ;
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
                                MELT_LOCATION("warmelt-moremacro.melt:151:/ quasiblock");


                                /*_._RETVAL___V1*/ meltfptr[0] = /*_.EXPREST__V33*/ meltfptr[32];;

                                {
                                    MELT_LOCATION("warmelt-moremacro.melt:151:/ locexp");
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
                                /*finalret*/ goto meltlabend_rout ;
                                /*_.LET___V32*/ meltfptr[23] = /*_.RETURN___V36*/ meltfptr[34];;

                                MELT_LOCATION("warmelt-moremacro.melt:148:/ clear");
                                /*clear*/ /*_.EXPREST__V33*/ meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V36*/ meltfptr[34] = 0 ;
                                /*_._IFELSE___V31*/ meltfptr[22] = /*_.LET___V32*/ meltfptr[23];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-moremacro.melt:147:/ clear");
                                /*clear*/ /*_.LET___V32*/ meltfptr[23] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-moremacro.melt:153:/ cppif.then");
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
                                    /*_#MELT_NEED_DBG__L13*/ meltfnum[11] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-moremacro.melt:153:/ cond");
                                    /*cond*/ if (/*_#MELT_NEED_DBG__L13*/ meltfnum[11]) /*then*/
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
                                                MELT_LOCATION("warmelt-moremacro.melt:153:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[6];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 153;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "mexpand_gccif sexpr gcc version mismatched";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                                                    /*_.MELT_DEBUG_FUN__V38*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V37*/ meltfptr[33] = /*_.MELT_DEBUG_FUN__V38*/ meltfptr[32];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-moremacro.melt:153:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L14*/ meltfnum[6] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V38*/ meltfptr[32] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V37*/ meltfptr[33] =  /*reallynil*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-moremacro.melt:153:/ locexp");
                                        /*void*/(void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L13*/ meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V37*/ meltfptr[33] = 0 ;
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
                                MELT_LOCATION("warmelt-moremacro.melt:154:/ quasiblock");


                                /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-moremacro.melt:154:/ locexp");
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
                                /*finalret*/ goto meltlabend_rout ;
                                MELT_LOCATION("warmelt-moremacro.melt:152:/ quasiblock");


                                /*_.PROGN___V40*/ meltfptr[23] = /*_.RETURN___V39*/ meltfptr[34];;
                                /*^compute*/
                                /*_._IFELSE___V31*/ meltfptr[22] = /*_.PROGN___V40*/ meltfptr[23];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-moremacro.melt:147:/ clear");
                                /*clear*/ /*_.RETURN___V39*/ meltfptr[34] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V40*/ meltfptr[23] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V29*/ meltfptr[24] = /*_._IFELSE___V31*/ meltfptr[22];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-moremacro.melt:146:/ clear");
                    /*clear*/ /*_.FILTERGCCVERSION__V30*/ meltfptr[21] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V31*/ meltfptr[22] = 0 ;
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
                    /*_#IS_LIST__L15*/ meltfnum[6] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CURIF__V19*/ meltfptr[18])) == MELTOBMAG_LIST);;
                    MELT_LOCATION("warmelt-moremacro.melt:155:/ cond");
                    /*cond*/ if (/*_#IS_LIST__L15*/ meltfnum[6]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-moremacro.melt:156:/ quasiblock");


                                /*_.OK__V43*/ meltfptr[34] = (/*nil*/NULL);;
                                /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                {
                                    /* start foreach_pair_component_in_list meltcit1__EACHLIST */
                                    for (/*_.CURPAIR__V44*/ meltfptr[23] = melt_list_first( (melt_ptr_t)/*_.CURIF__V19*/ meltfptr[18]);
                                                            melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V44*/ meltfptr[23]) == MELTOBMAG_PAIR;
                                                            /*_.CURPAIR__V44*/ meltfptr[23] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V44*/ meltfptr[23]))
                                        {
                                            /*_.CURSTR__V45*/ meltfptr[21] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V44*/ meltfptr[23]);



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#IS_STRING__L16*/ meltfnum[11] =
                                                (melt_magic_discr((melt_ptr_t)(/*_.CURSTR__V45*/ meltfptr[21])) == MELTOBMAG_STRING);;
                                            /*^compute*/
                                            /*_#NOT__L17*/ meltfnum[16] =
                                                (!(/*_#IS_STRING__L16*/ meltfnum[11]));;
                                            MELT_LOCATION("warmelt-moremacro.melt:161:/ cond");
                                            /*cond*/ if (/*_#NOT__L17*/ meltfnum[16]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        {
                                                            MELT_LOCATION("warmelt-moremacro.melt:162:/ locexp");
                                                            /* error_plain */ melt_error_str((melt_ptr_t)(/*_.SLOC__V16*/ meltfptr[15]), ( "GCCIF condition not a list of strings"), (melt_ptr_t)0);
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
                                            MELT_LOCATION("warmelt-moremacro.melt:163:/ apply");
                                            /*apply*/
                                            {
                                                /*_.FILTERGCCVERSION__V46*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!FILTERGCCVERSION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.CURSTR__V45*/ meltfptr[21]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*^cond*/
                                            /*cond*/ if (/*_.FILTERGCCVERSION__V46*/ meltfptr[22]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-moremacro.melt:164:/ compute");
                                                        /*_.OK__V43*/ meltfptr[34] = /*_.SETQ___V48*/ meltfptr[47] = (/*!konst_6_TRUE*/ meltfrout->tabval[6]);;
                                                        /*_._IF___V47*/ meltfptr[46] = /*_.SETQ___V48*/ meltfptr[47];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-moremacro.melt:163:/ clear");
                                                        /*clear*/ /*_.SETQ___V48*/ meltfptr[47] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*_._IF___V47*/ meltfptr[46] =  /*reallynil*/ NULL ;;
                                                }
                                            ;
                                        } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
                                    /*_.CURPAIR__V44*/ meltfptr[23] = NULL;
                                    /*_.CURSTR__V45*/ meltfptr[21] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-moremacro.melt:158:/ clear");
                                    /*clear*/ /*_.CURPAIR__V44*/ meltfptr[23] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURSTR__V45*/ meltfptr[21] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IS_STRING__L16*/ meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#NOT__L17*/ meltfnum[16] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.FILTERGCCVERSION__V46*/ meltfptr[22] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V47*/ meltfptr[46] = 0 ;
                                } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-moremacro.melt:166:/ cond");
                                /*cond*/ if (/*_.OK__V43*/ meltfptr[34]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-moremacro.melt:167:/ quasiblock");



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
                                                /*_.EXPREST__V51*/ meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_PAIRLIST_AS_TUPLE*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.RESTPAIR__V20*/ meltfptr[19]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-moremacro.melt:169:/ cppif.then");
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
                                                /*_#MELT_NEED_DBG__L18*/ meltfnum[17] =
                                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                    ;;
                                                MELT_LOCATION("warmelt-moremacro.melt:169:/ cond");
                                                /*cond*/ if (/*_#MELT_NEED_DBG__L18*/ meltfnum[17]) /*then*/
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
                                                            MELT_LOCATION("warmelt-moremacro.melt:169:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[5];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 169;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_cstring =  "mexpand_gccif return exprest multicond";
                                                                /*^apply.arg*/
                                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.EXPREST__V51*/ meltfptr[50];
                                                                /*_.MELT_DEBUG_FUN__V53*/ meltfptr[52] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IF___V52*/ meltfptr[51] = /*_.MELT_DEBUG_FUN__V53*/ meltfptr[52];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-moremacro.melt:169:/ clear");
                                                            /*clear*/ /*_#MELT_CALLCOUNT__L19*/ meltfnum[18] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.MELT_DEBUG_FUN__V53*/ meltfptr[52] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_._IF___V52*/ meltfptr[51] =  /*reallynil*/ NULL ;;
                                                    }
                                                ;

                                                {
                                                    MELT_LOCATION("warmelt-moremacro.melt:169:/ locexp");
                                                    /*void*/(void)0;
                                                }
                                                ;
                                                /*^quasiblock*/


                                                /*epilog*/

                                                /*^clear*/
                                                /*clear*/ /*_#MELT_NEED_DBG__L18*/ meltfnum[17] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IF___V52*/ meltfptr[51] = 0 ;
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
                                            MELT_LOCATION("warmelt-moremacro.melt:170:/ quasiblock");


                                            /*_._RETVAL___V1*/ meltfptr[0] = /*_.EXPREST__V51*/ meltfptr[50];;

                                            {
                                                MELT_LOCATION("warmelt-moremacro.melt:170:/ locexp");
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
                                            /*finalret*/ goto meltlabend_rout ;
                                            /*_.LET___V50*/ meltfptr[49] = /*_.RETURN___V54*/ meltfptr[52];;

                                            MELT_LOCATION("warmelt-moremacro.melt:167:/ clear");
                                            /*clear*/ /*_.EXPREST__V51*/ meltfptr[50] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.RETURN___V54*/ meltfptr[52] = 0 ;
                                            /*_._IFELSE___V49*/ meltfptr[47] = /*_.LET___V50*/ meltfptr[49];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-moremacro.melt:166:/ clear");
                                            /*clear*/ /*_.LET___V50*/ meltfptr[49] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {


#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-moremacro.melt:172:/ cppif.then");
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
                                                /*_#MELT_NEED_DBG__L20*/ meltfnum[18] =
                                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                    ;;
                                                MELT_LOCATION("warmelt-moremacro.melt:172:/ cond");
                                                /*cond*/ if (/*_#MELT_NEED_DBG__L20*/ meltfnum[18]) /*then*/
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
                                                            MELT_LOCATION("warmelt-moremacro.melt:172:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[5];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L21*/ meltfnum[17];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 172;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_cstring =  "mexpand_gccif sexpr gcc version multicond mismatched";
                                                                /*^apply.arg*/
                                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                                                                /*_.MELT_DEBUG_FUN__V56*/ meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IF___V55*/ meltfptr[51] = /*_.MELT_DEBUG_FUN__V56*/ meltfptr[50];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-moremacro.melt:172:/ clear");
                                                            /*clear*/ /*_#MELT_CALLCOUNT__L21*/ meltfnum[17] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.MELT_DEBUG_FUN__V56*/ meltfptr[50] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_._IF___V55*/ meltfptr[51] =  /*reallynil*/ NULL ;;
                                                    }
                                                ;

                                                {
                                                    MELT_LOCATION("warmelt-moremacro.melt:172:/ locexp");
                                                    /*void*/(void)0;
                                                }
                                                ;
                                                /*^quasiblock*/


                                                /*epilog*/

                                                /*^clear*/
                                                /*clear*/ /*_#MELT_NEED_DBG__L20*/ meltfnum[18] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IF___V55*/ meltfptr[51] = 0 ;
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
                                            MELT_LOCATION("warmelt-moremacro.melt:173:/ quasiblock");


                                            /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                            {
                                                MELT_LOCATION("warmelt-moremacro.melt:173:/ locexp");
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
                                            /*finalret*/ goto meltlabend_rout ;
                                            MELT_LOCATION("warmelt-moremacro.melt:171:/ quasiblock");


                                            /*_.PROGN___V58*/ meltfptr[49] = /*_.RETURN___V57*/ meltfptr[52];;
                                            /*^compute*/
                                            /*_._IFELSE___V49*/ meltfptr[47] = /*_.PROGN___V58*/ meltfptr[49];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-moremacro.melt:166:/ clear");
                                            /*clear*/ /*_.RETURN___V57*/ meltfptr[52] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PROGN___V58*/ meltfptr[49] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_.LET___V42*/ meltfptr[33] = /*_._IFELSE___V49*/ meltfptr[47];;

                                MELT_LOCATION("warmelt-moremacro.melt:156:/ clear");
                                /*clear*/ /*_.OK__V43*/ meltfptr[34] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V49*/ meltfptr[47] = 0 ;
                                /*_._IFELSE___V41*/ meltfptr[32] = /*_.LET___V42*/ meltfptr[33];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-moremacro.melt:155:/ clear");
                                /*clear*/ /*_.LET___V42*/ meltfptr[33] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-moremacro.melt:177:/ locexp");
                                    /* error_plain */ melt_error_str((melt_ptr_t)(/*_.SLOC__V16*/ meltfptr[15]), ( "GCCIF bad condition, should be a string or a list of strings"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-moremacro.melt:178:/ quasiblock");


                                /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-moremacro.melt:178:/ locexp");
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
                                /*finalret*/ goto meltlabend_rout ;
                                MELT_LOCATION("warmelt-moremacro.melt:176:/ quasiblock");


                                /*_.PROGN___V60*/ meltfptr[51] = /*_.RETURN___V59*/ meltfptr[50];;
                                /*^compute*/
                                /*_._IFELSE___V41*/ meltfptr[32] = /*_.PROGN___V60*/ meltfptr[51];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-moremacro.melt:155:/ clear");
                                /*clear*/ /*_.RETURN___V59*/ meltfptr[50] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V60*/ meltfptr[51] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V29*/ meltfptr[24] = /*_._IFELSE___V41*/ meltfptr[32];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-moremacro.melt:146:/ clear");
                    /*clear*/ /*_#IS_LIST__L15*/ meltfnum[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V41*/ meltfptr[32] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V14*/ meltfptr[6] = /*_._IFELSE___V29*/ meltfptr[24];;

        MELT_LOCATION("warmelt-moremacro.melt:131:/ clear");
        /*clear*/ /*_.CONT__V15*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SLOC__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURIF__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RESTPAIR__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L5*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L10*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V29*/ meltfptr[24] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:127:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V14*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-moremacro.melt:127:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V6*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V14*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_GCCIF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER fromline 1719 */

    /** start of frame for meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER of CLASS_PROCROUTINEOBJ from 1522**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER// fromline 1535
        : public Melt_CallFrameWithValues<12>
    {
    public: /* fromline 1539*/
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
        MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t clos) //the closure constructor fromline 1597
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1601*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1605*/
                sizeof(MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER), clos) {};
        MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER() //the constructor fromline 1609
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1613*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1617*/
                sizeof(MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER)) {};
#if ENABLE_CHECKING /*fromline 1621*/
        MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER(const char*fil, int lin) //the constructor fromline 1623
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER)) {};
        MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1628
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER), clos) {};
#endif /* ENABLE_CHECKING fromline 1633*/

    }; // end  class MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER


    /** end of frame for meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER fromline 1664**/

    /* end of frame for routine meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER fromline 1723 */

    /* classy proc frame meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER */ MeltFrame_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER
    meltfram__ /*declfrastruct fromline 1747*/
    /*classyprocarg meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER fromline 1752*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1756*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1760*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPANDOBSOLETE_CONTAINER", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-moremacro.melt:191:/ getarg");
    /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/ meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
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
        MELT_LOCATION("warmelt-moremacro.melt:192:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:192:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-moremacro.melt:192:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 192;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpandobsolete_container sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/ meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:192:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:192:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/ meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-moremacro.melt:193:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-moremacro.melt:193:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-moremacro.melt:193:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 193;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:193:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-moremacro.melt:193:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-moremacro.melt:194:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                    /*_.LOCA_LOCATION__V11*/ meltfptr[9] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LOCA_LOCATION__V11*/ meltfptr[9] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-moremacro.melt:194:/ locexp");
            melt_warning_str(0, (melt_ptr_t)(/*_.LOCA_LOCATION__V11*/ meltfptr[9]), ( "obsolete use of CONTAINER in expression; use REFERENCE instead"), (melt_ptr_t)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:196:/ apply");
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
            /*_.MEXPAND_REFERENCE__V12*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MEXPAND_REFERENCE*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:191:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.MEXPAND_REFERENCE__V12*/ meltfptr[5];;

        {
            MELT_LOCATION("warmelt-moremacro.melt:191:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOCA_LOCATION__V11*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MEXPAND_REFERENCE__V12*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPANDOBSOLETE_CONTAINER", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER fromline 1719 */

    /** start of frame for meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER of CLASS_PROCROUTINEOBJ from 1522**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER// fromline 1535
        : public Melt_CallFrameWithValues<11>
    {
    public: /* fromline 1539*/
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
        MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t clos) //the closure constructor fromline 1597
            : Melt_CallFrameWithValues<11> (
#if ENABLE_CHECKING /*fromline 1601*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1605*/
                sizeof(MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER), clos) {};
        MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER() //the constructor fromline 1609
            : Melt_CallFrameWithValues<11> (
#if ENABLE_CHECKING /*fromline 1613*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1617*/
                sizeof(MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER)) {};
#if ENABLE_CHECKING /*fromline 1621*/
        MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER(const char*fil, int lin) //the constructor fromline 1623
            : Melt_CallFrameWithValues<11> (fil,lin, sizeof(MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER)) {};
        MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1628
            : Melt_CallFrameWithValues<11> (fil,lin, sizeof(MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER), clos) {};
#endif /* ENABLE_CHECKING fromline 1633*/

    }; // end  class MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER


    /** end of frame for meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER fromline 1664**/

    /* end of frame for routine meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER fromline 1723 */

    /* classy proc frame meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER */ MeltFrame_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER
    meltfram__ /*declfrastruct fromline 1747*/
    /*classyprocarg meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER fromline 1752*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1756*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1760*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PATEXPANDOBSOLETE_CONTAINER", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-moremacro.melt:198:/ getarg");
    /*_.SEXPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PCTX__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
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
        MELT_LOCATION("warmelt-moremacro.melt:199:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-moremacro.melt:199:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-moremacro.melt:199:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 199;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpandobsolete_container sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:199:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-moremacro.melt:199:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V5*/ meltfptr[4] = 0 ;
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
        MELT_LOCATION("warmelt-moremacro.melt:200:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-moremacro.melt:200:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-moremacro.melt:200:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-moremacro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 200;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-moremacro.melt:200:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-moremacro.melt:200:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-moremacro.melt:201:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                    /*_.LOCA_LOCATION__V10*/ meltfptr[8] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LOCA_LOCATION__V10*/ meltfptr[8] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-moremacro.melt:201:/ locexp");
            melt_warning_str(0, (melt_ptr_t)(/*_.LOCA_LOCATION__V10*/ meltfptr[8]), ( "obsolete use of CONTAINER in pattern; use REFERENCE instead"), (melt_ptr_t)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:203:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
            /*_.PATEXPAND_REFERENCE__V11*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!PATEXPAND_REFERENCE*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-moremacro.melt:198:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.PATEXPAND_REFERENCE__V11*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-moremacro.melt:198:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOCA_LOCATION__V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PATEXPAND_REFERENCE__V11*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PATEXPANDOBSOLETE_CONTAINER", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER*/



/*no syntax testing generated*/

/* the frame for initial routine */
/** start of frame for melt_start_this_module of CLASS_INITIAL_MODULE_ROUTINEOBJ from 1522**/

/*curframdeclclassy*/ class Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166// fromline 1535
    : public Melt_CallFrameWithValues<87>
{
public: /* fromline 1539*/
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
    Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166(meltclosure_ptr_t clos) //the closure constructor fromline 1597
        : Melt_CallFrameWithValues<87> (
#if ENABLE_CHECKING /*fromline 1601*/
            __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1605*/
            sizeof(Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166), clos) {};
    Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166() //the constructor fromline 1609
        : Melt_CallFrameWithValues<87> (
#if ENABLE_CHECKING /*fromline 1613*/
            __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1617*/
            sizeof(Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166)) {};
#if ENABLE_CHECKING /*fromline 1621*/
    Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166(const char*fil, int lin) //the constructor fromline 1623
        : Melt_CallFrameWithValues<87> (fil,lin, sizeof(Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166)) {};
#endif /* ENABLE_CHECKING fromline 1633*/

}; // end  class Melt_InitialClassyFrame_WARMELTmiMOREMACRO_h160297166


/** end of frame for melt_start_this_module fromline 1664**/



static void melt_initialize_module_meltdata_WARMELTmiMOREMACRO (meltinitial_frame_t *iniframp__, char meltpredefinited[])
{
#define meltfram__  (*iniframp__)

    /* curframe_cdat_struct */ struct meltcdata_st
    {
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
        struct MELT_ROUTINE_STRUCT(1) drout_35__FILTERGCCVERSION;
        struct MELT_CLOSURE_STRUCT(0) dclo_36__FILTERGCCVERSION;
        struct MELT_ROUTINE_STRUCT(7) drout_37__MEXPAND_GCCIF;
        struct MELT_CLOSURE_STRUCT(0) dclo_38__MEXPAND_GCCIF;
        struct MELT_OBJECT_STRUCT(3) dsym_39__CLASS_ENVIRONMENT;
        struct MELT_STRING_STRUCT(17) dstr_40__CLASS_ENVIRONMENT;
        struct MELT_OBJECT_STRUCT(3) dsym_41__EXPAND_PAIRLIST_AS_TUPLE;
        struct MELT_STRING_STRUCT(24) dstr_42__EXPAND_PAIRLIST_AS_TUPLE;
        struct MELT_OBJECT_STRUCT(3) dsym_43__TRUE;
        struct MELT_STRING_STRUCT(4) dstr_44__TRUE;
        struct MELT_OBJECT_STRUCT(3) dsym_45__INSTALL_INITIAL_MACRO;
        struct MELT_STRING_STRUCT(21) dstr_46__INSTALL_INITIAL_MACRO;
        struct MELT_OBJECT_STRUCT(3) dsym_47__GCCIF;
        struct MELT_STRING_STRUCT(5) dstr_48__GCCIF;
        struct MELT_ROUTINE_STRUCT(5) drout_49__MEXPANDOBSOLETE_CONTAINER;
        struct MELT_CLOSURE_STRUCT(0) dclo_50__MEXPANDOBSOLETE_CONTAINER;
        struct MELT_OBJECT_STRUCT(3) dsym_51__MEXPAND_REFERENCE;
        struct MELT_STRING_STRUCT(17) dstr_52__MEXPAND_REFERENCE;
        struct MELT_ROUTINE_STRUCT(5) drout_53__PATEXPANDOBSOLETE_CONTAINER;
        struct MELT_CLOSURE_STRUCT(0) dclo_54__PATEXPANDOBSOLETE_CONTAINER;
        struct MELT_OBJECT_STRUCT(3) dsym_55__PATEXPAND_REFERENCE;
        struct MELT_STRING_STRUCT(19) dstr_56__PATEXPAND_REFERENCE;
        struct MELT_OBJECT_STRUCT(3) dsym_57__INSTALL_INITIAL_PATMACRO;
        struct MELT_STRING_STRUCT(24) dstr_58__INSTALL_INITIAL_PATMACRO;
        struct MELT_OBJECT_STRUCT(3) dsym_59__CONTAINER;
        struct MELT_STRING_STRUCT(9) dstr_60__CONTAINER;
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
    /*uniqueobj*/ if (!/*_._VALDATA___V24*/ meltfptr[23])
        /*_._VALDATA___V24*/ meltfptr[23] = (melt_ptr_t )&meltcdat->dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER.obj_hash = 824058733;
    meltcdat->dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER.obj_len = 3;


    /*inistring dstr_2__HOOK_FRESH_ENVIRONMENT_R*/
    /*_._VALSTR___V25*/ meltfptr[24] = (melt_ptr_t) &meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R;
    meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.val, "HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER", sizeof (meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.val)-1);
    meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.val[38] = (char)0 ;
    meltcdat->dstr_2__HOOK_FRESH_ENVIRONMENT_R.slen = 38 ;

    /*inirout drout_3__EACH_COMPONENT_IN_LIST*/
    /*iniroutlocvar*/ /*_._VALROUT___V26*/ meltfptr[25] = (melt_ptr_t) &meltcdat->drout_3__EACH_COMPONENT_IN_LIST;
    meltcdat->drout_3__EACH_COMPONENT_IN_LIST.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_ROUTINE))));
    strncpy (meltcdat->drout_3__EACH_COMPONENT_IN_LIST.routdescr, "EACH_COMPONENT_IN_LIST @warmelt-moremacro.melt:39",  MELT_ROUTDESCR_LEN - 1);
    meltcdat->drout_3__EACH_COMPONENT_IN_LIST.nbval = 14;
    MELT_ROUTINE_SET_ROUTCODE(&meltcdat->drout_3__EACH_COMPONENT_IN_LIST, meltrout_1_WARMELTmiMOREMACRO_EACH_COMPONENT_IN_LIST);

    /*iniclos dclo_4__EACH_COMPONENT_IN_LIST*/
    /*_._VALCLO___V31*/ meltfptr[30] = (melt_ptr_t) &meltcdat->dclo_4__EACH_COMPONENT_IN_LIST;
    meltcdat->dclo_4__EACH_COMPONENT_IN_LIST.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MACRO_CLOSURE))));
    meltcdat->dclo_4__EACH_COMPONENT_IN_LIST.nbval = 0;


    /*iniobj dsym_5__MELT_DEBUG_FUN*/
    /*uniqueobj*/ if (!/*_._VALDATA___V32*/ meltfptr[31])
        /*_._VALDATA___V32*/ meltfptr[31] = (melt_ptr_t )&meltcdat->dsym_5__MELT_DEBUG_FUN;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_5__MELT_DEBUG_FUN", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_5__MELT_DEBUG_FUN", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_5__MELT_DEBUG_FUN.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_5__MELT_DEBUG_FUN.obj_hash = 972123377;
    meltcdat->dsym_5__MELT_DEBUG_FUN.obj_len = 3;


    /*inistring dstr_6__MELT_DEBUG_FUN*/
    /*_._VALSTR___V33*/ meltfptr[32] = (melt_ptr_t) &meltcdat->dstr_6__MELT_DEBUG_FUN;
    meltcdat->dstr_6__MELT_DEBUG_FUN.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_6__MELT_DEBUG_FUN.val, "MELT_DEBUG_FUN", sizeof (meltcdat->dstr_6__MELT_DEBUG_FUN.val)-1);
    meltcdat->dstr_6__MELT_DEBUG_FUN.val[14] = (char)0 ;
    meltcdat->dstr_6__MELT_DEBUG_FUN.slen = 14 ;


    /*iniobj dsym_7__DEBUG_LESS*/
    /*uniqueobj*/ if (!/*_._VALDATA___V34*/ meltfptr[33])
        /*_._VALDATA___V34*/ meltfptr[33] = (melt_ptr_t )&meltcdat->dsym_7__DEBUG_LESS;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_7__DEBUG_LESS", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_7__DEBUG_LESS", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_7__DEBUG_LESS.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_7__DEBUG_LESS.obj_hash = 977096719;
    meltcdat->dsym_7__DEBUG_LESS.obj_len = 3;


    /*inistring dstr_8__DEBUG_LESS*/
    /*_._VALSTR___V35*/ meltfptr[34] = (melt_ptr_t) &meltcdat->dstr_8__DEBUG_LESS;
    meltcdat->dstr_8__DEBUG_LESS.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_8__DEBUG_LESS.val, "DEBUG_LESS", sizeof (meltcdat->dstr_8__DEBUG_LESS.val)-1);
    meltcdat->dstr_8__DEBUG_LESS.val[10] = (char)0 ;
    meltcdat->dstr_8__DEBUG_LESS.slen = 10 ;


    /*iniobj dsym_9__CLASS_LOCATED*/
    /*uniqueobj*/ if (!/*_._VALDATA___V36*/ meltfptr[35])
        /*_._VALDATA___V36*/ meltfptr[35] = (melt_ptr_t )&meltcdat->dsym_9__CLASS_LOCATED;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_9__CLASS_LOCATED", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_9__CLASS_LOCATED", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_9__CLASS_LOCATED.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_9__CLASS_LOCATED.obj_hash = 979003;
    meltcdat->dsym_9__CLASS_LOCATED.obj_len = 3;


    /*inistring dstr_10__CLASS_LOCATED*/
    /*_._VALSTR___V37*/ meltfptr[36] = (melt_ptr_t) &meltcdat->dstr_10__CLASS_LOCATED;
    meltcdat->dstr_10__CLASS_LOCATED.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_10__CLASS_LOCATED.val, "CLASS_LOCATED", sizeof (meltcdat->dstr_10__CLASS_LOCATED.val)-1);
    meltcdat->dstr_10__CLASS_LOCATED.val[13] = (char)0 ;
    meltcdat->dstr_10__CLASS_LOCATED.slen = 13 ;


    /*iniobj dsym_11__CLASS_SEXPR*/
    /*uniqueobj*/ if (!/*_._VALDATA___V38*/ meltfptr[37])
        /*_._VALDATA___V38*/ meltfptr[37] = (melt_ptr_t )&meltcdat->dsym_11__CLASS_SEXPR;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_11__CLASS_SEXPR", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_11__CLASS_SEXPR", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_11__CLASS_SEXPR.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_11__CLASS_SEXPR.obj_hash = 53411612;
    meltcdat->dsym_11__CLASS_SEXPR.obj_len = 3;


    /*inistring dstr_12__CLASS_SEXPR*/
    /*_._VALSTR___V39*/ meltfptr[38] = (melt_ptr_t) &meltcdat->dstr_12__CLASS_SEXPR;
    meltcdat->dstr_12__CLASS_SEXPR.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_12__CLASS_SEXPR.val, "CLASS_SEXPR", sizeof (meltcdat->dstr_12__CLASS_SEXPR.val)-1);
    meltcdat->dstr_12__CLASS_SEXPR.val[11] = (char)0 ;
    meltcdat->dstr_12__CLASS_SEXPR.slen = 11 ;


    /*iniobj dsym_13__MELT_ASSERT_FAILURE_FUN*/
    /*uniqueobj*/ if (!/*_._VALDATA___V40*/ meltfptr[39])
        /*_._VALDATA___V40*/ meltfptr[39] = (melt_ptr_t )&meltcdat->dsym_13__MELT_ASSERT_FAILURE_FUN;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_13__MELT_ASSERT_FAILURE_FUN", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_13__MELT_ASSERT_FAILURE_FUN", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_13__MELT_ASSERT_FAILURE_FUN.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_13__MELT_ASSERT_FAILURE_FUN.obj_hash = 215547874;
    meltcdat->dsym_13__MELT_ASSERT_FAILURE_FUN.obj_len = 3;


    /*inistring dstr_14__MELT_ASSERT_FAILURE_FUN*/
    /*_._VALSTR___V41*/ meltfptr[40] = (melt_ptr_t) &meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN;
    meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.val, "MELT_ASSERT_FAILURE_FUN", sizeof (meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.val)-1);
    meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.val[23] = (char)0 ;
    meltcdat->dstr_14__MELT_ASSERT_FAILURE_FUN.slen = 23 ;


    /*iniobj dsym_15__LIST_TO_MULTIPLE*/
    /*uniqueobj*/ if (!/*_._VALDATA___V42*/ meltfptr[41])
        /*_._VALDATA___V42*/ meltfptr[41] = (melt_ptr_t )&meltcdat->dsym_15__LIST_TO_MULTIPLE;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_15__LIST_TO_MULTIPLE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_15__LIST_TO_MULTIPLE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_15__LIST_TO_MULTIPLE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_15__LIST_TO_MULTIPLE.obj_hash = 987171205;
    meltcdat->dsym_15__LIST_TO_MULTIPLE.obj_len = 3;


    /*inistring dstr_16__LIST_TO_MULTIPLE*/
    /*_._VALSTR___V43*/ meltfptr[42] = (melt_ptr_t) &meltcdat->dstr_16__LIST_TO_MULTIPLE;
    meltcdat->dstr_16__LIST_TO_MULTIPLE.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_16__LIST_TO_MULTIPLE.val, "LIST_TO_MULTIPLE", sizeof (meltcdat->dstr_16__LIST_TO_MULTIPLE.val)-1);
    meltcdat->dstr_16__LIST_TO_MULTIPLE.val[16] = (char)0 ;
    meltcdat->dstr_16__LIST_TO_MULTIPLE.slen = 16 ;


    /*iniobj dsym_17__DISCR_MULTIPLE*/
    /*uniqueobj*/ if (!/*_._VALDATA___V44*/ meltfptr[43])
        /*_._VALDATA___V44*/ meltfptr[43] = (melt_ptr_t )&meltcdat->dsym_17__DISCR_MULTIPLE;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_17__DISCR_MULTIPLE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_17__DISCR_MULTIPLE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_17__DISCR_MULTIPLE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_17__DISCR_MULTIPLE.obj_hash = 91864583;
    meltcdat->dsym_17__DISCR_MULTIPLE.obj_len = 3;


    /*inistring dstr_18__DISCR_MULTIPLE*/
    /*_._VALSTR___V45*/ meltfptr[44] = (melt_ptr_t) &meltcdat->dstr_18__DISCR_MULTIPLE;
    meltcdat->dstr_18__DISCR_MULTIPLE.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_18__DISCR_MULTIPLE.val, "DISCR_MULTIPLE", sizeof (meltcdat->dstr_18__DISCR_MULTIPLE.val)-1);
    meltcdat->dstr_18__DISCR_MULTIPLE.val[14] = (char)0 ;
    meltcdat->dstr_18__DISCR_MULTIPLE.slen = 14 ;


    /*iniobj dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE*/
    /*uniqueobj*/ if (!/*_._VALDATA___V46*/ meltfptr[45])
        /*_._VALDATA___V46*/ meltfptr[45] = (melt_ptr_t )&meltcdat->dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE.obj_hash = 746513982;
    meltcdat->dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE.obj_len = 3;


    /*inistring dstr_20__EXPAND_TUPLE_SLICE_AS_TU*/
    /*_._VALSTR___V47*/ meltfptr[46] = (melt_ptr_t) &meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU;
    meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.val, "EXPAND_TUPLE_SLICE_AS_TUPLE", sizeof (meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.val)-1);
    meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.val[27] = (char)0 ;
    meltcdat->dstr_20__EXPAND_TUPLE_SLICE_AS_TU.slen = 27 ;


    /*iniobj dsym_21__CLASS_SYMBOL*/
    /*uniqueobj*/ if (!/*_._VALDATA___V48*/ meltfptr[47])
        /*_._VALDATA___V48*/ meltfptr[47] = (melt_ptr_t )&meltcdat->dsym_21__CLASS_SYMBOL;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_21__CLASS_SYMBOL", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_21__CLASS_SYMBOL", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_21__CLASS_SYMBOL.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_21__CLASS_SYMBOL.obj_hash = 53524182;
    meltcdat->dsym_21__CLASS_SYMBOL.obj_len = 3;


    /*inistring dstr_22__CLASS_SYMBOL*/
    /*_._VALSTR___V49*/ meltfptr[48] = (melt_ptr_t) &meltcdat->dstr_22__CLASS_SYMBOL;
    meltcdat->dstr_22__CLASS_SYMBOL.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_22__CLASS_SYMBOL.val, "CLASS_SYMBOL", sizeof (meltcdat->dstr_22__CLASS_SYMBOL.val)-1);
    meltcdat->dstr_22__CLASS_SYMBOL.val[12] = (char)0 ;
    meltcdat->dstr_22__CLASS_SYMBOL.slen = 12 ;


    /*iniobj dsym_23__CLONE_SYMBOL*/
    /*uniqueobj*/ if (!/*_._VALDATA___V50*/ meltfptr[49])
        /*_._VALDATA___V50*/ meltfptr[49] = (melt_ptr_t )&meltcdat->dsym_23__CLONE_SYMBOL;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_23__CLONE_SYMBOL", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_23__CLONE_SYMBOL", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_23__CLONE_SYMBOL.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_23__CLONE_SYMBOL.obj_hash = 213258240;
    meltcdat->dsym_23__CLONE_SYMBOL.obj_len = 3;


    /*inistring dstr_24__CLONE_SYMBOL*/
    /*_._VALSTR___V51*/ meltfptr[50] = (melt_ptr_t) &meltcdat->dstr_24__CLONE_SYMBOL;
    meltcdat->dstr_24__CLONE_SYMBOL.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_24__CLONE_SYMBOL.val, "CLONE_SYMBOL", sizeof (meltcdat->dstr_24__CLONE_SYMBOL.val)-1);
    meltcdat->dstr_24__CLONE_SYMBOL.val[12] = (char)0 ;
    meltcdat->dstr_24__CLONE_SYMBOL.slen = 12 ;


    /*iniobj dsym_25__CURPAIRINLISTSYMB*/
    /*uniqueobj*/ if (!/*_._VALDATA___V27*/ meltfptr[26])
        /*_._VALDATA___V27*/ meltfptr[26] = (melt_ptr_t )&meltcdat->dsym_25__CURPAIRINLISTSYMB;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_25__CURPAIRINLISTSYMB", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_25__CURPAIRINLISTSYMB", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_25__CURPAIRINLISTSYMB.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_25__CURPAIRINLISTSYMB.obj_hash = 580598334;
    meltcdat->dsym_25__CURPAIRINLISTSYMB.obj_len = 3;


    /*inistring dstr_26__CURPAIRINLISTSYMB*/
    /*_._VALSTR___V28*/ meltfptr[27] = (melt_ptr_t) &meltcdat->dstr_26__CURPAIRINLISTSYMB;
    meltcdat->dstr_26__CURPAIRINLISTSYMB.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_26__CURPAIRINLISTSYMB.val, "CURPAIRINLISTSYMB", sizeof (meltcdat->dstr_26__CURPAIRINLISTSYMB.val)-1);
    meltcdat->dstr_26__CURPAIRINLISTSYMB.val[17] = (char)0 ;
    meltcdat->dstr_26__CURPAIRINLISTSYMB.slen = 17 ;


    /*iniobj dsym_27__MELT_MAKE_SEXPR*/
    /*uniqueobj*/ if (!/*_._VALDATA___V52*/ meltfptr[51])
        /*_._VALDATA___V52*/ meltfptr[51] = (melt_ptr_t )&meltcdat->dsym_27__MELT_MAKE_SEXPR;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_27__MELT_MAKE_SEXPR", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_27__MELT_MAKE_SEXPR", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_27__MELT_MAKE_SEXPR.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_27__MELT_MAKE_SEXPR.obj_hash = 383667794;
    meltcdat->dsym_27__MELT_MAKE_SEXPR.obj_len = 3;


    /*inistring dstr_28__MELT_MAKE_SEXPR*/
    /*_._VALSTR___V53*/ meltfptr[52] = (melt_ptr_t) &meltcdat->dstr_28__MELT_MAKE_SEXPR;
    meltcdat->dstr_28__MELT_MAKE_SEXPR.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_28__MELT_MAKE_SEXPR.val, "MELT_MAKE_SEXPR", sizeof (meltcdat->dstr_28__MELT_MAKE_SEXPR.val)-1);
    meltcdat->dstr_28__MELT_MAKE_SEXPR.val[15] = (char)0 ;
    meltcdat->dstr_28__MELT_MAKE_SEXPR.slen = 15 ;


    /*iniobj dsym_29__HOOK_MELT_MAKE_LOCATION*/
    /*uniqueobj*/ if (!/*_._VALDATA___V54*/ meltfptr[53])
        /*_._VALDATA___V54*/ meltfptr[53] = (melt_ptr_t )&meltcdat->dsym_29__HOOK_MELT_MAKE_LOCATION;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_29__HOOK_MELT_MAKE_LOCATION", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_29__HOOK_MELT_MAKE_LOCATION", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_29__HOOK_MELT_MAKE_LOCATION.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_29__HOOK_MELT_MAKE_LOCATION.obj_hash = 753739361;
    meltcdat->dsym_29__HOOK_MELT_MAKE_LOCATION.obj_len = 3;


    /*inistring dstr_30__HOOK_MELT_MAKE_LOCATION*/
    /*_._VALSTR___V55*/ meltfptr[54] = (melt_ptr_t) &meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION;
    meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.val, "HOOK_MELT_MAKE_LOCATION", sizeof (meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.val)-1);
    meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.val[23] = (char)0 ;
    meltcdat->dstr_30__HOOK_MELT_MAKE_LOCATION.slen = 23 ;


    /*iniobj dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST*/
    /*uniqueobj*/ if (!/*_._VALDATA___V29*/ meltfptr[28])
        /*_._VALDATA___V29*/ meltfptr[28] = (melt_ptr_t )&meltcdat->dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST.obj_hash = 535752972;
    meltcdat->dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST.obj_len = 3;


    /*inistring dstr_32__FOREACH_PAIR_COMPONENT_I*/
    /*_._VALSTR___V30*/ meltfptr[29] = (melt_ptr_t) &meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I;
    meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.val, "FOREACH_PAIR_COMPONENT_IN_LIST", sizeof (meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.val)-1);
    meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.val[30] = (char)0 ;
    meltcdat->dstr_32__FOREACH_PAIR_COMPONENT_I.slen = 30 ;


    /*iniobj dsym_33__EACH_COMPONENT_IN_LIST*/
    /*uniqueobj*/ if (!/*_._VALDATA___V56*/ meltfptr[55])
        /*_._VALDATA___V56*/ meltfptr[55] = (melt_ptr_t )&meltcdat->dsym_33__EACH_COMPONENT_IN_LIST;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_33__EACH_COMPONENT_IN_LIST", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_33__EACH_COMPONENT_IN_LIST", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_33__EACH_COMPONENT_IN_LIST.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_33__EACH_COMPONENT_IN_LIST.obj_hash = 396791142;
    meltcdat->dsym_33__EACH_COMPONENT_IN_LIST.obj_len = 3;


    /*inistring dstr_34__EACH_COMPONENT_IN_LIST*/
    /*_._VALSTR___V57*/ meltfptr[56] = (melt_ptr_t) &meltcdat->dstr_34__EACH_COMPONENT_IN_LIST;
    meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.val, "EACH_COMPONENT_IN_LIST", sizeof (meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.val)-1);
    meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.val[22] = (char)0 ;
    meltcdat->dstr_34__EACH_COMPONENT_IN_LIST.slen = 22 ;

    /*inirout drout_35__FILTERGCCVERSION*/
    /*iniroutlocvar*/ /*_._VALROUT___V58*/ meltfptr[57] = (melt_ptr_t) &meltcdat->drout_35__FILTERGCCVERSION;
    meltcdat->drout_35__FILTERGCCVERSION.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_ROUTINE))));
    strncpy (meltcdat->drout_35__FILTERGCCVERSION.routdescr, "FILTERGCCVERSION @warmelt-moremacro.melt:112",  MELT_ROUTDESCR_LEN - 1);
    meltcdat->drout_35__FILTERGCCVERSION.nbval = 1;
    MELT_ROUTINE_SET_ROUTCODE(&meltcdat->drout_35__FILTERGCCVERSION, meltrout_2_WARMELTmiMOREMACRO_FILTERGCCVERSION);

    /*iniclos dclo_36__FILTERGCCVERSION*/
    /*_._VALCLO___V59*/ meltfptr[58] = (melt_ptr_t) &meltcdat->dclo_36__FILTERGCCVERSION;
    meltcdat->dclo_36__FILTERGCCVERSION.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE))));
    meltcdat->dclo_36__FILTERGCCVERSION.nbval = 0;

    /*inirout drout_37__MEXPAND_GCCIF*/
    /*iniroutlocvar*/ /*_._VALROUT___V60*/ meltfptr[59] = (melt_ptr_t) &meltcdat->drout_37__MEXPAND_GCCIF;
    meltcdat->drout_37__MEXPAND_GCCIF.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_ROUTINE))));
    strncpy (meltcdat->drout_37__MEXPAND_GCCIF.routdescr, "MEXPAND_GCCIF @warmelt-moremacro.melt:127",  MELT_ROUTDESCR_LEN - 1);
    meltcdat->drout_37__MEXPAND_GCCIF.nbval = 7;
    MELT_ROUTINE_SET_ROUTCODE(&meltcdat->drout_37__MEXPAND_GCCIF, meltrout_3_WARMELTmiMOREMACRO_MEXPAND_GCCIF);

    /*iniclos dclo_38__MEXPAND_GCCIF*/
    /*_._VALCLO___V63*/ meltfptr[62] = (melt_ptr_t) &meltcdat->dclo_38__MEXPAND_GCCIF;
    meltcdat->dclo_38__MEXPAND_GCCIF.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE))));
    meltcdat->dclo_38__MEXPAND_GCCIF.nbval = 0;


    /*iniobj dsym_39__CLASS_ENVIRONMENT*/
    /*uniqueobj*/ if (!/*_._VALDATA___V64*/ meltfptr[63])
        /*_._VALDATA___V64*/ meltfptr[63] = (melt_ptr_t )&meltcdat->dsym_39__CLASS_ENVIRONMENT;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_39__CLASS_ENVIRONMENT", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_39__CLASS_ENVIRONMENT", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_39__CLASS_ENVIRONMENT.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_39__CLASS_ENVIRONMENT.obj_hash = 59212821;
    meltcdat->dsym_39__CLASS_ENVIRONMENT.obj_len = 3;


    /*inistring dstr_40__CLASS_ENVIRONMENT*/
    /*_._VALSTR___V65*/ meltfptr[64] = (melt_ptr_t) &meltcdat->dstr_40__CLASS_ENVIRONMENT;
    meltcdat->dstr_40__CLASS_ENVIRONMENT.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_40__CLASS_ENVIRONMENT.val, "CLASS_ENVIRONMENT", sizeof (meltcdat->dstr_40__CLASS_ENVIRONMENT.val)-1);
    meltcdat->dstr_40__CLASS_ENVIRONMENT.val[17] = (char)0 ;
    meltcdat->dstr_40__CLASS_ENVIRONMENT.slen = 17 ;


    /*iniobj dsym_41__EXPAND_PAIRLIST_AS_TUPLE*/
    /*uniqueobj*/ if (!/*_._VALDATA___V66*/ meltfptr[65])
        /*_._VALDATA___V66*/ meltfptr[65] = (melt_ptr_t )&meltcdat->dsym_41__EXPAND_PAIRLIST_AS_TUPLE;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_41__EXPAND_PAIRLIST_AS_TUPLE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_41__EXPAND_PAIRLIST_AS_TUPLE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_41__EXPAND_PAIRLIST_AS_TUPLE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_41__EXPAND_PAIRLIST_AS_TUPLE.obj_hash = 953849859;
    meltcdat->dsym_41__EXPAND_PAIRLIST_AS_TUPLE.obj_len = 3;


    /*inistring dstr_42__EXPAND_PAIRLIST_AS_TUPLE*/
    /*_._VALSTR___V67*/ meltfptr[66] = (melt_ptr_t) &meltcdat->dstr_42__EXPAND_PAIRLIST_AS_TUPLE;
    meltcdat->dstr_42__EXPAND_PAIRLIST_AS_TUPLE.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_42__EXPAND_PAIRLIST_AS_TUPLE.val, "EXPAND_PAIRLIST_AS_TUPLE", sizeof (meltcdat->dstr_42__EXPAND_PAIRLIST_AS_TUPLE.val)-1);
    meltcdat->dstr_42__EXPAND_PAIRLIST_AS_TUPLE.val[24] = (char)0 ;
    meltcdat->dstr_42__EXPAND_PAIRLIST_AS_TUPLE.slen = 24 ;


    /*iniobj dsym_43__TRUE*/
    /*uniqueobj*/ if (!/*_._VALDATA___V61*/ meltfptr[60])
        /*_._VALDATA___V61*/ meltfptr[60] = (melt_ptr_t )&meltcdat->dsym_43__TRUE;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_43__TRUE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_KEYWORD))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_43__TRUE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_KEYWORD)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_43__TRUE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_KEYWORD))));
    meltcdat->dsym_43__TRUE.obj_hash = 576485513;
    meltcdat->dsym_43__TRUE.obj_len = 3;


    /*inistring dstr_44__TRUE*/
    /*_._VALSTR___V62*/ meltfptr[61] = (melt_ptr_t) &meltcdat->dstr_44__TRUE;
    meltcdat->dstr_44__TRUE.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_44__TRUE.val, "TRUE", sizeof (meltcdat->dstr_44__TRUE.val)-1);
    meltcdat->dstr_44__TRUE.val[4] = (char)0 ;
    meltcdat->dstr_44__TRUE.slen = 4 ;


    /*iniobj dsym_45__INSTALL_INITIAL_MACRO*/
    /*uniqueobj*/ if (!/*_._VALDATA___V68*/ meltfptr[67])
        /*_._VALDATA___V68*/ meltfptr[67] = (melt_ptr_t )&meltcdat->dsym_45__INSTALL_INITIAL_MACRO;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_45__INSTALL_INITIAL_MACRO", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_45__INSTALL_INITIAL_MACRO", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_45__INSTALL_INITIAL_MACRO.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_45__INSTALL_INITIAL_MACRO.obj_hash = 101362976;
    meltcdat->dsym_45__INSTALL_INITIAL_MACRO.obj_len = 3;


    /*inistring dstr_46__INSTALL_INITIAL_MACRO*/
    /*_._VALSTR___V69*/ meltfptr[68] = (melt_ptr_t) &meltcdat->dstr_46__INSTALL_INITIAL_MACRO;
    meltcdat->dstr_46__INSTALL_INITIAL_MACRO.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_46__INSTALL_INITIAL_MACRO.val, "INSTALL_INITIAL_MACRO", sizeof (meltcdat->dstr_46__INSTALL_INITIAL_MACRO.val)-1);
    meltcdat->dstr_46__INSTALL_INITIAL_MACRO.val[21] = (char)0 ;
    meltcdat->dstr_46__INSTALL_INITIAL_MACRO.slen = 21 ;


    /*iniobj dsym_47__GCCIF*/
    /*uniqueobj*/ if (!/*_._VALDATA___V70*/ meltfptr[69])
        /*_._VALDATA___V70*/ meltfptr[69] = (melt_ptr_t )&meltcdat->dsym_47__GCCIF;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_47__GCCIF", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_47__GCCIF", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_47__GCCIF.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_47__GCCIF.obj_hash = 796305141;
    meltcdat->dsym_47__GCCIF.obj_len = 3;


    /*inistring dstr_48__GCCIF*/
    /*_._VALSTR___V71*/ meltfptr[70] = (melt_ptr_t) &meltcdat->dstr_48__GCCIF;
    meltcdat->dstr_48__GCCIF.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_48__GCCIF.val, "GCCIF", sizeof (meltcdat->dstr_48__GCCIF.val)-1);
    meltcdat->dstr_48__GCCIF.val[5] = (char)0 ;
    meltcdat->dstr_48__GCCIF.slen = 5 ;

    /*inirout drout_49__MEXPANDOBSOLETE_CONTAINER*/
    /*iniroutlocvar*/ /*_._VALROUT___V72*/ meltfptr[71] = (melt_ptr_t) &meltcdat->drout_49__MEXPANDOBSOLETE_CONTAINER;
    meltcdat->drout_49__MEXPANDOBSOLETE_CONTAINER.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_ROUTINE))));
    strncpy (meltcdat->drout_49__MEXPANDOBSOLETE_CONTAINER.routdescr, "MEXPANDOBSOLETE_CONTAINER @warmelt-moremacro.melt:191",  MELT_ROUTDESCR_LEN - 1);
    meltcdat->drout_49__MEXPANDOBSOLETE_CONTAINER.nbval = 5;
    MELT_ROUTINE_SET_ROUTCODE(&meltcdat->drout_49__MEXPANDOBSOLETE_CONTAINER, meltrout_4_WARMELTmiMOREMACRO_MEXPANDOBSOLETE_CONTAINER);

    /*iniclos dclo_50__MEXPANDOBSOLETE_CONTAINER*/
    /*_._VALCLO___V73*/ meltfptr[72] = (melt_ptr_t) &meltcdat->dclo_50__MEXPANDOBSOLETE_CONTAINER;
    meltcdat->dclo_50__MEXPANDOBSOLETE_CONTAINER.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE))));
    meltcdat->dclo_50__MEXPANDOBSOLETE_CONTAINER.nbval = 0;


    /*iniobj dsym_51__MEXPAND_REFERENCE*/
    /*uniqueobj*/ if (!/*_._VALDATA___V74*/ meltfptr[73])
        /*_._VALDATA___V74*/ meltfptr[73] = (melt_ptr_t )&meltcdat->dsym_51__MEXPAND_REFERENCE;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_51__MEXPAND_REFERENCE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_51__MEXPAND_REFERENCE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_51__MEXPAND_REFERENCE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_51__MEXPAND_REFERENCE.obj_hash = 454324238;
    meltcdat->dsym_51__MEXPAND_REFERENCE.obj_len = 3;


    /*inistring dstr_52__MEXPAND_REFERENCE*/
    /*_._VALSTR___V75*/ meltfptr[74] = (melt_ptr_t) &meltcdat->dstr_52__MEXPAND_REFERENCE;
    meltcdat->dstr_52__MEXPAND_REFERENCE.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_52__MEXPAND_REFERENCE.val, "MEXPAND_REFERENCE", sizeof (meltcdat->dstr_52__MEXPAND_REFERENCE.val)-1);
    meltcdat->dstr_52__MEXPAND_REFERENCE.val[17] = (char)0 ;
    meltcdat->dstr_52__MEXPAND_REFERENCE.slen = 17 ;

    /*inirout drout_53__PATEXPANDOBSOLETE_CONTAINER*/
    /*iniroutlocvar*/ /*_._VALROUT___V76*/ meltfptr[75] = (melt_ptr_t) &meltcdat->drout_53__PATEXPANDOBSOLETE_CONTAINER;
    meltcdat->drout_53__PATEXPANDOBSOLETE_CONTAINER.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_ROUTINE))));
    strncpy (meltcdat->drout_53__PATEXPANDOBSOLETE_CONTAINER.routdescr, "PATEXPANDOBSOLETE_CONTAINER @warmelt-moremacro.melt:198",  MELT_ROUTDESCR_LEN - 1);
    meltcdat->drout_53__PATEXPANDOBSOLETE_CONTAINER.nbval = 5;
    MELT_ROUTINE_SET_ROUTCODE(&meltcdat->drout_53__PATEXPANDOBSOLETE_CONTAINER, meltrout_5_WARMELTmiMOREMACRO_PATEXPANDOBSOLETE_CONTAINER);

    /*iniclos dclo_54__PATEXPANDOBSOLETE_CONTAINER*/
    /*_._VALCLO___V77*/ meltfptr[76] = (melt_ptr_t) &meltcdat->dclo_54__PATEXPANDOBSOLETE_CONTAINER;
    meltcdat->dclo_54__PATEXPANDOBSOLETE_CONTAINER.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE))));
    meltcdat->dclo_54__PATEXPANDOBSOLETE_CONTAINER.nbval = 0;


    /*iniobj dsym_55__PATEXPAND_REFERENCE*/
    /*uniqueobj*/ if (!/*_._VALDATA___V78*/ meltfptr[77])
        /*_._VALDATA___V78*/ meltfptr[77] = (melt_ptr_t )&meltcdat->dsym_55__PATEXPAND_REFERENCE;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_55__PATEXPAND_REFERENCE", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_55__PATEXPAND_REFERENCE", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_55__PATEXPAND_REFERENCE.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_55__PATEXPAND_REFERENCE.obj_hash = 929282371;
    meltcdat->dsym_55__PATEXPAND_REFERENCE.obj_len = 3;


    /*inistring dstr_56__PATEXPAND_REFERENCE*/
    /*_._VALSTR___V79*/ meltfptr[78] = (melt_ptr_t) &meltcdat->dstr_56__PATEXPAND_REFERENCE;
    meltcdat->dstr_56__PATEXPAND_REFERENCE.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_56__PATEXPAND_REFERENCE.val, "PATEXPAND_REFERENCE", sizeof (meltcdat->dstr_56__PATEXPAND_REFERENCE.val)-1);
    meltcdat->dstr_56__PATEXPAND_REFERENCE.val[19] = (char)0 ;
    meltcdat->dstr_56__PATEXPAND_REFERENCE.slen = 19 ;


    /*iniobj dsym_57__INSTALL_INITIAL_PATMACRO*/
    /*uniqueobj*/ if (!/*_._VALDATA___V80*/ meltfptr[79])
        /*_._VALDATA___V80*/ meltfptr[79] = (melt_ptr_t )&meltcdat->dsym_57__INSTALL_INITIAL_PATMACRO;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_57__INSTALL_INITIAL_PATMACRO", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_57__INSTALL_INITIAL_PATMACRO", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_57__INSTALL_INITIAL_PATMACRO.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_57__INSTALL_INITIAL_PATMACRO.obj_hash = 959787197;
    meltcdat->dsym_57__INSTALL_INITIAL_PATMACRO.obj_len = 3;


    /*inistring dstr_58__INSTALL_INITIAL_PATMACRO*/
    /*_._VALSTR___V81*/ meltfptr[80] = (melt_ptr_t) &meltcdat->dstr_58__INSTALL_INITIAL_PATMACRO;
    meltcdat->dstr_58__INSTALL_INITIAL_PATMACRO.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_58__INSTALL_INITIAL_PATMACRO.val, "INSTALL_INITIAL_PATMACRO", sizeof (meltcdat->dstr_58__INSTALL_INITIAL_PATMACRO.val)-1);
    meltcdat->dstr_58__INSTALL_INITIAL_PATMACRO.val[24] = (char)0 ;
    meltcdat->dstr_58__INSTALL_INITIAL_PATMACRO.slen = 24 ;


    /*iniobj dsym_59__CONTAINER*/
    /*uniqueobj*/ if (!/*_._VALDATA___V82*/ meltfptr[81])
        /*_._VALDATA___V82*/ meltfptr[81] = (melt_ptr_t )&meltcdat->dsym_59__CONTAINER;
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr isobj dsym_59__CONTAINER", melt_magic_discr ((melt_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))))) == MELTOBMAG_OBJECT);
    if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg("iniobj check.discr objmagic dsym_59__CONTAINER", ((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL)))))->meltobj_magic == MELTOBMAG_OBJECT);
    meltcdat->dsym_59__CONTAINER.meltobj_class = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(CLASS_SYMBOL))));
    meltcdat->dsym_59__CONTAINER.obj_hash = 910562261;
    meltcdat->dsym_59__CONTAINER.obj_len = 3;


    /*inistring dstr_60__CONTAINER*/
    /*_._VALSTR___V83*/ meltfptr[82] = (melt_ptr_t) &meltcdat->dstr_60__CONTAINER;
    meltcdat->dstr_60__CONTAINER.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_STRING))));
    /*small inistring*/ strncpy(meltcdat->dstr_60__CONTAINER.val, "CONTAINER", sizeof (meltcdat->dstr_60__CONTAINER.val)-1);
    meltcdat->dstr_60__CONTAINER.val[9] = (char)0 ;
    meltcdat->dstr_60__CONTAINER.slen = 9 ;
    meltcdat = NULL;
    melt_prohibit_garbcoll = FALSE;

#undef meltfram__
} /*end melt_initialize_module_meltdata_WARMELTmiMOREMACRO*/


#if MELT_HAVE_DEBUG
const char meltmodule_WARMELTmiMOREMACRO__melt_have_debug_enabled[] = "MELT module warmelt-moremacro have debug enabled";
#else /*!MELT_HAVE_DEBUG*/
const char meltmodule_WARMELTmiMOREMACRO__melt_have_debug_disabled[] = "MELT module warmelt-moremacro have debug disabled";
#endif /*MELT_HAVE_DEBUG*/


void* melt_start_this_module (void* modargp_)
{

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
    /*_._PREVENV___V4*/ meltfptr[3] = /*getprevenv*/ (melt_ptr_t) modargp_;
    /*^comment*/
    /**COMMENT: compute boxloc **/;

    /*^cond*/
    /*cond*/ if (/*_._CONTENV___V2*/ meltfptr[1]) /*then*/
        {
        }
    else    /*^cond.else*/
        {

            /*^compute*/

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
    /**COMMENT: get symbols & keywords for initial procedure **/;

    /*^getnamedsymbol*/
    /*getnamedsym:HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER*/

    if (NULL == /*_._VALDATA___V24*/ meltfptr[23])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER = melthookproc_HOOK_NAMED_SYMBOL ("HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER", (long) MELT_GET);
            /*_._VALDATA___V24*/ meltfptr[23] = (melt_ptr_t) meltsy_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER;
        } /*endgetnamedsym HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER*/


    /*^getnamedsymbol*/
    /*getnamedsym:MELT_DEBUG_FUN*/

    if (NULL == /*_._VALDATA___V32*/ meltfptr[31])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MELT_DEBUG_FUN without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_MELT_DEBUG_FUN = melthookproc_HOOK_NAMED_SYMBOL ("MELT_DEBUG_FUN", (long) MELT_GET);
            /*_._VALDATA___V32*/ meltfptr[31] = (melt_ptr_t) meltsy_MELT_DEBUG_FUN;
        } /*endgetnamedsym MELT_DEBUG_FUN*/


    /*^getnamedsymbol*/
    /*getnamedsym:DEBUG_LESS*/

    if (NULL == /*_._VALDATA___V34*/ meltfptr[33])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol DEBUG_LESS without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_DEBUG_LESS = melthookproc_HOOK_NAMED_SYMBOL ("DEBUG_LESS", (long) MELT_GET);
            /*_._VALDATA___V34*/ meltfptr[33] = (melt_ptr_t) meltsy_DEBUG_LESS;
        } /*endgetnamedsym DEBUG_LESS*/


    /*^getnamedsymbol*/
    /*getnamedsym:CLASS_LOCATED*/

    if (NULL == /*_._VALDATA___V36*/ meltfptr[35])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_LOCATED without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_CLASS_LOCATED = melthookproc_HOOK_NAMED_SYMBOL ("CLASS_LOCATED", (long) MELT_GET);
            /*_._VALDATA___V36*/ meltfptr[35] = (melt_ptr_t) meltsy_CLASS_LOCATED;
        } /*endgetnamedsym CLASS_LOCATED*/


    /*^getnamedsymbol*/
    /*getnamedsym:CLASS_SEXPR*/

    if (NULL == /*_._VALDATA___V38*/ meltfptr[37])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_SEXPR without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_CLASS_SEXPR = melthookproc_HOOK_NAMED_SYMBOL ("CLASS_SEXPR", (long) MELT_GET);
            /*_._VALDATA___V38*/ meltfptr[37] = (melt_ptr_t) meltsy_CLASS_SEXPR;
        } /*endgetnamedsym CLASS_SEXPR*/


    /*^getnamedsymbol*/
    /*getnamedsym:MELT_ASSERT_FAILURE_FUN*/

    if (NULL == /*_._VALDATA___V40*/ meltfptr[39])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MELT_ASSERT_FAILURE_FUN without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_MELT_ASSERT_FAILURE_FUN = melthookproc_HOOK_NAMED_SYMBOL ("MELT_ASSERT_FAILURE_FUN", (long) MELT_GET);
            /*_._VALDATA___V40*/ meltfptr[39] = (melt_ptr_t) meltsy_MELT_ASSERT_FAILURE_FUN;
        } /*endgetnamedsym MELT_ASSERT_FAILURE_FUN*/


    /*^getnamedsymbol*/
    /*getnamedsym:LIST_TO_MULTIPLE*/

    if (NULL == /*_._VALDATA___V42*/ meltfptr[41])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol LIST_TO_MULTIPLE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_LIST_TO_MULTIPLE = melthookproc_HOOK_NAMED_SYMBOL ("LIST_TO_MULTIPLE", (long) MELT_GET);
            /*_._VALDATA___V42*/ meltfptr[41] = (melt_ptr_t) meltsy_LIST_TO_MULTIPLE;
        } /*endgetnamedsym LIST_TO_MULTIPLE*/


    /*^getnamedsymbol*/
    /*getnamedsym:DISCR_MULTIPLE*/

    if (NULL == /*_._VALDATA___V44*/ meltfptr[43])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol DISCR_MULTIPLE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_DISCR_MULTIPLE = melthookproc_HOOK_NAMED_SYMBOL ("DISCR_MULTIPLE", (long) MELT_GET);
            /*_._VALDATA___V44*/ meltfptr[43] = (melt_ptr_t) meltsy_DISCR_MULTIPLE;
        } /*endgetnamedsym DISCR_MULTIPLE*/


    /*^getnamedsymbol*/
    /*getnamedsym:EXPAND_TUPLE_SLICE_AS_TUPLE*/

    if (NULL == /*_._VALDATA___V46*/ meltfptr[45])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol EXPAND_TUPLE_SLICE_AS_TUPLE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_EXPAND_TUPLE_SLICE_AS_TUPLE = melthookproc_HOOK_NAMED_SYMBOL ("EXPAND_TUPLE_SLICE_AS_TUPLE", (long) MELT_GET);
            /*_._VALDATA___V46*/ meltfptr[45] = (melt_ptr_t) meltsy_EXPAND_TUPLE_SLICE_AS_TUPLE;
        } /*endgetnamedsym EXPAND_TUPLE_SLICE_AS_TUPLE*/


    /*^getnamedsymbol*/
    /*getnamedsym:CLASS_SYMBOL*/

    if (NULL == /*_._VALDATA___V48*/ meltfptr[47])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_SYMBOL without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_CLASS_SYMBOL = melthookproc_HOOK_NAMED_SYMBOL ("CLASS_SYMBOL", (long) MELT_GET);
            /*_._VALDATA___V48*/ meltfptr[47] = (melt_ptr_t) meltsy_CLASS_SYMBOL;
        } /*endgetnamedsym CLASS_SYMBOL*/


    /*^getnamedsymbol*/
    /*getnamedsym:CLONE_SYMBOL*/

    if (NULL == /*_._VALDATA___V50*/ meltfptr[49])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLONE_SYMBOL without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_CLONE_SYMBOL = melthookproc_HOOK_NAMED_SYMBOL ("CLONE_SYMBOL", (long) MELT_GET);
            /*_._VALDATA___V50*/ meltfptr[49] = (melt_ptr_t) meltsy_CLONE_SYMBOL;
        } /*endgetnamedsym CLONE_SYMBOL*/


    /*^getnamedsymbol*/
    /*getnamedsym:CURPAIRINLISTSYMB*/

    if (NULL == /*_._VALDATA___V27*/ meltfptr[26])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CURPAIRINLISTSYMB without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_CURPAIRINLISTSYMB = melthookproc_HOOK_NAMED_SYMBOL ("CURPAIRINLISTSYMB", (long) MELT_GET);
            /*_._VALDATA___V27*/ meltfptr[26] = (melt_ptr_t) meltsy_CURPAIRINLISTSYMB;
        } /*endgetnamedsym CURPAIRINLISTSYMB*/


    /*^getnamedsymbol*/
    /*getnamedsym:MELT_MAKE_SEXPR*/

    if (NULL == /*_._VALDATA___V52*/ meltfptr[51])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MELT_MAKE_SEXPR without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_MELT_MAKE_SEXPR = melthookproc_HOOK_NAMED_SYMBOL ("MELT_MAKE_SEXPR", (long) MELT_GET);
            /*_._VALDATA___V52*/ meltfptr[51] = (melt_ptr_t) meltsy_MELT_MAKE_SEXPR;
        } /*endgetnamedsym MELT_MAKE_SEXPR*/


    /*^getnamedsymbol*/
    /*getnamedsym:HOOK_MELT_MAKE_LOCATION*/

    if (NULL == /*_._VALDATA___V54*/ meltfptr[53])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol HOOK_MELT_MAKE_LOCATION without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_HOOK_MELT_MAKE_LOCATION = melthookproc_HOOK_NAMED_SYMBOL ("HOOK_MELT_MAKE_LOCATION", (long) MELT_GET);
            /*_._VALDATA___V54*/ meltfptr[53] = (melt_ptr_t) meltsy_HOOK_MELT_MAKE_LOCATION;
        } /*endgetnamedsym HOOK_MELT_MAKE_LOCATION*/


    /*^getnamedsymbol*/
    /*getnamedsym:FOREACH_PAIR_COMPONENT_IN_LIST*/

    if (NULL == /*_._VALDATA___V29*/ meltfptr[28])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol FOREACH_PAIR_COMPONENT_IN_LIST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_FOREACH_PAIR_COMPONENT_IN_LIST = melthookproc_HOOK_NAMED_SYMBOL ("FOREACH_PAIR_COMPONENT_IN_LIST", (long) MELT_GET);
            /*_._VALDATA___V29*/ meltfptr[28] = (melt_ptr_t) meltsy_FOREACH_PAIR_COMPONENT_IN_LIST;
        } /*endgetnamedsym FOREACH_PAIR_COMPONENT_IN_LIST*/


    /*^getnamedsymbol*/
    /*getnamedsym:EACH_COMPONENT_IN_LIST*/

    if (NULL == /*_._VALDATA___V56*/ meltfptr[55])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol EACH_COMPONENT_IN_LIST without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_EACH_COMPONENT_IN_LIST = melthookproc_HOOK_NAMED_SYMBOL ("EACH_COMPONENT_IN_LIST", (long) MELT_GET);
            /*_._VALDATA___V56*/ meltfptr[55] = (melt_ptr_t) meltsy_EACH_COMPONENT_IN_LIST;
        } /*endgetnamedsym EACH_COMPONENT_IN_LIST*/


    /*^getnamedsymbol*/
    /*getnamedsym:CLASS_ENVIRONMENT*/

    if (NULL == /*_._VALDATA___V64*/ meltfptr[63])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CLASS_ENVIRONMENT without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_CLASS_ENVIRONMENT = melthookproc_HOOK_NAMED_SYMBOL ("CLASS_ENVIRONMENT", (long) MELT_GET);
            /*_._VALDATA___V64*/ meltfptr[63] = (melt_ptr_t) meltsy_CLASS_ENVIRONMENT;
        } /*endgetnamedsym CLASS_ENVIRONMENT*/


    /*^getnamedsymbol*/
    /*getnamedsym:EXPAND_PAIRLIST_AS_TUPLE*/

    if (NULL == /*_._VALDATA___V66*/ meltfptr[65])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol EXPAND_PAIRLIST_AS_TUPLE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_EXPAND_PAIRLIST_AS_TUPLE = melthookproc_HOOK_NAMED_SYMBOL ("EXPAND_PAIRLIST_AS_TUPLE", (long) MELT_GET);
            /*_._VALDATA___V66*/ meltfptr[65] = (melt_ptr_t) meltsy_EXPAND_PAIRLIST_AS_TUPLE;
        } /*endgetnamedsym EXPAND_PAIRLIST_AS_TUPLE*/


    /*^getnamedkeyword*/
    /*getnamedkeyw:TRUE*/
    {
        melt_ptr_t kw_TRUE = melthookproc_HOOK_NAMED_KEYWORD ("TRUE", (long) MELT_GET);
        if (kw_TRUE) /*_._VALDATA___V61*/ meltfptr[60] = (melt_ptr_t) kw_TRUE;
    }

    /*^getnamedsymbol*/
    /*getnamedsym:INSTALL_INITIAL_MACRO*/

    if (NULL == /*_._VALDATA___V68*/ meltfptr[67])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol INSTALL_INITIAL_MACRO without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_INSTALL_INITIAL_MACRO = melthookproc_HOOK_NAMED_SYMBOL ("INSTALL_INITIAL_MACRO", (long) MELT_GET);
            /*_._VALDATA___V68*/ meltfptr[67] = (melt_ptr_t) meltsy_INSTALL_INITIAL_MACRO;
        } /*endgetnamedsym INSTALL_INITIAL_MACRO*/


    /*^getnamedsymbol*/
    /*getnamedsym:GCCIF*/

    if (NULL == /*_._VALDATA___V70*/ meltfptr[69])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol GCCIF without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_GCCIF = melthookproc_HOOK_NAMED_SYMBOL ("GCCIF", (long) MELT_GET);
            /*_._VALDATA___V70*/ meltfptr[69] = (melt_ptr_t) meltsy_GCCIF;
        } /*endgetnamedsym GCCIF*/


    /*^getnamedsymbol*/
    /*getnamedsym:MEXPAND_REFERENCE*/

    if (NULL == /*_._VALDATA___V74*/ meltfptr[73])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol MEXPAND_REFERENCE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_MEXPAND_REFERENCE = melthookproc_HOOK_NAMED_SYMBOL ("MEXPAND_REFERENCE", (long) MELT_GET);
            /*_._VALDATA___V74*/ meltfptr[73] = (melt_ptr_t) meltsy_MEXPAND_REFERENCE;
        } /*endgetnamedsym MEXPAND_REFERENCE*/


    /*^getnamedsymbol*/
    /*getnamedsym:PATEXPAND_REFERENCE*/

    if (NULL == /*_._VALDATA___V78*/ meltfptr[77])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol PATEXPAND_REFERENCE without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_PATEXPAND_REFERENCE = melthookproc_HOOK_NAMED_SYMBOL ("PATEXPAND_REFERENCE", (long) MELT_GET);
            /*_._VALDATA___V78*/ meltfptr[77] = (melt_ptr_t) meltsy_PATEXPAND_REFERENCE;
        } /*endgetnamedsym PATEXPAND_REFERENCE*/


    /*^getnamedsymbol*/
    /*getnamedsym:INSTALL_INITIAL_PATMACRO*/

    if (NULL == /*_._VALDATA___V80*/ meltfptr[79])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol INSTALL_INITIAL_PATMACRO without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_INSTALL_INITIAL_PATMACRO = melthookproc_HOOK_NAMED_SYMBOL ("INSTALL_INITIAL_PATMACRO", (long) MELT_GET);
            /*_._VALDATA___V80*/ meltfptr[79] = (melt_ptr_t) meltsy_INSTALL_INITIAL_PATMACRO;
        } /*endgetnamedsym INSTALL_INITIAL_PATMACRO*/


    /*^getnamedsymbol*/
    /*getnamedsym:CONTAINER*/

    if (NULL == /*_._VALDATA___V82*/ meltfptr[81])
        {
#if !MELT_HAS_INITIAL_ENVIRONMENT
#error MELT getting named symbol CONTAINER without initial environment
#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/
            melt_ptr_t meltsy_CONTAINER = melthookproc_HOOK_NAMED_SYMBOL ("CONTAINER", (long) MELT_GET);
            /*_._VALDATA___V82*/ meltfptr[81] = (melt_ptr_t) meltsy_CONTAINER;
        } /*endgetnamedsym CONTAINER*/


    /*^comment*/
    /**COMMENT: before getting imported values thru hook **/;

    MELT_LOCATION("warmelt-moremacro.melt:3:/ compute");
    /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V5*/ meltfptr[4] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:45:/ compute");
    /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "MELT_DEBUG_FUN",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    /*^compute*/
    /*_.DEBUG_LESS__V7*/ meltfptr[6] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "DEBUG_LESS",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:47:/ compute");
    /*_.CLASS_LOCATED__V8*/ meltfptr[7] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "CLASS_LOCATED",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:48:/ compute");
    /*_.CLASS_SEXPR__V9*/ meltfptr[8] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "CLASS_SEXPR",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:51:/ compute");
    /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "MELT_ASSERT_FAILURE_FUN",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:53:/ compute");
    /*_.LIST_TO_MULTIPLE__V11*/ meltfptr[10] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "LIST_TO_MULTIPLE",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    /*^compute*/
    /*_.DISCR_MULTIPLE__V12*/ meltfptr[11] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "DISCR_MULTIPLE",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:80:/ compute");
    /*_.EXPAND_TUPLE_SLICE_AS_TUPLE__V13*/ meltfptr[12] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "EXPAND_TUPLE_SLICE_AS_TUPLE",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:87:/ compute");
    /*_.CLASS_SYMBOL__V14*/ meltfptr[13] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "CLASS_SYMBOL",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:91:/ compute");
    /*_.CLONE_SYMBOL__V15*/ meltfptr[14] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "CLONE_SYMBOL",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:93:/ compute");
    /*_.MELT_MAKE_SEXPR__V16*/ meltfptr[15] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "MELT_MAKE_SEXPR",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    /*^compute*/
    /*_.HOOK_MELT_MAKE_LOCATION__V17*/ meltfptr[16] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "HOOK_MELT_MAKE_LOCATION",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:129:/ compute");
    /*_.CLASS_ENVIRONMENT__V18*/ meltfptr[17] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "CLASS_ENVIRONMENT",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:148:/ compute");
    /*_.EXPAND_PAIRLIST_AS_TUPLE__V19*/ meltfptr[18] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "EXPAND_PAIRLIST_AS_TUPLE",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:181:/ compute");
    /*_.INSTALL_INITIAL_MACRO__V20*/ meltfptr[19] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "INSTALL_INITIAL_MACRO",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:196:/ compute");
    /*_.MEXPAND_REFERENCE__V21*/ meltfptr[20] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "MEXPAND_REFERENCE",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:203:/ compute");
    /*_.PATEXPAND_REFERENCE__V22*/ meltfptr[21] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "PATEXPAND_REFERENCE",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    MELT_LOCATION("warmelt-moremacro.melt:205:/ compute");
    /*_.INSTALL_INITIAL_PATMACRO__V23*/ meltfptr[22] =
        /*hookcall*/ melthook_HOOK_SYMBOL_IMPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_SYMBOL_IMPORTER))),  "INSTALL_INITIAL_PATMACRO",  "warmelt-moremacro", /*_._PREVENV___V4*/ meltfptr[3]) ;
    /**COMMENT: after getting imported values **/;

    /**initial routine cdata initializer**/

    memset(meltpredefinited, 0, sizeof(meltpredefinited));
    melt_initialize_module_meltdata_WARMELTmiMOREMACRO (&meltfram__, meltpredefinited);
    /**initial routine body**/

    debugeprintf (" in initialization of warmelt-moremacro thru 2 chunk routines with frame @%p of %d bytes",
                  /*fromline 2274*/ (void*)&meltfram__, (int) sizeof(meltfram__));
    melt_debuggc_eprintf ("initialization of warmelt-moremacro thru 2 chunks, frame @%p of %d bytes",
                          (void*)&meltfram__, (int) sizeof(meltfram__));

    meltmod__WARMELTmiMOREMACRO__initialmeltchunk_0 (&meltfram__, meltpredefinited);
    meltmod__WARMELTmiMOREMACRO__initialmeltchunk_1 (&meltfram__, meltpredefinited);;
    goto meltlabend_rout;
meltlabend_rout:
    ;
    {
        /* return out of initial fromline 2306 */ melt_ptr_t retval = /*_._RETINIT___V1*/ meltfptr[0];
        debugeprintf ("returning @%p from initial routine of warmelt-moremacro", (void*)retval); /*fromline 2314*/
        return retval;
    }
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR

} /* end fromline 2327 */

void meltmod__WARMELTmiMOREMACRO__initialmeltchunk_0 (meltinitial_frame_t* meltframptr__, char meltpredefinited[])
{
#define meltfram__ (*meltframptr__)
#undef meltcallcount
#define meltcallcount 0L
    (void) meltpredefinited;
    /**COMMENT: start of oinibody **/;

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V24*/ meltfptr[23])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_1__HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V24*/ meltfptr[23]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V24*/ meltfptr[23]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V25*/ meltfptr[24]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V24*/ meltfptr[23]);


    melt_dbgtrace_written_object (/*_._VALDATA___V24*/ meltfptr[23], "new static instance");

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#0", NULL != (/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[0] = (melt_ptr_t)(/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#1", NULL != (/*_.DEBUG_LESS__V7*/ meltfptr[6]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[1] = (melt_ptr_t)(/*_.DEBUG_LESS__V7*/ meltfptr[6]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#2", NULL != (/*_.CLASS_LOCATED__V8*/ meltfptr[7]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[2] = (melt_ptr_t)(/*_.CLASS_LOCATED__V8*/ meltfptr[7]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#3", NULL != (/*_.CLASS_SEXPR__V9*/ meltfptr[8]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[3] = (melt_ptr_t)(/*_.CLASS_SEXPR__V9*/ meltfptr[8]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#4", NULL != (/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[4] = (melt_ptr_t)(/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#5", NULL != (/*_.LIST_TO_MULTIPLE__V11*/ meltfptr[10]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[5] = (melt_ptr_t)(/*_.LIST_TO_MULTIPLE__V11*/ meltfptr[10]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#6", NULL != (/*_.DISCR_MULTIPLE__V12*/ meltfptr[11]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[6] = (melt_ptr_t)(/*_.DISCR_MULTIPLE__V12*/ meltfptr[11]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#7", NULL != (/*_.EXPAND_TUPLE_SLICE_AS_TUPLE__V13*/ meltfptr[12]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[7] = (melt_ptr_t)(/*_.EXPAND_TUPLE_SLICE_AS_TUPLE__V13*/ meltfptr[12]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#8", NULL != (/*_.CLASS_SYMBOL__V14*/ meltfptr[13]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[8] = (melt_ptr_t)(/*_.CLASS_SYMBOL__V14*/ meltfptr[13]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#9", NULL != (/*_.CLONE_SYMBOL__V15*/ meltfptr[14]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[9] = (melt_ptr_t)(/*_.CLONE_SYMBOL__V15*/ meltfptr[14]);

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_25__CURPAIRINLISTSYMB @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V27*/ meltfptr[26])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_25__CURPAIRINLISTSYMB @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V27*/ meltfptr[26]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V27*/ meltfptr[26]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V28*/ meltfptr[27]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V27*/ meltfptr[26]);


    melt_dbgtrace_written_object (/*_._VALDATA___V27*/ meltfptr[26], "new static instance");

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#10", NULL != (/*_._VALDATA___V27*/ meltfptr[26]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[10] = (melt_ptr_t)(/*_._VALDATA___V27*/ meltfptr[26]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#11", NULL != (/*_.MELT_MAKE_SEXPR__V16*/ meltfptr[15]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[11] = (melt_ptr_t)(/*_.MELT_MAKE_SEXPR__V16*/ meltfptr[15]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#12", NULL != (/*_.HOOK_MELT_MAKE_LOCATION__V17*/ meltfptr[16]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[12] = (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V17*/ meltfptr[16]);

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V29*/ meltfptr[28])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_31__FOREACH_PAIR_COMPONENT_IN_LIST @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V29*/ meltfptr[28]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V29*/ meltfptr[28]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V30*/ meltfptr[29]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V29*/ meltfptr[28]);


    melt_dbgtrace_written_object (/*_._VALDATA___V29*/ meltfptr[28], "new static instance");

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_3__EACH_COMPONENT_IN_LIST#13", NULL != (/*_._VALDATA___V29*/ meltfptr[28]));
    ((meltroutine_ptr_t)/*_._VALROUT___V26*/ meltfptr[25])->tabval[13] = (melt_ptr_t)(/*_._VALDATA___V29*/ meltfptr[28]);

    MELT_LOCATION("warmelt-moremacro.melt:39:/ initchunk");
    /*^touch*/
    /*touch:_IROUTVAL_*/
    meltgc_touch(/*_._VALROUT___V26*/ meltfptr[25]);

    /*putclosurout#1*/
    melt_assertmsg("putclosrout#1 checkclo", melt_magic_discr((melt_ptr_t)(/*_._VALCLO___V31*/ meltfptr[30])) == MELTOBMAG_CLOSURE);
    melt_assertmsg("putclosrout#1 checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V26*/ meltfptr[25])) == MELTOBMAG_ROUTINE);
    ((meltclosure_ptr_t)/*_._VALCLO___V31*/ meltfptr[30])->rout = (meltroutine_ptr_t) (/*_._VALROUT___V26*/ meltfptr[25]);

    /*touch:_DATACLOSURE_*/
    meltgc_touch(/*_._VALCLO___V31*/ meltfptr[30]);

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_5__MELT_DEBUG_FUN @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V32*/ meltfptr[31])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_5__MELT_DEBUG_FUN @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V32*/ meltfptr[31]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V32*/ meltfptr[31]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V33*/ meltfptr[32]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V32*/ meltfptr[31]);


    melt_dbgtrace_written_object (/*_._VALDATA___V32*/ meltfptr[31], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_7__DEBUG_LESS @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_7__DEBUG_LESS @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V34*/ meltfptr[33]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V34*/ meltfptr[33]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V35*/ meltfptr[34]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V34*/ meltfptr[33]);


    melt_dbgtrace_written_object (/*_._VALDATA___V34*/ meltfptr[33], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_9__CLASS_LOCATED @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_9__CLASS_LOCATED @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V36*/ meltfptr[35]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V36*/ meltfptr[35]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V37*/ meltfptr[36]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V36*/ meltfptr[35]);


    melt_dbgtrace_written_object (/*_._VALDATA___V36*/ meltfptr[35], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_11__CLASS_SEXPR @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V38*/ meltfptr[37])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_11__CLASS_SEXPR @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V38*/ meltfptr[37]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V38*/ meltfptr[37]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V39*/ meltfptr[38]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V38*/ meltfptr[37]);


    melt_dbgtrace_written_object (/*_._VALDATA___V38*/ meltfptr[37], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_13__MELT_ASSERT_FAILURE_FUN @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V40*/ meltfptr[39])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_13__MELT_ASSERT_FAILURE_FUN @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V40*/ meltfptr[39]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V40*/ meltfptr[39]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V41*/ meltfptr[40]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V40*/ meltfptr[39]);


    melt_dbgtrace_written_object (/*_._VALDATA___V40*/ meltfptr[39], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_15__LIST_TO_MULTIPLE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_15__LIST_TO_MULTIPLE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V42*/ meltfptr[41]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V42*/ meltfptr[41]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V43*/ meltfptr[42]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V42*/ meltfptr[41]);


    melt_dbgtrace_written_object (/*_._VALDATA___V42*/ meltfptr[41], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_17__DISCR_MULTIPLE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V44*/ meltfptr[43])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_17__DISCR_MULTIPLE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V44*/ meltfptr[43]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V44*/ meltfptr[43]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V45*/ meltfptr[44]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V44*/ meltfptr[43]);


    melt_dbgtrace_written_object (/*_._VALDATA___V44*/ meltfptr[43], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V46*/ meltfptr[45])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_19__EXPAND_TUPLE_SLICE_AS_TUPLE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V46*/ meltfptr[45]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V46*/ meltfptr[45]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V47*/ meltfptr[46]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V46*/ meltfptr[45]);


    melt_dbgtrace_written_object (/*_._VALDATA___V46*/ meltfptr[45], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_21__CLASS_SYMBOL @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V48*/ meltfptr[47])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_21__CLASS_SYMBOL @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V48*/ meltfptr[47]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V48*/ meltfptr[47]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V49*/ meltfptr[48]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V48*/ meltfptr[47]);


    melt_dbgtrace_written_object (/*_._VALDATA___V48*/ meltfptr[47], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_23__CLONE_SYMBOL @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V50*/ meltfptr[49])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_23__CLONE_SYMBOL @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V50*/ meltfptr[49]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V50*/ meltfptr[49]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V51*/ meltfptr[50]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V50*/ meltfptr[49]);


    melt_dbgtrace_written_object (/*_._VALDATA___V50*/ meltfptr[49], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_27__MELT_MAKE_SEXPR @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V52*/ meltfptr[51])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_27__MELT_MAKE_SEXPR @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V52*/ meltfptr[51]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V52*/ meltfptr[51]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V53*/ meltfptr[52]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V52*/ meltfptr[51]);


    melt_dbgtrace_written_object (/*_._VALDATA___V52*/ meltfptr[51], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_29__HOOK_MELT_MAKE_LOCATION @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V54*/ meltfptr[53])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_29__HOOK_MELT_MAKE_LOCATION @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V54*/ meltfptr[53]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V54*/ meltfptr[53]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V55*/ meltfptr[54]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V54*/ meltfptr[53]);


    melt_dbgtrace_written_object (/*_._VALDATA___V54*/ meltfptr[53], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_33__EACH_COMPONENT_IN_LIST @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_33__EACH_COMPONENT_IN_LIST @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V56*/ meltfptr[55]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V56*/ meltfptr[55]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V57*/ meltfptr[56]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V56*/ meltfptr[55]);


    melt_dbgtrace_written_object (/*_._VALDATA___V56*/ meltfptr[55], "new static instance");

    MELT_LOCATION("warmelt-moremacro.melt:112:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V58*/ meltfptr[57])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_35__FILTERGCCVERSION#0", NULL != (/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]));
    ((meltroutine_ptr_t)/*_._VALROUT___V58*/ meltfptr[57])->tabval[0] = (melt_ptr_t)(/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]);

    MELT_LOCATION("warmelt-moremacro.melt:112:/ initchunk");
    /*^touch*/
    /*touch:_IROUTVAL_*/
    meltgc_touch(/*_._VALROUT___V58*/ meltfptr[57]);

    /*putclosurout#2*/
    melt_assertmsg("putclosrout#2 checkclo", melt_magic_discr((melt_ptr_t)(/*_._VALCLO___V59*/ meltfptr[58])) == MELTOBMAG_CLOSURE);
    melt_assertmsg("putclosrout#2 checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V58*/ meltfptr[57])) == MELTOBMAG_ROUTINE);
    ((meltclosure_ptr_t)/*_._VALCLO___V59*/ meltfptr[58])->rout = (meltroutine_ptr_t) (/*_._VALROUT___V58*/ meltfptr[57]);

    /*touch:_DATACLOSURE_*/
    meltgc_touch(/*_._VALCLO___V59*/ meltfptr[58]);

    MELT_LOCATION("warmelt-moremacro.melt:127:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V60*/ meltfptr[59])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_37__MEXPAND_GCCIF#0", NULL != (/*_.CLASS_SEXPR__V9*/ meltfptr[8]));
    ((meltroutine_ptr_t)/*_._VALROUT___V60*/ meltfptr[59])->tabval[0] = (melt_ptr_t)(/*_.CLASS_SEXPR__V9*/ meltfptr[8]);

    MELT_LOCATION("warmelt-moremacro.melt:127:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V60*/ meltfptr[59])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_37__MEXPAND_GCCIF#1", NULL != (/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]));
    ((meltroutine_ptr_t)/*_._VALROUT___V60*/ meltfptr[59])->tabval[1] = (melt_ptr_t)(/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]);

    MELT_LOCATION("warmelt-moremacro.melt:127:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V60*/ meltfptr[59])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_37__MEXPAND_GCCIF#2", NULL != (/*_.CLASS_ENVIRONMENT__V18*/ meltfptr[17]));
    ((meltroutine_ptr_t)/*_._VALROUT___V60*/ meltfptr[59])->tabval[2] = (melt_ptr_t)(/*_.CLASS_ENVIRONMENT__V18*/ meltfptr[17]);

    MELT_LOCATION("warmelt-moremacro.melt:127:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V60*/ meltfptr[59])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_37__MEXPAND_GCCIF#3", NULL != (/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]));
    ((meltroutine_ptr_t)/*_._VALROUT___V60*/ meltfptr[59])->tabval[3] = (melt_ptr_t)(/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]);

    MELT_LOCATION("warmelt-moremacro.melt:127:/ initchunk");
    /*^putroutconstnotnull*/
    /*putroutconstnotnull*/
    melt_assertmsg("putroutconstnotnull checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V60*/ meltfptr[59])) == MELTOBMAG_ROUTINE);
    melt_assertmsg("putroutconstnotnull notnullconst", NULL != /*_._VALCLO___V59*/ meltfptr[58]);
    ((meltroutine_ptr_t)/*_._VALROUT___V60*/ meltfptr[59])->tabval[4] = (melt_ptr_t)(/*_._VALCLO___V59*/ meltfptr[58]);

    MELT_LOCATION("warmelt-moremacro.melt:127:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V60*/ meltfptr[59])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_37__MEXPAND_GCCIF#5", NULL != (/*_.EXPAND_PAIRLIST_AS_TUPLE__V19*/ meltfptr[18]));
    ((meltroutine_ptr_t)/*_._VALROUT___V60*/ meltfptr[59])->tabval[5] = (melt_ptr_t)(/*_.EXPAND_PAIRLIST_AS_TUPLE__V19*/ meltfptr[18]);

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_43__TRUE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V61*/ meltfptr[60])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_43__TRUE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V61*/ meltfptr[60]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V61*/ meltfptr[60]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V62*/ meltfptr[61]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V61*/ meltfptr[60]);


    melt_dbgtrace_written_object (/*_._VALDATA___V61*/ meltfptr[60], "new static instance");

    MELT_LOCATION("warmelt-moremacro.melt:127:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V60*/ meltfptr[59])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_37__MEXPAND_GCCIF#6", NULL != (/*_._VALDATA___V61*/ meltfptr[60]));
    ((meltroutine_ptr_t)/*_._VALROUT___V60*/ meltfptr[59])->tabval[6] = (melt_ptr_t)(/*_._VALDATA___V61*/ meltfptr[60]);

    MELT_LOCATION("warmelt-moremacro.melt:127:/ initchunk");
    /*^touch*/
    /*touch:_IROUTVAL_*/
    meltgc_touch(/*_._VALROUT___V60*/ meltfptr[59]);

    /*putclosurout#3*/
    melt_assertmsg("putclosrout#3 checkclo", melt_magic_discr((melt_ptr_t)(/*_._VALCLO___V63*/ meltfptr[62])) == MELTOBMAG_CLOSURE);
    melt_assertmsg("putclosrout#3 checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V60*/ meltfptr[59])) == MELTOBMAG_ROUTINE);
    ((meltclosure_ptr_t)/*_._VALCLO___V63*/ meltfptr[62])->rout = (meltroutine_ptr_t) (/*_._VALROUT___V60*/ meltfptr[59]);

    /*touch:_DATACLOSURE_*/
    meltgc_touch(/*_._VALCLO___V63*/ meltfptr[62]);

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_39__CLASS_ENVIRONMENT @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V64*/ meltfptr[63])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_39__CLASS_ENVIRONMENT @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V64*/ meltfptr[63]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V64*/ meltfptr[63]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V65*/ meltfptr[64]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V64*/ meltfptr[63]);


    melt_dbgtrace_written_object (/*_._VALDATA___V64*/ meltfptr[63], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_41__EXPAND_PAIRLIST_AS_TUPLE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V66*/ meltfptr[65])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_41__EXPAND_PAIRLIST_AS_TUPLE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V66*/ meltfptr[65]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V66*/ meltfptr[65]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V67*/ meltfptr[66]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V66*/ meltfptr[65]);


    melt_dbgtrace_written_object (/*_._VALDATA___V66*/ meltfptr[65], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_45__INSTALL_INITIAL_MACRO @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V68*/ meltfptr[67])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_45__INSTALL_INITIAL_MACRO @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V68*/ meltfptr[67]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V68*/ meltfptr[67]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V69*/ meltfptr[68]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V68*/ meltfptr[67]);


    melt_dbgtrace_written_object (/*_._VALDATA___V68*/ meltfptr[67], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_47__GCCIF @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V70*/ meltfptr[69])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_47__GCCIF @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V70*/ meltfptr[69]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V70*/ meltfptr[69]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V71*/ meltfptr[70]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V70*/ meltfptr[69]);


    melt_dbgtrace_written_object (/*_._VALDATA___V70*/ meltfptr[69], "new static instance");

    MELT_LOCATION("warmelt-moremacro.melt:191:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V72*/ meltfptr[71])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_49__MEXPANDOBSOLETE_CONTAINER#0", NULL != (/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]));
    ((meltroutine_ptr_t)/*_._VALROUT___V72*/ meltfptr[71])->tabval[0] = (melt_ptr_t)(/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]);

    MELT_LOCATION("warmelt-moremacro.melt:191:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V72*/ meltfptr[71])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_49__MEXPANDOBSOLETE_CONTAINER#1", NULL != (/*_.CLASS_SEXPR__V9*/ meltfptr[8]));
    ((meltroutine_ptr_t)/*_._VALROUT___V72*/ meltfptr[71])->tabval[1] = (melt_ptr_t)(/*_.CLASS_SEXPR__V9*/ meltfptr[8]);

    MELT_LOCATION("warmelt-moremacro.melt:191:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V72*/ meltfptr[71])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_49__MEXPANDOBSOLETE_CONTAINER#2", NULL != (/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]));
    ((meltroutine_ptr_t)/*_._VALROUT___V72*/ meltfptr[71])->tabval[2] = (melt_ptr_t)(/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]);

    MELT_LOCATION("warmelt-moremacro.melt:191:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V72*/ meltfptr[71])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_49__MEXPANDOBSOLETE_CONTAINER#3", NULL != (/*_.CLASS_LOCATED__V8*/ meltfptr[7]));
    ((meltroutine_ptr_t)/*_._VALROUT___V72*/ meltfptr[71])->tabval[3] = (melt_ptr_t)(/*_.CLASS_LOCATED__V8*/ meltfptr[7]);

    MELT_LOCATION("warmelt-moremacro.melt:191:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V72*/ meltfptr[71])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_49__MEXPANDOBSOLETE_CONTAINER#4", NULL != (/*_.MEXPAND_REFERENCE__V21*/ meltfptr[20]));
    ((meltroutine_ptr_t)/*_._VALROUT___V72*/ meltfptr[71])->tabval[4] = (melt_ptr_t)(/*_.MEXPAND_REFERENCE__V21*/ meltfptr[20]);

    MELT_LOCATION("warmelt-moremacro.melt:191:/ initchunk");
    /*^touch*/
    /*touch:_IROUTVAL_*/
    meltgc_touch(/*_._VALROUT___V72*/ meltfptr[71]);

    /*putclosurout#4*/
    melt_assertmsg("putclosrout#4 checkclo", melt_magic_discr((melt_ptr_t)(/*_._VALCLO___V73*/ meltfptr[72])) == MELTOBMAG_CLOSURE);
    melt_assertmsg("putclosrout#4 checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V72*/ meltfptr[71])) == MELTOBMAG_ROUTINE);
    ((meltclosure_ptr_t)/*_._VALCLO___V73*/ meltfptr[72])->rout = (meltroutine_ptr_t) (/*_._VALROUT___V72*/ meltfptr[71]);

    /*touch:_DATACLOSURE_*/
    meltgc_touch(/*_._VALCLO___V73*/ meltfptr[72]);

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_51__MEXPAND_REFERENCE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V74*/ meltfptr[73])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_51__MEXPAND_REFERENCE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V74*/ meltfptr[73]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V74*/ meltfptr[73]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V75*/ meltfptr[74]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V74*/ meltfptr[73]);


    melt_dbgtrace_written_object (/*_._VALDATA___V74*/ meltfptr[73], "new static instance");

    MELT_LOCATION("warmelt-moremacro.melt:198:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V76*/ meltfptr[75])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_53__PATEXPANDOBSOLETE_CONTAINER#0", NULL != (/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]));
    ((meltroutine_ptr_t)/*_._VALROUT___V76*/ meltfptr[75])->tabval[0] = (melt_ptr_t)(/*_.MELT_DEBUG_FUN__V6*/ meltfptr[5]);

    MELT_LOCATION("warmelt-moremacro.melt:198:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V76*/ meltfptr[75])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_53__PATEXPANDOBSOLETE_CONTAINER#1", NULL != (/*_.CLASS_SEXPR__V9*/ meltfptr[8]));
    ((meltroutine_ptr_t)/*_._VALROUT___V76*/ meltfptr[75])->tabval[1] = (melt_ptr_t)(/*_.CLASS_SEXPR__V9*/ meltfptr[8]);

    MELT_LOCATION("warmelt-moremacro.melt:198:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V76*/ meltfptr[75])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_53__PATEXPANDOBSOLETE_CONTAINER#2", NULL != (/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]));
    ((meltroutine_ptr_t)/*_._VALROUT___V76*/ meltfptr[75])->tabval[2] = (melt_ptr_t)(/*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9]);

    MELT_LOCATION("warmelt-moremacro.melt:198:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V76*/ meltfptr[75])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_53__PATEXPANDOBSOLETE_CONTAINER#3", NULL != (/*_.CLASS_LOCATED__V8*/ meltfptr[7]));
    ((meltroutine_ptr_t)/*_._VALROUT___V76*/ meltfptr[75])->tabval[3] = (melt_ptr_t)(/*_.CLASS_LOCATED__V8*/ meltfptr[7]);

    MELT_LOCATION("warmelt-moremacro.melt:198:/ initchunk");
    /*^putroutconst*/
    /*putroutconst*/
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg("putroutconst checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V76*/ meltfptr[75])) == MELTOBMAG_ROUTINE);
    if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg("putroutconst constnull.drout_53__PATEXPANDOBSOLETE_CONTAINER#4", NULL != (/*_.PATEXPAND_REFERENCE__V22*/ meltfptr[21]));
    ((meltroutine_ptr_t)/*_._VALROUT___V76*/ meltfptr[75])->tabval[4] = (melt_ptr_t)(/*_.PATEXPAND_REFERENCE__V22*/ meltfptr[21]);

    MELT_LOCATION("warmelt-moremacro.melt:198:/ initchunk");
    /*^touch*/
    /*touch:_IROUTVAL_*/
    meltgc_touch(/*_._VALROUT___V76*/ meltfptr[75]);

    /*putclosurout#5*/
    melt_assertmsg("putclosrout#5 checkclo", melt_magic_discr((melt_ptr_t)(/*_._VALCLO___V77*/ meltfptr[76])) == MELTOBMAG_CLOSURE);
    melt_assertmsg("putclosrout#5 checkrout", melt_magic_discr((melt_ptr_t)(/*_._VALROUT___V76*/ meltfptr[75])) == MELTOBMAG_ROUTINE);
    ((meltclosure_ptr_t)/*_._VALCLO___V77*/ meltfptr[76])->rout = (meltroutine_ptr_t) (/*_._VALROUT___V76*/ meltfptr[75]);

    /*touch:_DATACLOSURE_*/
    meltgc_touch(/*_._VALCLO___V77*/ meltfptr[76]);

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_55__PATEXPAND_REFERENCE @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V78*/ meltfptr[77])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_55__PATEXPAND_REFERENCE @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V78*/ meltfptr[77]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V78*/ meltfptr[77]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V79*/ meltfptr[78]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V78*/ meltfptr[77]);


    melt_dbgtrace_written_object (/*_._VALDATA___V78*/ meltfptr[77], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_57__INSTALL_INITIAL_PATMACRO @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V80*/ meltfptr[79])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_57__INSTALL_INITIAL_PATMACRO @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V80*/ meltfptr[79]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V80*/ meltfptr[79]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V81*/ meltfptr[80]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V80*/ meltfptr[79]);


    melt_dbgtrace_written_object (/*_._VALDATA___V80*/ meltfptr[79], "new static instance");

    /*putslot*/
    melt_assertmsg("checkobj putslot  ~dsym_59__CONTAINER @#1", melt_magic_discr((melt_ptr_t)(/*_._VALDATA___V82*/ meltfptr[81])) == MELTOBMAG_OBJECT);

    melt_assertmsg("checkoff putslot  ~dsym_59__CONTAINER @#1", (1>=0 && 1< melt_object_length((melt_ptr_t)(/*_._VALDATA___V82*/ meltfptr[81]))));
    ((meltobject_ptr_t)(/*_._VALDATA___V82*/ meltfptr[81]))->obj_vartab[1] = (melt_ptr_t)(
                /*_._VALSTR___V83*/ meltfptr[82]);

    /*touch:_VALUEDATA_*/
    meltgc_touch(/*_._VALDATA___V82*/ meltfptr[81]);


    melt_dbgtrace_written_object (/*_._VALDATA___V82*/ meltfptr[81], "new static instance");

    /**COMMENT: before toplevel body **/;

    MELT_LOCATION("warmelt-moremacro.melt:3:/ initchunk");
    /*^block*/
    /*anyblock*/
    {

        /*^cond*/
        /*cond*/ if (
            /*quasi.cur.mod.env.ref cur.mod.env.cont : at very start*/
            /* check quasi.cur.mod.env.ref 480689242 */
            (/*_._CONTENV___V2*/ meltfptr[1])
        ) /*then*/
            {
                /*^cond.then*/
                /*_.OR___V84*/ meltfptr[83] =
                    /*quasi.cur.mod.env.ref cur.mod.env.cont : at very start*/
                    /* check quasi.cur.mod.env.ref 480689242 */
                    (/*_._CONTENV___V2*/ meltfptr[1])
                    ;;
            }
        else
            {
                MELT_LOCATION("warmelt-moremacro.melt:3:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V85*/ meltfptr[84] =
                        /*hookcall*/ melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER ((melt_ptr_t) /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V5*/ meltfptr[4], /*_._PREVENV___V4*/ meltfptr[3],  "warmelt-moremacro") ;;
                    /*^compute*/
                    /*_.OR___V84*/ meltfptr[83] = /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V85*/ meltfptr[84];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-moremacro.melt:3:/ clear");
                    /*clear*/ /*_.HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER__V85*/ meltfptr[84] = 0 ;
                }
                ;
            }
        ;
        /*^block*/
        /*com.block:upd.cur.mod.env.cont : at very start*/
        {
        }/*com.end block:upd.cur.mod.env.cont : at very start*/
        ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.OR___V84*/ meltfptr[83] = 0 ;
    }

    MELT_LOCATION("warmelt-moremacro.melt:107:/ initchunk");
    /*^block*/
    /*anyblock*/
    {


        {
            /*hookcall*/ melthook_HOOK_MACRO_EXPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_MACRO_EXPORTER))), /*_._VALDATA___V56*/ meltfptr[55], /*_._VALCLO___V31*/ meltfptr[30],
                    /*quasi.cur.mod.env.ref norm.exp.val : EACH_COMPONENT_IN_LIST*/
                    /* check quasi.cur.mod.env.ref 225518753 */
                    (/*_._CONTENV___V2*/ meltfptr[1])
                                                      ) ;
        }
        ;
        /*epilog*/
    }

    MELT_LOCATION("warmelt-moremacro.melt:181:/ initchunk");
    /*^block*/
    /*anyblock*/
    {


        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_._VALCLO___V63*/ meltfptr[62];
            /*_.INSTALL_INITIAL_MACRO__V86*/ meltfptr[84] =  melt_apply ((meltclosure_ptr_t)(/*_.INSTALL_INITIAL_MACRO__V20*/ meltfptr[19]), (melt_ptr_t)(/*_._VALDATA___V70*/ meltfptr[69]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.INSTALL_INITIAL_MACRO__V86*/ meltfptr[84] = 0 ;
    }

    MELT_LOCATION("warmelt-moremacro.melt:182:/ initchunk");
    /*^block*/
    /*anyblock*/
    {


        {
            /*hookcall*/ melthook_HOOK_MACRO_EXPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_MACRO_EXPORTER))), /*_._VALDATA___V70*/ meltfptr[69], /*_._VALCLO___V63*/ meltfptr[62],
                    /*quasi.cur.mod.env.ref norm.exp.val : GCCIF*/
                    /* check quasi.cur.mod.env.ref 789065257 */
                    (/*_._CONTENV___V2*/ meltfptr[1])
                                                      ) ;
        }
        ;
        /*epilog*/
    }


#undef meltfram__
} /*end of meltmod__WARMELTmiMOREMACRO__initialmeltchunk_0*/

void meltmod__WARMELTmiMOREMACRO__initialmeltchunk_1 (meltinitial_frame_t* meltframptr__, char meltpredefinited[])
{
#define meltfram__ (*meltframptr__)
#undef meltcallcount
#define meltcallcount 0L
    (void) meltpredefinited;
    MELT_LOCATION("warmelt-moremacro.melt:205:/ initchunk");
    /*^block*/
    /*anyblock*/
    {


        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_._VALCLO___V77*/ meltfptr[76];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_._VALCLO___V73*/ meltfptr[72];
            /*_.INSTALL_INITIAL_PATMACRO__V87*/ meltfptr[83] =  melt_apply ((meltclosure_ptr_t)(/*_.INSTALL_INITIAL_PATMACRO__V23*/ meltfptr[22]), (melt_ptr_t)(/*_._VALDATA___V82*/ meltfptr[81]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.INSTALL_INITIAL_PATMACRO__V87*/ meltfptr[83] = 0 ;
    }

    MELT_LOCATION("warmelt-moremacro.melt:206:/ initchunk");
    /*^block*/
    /*anyblock*/
    {


        {
            /*hookcall*/ melthook_HOOK_PATMACRO_EXPORTER ((melt_ptr_t) ((melt_ptr_t)(MELT_PREDEF(HOOK_PATMACRO_EXPORTER))), /*_._VALDATA___V82*/ meltfptr[81], /*_._VALCLO___V73*/ meltfptr[72], /*_._VALCLO___V77*/ meltfptr[76],
                    /*quasi.cur.mod.env.ref norm.exp.pat : CONTAINER*/
                    /* check quasi.cur.mod.env.ref 422025051 */
                    (/*_._CONTENV___V2*/ meltfptr[1])
                                                         ) ;
        }
        ;
        /*epilog*/
    }

    /**COMMENT: after toplevel body **/;

    /**COMMENT: compute boxloc again **/;

    /*cond*/ if (/*_._CONTENV___V2*/ meltfptr[1]) /*then*/
        {
        }
    else    /*^cond.else*/
        {

            /*^compute*/

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
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V24*/ meltfptr[23]));

    /*internsym:MELT_DEBUG_FUN*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V32*/ meltfptr[31]));

    /*internsym:DEBUG_LESS*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V34*/ meltfptr[33]));

    /*internsym:CLASS_LOCATED*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V36*/ meltfptr[35]));

    /*internsym:CLASS_SEXPR*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V38*/ meltfptr[37]));

    /*internsym:MELT_ASSERT_FAILURE_FUN*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V40*/ meltfptr[39]));

    /*internsym:LIST_TO_MULTIPLE*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V42*/ meltfptr[41]));

    /*internsym:DISCR_MULTIPLE*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V44*/ meltfptr[43]));

    /*internsym:EXPAND_TUPLE_SLICE_AS_TUPLE*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V46*/ meltfptr[45]));

    /*internsym:CLASS_SYMBOL*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V48*/ meltfptr[47]));

    /*internsym:CLONE_SYMBOL*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V50*/ meltfptr[49]));

    /*internsym:CURPAIRINLISTSYMB*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V27*/ meltfptr[26]));

    /*internsym:MELT_MAKE_SEXPR*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V52*/ meltfptr[51]));

    /*internsym:HOOK_MELT_MAKE_LOCATION*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V54*/ meltfptr[53]));

    /*internsym:FOREACH_PAIR_COMPONENT_IN_LIST*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V29*/ meltfptr[28]));

    /*internsym:EACH_COMPONENT_IN_LIST*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V56*/ meltfptr[55]));

    /*internsym:CLASS_ENVIRONMENT*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V64*/ meltfptr[63]));

    /*internsym:EXPAND_PAIRLIST_AS_TUPLE*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V66*/ meltfptr[65]));

    /*internkeyw:TRUE*/
    (void) melthookproc_HOOK_INTERN_KEYWORD ((melt_ptr_t)(/*_._VALDATA___V61*/ meltfptr[60]));

    /*internsym:INSTALL_INITIAL_MACRO*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V68*/ meltfptr[67]));

    /*internsym:GCCIF*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V70*/ meltfptr[69]));

    /*internsym:MEXPAND_REFERENCE*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V74*/ meltfptr[73]));

    /*internsym:PATEXPAND_REFERENCE*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V78*/ meltfptr[77]));

    /*internsym:INSTALL_INITIAL_PATMACRO*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V80*/ meltfptr[79]));

    /*internsym:CONTAINER*/
    (void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)(/*_._VALDATA___V82*/ meltfptr[81]));

    /**COMMENT: set retinit from boxloc **/;

    /*_._RETINIT___V1*/ meltfptr[0] =
        /* finalsetretinit */ melt_reference_value((melt_ptr_t)(/*_._CONTENV___V2*/ meltfptr[1]));
    /**COMMENT: end the initproc **/;


#undef meltfram__
} /*end of meltmod__WARMELTmiMOREMACRO__initialmeltchunk_1*/




/* exported 0 field offsets */

/* exported 0 class lengths */



/**** end of warmelt-moremacro ****/
