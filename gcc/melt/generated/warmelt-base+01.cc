/* GCC MELT GENERATED C++ FILE warmelt-base+01.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #1 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f1[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-base+01.cc declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-base ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2008 - 2015  Free Software Foundation, Inc.
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



class Melt_InitialClassyFrame_WARMELTmiBASE_h1045706295; // forward declaration fromline 6691
typedef Melt_InitialClassyFrame_WARMELTmiBASE_h1045706295 Melt_InitialFrame;
/**** no MELT module variables ****/

/*** 4 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MELT_MAKE_LOCATION **/
MELT_EXTERN melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION (melt_ptr_t melthookdatap, const char* meltinp0_FILENAME, long meltinp1_LINENO);

/*declare MELT called hook #2 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #3 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 4 MELT called hook declarations ***/

/*** 2 extra MELT c-headers ***/


/** MELT extra c-header 1 : h966007662 **/


/* C++ standard headers for multiple_sort */
#include <vector>
#include <algorithm>




/** MELT extra c-header 2 : h611433 **/


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




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiBASE_MELT_MAKE_SEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiBASE_plIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_3_WARMELTmiBASE_miIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiBASE_stIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiBASE_diIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_6_WARMELTmiBASE_pcIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_MELT_MAKE_LOCATION**/

/** declaration of hook melthook_HOOK_MELT_MAKE_LOCATION */
MELT_EXTERN
melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION(melt_ptr_t melthookdatap,
        const char* meltinp0_FILENAME,
        long meltinp1_LINENO)
;



/** end of declaration for hook melthook_HOOK_MELT_MAKE_LOCATION**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiBASE_LIST_NTH_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiBASE_LIST_CLONE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiBASE_ADD2LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_13_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiBASE_OUTPUT_JSON(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_SORT_COMPARE_LESS**/

/** declaration of hook melthook_HOOK_SORT_COMPARE_LESS */
MELT_EXTERN
long melthook_HOOK_SORT_COMPARE_LESS(melt_ptr_t melthookdatap,
                                     melt_ptr_t meltinp0_LEFT,
                                     melt_ptr_t meltinp1_RIGHT,
                                     melt_ptr_t meltinp2_CMP)
;



/** end of declaration for hook melthook_HOOK_SORT_COMPARE_LESS**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiBASE_MULTIPLE_SORT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiBASE_MAPOBJECT_SORTED_ATTRIBUTE_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_39_WARMELTmiBASE_LAMBDA_cl1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_40_WARMELTmiBASE_LAMBDA_cl2(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_41_WARMELTmiBASE_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_42_WARMELTmiBASE_JSON_NAME_LESS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_43_WARMELTmiBASE_JSON_NAME_EQUAL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_44_WARMELTmiBASE_JSON_CANONICAL_NAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_45_WARMELTmiBASE_MAKE_JSONOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_46_WARMELTmiBASE_LAMBDA_cl4(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_47_WARMELTmiBASE_JSONOBJECT_GET_NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_48_WARMELTmiBASE_OUTJSON_JSONOBJ_METHOD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_49_WARMELTmiBASE_OUTJSON_MULTIPLE_METHOD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_50_WARMELTmiBASE_OUTJSON_NAMED_METHOD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN MELT_LOW_OPTIMIZATION void* melt_start_this_module (void*); /*mandatory start of module*/

MELT_EXTERN const char melt_module_is_gpl_compatible[];

/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiBASE_h1045706295 /*opaqueinitialclassy*/ meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiBASE__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiBASE__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiBASE__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiBASE__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_0 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_1 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_2 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_3 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_4 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_5 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_6 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_7 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_8 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_9 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_10 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_11 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_12 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_13 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_14 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_15 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_16 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_17 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_18 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_19 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_20 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_21 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_22 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_23 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_24 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_25 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_26 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_27 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_28 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_29 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_30 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_31 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_32 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_33 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_34 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_35 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_36 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_37 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_38 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_39 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_40 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_41 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_42 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_43 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_44 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_45 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_46 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_47 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_48 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_49 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_50 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_51 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_52 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_53 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_54 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_55 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_56 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_57 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_58 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_59 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiBASE__initialmeltchunk_60 (meltinitial_frame_t*, char*); //declare



