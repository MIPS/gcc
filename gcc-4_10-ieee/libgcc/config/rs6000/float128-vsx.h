/* Map __<func>tf<n> to the name used for __float128 when used on a VSX system.
   We compile two versions, one that passes __float128 in a vector register,
   and the other that expects it to be a pair of scalar FPR registers.  */

#define __addtf3	__addtf3_vector
#define __subtf3	__subtf3_vector
#define __multf3	__multf3_vector
#define __divtf3	__divtf3_vector
#define __eqtf2		__eqtf2_vector
#define __netf2		__netf2_vector
#define __getf2		__getf2_vector
#define __gttf2		__gttf2_vector
#define __letf2		__letf2_vector
#define __lttf2		__lttf2_vector
#define __unordtf2	__unordtf2_vector
#define __negtf2	__negtf2_vector
#define __extenddftf2	__extenddftf2_vector
#define __extendsftf2	__extendsftf2_vector
#define __trunctfdf2	__trunctfdf2_vector
#define __trunctfsf2	__trunctfsf2_vector
#define __fixtfsi	__fixtfsi_vector
#define __fixtfdi	__fixtfdi_vector
#define __fixtfti	__fixtfti_vector
#define __fixunstfsi	__fixunstfsi_vector
#define __fixunstfdi	__fixunstfdi_vector
#define __fixunstfti	__fixunstfti_vector
#define __floatsitf	__floatsitf_vector
#define __floatditf	__floatditf_vector
#define __floattitf	__floattitf_vector
#define __floatunsitf	__floatunsitf_vector
#define __floatunditf	__floatunditf_vector
#define __floatuntitf	__floatuntitf_vector
