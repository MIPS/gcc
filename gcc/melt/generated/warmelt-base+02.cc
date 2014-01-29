/* GCC MELT GENERATED C++ FILE warmelt-base+02.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #2 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f2[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-base+02.cc declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-base ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2008 - 2014  Free Software Foundation, Inc.
    Contributed by Basile Starynkevitch <basile@starynkevitch.net>
               and Pierre Vittet  <piervit@pvittet.com>

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


/** ordinary MELT module meltbuild-sources/warmelt-base**/
#define MELT_HAS_INITIAL_ENVIRONMENT 1 /*usualmodule*/



class Melt_InitialClassyFrame_WARMELTmiBASE_h989351527; // forward declaration fromline 6566
typedef Melt_InitialClassyFrame_WARMELTmiBASE_h989351527 Melt_InitialFrame;
/**** no MELT module variables ****/

/*** 3 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #2 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 3 MELT called hook declarations ***/

/*** 2 extra MELT c-headers ***/


/** MELT extra c-header 1 : **/


/* C++ standard headers for multiple_sort */
#include <vector>
#include <algorithm>

/*** end of 2 extra MELT c-headers ***/


/** MELT extra c-header 2 : **/


/* A compare structure used in multiple_sort for std::stable_sort;
   it has to be a compilation-unit global type;
   it cannot be a struct inside multiple_sort;
   see http://stackoverflow.com/q/21201685/841108
   and ISO/IEC 14882 C++03 standard section 14.3.1
 */
class Melt_Sort_Compare_Index
{
    struct meltmultiple_st** melttup_ad;
    melt_ptr_t* meltcmp_ad;
public:
    Melt_Sort_Compare_Index (struct meltmultiple_st**tup_ad, melt_ptr_t* cmp_ad)
        : melttup_ad(tup_ad), meltcmp_ad(cmp_ad) {};
    ~Melt_Sort_Compare_Index()
    {
        meltcmp_ad=NULL;
        melttup_ad=NULL;
    };
    bool operator () (int meltleftix, int meltrightix)
    {
        return (bool)
               melthookproc_HOOK_SORT_COMPARE_LESS((*melttup_ad)->tabval[meltleftix],
                       (*melttup_ad)->tabval[meltrightix],
                       *meltcmp_ad);
    };
}; /* end class Melt_Sort_Compare_Index */

/*** end of 2 extra MELT c-headers ***/




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiBASE_plIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiBASE_miIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_3_WARMELTmiBASE_stIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiBASE_diIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiBASE_pcIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_MELT_MAKE_LOCATION**/

/** declaration of hook melthook_HOOK_MELT_MAKE_LOCATION */
MELT_EXTERN
melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION(melt_ptr_t melthookdatap,
        const char* meltinp0_FILENAME,
        long meltinp1_LINENO)
;



/** end of declaration for hook melthook_HOOK_MELT_MAKE_LOCATION**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_7_WARMELTmiBASE_LIST_NTH_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_13_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_SORT_COMPARE_LESS**/

/** declaration of hook melthook_HOOK_SORT_COMPARE_LESS */
MELT_EXTERN
long melthook_HOOK_SORT_COMPARE_LESS(melt_ptr_t melthookdatap,
                                     melt_ptr_t meltinp0_LEFT,
                                     melt_ptr_t meltinp1_RIGHT,
                                     melt_ptr_t meltinp2_CMP)
;



/** end of declaration for hook melthook_HOOK_SORT_COMPARE_LESS**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiBASE_MULTIPLE_SORT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_36_WARMELTmiBASE_LAMBDA_cl1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiBASE_LAMBDA_cl2(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiBASE_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/

// this MELT module warmelt-base does not claim GPL compatibility.

/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiBASE_h989351527 /*opaqueinitialclassy*/ meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiBASE__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiBASE__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiBASE__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiBASE__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_0 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_1 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_2 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_3 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_4 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_5 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_6 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_7 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_8 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_9 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_10 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_11 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_12 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_13 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_14 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_15 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_16 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_17 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_18 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_19 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_20 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_21 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_22 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_23 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_24 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_25 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_26 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_27 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_28 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_29 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_30 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_31 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_32 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_33 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_34 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_35 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_36 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_37 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_38 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_39 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_40 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_41 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_42 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_43 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_44 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_45 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_46 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_47 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_48 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_49 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_50 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_51 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_52 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_53 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_54 (meltinitial_frame_t*, char*);


/**** warmelt-base+02.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_COMPLETE_SEQUENCE_AS_LIST_ix = 0, variad_COMPLETE_SEQUENCE_AS_LIST_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_COMPLETE_SEQUENCE_AS_LIST_len)
#define melt_variadic_index variad_COMPLETE_SEQUENCE_AS_LIST_ix

    long current_blocklevel_signals_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1715 */

    /** start of frame for meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST// fromline 1531
        : public Melt_CallFrameWithValues<17>
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
        MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<17> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST), clos) {};
        MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST() //the constructor fromline 1605
            : Melt_CallFrameWithValues<17> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<17> (fil,lin, sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST)) {};
        MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<17> (fil,lin, sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST


    /** end of frame for meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1660**/

    /* end of frame for routine meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1719 */

    /* classy proc frame meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST */
    MeltFrame_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("COMPLETE_SEQUENCE_AS_LIST", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3023:/ getarg");
    /*_.SRC__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:3027:/ quasiblock");


        /*_.NEWLIST__V4*/
        meltfptr[3] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[0]))));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_MULTIPLE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:3029:/ cond");
        /*cond*/
        if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit1__EACHTUP */
                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.SRC__V2*/ meltfptr[1]);
                        for (/*_#IX__L2*/ meltfnum[1] = 0;
                                          (/*_#IX__L2*/ meltfnum[1] >= 0) && (/*_#IX__L2*/ meltfnum[1] <  meltcit1__EACHTUP_ln);
                                          /*_#IX__L2*/ meltfnum[1]++)
                            {
                                /*_.COMP__V5*/ meltfptr[4] = melt_multiple_nth((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]),  /*_#IX__L2*/ meltfnum[1]);




                                {
                                    MELT_LOCATION("warmelt-base.melt:3033:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.COMP__V5*/ meltfptr[4]));
                                }
                                ;
                                if (/*_#IX__L2*/ meltfnum[1]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-base.melt:3030:/ clear");
                        /*clear*/ /*_.COMP__V5*/
                        meltfptr[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#IX__L2*/
                        meltfnum[1] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:3029:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_LIST__L3*/
                    meltfnum[2] =
                        (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
                    MELT_LOCATION("warmelt-base.melt:3035:/ cond");
                    /*cond*/
                    if (/*_#IS_LIST__L3*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                {
                                    /* start foreach_pair_component_in_list meltcit2__EACHLIST */
                                    for (/*_.CURPAIR__V6*/ meltfptr[5] = melt_list_first( (melt_ptr_t)/*_.SRC__V2*/ meltfptr[1]);
                                                           melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[5]) == MELTOBMAG_PAIR;
                                                           /*_.CURPAIR__V6*/ meltfptr[5] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[5]))
                                        {
                                            /*_.CURCOMP__V7*/ meltfptr[6] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[5]);



                                            {
                                                MELT_LOCATION("warmelt-base.melt:3039:/ locexp");
                                                meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURCOMP__V7*/ meltfptr[6]));
                                            }
                                            ;
                                        } /* end foreach_pair_component_in_list meltcit2__EACHLIST */
                                    /*_.CURPAIR__V6*/ meltfptr[5] = NULL;
                                    /*_.CURCOMP__V7*/
                                    meltfptr[6] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:3036:/ clear");
                                    /*clear*/ /*_.CURPAIR__V6*/
                                    meltfptr[5] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURCOMP__V7*/
                                    meltfptr[6] = 0 ;
                                } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                ;
                                /*epilog*/
                            }
                            ;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-base.melt:3035:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-base.melt:3042:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]));
                                }
                                ;
                                MELT_LOCATION("warmelt-base.melt:3041:/ quasiblock");


                                /*epilog*/
                            }
                            ;
                        }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3029:/ clear");
                    /*clear*/ /*_#IS_LIST__L3*/
                    meltfnum[2] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3043:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_4:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:3045:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_COMPLETE_SEQUENCE_AS_LIST_ix == variad_COMPLETE_SEQUENCE_AS_LIST_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_COMPLETE_SEQUENCE_AS_LIST_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:3047:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V9*/
                            meltfptr[8] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_ARGLOOP_4;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:3045:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            /*^cond*/
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_COMPLETE_SEQUENCE_AS_LIST_ix>=0 && variad_COMPLETE_SEQUENCE_AS_LIST_ix + 1 <= variad_COMPLETE_SEQUENCE_AS_LIST_len && meltxargdescr_[variad_COMPLETE_SEQUENCE_AS_LIST_ix]== MELTBPAR_PTR) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_.V__V10*/ meltfptr[9] =
                                            /*variadic argument value*/ ((meltxargtab_[variad_COMPLETE_SEQUENCE_AS_LIST_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_COMPLETE_SEQUENCE_AS_LIST_ix + 0].meltbp_aptr)) : NULL);;
                                        /*^compute*/

                                        /*consume variadic Value !*/
                                        variad_COMPLETE_SEQUENCE_AS_LIST_ix += 1;;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:3049:/ locexp");
                                            meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.V__V10*/ meltfptr[9]));
                                        }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3045:/ clear");
                                        /*clear*/ /*_.V__V10*/
                                        meltfptr[9] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        MELT_LOCATION("warmelt-base.melt:3051:/ quasiblock");


                                        /*_.VCTY__V12*/
                                        meltfptr[11] =
                                            /*variadic_type_code*/
#ifdef melt_variadic_index
                                            (((melt_variadic_index + 0) >= 0
                                              && (melt_variadic_index + 0) < melt_variadic_length)
                                             ?  melt_code_to_ctype (meltxargdescr_[melt_variadic_index + 0]
                                                                    & MELT_ARGDESCR_MAX)
                                             : NULL)
#else
                                            NULL /* no variadic_ctype outside of variadic functions */
#endif /*melt_variadic_index*/
                                            ;;
                                        MELT_LOCATION("warmelt-base.melt:3054:/ cond");
                                        /*cond*/
                                        if (
                                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V12*/ meltfptr[11]),
                                                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[1])))
                                        ) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^getslot*/
                                                {
                                                    melt_ptr_t slot=NULL, obj=NULL;
                                                    obj = (melt_ptr_t)(/*_.VCTY__V12*/ meltfptr[11]) /*=obj*/;
                                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                    /*_.NAMED_NAME__V13*/
                                                    meltfptr[12] = slot;
                                                };
                                                ;
                                            }
                                        else    /*^cond.else*/
                                            {

                                                /*_.NAMED_NAME__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
                                            }
                                        ;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:3053:/ locexp");
                                            error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "COMPLETE_SEQUENCE_AS_LIST with unsupported ctype"),
                                                   melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V13*/ meltfptr[12])));
                                        }
                                        ;

#if MELT_HAVE_DEBUG
                                        MELT_LOCATION("warmelt-base.melt:3055:/ cppif.then");
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
                                                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-base.melt:3055:/ cond.else");

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
                                                            argtab[0].meltbp_cstring =  "invalid variadic argument to COMPLETE_SEQUENCE_AS_LIST";
                                                            /*^apply.arg*/
                                                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                            /*^apply.arg*/
                                                            argtab[2].meltbp_long = 3055;
                                                            /*^apply.arg*/
                                                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V12*/ meltfptr[11];
                                                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IFELSE___V15*/
                                                        meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:3055:/ clear");
                                                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                        meltfptr[15] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_.IFCPP___V14*/
                                            meltfptr[13] = /*_._IFELSE___V15*/ meltfptr[14];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3055:/ clear");
                                            /*clear*/ /*_._IFELSE___V15*/
                                            meltfptr[14] = 0 ;
                                        }

#else /*MELT_HAVE_DEBUG*/
                                        /*^cppif.else*/
                                        /*_.IFCPP___V14*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                        ;
                                        /*^compute*/
                                        /*_.LET___V11*/
                                        meltfptr[9] = /*_.IFCPP___V14*/ meltfptr[13];;

                                        MELT_LOCATION("warmelt-base.melt:3051:/ clear");
                                        /*clear*/ /*_.VCTY__V12*/
                                        meltfptr[11] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.NAMED_NAME__V13*/
                                        meltfptr[12] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.IFCPP___V14*/
                                        meltfptr[13] = 0 ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3045:/ clear");
                                        /*clear*/ /*_.LET___V11*/
                                        meltfptr[9] = 0 ;
                                    }
                                    ;
                                }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                ;
                /*epilog*/
            }
            ;
            ;
            goto meltlabloop_ARGLOOP_4;