/**** warmelt-base+01.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_9_WARMELTmiBASE_LIST_CLONE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                    const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_9_WARMELTmiBASE_LIST_CLONE_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_9_WARMELTmiBASE_LIST_CLONE fromline 1752 */

    /** start of frame for meltrout_9_WARMELTmiBASE_LIST_CLONE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE// fromline 1568
        : public Melt_CallFrameWithValues<15>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<15> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE), clos) {};
        MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<15> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<15> (fil,lin, sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE)) {};
        MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<15> (fil,lin, sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE


    /** end of frame for meltrout_9_WARMELTmiBASE_LIST_CLONE fromline 1697**/

    /* end of frame for routine meltrout_9_WARMELTmiBASE_LIST_CLONE fromline 1756 */

    /* classy proc frame meltrout_9_WARMELTmiBASE_LIST_CLONE */ MeltFrame_meltrout_9_WARMELTmiBASE_LIST_CLONE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_9_WARMELTmiBASE_LIST_CLONE fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LIST_CLONE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:664:/ getarg");
    /*_.LIS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DISC__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DISC__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_LIST__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-base.melt:666:/ cond");
        /*cond*/ if (/*_#IS_LIST__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V4*/ meltfptr[3] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:666:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:667:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:667:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:666:/ quasiblock");


                    /*_.PROGN___V6*/ meltfptr[5] = /*_.RETURN___V5*/ meltfptr[4];;
                    /*^compute*/
                    /*_._IFELSE___V4*/ meltfptr[3] = /*_.PROGN___V6*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:666:/ clear");
                    /*clear*/ /*_.RETURN___V5*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V6*/ meltfptr[5] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L2*/ meltfnum[1] =
            (/*null*/(/*_.DISC__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-base.melt:668:/ cond");
        /*cond*/ if (/*_#NULL__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.DISC__V3*/ meltfptr[2] = /*_.SETQ___V8*/ meltfptr[5] = (/*!DISCR_LIST*/ meltfrout->tabval[0]);;
                    /*_._IF___V7*/ meltfptr[4] = /*_.SETQ___V8*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:668:/ clear");
                    /*clear*/ /*_.SETQ___V8*/ meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V7*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:669:/ quasiblock");


        /*_.NEWLIS__V10*/ meltfptr[9] =
            (meltgc_new_list((meltobject_ptr_t)(/*_.DISC__V3*/ meltfptr[2])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L3*/ meltfnum[2] =
            (/*null*/(/*_.NEWLIS__V10*/ meltfptr[9]) == NULL);;
        MELT_LOCATION("warmelt-base.melt:671:/ cond");
        /*cond*/ if (/*_#NULL__L3*/ meltfnum[2]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^quasiblock*/


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:671:/ locexp");
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
                    /*_._IF___V11*/ meltfptr[10] = /*_.RETURN___V12*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:671:/ clear");
                    /*clear*/ /*_.RETURN___V12*/ meltfptr[11] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V11*/ meltfptr[10] =  /*reallynil*/ NULL ;;
            }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.CURPAIR__V13*/ meltfptr[11] = melt_list_first( (melt_ptr_t)/*_.LIS__V2*/ meltfptr[1]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V13*/ meltfptr[11]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V13*/ meltfptr[11] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V13*/ meltfptr[11]))
                {
                    /*_.CURCOMP__V14*/ meltfptr[13] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V13*/ meltfptr[11]);



                    {
                        MELT_LOCATION("warmelt-base.melt:675:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.NEWLIS__V10*/ meltfptr[9]), (melt_ptr_t)(/*_.CURCOMP__V14*/ meltfptr[13]));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.CURPAIR__V13*/ meltfptr[11] = NULL;
            /*_.CURCOMP__V14*/ meltfptr[13] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-base.melt:672:/ clear");
            /*clear*/ /*_.CURPAIR__V13*/ meltfptr[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURCOMP__V14*/ meltfptr[13] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:676:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.NEWLIS__V10*/ meltfptr[9];;

        {
            MELT_LOCATION("warmelt-base.melt:676:/ locexp");
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
        /*_.LET___V9*/ meltfptr[5] = /*_.RETURN___V15*/ meltfptr[14];;

        MELT_LOCATION("warmelt-base.melt:669:/ clear");
        /*clear*/ /*_.NEWLIS__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V15*/ meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:664:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V9*/ meltfptr[5];;

        {
            MELT_LOCATION("warmelt-base.melt:664:/ locexp");
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
        /*clear*/ /*_#IS_LIST__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V7*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V9*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LIST_CLONE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_9_WARMELTmiBASE_LIST_CLONE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_9_WARMELTmiBASE_LIST_CLONE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1752 */

    /** start of frame for meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA// fromline 1568
        : public Melt_CallFrameWithValues<19>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA), clos) {};
        MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA() //the constructor fromline 1642
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA)) {};
        MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA


    /** end of frame for meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1697**/

    /* end of frame for routine meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1756 */

    /* classy proc frame meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA */ MeltFrame_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("COMPARE_NAMED_ALPHA", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1223:/ getarg");
    /*_.N1__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.N2__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.N2__V3*/ meltfptr[2])) != NULL);

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
        /*_#eqeq__L1*/ meltfnum[0] =
            ((/*_.N1__V2*/ meltfptr[1]) == (/*_.N2__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-base.melt:1227:/ cond");
        /*cond*/ if (/*_#eqeq__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:1228:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_0*/ meltfrout->tabval[0]);;
                    MELT_LOCATION("warmelt-base.melt:1228:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/ goto meltlabend_rout ;
                    /*_._IFELSE___V4*/ meltfptr[3] = /*_.RETURN___V5*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1227:/ clear");
                    /*clear*/ /*_.RETURN___V5*/ meltfptr[4] = 0 ;
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
                    /*_#IS_NOT_A__L2*/ meltfnum[1] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.N1__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[1])));;
                    MELT_LOCATION("warmelt-base.melt:1229:/ cond");
                    /*cond*/ if (/*_#IS_NOT_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-base.melt:1230:/ quasiblock");


                                /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_2*/ meltfrout->tabval[2]);;
                                MELT_LOCATION("warmelt-base.melt:1230:/ putxtraresult");
                                if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                ;
                                /*^finalreturn*/
                                ;
                                /*finalret*/ goto meltlabend_rout ;
                                /*_._IFELSE___V6*/ meltfptr[4] = /*_.RETURN___V7*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1229:/ clear");
                                /*clear*/ /*_.RETURN___V7*/ meltfptr[6] = 0 ;
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
                                /*_#IS_NOT_A__L3*/ meltfnum[2] =
                                    !melt_is_instance_of((melt_ptr_t)(/*_.N2__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[1])));;
                                MELT_LOCATION("warmelt-base.melt:1231:/ cond");
                                /*cond*/ if (/*_#IS_NOT_A__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1232:/ quasiblock");


                                            /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_3*/ meltfrout->tabval[3]);;
                                            MELT_LOCATION("warmelt-base.melt:1232:/ putxtraresult");
                                            if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                            if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                            if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                            ;
                                            /*^finalreturn*/
                                            ;
                                            /*finalret*/ goto meltlabend_rout ;
                                            /*_._IFELSE___V8*/ meltfptr[6] = /*_.RETURN___V9*/ meltfptr[8];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1231:/ clear");
                                            /*clear*/ /*_.RETURN___V9*/ meltfptr[8] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:1234:/ quasiblock");


                                            /*^getslot*/
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.N1__V2*/ meltfptr[1]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                /*_.SN1__V11*/ meltfptr[10] = slot;
                                            };
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1235:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.N2__V3*/ meltfptr[2]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                /*_.SN2__V12*/ meltfptr[11] = slot;
                                            };
                                            ;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#STRINGlt__L4*/ meltfnum[3] =
                                                melt_string_less((melt_ptr_t)(/*_.SN1__V11*/ meltfptr[10]), (melt_ptr_t)(/*_.SN2__V12*/ meltfptr[11]));;
                                            MELT_LOCATION("warmelt-base.melt:1237:/ cond");
                                            /*cond*/ if (/*_#STRINGlt__L4*/ meltfnum[3]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-base.melt:1238:/ quasiblock");


                                                        /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_4*/ meltfrout->tabval[4]);;
                                                        MELT_LOCATION("warmelt-base.melt:1238:/ putxtraresult");
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                        ;
                                                        /*^finalreturn*/
                                                        ;
                                                        /*finalret*/ goto meltlabend_rout ;
                                                        /*_._IFELSE___V13*/ meltfptr[12] = /*_.RETURN___V14*/ meltfptr[13];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1237:/ clear");
                                                        /*clear*/ /*_.RETURN___V14*/ meltfptr[13] = 0 ;
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
                                                        /*_#STRINGgt__L5*/ meltfnum[4] =
                                                            melt_string_less((melt_ptr_t)(/*_.SN2__V12*/ meltfptr[11]), (melt_ptr_t)(/*_.SN1__V11*/ meltfptr[10]));;
                                                        MELT_LOCATION("warmelt-base.melt:1239:/ cond");
                                                        /*cond*/ if (/*_#STRINGgt__L5*/ meltfnum[4]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-base.melt:1240:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_5*/ meltfrout->tabval[5]);;
                                                                    MELT_LOCATION("warmelt-base.melt:1240:/ putxtraresult");
                                                                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                                    ;
                                                                    /*^finalreturn*/
                                                                    ;
                                                                    /*finalret*/ goto meltlabend_rout ;
                                                                    /*_._IFELSE___V15*/ meltfptr[13] = /*_.RETURN___V16*/ meltfptr[15];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1239:/ clear");
                                                                    /*clear*/ /*_.RETURN___V16*/ meltfptr[15] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-base.melt:1242:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_6*/ meltfrout->tabval[6]);;
                                                                    MELT_LOCATION("warmelt-base.melt:1242:/ putxtraresult");
                                                                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                                    ;
                                                                    /*^finalreturn*/
                                                                    ;
                                                                    /*finalret*/ goto meltlabend_rout ;
                                                                    MELT_LOCATION("warmelt-base.melt:1241:/ quasiblock");


                                                                    /*_.PROGN___V18*/ meltfptr[17] = /*_.RETURN___V17*/ meltfptr[15];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V15*/ meltfptr[13] = /*_.PROGN___V18*/ meltfptr[17];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1239:/ clear");
                                                                    /*clear*/ /*_.RETURN___V17*/ meltfptr[15] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.PROGN___V18*/ meltfptr[17] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V13*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[13];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1237:/ clear");
                                                        /*clear*/ /*_#STRINGgt__L5*/ meltfnum[4] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V15*/ meltfptr[13] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_.LET___V10*/ meltfptr[8] = /*_._IFELSE___V13*/ meltfptr[12];;

                                            MELT_LOCATION("warmelt-base.melt:1234:/ clear");
                                            /*clear*/ /*_.SN1__V11*/ meltfptr[10] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.SN2__V12*/ meltfptr[11] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#STRINGlt__L4*/ meltfnum[3] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V13*/ meltfptr[12] = 0 ;
                                            MELT_LOCATION("warmelt-base.melt:1233:/ quasiblock");


                                            /*_.PROGN___V19*/ meltfptr[15] = /*_.LET___V10*/ meltfptr[8];;
                                            /*^compute*/
                                            /*_._IFELSE___V8*/ meltfptr[6] = /*_.PROGN___V19*/ meltfptr[15];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1231:/ clear");
                                            /*clear*/ /*_.LET___V10*/ meltfptr[8] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PROGN___V19*/ meltfptr[15] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V6*/ meltfptr[4] = /*_._IFELSE___V8*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1229:/ clear");
                                /*clear*/ /*_#IS_NOT_A__L3*/ meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V8*/ meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V4*/ meltfptr[3] = /*_._IFELSE___V6*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1227:/ clear");
                    /*clear*/ /*_#IS_NOT_A__L2*/ meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V6*/ meltfptr[4] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1223:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_._IFELSE___V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-base.melt:1223:/ locexp");
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
        /*clear*/ /*_#eqeq__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("COMPARE_NAMED_ALPHA", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_10_WARMELTmiBASE_COMPARE_NAMED_ALPHA*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                  const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_ADD2OUT_ix = 0, variad_ADD2OUT_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_ADD2OUT_len)
#define melt_variadic_index variad_ADD2OUT_ix

    long current_blocklevel_signals_meltrout_11_WARMELTmiBASE_ADD2OUT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_11_WARMELTmiBASE_ADD2OUT fromline 1752 */

    /** start of frame for meltrout_11_WARMELTmiBASE_ADD2OUT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT// fromline 1568
        : public Melt_CallFrameWithValues<33>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[5];
        /*classy others*/
        double loc_DOUBLE__o0;
        const char* loc_CSTRING__o1;
        melt_treeptr_t loc_TREE__o2;
        melt_gimpleptr_t loc_GIMPLE__o3;
        melt_gimpleseqptr_t loc_GIMPLE_SEQ__o4;
        melt_edgeptr_t loc_EDGE__o5;
        melt_loopptr_t loc_LOOP__o6;
        double loc_DOUBLE__o7;
        const char* loc_CSTRING__o8;
        long _meltsparel;
        void melt_mark_stuff (void)
        {
            /*marking melt_treeptr_t*/
            /*:tree marker*/ gt_ggc_mx_tree_node (loc_TREE__o2);
            /*marking melt_gimpleptr_t*/
            /*:gimple marker*/ gt_ggc_mx_gimple (loc_GIMPLE__o3);
            /*marking melt_gimpleseqptr_t*/
            /*:gimple_seq marker, really gimple marker*/ gt_ggc_mx_gimple (loc_GIMPLE_SEQ__o4);
            /*marking melt_edgeptr_t*/
            /*:edge marker*/ gt_ggc_mx_edge_def (loc_EDGE__o5);
            /*marking melt_loopptr_t*/
            /*:loop marker*/ gt_ggc_mx_loop (loc_LOOP__o6);
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<33> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT), clos) {};
        MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<33> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<33> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT)) {};
        MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<33> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT


    /** end of frame for meltrout_11_WARMELTmiBASE_ADD2OUT fromline 1697**/

    /* end of frame for routine meltrout_11_WARMELTmiBASE_ADD2OUT fromline 1756 */

    /* classy proc frame meltrout_11_WARMELTmiBASE_ADD2OUT */ MeltFrame_meltrout_11_WARMELTmiBASE_ADD2OUT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_11_WARMELTmiBASE_ADD2OUT fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1563:/ getarg");
    /*_.OUT__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        /*_#IS_OUT__L1*/ meltfnum[0] =
            (melt_is_out ((melt_ptr_t) /*_.OUT__V2*/ meltfptr[1])) ;;
        /*^compute*/
        /*_#NOT__L2*/ meltfnum[1] =
            (!(/*_#IS_OUT__L1*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-base.melt:1567:/ cond");
        /*cond*/ if (/*_#NOT__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:1568:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1568:/ locexp");
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
                    /*_._IF___V3*/ meltfptr[2] = /*_.RETURN___V4*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1567:/ clear");
                    /*clear*/ /*_.RETURN___V4*/ meltfptr[3] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1569:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_2:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:1572:/ cond");
                /*cond*/ if (/*ifvariadic nomore*/ variad_ADD2OUT_ix == variad_ADD2OUT_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_ADD2OUT_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:1573:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_ARGLOOP_2;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1572:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-base.melt:1574:/ cond");
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_PTR) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_.V__V7*/ meltfptr[6] =
                                            /*variadic argument value*/ ((meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr)) : NULL);;
                                        /*^compute*/

                                        /*consume variadic Value !*/ variad_ADD2OUT_ix += 1;;

                                        MELT_CHECK_SIGNAL();
                                        ;
                                        /*_#IS_CLOSURE__L3*/ meltfnum[2] =
                                            (melt_magic_discr((melt_ptr_t)(/*_.V__V7*/ meltfptr[6])) == MELTOBMAG_CLOSURE);;
                                        MELT_LOCATION("warmelt-base.melt:1575:/ cond");
                                        /*cond*/ if (/*_#IS_CLOSURE__L3*/ meltfnum[2]) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1577:/ cond");
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_PTR) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_.VV__V9*/ meltfptr[8] =
                                                                    /*variadic argument value*/ ((meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr)) : NULL);;
                                                                /*^compute*/

                                                                /*consume variadic Value !*/ variad_ADD2OUT_ix += 1;;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-base.melt:1578:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[1];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.VV__V9*/ meltfptr[8];
                                                                    /*_.V__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;
                                                                /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V10*/ meltfptr[9];;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1577:/ clear");
                                                                /*clear*/ /*_.VV__V9*/ meltfptr[8] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.V__V10*/ meltfptr[9] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:1579:/ cond");
                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_LONG) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            /*^compute*/
                                                                            /*_#LL__L4*/ meltfnum[3] =
                                                                                /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_long;;
                                                                            /*^compute*/

                                                                            /*consume variadic LONG !*/ variad_ADD2OUT_ix += 1;;

                                                                            MELT_CHECK_SIGNAL();
                                                                            ;
                                                                            MELT_LOCATION("warmelt-base.melt:1580:/ apply");
                                                                            /*apply*/
                                                                            {
                                                                                union meltparam_un argtab[1];
                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                /*^apply.arg*/
                                                                                argtab[0].meltbp_long = /*_#LL__L4*/ meltfnum[3];
                                                                                /*_.V__V11*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                            }
                                                                            ;
                                                                            /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V11*/ meltfptr[8];;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:1579:/ clear");
                                                                            /*clear*/ /*_#LL__L4*/ meltfnum[3] = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_.V__V11*/ meltfptr[8] = 0 ;
                                                                        }
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            MELT_LOCATION("warmelt-base.melt:1581:/ cond");
                                                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_DOUBLE) /*then*/
                                                                                {
                                                                                    /*^cond.then*/
                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        /*^compute*/
                                                                                        /*_?*/ meltfram__.loc_DOUBLE__o0 =
                                                                                            /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_double;;
                                                                                        /*^compute*/

                                                                                        /*consume variadic DOUBLE !*/ variad_ADD2OUT_ix += 1;;

                                                                                        MELT_CHECK_SIGNAL();
                                                                                        ;
                                                                                        MELT_LOCATION("warmelt-base.melt:1582:/ apply");
                                                                                        /*apply*/
                                                                                        {
                                                                                            union meltparam_un argtab[1];
                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                            /*^apply.arg*/
                                                                                            argtab[0].meltbp_double = /*_?*/ meltfram__.loc_DOUBLE__o0;
                                                                                            /*_.V__V12*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_DOUBLE ""), argtab, "", (union meltparam_un*)0);
                                                                                        }
                                                                                        ;
                                                                                        /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V12*/ meltfptr[9];;
                                                                                        /*epilog*/

                                                                                        MELT_LOCATION("warmelt-base.melt:1581:/ clear");
                                                                                        /*clear*/ /*_?*/ meltfram__.loc_DOUBLE__o0 = 0 ;
                                                                                        /*^clear*/
                                                                                        /*clear*/ /*_.V__V12*/ meltfptr[9] = 0 ;
                                                                                    }
                                                                                    ;
                                                                                }
                                                                            else    /*^cond.else*/
                                                                                {

                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        MELT_LOCATION("warmelt-base.melt:1583:/ cond");
                                                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_CSTRING) /*then*/
                                                                                            {
                                                                                                /*^cond.then*/
                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    /*^compute*/
                                                                                                    /*_?*/ meltfram__.loc_CSTRING__o1 =
                                                                                                        /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_cstring;;
                                                                                                    /*^compute*/

                                                                                                    /*consume variadic CSTRING !*/ variad_ADD2OUT_ix += 1;;

                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                    ;
                                                                                                    MELT_LOCATION("warmelt-base.melt:1584:/ apply");
                                                                                                    /*apply*/
                                                                                                    {
                                                                                                        union meltparam_un argtab[1];
                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                        /*^apply.arg*/
                                                                                                        argtab[0].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o1;
                                                                                                        /*_.V__V13*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                                    }
                                                                                                    ;
                                                                                                    /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V13*/ meltfptr[8];;
                                                                                                    /*epilog*/

                                                                                                    MELT_LOCATION("warmelt-base.melt:1583:/ clear");
                                                                                                    /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o1 = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_.V__V13*/ meltfptr[8] = 0 ;
                                                                                                }
                                                                                                ;
                                                                                            }
                                                                                        else    /*^cond.else*/
                                                                                            {

                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    MELT_LOCATION("warmelt-base.melt:1585:/ cond");
                                                                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_TREE) /*then*/
                                                                                                        {
                                                                                                            /*^cond.then*/
                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {

                                                                                                                /*^compute*/
                                                                                                                /*_?*/ meltfram__.loc_TREE__o2 =
                                                                                                                    /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_tree;;
                                                                                                                /*^compute*/

                                                                                                                /*consume variadic TREE !*/ variad_ADD2OUT_ix += 1;;

                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                ;
                                                                                                                MELT_LOCATION("warmelt-base.melt:1586:/ apply");
                                                                                                                /*apply*/
                                                                                                                {
                                                                                                                    union meltparam_un argtab[1];
                                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                                    /*^apply.arg*/
                                                                                                                    argtab[0].meltbp_tree = /*_?*/ meltfram__.loc_TREE__o2;
                                                                                                                    /*_.V__V14*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_TREE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                }
                                                                                                                ;
                                                                                                                /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V14*/ meltfptr[9];;
                                                                                                                /*epilog*/

                                                                                                                MELT_LOCATION("warmelt-base.melt:1585:/ clear");
                                                                                                                /*clear*/ /*_?*/ meltfram__.loc_TREE__o2 = 0 ;
                                                                                                                /*^clear*/
                                                                                                                /*clear*/ /*_.V__V14*/ meltfptr[9] = 0 ;
                                                                                                            }
                                                                                                            ;
                                                                                                        }
                                                                                                    else    /*^cond.else*/
                                                                                                        {

                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {

                                                                                                                MELT_LOCATION("warmelt-base.melt:1587:/ cond");
                                                                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_GIMPLE) /*then*/
                                                                                                                    {
                                                                                                                        /*^cond.then*/
                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {

                                                                                                                            /*^compute*/
                                                                                                                            /*_?*/ meltfram__.loc_GIMPLE__o3 =
                                                                                                                                /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_gimple;;
                                                                                                                            /*^compute*/

                                                                                                                            /*consume variadic GIMPLE !*/ variad_ADD2OUT_ix += 1;;

                                                                                                                            MELT_CHECK_SIGNAL();
                                                                                                                            ;
                                                                                                                            MELT_LOCATION("warmelt-base.melt:1588:/ apply");
                                                                                                                            /*apply*/
                                                                                                                            {
                                                                                                                                union meltparam_un argtab[1];
                                                                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                                                                /*^apply.arg*/
                                                                                                                                argtab[0].meltbp_gimple = /*_?*/ meltfram__.loc_GIMPLE__o3;
                                                                                                                                /*_.V__V15*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_GIMPLE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                            }
                                                                                                                            ;
                                                                                                                            /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V15*/ meltfptr[8];;
                                                                                                                            /*epilog*/

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                                                                            /*clear*/ /*_?*/ meltfram__.loc_GIMPLE__o3 = 0 ;
                                                                                                                            /*^clear*/
                                                                                                                            /*clear*/ /*_.V__V15*/ meltfptr[8] = 0 ;
                                                                                                                        }
                                                                                                                        ;
                                                                                                                    }
                                                                                                                else    /*^cond.else*/
                                                                                                                    {

                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1589:/ cond");
                                                                                                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_GIMPLESEQ) /*then*/
                                                                                                                                {
                                                                                                                                    /*^cond.then*/
                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {

                                                                                                                                        /*^compute*/
                                                                                                                                        /*_?*/ meltfram__.loc_GIMPLE_SEQ__o4 =
                                                                                                                                            /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_gimpleseq;;
                                                                                                                                        /*^compute*/

                                                                                                                                        /*consume variadic GIMPLE_SEQ !*/ variad_ADD2OUT_ix += 1;;

                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                        ;
                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1590:/ apply");
                                                                                                                                        /*apply*/
                                                                                                                                        {
                                                                                                                                            union meltparam_un argtab[1];
                                                                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                                                                            /*^apply.arg*/
                                                                                                                                            argtab[0].meltbp_gimpleseq = /*_?*/ meltfram__.loc_GIMPLE_SEQ__o4;
                                                                                                                                            /*_.V__V16*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_GIMPLESEQ ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                        }
                                                                                                                                        ;
                                                                                                                                        /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V16*/ meltfptr[9];;
                                                                                                                                        /*epilog*/

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1589:/ clear");
                                                                                                                                        /*clear*/ /*_?*/ meltfram__.loc_GIMPLE_SEQ__o4 = 0 ;
                                                                                                                                        /*^clear*/
                                                                                                                                        /*clear*/ /*_.V__V16*/ meltfptr[9] = 0 ;
                                                                                                                                    }
                                                                                                                                    ;
                                                                                                                                }
                                                                                                                            else    /*^cond.else*/
                                                                                                                                {

                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1591:/ cond");
                                                                                                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_EDGE) /*then*/
                                                                                                                                            {
                                                                                                                                                /*^cond.then*/
                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    /*^compute*/
                                                                                                                                                    /*_?*/ meltfram__.loc_EDGE__o5 =
                                                                                                                                                        /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_edge;;
                                                                                                                                                    /*^compute*/

                                                                                                                                                    /*consume variadic EDGE !*/ variad_ADD2OUT_ix += 1;;

                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1592:/ apply");
                                                                                                                                                    /*apply*/
                                                                                                                                                    {
                                                                                                                                                        union meltparam_un argtab[1];
                                                                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                        argtab[0].meltbp_edge = /*_?*/ meltfram__.loc_EDGE__o5;
                                                                                                                                                        /*_.V__V17*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_EDGE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                    }
                                                                                                                                                    ;
                                                                                                                                                    /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V17*/ meltfptr[8];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1591:/ clear");
                                                                                                                                                    /*clear*/ /*_?*/ meltfram__.loc_EDGE__o5 = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.V__V17*/ meltfptr[8] = 0 ;
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        else    /*^cond.else*/
                                                                                                                                            {

                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1593:/ cond");
                                                                                                                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_LOOP) /*then*/
                                                                                                                                                        {
                                                                                                                                                            /*^cond.then*/
                                                                                                                                                            /*^block*/
                                                                                                                                                            /*anyblock*/
                                                                                                                                                            {

                                                                                                                                                                /*^compute*/
                                                                                                                                                                /*_?*/ meltfram__.loc_LOOP__o6 =
                                                                                                                                                                    /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_loop;;
                                                                                                                                                                /*^compute*/

                                                                                                                                                                /*consume variadic LOOP !*/ variad_ADD2OUT_ix += 1;;

                                                                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                                                                ;
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1594:/ apply");
                                                                                                                                                                /*apply*/
                                                                                                                                                                {
                                                                                                                                                                    union meltparam_un argtab[1];
                                                                                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                                                                                    /*^apply.arg*/
                                                                                                                                                                    argtab[0].meltbp_loop = /*_?*/ meltfram__.loc_LOOP__o6;
                                                                                                                                                                    /*_.V__V18*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_LOOP ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                                /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V18*/ meltfptr[9];;
                                                                                                                                                                /*epilog*/

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1593:/ clear");
                                                                                                                                                                /*clear*/ /*_?*/ meltfram__.loc_LOOP__o6 = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.V__V18*/ meltfptr[9] = 0 ;
                                                                                                                                                            }
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else    /*^cond.else*/
                                                                                                                                                        {

                                                                                                                                                            /*^block*/
                                                                                                                                                            /*anyblock*/
                                                                                                                                                            {

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1596:/ quasiblock");


                                                                                                                                                                /*_.VCTY__V20*/ meltfptr[9] =
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
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1599:/ cond");
                                                                                                                                                                /*cond*/ if (
                                                                                                                                                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V20*/ meltfptr[9]),
                                                                                                                                                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
                                                                                                                                                                ) /*then*/
                                                                                                                                                                    {
                                                                                                                                                                        /*^cond.then*/
                                                                                                                                                                        /*^getslot*/
                                                                                                                                                                        {
                                                                                                                                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                                                                                                                                            obj = (melt_ptr_t)(/*_.VCTY__V20*/ meltfptr[9]) /*=obj*/;
                                                                                                                                                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                                                                                                            /*_.NAMED_NAME__V21*/ meltfptr[20] = slot;
                                                                                                                                                                        };
                                                                                                                                                                        ;
                                                                                                                                                                    }
                                                                                                                                                                else    /*^cond.else*/
                                                                                                                                                                    {

                                                                                                                                                                        /*_.NAMED_NAME__V21*/ meltfptr[20] =  /*reallynil*/ NULL ;;
                                                                                                                                                                    }
                                                                                                                                                                ;

                                                                                                                                                                {
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1598:/ locexp");
                                                                                                                                                                    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "ADD2OUT with manipulator for unsupported ctype"),
                                                                                                                                                                           melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V21*/ meltfptr[20])));
                                                                                                                                                                }
                                                                                                                                                                ;

#if MELT_HAVE_DEBUG
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1600:/ cppif.then");
                                                                                                                                                                /*^block*/
                                                                                                                                                                /*anyblock*/
                                                                                                                                                                {


                                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                                    ;
                                                                                                                                                                    /*^cond*/
                                                                                                                                                                    /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                                                                                                                        {
                                                                                                                                                                            /*^cond.then*/
                                                                                                                                                                            /*_._IFELSE___V23*/ meltfptr[22] = (/*nil*/NULL);;
                                                                                                                                                                        }
                                                                                                                                                                    else
                                                                                                                                                                        {
                                                                                                                                                                            MELT_LOCATION("warmelt-base.melt:1600:/ cond.else");

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
                                                                                                                                                                                    argtab[0].meltbp_cstring =  "invalid variadic argument after closure to ADD2OUT";
                                                                                                                                                                                    /*^apply.arg*/
                                                                                                                                                                                    argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                                                                                                                                                    /*^apply.arg*/
                                                                                                                                                                                    argtab[2].meltbp_long = 1600;
                                                                                                                                                                                    /*^apply.arg*/
                                                                                                                                                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V20*/ meltfptr[9];
                                                                                                                                                                                    /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                                                }
                                                                                                                                                                                ;
                                                                                                                                                                                /*_._IFELSE___V23*/ meltfptr[22] = /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23];;
                                                                                                                                                                                /*epilog*/

                                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1600:/ clear");
                                                                                                                                                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] = 0 ;
                                                                                                                                                                            }
                                                                                                                                                                            ;
                                                                                                                                                                        }
                                                                                                                                                                    ;
                                                                                                                                                                    /*_.IFCPP___V22*/ meltfptr[21] = /*_._IFELSE___V23*/ meltfptr[22];;
                                                                                                                                                                    /*epilog*/

                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1600:/ clear");
                                                                                                                                                                    /*clear*/ /*_._IFELSE___V23*/ meltfptr[22] = 0 ;
                                                                                                                                                                }

#else /*MELT_HAVE_DEBUG*/
                                                                                                                                                                /*^cppif.else*/
                                                                                                                                                                /*_.IFCPP___V22*/ meltfptr[21] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                                                                                                ;
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1601:/ quasiblock");


                                                                                                                                                                /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                                                                                                                {
                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1601:/ locexp");
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
                                                                                                                                                                /*_.LET___V19*/ meltfptr[8] = /*_.RETURN___V25*/ meltfptr[23];;

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1596:/ clear");
                                                                                                                                                                /*clear*/ /*_.VCTY__V20*/ meltfptr[9] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.NAMED_NAME__V21*/ meltfptr[20] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.IFCPP___V22*/ meltfptr[21] = 0 ;
                                                                                                                                                                /*^clear*/
                                                                                                                                                                /*clear*/ /*_.RETURN___V25*/ meltfptr[23] = 0 ;
                                                                                                                                                                /*_._IFELSE___V8*/ meltfptr[7] = /*_.LET___V19*/ meltfptr[8];;
                                                                                                                                                                /*epilog*/

                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1593:/ clear");
                                                                                                                                                                /*clear*/ /*_.LET___V19*/ meltfptr[8] = 0 ;
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
                                                        }
                                                    ;
                                                    /*epilog*/
                                                }
                                                ;
                                            }
                                        else
                                            {
                                                MELT_LOCATION("warmelt-base.melt:1575:/ cond.else");

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1603:/ msend");
                                                    /*msend*/
                                                    {
                                                        union meltparam_un argtab[1];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^ojbmsend.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OUT__V2*/ meltfptr[1];
                                                        /*_.ADD_TO_OUT__V26*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)((/*!ADD_TO_OUT*/ meltfrout->tabval[2])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_._IFELSE___V8*/ meltfptr[7] = /*_.ADD_TO_OUT__V26*/ meltfptr[22];;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1575:/ clear");
                                                    /*clear*/ /*_.ADD_TO_OUT__V26*/ meltfptr[22] = 0 ;
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:1574:/ clear");
                                        /*clear*/ /*_.V__V7*/ meltfptr[6] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_#IS_CLOSURE__L3*/ meltfnum[2] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_._IFELSE___V8*/ meltfptr[7] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        MELT_LOCATION("warmelt-base.melt:1604:/ cond");
                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_LONG) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    /*^compute*/
                                                    /*_#N__L5*/ meltfnum[3] =
                                                        /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_long;;
                                                    /*^compute*/

                                                    /*consume variadic LONG !*/ variad_ADD2OUT_ix += 1;;

                                                    {
                                                        MELT_LOCATION("warmelt-base.melt:1605:/ locexp");
                                                        meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (/*_#N__L5*/ meltfnum[3]));
                                                    }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1604:/ clear");
                                                    /*clear*/ /*_#N__L5*/ meltfnum[3] = 0 ;
                                                }
                                                ;
                                            }
                                        else    /*^cond.else*/
                                            {

                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1606:/ cond");
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_DOUBLE) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_?*/ meltfram__.loc_DOUBLE__o7 =
                                                                    /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_double;;
                                                                /*^compute*/

                                                                /*consume variadic DOUBLE !*/ variad_ADD2OUT_ix += 1;;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1607:/ locexp");
                                                                    /*add2out_double*/
                                                                    char dblbuf[64];
                                                                    memset (dblbuf, 0, sizeof(dblbuf));
                                                                    snprintf(dblbuf, sizeof(dblbuf), "%.3f", /*_?*/ meltfram__.loc_DOUBLE__o7);
                                                                    if (strtod (dblbuf, NULL) != /*_?*/ meltfram__.loc_DOUBLE__o7)
                                                                        snprintf(dblbuf, sizeof(dblbuf), "%.7f", /*_?*/ meltfram__.loc_DOUBLE__o7);
                                                                    if (strtod (dblbuf, NULL) != /*_?*/ meltfram__.loc_DOUBLE__o7)
                                                                        snprintf(dblbuf, sizeof(dblbuf), "%.15f", /*_?*/ meltfram__.loc_DOUBLE__o7);
                                                                    meltgc_add_out_raw ((melt_ptr_t) /*_.OUT__V2*/ meltfptr[1], dblbuf);
                                                                    /*end add2out_double*/;
                                                                }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1606:/ clear");
                                                                /*clear*/ /*_?*/ meltfram__.loc_DOUBLE__o7 = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:1608:/ cond");
                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_CSTRING) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            /*^compute*/
                                                                            /*_?*/ meltfram__.loc_CSTRING__o8 =
                                                                                /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_cstring;;
                                                                            /*^compute*/

                                                                            /*consume variadic CSTRING !*/ variad_ADD2OUT_ix += 1;;

                                                                            {
                                                                                MELT_LOCATION("warmelt-base.melt:1609:/ locexp");
                                                                                meltgc_add_out((melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (/*_?*/ meltfram__.loc_CSTRING__o8));
                                                                            }
                                                                            ;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:1608:/ clear");
                                                                            /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o8 = 0 ;
                                                                        }
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            MELT_LOCATION("warmelt-base.melt:1611:/ quasiblock");


                                                                            /*_.VCTY__V28*/ meltfptr[20] =
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
                                                                            MELT_LOCATION("warmelt-base.melt:1614:/ cond");
                                                                            /*cond*/ if (
                                                                                /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V28*/ meltfptr[20]),
                                                                                                              (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
                                                                            ) /*then*/
                                                                                {
                                                                                    /*^cond.then*/
                                                                                    /*^getslot*/
                                                                                    {
                                                                                        melt_ptr_t slot=NULL, obj=NULL;
                                                                                        obj = (melt_ptr_t)(/*_.VCTY__V28*/ meltfptr[20]) /*=obj*/;
                                                                                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                        /*_.NAMED_NAME__V29*/ meltfptr[21] = slot;
                                                                                    };
                                                                                    ;
                                                                                }
                                                                            else    /*^cond.else*/
                                                                                {

                                                                                    /*_.NAMED_NAME__V29*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                                                                                }
                                                                            ;

                                                                            {
                                                                                MELT_LOCATION("warmelt-base.melt:1613:/ locexp");
                                                                                error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "ADD2OUT for unsupported ctype; use a manipulator like OUTPUT_TREE "),
                                                                                       melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V29*/ meltfptr[21])));
                                                                            }
                                                                            ;

