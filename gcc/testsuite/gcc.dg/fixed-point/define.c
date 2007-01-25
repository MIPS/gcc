/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* Check if defined fixed-point constants are ok.  */

short _Fract sf = __SFRACT_MAX__;
_Fract f = __FRACT_MAX__;
long _Fract lf = __LFRACT_MAX__;
long long _Fract llf = __LLFRACT_MAX__;

unsigned short _Fract usf = __USFRACT_MAX__;
unsigned _Fract uf = __UFRACT_MAX__;
unsigned long _Fract ulf = __ULFRACT_MAX__;
unsigned long long _Fract ullf = __ULLFRACT_MAX__;

short _Fract sfm = __SFRACT_MIN__;
_Fract fm = __FRACT_MIN__;
long _Fract lfm = __LFRACT_MIN__;
long long _Fract llfm = __LLFRACT_MIN__;

unsigned short _Fract usfm = __USFRACT_MIN__;
unsigned _Fract ufm = __UFRACT_MIN__;
unsigned long _Fract ulfm = __ULFRACT_MIN__;
unsigned long long _Fract ullfm = __ULLFRACT_MIN__;

short _Fract sfE = __SFRACT_EPSILON__;
_Fract fE = __FRACT_EPSILON__;
long _Fract lfE = __LFRACT_EPSILON__;
long long _Fract llfE = __LLFRACT_EPSILON__;

unsigned short _Fract usfE = __USFRACT_EPSILON__;
unsigned _Fract ufE = __UFRACT_EPSILON__;
unsigned long _Fract ulfE = __ULFRACT_EPSILON__;
unsigned long long _Fract ullfE = __ULLFRACT_EPSILON__;

short _Accum sk = __SACCUM_MAX__;
_Accum k = __ACCUM_MAX__;
long _Accum lk = __LACCUM_MAX__;
long long _Accum llk = __LLACCUM_MAX__;

unsigned short _Accum usk = __USACCUM_MAX__;
unsigned _Accum uk = __UACCUM_MAX__;
unsigned long _Accum ulk = __ULACCUM_MAX__;
unsigned long long _Accum ullk = __ULLACCUM_MAX__;

short _Accum skm = __SACCUM_MIN__;
_Accum km = __ACCUM_MIN__;
long _Accum lkm = __LACCUM_MIN__;
long long _Accum llkm = __LLACCUM_MIN__;

unsigned short _Accum uskm = __USACCUM_MIN__;
unsigned _Accum ukm = __UACCUM_MIN__;
unsigned long _Accum ulkm = __ULACCUM_MIN__;
unsigned long long _Accum ullkm = __ULLACCUM_MIN__;

short _Accum skE = __SACCUM_EPSILON__;
_Accum kE = __ACCUM_EPSILON__;
long _Accum lkE = __LACCUM_EPSILON__;
long long _Accum llkE = __LLACCUM_EPSILON__;

unsigned short _Accum uskE = __USACCUM_EPSILON__;
unsigned _Accum ukE = __UACCUM_EPSILON__;
unsigned long _Accum ulkE = __ULACCUM_EPSILON__;
unsigned long long _Accum ullkE = __ULLACCUM_EPSILON__;