meltlabexit_ARGLOOP_4:
            ;
            MELT_LOCATION("warmelt-base.melt:3043:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V8*/
            meltfptr[7] = /*_.ARGLOOP__V9*/ meltfptr[8];;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3057:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.NEWLIST__V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-base.melt:3057:/ locexp");
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
        /*_.LET___V3*/
        meltfptr[2] = /*_.RETURN___V17*/ meltfptr[15];;

        MELT_LOCATION("warmelt-base.melt:3027:/ clear");
        /*clear*/ /*_.NEWLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V8*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V17*/
        meltfptr[15] = 0 ;
        MELT_LOCATION("warmelt-base.melt:3023:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3023:/ locexp");
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
    melt_trace_end("COMPLETE_SEQUENCE_AS_LIST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_29_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1715 */

    /** start of frame for meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET// fromline 1531
        : public Melt_CallFrameWithValues<12>
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
        MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET), clos) {};
        MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET() //the constructor fromline 1605
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET)) {};
        MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET


    /** end of frame for meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1660**/

    /* end of frame for routine meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1719 */

    /* classy proc frame meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET */
    MeltFrame_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MELT_PREDEFINED_GET", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3084:/ getarg");
    /*_.NAMV__V2*/
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
        /*_#IS_STRING__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-base.melt:3086:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.PREDEFSTR_CHK__V4*/ meltfptr[3] =
                        /*melt_predefined_get PREDEFSTR_CHK__1 */
                        melt_fetch_predefined (melt_predefined_index_by_name (melt_string_str ((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1])))) ;;
                    /*^compute*/
                    /*_._IFELSE___V3*/
                    meltfptr[2] = /*_.PREDEFSTR_CHK__V4*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3086:/ clear");
                    /*clear*/ /*_.PREDEFSTR_CHK__V4*/
                    meltfptr[3] = 0 ;
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
                    /*_#IS_A__L2*/
                    meltfnum[1] =
                        melt_is_instance_of((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])));;
                    MELT_LOCATION("warmelt-base.melt:3089:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-base.melt:3090:/ quasiblock");


                                /*^cond*/
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMSTR__V7*/
                                            meltfptr[6] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMSTR__V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_.PREDEFNAM_CHK__V8*/
                                meltfptr[7] =
                                    /*melt_predefined_get PREDEFNAM_CHK__1 */
                                    melt_fetch_predefined (melt_predefined_index_by_name (melt_string_str ((melt_ptr_t)(/*_.NAMSTR__V7*/ meltfptr[6])))) ;;
                                /*^compute*/
                                /*_.LET___V6*/
                                meltfptr[5] = /*_.PREDEFNAM_CHK__V8*/ meltfptr[7];;

                                MELT_LOCATION("warmelt-base.melt:3090:/ clear");
                                /*clear*/ /*_.NAMSTR__V7*/
                                meltfptr[6] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PREDEFNAM_CHK__V8*/
                                meltfptr[7] = 0 ;
                                /*_._IFELSE___V5*/
                                meltfptr[3] = /*_.LET___V6*/ meltfptr[5];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3089:/ clear");
                                /*clear*/ /*_.LET___V6*/
                                meltfptr[5] = 0 ;
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
                                /*_#IS_INTEGERBOX__L3*/
                                meltfnum[2] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1])) == MELTOBMAG_INT);;
                                MELT_LOCATION("warmelt-base.melt:3095:/ cond");
                                /*cond*/
                                if (/*_#IS_INTEGERBOX__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3096:/ quasiblock");


                                            /*_#NUM__L4*/
                                            meltfnum[3] =
                                                (melt_get_int((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1])));;
                                            /*^compute*/
                                            /*_.PREDEFNAM_CHK__V11*/
                                            meltfptr[5] =
                                                /*melt_predefined_get PREDEFNAM_CHK__2 */
                                                melt_fetch_predefined (/*_#NUM__L4*/ meltfnum[3]) ;;
                                            /*^compute*/
                                            /*_.LET___V10*/
                                            meltfptr[7] = /*_.PREDEFNAM_CHK__V11*/ meltfptr[5];;

                                            MELT_LOCATION("warmelt-base.melt:3096:/ clear");
                                            /*clear*/ /*_#NUM__L4*/
                                            meltfnum[3] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PREDEFNAM_CHK__V11*/
                                            meltfptr[5] = 0 ;
                                            /*_._IFELSE___V9*/
                                            meltfptr[6] = /*_.LET___V10*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3095:/ clear");
                                            /*clear*/ /*_.LET___V10*/
                                            meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3100:/ quasiblock");


                                            /*_.PROGN___V12*/
                                            meltfptr[5] = (/*nil*/NULL);;
                                            /*^compute*/
                                            /*_._IFELSE___V9*/
                                            meltfptr[6] = /*_.PROGN___V12*/ meltfptr[5];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3095:/ clear");
                                            /*clear*/ /*_.PROGN___V12*/
                                            meltfptr[5] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V5*/
                                meltfptr[3] = /*_._IFELSE___V9*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3089:/ clear");
                                /*clear*/ /*_#IS_INTEGERBOX__L3*/
                                meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V9*/
                                meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V3*/
                    meltfptr[2] = /*_._IFELSE___V5*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3086:/ clear");
                    /*clear*/ /*_#IS_A__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V5*/
                    meltfptr[3] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3084:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3084:/ locexp");
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
        /*clear*/ /*_#IS_STRING__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MELT_PREDEFINED_GET", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_30_WARMELTmiBASE_MELT_PREDEFINED_GET*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_31_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_31_WARMELTmiBASE_SYMBOL_CNAME fromline 1715 */

    /** start of frame for meltrout_31_WARMELTmiBASE_SYMBOL_CNAME of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME// fromline 1531
        : public Melt_CallFrameWithValues<20>
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
        MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME), clos) {};
        MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME() //the constructor fromline 1605
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME)) {};
        MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME


    /** end of frame for meltrout_31_WARMELTmiBASE_SYMBOL_CNAME fromline 1660**/

    /* end of frame for routine meltrout_31_WARMELTmiBASE_SYMBOL_CNAME fromline 1719 */

    /* classy proc frame meltrout_31_WARMELTmiBASE_SYMBOL_CNAME */
    MeltFrame_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_31_WARMELTmiBASE_SYMBOL_CNAME fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("SYMBOL_CNAME", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3104:/ getarg");
    /*_.SY__V2*/
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
        /*_#IS_A__L1*/
        meltfnum[0] =
            melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-base.melt:3106:/ cond");
        /*cond*/
        if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:3107:/ quasiblock");


                    /*_.SBUF__V5*/
                    meltfptr[4] =
                        (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[1])), (const char*)0);;
                    MELT_LOCATION("warmelt-base.melt:3108:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]),
                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[2])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V6*/
                                meltfptr[5] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.NAMED_NAME__V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-base.melt:3108:/ locexp");
                        meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V5*/ meltfptr[4]),
                                                 melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V6*/ meltfptr[5])));
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:3109:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]),
                                                      (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[0])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]) /*=obj*/;
                                melt_object_get_field(slot,obj, 3, "CSYM_URANK");
                                /*_.CSYM_URANK__V7*/
                                meltfptr[6] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.CSYM_URANK__V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:3109:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "cl";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CSYM_URANK__V7*/ meltfptr[6];
                        /*_.ADD2OUT__V8*/
                        meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.SBUF__V5*/ meltfptr[4]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.STRBUF2STRING__V9*/
                    meltfptr[8] =
                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[4])), melt_strbuf_str((melt_ptr_t)(/*_.SBUF__V5*/ meltfptr[4]))));;
                    /*^compute*/
                    /*_.LET___V4*/
                    meltfptr[3] = /*_.STRBUF2STRING__V9*/ meltfptr[8];;

                    MELT_LOCATION("warmelt-base.melt:3107:/ clear");
                    /*clear*/ /*_.SBUF__V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.NAMED_NAME__V6*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.CSYM_URANK__V7*/
                    meltfptr[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V8*/
                    meltfptr[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.STRBUF2STRING__V9*/
                    meltfptr[8] = 0 ;
                    /*_._IFELSE___V3*/
                    meltfptr[2] = /*_.LET___V4*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3106:/ clear");
                    /*clear*/ /*_.LET___V4*/
                    meltfptr[3] = 0 ;
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
                    /*_#IS_A__L2*/
                    meltfnum[1] =
                        melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[5])));;
                    MELT_LOCATION("warmelt-base.melt:3112:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-base.melt:3113:/ quasiblock");


                                /*_.SBUF__V12*/
                                meltfptr[6] =
                                    (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[1])), (const char*)0);;
                                MELT_LOCATION("warmelt-base.melt:3114:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[2])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMED_NAME__V13*/
                                            meltfptr[7] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMED_NAME__V13*/ meltfptr[7] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-base.melt:3114:/ locexp");
                                    meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V12*/ meltfptr[6]),
                                                             melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V13*/ meltfptr[7])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-base.melt:3115:/ locexp");
                                    /*add2sbuf_strconst*/
                                    meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V12*/ meltfptr[6]), ( "kw"));
                                }
                                ;
                                /*_.STRBUF2STRING__V14*/
                                meltfptr[8] =
                                    (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[4])), melt_strbuf_str((melt_ptr_t)(/*_.SBUF__V12*/ meltfptr[6]))));;
                                /*^compute*/
                                /*_.LET___V11*/
                                meltfptr[5] = /*_.STRBUF2STRING__V14*/ meltfptr[8];;

                                MELT_LOCATION("warmelt-base.melt:3113:/ clear");
                                /*clear*/ /*_.SBUF__V12*/
                                meltfptr[6] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.NAMED_NAME__V13*/
                                meltfptr[7] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.STRBUF2STRING__V14*/
                                meltfptr[8] = 0 ;
                                /*_._IFELSE___V10*/
                                meltfptr[4] = /*_.LET___V11*/ meltfptr[5];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3112:/ clear");
                                /*clear*/ /*_.LET___V11*/
                                meltfptr[5] = 0 ;
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
                                /*_#IS_A__L3*/
                                meltfnum[2] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[6])));;
                                MELT_LOCATION("warmelt-base.melt:3118:/ cond");
                                /*cond*/
                                if (/*_#IS_A__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3119:/ quasiblock");


                                            /*_.SBUF__V17*/
                                            meltfptr[7] =
                                                (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[1])), (const char*)0);;
                                            MELT_LOCATION("warmelt-base.melt:3120:/ cond");
                                            /*cond*/
                                            if (
                                                /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]),
                                                                              (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[2])))
                                            ) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^getslot*/
                                                    {
                                                        melt_ptr_t slot=NULL, obj=NULL;
                                                        obj = (melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]) /*=obj*/;
                                                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                        /*_.NAMED_NAME__V18*/
                                                        meltfptr[8] = slot;
                                                    };
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*_.NAMED_NAME__V18*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                                                }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-base.melt:3120:/ locexp");
                                                meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V17*/ meltfptr[7]),
                                                                         melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V18*/ meltfptr[8])));
                                            }
                                            ;
                                            /*_.STRBUF2STRING__V19*/
                                            meltfptr[5] =
                                                (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[4])), melt_strbuf_str((melt_ptr_t)(/*_.SBUF__V17*/ meltfptr[7]))));;
                                            /*^compute*/
                                            /*_.LET___V16*/
                                            meltfptr[6] = /*_.STRBUF2STRING__V19*/ meltfptr[5];;

                                            MELT_LOCATION("warmelt-base.melt:3119:/ clear");
                                            /*clear*/ /*_.SBUF__V17*/
                                            meltfptr[7] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.NAMED_NAME__V18*/
                                            meltfptr[8] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.STRBUF2STRING__V19*/
                                            meltfptr[5] = 0 ;
                                            /*_._IFELSE___V15*/
                                            meltfptr[3] = /*_.LET___V16*/ meltfptr[6];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3118:/ clear");
                                            /*clear*/ /*_.LET___V16*/
                                            meltfptr[6] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3123:/ quasiblock");


                                            /*_.PROGN___V20*/
                                            meltfptr[7] = (/*nil*/NULL);;
                                            /*^compute*/
                                            /*_._IFELSE___V15*/
                                            meltfptr[3] = /*_.PROGN___V20*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3118:/ clear");
                                            /*clear*/ /*_.PROGN___V20*/
                                            meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V10*/
                                meltfptr[4] = /*_._IFELSE___V15*/ meltfptr[3];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3112:/ clear");
                                /*clear*/ /*_#IS_A__L3*/
                                meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V15*/
                                meltfptr[3] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V3*/
                    meltfptr[2] = /*_._IFELSE___V10*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3106:/ clear");
                    /*clear*/ /*_#IS_A__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V10*/
                    meltfptr[4] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3104:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3104:/ locexp");
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
        /*clear*/ /*_#IS_A__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("SYMBOL_CNAME", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_31_WARMELTmiBASE_SYMBOL_CNAME_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_31_WARMELTmiBASE_SYMBOL_CNAME*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_MELT_MAKE_SEXPR_ix = 0, variad_MELT_MAKE_SEXPR_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_MELT_MAKE_SEXPR_len)
#define melt_variadic_index variad_MELT_MAKE_SEXPR_ix

    long current_blocklevel_signals_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR fromline 1715 */

    /** start of frame for meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR// fromline 1531
        : public Melt_CallFrameWithValues<58>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[23];
        /*classy others*/
        const char* loc_CSTRING__o0;
        const char* loc_CSTRING__o1;
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR), clos) {};
        MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR() //the constructor fromline 1605
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR)) {};
        MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR


    /** end of frame for meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR fromline 1660**/

    /* end of frame for routine meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR fromline 1719 */

    /* classy proc frame meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR */
    MeltFrame_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MELT_MAKE_SEXPR", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3135:/ getarg");
    /*_.LOC__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:3139:/ quasiblock");


        /*_.CONT__V4*/
        meltfptr[3] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[0]))));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_MIXINT__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1])) == MELTOBMAG_MIXINT);;
        MELT_LOCATION("warmelt-base.melt:3140:/ cond");
        /*cond*/
        if (/*_#IS_MIXINT__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*_.VLOC__V5*/ meltfptr[4] = /*_.LOC__V2*/ meltfptr[1];;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:3140:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_MIXLOC__L2*/
                    meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1])) == MELTOBMAG_MIXLOC);;
                    MELT_LOCATION("warmelt-base.melt:3143:/ cond");
                    /*cond*/
                    if (/*_#IS_MIXLOC__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*_._IFELSE___V6*/ meltfptr[5] = /*_.LOC__V2*/ meltfptr[1];;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-base.melt:3143:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_A__L3*/
                                meltfnum[2] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[1])));;
                                MELT_LOCATION("warmelt-base.melt:3145:/ cond");
                                /*cond*/
                                if (/*_#IS_A__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3146:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                                                /*_.LOCA_LOCATION__V8*/
                                                meltfptr[7] = slot;
                                            };
                                            ;
                                            /*_._IFELSE___V7*/
                                            meltfptr[6] = /*_.LOCA_LOCATION__V8*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3145:/ clear");
                                            /*clear*/ /*_.LOCA_LOCATION__V8*/
                                            meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3147:/ quasiblock");


                                            /*_.PROGN___V9*/
                                            meltfptr[7] = (/*nil*/NULL);;
                                            /*^compute*/
                                            /*_._IFELSE___V7*/
                                            meltfptr[6] = /*_.PROGN___V9*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3145:/ clear");
                                            /*clear*/ /*_.PROGN___V9*/
                                            meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V6*/
                                meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3143:/ clear");
                                /*clear*/ /*_#IS_A__L3*/
                                meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V7*/
                                meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_.VLOC__V5*/
                    meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3140:/ clear");
                    /*clear*/ /*_#IS_MIXLOC__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V6*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3148:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[2])), (3), "CLASS_SEXPR");
            /*_.INST__V11*/
            meltfptr[6] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V11*/ meltfptr[6])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V11*/ meltfptr[6]), (1), (/*_.VLOC__V5*/ meltfptr[4]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SEXP_CONTENTS", melt_magic_discr((melt_ptr_t)(/*_.INST__V11*/ meltfptr[6])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V11*/ meltfptr[6]), (2), (/*_.CONT__V4*/ meltfptr[3]), "SEXP_CONTENTS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V11*/ meltfptr[6], "newly made instance");
        ;
        /*_.SEXPR__V10*/
        meltfptr[7] = /*_.INST__V11*/ meltfptr[6];;
        MELT_LOCATION("warmelt-base.melt:3152:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_5:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:3154:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_MELT_MAKE_SEXPR_ix == variad_MELT_MAKE_SEXPR_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_MELT_MAKE_SEXPR_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:3156:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V13*/
                            meltfptr[12] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_ARGLOOP_5;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:3154:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            /*^cond*/
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_MAKE_SEXPR_ix>=0 && variad_MELT_MAKE_SEXPR_ix + 1 <= variad_MELT_MAKE_SEXPR_len && meltxargdescr_[variad_MELT_MAKE_SEXPR_ix]== MELTBPAR_CSTRING) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_?*/ meltfram__.loc_CSTRING__o0 =
                                            /*variadic argument stuff*/ meltxargtab_[variad_MELT_MAKE_SEXPR_ix + 0].meltbp_cstring;;
                                        /*^compute*/

                                        /*consume variadic CSTRING !*/
                                        variad_MELT_MAKE_SEXPR_ix += 1;;
                                        MELT_LOCATION("warmelt-base.melt:3158:/ quasiblock");


                                        /*_.BS__V14*/
                                        meltfptr[13] =
                                            /*full constboxing*/ /*ctype_cstring boxing*/ meltgc_new_stringdup((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_STRING)))), (/*_?*/ meltfram__.loc_CSTRING__o0));;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:3160:/ locexp");
                                            meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.BS__V14*/ meltfptr[13]));
                                        }
                                        ;

                                        MELT_LOCATION("warmelt-base.melt:3158:/ clear");
                                        /*clear*/ /*_.BS__V14*/
                                        meltfptr[13] = 0 ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3154:/ clear");
                                        /*clear*/ /*_?*/
                                        meltfram__.loc_CSTRING__o0 = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^cond*/
                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_MAKE_SEXPR_ix>=0 && variad_MELT_MAKE_SEXPR_ix + 1 <= variad_MELT_MAKE_SEXPR_len && meltxargdescr_[variad_MELT_MAKE_SEXPR_ix]== MELTBPAR_LONG) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    /*^compute*/
                                                    /*_#NUM__L4*/ meltfnum[2] =
                                                        /*variadic argument stuff*/ meltxargtab_[variad_MELT_MAKE_SEXPR_ix + 0].meltbp_long;;
                                                    /*^compute*/

                                                    /*consume variadic LONG !*/
                                                    variad_MELT_MAKE_SEXPR_ix += 1;;
                                                    MELT_LOCATION("warmelt-base.melt:3162:/ quasiblock");


                                                    /*_.BS__V15*/
                                                    meltfptr[13] =
                                                        /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#NUM__L4*/ meltfnum[2]));;

                                                    {
                                                        MELT_LOCATION("warmelt-base.melt:3164:/ locexp");
                                                        meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.BS__V15*/ meltfptr[13]));
                                                    }
                                                    ;

                                                    MELT_LOCATION("warmelt-base.melt:3162:/ clear");
                                                    /*clear*/ /*_.BS__V15*/
                                                    meltfptr[13] = 0 ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:3154:/ clear");
                                                    /*clear*/ /*_#NUM__L4*/
                                                    meltfnum[2] = 0 ;
                                                }
                                                ;
                                            }
                                        else    /*^cond.else*/
                                            {

                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    /*^cond*/
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_MAKE_SEXPR_ix>=0 && variad_MELT_MAKE_SEXPR_ix + 1 <= variad_MELT_MAKE_SEXPR_len && meltxargdescr_[variad_MELT_MAKE_SEXPR_ix]== MELTBPAR_PTR) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_.V__V16*/ meltfptr[13] =
                                                                    /*variadic argument value*/ ((meltxargtab_[variad_MELT_MAKE_SEXPR_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_MELT_MAKE_SEXPR_ix + 0].meltbp_aptr)) : NULL);;
                                                                /*^compute*/

                                                                /*consume variadic Value !*/
                                                                variad_MELT_MAKE_SEXPR_ix += 1;;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                /*_#NULL__L5*/
                                                                meltfnum[1] =
                                                                    ((/*_.V__V16*/ meltfptr[13]) == NULL);;
                                                                MELT_LOCATION("warmelt-base.melt:3166:/ cond");
                                                                /*cond*/
                                                                if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {




                                                                            {
                                                                                MELT_LOCATION("warmelt-base.melt:3167:/ locexp");
                                                                                meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.V__V16*/ meltfptr[13]));
                                                                            }
                                                                            ;
                                                                            /*clear*/ /*_._IFELSE___V17*/
                                                                            meltfptr[16] = 0 ;
                                                                            /*epilog*/
                                                                        }
                                                                        ;
                                                                    }
                                                                else
                                                                    {
                                                                        MELT_LOCATION("warmelt-base.melt:3166:/ cond.else");

                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {


                                                                            MELT_CHECK_SIGNAL();
                                                                            ;
                                                                            /*_#IS_OBJECT__L6*/
                                                                            meltfnum[2] =
                                                                                (melt_magic_discr((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])) == MELTOBMAG_OBJECT);;
                                                                            MELT_LOCATION("warmelt-base.melt:3168:/ cond");
                                                                            /*cond*/
                                                                            if (/*_#IS_OBJECT__L6*/ meltfnum[2]) /*then*/
                                                                                {
                                                                                    /*^cond.then*/
                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {




                                                                                        {
                                                                                            MELT_LOCATION("warmelt-base.melt:3169:/ locexp");
                                                                                            meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.V__V16*/ meltfptr[13]));
                                                                                        }
                                                                                        ;
                                                                                        /*clear*/ /*_._IFELSE___V18*/
                                                                                        meltfptr[17] = 0 ;
                                                                                        /*epilog*/
                                                                                    }
                                                                                    ;
                                                                                }
                                                                            else
                                                                                {
                                                                                    MELT_LOCATION("warmelt-base.melt:3168:/ cond.else");

                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {


                                                                                        MELT_CHECK_SIGNAL();
                                                                                        ;
                                                                                        /*_#IS_INTEGERBOX__L7*/
                                                                                        meltfnum[6] =
                                                                                            (melt_magic_discr((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])) == MELTOBMAG_INT);;
                                                                                        MELT_LOCATION("warmelt-base.melt:3170:/ cond");
                                                                                        /*cond*/
                                                                                        if (/*_#IS_INTEGERBOX__L7*/ meltfnum[6]) /*then*/
                                                                                            {
                                                                                                /*^cond.then*/
                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {




                                                                                                    {
                                                                                                        MELT_LOCATION("warmelt-base.melt:3171:/ locexp");
                                                                                                        meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.V__V16*/ meltfptr[13]));
                                                                                                    }
                                                                                                    ;
                                                                                                    /*clear*/ /*_._IFELSE___V19*/
                                                                                                    meltfptr[18] = 0 ;
                                                                                                    /*epilog*/
                                                                                                }
                                                                                                ;
                                                                                            }
                                                                                        else
                                                                                            {
                                                                                                MELT_LOCATION("warmelt-base.melt:3170:/ cond.else");

                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {


                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                    ;
                                                                                                    /*_#IS_STRING__L8*/
                                                                                                    meltfnum[7] =
                                                                                                        (melt_magic_discr((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])) == MELTOBMAG_STRING);;
                                                                                                    MELT_LOCATION("warmelt-base.melt:3172:/ cond");
                                                                                                    /*cond*/
                                                                                                    if (/*_#IS_STRING__L8*/ meltfnum[7]) /*then*/
                                                                                                        {
                                                                                                            /*^cond.then*/
                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {




                                                                                                                {
                                                                                                                    MELT_LOCATION("warmelt-base.melt:3173:/ locexp");
                                                                                                                    meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.V__V16*/ meltfptr[13]));
                                                                                                                }
                                                                                                                ;
                                                                                                                /*clear*/ /*_._IFELSE___V20*/
                                                                                                                meltfptr[19] = 0 ;
                                                                                                                /*epilog*/
                                                                                                            }
                                                                                                            ;
                                                                                                        }
                                                                                                    else
                                                                                                        {
                                                                                                            MELT_LOCATION("warmelt-base.melt:3172:/ cond.else");

                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {


                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                ;
                                                                                                                /*_#IS_MULTIPLE__L9*/
                                                                                                                meltfnum[8] =
                                                                                                                    (melt_magic_discr((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])) == MELTOBMAG_MULTIPLE);;
                                                                                                                MELT_LOCATION("warmelt-base.melt:3174:/ cond");
                                                                                                                /*cond*/
                                                                                                                if (/*_#IS_MULTIPLE__L9*/ meltfnum[8]) /*then*/
                                                                                                                    {
                                                                                                                        /*^cond.then*/
                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {

                                                                                                                            /*citerblock FOREACH_IN_MULTIPLE*/
                                                                                                                            {
                                                                                                                                /* start foreach_in_multiple meltcit1__EACHTUP */
                                                                                                                                long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.V__V16*/ meltfptr[13]);
                                                                                                                                for (/*_#IX__L10*/ meltfnum[9] = 0;
                                                                                                                                                   (/*_#IX__L10*/ meltfnum[9] >= 0) && (/*_#IX__L10*/ meltfnum[9] <  meltcit1__EACHTUP_ln);
                                                                                                                                                   /*_#IX__L10*/ meltfnum[9]++)
                                                                                                                                    {
                                                                                                                                        /*_.COMP__V22*/ meltfptr[21] = melt_multiple_nth((melt_ptr_t)(/*_.V__V16*/ meltfptr[13]),  /*_#IX__L10*/ meltfnum[9]);




                                                                                                                                        {
                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3178:/ locexp");
                                                                                                                                            meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.COMP__V22*/ meltfptr[21]));
                                                                                                                                        }
                                                                                                                                        ;
                                                                                                                                        if (/*_#IX__L10*/ meltfnum[9]<0) break;
                                                                                                                                    } /* end  foreach_in_multiple meltcit1__EACHTUP */

                                                                                                                                /*citerepilog*/

                                                                                                                                MELT_LOCATION("warmelt-base.melt:3175:/ clear");
                                                                                                                                /*clear*/ /*_.COMP__V22*/
                                                                                                                                meltfptr[21] = 0 ;
                                                                                                                                /*^clear*/
                                                                                                                                /*clear*/ /*_#IX__L10*/
                                                                                                                                meltfnum[9] = 0 ;
                                                                                                                            } /*endciterblock FOREACH_IN_MULTIPLE*/
                                                                                                                            ;
                                                                                                                            /*epilog*/
                                                                                                                        }
                                                                                                                        ;
                                                                                                                    }
                                                                                                                else
                                                                                                                    {
                                                                                                                        MELT_LOCATION("warmelt-base.melt:3174:/ cond.else");

                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {


                                                                                                                            MELT_CHECK_SIGNAL();
                                                                                                                            ;
                                                                                                                            /*_#IS_LIST__L11*/
                                                                                                                            meltfnum[10] =
                                                                                                                                (melt_magic_discr((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])) == MELTOBMAG_LIST);;
                                                                                                                            MELT_LOCATION("warmelt-base.melt:3179:/ cond");
                                                                                                                            /*cond*/
                                                                                                                            if (/*_#IS_LIST__L11*/ meltfnum[10]) /*then*/
                                                                                                                                {
                                                                                                                                    /*^cond.then*/
                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {

                                                                                                                                        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                                                                                                                        {
                                                                                                                                            /* start foreach_pair_component_in_list meltcit2__EACHLIST */
                                                                                                                                            for (/*_.CURPAIR__V24*/ meltfptr[23] = melt_list_first( (melt_ptr_t)/*_.V__V16*/ meltfptr[13]);
                                                                                                                                                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V24*/ meltfptr[23]) == MELTOBMAG_PAIR;
                                                                                                                                                                    /*_.CURPAIR__V24*/ meltfptr[23] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V24*/ meltfptr[23]))
                                                                                                                                                {
                                                                                                                                                    /*_.CURCOMP__V25*/ meltfptr[24] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V24*/ meltfptr[23]);



                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:3183:/ locexp");
                                                                                                                                                        meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURCOMP__V25*/ meltfptr[24]));
                                                                                                                                                    }
                                                                                                                                                    ;
                                                                                                                                                } /* end foreach_pair_component_in_list meltcit2__EACHLIST */
                                                                                                                                            /*_.CURPAIR__V24*/ meltfptr[23] = NULL;
                                                                                                                                            /*_.CURCOMP__V25*/
                                                                                                                                            meltfptr[24] = NULL;


                                                                                                                                            /*citerepilog*/

                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3180:/ clear");
                                                                                                                                            /*clear*/ /*_.CURPAIR__V24*/
                                                                                                                                            meltfptr[23] = 0 ;
                                                                                                                                            /*^clear*/
                                                                                                                                            /*clear*/ /*_.CURCOMP__V25*/
                                                                                                                                            meltfptr[24] = 0 ;
                                                                                                                                        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                                                                                                                        ;
                                                                                                                                        /*epilog*/
                                                                                                                                    }
                                                                                                                                    ;
                                                                                                                                }
                                                                                                                            else
                                                                                                                                {
                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3179:/ cond.else");

                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {


                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                        ;
                                                                                                                                        /*_#IS_MIXLOC__L12*/
                                                                                                                                        meltfnum[11] =
                                                                                                                                            (melt_magic_discr((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])) == MELTOBMAG_MIXLOC);;
                                                                                                                                        MELT_LOCATION("warmelt-base.melt:3185:/ cond");
                                                                                                                                        /*cond*/
                                                                                                                                        if (/*_#IS_MIXLOC__L12*/ meltfnum[11]) /*then*/
                                                                                                                                            {
                                                                                                                                                /*^cond.then*/
                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3186:/ quasiblock");


                                                                                                                                                    /*_#LOCLINE__L13*/
                                                                                                                                                    meltfnum[12] =
                                                                                                                                                        (LOCATION_LINE(melt_location_mixloc((melt_ptr_t)/*_.V__V16*/ meltfptr[13])));;
                                                                                                                                                    /*^compute*/
                                                                                                                                                    /*_?*/
                                                                                                                                                    meltfram__.loc_CSTRING__o1 =
                                                                                                                                                        (LOCATION_FILE(melt_location_mixloc((melt_ptr_t)/*_.V__V16*/ meltfptr[13])));;

                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;

                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;
                                                                                                                                                    /*^compute*/
                                                                                                                                                    /*_#gtI__L14*/
                                                                                                                                                    meltfnum[13] =
                                                                                                                                                        ((/*_#LOCLINE__L13*/ meltfnum[12]) > (0));;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3189:/ cond");
                                                                                                                                                    /*cond*/
                                                                                                                                                    if (/*_#gtI__L14*/ meltfnum[13]) /*then*/
                                                                                                                                                        {
                                                                                                                                                            /*^cond.then*/
                                                                                                                                                            /*^block*/
                                                                                                                                                            /*anyblock*/
                                                                                                                                                            {

                                                                                                                                                                /*_#CSTRING_NON_EMPTY__L16*/ meltfnum[15] =
                                                                                                                                                                    (((/*_?*/ meltfram__.loc_CSTRING__o1) != NULL) && ((const char*)(/*_?*/ meltfram__.loc_CSTRING__o1))[0] != (char)0);;
                                                                                                                                                                /*^compute*/
                                                                                                                                                                /*_#_IF___L15*/
                                                                                                                                                                meltfnum[14] = /*_#CSTRING_NON_EMPTY__L16*/ meltfnum[15];;
                                                                                                                                                                /*epilog*/

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3189:/ clear");
                                                                                                                                                                /*clear*/ /*_#CSTRING_NON_EMPTY__L16*/
                                                                                                                                                                meltfnum[15] = 0 ;
                                                                                                                                                            }
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else    /*^cond.else*/
                                                                                                                                                        {

                                                                                                                                                            /*_#_IF___L15*/ meltfnum[14] = 0;;
                                                                                                                                                        }
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3189:/ cond");
                                                                                                                                                    /*cond*/
                                                                                                                                                    if (/*_#_IF___L15*/ meltfnum[14]) /*then*/
                                                                                                                                                        {
                                                                                                                                                            /*^cond.then*/
                                                                                                                                                            /*^block*/
                                                                                                                                                            /*anyblock*/
                                                                                                                                                            {


                                                                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                                                                ;
                                                                                                                                                                /*_.BOX__V27*/
                                                                                                                                                                meltfptr[26] =
                                                                                                                                                                    /*full constboxing*/ /*ctype_cstring boxing*/ meltgc_new_stringdup((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_STRING)))), (/*_?*/ meltfram__.loc_CSTRING__o1));;
                                                                                                                                                                /*^compute*/
                                                                                                                                                                /*_.BOX__V28*/
                                                                                                                                                                meltfptr[27] =
                                                                                                                                                                    /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#LOCLINE__L13*/ meltfnum[12]));;
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3193:/ blockmultialloc");
                                                                                                                                                                /*multiallocblock*/
                                                                                                                                                                {
                                                                                                                                                                    struct meltletrec_1_st
                                                                                                                                                                    {
                                                                                                                                                                        struct meltpair_st rpair_0___PAIROFLIST_x1;
                                                                                                                                                                        struct meltpair_st rpair_1___PAIROFLIST_x2;
                                                                                                                                                                        struct meltpair_st rpair_2___PAIROFLIST_x3;
                                                                                                                                                                        struct meltlist_st rlist_3__LIST_;
                                                                                                                                                                        long meltletrec_1_endgap;
                                                                                                                                                                    } *meltletrec_1_ptr = 0;
                                                                                                                                                                    meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
                                                                                                                                                                    /*^blockmultialloc.initfill*/
                                                                                                                                                                    /*inipair rpair_0___PAIROFLIST_x1*/
                                                                                                                                                                    /*_._PAIROFLIST__V30*/
                                                                                                                                                                    meltfptr[29] = (melt_ptr_t) &meltletrec_1_ptr->rpair_0___PAIROFLIST_x1;
                                                                                                                                                                    meltletrec_1_ptr->rpair_0___PAIROFLIST_x1.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

                                                                                                                                                                    /*inipair rpair_1___PAIROFLIST_x2*/
                                                                                                                                                                    /*_._PAIROFLIST__V31*/
                                                                                                                                                                    meltfptr[30] = (melt_ptr_t) &meltletrec_1_ptr->rpair_1___PAIROFLIST_x2;
                                                                                                                                                                    meltletrec_1_ptr->rpair_1___PAIROFLIST_x2.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

                                                                                                                                                                    /*inipair rpair_2___PAIROFLIST_x3*/
                                                                                                                                                                    /*_._PAIROFLIST__V32*/
                                                                                                                                                                    meltfptr[31] = (melt_ptr_t) &meltletrec_1_ptr->rpair_2___PAIROFLIST_x3;
                                                                                                                                                                    meltletrec_1_ptr->rpair_2___PAIROFLIST_x3.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

                                                                                                                                                                    /*inilist rlist_3__LIST_*/
                                                                                                                                                                    /*_.LIST___V33*/
                                                                                                                                                                    meltfptr[32] = (melt_ptr_t) &meltletrec_1_ptr->rlist_3__LIST_;
                                                                                                                                                                    meltletrec_1_ptr->rlist_3__LIST_.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_LIST))));



                                                                                                                                                                    /*^putpairhead*/
                                                                                                                                                                    /*putpairhead*/
                                                                                                                                                                    melt_assertmsg("putpairhead /1 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V30*/ meltfptr[29]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V30*/ meltfptr[29]))->hd = (melt_ptr_t) ((/*!konst_3_HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[3]));
                                                                                                                                                                    ;
                                                                                                                                                                    /*^putpairtail*/
                                                                                                                                                                    /*putpairtail*/
                                                                                                                                                                    melt_assertmsg("putpairtail /b8b550e checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V30*/ meltfptr[29]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V30*/ meltfptr[29]))->tl = (meltpair_ptr_t) (/*_._PAIROFLIST__V31*/ meltfptr[30]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^touch*/
                                                                                                                                                                    meltgc_touch(/*_._PAIROFLIST__V30*/ meltfptr[29]);
                                                                                                                                                                    ;
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3194:/ putpairhead");
                                                                                                                                                                    /*putpairhead*/
                                                                                                                                                                    melt_assertmsg("putpairhead /2 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V31*/ meltfptr[30]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V31*/ meltfptr[30]))->hd = (melt_ptr_t) (/*_.BOX__V27*/ meltfptr[26]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^putpairtail*/
                                                                                                                                                                    /*putpairtail*/
                                                                                                                                                                    melt_assertmsg("putpairtail /2bf76c90 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V31*/ meltfptr[30]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V31*/ meltfptr[30]))->tl = (meltpair_ptr_t) (/*_._PAIROFLIST__V32*/ meltfptr[31]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^touch*/
                                                                                                                                                                    meltgc_touch(/*_._PAIROFLIST__V31*/ meltfptr[30]);
                                                                                                                                                                    ;
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3195:/ putpairhead");
                                                                                                                                                                    /*putpairhead*/
                                                                                                                                                                    melt_assertmsg("putpairhead /3 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V32*/ meltfptr[31]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V32*/ meltfptr[31]))->hd = (melt_ptr_t) (/*_.BOX__V28*/ meltfptr[27]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^touch*/
                                                                                                                                                                    meltgc_touch(/*_._PAIROFLIST__V32*/ meltfptr[31]);
                                                                                                                                                                    ;
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3193:/ putlist");
                                                                                                                                                                    /*putlist*/
                                                                                                                                                                    melt_assertmsg("putlist checklist", melt_magic_discr((melt_ptr_t)(/*_.LIST___V33*/ meltfptr[32]))== MELTOBMAG_LIST);
                                                                                                                                                                    ((meltlist_ptr_t)(/*_.LIST___V33*/ meltfptr[32]))->first = (meltpair_ptr_t) (/*_._PAIROFLIST__V30*/ meltfptr[29]);
                                                                                                                                                                    ((meltlist_ptr_t)(/*_.LIST___V33*/ meltfptr[32]))->last = (meltpair_ptr_t) (/*_._PAIROFLIST__V32*/ meltfptr[31]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^touch*/
                                                                                                                                                                    meltgc_touch(/*_.LIST___V33*/ meltfptr[32]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*_.LIST___V29*/
                                                                                                                                                                    meltfptr[28] = /*_.LIST___V33*/ meltfptr[32];;
                                                                                                                                                                    /*epilog*/

                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3193:/ clear");
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V30*/
                                                                                                                                                                    meltfptr[29] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V31*/
                                                                                                                                                                    meltfptr[30] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V32*/
                                                                                                                                                                    meltfptr[31] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_.LIST___V33*/
                                                                                                                                                                    meltfptr[32] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V30*/
                                                                                                                                                                    meltfptr[29] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V31*/
                                                                                                                                                                    meltfptr[30] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V32*/
                                                                                                                                                                    meltfptr[31] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_.LIST___V33*/
                                                                                                                                                                    meltfptr[32] = 0 ;
                                                                                                                                                                } /*end multiallocblock*/
                                                                                                                                                                ;
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3191:/ quasiblock");


                                                                                                                                                                /*^rawallocobj*/
                                                                                                                                                                /*rawallocobj*/
                                                                                                                                                                {
                                                                                                                                                                    melt_ptr_t newobj = 0;
                                                                                                                                                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[2])), (3), "CLASS_SEXPR");
                                                                                                                                                                    /*_.INST__V35*/
                                                                                                                                                                    meltfptr[30] =
                                                                                                                                                                        newobj;
                                                                                                                                                                };
                                                                                                                                                                ;
                                                                                                                                                                /*^putslot*/
                                                                                                                                                                /*putslot*/
                                                                                                                                                                melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V35*/ meltfptr[30])) == MELTOBMAG_OBJECT);
                                                                                                                                                                melt_putfield_object((/*_.INST__V35*/ meltfptr[30]), (1), (/*_.LOC__V2*/ meltfptr[1]), "LOCA_LOCATION");
                                                                                                                                                                ;
                                                                                                                                                                /*^putslot*/
                                                                                                                                                                /*putslot*/
                                                                                                                                                                melt_assertmsg("checkobj putslot  _ @SEXP_CONTENTS", melt_magic_discr((melt_ptr_t)(/*_.INST__V35*/ meltfptr[30])) == MELTOBMAG_OBJECT);
                                                                                                                                                                melt_putfield_object((/*_.INST__V35*/ meltfptr[30]), (2), (/*_.LIST___V29*/ meltfptr[28]), "SEXP_CONTENTS");
                                                                                                                                                                ;
                                                                                                                                                                /*^touchobj*/

                                                                                                                                                                melt_dbgtrace_written_object (/*_.INST__V35*/ meltfptr[30], "newly made instance");
                                                                                                                                                                ;
                                                                                                                                                                /*_.INST___V34*/
                                                                                                                                                                meltfptr[29] = /*_.INST__V35*/ meltfptr[30];;



                                                                                                                                                                {
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3191:/ locexp");
                                                                                                                                                                    meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.INST___V34*/ meltfptr[29]));
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                                /*clear*/ /*_._IFELSE___V26*/
                                                                                                                                                                meltfptr[25] = 0 ;
                                                                                                                                                                /*epilog*/

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3189:/ clear");
                                                                                                                                                                /*clear*/ /*_.BOX__V27*/
                                                                                                                                                                meltfptr[26] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.BOX__V28*/
                                                                                                                                                                meltfptr[27] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.LIST___V29*/
                                                                                                                                                                meltfptr[28] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.INST___V34*/
                                                                                                                                                                meltfptr[29] = 0 ;
                                                                                                                                                            }
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else    /*^cond.else*/
                                                                                                                                                        {

                                                                                                                                                            /*^block*/
                                                                                                                                                            /*anyblock*/
                                                                                                                                                            {




                                                                                                                                                                {
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3196:/ locexp");
                                                                                                                                                                    meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)((/*nil*/NULL)));
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                                /*clear*/ /*_._IFELSE___V26*/
                                                                                                                                                                meltfptr[25] = 0 ;
                                                                                                                                                                /*epilog*/
                                                                                                                                                            }
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    ;

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3186:/ clear");
                                                                                                                                                    /*clear*/ /*_#LOCLINE__L13*/
                                                                                                                                                    meltfnum[12] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_?*/
                                                                                                                                                    meltfram__.loc_CSTRING__o1 = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_#gtI__L14*/
                                                                                                                                                    meltfnum[13] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_#_IF___L15*/
                                                                                                                                                    meltfnum[14] = 0 ;
                                                                                                                                                    /*epilog*/
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        else
                                                                                                                                            {
                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3185:/ cond.else");

                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {


                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;

                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;
                                                                                                                                                    /*_#IS_MIXINT__L17*/
                                                                                                                                                    meltfnum[15] =
                                                                                                                                                        (melt_magic_discr((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])) == MELTOBMAG_MIXINT);;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3198:/ cond");
                                                                                                                                                    /*cond*/
                                                                                                                                                    if (/*_#IS_MIXINT__L17*/ meltfnum[15]) /*then*/
                                                                                                                                                        {
                                                                                                                                                            /*^cond.then*/
                                                                                                                                                            /*^block*/
                                                                                                                                                            /*anyblock*/
                                                                                                                                                            {

                                                                                                                                                                /*_.MIXINT_VAL__V36*/ meltfptr[31] =
                                                                                                                                                                    (melt_val_mixint((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])));;
                                                                                                                                                                /*^compute*/
                                                                                                                                                                /*_#IS_STRING__L19*/
                                                                                                                                                                meltfnum[13] =
                                                                                                                                                                    (melt_magic_discr((melt_ptr_t)(/*_.MIXINT_VAL__V36*/ meltfptr[31])) == MELTOBMAG_STRING);;
                                                                                                                                                                /*^compute*/
                                                                                                                                                                /*_#_IF___L18*/
                                                                                                                                                                meltfnum[12] = /*_#IS_STRING__L19*/ meltfnum[13];;
                                                                                                                                                                /*epilog*/

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3198:/ clear");
                                                                                                                                                                /*clear*/ /*_.MIXINT_VAL__V36*/
                                                                                                                                                                meltfptr[31] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_#IS_STRING__L19*/
                                                                                                                                                                meltfnum[13] = 0 ;
                                                                                                                                                            }
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else    /*^cond.else*/
                                                                                                                                                        {

                                                                                                                                                            /*_#_IF___L18*/ meltfnum[12] = 0;;
                                                                                                                                                        }
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3198:/ cond");
                                                                                                                                                    /*cond*/
                                                                                                                                                    if (/*_#_IF___L18*/ meltfnum[12]) /*then*/
                                                                                                                                                        {
                                                                                                                                                            /*^cond.then*/
                                                                                                                                                            /*^block*/
                                                                                                                                                            /*anyblock*/
                                                                                                                                                            {

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3201:/ quasiblock");


                                                                                                                                                                /*_#LOCLINE__L20*/
                                                                                                                                                                meltfnum[14] =
                                                                                                                                                                    (melt_get_int((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])));;

                                                                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                                                                ;
                                                                                                                                                                /*^compute*/
                                                                                                                                                                /*_.MIXINT_VAL__V38*/
                                                                                                                                                                meltfptr[26] =
                                                                                                                                                                    (melt_val_mixint((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])));;
                                                                                                                                                                /*^compute*/
                                                                                                                                                                /*_.BOX__V39*/
                                                                                                                                                                meltfptr[27] =
                                                                                                                                                                    /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#LOCLINE__L20*/ meltfnum[14]));;
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3205:/ blockmultialloc");
                                                                                                                                                                /*multiallocblock*/
                                                                                                                                                                {
                                                                                                                                                                    struct meltletrec_2_st
                                                                                                                                                                    {
                                                                                                                                                                        struct meltpair_st rpair_0___PAIROFLIST_x4;
                                                                                                                                                                        struct meltpair_st rpair_1___PAIROFLIST_x5;
                                                                                                                                                                        struct meltpair_st rpair_2___PAIROFLIST_x6;
                                                                                                                                                                        struct meltlist_st rlist_3__LIST_;
                                                                                                                                                                        long meltletrec_2_endgap;
                                                                                                                                                                    } *meltletrec_2_ptr = 0;
                                                                                                                                                                    meltletrec_2_ptr = (struct meltletrec_2_st *) meltgc_allocate (sizeof (struct meltletrec_2_st), 0);
                                                                                                                                                                    /*^blockmultialloc.initfill*/
                                                                                                                                                                    /*inipair rpair_0___PAIROFLIST_x4*/
                                                                                                                                                                    /*_._PAIROFLIST__V41*/
                                                                                                                                                                    meltfptr[29] = (melt_ptr_t) &meltletrec_2_ptr->rpair_0___PAIROFLIST_x4;
                                                                                                                                                                    meltletrec_2_ptr->rpair_0___PAIROFLIST_x4.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

                                                                                                                                                                    /*inipair rpair_1___PAIROFLIST_x5*/
                                                                                                                                                                    /*_._PAIROFLIST__V42*/
                                                                                                                                                                    meltfptr[31] = (melt_ptr_t) &meltletrec_2_ptr->rpair_1___PAIROFLIST_x5;
                                                                                                                                                                    meltletrec_2_ptr->rpair_1___PAIROFLIST_x5.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

                                                                                                                                                                    /*inipair rpair_2___PAIROFLIST_x6*/
                                                                                                                                                                    /*_._PAIROFLIST__V43*/
                                                                                                                                                                    meltfptr[42] = (melt_ptr_t) &meltletrec_2_ptr->rpair_2___PAIROFLIST_x6;
                                                                                                                                                                    meltletrec_2_ptr->rpair_2___PAIROFLIST_x6.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

                                                                                                                                                                    /*inilist rlist_3__LIST_*/
                                                                                                                                                                    /*_.LIST___V44*/
                                                                                                                                                                    meltfptr[43] = (melt_ptr_t) &meltletrec_2_ptr->rlist_3__LIST_;
                                                                                                                                                                    meltletrec_2_ptr->rlist_3__LIST_.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_LIST))));



                                                                                                                                                                    /*^putpairhead*/
                                                                                                                                                                    /*putpairhead*/
                                                                                                                                                                    melt_assertmsg("putpairhead /4 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V41*/ meltfptr[29]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V41*/ meltfptr[29]))->hd = (melt_ptr_t) ((/*!konst_3_HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[3]));
                                                                                                                                                                    ;
                                                                                                                                                                    /*^putpairtail*/
                                                                                                                                                                    /*putpairtail*/
                                                                                                                                                                    melt_assertmsg("putpairtail /b5f743 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V41*/ meltfptr[29]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V41*/ meltfptr[29]))->tl = (meltpair_ptr_t) (/*_._PAIROFLIST__V42*/ meltfptr[31]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^touch*/
                                                                                                                                                                    meltgc_touch(/*_._PAIROFLIST__V41*/ meltfptr[29]);
                                                                                                                                                                    ;
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3206:/ putpairhead");
                                                                                                                                                                    /*putpairhead*/
                                                                                                                                                                    melt_assertmsg("putpairhead /5 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V42*/ meltfptr[31]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V42*/ meltfptr[31]))->hd = (melt_ptr_t) (/*_.MIXINT_VAL__V38*/ meltfptr[26]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^putpairtail*/
                                                                                                                                                                    /*putpairtail*/
                                                                                                                                                                    melt_assertmsg("putpairtail /b8869b8 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V42*/ meltfptr[31]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V42*/ meltfptr[31]))->tl = (meltpair_ptr_t) (/*_._PAIROFLIST__V43*/ meltfptr[42]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^touch*/
                                                                                                                                                                    meltgc_touch(/*_._PAIROFLIST__V42*/ meltfptr[31]);
                                                                                                                                                                    ;
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3207:/ putpairhead");
                                                                                                                                                                    /*putpairhead*/
                                                                                                                                                                    melt_assertmsg("putpairhead /6 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V43*/ meltfptr[42]))== MELTOBMAG_PAIR);
                                                                                                                                                                    ((meltpair_ptr_t)(/*_._PAIROFLIST__V43*/ meltfptr[42]))->hd = (melt_ptr_t) (/*_.BOX__V39*/ meltfptr[27]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^touch*/
                                                                                                                                                                    meltgc_touch(/*_._PAIROFLIST__V43*/ meltfptr[42]);
                                                                                                                                                                    ;
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3205:/ putlist");
                                                                                                                                                                    /*putlist*/
                                                                                                                                                                    melt_assertmsg("putlist checklist", melt_magic_discr((melt_ptr_t)(/*_.LIST___V44*/ meltfptr[43]))== MELTOBMAG_LIST);
                                                                                                                                                                    ((meltlist_ptr_t)(/*_.LIST___V44*/ meltfptr[43]))->first = (meltpair_ptr_t) (/*_._PAIROFLIST__V41*/ meltfptr[29]);
                                                                                                                                                                    ((meltlist_ptr_t)(/*_.LIST___V44*/ meltfptr[43]))->last = (meltpair_ptr_t) (/*_._PAIROFLIST__V43*/ meltfptr[42]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*^touch*/
                                                                                                                                                                    meltgc_touch(/*_.LIST___V44*/ meltfptr[43]);
                                                                                                                                                                    ;
                                                                                                                                                                    /*_.LIST___V40*/
                                                                                                                                                                    meltfptr[28] = /*_.LIST___V44*/ meltfptr[43];;
                                                                                                                                                                    /*epilog*/

                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3205:/ clear");
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V41*/
                                                                                                                                                                    meltfptr[29] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V42*/
                                                                                                                                                                    meltfptr[31] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V43*/
                                                                                                                                                                    meltfptr[42] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_.LIST___V44*/
                                                                                                                                                                    meltfptr[43] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V41*/
                                                                                                                                                                    meltfptr[29] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V42*/
                                                                                                                                                                    meltfptr[31] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_._PAIROFLIST__V43*/
                                                                                                                                                                    meltfptr[42] = 0 ;
                                                                                                                                                                    /*^clear*/
                                                                                                                                                                    /*clear*/ /*_.LIST___V44*/
                                                                                                                                                                    meltfptr[43] = 0 ;
                                                                                                                                                                } /*end multiallocblock*/
                                                                                                                                                                ;
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3203:/ quasiblock");


                                                                                                                                                                /*^rawallocobj*/
                                                                                                                                                                /*rawallocobj*/
                                                                                                                                                                {
                                                                                                                                                                    melt_ptr_t newobj = 0;
                                                                                                                                                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[2])), (3), "CLASS_SEXPR");
                                                                                                                                                                    /*_.INST__V46*/
                                                                                                                                                                    meltfptr[31] =
                                                                                                                                                                        newobj;
                                                                                                                                                                };
                                                                                                                                                                ;
                                                                                                                                                                /*^putslot*/
                                                                                                                                                                /*putslot*/
                                                                                                                                                                melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V46*/ meltfptr[31])) == MELTOBMAG_OBJECT);
                                                                                                                                                                melt_putfield_object((/*_.INST__V46*/ meltfptr[31]), (1), (/*_.LOC__V2*/ meltfptr[1]), "LOCA_LOCATION");
                                                                                                                                                                ;
                                                                                                                                                                /*^putslot*/
                                                                                                                                                                /*putslot*/
                                                                                                                                                                melt_assertmsg("checkobj putslot  _ @SEXP_CONTENTS", melt_magic_discr((melt_ptr_t)(/*_.INST__V46*/ meltfptr[31])) == MELTOBMAG_OBJECT);
                                                                                                                                                                melt_putfield_object((/*_.INST__V46*/ meltfptr[31]), (2), (/*_.LIST___V40*/ meltfptr[28]), "SEXP_CONTENTS");
                                                                                                                                                                ;
                                                                                                                                                                /*^touchobj*/

                                                                                                                                                                melt_dbgtrace_written_object (/*_.INST__V46*/ meltfptr[31], "newly made instance");
                                                                                                                                                                ;
                                                                                                                                                                /*_.INST___V45*/
                                                                                                                                                                meltfptr[29] = /*_.INST__V46*/ meltfptr[31];;

                                                                                                                                                                {
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3203:/ locexp");
                                                                                                                                                                    meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.INST___V45*/ meltfptr[29]));
                                                                                                                                                                }
                                                                                                                                                                ;



                                                                                                                                                                {
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3208:/ locexp");
                                                                                                                                                                    meltgc_append_list((melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (melt_ptr_t)((/*nil*/NULL)));
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                                /*clear*/ /*_._IFELSE___V37*/
                                                                                                                                                                meltfptr[32] = 0 ;

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3201:/ clear");
                                                                                                                                                                /*clear*/ /*_#LOCLINE__L20*/
                                                                                                                                                                meltfnum[14] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.MIXINT_VAL__V38*/
                                                                                                                                                                meltfptr[26] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.BOX__V39*/
                                                                                                                                                                meltfptr[27] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.LIST___V40*/
                                                                                                                                                                meltfptr[28] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.INST___V45*/
                                                                                                                                                                meltfptr[29] = 0 ;
                                                                                                                                                                /*epilog*/
                                                                                                                                                            }
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else
                                                                                                                                                        {
                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3198:/ cond.else");

                                                                                                                                                            /*^block*/
                                                                                                                                                            /*anyblock*/
                                                                                                                                                            {


                                                                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                                                                ;
                                                                                                                                                                /*_#IS_CLOSURE__L21*/
                                                                                                                                                                meltfnum[13] =
                                                                                                                                                                    (melt_magic_discr((melt_ptr_t)(/*_.V__V16*/ meltfptr[13])) == MELTOBMAG_CLOSURE);;
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3210:/ cond");
                                                                                                                                                                /*cond*/
                                                                                                                                                                if (/*_#IS_CLOSURE__L21*/ meltfnum[13]) /*then*/
                                                                                                                                                                    {
                                                                                                                                                                        /*^cond.then*/
                                                                                                                                                                        /*^block*/
                                                                                                                                                                        /*anyblock*/
                                                                                                                                                                        {


                                                                                                                                                                            MELT_CHECK_SIGNAL();
                                                                                                                                                                            ;
                                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3211:/ apply");
                                                                                                                                                                            /*apply*/
                                                                                                                                                                            {
                                                                                                                                                                                /*_.V__V48*/ meltfptr[43] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V16*/ meltfptr[13]), (melt_ptr_t)(/*_.CONT__V4*/ meltfptr[3]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                                                                                                                                                            }
                                                                                                                                                                            ;
                                                                                                                                                                            /*_._IFELSE___V47*/
                                                                                                                                                                            meltfptr[42] = /*_.V__V48*/ meltfptr[43];;
                                                                                                                                                                            /*epilog*/

                                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3210:/ clear");
                                                                                                                                                                            /*clear*/ /*_.V__V48*/
                                                                                                                                                                            meltfptr[43] = 0 ;
                                                                                                                                                                        }
                                                                                                                                                                        ;
                                                                                                                                                                    }
                                                                                                                                                                else    /*^cond.else*/
                                                                                                                                                                    {

                                                                                                                                                                        /*^block*/
                                                                                                                                                                        /*anyblock*/
                                                                                                                                                                        {

                                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3213:/ quasiblock");


                                                                                                                                                                            /*_.DISCRIM__V50*/
                                                                                                                                                                            meltfptr[27] =
                                                                                                                                                                                ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.V__V16*/ meltfptr[13]))));;
                                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3213:/ cond");
                                                                                                                                                                            /*cond*/
                                                                                                                                                                            if (
                                                                                                                                                                                /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.DISCRIM__V50*/ meltfptr[27]),
                                                                                                                                                                                                              (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[5])))
                                                                                                                                                                            ) /*then*/
                                                                                                                                                                                {
                                                                                                                                                                                    /*^cond.then*/
                                                                                                                                                                                    /*^getslot*/
                                                                                                                                                                                    {
                                                                                                                                                                                        melt_ptr_t slot=NULL, obj=NULL;
                                                                                                                                                                                        obj = (melt_ptr_t)(/*_.DISCRIM__V50*/ meltfptr[27]) /*=obj*/;
                                                                                                                                                                                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                                                                                                                        /*_.DISCRNAME__V51*/
                                                                                                                                                                                        meltfptr[28] = slot;
                                                                                                                                                                                    };
                                                                                                                                                                                    ;
                                                                                                                                                                                }
                                                                                                                                                                            else    /*^cond.else*/
                                                                                                                                                                                {

                                                                                                                                                                                    /*_.DISCRNAME__V51*/ meltfptr[28] =  /*reallynil*/ NULL ;;
                                                                                                                                                                                }
                                                                                                                                                                            ;
                                                                                                                                                                            /*^compute*/
                                                                                                                                                                            /*_#VIX__L22*/
                                                                                                                                                                            meltfnum[14] =
                                                                                                                                                                                /*variadic_index*/