#if MELT_HAVE_DEBUG
                                                                            MELT_LOCATION("warmelt-base.melt:1615:/ cppif.then");
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {


                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                /*^cond*/
                                                                                /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                                    {
                                                                                        /*^cond.then*/
                                                                                        /*_._IFELSE___V31*/ meltfptr[8] = (/*nil*/NULL);;
                                                                                    }
                                                                                else
                                                                                    {
                                                                                        MELT_LOCATION("warmelt-base.melt:1615:/ cond.else");

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
                                                                                                argtab[0].meltbp_cstring =  "invalid variadic argument to ADD2OUT";
                                                                                                /*^apply.arg*/
                                                                                                argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                                                                /*^apply.arg*/
                                                                                                argtab[2].meltbp_long = 1615;
                                                                                                /*^apply.arg*/
                                                                                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V28*/ meltfptr[20];
                                                                                                /*_.MELT_ASSERT_FAILURE_FUN__V32*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                            }
                                                                                            ;
                                                                                            /*_._IFELSE___V31*/ meltfptr[8] = /*_.MELT_ASSERT_FAILURE_FUN__V32*/ meltfptr[22];;
                                                                                            /*epilog*/

                                                                                            MELT_LOCATION("warmelt-base.melt:1615:/ clear");
                                                                                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V32*/ meltfptr[22] = 0 ;
                                                                                        }
                                                                                        ;
                                                                                    }
                                                                                ;
                                                                                /*_.IFCPP___V30*/ meltfptr[23] = /*_._IFELSE___V31*/ meltfptr[8];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-base.melt:1615:/ clear");
                                                                                /*clear*/ /*_._IFELSE___V31*/ meltfptr[8] = 0 ;
                                                                            }

#else /*MELT_HAVE_DEBUG*/
                                                                            /*^cppif.else*/
                                                                            /*_.IFCPP___V30*/ meltfptr[23] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                            ;
                                                                            /*^compute*/
                                                                            /*_.LET___V27*/ meltfptr[9] = /*_.IFCPP___V30*/ meltfptr[23];;

                                                                            MELT_LOCATION("warmelt-base.melt:1611:/ clear");
                                                                            /*clear*/ /*_.VCTY__V28*/ meltfptr[20] = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_.NAMED_NAME__V29*/ meltfptr[21] = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_.IFCPP___V30*/ meltfptr[23] = 0 ;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:1608:/ clear");
                                                                            /*clear*/ /*_.LET___V27*/ meltfptr[9] = 0 ;
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
                    }
                ;
                /*epilog*/
            }
            ;
            ;
            goto meltlabloop_ARGLOOP_2;
meltlabexit_ARGLOOP_2:
            ;
            MELT_LOCATION("warmelt-base.melt:1569:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V5*/ meltfptr[3] = /*_.ARGLOOP__V6*/ meltfptr[5];;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1618:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.OUT__V2*/ meltfptr[1];;

        {
            MELT_LOCATION("warmelt-base.melt:1618:/ locexp");
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1563:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.RETURN___V33*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-base.melt:1563:/ locexp");
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
        /*clear*/ /*_#IS_OUT__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V5*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V33*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_11_WARMELTmiBASE_ADD2OUT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_11_WARMELTmiBASE_ADD2OUT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_12_WARMELTmiBASE_ADD2LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                   const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_ADD2LIST_ix = 0, variad_ADD2LIST_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_ADD2LIST_len)
#define melt_variadic_index variad_ADD2LIST_ix

    long current_blocklevel_signals_meltrout_12_WARMELTmiBASE_ADD2LIST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_12_WARMELTmiBASE_ADD2LIST fromline 1752 */

    /** start of frame for meltrout_12_WARMELTmiBASE_ADD2LIST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST// fromline 1568
        : public Melt_CallFrameWithValues<19>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[3];
        /*classy others*/
        const char* loc_CSTRING__o0;
        melt_treeptr_t loc_TREE__o1;
        melt_gimpleptr_t loc_GIMPLE__o2;
        melt_edgeptr_t loc_EDGE__o3;
        long _meltsparel;
        void melt_mark_stuff (void)
        {
            /*marking melt_treeptr_t*/
            /*:tree marker*/ gt_ggc_mx_tree_node (loc_TREE__o1);
            /*marking melt_gimpleptr_t*/
            /*:gimple marker*/ gt_ggc_mx_gimple (loc_GIMPLE__o2);
            /*marking melt_edgeptr_t*/
            /*:edge marker*/ gt_ggc_mx_edge_def (loc_EDGE__o3);
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST), clos) {};
        MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST)) {};
        MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST


    /** end of frame for meltrout_12_WARMELTmiBASE_ADD2LIST fromline 1697**/

    /* end of frame for routine meltrout_12_WARMELTmiBASE_ADD2LIST fromline 1756 */

    /* classy proc frame meltrout_12_WARMELTmiBASE_ADD2LIST */ MeltFrame_meltrout_12_WARMELTmiBASE_ADD2LIST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_12_WARMELTmiBASE_ADD2LIST fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2LIST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1624:/ getarg");
    /*_.LIS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        /*_#IS_LIST__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
        /*^compute*/
        /*_#NOT__L2*/ meltfnum[1] =
            (!(/*_#IS_LIST__L1*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-base.melt:1628:/ cond");
        /*cond*/ if (/*_#NOT__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:1629:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-base.melt:1629:/ locexp");
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
                    /*_._IF___V3*/ meltfptr[2] = /*_.RETURN___V4*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1628:/ clear");
                    /*clear*/ /*_.RETURN___V4*/ meltfptr[3] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1630:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_3:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:1633:/ cond");
                /*cond*/ if (/*ifvariadic nomore*/ variad_ADD2LIST_ix == variad_ADD2LIST_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_ADD2LIST_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:1634:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_ARGLOOP_3;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1633:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-base.melt:1635:/ cond");
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2LIST_ix>=0 && variad_ADD2LIST_ix + 1 <= variad_ADD2LIST_len && meltxargdescr_[variad_ADD2LIST_ix]== MELTBPAR_PTR) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_.V__V7*/ meltfptr[6] =
                                            /*variadic argument value*/ ((meltxargtab_[variad_ADD2LIST_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_ADD2LIST_ix + 0].meltbp_aptr)) : NULL);;
                                        /*^compute*/

                                        /*consume variadic Value !*/ variad_ADD2LIST_ix += 1;;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:1636:/ locexp");
                                            meltgc_append_list((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1]), (melt_ptr_t)(/*_.V__V7*/ meltfptr[6]));
                                        }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:1635:/ clear");
                                        /*clear*/ /*_.V__V7*/ meltfptr[6] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        MELT_LOCATION("warmelt-base.melt:1637:/ cond");
                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2LIST_ix>=0 && variad_ADD2LIST_ix + 1 <= variad_ADD2LIST_len && meltxargdescr_[variad_ADD2LIST_ix]== MELTBPAR_LONG) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    /*^compute*/
                                                    /*_#L__L3*/ meltfnum[2] =
                                                        /*variadic argument stuff*/ meltxargtab_[variad_ADD2LIST_ix + 0].meltbp_long;;
                                                    /*^compute*/

                                                    /*consume variadic LONG !*/ variad_ADD2LIST_ix += 1;;
                                                    /*_.BOX__V8*/ meltfptr[6] =
                                                        /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#L__L3*/ meltfnum[2]));;

                                                    {
                                                        MELT_LOCATION("warmelt-base.melt:1638:/ locexp");
                                                        meltgc_append_list((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1]), (melt_ptr_t)(/*_.BOX__V8*/ meltfptr[6]));
                                                    }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1637:/ clear");
                                                    /*clear*/ /*_#L__L3*/ meltfnum[2] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.BOX__V8*/ meltfptr[6] = 0 ;
                                                }
                                                ;
                                            }
                                        else    /*^cond.else*/
                                            {

                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1639:/ cond");
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2LIST_ix>=0 && variad_ADD2LIST_ix + 1 <= variad_ADD2LIST_len && meltxargdescr_[variad_ADD2LIST_ix]== MELTBPAR_CSTRING) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_?*/ meltfram__.loc_CSTRING__o0 =
                                                                    /*variadic argument stuff*/ meltxargtab_[variad_ADD2LIST_ix + 0].meltbp_cstring;;
                                                                /*^compute*/

                                                                /*consume variadic CSTRING !*/ variad_ADD2LIST_ix += 1;;
                                                                /*_.BOX__V9*/ meltfptr[6] =
                                                                    /*full constboxing*/ /*ctype_cstring boxing*/ meltgc_new_stringdup((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_STRING)))), (/*_?*/ meltfram__.loc_CSTRING__o0));;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1640:/ locexp");
                                                                    meltgc_append_list((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1]), (melt_ptr_t)(/*_.BOX__V9*/ meltfptr[6]));
                                                                }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1639:/ clear");
                                                                /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o0 = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.BOX__V9*/ meltfptr[6] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:1641:/ cond");
                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2LIST_ix>=0 && variad_ADD2LIST_ix + 1 <= variad_ADD2LIST_len && meltxargdescr_[variad_ADD2LIST_ix]== MELTBPAR_TREE) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            /*^compute*/
                                                                            /*_?*/ meltfram__.loc_TREE__o1 =
                                                                                /*variadic argument stuff*/ meltxargtab_[variad_ADD2LIST_ix + 0].meltbp_tree;;
                                                                            /*^compute*/

                                                                            /*consume variadic TREE !*/ variad_ADD2LIST_ix += 1;;
                                                                            /*_.BOX__V10*/ meltfptr[6] =
                                                                                /*full constboxing*/ meltgc_new_tree((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_TREE)))), (/*_?*/ meltfram__.loc_TREE__o1));;

                                                                            {
                                                                                MELT_LOCATION("warmelt-base.melt:1642:/ locexp");
                                                                                meltgc_append_list((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1]), (melt_ptr_t)(/*_.BOX__V10*/ meltfptr[6]));
                                                                            }
                                                                            ;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:1641:/ clear");
                                                                            /*clear*/ /*_?*/ meltfram__.loc_TREE__o1 = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_.BOX__V10*/ meltfptr[6] = 0 ;
                                                                        }
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            MELT_LOCATION("warmelt-base.melt:1643:/ cond");
                                                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2LIST_ix>=0 && variad_ADD2LIST_ix + 1 <= variad_ADD2LIST_len && meltxargdescr_[variad_ADD2LIST_ix]== MELTBPAR_GIMPLE) /*then*/
                                                                                {
                                                                                    /*^cond.then*/
                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        /*^compute*/
                                                                                        /*_?*/ meltfram__.loc_GIMPLE__o2 =
                                                                                            /*variadic argument stuff*/ meltxargtab_[variad_ADD2LIST_ix + 0].meltbp_gimple;;
                                                                                        /*^compute*/

                                                                                        /*consume variadic GIMPLE !*/ variad_ADD2LIST_ix += 1;;
                                                                                        /*_.BOX__V11*/ meltfptr[6] =
                                                                                            /*full constboxing*/ meltgc_new_gimple((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_GIMPLE)))), (/*_?*/ meltfram__.loc_GIMPLE__o2));;

                                                                                        {
                                                                                            MELT_LOCATION("warmelt-base.melt:1644:/ locexp");
                                                                                            meltgc_append_list((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1]), (melt_ptr_t)(/*_.BOX__V11*/ meltfptr[6]));
                                                                                        }
                                                                                        ;
                                                                                        /*epilog*/

                                                                                        MELT_LOCATION("warmelt-base.melt:1643:/ clear");
                                                                                        /*clear*/ /*_?*/ meltfram__.loc_GIMPLE__o2 = 0 ;
                                                                                        /*^clear*/
                                                                                        /*clear*/ /*_.BOX__V11*/ meltfptr[6] = 0 ;
                                                                                    }
                                                                                    ;
                                                                                }
                                                                            else    /*^cond.else*/
                                                                                {

                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        MELT_LOCATION("warmelt-base.melt:1645:/ cond");
                                                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2LIST_ix>=0 && variad_ADD2LIST_ix + 1 <= variad_ADD2LIST_len && meltxargdescr_[variad_ADD2LIST_ix]== MELTBPAR_EDGE) /*then*/
                                                                                            {
                                                                                                /*^cond.then*/
                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    /*^compute*/
                                                                                                    /*_?*/ meltfram__.loc_EDGE__o3 =
                                                                                                        /*variadic argument stuff*/ meltxargtab_[variad_ADD2LIST_ix + 0].meltbp_edge;;
                                                                                                    /*^compute*/

                                                                                                    /*consume variadic EDGE !*/ variad_ADD2LIST_ix += 1;;
                                                                                                    /*_.BOX__V12*/ meltfptr[6] =
                                                                                                        /*full constboxing*/ meltgc_new_edge((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_EDGE)))), (/*_?*/ meltfram__.loc_EDGE__o3));;

                                                                                                    {
                                                                                                        MELT_LOCATION("warmelt-base.melt:1646:/ locexp");
                                                                                                        meltgc_append_list((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1]), (melt_ptr_t)(/*_.BOX__V12*/ meltfptr[6]));
                                                                                                    }
                                                                                                    ;
                                                                                                    /*epilog*/

                                                                                                    MELT_LOCATION("warmelt-base.melt:1645:/ clear");
                                                                                                    /*clear*/ /*_?*/ meltfram__.loc_EDGE__o3 = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_.BOX__V12*/ meltfptr[6] = 0 ;
                                                                                                }
                                                                                                ;
                                                                                            }
                                                                                        else    /*^cond.else*/
                                                                                            {

                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    MELT_LOCATION("warmelt-base.melt:1648:/ quasiblock");


                                                                                                    /*_.VCTY__V14*/ meltfptr[13] =
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
                                                                                                    MELT_LOCATION("warmelt-base.melt:1651:/ cond");
                                                                                                    /*cond*/ if (
                                                                                                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V14*/ meltfptr[13]),
                                                                                                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
                                                                                                    ) /*then*/
                                                                                                        {
                                                                                                            /*^cond.then*/
                                                                                                            /*^getslot*/
                                                                                                            {
                                                                                                                melt_ptr_t slot=NULL, obj=NULL;
                                                                                                                obj = (melt_ptr_t)(/*_.VCTY__V14*/ meltfptr[13]) /*=obj*/;
                                                                                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                                                /*_.NAMED_NAME__V15*/ meltfptr[14] = slot;
                                                                                                            };
                                                                                                            ;
                                                                                                        }
                                                                                                    else    /*^cond.else*/
                                                                                                        {

                                                                                                            /*_.NAMED_NAME__V15*/ meltfptr[14] =  /*reallynil*/ NULL ;;
                                                                                                        }
                                                                                                    ;

                                                                                                    {
                                                                                                        MELT_LOCATION("warmelt-base.melt:1650:/ locexp");
                                                                                                        error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "ADD2LIST for unsupported ctype "),
                                                                                                               melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V15*/ meltfptr[14])));
                                                                                                    }
                                                                                                    ;

