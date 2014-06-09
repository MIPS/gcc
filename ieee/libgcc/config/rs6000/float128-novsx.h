/* Map __<func>tf<n> to the name used for __float128 when used on system
   without VSX.  We compile two versions, one that passes __float128 in a
   vector register, and the other that expects it to be a pair of scalar FPR
   registers.  */

#define __addtf3	__addtf3_fpr
#define __subtf3	__subtf3_fpr
#define __multf3	__multf3_fpr
#define __divtf3	__divtf3_fpr
#define __eqtf2		__eqtf2_fpr
#define __netf2		__netf2_fpr
#define __getf2		__getf2_fpr
#define __gttf2		__gttf2_fpr
#define __letf2		__letf2_fpr
#define __lttf2		__lttf2_fpr
#define __unordtf2	__unordtf2_fpr
#define __negtf2	__negtf2_fpr
#define __extenddftf2	__extenddftf2_fpr
#define __extendsftf2	__extendsftf2_fpr
#define __trunctfdf2	__trunctfdf2_fpr
#define __trunctfsf2	__trunctfsf2_fpr
#define __fixtfsi	__fixtfsi_fpr
#define __fixtfdi	__fixtfdi_fpr
#define __fixtfti	__fixtfti_fpr
#define __fixunstfsi	__fixunstfsi_fpr
#define __fixunstfdi	__fixunstfdi_fpr
#define __fixunstfti	__fixunstfti_fpr
#define __floatsitf	__floatsitf_fpr
#define __floatditf	__floatditf_fpr
#define __floattitf	__floattitf_fpr
#define __floatunsitf	__floatunsitf_fpr
#define __floatunditf	__floatunditf_fpr
#define __floatuntitf	__floatuntitf_fpr