#ifdef melt_variadic_index
                                                                                                                                                                                (melt_variadic_index)
#else
                                                                                                                                                                                -1
#endif /*melt_variadic_index*/
                                                                                                                                                                                ;;

                                                                                                                                                                            {
                                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3217:/ locexp");
                                                                                                                                                                                melt_error_str((melt_ptr_t)(/*_.VLOC__V5*/ meltfptr[4]), ( "in MELT_MAKE_SEXPR unexpected value of discriminant"), (melt_ptr_t)(/*_.DISCRNAME__V51*/ meltfptr[28]));
                                                                                                                                                                            }
                                                                                                                                                                            ;

                                                                                                                                                                            {
                                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3218:/ locexp");
                                                                                                                                                                                /* melt_make_sexpr ERRBADVAL_CHK__1 */
                                                                                                                                                                                inform (UNKNOWN_LOCATION,
                                                                                                                                                                                        "unexpected value index in MELT_MAKE_SEXPR is %d", (int) /*_#VIX__L22*/ meltfnum[14]) ;
                                                                                                                                                                                ;
                                                                                                                                                                            }
                                                                                                                                                                            ;

                                                                                                                                                                            MELT_CHECK_SIGNAL();
                                                                                                                                                                            ;
                                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3223:/ quasiblock");


                                                                                                                                                                            /*_._RETVAL___V1*/
                                                                                                                                                                            meltfptr[0] = (/*nil*/NULL);;

                                                                                                                                                                            {
                                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3223:/ locexp");
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
                                                                                                                                                                            meltfptr[26] = /*_.RETURN___V52*/ meltfptr[29];;

                                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3213:/ clear");
                                                                                                                                                                            /*clear*/ /*_.DISCRIM__V50*/
                                                                                                                                                                            meltfptr[27] = 0 ;
                                                                                                                                                                            /*^clear*/
                                                                                                                                                                            /*clear*/ /*_.DISCRNAME__V51*/
                                                                                                                                                                            meltfptr[28] = 0 ;
                                                                                                                                                                            /*^clear*/
                                                                                                                                                                            /*clear*/ /*_#VIX__L22*/
                                                                                                                                                                            meltfnum[14] = 0 ;
                                                                                                                                                                            /*^clear*/
                                                                                                                                                                            /*clear*/ /*_.RETURN___V52*/
                                                                                                                                                                            meltfptr[29] = 0 ;
                                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3212:/ quasiblock");


                                                                                                                                                                            /*_.PROGN___V53*/
                                                                                                                                                                            meltfptr[43] = /*_.LET___V49*/ meltfptr[26];;
                                                                                                                                                                            /*^compute*/
                                                                                                                                                                            /*_._IFELSE___V47*/
                                                                                                                                                                            meltfptr[42] = /*_.PROGN___V53*/ meltfptr[43];;
                                                                                                                                                                            /*epilog*/

                                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:3210:/ clear");
                                                                                                                                                                            /*clear*/ /*_.LET___V49*/
                                                                                                                                                                            meltfptr[26] = 0 ;
                                                                                                                                                                            /*^clear*/
                                                                                                                                                                            /*clear*/ /*_.PROGN___V53*/
                                                                                                                                                                            meltfptr[43] = 0 ;
                                                                                                                                                                        }
                                                                                                                                                                        ;
                                                                                                                                                                    }
                                                                                                                                                                ;
                                                                                                                                                                /*_._IFELSE___V37*/
                                                                                                                                                                meltfptr[32] = /*_._IFELSE___V47*/ meltfptr[42];;
                                                                                                                                                                /*epilog*/

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:3198:/ clear");
                                                                                                                                                                /*clear*/ /*_#IS_CLOSURE__L21*/
                                                                                                                                                                meltfnum[13] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_._IFELSE___V47*/
                                                                                                                                                                meltfptr[42] = 0 ;
                                                                                                                                                            }
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    ;
                                                                                                                                                    /*_._IFELSE___V26*/
                                                                                                                                                    meltfptr[25] = /*_._IFELSE___V37*/ meltfptr[32];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:3185:/ clear");
                                                                                                                                                    /*clear*/ /*_#IS_MIXINT__L17*/
                                                                                                                                                    meltfnum[15] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_#_IF___L18*/
                                                                                                                                                    meltfnum[12] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_._IFELSE___V37*/
                                                                                                                                                    meltfptr[32] = 0 ;
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        ;
                                                                                                                                        /*_._IFELSE___V23*/
                                                                                                                                        meltfptr[22] = /*_._IFELSE___V26*/ meltfptr[25];;
                                                                                                                                        /*epilog*/

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:3179:/ clear");
                                                                                                                                        /*clear*/ /*_#IS_MIXLOC__L12*/
                                                                                                                                        meltfnum[11] = 0 ;
                                                                                                                                        /*^clear*/
                                                                                                                                        /*clear*/ /*_._IFELSE___V26*/
                                                                                                                                        meltfptr[25] = 0 ;
                                                                                                                                    }
                                                                                                                                    ;
                                                                                                                                }
                                                                                                                            ;
                                                                                                                            /*_._IFELSE___V21*/
                                                                                                                            meltfptr[20] = /*_._IFELSE___V23*/ meltfptr[22];;
                                                                                                                            /*epilog*/

                                                                                                                            MELT_LOCATION("warmelt-base.melt:3174:/ clear");
                                                                                                                            /*clear*/ /*_#IS_LIST__L11*/
                                                                                                                            meltfnum[10] = 0 ;
                                                                                                                            /*^clear*/
                                                                                                                            /*clear*/ /*_._IFELSE___V23*/
                                                                                                                            meltfptr[22] = 0 ;
                                                                                                                        }
                                                                                                                        ;
                                                                                                                    }
                                                                                                                ;
                                                                                                                /*_._IFELSE___V20*/
                                                                                                                meltfptr[19] = /*_._IFELSE___V21*/ meltfptr[20];;
                                                                                                                /*epilog*/

                                                                                                                MELT_LOCATION("warmelt-base.melt:3172:/ clear");
                                                                                                                /*clear*/ /*_#IS_MULTIPLE__L9*/
                                                                                                                meltfnum[8] = 0 ;
                                                                                                                /*^clear*/
                                                                                                                /*clear*/ /*_._IFELSE___V21*/
                                                                                                                meltfptr[20] = 0 ;
                                                                                                            }
                                                                                                            ;
                                                                                                        }
                                                                                                    ;
                                                                                                    /*_._IFELSE___V19*/
                                                                                                    meltfptr[18] = /*_._IFELSE___V20*/ meltfptr[19];;
                                                                                                    /*epilog*/

                                                                                                    MELT_LOCATION("warmelt-base.melt:3170:/ clear");
                                                                                                    /*clear*/ /*_#IS_STRING__L8*/
                                                                                                    meltfnum[7] = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_._IFELSE___V20*/
                                                                                                    meltfptr[19] = 0 ;
                                                                                                }
                                                                                                ;
                                                                                            }
                                                                                        ;
                                                                                        /*_._IFELSE___V18*/
                                                                                        meltfptr[17] = /*_._IFELSE___V19*/ meltfptr[18];;
                                                                                        /*epilog*/

                                                                                        MELT_LOCATION("warmelt-base.melt:3168:/ clear");
                                                                                        /*clear*/ /*_#IS_INTEGERBOX__L7*/
                                                                                        meltfnum[6] = 0 ;
                                                                                        /*^clear*/
                                                                                        /*clear*/ /*_._IFELSE___V19*/
                                                                                        meltfptr[18] = 0 ;
                                                                                    }
                                                                                    ;
                                                                                }
                                                                            ;
                                                                            /*_._IFELSE___V17*/
                                                                            meltfptr[16] = /*_._IFELSE___V18*/ meltfptr[17];;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:3166:/ clear");
                                                                            /*clear*/ /*_#IS_OBJECT__L6*/
                                                                            meltfnum[2] = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_._IFELSE___V18*/
                                                                            meltfptr[17] = 0 ;
                                                                        }
                                                                        ;
                                                                    }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:3154:/ clear");
                                                                /*clear*/ /*_.V__V16*/
                                                                meltfptr[13] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_#NULL__L5*/
                                                                meltfnum[1] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_._IFELSE___V17*/
                                                                meltfptr[16] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:3226:/ quasiblock");


                                                                /*_.VARIADIC_CTYPE__V55*/
                                                                meltfptr[28] =
                                                                    /*variadic_type_code*/