#if MELT_HAVE_DEBUG
                                                                                                    MELT_LOCATION("warmelt-base.melt:1652:/ cppif.then");
                                                                                                    /*^block*/
                                                                                                    /*anyblock*/
                                                                                                    {


                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                        ;
                                                                                                        /*^cond*/
                                                                                                        /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                                                            {
                                                                                                                /*^cond.then*/
                                                                                                                /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                                                                                                            }
                                                                                                        else
                                                                                                            {
                                                                                                                MELT_LOCATION("warmelt-base.melt:1652:/ cond.else");

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
                                                                                                                        argtab[0].meltbp_cstring =  "invalid variadic argument to ADD2LIST";
                                                                                                                        /*^apply.arg*/
                                                                                                                        argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                                                                                        /*^apply.arg*/
                                                                                                                        argtab[2].meltbp_long = 1652;
                                                                                                                        /*^apply.arg*/
                                                                                                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V14*/ meltfptr[13];
                                                                                                                        /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                                                    }
                                                                                                                    ;
                                                                                                                    /*_._IFELSE___V17*/ meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                                                                                                                    /*epilog*/

                                                                                                                    MELT_LOCATION("warmelt-base.melt:1652:/ clear");
                                                                                                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                                                                                                                }
                                                                                                                ;
                                                                                                            }
                                                                                                        ;
                                                                                                        /*_.IFCPP___V16*/ meltfptr[15] = /*_._IFELSE___V17*/ meltfptr[16];;
                                                                                                        /*epilog*/

                                                                                                        MELT_LOCATION("warmelt-base.melt:1652:/ clear");
                                                                                                        /*clear*/ /*_._IFELSE___V17*/ meltfptr[16] = 0 ;
                                                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                                                    /*^cppif.else*/
                                                                                                    /*_.IFCPP___V16*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                                    ;
                                                                                                    /*^compute*/
                                                                                                    /*_.LET___V13*/ meltfptr[6] = /*_.IFCPP___V16*/ meltfptr[15];;

                                                                                                    MELT_LOCATION("warmelt-base.melt:1648:/ clear");
                                                                                                    /*clear*/ /*_.VCTY__V14*/ meltfptr[13] = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_.NAMED_NAME__V15*/ meltfptr[14] = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_.IFCPP___V16*/ meltfptr[15] = 0 ;
                                                                                                    /*epilog*/

                                                                                                    MELT_LOCATION("warmelt-base.melt:1645:/ clear");
                                                                                                    /*clear*/ /*_.LET___V13*/ meltfptr[6] = 0 ;
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
            goto meltlabloop_ARGLOOP_3;
meltlabexit_ARGLOOP_3:
            ;
            MELT_LOCATION("warmelt-base.melt:1630:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V5*/ meltfptr[3] = /*_.ARGLOOP__V6*/ meltfptr[5];;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1654:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LIS__V2*/ meltfptr[1];;

        {
            MELT_LOCATION("warmelt-base.melt:1654:/ locexp");
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1624:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.RETURN___V19*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-base.melt:1624:/ locexp");
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
        /*clear*/ /*_#IS_LIST__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V5*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V19*/ meltfptr[17] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2LIST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_12_WARMELTmiBASE_ADD2LIST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_12_WARMELTmiBASE_ADD2LIST*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                     const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_STRING4OUT_ix = 0, variad_STRING4OUT_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_STRING4OUT_len)
#define melt_variadic_index variad_STRING4OUT_ix

    long current_blocklevel_signals_meltrout_13_WARMELTmiBASE_STRING4OUT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_13_WARMELTmiBASE_STRING4OUT fromline 1752 */

    /** start of frame for meltrout_13_WARMELTmiBASE_STRING4OUT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT// fromline 1568
        : public Melt_CallFrameWithValues<44>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[5];
        /*classy others*/
        const char* loc_CSTRING__o0;
        melt_treeptr_t loc_TREE__o1;
        melt_gimpleptr_t loc_GIMPLE__o2;
        melt_gimpleseqptr_t loc_GIMPLE_SEQ__o3;
        melt_edgeptr_t loc_EDGE__o4;
        melt_loopptr_t loc_LOOP__o5;
        const char* loc_CSTRING__o6;
        long _meltsparel;
        void melt_mark_stuff (void)
        {
            /*marking melt_treeptr_t*/
            /*:tree marker*/ gt_ggc_mx_tree_node (loc_TREE__o1);
            /*marking melt_gimpleptr_t*/
            /*:gimple marker*/ gt_ggc_mx_gimple (loc_GIMPLE__o2);
            /*marking melt_gimpleseqptr_t*/
            /*:gimple_seq marker, really gimple marker*/ gt_ggc_mx_gimple (loc_GIMPLE_SEQ__o3);
            /*marking melt_edgeptr_t*/
            /*:edge marker*/ gt_ggc_mx_edge_def (loc_EDGE__o4);
            /*marking melt_loopptr_t*/
            /*:loop marker*/ gt_ggc_mx_loop (loc_LOOP__o5);
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<44> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT), clos) {};
        MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<44> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<44> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT)) {};
        MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<44> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT


    /** end of frame for meltrout_13_WARMELTmiBASE_STRING4OUT fromline 1697**/

    /* end of frame for routine meltrout_13_WARMELTmiBASE_STRING4OUT fromline 1756 */

    /* classy proc frame meltrout_13_WARMELTmiBASE_STRING4OUT */ MeltFrame_meltrout_13_WARMELTmiBASE_STRING4OUT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_13_WARMELTmiBASE_STRING4OUT fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("STRING4OUT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1658:/ getarg");
    /*_.DIS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        MELT_LOCATION("warmelt-base.melt:1661:/ cond");
        /*cond*/ if (/*_.DIS__V2*/ meltfptr[1]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V3*/ meltfptr[2] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1661:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.DIS__V2*/ meltfptr[1] = /*_.SETQ___V4*/ meltfptr[3] = (/*!DISCR_STRING*/ meltfrout->tabval[0]);;
                    /*^quasiblock*/


                    /*_.PROGN___V5*/ meltfptr[4] = /*_.SETQ___V4*/ meltfptr[3];;
                    /*^compute*/
                    /*_._IFELSE___V3*/ meltfptr[2] = /*_.PROGN___V5*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1661:/ clear");
                    /*clear*/ /*_.SETQ___V4*/ meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V5*/ meltfptr[4] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L1*/ meltfnum[0] =
            melt_is_instance_of((melt_ptr_t)(/*_.DIS__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_DISCRIMINANT*/ meltfrout->tabval[1])));;
        MELT_LOCATION("warmelt-base.melt:1662:/ cond");
        /*cond*/ if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V6*/ meltfptr[3] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1662:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:1663:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1663:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:1662:/ quasiblock");


                    /*_.PROGN___V8*/ meltfptr[7] = /*_.RETURN___V7*/ meltfptr[4];;
                    /*^compute*/
                    /*_._IFELSE___V6*/ meltfptr[3] = /*_.PROGN___V8*/ meltfptr[7];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1662:/ clear");
                    /*clear*/ /*_.RETURN___V7*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V8*/ meltfptr[7] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1664:/ quasiblock");


        /*_#GOODMAG__L2*/ meltfnum[1] = 0;;

        {
            MELT_LOCATION("warmelt-base.melt:1666:/ locexp");
            /* string4out GETMAGIDISCHK__1 */ /*_#GOODMAG__L2*/ meltfnum[1] =
                ((meltobject_ptr_t)/*_.DIS__V2*/ meltfptr[1])->meltobj_magic == MELTOBMAG_STRING ;;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1669:/ cond");
        /*cond*/ if (/*_#GOODMAG__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V10*/ meltfptr[7] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1669:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^quasiblock*/


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1669:/ locexp");
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
                    /*^quasiblock*/


                    /*_.PROGN___V12*/ meltfptr[11] = /*_.RETURN___V11*/ meltfptr[10];;
                    /*^compute*/
                    /*_._IFELSE___V10*/ meltfptr[7] = /*_.PROGN___V12*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1669:/ clear");
                    /*clear*/ /*_.RETURN___V11*/ meltfptr[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V12*/ meltfptr[11] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V9*/ meltfptr[4] = /*_._IFELSE___V10*/ meltfptr[7];;

        MELT_LOCATION("warmelt-base.melt:1664:/ clear");
        /*clear*/ /*_#GOODMAG__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V10*/ meltfptr[7] = 0 ;
        MELT_LOCATION("warmelt-base.melt:1670:/ quasiblock");


        /*_.OUT__V14*/ meltfptr[11] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[2])), (const char*)0);;
        MELT_LOCATION("warmelt-base.melt:1672:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_4:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:1675:/ cond");
                /*cond*/ if (/*ifvariadic nomore*/ variad_STRING4OUT_ix == variad_STRING4OUT_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_STRING4OUT_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:1676:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V16*/ meltfptr[15] =  /*reallynil*/ NULL ;;

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
                        MELT_LOCATION("warmelt-base.melt:1675:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-base.melt:1677:/ cond");
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_PTR) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_.V__V17*/ meltfptr[16] =
                                            /*variadic argument value*/ ((meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr)) : NULL);;
                                        /*^compute*/

                                        /*consume variadic Value !*/ variad_STRING4OUT_ix += 1;;

                                        MELT_CHECK_SIGNAL();
                                        ;
                                        /*_#IS_CLOSURE__L3*/ meltfnum[1] =
                                            (melt_magic_discr((melt_ptr_t)(/*_.V__V17*/ meltfptr[16])) == MELTOBMAG_CLOSURE);;
                                        MELT_LOCATION("warmelt-base.melt:1678:/ cond");
                                        /*cond*/ if (/*_#IS_CLOSURE__L3*/ meltfnum[1]) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1680:/ cond");
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_PTR) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_.VV__V19*/ meltfptr[18] =
                                                                    /*variadic argument value*/ ((meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr)) : NULL);;
                                                                /*^compute*/

                                                                /*consume variadic Value !*/ variad_STRING4OUT_ix += 1;;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-base.melt:1681:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[1];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.VV__V19*/ meltfptr[18];
                                                                    /*_.V__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;
                                                                /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V20*/ meltfptr[19];;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1680:/ clear");
                                                                /*clear*/ /*_.VV__V19*/ meltfptr[18] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.V__V20*/ meltfptr[19] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:1682:/ cond");
                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_LONG) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            /*^compute*/
                                                                            /*_#LL__L4*/ meltfnum[3] =
                                                                                /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_long;;
                                                                            /*^compute*/

                                                                            /*consume variadic LONG !*/ variad_STRING4OUT_ix += 1;;

                                                                            MELT_CHECK_SIGNAL();
                                                                            ;
                                                                            MELT_LOCATION("warmelt-base.melt:1683:/ apply");
                                                                            /*apply*/
                                                                            {
                                                                                union meltparam_un argtab[1];
                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                /*^apply.arg*/
                                                                                argtab[0].meltbp_long = /*_#LL__L4*/ meltfnum[3];
                                                                                /*_.V__V21*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                            }
                                                                            ;
                                                                            /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V21*/ meltfptr[18];;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:1682:/ clear");
                                                                            /*clear*/ /*_#LL__L4*/ meltfnum[3] = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_.V__V21*/ meltfptr[18] = 0 ;
                                                                        }
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            MELT_LOCATION("warmelt-base.melt:1684:/ cond");
                                                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_CSTRING) /*then*/
                                                                                {
                                                                                    /*^cond.then*/
                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        /*^compute*/
                                                                                        /*_?*/ meltfram__.loc_CSTRING__o0 =
                                                                                            /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_cstring;;
                                                                                        /*^compute*/

                                                                                        /*consume variadic CSTRING !*/ variad_STRING4OUT_ix += 1;;

                                                                                        MELT_CHECK_SIGNAL();
                                                                                        ;
                                                                                        MELT_LOCATION("warmelt-base.melt:1685:/ apply");
                                                                                        /*apply*/
                                                                                        {
                                                                                            union meltparam_un argtab[1];
                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                            /*^apply.arg*/
                                                                                            argtab[0].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o0;
                                                                                            /*_.V__V22*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                        }
                                                                                        ;
                                                                                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V22*/ meltfptr[19];;
                                                                                        /*epilog*/

                                                                                        MELT_LOCATION("warmelt-base.melt:1684:/ clear");
                                                                                        /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o0 = 0 ;
                                                                                        /*^clear*/
                                                                                        /*clear*/ /*_.V__V22*/ meltfptr[19] = 0 ;
                                                                                    }
                                                                                    ;
                                                                                }
                                                                            else    /*^cond.else*/
                                                                                {

                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        MELT_LOCATION("warmelt-base.melt:1686:/ cond");
                                                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_TREE) /*then*/
                                                                                            {
                                                                                                /*^cond.then*/
                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    /*^compute*/
                                                                                                    /*_?*/ meltfram__.loc_TREE__o1 =
                                                                                                        /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_tree;;
                                                                                                    /*^compute*/

                                                                                                    /*consume variadic TREE !*/ variad_STRING4OUT_ix += 1;;

                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                    ;
                                                                                                    MELT_LOCATION("warmelt-base.melt:1687:/ apply");
                                                                                                    /*apply*/
                                                                                                    {
                                                                                                        union meltparam_un argtab[1];
                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                        /*^apply.arg*/
                                                                                                        argtab[0].meltbp_tree = /*_?*/ meltfram__.loc_TREE__o1;
                                                                                                        /*_.V__V23*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_TREE ""), argtab, "", (union meltparam_un*)0);
                                                                                                    }
                                                                                                    ;
                                                                                                    /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V23*/ meltfptr[18];;
                                                                                                    /*epilog*/

                                                                                                    MELT_LOCATION("warmelt-base.melt:1686:/ clear");
                                                                                                    /*clear*/ /*_?*/ meltfram__.loc_TREE__o1 = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_.V__V23*/ meltfptr[18] = 0 ;
                                                                                                }
                                                                                                ;
                                                                                            }
                                                                                        else    /*^cond.else*/
                                                                                            {

                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    MELT_LOCATION("warmelt-base.melt:1688:/ cond");
                                                                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_GIMPLE) /*then*/
                                                                                                        {
                                                                                                            /*^cond.then*/
                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {

                                                                                                                /*^compute*/
                                                                                                                /*_?*/ meltfram__.loc_GIMPLE__o2 =
                                                                                                                    /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_gimple;;
                                                                                                                /*^compute*/

                                                                                                                /*consume variadic GIMPLE !*/ variad_STRING4OUT_ix += 1;;

                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                ;
                                                                                                                MELT_LOCATION("warmelt-base.melt:1689:/ apply");
                                                                                                                /*apply*/
                                                                                                                {
                                                                                                                    union meltparam_un argtab[1];
                                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                                    /*^apply.arg*/
                                                                                                                    argtab[0].meltbp_gimple = /*_?*/ meltfram__.loc_GIMPLE__o2;
                                                                                                                    /*_.V__V24*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_GIMPLE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                }
                                                                                                                ;
                                                                                                                /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V24*/ meltfptr[19];;
                                                                                                                /*epilog*/

                                                                                                                MELT_LOCATION("warmelt-base.melt:1688:/ clear");
                                                                                                                /*clear*/ /*_?*/ meltfram__.loc_GIMPLE__o2 = 0 ;
                                                                                                                /*^clear*/
                                                                                                                /*clear*/ /*_.V__V24*/ meltfptr[19] = 0 ;
                                                                                                            }
                                                                                                            ;
                                                                                                        }
                                                                                                    else    /*^cond.else*/
                                                                                                        {

                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {

                                                                                                                MELT_LOCATION("warmelt-base.melt:1690:/ cond");
                                                                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_GIMPLESEQ) /*then*/
                                                                                                                    {
                                                                                                                        /*^cond.then*/
                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {

                                                                                                                            /*^compute*/
                                                                                                                            /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3 =
                                                                                                                                /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_gimpleseq;;
                                                                                                                            /*^compute*/

                                                                                                                            /*consume variadic GIMPLE_SEQ !*/ variad_STRING4OUT_ix += 1;;

                                                                                                                            MELT_CHECK_SIGNAL();
                                                                                                                            ;
                                                                                                                            MELT_LOCATION("warmelt-base.melt:1691:/ apply");
                                                                                                                            /*apply*/
                                                                                                                            {
                                                                                                                                union meltparam_un argtab[1];
                                                                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                                                                /*^apply.arg*/
                                                                                                                                argtab[0].meltbp_gimpleseq = /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3;
                                                                                                                                /*_.V__V25*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_GIMPLESEQ ""), argtab, "", (union meltparam_un*)0);
                                                                                                                            }
                                                                                                                            ;
                                                                                                                            /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V25*/ meltfptr[18];;
                                                                                                                            /*epilog*/

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1690:/ clear");
                                                                                                                            /*clear*/ /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3 = 0 ;
                                                                                                                            /*^clear*/
                                                                                                                            /*clear*/ /*_.V__V25*/ meltfptr[18] = 0 ;
                                                                                                                        }
                                                                                                                        ;
                                                                                                                    }
                                                                                                                else    /*^cond.else*/
                                                                                                                    {

                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1692:/ cond");
                                                                                                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_EDGE) /*then*/
                                                                                                                                {
                                                                                                                                    /*^cond.then*/
                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {

                                                                                                                                        /*^compute*/
                                                                                                                                        /*_?*/ meltfram__.loc_EDGE__o4 =
                                                                                                                                            /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_edge;;
                                                                                                                                        /*^compute*/

                                                                                                                                        /*consume variadic EDGE !*/ variad_STRING4OUT_ix += 1;;

                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                        ;
                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1693:/ apply");
                                                                                                                                        /*apply*/
                                                                                                                                        {
                                                                                                                                            union meltparam_un argtab[1];
                                                                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                                                                            /*^apply.arg*/
                                                                                                                                            argtab[0].meltbp_edge = /*_?*/ meltfram__.loc_EDGE__o4;
                                                                                                                                            /*_.V__V26*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_EDGE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                        }
                                                                                                                                        ;
                                                                                                                                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V26*/ meltfptr[19];;
                                                                                                                                        /*epilog*/

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1692:/ clear");
                                                                                                                                        /*clear*/ /*_?*/ meltfram__.loc_EDGE__o4 = 0 ;
                                                                                                                                        /*^clear*/
                                                                                                                                        /*clear*/ /*_.V__V26*/ meltfptr[19] = 0 ;
                                                                                                                                    }
                                                                                                                                    ;
                                                                                                                                }
                                                                                                                            else    /*^cond.else*/
                                                                                                                                {

                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1694:/ cond");
                                                                                                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_LOOP) /*then*/
                                                                                                                                            {
                                                                                                                                                /*^cond.then*/
                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    /*^compute*/
                                                                                                                                                    /*_?*/ meltfram__.loc_LOOP__o5 =
                                                                                                                                                        /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_loop;;
                                                                                                                                                    /*^compute*/

                                                                                                                                                    /*consume variadic LOOP !*/ variad_STRING4OUT_ix += 1;;

                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1695:/ apply");
                                                                                                                                                    /*apply*/
                                                                                                                                                    {
                                                                                                                                                        union meltparam_un argtab[1];
                                                                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                        argtab[0].meltbp_loop = /*_?*/ meltfram__.loc_LOOP__o5;
                                                                                                                                                        /*_.V__V27*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_LOOP ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                    }
                                                                                                                                                    ;
                                                                                                                                                    /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V27*/ meltfptr[18];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1694:/ clear");
                                                                                                                                                    /*clear*/ /*_?*/ meltfram__.loc_LOOP__o5 = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.V__V27*/ meltfptr[18] = 0 ;
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        else    /*^cond.else*/
                                                                                                                                            {

                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1697:/ quasiblock");


                                                                                                                                                    /*_.VCTY__V29*/ meltfptr[18] =
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
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1700:/ cond");
                                                                                                                                                    /*cond*/ if (
                                                                                                                                                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V29*/ meltfptr[18]),
                                                                                                                                                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[3])))
                                                                                                                                                    ) /*then*/
                                                                                                                                                        {
                                                                                                                                                            /*^cond.then*/
                                                                                                                                                            /*^getslot*/
                                                                                                                                                            {
                                                                                                                                                                melt_ptr_t slot=NULL, obj=NULL;
                                                                                                                                                                obj = (melt_ptr_t)(/*_.VCTY__V29*/ meltfptr[18]) /*=obj*/;
                                                                                                                                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                                                                                                /*_.NAMED_NAME__V30*/ meltfptr[29] = slot;
                                                                                                                                                            };
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else    /*^cond.else*/
                                                                                                                                                        {

                                                                                                                                                            /*_.NAMED_NAME__V30*/ meltfptr[29] =  /*reallynil*/ NULL ;;
                                                                                                                                                        }
                                                                                                                                                    ;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1699:/ locexp");
                                                                                                                                                        error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "STRING4OUT with manipulator for unsupported ctype"),
                                                                                                                                                               melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V30*/ meltfptr[29])));
                                                                                                                                                    }
                                                                                                                                                    ;

