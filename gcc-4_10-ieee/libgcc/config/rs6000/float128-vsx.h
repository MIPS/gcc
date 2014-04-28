/* Map __<func>tf<n> to the name used for __float128 when used on a VSX
   system.  */

#define __addtf3	__addxf3
#define __subtf3	__subxf3
#define __multf3	__mulxf3
#define __divtf3	__divxf3
#define __eqtf2		__eqxf2
#define __netf2		__nexf2
#define __getf2		__gexf2
#define __gttf2		__gtxf2
#define __letf2		__lexf2
#define __lttf2		__ltxf2
#define __unordtf2	__unordxf2
#define __negtf2	__negxf2
#define __extenddftf2	__extenddfxf2
#define __extendsftf2	__extendsfxf2
#define __trunctfdf2	__truncxfdf2
#define __trunctfsf2	__truncxfsf2
#define __fixtfsi	__fixtxsi
#define __fixtfdi	__fixtxdi
#define __fixtfti	__fixtxti
#define __fixunstfsi	__fixunstxsi
#define __fixunstfdi	__fixunstxdi
#define __fixunstfti	__fixunstxti
#define __floatsitf	__floatsixf
#define __floatditf	__floatdixf
#define __floattitf	__floattixf
#define __floatunsitf	__floatunsixf
#define __floatunditf	__floatundixf
#define __floatuntitf	__floatuntixf