#ifdef melt_variadic_index
                                                                    (((melt_variadic_index + 0) >= 0
                                                                      && (melt_variadic_index + 0) < melt_variadic_length)
                                                                     ?  melt_code_to_ctype (meltxargdescr_[melt_variadic_index + 0]
                                                                                            & MELT_ARGDESCR_MAX)
                                                                     : NULL)
#else
                                                                    NULL /* no variadic_ctype outside of variadic functions */
#endif /*melt_variadic_index*/
                                                                    ;;
                                                                MELT_LOCATION("warmelt-base.melt:3226:/ cond");
                                                                /*cond*/
                                                                if (
                                                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VARIADIC_CTYPE__V55*/ meltfptr[28]),
                                                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[5])))
                                                                ) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^getslot*/
                                                                        {
                                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                                            obj = (melt_ptr_t)(/*_.VARIADIC_CTYPE__V55*/ meltfptr[28]) /*=obj*/;
                                                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                            /*_.CTYNAME__V56*/
                                                                            meltfptr[29] = slot;
                                                                        };
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*_.CTYNAME__V56*/ meltfptr[29] =  /*reallynil*/ NULL ;;
                                                                    }
                                                                ;
                                                                /*^compute*/
                                                                /*_#VIX__L23*/
                                                                meltfnum[14] =
                                                                    /*variadic_index*/