#if MELT_HAVE_DEBUG
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1701:/ cppif.then");
                                                                                                                                                    /*^block*/
                                                                                                                                                    /*anyblock*/
                                                                                                                                                    {


                                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                                        ;
                                                                                                                                                        /*^cond*/
                                                                                                                                                        /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                                                                                                            {
                                                                                                                                                                /*^cond.then*/
                                                                                                                                                                /*_._IFELSE___V32*/ meltfptr[31] = (/*nil*/NULL);;
                                                                                                                                                            }
                                                                                                                                                        else
                                                                                                                                                            {
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1701:/ cond.else");

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
                                                                                                                                                                        argtab[0].meltbp_cstring =  "invalid variadic argument after closure to STRING4OUT";
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[2].meltbp_long = 1701;
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V29*/ meltfptr[18];
                                                                                                                                                                        /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                                    }
                                                                                                                                                                    ;
                                                                                                                                                                    /*_._IFELSE___V32*/ meltfptr[31] = /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32];;
                                                                                                                                                                    /*epilog*/

                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1701:/ clear");
                                                                                                                                                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] = 0 ;
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                            }
                                                                                                                                                        ;
                                                                                                                                                        /*_.IFCPP___V31*/ meltfptr[30] = /*_._IFELSE___V32*/ meltfptr[31];;
                                                                                                                                                        /*epilog*/

                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1701:/ clear");
                                                                                                                                                        /*clear*/ /*_._IFELSE___V32*/ meltfptr[31] = 0 ;
                                                                                                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                                                                                                    /*^cppif.else*/
                                                                                                                                                    /*_.IFCPP___V31*/ meltfptr[30] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1702:/ quasiblock");


                                                                                                                                                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1702:/ locexp");
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
                                                                                                                                                    /*_.LET___V28*/ meltfptr[19] = /*_.RETURN___V34*/ meltfptr[32];;

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1697:/ clear");
                                                                                                                                                    /*clear*/ /*_.VCTY__V29*/ meltfptr[18] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.NAMED_NAME__V30*/ meltfptr[29] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.IFCPP___V31*/ meltfptr[30] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.RETURN___V34*/ meltfptr[32] = 0 ;
                                                                                                                                                    /*_._IFELSE___V18*/ meltfptr[17] = /*_.LET___V28*/ meltfptr[19];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1694:/ clear");
                                                                                                                                                    /*clear*/ /*_.LET___V28*/ meltfptr[19] = 0 ;
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
                                            }
                                        else
                                            {
                                                MELT_LOCATION("warmelt-base.melt:1678:/ cond.else");

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1704:/ msend");
                                                    /*msend*/
                                                    {
                                                        union meltparam_un argtab[1];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^ojbmsend.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OUT__V14*/ meltfptr[11];
                                                        /*_.ADD_TO_OUT__V35*/ meltfptr[31] = meltgc_send((melt_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)((/*!ADD_TO_OUT*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_._IFELSE___V18*/ meltfptr[17] = /*_.ADD_TO_OUT__V35*/ meltfptr[31];;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1678:/ clear");
                                                    /*clear*/ /*_.ADD_TO_OUT__V35*/ meltfptr[31] = 0 ;
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:1677:/ clear");
                                        /*clear*/ /*_.V__V17*/ meltfptr[16] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_#IS_CLOSURE__L3*/ meltfnum[1] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_._IFELSE___V18*/ meltfptr[17] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        MELT_LOCATION("warmelt-base.melt:1705:/ cond");
                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_LONG) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    /*^compute*/
                                                    /*_#N__L5*/ meltfnum[3] =
                                                        /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_long;;
                                                    /*^compute*/

                                                    /*consume variadic LONG !*/ variad_STRING4OUT_ix += 1;;

                                                    {
                                                        MELT_LOCATION("warmelt-base.melt:1706:/ locexp");
                                                        meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (/*_#N__L5*/ meltfnum[3]));
                                                    }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1705:/ clear");
                                                    /*clear*/ /*_#N__L5*/ meltfnum[3] = 0 ;
                                                }
                                                ;
                                            }
                                        else    /*^cond.else*/
                                            {

                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1707:/ cond");
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_CSTRING) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_?*/ meltfram__.loc_CSTRING__o6 =
                                                                    /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_cstring;;
                                                                /*^compute*/

                                                                /*consume variadic CSTRING !*/ variad_STRING4OUT_ix += 1;;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1708:/ locexp");
                                                                    meltgc_add_out((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (/*_?*/ meltfram__.loc_CSTRING__o6));
                                                                }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1707:/ clear");
                                                                /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o6 = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:1710:/ quasiblock");


                                                                /*_.VCTY__V37*/ meltfptr[29] =
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
                                                                MELT_LOCATION("warmelt-base.melt:1713:/ cond");
                                                                /*cond*/ if (
                                                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V37*/ meltfptr[29]),
                                                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[3])))
                                                                ) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^getslot*/
                                                                        {
                                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                                            obj = (melt_ptr_t)(/*_.VCTY__V37*/ meltfptr[29]) /*=obj*/;
                                                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                            /*_.NAMED_NAME__V38*/ meltfptr[30] = slot;
                                                                        };
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*_.NAMED_NAME__V38*/ meltfptr[30] =  /*reallynil*/ NULL ;;
                                                                    }
                                                                ;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1712:/ locexp");
                                                                    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "STRING4OUT for unsupported ctype; use a manipulator like OUTPUT_TREE "),
                                                                           melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V38*/ meltfptr[30])));
                                                                }
                                                                ;

#if MELT_HAVE_DEBUG
                                                                MELT_LOCATION("warmelt-base.melt:1714:/ cppif.then");
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*^cond*/
                                                                    /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*_._IFELSE___V40*/ meltfptr[19] = (/*nil*/NULL);;
                                                                        }
                                                                    else
                                                                        {
                                                                            MELT_LOCATION("warmelt-base.melt:1714:/ cond.else");

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
                                                                                    argtab[0].meltbp_cstring =  "invalid variadic argument to STRING4OUT";
                                                                                    /*^apply.arg*/
                                                                                    argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                                                    /*^apply.arg*/
                                                                                    argtab[2].meltbp_long = 1714;
                                                                                    /*^apply.arg*/
                                                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V37*/ meltfptr[29];
                                                                                    /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                /*_._IFELSE___V40*/ meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-base.melt:1714:/ clear");
                                                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*_.IFCPP___V39*/ meltfptr[32] = /*_._IFELSE___V40*/ meltfptr[19];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1714:/ clear");
                                                                    /*clear*/ /*_._IFELSE___V40*/ meltfptr[19] = 0 ;
                                                                }

#else /*MELT_HAVE_DEBUG*/
                                                                /*^cppif.else*/
                                                                /*_.IFCPP___V39*/ meltfptr[32] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                ;
                                                                /*^compute*/
                                                                /*_.LET___V36*/ meltfptr[18] = /*_.IFCPP___V39*/ meltfptr[32];;

                                                                MELT_LOCATION("warmelt-base.melt:1710:/ clear");
                                                                /*clear*/ /*_.VCTY__V37*/ meltfptr[29] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.NAMED_NAME__V38*/ meltfptr[30] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.IFCPP___V39*/ meltfptr[32] = 0 ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1707:/ clear");
                                                                /*clear*/ /*_.LET___V36*/ meltfptr[18] = 0 ;
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
            goto meltlabloop_ARGLOOP_4;
meltlabexit_ARGLOOP_4:
            ;
            MELT_LOCATION("warmelt-base.melt:1672:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V15*/ meltfptr[7] = /*_.ARGLOOP__V16*/ meltfptr[15];;
        }
        ;
        MELT_LOCATION("warmelt-base.melt:1717:/ quasiblock");


        /*_.STR__V43*/ meltfptr[17] =
            (meltgc_new_stringdup((meltobject_ptr_t)(/*_.DIS__V2*/ meltfptr[1]), melt_strbuf_str((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]))));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1719:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.STR__V43*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-base.melt:1719:/ locexp");
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
        /*_.LET___V42*/ meltfptr[16] = /*_.RETURN___V44*/ meltfptr[31];;

        MELT_LOCATION("warmelt-base.melt:1717:/ clear");
        /*clear*/ /*_.STR__V43*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V44*/ meltfptr[31] = 0 ;
        /*_.LET___V13*/ meltfptr[10] = /*_.LET___V42*/ meltfptr[16];;

        MELT_LOCATION("warmelt-base.melt:1670:/ clear");
        /*clear*/ /*_.OUT__V14*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V15*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V42*/ meltfptr[16] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1658:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V13*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-base.melt:1658:/ locexp");
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
        /*clear*/ /*_._IFELSE___V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V6*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V9*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/ meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("STRING4OUT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_13_WARMELTmiBASE_STRING4OUT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_13_WARMELTmiBASE_STRING4OUT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_14_WARMELTmiBASE_ADD2OUT4NULL fromline 1752 */

    /** start of frame for meltrout_14_WARMELTmiBASE_ADD2OUT4NULL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL// fromline 1568
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL), clos) {};
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL)) {};
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL


    /** end of frame for meltrout_14_WARMELTmiBASE_ADD2OUT4NULL fromline 1697**/

    /* end of frame for routine meltrout_14_WARMELTmiBASE_ADD2OUT4NULL fromline 1756 */

    /* classy proc frame meltrout_14_WARMELTmiBASE_ADD2OUT4NULL */ MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_14_WARMELTmiBASE_ADD2OUT4NULL fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4NULL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1725:/ getarg");
    /*_.V__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1726:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#NULL__L1*/ meltfnum[0] =
                (/*null*/(/*_.V__V2*/ meltfptr[1]) == NULL);;
            MELT_LOCATION("warmelt-base.melt:1726:/ cond");
            /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1726:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check null v";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1726;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.V__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1726:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1726:/ clear");
            /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1727:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*nil*"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1725:/ clear");
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4NULL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_14_WARMELTmiBASE_ADD2OUT4NULL_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_14_WARMELTmiBASE_ADD2OUT4NULL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1752 */

    /** start of frame for meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER// fromline 1568
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER), clos) {};
        MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER() //the constructor fromline 1642
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER)) {};
        MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER


    /** end of frame for meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1697**/

    /* end of frame for routine meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1756 */

    /* classy proc frame meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER */ MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4INTEGER", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1730:/ getarg");
    /*_.VN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1731:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_INTEGERBOX__L1*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VN__V2*/ meltfptr[1])) == MELTOBMAG_INT);;
            MELT_LOCATION("warmelt-base.melt:1731:/ cond");
            /*cond*/ if (/*_#IS_INTEGERBOX__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1731:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check boxed number vn";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1731;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VN__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1731:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1731:/ clear");
            /*clear*/ /*_#IS_INTEGERBOX__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_#GET_INT__L2*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.VN__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-base.melt:1732:/ locexp");
            meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#GET_INT__L2*/ meltfnum[0]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1730:/ clear");
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L2*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4INTEGER", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_15_WARMELTmiBASE_ADD2OUT4INTEGER*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_16_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_16_WARMELTmiBASE_ADD2OUT4STRING fromline 1752 */

    /** start of frame for meltrout_16_WARMELTmiBASE_ADD2OUT4STRING of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING// fromline 1568
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING), clos) {};
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING() //the constructor fromline 1642
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING)) {};
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING


    /** end of frame for meltrout_16_WARMELTmiBASE_ADD2OUT4STRING fromline 1697**/

    /* end of frame for routine meltrout_16_WARMELTmiBASE_ADD2OUT4STRING fromline 1756 */

    /* classy proc frame meltrout_16_WARMELTmiBASE_ADD2OUT4STRING */ MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_16_WARMELTmiBASE_ADD2OUT4STRING fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4STRING", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1735:/ getarg");
    /*_.VS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1736:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L1*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VS__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-base.melt:1736:/ cond");
            /*cond*/ if (/*_#IS_STRING__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1736:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check boxed string vs";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1736;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VS__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1736:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1736:/ clear");
            /*clear*/ /*_#IS_STRING__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1737:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.VS__V2*/ meltfptr[1])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1735:/ clear");
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4STRING", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_16_WARMELTmiBASE_ADD2OUT4STRING_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_16_WARMELTmiBASE_ADD2OUT4STRING*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1752 */

    /** start of frame for meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF// fromline 1568
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF), clos) {};
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF() //the constructor fromline 1642
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF)) {};
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF


    /** end of frame for meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1697**/

    /* end of frame for routine meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1756 */

    /* classy proc frame meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF */ MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4STRBUF", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1740:/ getarg");
    /*_.VSBUF__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1741:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L1*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VSBUF__V2*/ meltfptr[1])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-base.melt:1741:/ cond");
            /*cond*/ if (/*_#IS_STRBUF__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1741:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check strbuf vsbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1741;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VSBUF__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1741:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1741:/ clear");
            /*clear*/ /*_#IS_STRBUF__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1742:/ locexp");
            meltgc_add_out ((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_strbuf_str ((melt_ptr_t) /*_.VSBUF__V2*/ meltfptr[1]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1740:/ clear");
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4STRBUF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_17_WARMELTmiBASE_ADD2OUT4STRBUF*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiBASE_OUTPUT_JSON(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                      const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_18_WARMELTmiBASE_OUTPUT_JSON_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_18_WARMELTmiBASE_OUTPUT_JSON fromline 1752 */

    /** start of frame for meltrout_18_WARMELTmiBASE_OUTPUT_JSON of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON// fromline 1568
        : public Melt_CallFrameWithValues<26>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<26> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON), clos) {};
        MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON() //the constructor fromline 1642
            : Melt_CallFrameWithValues<26> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<26> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON)) {};
        MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<26> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON


    /** end of frame for meltrout_18_WARMELTmiBASE_OUTPUT_JSON fromline 1697**/

    /* end of frame for routine meltrout_18_WARMELTmiBASE_OUTPUT_JSON fromline 1756 */

    /* classy proc frame meltrout_18_WARMELTmiBASE_OUTPUT_JSON */ MeltFrame_meltrout_18_WARMELTmiBASE_OUTPUT_JSON
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_18_WARMELTmiBASE_OUTPUT_JSON fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUT_JSON", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1753:/ getarg");
    /*_.OUT__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.V__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.V__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_INTEGERBOX__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.V__V3*/ meltfptr[2])) == MELTOBMAG_INT);;
        MELT_LOCATION("warmelt-base.melt:1755:/ cond");
        /*cond*/ if (/*_#IS_INTEGERBOX__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#GET_INT__L2*/ meltfnum[1] =
                        (melt_get_int((melt_ptr_t)(/*_.V__V3*/ meltfptr[2])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:1756:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_long = /*_#GET_INT__L2*/ meltfnum[1];
                        /*_.ADD2OUT__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IFELSE___V4*/ meltfptr[3] = /*_.ADD2OUT__V5*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1755:/ clear");
                    /*clear*/ /*_#GET_INT__L2*/ meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V5*/ meltfptr[4] = 0 ;
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
                    /*_#eqeq__L3*/ meltfnum[1] =
                        ((/*_.V__V3*/ meltfptr[2]) == ((/*!konst_1_TRUE*/ meltfrout->tabval[1])));;
                    MELT_LOCATION("warmelt-base.melt:1757:/ cond");
                    /*cond*/ if (/*_#eqeq__L3*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-base.melt:1758:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  "true";
                                    /*_.ADD2OUT__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IFELSE___V6*/ meltfptr[4] = /*_.ADD2OUT__V7*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1757:/ clear");
                                /*clear*/ /*_.ADD2OUT__V7*/ meltfptr[6] = 0 ;
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
                                /*_#eqeq__L4*/ meltfnum[3] =
                                    ((/*_.V__V3*/ meltfptr[2]) == ((/*!konst_2_JSON_FALSE*/ meltfrout->tabval[2])));;
                                MELT_LOCATION("warmelt-base.melt:1760:/ cond");
                                /*cond*/ if (/*_#eqeq__L4*/ meltfnum[3]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1761:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_cstring =  "false";
                                                /*_.ADD2OUT__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*_._IFELSE___V8*/ meltfptr[6] = /*_.ADD2OUT__V9*/ meltfptr[8];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1760:/ clear");
                                            /*clear*/ /*_.ADD2OUT__V9*/ meltfptr[8] = 0 ;
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
                                            /*_#NULL__L5*/ meltfnum[4] =
                                                (/*null*/(/*_.V__V3*/ meltfptr[2]) == NULL);;
                                            MELT_LOCATION("warmelt-base.melt:1763:/ cond");
                                            /*cond*/ if (/*_#NULL__L5*/ meltfnum[4]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-base.melt:1764:/ apply");
                                                        /*apply*/
                                                        {
                                                            union meltparam_un argtab[1];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^apply.arg*/
                                                            argtab[0].meltbp_cstring =  "null";
                                                            /*_.ADD2OUT__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IFELSE___V10*/ meltfptr[8] = /*_.ADD2OUT__V11*/ meltfptr[10];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1763:/ clear");
                                                        /*clear*/ /*_.ADD2OUT__V11*/ meltfptr[10] = 0 ;
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
                                                        /*_#IS_MULTIPLE__L6*/ meltfnum[5] =
                                                            (melt_magic_discr((melt_ptr_t)(/*_.V__V3*/ meltfptr[2])) == MELTOBMAG_MULTIPLE);;
                                                        MELT_LOCATION("warmelt-base.melt:1766:/ cond");
                                                        /*cond*/ if (/*_#IS_MULTIPLE__L6*/ meltfnum[5]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-base.melt:1767:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[1];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_cstring =  "[";
                                                                        /*_.ADD2OUT__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    /*citerblock FOREACH_IN_MULTIPLE*/
                                                                    {
                                                                        /* start foreach_in_multiple meltcit1__EACHTUP */
                                                                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.V__V3*/ meltfptr[2]);
                                                                        for (/*_#IX__L7*/ meltfnum[6] = 0;
                                                                                          (/*_#IX__L7*/ meltfnum[6] >= 0) && (/*_#IX__L7*/ meltfnum[6] <  meltcit1__EACHTUP_ln);
                                                                                          /*_#IX__L7*/ meltfnum[6]++)
                                                                            {
                                                                                /*_.CURCOMP__V14*/ meltfptr[13] = melt_multiple_nth((melt_ptr_t)(/*_.V__V3*/ meltfptr[2]),  /*_#IX__L7*/ meltfnum[6]);




                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-base.melt:1771:/ cond");
                                                                                /*cond*/ if (/*_#IX__L7*/ meltfnum[6]) /*then*/
                                                                                    {
                                                                                        /*^cond.then*/
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
                                                                                                argtab[0].meltbp_cstring =  ",";
                                                                                                /*_.ADD2OUT__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                            }
                                                                                            ;
                                                                                            /*_._IF___V15*/ meltfptr[14] = /*_.ADD2OUT__V16*/ meltfptr[15];;
                                                                                            /*epilog*/

                                                                                            MELT_LOCATION("warmelt-base.melt:1771:/ clear");
                                                                                            /*clear*/ /*_.ADD2OUT__V16*/ meltfptr[15] = 0 ;
                                                                                        }
                                                                                        ;
                                                                                    }
                                                                                else    /*^cond.else*/
                                                                                    {

                                                                                        /*_._IF___V15*/ meltfptr[14] =  /*reallynil*/ NULL ;;
                                                                                    }
                                                                                ;

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-base.melt:1772:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[1];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURCOMP__V14*/ meltfptr[13];
                                                                                    /*_.OUTPUT_JSON__V17*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_JSON*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                if (/*_#IX__L7*/ meltfnum[6]<0) break;
                                                                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                                                                        /*citerepilog*/

                                                                        MELT_LOCATION("warmelt-base.melt:1768:/ clear");
                                                                        /*clear*/ /*_.CURCOMP__V14*/ meltfptr[13] = 0 ;
                                                                        /*^clear*/
                                                                        /*clear*/ /*_#IX__L7*/ meltfnum[6] = 0 ;
                                                                        /*^clear*/
                                                                        /*clear*/ /*_._IF___V15*/ meltfptr[14] = 0 ;
                                                                        /*^clear*/
                                                                        /*clear*/ /*_.OUTPUT_JSON__V17*/ meltfptr[15] = 0 ;
                                                                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                                                                    ;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-base.melt:1773:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[1];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_cstring =  "]";
                                                                        /*_.ADD2OUT__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-base.melt:1766:/ quasiblock");


                                                                    /*_.PROGN___V19*/ meltfptr[18] = /*_.ADD2OUT__V18*/ meltfptr[17];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V12*/ meltfptr[10] = /*_.PROGN___V19*/ meltfptr[18];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1766:/ clear");
                                                                    /*clear*/ /*_.ADD2OUT__V13*/ meltfptr[12] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.ADD2OUT__V18*/ meltfptr[17] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.PROGN___V19*/ meltfptr[18] = 0 ;
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
                                                                    /*_#IS_STRING__L8*/ meltfnum[7] =
                                                                        (melt_magic_discr((melt_ptr_t)(/*_.V__V3*/ meltfptr[2])) == MELTOBMAG_STRING);;
                                                                    MELT_LOCATION("warmelt-base.melt:1774:/ cond");
                                                                    /*cond*/ if (/*_#IS_STRING__L8*/ meltfnum[7]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {


                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-base.melt:1775:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[1];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_cstring =  "\"";
                                                                                    /*_.ADD2OUT__V21*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-base.melt:1776:/ locexp");
                                                                                    /*add2out_utf8json_encstring*/
                                                                                    meltgc_add_out_cstr_len_mode ((melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]),
                                                                                                                  melt_string_str ((melt_ptr_t)(/*_.V__V3*/ meltfptr[2])),
                                                                                                                  -1,
                                                                                                                  MELTCOUT_UTF8JSON);
                                                                                }
                                                                                ;

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-base.melt:1777:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[1];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_cstring =  "\"";
                                                                                    /*_.ADD2OUT__V22*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-base.melt:1774:/ quasiblock");


                                                                                /*_.PROGN___V23*/ meltfptr[22] = /*_.ADD2OUT__V22*/ meltfptr[18];;
                                                                                /*^compute*/
                                                                                /*_._IFELSE___V20*/ meltfptr[12] = /*_.PROGN___V23*/ meltfptr[22];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-base.melt:1774:/ clear");
                                                                                /*clear*/ /*_.ADD2OUT__V21*/ meltfptr[17] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.ADD2OUT__V22*/ meltfptr[18] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.PROGN___V23*/ meltfptr[22] = 0 ;
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
                                                                                /*_#IS_MIXBIGINT__L9*/ meltfnum[8] =
                                                                                    (melt_magic_discr((melt_ptr_t)(/*_.V__V3*/ meltfptr[2])) == MELTOBMAG_MIXBIGINT);;
                                                                                MELT_LOCATION("warmelt-base.melt:1779:/ cond");
                                                                                /*cond*/ if (/*_#IS_MIXBIGINT__L9*/ meltfnum[8]) /*then*/
                                                                                    {
                                                                                        /*^cond.then*/
                                                                                        /*^block*/
                                                                                        /*anyblock*/
                                                                                        {




                                                                                            {
                                                                                                MELT_LOCATION("warmelt-base.melt:1780:/ locexp");
                                                                                                meltgc_ppstrbuf_mixbigint((melt_ptr_t) /*_.OUT__V2*/ meltfptr[1],0,(melt_ptr_t) /*_.V__V3*/ meltfptr[2]);;
                                                                                            }
                                                                                            ;
                                                                                            /*clear*/ /*_._IFELSE___V24*/ meltfptr[17] = 0 ;
                                                                                            /*epilog*/
                                                                                        }
                                                                                        ;
                                                                                    }
                                                                                else
                                                                                    {
                                                                                        MELT_LOCATION("warmelt-base.melt:1779:/ cond.else");

                                                                                        /*^block*/
                                                                                        /*anyblock*/
                                                                                        {


                                                                                            MELT_CHECK_SIGNAL();
                                                                                            ;
                                                                                            MELT_LOCATION("warmelt-base.melt:1783:/ msend");
                                                                                            /*msend*/
                                                                                            {
                                                                                                union meltparam_un argtab[1];
                                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                                /*^ojbmsend.arg*/
                                                                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OUT__V2*/ meltfptr[1];
                                                                                                /*_.OUTPUT_AS_JSON__V25*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.V__V3*/ meltfptr[2]), (melt_ptr_t)((/*!OUTPUT_AS_JSON*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                            }
                                                                                            ;
                                                                                            MELT_LOCATION("warmelt-base.melt:1782:/ quasiblock");


                                                                                            /*_.PROGN___V26*/ meltfptr[22] = /*_.OUTPUT_AS_JSON__V25*/ meltfptr[18];;
                                                                                            /*^compute*/
                                                                                            /*_._IFELSE___V24*/ meltfptr[17] = /*_.PROGN___V26*/ meltfptr[22];;
                                                                                            /*epilog*/

                                                                                            MELT_LOCATION("warmelt-base.melt:1779:/ clear");
                                                                                            /*clear*/ /*_.OUTPUT_AS_JSON__V25*/ meltfptr[18] = 0 ;
                                                                                            /*^clear*/
                                                                                            /*clear*/ /*_.PROGN___V26*/ meltfptr[22] = 0 ;
                                                                                        }
                                                                                        ;
                                                                                    }
                                                                                ;
                                                                                /*_._IFELSE___V20*/ meltfptr[12] = /*_._IFELSE___V24*/ meltfptr[17];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-base.melt:1774:/ clear");
                                                                                /*clear*/ /*_#IS_MIXBIGINT__L9*/ meltfnum[8] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_._IFELSE___V24*/ meltfptr[17] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*_._IFELSE___V12*/ meltfptr[10] = /*_._IFELSE___V20*/ meltfptr[12];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1766:/ clear");
                                                                    /*clear*/ /*_#IS_STRING__L8*/ meltfnum[7] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_._IFELSE___V20*/ meltfptr[12] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V10*/ meltfptr[8] = /*_._IFELSE___V12*/ meltfptr[10];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1763:/ clear");
                                                        /*clear*/ /*_#IS_MULTIPLE__L6*/ meltfnum[5] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V12*/ meltfptr[10] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V8*/ meltfptr[6] = /*_._IFELSE___V10*/ meltfptr[8];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1760:/ clear");
                                            /*clear*/ /*_#NULL__L5*/ meltfnum[4] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V10*/ meltfptr[8] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V6*/ meltfptr[4] = /*_._IFELSE___V8*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1757:/ clear");
                                /*clear*/ /*_#eqeq__L4*/ meltfnum[3] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V8*/ meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V4*/ meltfptr[3] = /*_._IFELSE___V6*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1755:/ clear");
                    /*clear*/ /*_#eqeq__L3*/ meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V6*/ meltfptr[4] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1753:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_._IFELSE___V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-base.melt:1753:/ locexp");
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
        /*clear*/ /*_#IS_INTEGERBOX__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUT_JSON", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_18_WARMELTmiBASE_OUTPUT_JSON_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_18_WARMELTmiBASE_OUTPUT_JSON*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1752 */

    /** start of frame for meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB// fromline 1568
        : public Melt_CallFrameWithValues<12>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB), clos) {};
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB() //the constructor fromline 1642
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB)) {};
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB


    /** end of frame for meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1697**/

    /* end of frame for routine meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1756 */

    /* classy proc frame meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB */ MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4CLONEDSYMB", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1787:/ getarg");
    /*_.VC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

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
        MELT_LOCATION("warmelt-base.melt:1788:/ block");
        /*anyblock*/
        {

            /*^objgoto*/
            /*objgoto*/ goto mtch1_0;
            ;

/*objlabel*/ mtch1_0:
            ;
            MELT_LOCATION("warmelt-base.melt:1789:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_1;
            meltlab_count_1++;
            debugeprintf("objlabel_1 mtch1_*CLASS_NORMTESTER_INSTANCE/da91a1. *CLASS_OBJLABELINSTR/2bdbebc5. %ld", meltlab_count_1);
#endif

            ;
            /*^clear*/
            /*clear*/ /*_.NAMED_NAME__V4*/ meltfptr[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CSYM_URANK__V5*/ meltfptr[4] = 0 ;
            /*^cond*/
            /*cond*/ if (
                /*normtesterinst*/ (melt_is_instance_of((melt_ptr_t)(/*_.VC__V2*/ meltfptr[1]),
                                    (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[0]))))) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*^getslot*/
                        {
                            melt_ptr_t slot=NULL, obj=NULL;
                            obj = (melt_ptr_t)(/*_.VC__V2*/ meltfptr[1]) /*=obj*/;
                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                            /*_.NAMED_NAME__V4*/ meltfptr[3] = slot;
                        };
                        ;
                        /*^getslot*/
                        {
                            melt_ptr_t slot=NULL, obj=NULL;
                            obj = (melt_ptr_t)(/*_.VC__V2*/ meltfptr[1]) /*=obj*/;
                            melt_object_get_field(slot,obj, 3, "CSYM_URANK");
                            /*_.CSYM_URANK__V5*/ meltfptr[4] = slot;
                        };
                        ;
                        MELT_LOCATION("warmelt-base.melt:1791:/ objgoto");
                        /*objgoto*/ goto mtch1_1;
                        ;
                    }
                    ;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1789:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {

                        MELT_LOCATION("warmelt-base.melt:1796:/ objgoto");
                        /*objgoto*/ goto mtch1_3;
                        ;
                    }
                    ;
                }
            ;

/*objlabel*/ mtch1_1:
            ;
            MELT_LOCATION("warmelt-base.melt:1791:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_2;
            meltlab_count_2++;
            debugeprintf("objlabel_2 mtch1_*CLASS_NORMTESTER_MATCHER/36c28ea6. *CLASS_OBJLABELINSTR/175adadc. %ld", meltlab_count_2);
#endif

            ;
            /*^clear*/
            /*clear*/ /*_#ICT__L1*/ meltfnum[0] = 0 ;
            /*^cond*/
            /*cond*/ if (
                /* integerbox_of IBOXOF_mtch1__1 ?*/ /*_.CSYM_URANK__V5*/ meltfptr[4]
                && melt_magic_discr((melt_ptr_t) /*_.CSYM_URANK__V5*/ meltfptr[4]) == MELTOBMAG_INT ) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*^clear*/
                        /*clear*/ /*_#ICT__L1*/ meltfnum[0] = 0 ;

                        {
                            /*^locexp*/
                            /* integerbox_of IBOXOF_mtch1__1 !*/ /*_#ICT__L1*/ meltfnum[0] = ((struct meltint_st*)/*_.CSYM_URANK__V5*/ meltfptr[4])->val; ;
                        }
                        ;
                        MELT_LOCATION("warmelt-base.melt:1789:/ objgoto");
                        /*objgoto*/ goto mtch1_2;
                        ;
                    }
                    ;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1791:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {

                        MELT_LOCATION("warmelt-base.melt:1796:/ objgoto");
                        /*objgoto*/ goto mtch1_3;
                        ;
                    }
                    ;
                }
            ;

/*objlabel*/ mtch1_2:
            ;
            MELT_LOCATION("warmelt-base.melt:1789:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_3;
            meltlab_count_3++;
            debugeprintf("objlabel_3 mtch1_*CLASS_NORMTESTER_SUCCESS/20908fa3. *CLASS_OBJLABELINSTR/2a0444. %ld", meltlab_count_3);
#endif

            ;
            /*^quasiblock*/


            /*_.NVARNAM__V6*/ meltfptr[4] = /*_.NAMED_NAME__V4*/ meltfptr[3];;
            /*^compute*/
            /*_#NVARURANK__L2*/ meltfnum[1] = /*_#ICT__L1*/ meltfnum[0];;

#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-base.melt:1792:/ cppif.then");
            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                /*_#IS_STRING__L3*/ meltfnum[2] =
                    (melt_magic_discr((melt_ptr_t)(/*_.NVARNAM__V6*/ meltfptr[4])) == MELTOBMAG_STRING);;
                MELT_LOCATION("warmelt-base.melt:1792:/ cond");
                /*cond*/ if (/*_#IS_STRING__L3*/ meltfnum[2]) /*then*/
                    {
                        /*^cond.then*/
                        /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1792:/ cond.else");

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
                                argtab[0].meltbp_cstring =  "check nvarnam";
                                /*^apply.arg*/
                                argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                /*^apply.arg*/
                                argtab[2].meltbp_long = 1792;
                                /*^apply.arg*/
                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.NVARNAM__V6*/ meltfptr[4];
                                /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                            }
                            ;
                            /*_._IFELSE___V8*/ meltfptr[7] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                            /*epilog*/

                            MELT_LOCATION("warmelt-base.melt:1792:/ clear");
                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                        }
                        ;
                    }
                ;
                /*_.IFCPP___V7*/ meltfptr[6] = /*_._IFELSE___V8*/ meltfptr[7];;
                /*epilog*/

                MELT_LOCATION("warmelt-base.melt:1792:/ clear");
                /*clear*/ /*_#IS_STRING__L3*/ meltfnum[2] = 0 ;
                /*^clear*/
                /*clear*/ /*_._IFELSE___V8*/ meltfptr[7] = 0 ;
            }

#else /*MELT_HAVE_DEBUG*/
            /*^cppif.else*/
            /*_.IFCPP___V7*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1793:/ locexp");
                meltgc_add_out_cident((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]),
                                      melt_string_str((melt_ptr_t)(/*_.NVARNAM__V6*/ meltfptr[4])));
            }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1794:/ locexp");
                meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "__"));
            }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1795:/ locexp");
                meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#NVARURANK__L2*/ meltfnum[1]));
            }
            ;

            MELT_CHECK_SIGNAL();
            ;

            MELT_LOCATION("warmelt-base.melt:1789:/ clear");
            /*clear*/ /*_.NVARNAM__V6*/ meltfptr[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NVARURANK__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V7*/ meltfptr[6] = 0 ;
            /*^objgoto*/
            /*objgoto*/ goto mtch1__end /*endmatch*/;
            ;

/*objlabel*/ mtch1_3:
            ;
            MELT_LOCATION("warmelt-base.melt:1796:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_4;
            meltlab_count_4++;
            debugeprintf("objlabel_4 mtch1_*CLASS_NORMTESTER_SUCCESS/1b2517f1. *CLASS_OBJLABELINSTR/3d919754. %ld", meltlab_count_4);
#endif

            ;
            /*^quasiblock*/



#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-base.melt:1797:/ cppif.then");
            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                /*^cond*/
                /*cond*/ if ((/*nil*/NULL)) /*then*/
                    {
                        /*^cond.then*/
                        /*_._IFELSE___V11*/ meltfptr[7] = (/*nil*/NULL);;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1797:/ cond.else");

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
                                argtab[0].meltbp_cstring =  "bad cloned symbol";
                                /*^apply.arg*/
                                argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                /*^apply.arg*/
                                argtab[2].meltbp_long = 1797;
                                /*^apply.arg*/
                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VC__V2*/ meltfptr[1];
                                /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                            }
                            ;
                            /*_._IFELSE___V11*/ meltfptr[7] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[4];;
                            /*epilog*/

                            MELT_LOCATION("warmelt-base.melt:1797:/ clear");
                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[4] = 0 ;
                        }
                        ;
                    }
                ;
                /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[7];;
                /*epilog*/

                MELT_LOCATION("warmelt-base.melt:1797:/ clear");
                /*clear*/ /*_._IFELSE___V11*/ meltfptr[7] = 0 ;
            }