#ifdef melt_variadic_index
                                                                    (melt_variadic_index)
#else
                                                                    -1
#endif /*melt_variadic_index*/
                                                                    ;;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:3229:/ locexp");
                                                                    melt_error_str((melt_ptr_t)(/*_.VLOC__V5*/ meltfptr[4]), ( "in MELT_MAKE_SEXPR unexpected thing of ctype"), (melt_ptr_t)(/*_.CTYNAME__V56*/ meltfptr[29]));
                                                                }
                                                                ;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:3230:/ locexp");
                                                                    /* melt_make_sexpr ERRBADTHING_CHK__1 */
                                                                    inform (UNKNOWN_LOCATION,
                                                                            "unexpected thing index in MELT_MAKE_SEXPR is %d", (int) /*_#VIX__L23*/ meltfnum[14]) ;
                                                                    ;
                                                                }
                                                                ;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-base.melt:3235:/ quasiblock");


                                                                /*_._RETVAL___V1*/
                                                                meltfptr[0] = (/*nil*/NULL);;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:3235:/ locexp");
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
                                                                /*_.LET___V54*/
                                                                meltfptr[27] = /*_.RETURN___V57*/ meltfptr[26];;

                                                                MELT_LOCATION("warmelt-base.melt:3226:/ clear");
                                                                /*clear*/ /*_.VARIADIC_CTYPE__V55*/
                                                                meltfptr[28] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.CTYNAME__V56*/
                                                                meltfptr[29] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_#VIX__L23*/
                                                                meltfnum[14] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.RETURN___V57*/
                                                                meltfptr[26] = 0 ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:3154:/ clear");
                                                                /*clear*/ /*_.LET___V54*/
                                                                meltfptr[27] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    ;
                                                    /*epilog*/
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/
                                    }
                                    ;
                                }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                ;
                /*epilog*/
            }
            ;
            ;
            goto meltlabloop_ARGLOOP_5;