#else /*MELT_HAVE_DEBUG*/
            /*^cppif.else*/
            /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
            ;

            MELT_LOCATION("warmelt-base.melt:1796:/ clear");
            /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
            /*^objgoto*/
            /*objgoto*/ goto mtch1__end /*endmatch*/;
            ;

/*objlabel*/ mtch1__end:
            ;
            MELT_LOCATION("warmelt-base.melt:1788:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_5;
            meltlab_count_5++;
            debugeprintf("objlabel_5 mtch1__end_h670758135 *CLASS_OBJLABELINSTR/27faf4f7. %ld", meltlab_count_5);
#endif

            ;
        }
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4CLONEDSYMB", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_19_WARMELTmiBASE_ADD2OUT4CLONEDSYMB*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED fromline 1752 */

    /** start of frame for meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED// fromline 1568
        : public Melt_CallFrameWithValues<4>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<4> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED), clos) {};
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED() //the constructor fromline 1642
            : Melt_CallFrameWithValues<4> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED)) {};
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED


    /** end of frame for meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED fromline 1697**/

    /* end of frame for routine meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED fromline 1756 */

    /* classy proc frame meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED */ MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4NAMED", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1800:/ getarg");
    /*_.VN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:1801:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VN__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.VN__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.NAMED_NAME__V4*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAMED_NAME__V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1801:/ locexp");
            meltgc_add_out_cident((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]),
                                  melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V4*/ meltfptr[3])));
        }
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1800:/ clear");
        /*clear*/ /*_.NAMED_NAME__V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4NAMED", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_20_WARMELTmiBASE_ADD2OUT4NAMED*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1752 */

    /** start of frame for meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT// fromline 1568
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT), clos) {};
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT)) {};
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT


    /** end of frame for meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1697**/

    /* end of frame for routine meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1756 */

    /* classy proc frame meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT */ MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4ROOTOBJECT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1804:/ getarg");
    /*_.VO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-base.melt:1805:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*"));
        }
        ;
        /*_.DISCRIM__V4*/ meltfptr[3] =
            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.VO__V2*/ meltfptr[1]))));;
        MELT_LOCATION("warmelt-base.melt:1806:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.DISCRIM__V4*/ meltfptr[3]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.DISCRIM__V4*/ meltfptr[3]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.NAMED_NAME__V5*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAMED_NAME__V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1806:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V5*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1807:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "/"));
        }
        ;
        /*_#OBJ_HASH__L1*/ meltfnum[0] =
            (melt_obj_hash((melt_ptr_t)(/*_.VO__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-base.melt:1808:/ locexp");
            meltgc_add_out_hex((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#OBJ_HASH__L1*/ meltfnum[0]));
        }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1809:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "."));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1804:/ clear");
        /*clear*/ /*_.DISCRIM__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OBJ_HASH__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4ROOTOBJECT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_21_WARMELTmiBASE_ADD2OUT4ROOTOBJECT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_22_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                      const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_22_WARMELTmiBASE_ADD2OUT4ANY fromline 1752 */

    /** start of frame for meltrout_22_WARMELTmiBASE_ADD2OUT4ANY of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY// fromline 1568
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY), clos) {};
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY() //the constructor fromline 1642
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY)) {};
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY


    /** end of frame for meltrout_22_WARMELTmiBASE_ADD2OUT4ANY fromline 1697**/

    /* end of frame for routine meltrout_22_WARMELTmiBASE_ADD2OUT4ANY fromline 1756 */

    /* classy proc frame meltrout_22_WARMELTmiBASE_ADD2OUT4ANY */ MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_22_WARMELTmiBASE_ADD2OUT4ANY fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4ANY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1812:/ getarg");
    /*_.VA__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-base.melt:1813:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*"));
        }
        ;
        /*_.DISCRIM__V4*/ meltfptr[3] =
            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.VA__V2*/ meltfptr[1]))));;
        MELT_LOCATION("warmelt-base.melt:1814:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.DISCRIM__V4*/ meltfptr[3]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.DISCRIM__V4*/ meltfptr[3]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.NAMED_NAME__V5*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAMED_NAME__V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1814:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V5*/ meltfptr[4])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1812:/ clear");
        /*clear*/ /*_.DISCRIM__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4ANY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_22_WARMELTmiBASE_ADD2OUT4ANY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_22_WARMELTmiBASE_ADD2OUT4ANY*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY fromline 1752 */

    /** start of frame for meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY// fromline 1568
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY), clos) {};
        MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY() //the constructor fromline 1642
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY)) {};
        MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY


    /** end of frame for meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY fromline 1697**/

    /* end of frame for routine meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY fromline 1756 */

    /* classy proc frame meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY */ MeltFrame_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MAPSTRING_EVERY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1886:/ getarg");
    /*_.MAP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_MAPSTRING__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.MAP__V2*/ meltfptr[1])) == MELTOBMAG_MAPSTRINGS);;
        MELT_LOCATION("warmelt-base.melt:1890:/ cond");
        /*cond*/ if (/*_#IS_MAPSTRING__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1891:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_MAPSTRING*/
                                {
                                    /*foreach_in_mapstring meltcit1__EACHSTRMAP :*/ int meltcit1__EACHSTRMAP_ix=0, meltcit1__EACHSTRMAP_siz=0;
                                    for (meltcit1__EACHSTRMAP_ix=0;
                                            /* we retrieve in meltcit1__EACHSTRMAP_siz the size at each iteration since it could change. */
                                            meltcit1__EACHSTRMAP_ix>=0
                                            && (meltcit1__EACHSTRMAP_siz =
                                                    melt_size_mapstrings((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])) > 0
                                            && meltcit1__EACHSTRMAP_ix < meltcit1__EACHSTRMAP_siz;
                                            meltcit1__EACHSTRMAP_ix++)
                                        {
                                            const char* meltcit1__EACHSTRMAP_str = NULL;
                                            const char* meltcit1__EACHSTRMAP_nam = NULL;
                                            /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/ meltfptr[4] = NULL;
                                            meltcit1__EACHSTRMAP_str =
                                                ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_at;
                                            if (!meltcit1__EACHSTRMAP_str || meltcit1__EACHSTRMAP_str == HTAB_DELETED_ENTRY)
                                                continue; /*foreach_in_mapstring meltcit1__EACHSTRMAP inside before*/
                                            /*_.CURVAL__V5*/ meltfptr[4] = ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_va;
                                            if (!/*_.CURVAL__V5*/ meltfptr[4])
                                                continue;
                                            if (melt_is_instance_of((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4], (melt_ptr_t) MELT_PREDEF (CLASS_NAMED))
                                                    && (/*_.CURAT__V4*/ meltfptr[3] = melt_object_nth_field ((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4],
                                                                        MELTFIELD_NAMED_NAME)) != NULL
                                                    && (meltcit1__EACHSTRMAP_nam =
                                                            melt_string_str ((melt_ptr_t) /*_.CURAT__V4*/ meltfptr[3])) != (char*)0
                                                    && !strcmp (meltcit1__EACHSTRMAP_nam, meltcit1__EACHSTRMAP_str))
                                                /*_.CURAT__V4*/ meltfptr[3] = /*_.CURAT__V4*/ meltfptr[3];
                                            else
                                                {
                                                    /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                                    /*_.CURAT__V4*/ meltfptr[3] =
                                                        meltgc_new_stringdup ((meltobject_ptr_t) MELT_PREDEF (DISCR_STRING),
                                                                              meltcit1__EACHSTRMAP_str);
                                                }
                                            meltcit1__EACHSTRMAP_str = (const char*)0;
                                            meltcit1__EACHSTRMAP_nam = (const char*)0;




                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1895:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURVAL__V5*/ meltfptr[4];
                                                /*_.F__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.CURAT__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /* end foreach_in_mapstring meltcit1__EACHSTRMAP */
                                            /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/ meltfptr[4] = NULL;
                                        }

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1892:/ clear");
                                    /*clear*/ /*_.CURAT__V4*/ meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURVAL__V5*/ meltfptr[4] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.F__V6*/ meltfptr[5] = 0 ;
                                } /*endciterblock FOREACH_IN_MAPSTRING*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1890:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1886:/ clear");
        /*clear*/ /*_#IS_MAPSTRING__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MAPSTRING_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_23_WARMELTmiBASE_MAPSTRING_EVERY*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1752 */

    /** start of frame for meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST// fromline 1568
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST), clos) {};
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST)) {};
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST


    /** end of frame for meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1697**/

    /* end of frame for routine meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1756 */

    /* classy proc frame meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST */ MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MAPSTRING_ITERATE_TEST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1899:/ getarg");
    /*_.MAP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_MAPSTRING__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.MAP__V2*/ meltfptr[1])) == MELTOBMAG_MAPSTRINGS);;
        MELT_LOCATION("warmelt-base.melt:1904:/ cond");
        /*cond*/ if (/*_#IS_MAPSTRING__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1905:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_MAPSTRING*/
                                {
                                    /*foreach_in_mapstring meltcit1__EACHSTRMAP :*/ int meltcit1__EACHSTRMAP_ix=0, meltcit1__EACHSTRMAP_siz=0;
                                    for (meltcit1__EACHSTRMAP_ix=0;
                                            /* we retrieve in meltcit1__EACHSTRMAP_siz the size at each iteration since it could change. */
                                            meltcit1__EACHSTRMAP_ix>=0
                                            && (meltcit1__EACHSTRMAP_siz =
                                                    melt_size_mapstrings((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])) > 0
                                            && meltcit1__EACHSTRMAP_ix < meltcit1__EACHSTRMAP_siz;
                                            meltcit1__EACHSTRMAP_ix++)
                                        {
                                            const char* meltcit1__EACHSTRMAP_str = NULL;
                                            const char* meltcit1__EACHSTRMAP_nam = NULL;
                                            /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/ meltfptr[4] = NULL;
                                            meltcit1__EACHSTRMAP_str =
                                                ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_at;
                                            if (!meltcit1__EACHSTRMAP_str || meltcit1__EACHSTRMAP_str == HTAB_DELETED_ENTRY)
                                                continue; /*foreach_in_mapstring meltcit1__EACHSTRMAP inside before*/
                                            /*_.CURVAL__V5*/ meltfptr[4] = ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_va;
                                            if (!/*_.CURVAL__V5*/ meltfptr[4])
                                                continue;
                                            if (melt_is_instance_of((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4], (melt_ptr_t) MELT_PREDEF (CLASS_NAMED))
                                                    && (/*_.CURAT__V4*/ meltfptr[3] = melt_object_nth_field ((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4],
                                                                        MELTFIELD_NAMED_NAME)) != NULL
                                                    && (meltcit1__EACHSTRMAP_nam =
                                                            melt_string_str ((melt_ptr_t) /*_.CURAT__V4*/ meltfptr[3])) != (char*)0
                                                    && !strcmp (meltcit1__EACHSTRMAP_nam, meltcit1__EACHSTRMAP_str))
                                                /*_.CURAT__V4*/ meltfptr[3] = /*_.CURAT__V4*/ meltfptr[3];
                                            else
                                                {
                                                    /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                                    /*_.CURAT__V4*/ meltfptr[3] =
                                                        meltgc_new_stringdup ((meltobject_ptr_t) MELT_PREDEF (DISCR_STRING),
                                                                              meltcit1__EACHSTRMAP_str);
                                                }
                                            meltcit1__EACHSTRMAP_str = (const char*)0;
                                            meltcit1__EACHSTRMAP_nam = (const char*)0;




                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1909:/ quasiblock");


                                            /*^multiapply*/
                                            /*multiapply 2args, 1x.res*/
                                            {
                                                union meltparam_un argtab[1];

                                                union meltparam_un restab[1];
                                                memset(&restab, 0, sizeof(restab));
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^multiapply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURVAL__V5*/ meltfptr[4];
                                                /*^multiapply.xres*/
                                                restab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OTHER__V8*/ meltfptr[7];
                                                /*^multiapply.appl*/
                                                /*_.TEST__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.CURAT__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, (MELTBPARSTR_PTR ""), restab);
                                            }
                                            ;
                                            /*^quasiblock*/



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#NULL__L3*/ meltfnum[2] =
                                                (/*null*/(/*_.TEST__V7*/ meltfptr[6]) == NULL);;
                                            MELT_LOCATION("warmelt-base.melt:1912:/ cond");
                                            /*cond*/ if (/*_#NULL__L3*/ meltfnum[2]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-base.melt:1913:/ quasiblock");


                                                        /*_._RETVAL___V1*/ meltfptr[0] = /*_.CURAT__V4*/ meltfptr[3];;
                                                        MELT_LOCATION("warmelt-base.melt:1913:/ putxtraresult");
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (/*_.CURVAL__V5*/ meltfptr[4]);
                                                        ;
                                                        /*^putxtraresult*/
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[1] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[1].meltbp_aptr) *(meltxrestab_[1].meltbp_aptr) = (melt_ptr_t) (/*_.OTHER__V8*/ meltfptr[7]);
                                                        ;
                                                        /*^finalreturn*/
                                                        ;
                                                        /*finalret*/ goto meltlabend_rout ;
                                                        /*_._IF___V9*/ meltfptr[8] = /*_.RETURN___V10*/ meltfptr[9];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1912:/ clear");
                                                        /*clear*/ /*_.RETURN___V10*/ meltfptr[9] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*_._IF___V9*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                                                }
                                            ;
                                            /*^compute*/
                                            /*_.MULTI___V6*/ meltfptr[5] = /*_._IF___V9*/ meltfptr[8];;

                                            MELT_LOCATION("warmelt-base.melt:1909:/ clear");
                                            /*clear*/ /*_#NULL__L3*/ meltfnum[2] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IF___V9*/ meltfptr[8] = 0 ;

                                            /*^clear*/
                                            /*clear*/ /*_.OTHER__V8*/ meltfptr[7] = 0 ;
                                            /* end foreach_in_mapstring meltcit1__EACHSTRMAP */
                                            /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/ meltfptr[4] = NULL;
                                        }

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1906:/ clear");
                                    /*clear*/ /*_.CURAT__V4*/ meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURVAL__V5*/ meltfptr[4] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MULTI___V6*/ meltfptr[5] = 0 ;
                                } /*endciterblock FOREACH_IN_MAPSTRING*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1904:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1899:/ clear");
        /*clear*/ /*_#IS_MAPSTRING__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MAPSTRING_ITERATE_TEST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_24_WARMELTmiBASE_MAPSTRING_ITERATE_TEST*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY fromline 1752 */

    /** start of frame for meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY// fromline 1568
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY), clos) {};
        MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY() //the constructor fromline 1642
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY)) {};
        MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY


    /** end of frame for meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY fromline 1697**/

    /* end of frame for routine meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY fromline 1756 */

    /* classy proc frame meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY */ MeltFrame_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_EVERY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1948:/ getarg");
    /*_.TUP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:1951:/ cond");
        /*cond*/ if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1952:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_MULTIPLE*/
                                {
                                    /* start foreach_in_multiple meltcit1__EACHTUP */
                                    long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.TUP__V2*/ meltfptr[1]);
                                    for (/*_#IX__L3*/ meltfnum[2] = 0;
                                                      (/*_#IX__L3*/ meltfnum[2] >= 0) && (/*_#IX__L3*/ meltfnum[2] <  meltcit1__EACHTUP_ln);
                                                      /*_#IX__L3*/ meltfnum[2]++)
                                        {
                                            /*_.COMP__V4*/ meltfptr[3] = melt_multiple_nth((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1]),  /*_#IX__L3*/ meltfnum[2]);




                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1956:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_long = /*_#IX__L3*/ meltfnum[2];
                                                /*_.F__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.COMP__V4*/ meltfptr[3]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            if (/*_#IX__L3*/ meltfnum[2]<0) break;
                                        } /* end  foreach_in_multiple meltcit1__EACHTUP */

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1953:/ clear");
                                    /*clear*/ /*_.COMP__V4*/ meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IX__L3*/ meltfnum[2] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.F__V5*/ meltfptr[4] = 0 ;
                                } /*endciterblock FOREACH_IN_MULTIPLE*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1951:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1948:/ clear");
        /*clear*/ /*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_25_WARMELTmiBASE_MULTIPLE_EVERY*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1752 */

    /** start of frame for meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY// fromline 1568
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY() //the constructor fromline 1642
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY


    /** end of frame for meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1697**/

    /* end of frame for routine meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1756 */

    /* classy proc frame meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY */ MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_BACKWARD_EVERY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1959:/ getarg");
    /*_.TUP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:1962:/ cond");
        /*cond*/ if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1963:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_MULTIPLE_BACKWARD*/
                                {
                                    /* start foreach_in_multiple_backward meltcit1__EACHTUPBACK */
                                    long  meltcit1__EACHTUPBACK_ln = melt_multiple_length((melt_ptr_t)/*_.TUP__V2*/ meltfptr[1]);
                                    long  meltcit1__EACHTUPBACK_ix = 0;
                                    for (meltcit1__EACHTUPBACK_ix = meltcit1__EACHTUPBACK_ln - 1;
                                            meltcit1__EACHTUPBACK_ix >= 0;
                                            meltcit1__EACHTUPBACK_ix--)
                                        {
                                            /*_.COMP__V4*/ meltfptr[3] = melt_multiple_nth((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1]),  meltcit1__EACHTUPBACK_ix);
                                            /*_#IX__L3*/ meltfnum[2]  =  meltcit1__EACHTUPBACK_ix;



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1967:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_long = /*_#IX__L3*/ meltfnum[2];
                                                /*_.F__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.COMP__V4*/ meltfptr[3]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                        } /* end  foreach_in_multiple_backward meltcit1__EACHTUPBACK */

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1964:/ clear");
                                    /*clear*/ /*_.COMP__V4*/ meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IX__L3*/ meltfnum[2] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.F__V5*/ meltfptr[4] = 0 ;
                                } /*endciterblock FOREACH_IN_MULTIPLE_BACKWARD*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1962:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1959:/ clear");
        /*clear*/ /*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_BACKWARD_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_26_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1752 */

    /** start of frame for meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH// fromline 1568
        : public Melt_CallFrameWithValues<13>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<13> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH), clos) {};
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH() //the constructor fromline 1642
            : Melt_CallFrameWithValues<13> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH)) {};
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH


    /** end of frame for meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1697**/

    /* end of frame for routine meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1756 */

    /* classy proc frame meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH */ MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_EVERY_BOTH", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1971:/ getarg");
    /*_.TUP1__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.TUP2__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V4*/ meltfptr[3])) != NULL);

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
        /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:1974:/ cond");
        /*cond*/ if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_MULTIPLE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2])) == MELTOBMAG_MULTIPLE);;
                    MELT_LOCATION("warmelt-base.melt:1975:/ cond");
                    /*cond*/ if (/*_#IS_MULTIPLE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_CLOSURE__L3*/ meltfnum[2] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.F__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
                                MELT_LOCATION("warmelt-base.melt:1976:/ cond");
                                /*cond*/ if (/*_#IS_CLOSURE__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:1977:/ quasiblock");


                                            /*_#LN1__L4*/ meltfnum[3] =
                                                (melt_multiple_length((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1])));;
                                            /*^compute*/
                                            /*_#LN2__L5*/ meltfnum[4] =
                                                (melt_multiple_length((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2])));;
                                            /*^compute*/
                                            /*_#IX__L6*/ meltfnum[5] = 0;;
                                            MELT_LOCATION("warmelt-base.melt:1980:/ loop");
                                            /*loop*/
                                            {
meltlabloop_TUPLOOP_1:;/*^loopbody*/

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    /*_#gteqI__L7*/ meltfnum[6] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) >= (/*_#LN1__L4*/ meltfnum[3]));;
                                                    MELT_LOCATION("warmelt-base.melt:1981:/ cond");
                                                    /*cond*/ if (/*_#gteqI__L7*/ meltfnum[6]) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^quasiblock*/


                                                                /*^compute*/
                                                                /*_.TUPLOOP__V10*/ meltfptr[9] =  /*reallynil*/ NULL ;;

                                                                /*^exit*/
                                                                /*exit*/
                                                                {
                                                                    goto meltlabexit_TUPLOOP_1;
                                                                }
                                                                ;
                                                                /*epilog*/
                                                            }
                                                            ;
                                                        } /*noelse*/
                                                    ;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    /*_#gteqI__L8*/ meltfnum[7] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) >= (/*_#LN2__L5*/ meltfnum[4]));;
                                                    MELT_LOCATION("warmelt-base.melt:1982:/ cond");
                                                    /*cond*/ if (/*_#gteqI__L8*/ meltfnum[7]) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^quasiblock*/


                                                                /*^compute*/
                                                                /*_.TUPLOOP__V10*/ meltfptr[9] =  /*reallynil*/ NULL ;;

                                                                /*^exit*/
                                                                /*exit*/
                                                                {
                                                                    goto meltlabexit_TUPLOOP_1;
                                                                }
                                                                ;
                                                                /*epilog*/
                                                            }
                                                            ;
                                                        } /*noelse*/
                                                    ;
                                                    /*_.MULTIPLE_NTH__V11*/ meltfptr[10] =
                                                        (melt_multiple_nth((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1]), (/*_#IX__L6*/ meltfnum[5])));;
                                                    /*^compute*/
                                                    /*_.MULTIPLE_NTH__V12*/ meltfptr[11] =
                                                        (melt_multiple_nth((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2]), (/*_#IX__L6*/ meltfnum[5])));;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1983:/ apply");
                                                    /*apply*/
                                                    {
                                                        union meltparam_un argtab[2];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^apply.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MULTIPLE_NTH__V12*/ meltfptr[11];
                                                        /*^apply.arg*/
                                                        argtab[1].meltbp_long = /*_#IX__L6*/ meltfnum[5];
                                                        /*_.F__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.MULTIPLE_NTH__V11*/ meltfptr[10]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_#plI__L9*/ meltfnum[8] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) + (1));;
                                                    MELT_LOCATION("warmelt-base.melt:1984:/ compute");
                                                    /*_#IX__L6*/ meltfnum[5] = /*_#SETQ___L10*/ meltfnum[9] = /*_#plI__L9*/ meltfnum[8];;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1980:/ clear");
                                                    /*clear*/ /*_#gteqI__L7*/ meltfnum[6] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#gteqI__L8*/ meltfnum[7] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.MULTIPLE_NTH__V11*/ meltfptr[10] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.MULTIPLE_NTH__V12*/ meltfptr[11] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.F__V13*/ meltfptr[12] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#plI__L9*/ meltfnum[8] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#SETQ___L10*/ meltfnum[9] = 0 ;
                                                }
                                                ;
                                                ;
                                                goto meltlabloop_TUPLOOP_1;