meltlabexit_ARGLOOP_5:
            ;
            MELT_LOCATION("warmelt-base.melt:3152:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V12*/
            meltfptr[5] = /*_.ARGLOOP__V13*/ meltfptr[12];;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3238:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SEXPR__V10*/ meltfptr[7];;

        {
            MELT_LOCATION("warmelt-base.melt:3238:/ locexp");
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
        /*_.LET___V3*/
        meltfptr[2] = /*_.RETURN___V58*/ meltfptr[43];;

        MELT_LOCATION("warmelt-base.melt:3139:/ clear");
        /*clear*/ /*_.CONT__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_MIXINT__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.VLOC__V5*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SEXPR__V10*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V12*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V58*/
        meltfptr[43] = 0 ;
        MELT_LOCATION("warmelt-base.melt:3135:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3135:/ locexp");
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
    melt_trace_end("MELT_MAKE_SEXPR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_32_WARMELTmiBASE_MELT_MAKE_SEXPR*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_34_WARMELTmiBASE_MULTIPLE_SORT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_34_WARMELTmiBASE_MULTIPLE_SORT fromline 1715 */

    /** start of frame for meltrout_34_WARMELTmiBASE_MULTIPLE_SORT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT// fromline 1531
        : public Melt_CallFrameWithValues<21>
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
        MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<21> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT), clos) {};
        MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT() //the constructor fromline 1605
            : Melt_CallFrameWithValues<21> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<21> (fil,lin, sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT)) {};
        MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<21> (fil,lin, sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT


    /** end of frame for meltrout_34_WARMELTmiBASE_MULTIPLE_SORT fromline 1660**/

    /* end of frame for routine meltrout_34_WARMELTmiBASE_MULTIPLE_SORT fromline 1719 */

    /* classy proc frame meltrout_34_WARMELTmiBASE_MULTIPLE_SORT */
    MeltFrame_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_34_WARMELTmiBASE_MULTIPLE_SORT fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_SORT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3309:/ getarg");
    /*_.MUL__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.CMP__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.CMP__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DIS__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DIS__V4*/ meltfptr[3])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-base.melt:3318:/ locexp");
            /* melt_low_debug */
            melt_low_debug_value( "multiple_sort start mul==",(melt_ptr_t)/*_.MUL__V2*/ meltfptr[1]);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_MULTIPLE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.MUL__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:3319:/ cond");
        /*cond*/
        if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-base.melt:3320:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V5*/
                    meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:3319:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:3321:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-base.melt:3321:/ locexp");
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
                    /*_._IFELSE___V5*/
                    meltfptr[4] = /*_.RETURN___V6*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3319:/ clear");
                    /*clear*/ /*_.RETURN___V6*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.CMP__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-base.melt:3322:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-base.melt:3323:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V7*/
                    meltfptr[5] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:3322:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:3324:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-base.melt:3324:/ locexp");
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
                    /*_._IFELSE___V7*/
                    meltfptr[5] = /*_.RETURN___V8*/ meltfptr[7];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3322:/ clear");
                    /*clear*/ /*_.RETURN___V8*/
                    meltfptr[7] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L3*/
        meltfnum[2] =
            ((/*_.DIS__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-base.melt:3325:/ cond");
        /*cond*/
        if (/*_#NULL__L3*/ meltfnum[2]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:3326:/ compute");
                    /*_.DIS__V4*/
                    meltfptr[3] = /*_.SETQ___V10*/ meltfptr[9] = (/*!DISCR_MULTIPLE*/ meltfrout->tabval[0]);;
                    /*_._IF___V9*/
                    meltfptr[7] = /*_.SETQ___V10*/ meltfptr[9];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3325:/ clear");
                    /*clear*/ /*_.SETQ___V10*/
                    meltfptr[9] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V9*/ meltfptr[7] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L4*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.DIS__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_DISCRIMINANT*/ meltfrout->tabval[1])));;
        MELT_LOCATION("warmelt-base.melt:3327:/ cond");
        /*cond*/
        if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-base.melt:3328:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V11*/
                    meltfptr[9] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:3327:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:3329:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-base.melt:3329:/ locexp");
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
                    /*_._IFELSE___V11*/
                    meltfptr[9] = /*_.RETURN___V12*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3327:/ clear");
                    /*clear*/ /*_.RETURN___V12*/
                    meltfptr[11] = 0 ;
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:3330:/ locexp");
            /* multiple_sort DBGMULSORTDISCRIM_CHK__1 */
            debugeprintf ("multiple_sort DBGMULSORTDISCRIM_CHK__1 dis=%p of magic %d",
                          (void*)/*_.DIS__V4*/ meltfptr[3], melt_magic_discr(/*_.DIS__V4*/ meltfptr[3]));
            ;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#CHECKMAGIC_CHK__L5*/
        meltfnum[4] =
            /* multiple_sort CHECKMAGIC_CHK__1 */
            (((meltobject_ptr_t)(/*_.DIS__V4*/ meltfptr[3]))->meltobj_magic == MELTOBMAG_MULTIPLE) ;;
        MELT_LOCATION("warmelt-base.melt:3336:/ cond");
        /*cond*/
        if (/*_#CHECKMAGIC_CHK__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-base.melt:3339:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V13*/
                    meltfptr[11] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:3336:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:3340:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-base.melt:3340:/ locexp");
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
                    /*_._IFELSE___V13*/
                    meltfptr[11] = /*_.RETURN___V14*/ meltfptr[13];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3336:/ clear");
                    /*clear*/ /*_.RETURN___V14*/
                    meltfptr[13] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3341:/ quasiblock");


        /*_#TUPLEN__L6*/
        meltfnum[5] =
            (melt_multiple_length((melt_ptr_t)(/*_.MUL__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-base.melt:3343:/ locexp");
            /* multiple_sort DBGMULSORT_CHK__1 */
            debugeprintf ("multiple_sort DBGMULSORT_CHK__1 tuplen=%ld", /*_#TUPLEN__L6*/ meltfnum[5]);
            ;
        }
        ;
        MELT_LOCATION("warmelt-base.melt:3348:/ quasiblock");


        /*_.RES__V17*/
        meltfptr[16] =
            (meltgc_new_multiple((meltobject_ptr_t)(/*_.DIS__V4*/ meltfptr[3]), (/*_#TUPLEN__L6*/ meltfnum[5])));;
        /*^compute*/
        /*_#RESLEN__L7*/
        meltfnum[6] =
            (melt_multiple_length((melt_ptr_t)(/*_.RES__V17*/ meltfptr[16])));;

        {
            MELT_LOCATION("warmelt-base.melt:3352:/ locexp");
            /* multiple_sort SORTRES_CHK__1 start */
            melt_assertmsg ("check that MUL is multiple when starting multiple_sort",
                            melt_magic_discr (/*_.MUL__V2*/ meltfptr[1]) == MELTOBMAG_MULTIPLE) ;
            if (/*_#RESLEN__L7*/ meltfnum[6] >= (long)INT_MAX/2)
                melt_fatal_error("too big sized %ld tuple to sort", /*_#RESLEN__L7*/ meltfnum[6]) ;
            melt_assertmsg ("check that RESLEN == TUPLEN in multiple_sort", /*_#RESLEN__L7*/ meltfnum[6] == /*_#TUPLEN__L6*/ meltfnum[5]) ;
            Melt_Sort_Compare_Index meltixsorter_SORTRES_CHK__1 ((meltmultiple_st**)(&(/*_.MUL__V2*/ meltfptr[1])), &(/*_.CMP__V3*/ meltfptr[2])) ;
            std::vector<int> meltixvect_SORTRES_CHK__1	;
            meltixvect_SORTRES_CHK__1.resize(/*_#RESLEN__L7*/ meltfnum[6])	;
            for (long meltix=0;
                    meltix</*_#RESLEN__L7*/ meltfnum[6];
                    meltix++)
                meltixvect_SORTRES_CHK__1[meltix] = meltix ;
            /* do the sort SORTRES_CHK__1 */
            std::stable_sort (meltixvect_SORTRES_CHK__1.begin(),
                              meltixvect_SORTRES_CHK__1.end(),
                              meltixsorter_SORTRES_CHK__1) ;
            melt_assertmsg ("check that RES is multiple in multiple_sort", melt_magic_discr (/*_.RES__V17*/ meltfptr[16]) == MELTOBMAG_MULTIPLE) ;
            melt_assertmsg ("check that MUL is multiple in multiple_sort", melt_magic_discr (/*_.MUL__V2*/ meltfptr[1]) == MELTOBMAG_MULTIPLE) ;
            for (long meltix=0;
                    meltix</*_#RESLEN__L7*/ meltfnum[6];
                    meltix++)
                {
                    int sortix_SORTRES_CHK__1 = meltixvect_SORTRES_CHK__1[meltix] ;
                    melt_assertmsg ("check index in multiple_sort", sortix_SORTRES_CHK__1>=0 && (long)sortix_SORTRES_CHK__1 < /*_#RESLEN__L7*/ meltfnum[6]) ;
                    ((meltmultiple_st*)(/*_.RES__V17*/ meltfptr[16]))->tabval[meltix]
                        = ((meltmultiple_st*)(/*_.MUL__V2*/ meltfptr[1]))->tabval[sortix_SORTRES_CHK__1] ;
                } ;
            meltgc_touch (/*_.RES__V17*/ meltfptr[16]) ;
            /* multiple_sort SORTRES_CHK__1 end */
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:3384:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE__L8*/
            meltfnum[7] =
                (melt_magic_discr((melt_ptr_t)(/*_.RES__V17*/ meltfptr[16])) == MELTOBMAG_MULTIPLE);;
            MELT_LOCATION("warmelt-base.melt:3384:/ cond");
            /*cond*/
            if (/*_#IS_MULTIPLE__L8*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V19*/ meltfptr[18] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:3384:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check res";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3384;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V17*/ meltfptr[16];
                            /*_.MELT_ASSERT_FAILURE_FUN__V20*/
                            meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V19*/
                        meltfptr[18] = /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:3384:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V20*/
                        meltfptr[19] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V18*/
            meltfptr[17] = /*_._IFELSE___V19*/ meltfptr[18];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:3384:/ clear");
            /*clear*/ /*_#IS_MULTIPLE__L8*/
            meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V19*/
            meltfptr[18] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V18*/ meltfptr[17] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:3385:/ locexp");
            /* melt_low_debug */
            melt_low_debug_value( "multiple_sort end result res==",(melt_ptr_t)/*_.RES__V17*/ meltfptr[16]);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3386:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V17*/ meltfptr[16];;

        {
            MELT_LOCATION("warmelt-base.melt:3386:/ locexp");
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
        /*_.LET___V16*/
        meltfptr[15] = /*_.RETURN___V21*/ meltfptr[19];;

        MELT_LOCATION("warmelt-base.melt:3348:/ clear");
        /*clear*/ /*_.RES__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_#RESLEN__L7*/
        meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V21*/
        meltfptr[19] = 0 ;
        /*_.LET___V15*/
        meltfptr[13] = /*_.LET___V16*/ meltfptr[15];;

        MELT_LOCATION("warmelt-base.melt:3341:/ clear");
        /*clear*/ /*_#TUPLEN__L6*/
        meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[15] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3309:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V15*/ meltfptr[13];;

        {
            MELT_LOCATION("warmelt-base.melt:3309:/ locexp");
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
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V5*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V7*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/
        meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L4*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#CHECKMAGIC_CHK__L5*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V13*/
        meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V15*/
        meltfptr[13] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_SORT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_34_WARMELTmiBASE_MULTIPLE_SORT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_34_WARMELTmiBASE_MULTIPLE_SORT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE fromline 1715 */

    /** start of frame for meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE// fromline 1531
        : public Melt_CallFrameWithValues<18>
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
        MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<18> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE), clos) {};
        MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE() //the constructor fromline 1605
            : Melt_CallFrameWithValues<18> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<18> (fil,lin, sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE)) {};
        MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<18> (fil,lin, sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE


    /** end of frame for meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE fromline 1660**/

    /* end of frame for routine meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE fromline 1719 */

    /* classy proc frame meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE */
    MeltFrame_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MAPOBJECT_SORTED_ATTRIBUTE_TUPLE", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3391:/ getarg");
    /*_.MAPO__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:3394:/ quasiblock");


        /*_#MAPCOUNT__L1*/
        meltfnum[0] =
            (melt_count_mapobjects((meltmapobjects_ptr_t)(/*_.MAPO__V2*/ meltfptr[1])));;
        /*^compute*/
        /*_.COUNTBOX__V4*/
        meltfptr[3] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[0])), (0)));;
        /*^compute*/
        /*_.BOXEDONE__V5*/
        meltfptr[4] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[0])), (1)));;
        /*^compute*/
        /*_.BOXEDZERO__V6*/
        meltfptr[5] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[0])), (0)));;
        /*^compute*/
        /*_.BOXEDMINUSONE__V7*/
        meltfptr[6] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[0])), (-1)));;
        /*^compute*/
        /*_.TUPL__V8*/
        meltfptr[7] =
            (meltgc_new_multiple((meltobject_ptr_t)((/*!DISCR_MULTIPLE*/ meltfrout->tabval[1])), (/*_#MAPCOUNT__L1*/ meltfnum[0])));;
        MELT_LOCATION("warmelt-base.melt:3405:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V10*/
        meltfptr[9] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_4*/ meltfrout->tabval[4])), (2));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V10*/ meltfptr[9])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V10*/ meltfptr[9])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V10*/ meltfptr[9])->tabval[0] = (melt_ptr_t)(/*_.COUNTBOX__V4*/ meltfptr[3]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V10*/ meltfptr[9])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V10*/ meltfptr[9])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V10*/ meltfptr[9])->tabval[1] = (melt_ptr_t)(/*_.TUPL__V8*/ meltfptr[7]);
        ;
        /*_.LAMBDA___V9*/
        meltfptr[8] = /*_.LAMBDA___V10*/ meltfptr[9];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3403:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V9*/ meltfptr[8];
            /*_.MAPOBJECT_EVERY__V11*/
            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MAPOBJECT_EVERY*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.MAPO__V2*/ meltfptr[1]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-base.melt:3414:/ quasiblock");


        MELT_LOCATION("warmelt-base.melt:3417:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V14*/
        meltfptr[13] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_7*/ meltfrout->tabval[7])), (3));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V14*/ meltfptr[13])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V14*/ meltfptr[13])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V14*/ meltfptr[13])->tabval[0] = (melt_ptr_t)(/*_.BOXEDMINUSONE__V7*/ meltfptr[6]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V14*/ meltfptr[13])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V14*/ meltfptr[13])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V14*/ meltfptr[13])->tabval[1] = (melt_ptr_t)(/*_.BOXEDZERO__V6*/ meltfptr[5]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V14*/ meltfptr[13])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V14*/ meltfptr[13])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V14*/ meltfptr[13])->tabval[2] = (melt_ptr_t)(/*_.BOXEDONE__V5*/ meltfptr[4]);
        ;
        /*_.LAMBDA___V13*/
        meltfptr[12] = /*_.LAMBDA___V14*/ meltfptr[13];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3415:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V13*/ meltfptr[12];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[1]);
            /*_.SORTUPL__V15*/
            meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MULTIPLE_SORT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.TUPL__V8*/ meltfptr[7]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-base.melt:3431:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V17*/
        meltfptr[16] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_9*/ meltfrout->tabval[9])), (0));
        ;
        /*_.LAMBDA___V16*/
        meltfptr[15] = /*_.LAMBDA___V17*/ meltfptr[16];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3431:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V16*/ meltfptr[15];
            /*_.MULTIPLE_MAP__V18*/
            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MULTIPLE_MAP*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.SORTUPL__V15*/ meltfptr[14]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.LET___V12*/
        meltfptr[11] = /*_.MULTIPLE_MAP__V18*/ meltfptr[17];;

        MELT_LOCATION("warmelt-base.melt:3414:/ clear");
        /*clear*/ /*_.LAMBDA___V13*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SORTUPL__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MULTIPLE_MAP__V18*/
        meltfptr[17] = 0 ;
        /*_.LET___V3*/
        meltfptr[2] = /*_.LET___V12*/ meltfptr[11];;

        MELT_LOCATION("warmelt-base.melt:3394:/ clear");
        /*clear*/ /*_#MAPCOUNT__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.COUNTBOX__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXEDONE__V5*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXEDZERO__V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXEDMINUSONE__V7*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPL__V8*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V9*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAPOBJECT_EVERY__V11*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V12*/
        meltfptr[11] = 0 ;
        MELT_LOCATION("warmelt-base.melt:3391:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3391:/ locexp");
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
    melt_trace_end("MAPOBJECT_SORTED_ATTRIBUTE_TUPLE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_35_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_36_WARMELTmiBASE_LAMBDA_cl1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                     const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_36_WARMELTmiBASE_LAMBDA_cl1_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_36_WARMELTmiBASE_LAMBDA_cl1 fromline 1715 */

    /** start of frame for meltrout_36_WARMELTmiBASE_LAMBDA_cl1 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1// fromline 1531
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1), clos) {};
        MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1() //the constructor fromline 1605
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1)) {};
        MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1


    /** end of frame for meltrout_36_WARMELTmiBASE_LAMBDA_cl1 fromline 1660**/

    /* end of frame for routine meltrout_36_WARMELTmiBASE_LAMBDA_cl1 fromline 1719 */

    /* classy proc frame meltrout_36_WARMELTmiBASE_LAMBDA_cl1 */
    MeltFrame_meltrout_36_WARMELTmiBASE_LAMBDA_cl1
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_36_WARMELTmiBASE_LAMBDA_cl1 fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl1", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3405:/ getarg");
    /*_.AT__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.VA__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.VA__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:3406:/ quasiblock");


        /*_#CURCOUNT__L1*/
        meltfnum[0] =
            (melt_get_int((melt_ptr_t)((/*~COUNTBOX*/ meltfclos->tabval[0]))));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V4*/
        meltfptr[3] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[0])), (/*_#CURCOUNT__L1*/ meltfnum[0])));;
        MELT_LOCATION("warmelt-base.melt:3407:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_1_st
            {
                struct MELT_MULTIPLE_STRUCT(3) rtup_0__TUPLREC__x1;
                long meltletrec_1_endgap;
            } *meltletrec_1_ptr = 0;
            meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x1*/
            /*_.TUPLREC___V6*/
            meltfptr[5] = (melt_ptr_t) &meltletrec_1_ptr->rtup_0__TUPLREC__x1;
            meltletrec_1_ptr->rtup_0__TUPLREC__x1.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_1_ptr->rtup_0__TUPLREC__x1.nbval = 3;


            /*^putuple*/
            /*putupl#1*/
            melt_assertmsg("putupl [:3407] #1 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V6*/ meltfptr[5]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:3407] #1 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V6*/ meltfptr[5]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V6*/ meltfptr[5]))->tabval[0] = (melt_ptr_t)(/*_.AT__V2*/ meltfptr[1]);
            ;
            /*^putuple*/
            /*putupl#2*/
            melt_assertmsg("putupl [:3407] #2 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V6*/ meltfptr[5]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:3407] #2 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V6*/ meltfptr[5]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V6*/ meltfptr[5]))->tabval[1] = (melt_ptr_t)(/*_.VA__V3*/ meltfptr[2]);
            ;
            /*^putuple*/
            /*putupl#3*/
            melt_assertmsg("putupl [:3407] #3 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V6*/ meltfptr[5]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:3407] #3 checkoff", (2>=0 && 2< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V6*/ meltfptr[5]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V6*/ meltfptr[5]))->tabval[2] = (melt_ptr_t)(/*_.MAKE_INTEGERBOX__V4*/ meltfptr[3]);
            ;
            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V6*/ meltfptr[5]);
            ;
            /*_.ENT__V5*/
            meltfptr[4] = /*_.TUPLREC___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:3407:/ clear");
            /*clear*/ /*_.TUPLREC___V6*/
            meltfptr[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V6*/
            meltfptr[5] = 0 ;
        } /*end multiallocblock*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:3410:/ locexp");
            meltgc_multiple_put_nth((melt_ptr_t)((/*~TUPL*/ meltfclos->tabval[1])), (/*_#CURCOUNT__L1*/ meltfnum[0]), (melt_ptr_t)(/*_.ENT__V5*/ meltfptr[4]));
        }
        ;
        /*_#plI__L2*/
        meltfnum[1] =
            ((/*_#CURCOUNT__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-base.melt:3411:/ locexp");
            melt_put_int((melt_ptr_t)((/*~COUNTBOX*/ meltfclos->tabval[0])), (/*_#plI__L2*/ meltfnum[1]));
        }
        ;

        MELT_LOCATION("warmelt-base.melt:3406:/ clear");
        /*clear*/ /*_#CURCOUNT__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ENT__V5*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L2*/
        meltfnum[1] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl1", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_36_WARMELTmiBASE_LAMBDA_cl1_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_36_WARMELTmiBASE_LAMBDA_cl1*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_37_WARMELTmiBASE_LAMBDA_cl2(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                     const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_37_WARMELTmiBASE_LAMBDA_cl2_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_37_WARMELTmiBASE_LAMBDA_cl2 fromline 1715 */

    /** start of frame for meltrout_37_WARMELTmiBASE_LAMBDA_cl2 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2// fromline 1531
        : public Melt_CallFrameWithValues<12>
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
        MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2), clos) {};
        MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2() //the constructor fromline 1605
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2)) {};
        MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2


    /** end of frame for meltrout_37_WARMELTmiBASE_LAMBDA_cl2 fromline 1660**/

    /* end of frame for routine meltrout_37_WARMELTmiBASE_LAMBDA_cl2 fromline 1719 */

    /* classy proc frame meltrout_37_WARMELTmiBASE_LAMBDA_cl2 */
    MeltFrame_meltrout_37_WARMELTmiBASE_LAMBDA_cl2
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_37_WARMELTmiBASE_LAMBDA_cl2 fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl2", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3417:/ getarg");
    /*_.E1__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.E2__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.E2__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:3418:/ quasiblock");


        /*_.E1AT__V5*/
        meltfptr[4] =
            (melt_multiple_nth((melt_ptr_t)(/*_.E1__V2*/ meltfptr[1]), (0)));;
        /*^compute*/
        /*_.E1VA__V6*/
        meltfptr[5] =
            (melt_multiple_nth((melt_ptr_t)(/*_.E1__V2*/ meltfptr[1]), (1)));;
        /*^compute*/
        /*_.E1RK__V7*/
        meltfptr[6] =
            (melt_multiple_nth((melt_ptr_t)(/*_.E1__V2*/ meltfptr[1]), (2)));;
        /*^compute*/
        /*_.E2AT__V8*/
        meltfptr[7] =
            (melt_multiple_nth((melt_ptr_t)(/*_.E2__V3*/ meltfptr[2]), (0)));;
        /*^compute*/
        /*_.E2VA__V9*/
        meltfptr[8] =
            (melt_multiple_nth((melt_ptr_t)(/*_.E2__V3*/ meltfptr[2]), (1)));;
        /*^compute*/
        /*_.E2RK__V10*/
        meltfptr[9] =
            (melt_multiple_nth((melt_ptr_t)(/*_.E2__V3*/ meltfptr[2]), (2)));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3426:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[6];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.E1RK__V7*/ meltfptr[6];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.E2AT__V8*/ meltfptr[7];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.E2RK__V10*/ meltfptr[9];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*~BOXEDMINUSONE*/ meltfclos->tabval[0]);
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &(/*~BOXEDZERO*/ meltfclos->tabval[1]);
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &(/*~BOXEDONE*/ meltfclos->tabval[2]);
            /*_.COMPARE_OBJ_RANKED__V11*/
            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!COMPARE_OBJ_RANKED*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.E1AT__V5*/ meltfptr[4]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3425:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.COMPARE_OBJ_RANKED__V11*/ meltfptr[10];;
        MELT_LOCATION("warmelt-base.melt:3425:/ putxtraresult");
        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
        ;
        /*^finalreturn*/
        ;
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V4*/
        meltfptr[3] = /*_.RETURN___V12*/ meltfptr[11];;

        MELT_LOCATION("warmelt-base.melt:3418:/ clear");
        /*clear*/ /*_.E1AT__V5*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.E1VA__V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.E1RK__V7*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.E2AT__V8*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.E2VA__V9*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.E2RK__V10*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.COMPARE_OBJ_RANKED__V11*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V12*/
        meltfptr[11] = 0 ;
        MELT_LOCATION("warmelt-base.melt:3417:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-base.melt:3417:/ locexp");
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
        /*clear*/ /*_.LET___V4*/
        meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl2", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_37_WARMELTmiBASE_LAMBDA_cl2_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_37_WARMELTmiBASE_LAMBDA_cl2*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_38_WARMELTmiBASE_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                     const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_38_WARMELTmiBASE_LAMBDA_cl3_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_38_WARMELTmiBASE_LAMBDA_cl3 fromline 1715 */

    /** start of frame for meltrout_38_WARMELTmiBASE_LAMBDA_cl3 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3// fromline 1531
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
        MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3), clos) {};
        MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3() //the constructor fromline 1605
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3)) {};
        MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3


    /** end of frame for meltrout_38_WARMELTmiBASE_LAMBDA_cl3 fromline 1660**/

    /* end of frame for routine meltrout_38_WARMELTmiBASE_LAMBDA_cl3 fromline 1719 */

    /* classy proc frame meltrout_38_WARMELTmiBASE_LAMBDA_cl3 */
    MeltFrame_meltrout_38_WARMELTmiBASE_LAMBDA_cl3
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_38_WARMELTmiBASE_LAMBDA_cl3 fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl3", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3431:/ getarg");
    /*_.EL__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        /*_.MULTIPLE_NTH__V3*/ meltfptr[2] =
            (melt_multiple_nth((melt_ptr_t)(/*_.EL__V2*/ meltfptr[1]), (0)));;
        MELT_LOCATION("warmelt-base.melt:3431:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MULTIPLE_NTH__V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3431:/ locexp");
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
        /*clear*/ /*_.MULTIPLE_NTH__V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl3", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_38_WARMELTmiBASE_LAMBDA_cl3_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_38_WARMELTmiBASE_LAMBDA_cl3*/



/**** end of warmelt-base+02.cc ****/