meltlabexit_TUPLOOP_1:;/*^loopepilog*/
                                                /*loopepilog*/
                                                /*_.FOREVER___V9*/ meltfptr[8] = /*_.TUPLOOP__V10*/ meltfptr[9];;
                                            }
                                            ;
                                            /*^compute*/
                                            /*_.LET___V8*/ meltfptr[7] = /*_.FOREVER___V9*/ meltfptr[8];;

                                            MELT_LOCATION("warmelt-base.melt:1977:/ clear");
                                            /*clear*/ /*_#LN1__L4*/ meltfnum[3] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#LN2__L5*/ meltfnum[4] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#IX__L6*/ meltfnum[5] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.FOREVER___V9*/ meltfptr[8] = 0 ;
                                            /*_._IF___V7*/ meltfptr[6] = /*_.LET___V8*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1976:/ clear");
                                            /*clear*/ /*_.LET___V8*/ meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_._IF___V6*/ meltfptr[5] = /*_._IF___V7*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1975:/ clear");
                                /*clear*/ /*_#IS_CLOSURE__L3*/ meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V7*/ meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_._IF___V5*/ meltfptr[4] = /*_._IF___V6*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1974:/ clear");
                    /*clear*/ /*_#IS_MULTIPLE__L2*/ meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V6*/ meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1971:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_._IF___V5*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-base.melt:1971:/ locexp");
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
        /*clear*/ /*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_EVERY_BOTH", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_BOTH*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_28_WARMELTmiBASE_SET_REFERENCE_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_28_WARMELTmiBASE_SET_REFERENCE fromline 1752 */

    /** start of frame for meltrout_28_WARMELTmiBASE_SET_REFERENCE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE// fromline 1568
        : public Melt_CallFrameWithValues<3>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE), clos) {};
        MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE)) {};
        MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE


    /** end of frame for meltrout_28_WARMELTmiBASE_SET_REFERENCE fromline 1697**/

    /* end of frame for routine meltrout_28_WARMELTmiBASE_SET_REFERENCE fromline 1756 */

    /* classy proc frame meltrout_28_WARMELTmiBASE_SET_REFERENCE */ MeltFrame_meltrout_28_WARMELTmiBASE_SET_REFERENCE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_28_WARMELTmiBASE_SET_REFERENCE fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("SET_REFERENCE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:2053:/ getarg");
    /*_.R__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.V__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.V__V3*/ meltfptr[2])) != NULL);

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
        MELT_LOCATION("warmelt-base.melt:2056:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.R__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @REFERENCED_VALUE", melt_magic_discr((melt_ptr_t)(/*_.R__V2*/ meltfptr[1])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.R__V2*/ meltfptr[1]), (0), (/*_.V__V3*/ meltfptr[2]), "REFERENCED_VALUE");
                    ;
                    /*^touch*/
                    meltgc_touch(/*_.R__V2*/ meltfptr[1]);
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.R__V2*/ meltfptr[1], "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("SET_REFERENCE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_28_WARMELTmiBASE_SET_REFERENCE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_28_WARMELTmiBASE_SET_REFERENCE*/




/**** end of warmelt-base+01.cc ****/
