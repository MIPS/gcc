/* Test for valid and invalid combinations of type specifiers.
   Based off gcc.dg/test-spec-1.c */
/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

typedef char type;

void _Fract *x0;  /* { dg-error "error" "void _Fract" } */
char _Fract *x3;  /* { dg-error "error" "char _Fract" } */
short _Fract *x6;
int _Fract *x9;   /* { dg-error "error" "int _Fract" } */
long _Fract *x12;
float _Fract *x15;   /* { dg-error "error" "float _Fract" } */
double _Fract *x18;   /* { dg-error "error" "double _Fract" } */
signed _Fract *x21;
_Bool _Fract *x24;   /* { dg-error "error" "_Bool _Fract" } */
int _Fract *x27;    /* { dg-error "error" "int _Fract" } */
type _Fract *x30;   /* { dg-error "error" "type _Fract" } */
char signed _Fract *x33;  /* { dg-error "error" "char signed _Fract" } */
char unsigned _Fract *x36; /* { dg-error "error" "char unsigned _Fract" } */
short _Fract *x39;
short signed _Fract *x42;
short unsigned _Fract *x45;
_Fract short*x48;
_Fract short signed*x51;
_Fract short unsigned*x54;
_Fract long*x57;
_Fract long signed*x60;
_Fract long unsigned*x63;
_Fract signed long*x66;
_Fract signed short*x69;
int signed _Fract *x72;  /* { dg-error "error" "int signed _Fract" } */
int unsigned _Fract *x75;  /* { dg-error "error" "int unsigned _Fract" } */
long int _Fract *x78;  /* { dg-error "error" "long int _Fract" } */
long long _Fract *x81;
long double _Fract *x84;  /* { dg-error "error" "long double _Fract" } */
long signed _Fract *x87;
long unsigned _Fract *x90;
double long _Fract *x93;  /* { dg-error "error" "double long _Fract" } */
signed char _Fract *x96;   /* { dg-error "error" "signed char _Fract" } */
signed short _Fract *x99;
signed int _Fract *x102;  /* { dg-error "error" "signed int _Fract" } */
signed long _Fract *x105;
unsigned char _Fract *x108; /* { dg-error "error" "unsigned char _Fract" } */
unsigned short _Fract *x111;
unsigned int _Fract *x114; /* { dg-error "error" "unsigned int _Fract" } */
unsigned long _Fract *x117;
short int signed _Fract *x120; /* { dg-error "error" "short int signed _Fract" } */
short int unsigned _Fract *x123; /* { dg-error "error" "short int unsigned _Fract" } */
short signed int _Fract *x126; /* { dg-error "error" "short signed int _Fract" } */
short unsigned int _Fract *x129; /* { dg-error "error" "short unsigned int _Fract" } */
int short signed _Fract *x132; /* { dg-error "error" "int short signed _Fract" } */
int short unsigned _Fract *x135; /* { dg-error "error" "int short unsigned _Fract" } */
int long long _Fract *x138; /* { dg-error "error" "int long long _Fract" } */
int long signed _Fract *x141; /* { dg-error "error" "int long signed _Fract" } */
int long unsigned _Fract *x144; /* { dg-error "error" "int long unsigned _Fract" } */
int signed short _Fract *x147; /* { dg-error "error" "int signed short _Fract" } */
int signed long _Fract *x150; /* { dg-error "error" "int signed long _Fract" } */
int unsigned short _Fract *x153; /* { dg-error "error" "int unsigned short _Fract" } */
int unsigned long _Fract *x156; /* { dg-error "error" "int unsigned long _Fract" } */
long int long _Fract *x159; /* { dg-error "error" "long int long _Fract" } */
long int signed _Fract *x162; /* { dg-error "error" "long int signed _Fract" } */
long int unsigned _Fract *x165; /* { dg-error "error" "long int unsigned _Fract" } */
long long int _Fract *x168; /* { dg-error "error" "long long int _Fract" } */
long long signed _Fract *x171;
long long unsigned _Fract *x175;
long signed int _Fract *x178; /* { dg-error "error" "long signed int _Fract" } */
long unsigned int _Fract *x181; /* { dg-error "error" "long unsigned int _Fract" } */
long unsigned long _Fract *x184;
signed short int _Fract *x187; /* { dg-error "error" "signed short int _Fract" } */
signed int short _Fract *x190; /* { dg-error "error" "signed int short _Fract" } */
signed int long _Fract *x192; /* { dg-error "error" "signed int long _Fract" } */
signed long int _Fract *x195; /* { dg-error "error" "signed long int _Fract" } */
signed long long _Fract *x198;
unsigned short int _Fract *x201; /* { dg-error "error" "unsigned short int _Fract" } */
unsigned int short _Fract *x204; /* { dg-error "error" "unsigned int short _Fract" } */
unsigned int long _Fract *x207; /* { dg-error "error" "unsigned int long _Fract" } */
unsigned long int _Fract *x210; /* { dg-error "error" "unsigned long int _Fract" } */
unsigned long long _Fract *x213;
int long long signed _Fract *x216; /* { dg-error "error" "int long long signed _Fract" } */
int long long unsigned _Fract *x219; /* { dg-error "error" "int long long unsigned _Fract" } */
int long signed long _Fract *x222; /* { dg-error "error" "int long signed long _Fract" } */
int long unsigned long _Fract *x226; /* { dg-error "error" "int long unsigned long _Fract" } */
int signed long long _Fract *x229; /* { dg-error "error" "int signed long long _Fract" } */
int unsigned long long _Fract *x232; /* { dg-error "error" "int unsigned long long _Fract" } */
long int long signed _Fract *x235; /* { dg-error "error" "long int long signed _Fract" } */
long int long unsigned _Fract *x238; /* { dg-error "error" "long int long unsigned _Fract" } */
long int signed long _Fract *x241; /* { dg-error "error" "long int signed long _Fract" } */
long int unsigned long _Fract *x244; /* { dg-error "error" "long int unsigned long _Fract" } */
long long int signed _Fract *x247; /* { dg-error "error" "long long int signed _Fract" } */
long long int unsigned _Fract *x250; /* { dg-error "error" "long long int unsigned _Fract" } */
long long signed int _Fract *x253; /* { dg-error "error" "long long signed int _Fract" } */
long long unsigned int _Fract *x256; /* { dg-error "error" "long long unsigned int _Fract" } */
long signed int long _Fract *x259; /* { dg-error "error" "long signed int long _Fract" } */
long signed long int _Fract *x262; /* { dg-error "error" "long signed long int _Fract" } */
long unsigned int long _Fract *x265; /* { dg-error "error" "long unsigned int long _Fract" } */
long unsigned long int _Fract *x268; /* { dg-error "error" "long unsigned long int _Fract" } */
signed long long int _Fract *x271; /* { dg-error "error" "signed long long int _Fract" } */
unsigned int long long _Fract *x274; /* { dg-error "error" "unsigned int long long _Fract" } */
unsigned long int long _Fract *x277; /* { dg-error "error" "unsigned long int long _Fract" } */
unsigned long long int _Fract *x280; /* { dg-error "error" "unsigned long long int _Fract" } */
_Complex _Fract *x283; /* { dg-error "error" "_Complex _Fract" } */
_Fract _Complex *x286; /* { dg-error "error" "_Fract _Complex" } */
unsigned _Fract *x289;
signed _Fract *x292;

void _Accum *k0;  /* { dg-error "error" "void _Accum" } */
char _Accum *k3;  /* { dg-error "error" "char _Accum" } */
short _Accum *k6;
int _Accum *k9;   /* { dg-error "error" "int _Accum" } */
long _Accum *k12;
float _Accum *k15;   /* { dg-error "error" "float _Accum" } */
double _Accum *k18;   /* { dg-error "error" "double _Accum" } */
signed _Accum *k21;
_Bool _Accum *k24;   /* { dg-error "error" "_Bool _Accum" } */
int _Accum *k27;    /* { dg-error "error" "int _Accum" } */
type _Accum *k30;   /* { dg-error "error" "type _Accum" } */
char signed _Accum *k33;  /* { dg-error "error" "char signed _Accum" } */
char unsigned _Accum *k36; /* { dg-error "error" "char unsigned _Accum" } */
short _Accum *k39;
short signed _Accum *k42;
short unsigned _Accum *k45;
_Accum short*k48;
_Accum short signed*k51;
_Accum short unsigned*k54;
_Accum long*k57;
_Accum long signed*k60;
_Accum long unsigned*k63;
_Accum signed long*k66;
_Accum signed short*k69;
int signed _Accum *k72;  /* { dg-error "error" "int signed _Accum" } */
int unsigned _Accum *k75;  /* { dg-error "error" "int unsigned _Accum" } */
long int _Accum *k78;  /* { dg-error "error" "long int _Accum" } */
long long _Accum *k81;
long double _Accum *k84;  /* { dg-error "error" "long double _Accum" } */
long signed _Accum *k87;
long unsigned _Accum *k90;
double long _Accum *k93;  /* { dg-error "error" "double long _Accum" } */
signed char _Accum *k96;   /* { dg-error "error" "signed char _Accum" } */
signed short _Accum *k99;
signed int _Accum *k102;  /* { dg-error "error" "signed int _Accum" } */
signed long _Accum *k105;
unsigned char _Accum *k108; /* { dg-error "error" "unsigned char _Accum" } */
unsigned short _Accum *k111;
unsigned int _Accum *k114; /* { dg-error "error" "unsigned int _Accum" } */
unsigned long _Accum *k117;
short int signed _Accum *k120; /* { dg-error "error" "short int signed _Accum" } */
short int unsigned _Accum *k123; /* { dg-error "error" "short int unsigned _Accum" } */
short signed int _Accum *k126; /* { dg-error "error" "short signed int _Accum" } */
short unsigned int _Accum *k129; /* { dg-error "error" "short unsigned int _Accum" } */
int short signed _Accum *k132; /* { dg-error "error" "int short signed _Accum" } */
int short unsigned _Accum *k135; /* { dg-error "error" "int short unsigned _Accum" } */
int long long _Accum *k138; /* { dg-error "error" "int long long _Accum" } */
int long signed _Accum *k141; /* { dg-error "error" "int long signed _Accum" } */
int long unsigned _Accum *k144; /* { dg-error "error" "int long unsigned _Accum" } */
int signed short _Accum *k147; /* { dg-error "error" "int signed short _Accum" } */
int signed long _Accum *k150; /* { dg-error "error" "int signed long _Accum" } */
int unsigned short _Accum *k153; /* { dg-error "error" "int unsigned short _Accum" } */
int unsigned long _Accum *k156; /* { dg-error "error" "int unsigned long _Accum" } */
long int long _Accum *k159; /* { dg-error "error" "long int long _Accum" } */
long int signed _Accum *k162; /* { dg-error "error" "long int signed _Accum" } */
long int unsigned _Accum *k165; /* { dg-error "error" "long int unsigned _Accum" } */
long long int _Accum *k168; /* { dg-error "error" "long long int _Accum" } */
long long signed _Accum *k171;
long long unsigned _Accum *k175;
long signed int _Accum *k178; /* { dg-error "error" "long signed int _Accum" } */
long unsigned int _Accum *k181; /* { dg-error "error" "long unsigned int _Accum" } */
long unsigned long _Accum *k184;
signed short int _Accum *k187; /* { dg-error "error" "signed short int _Accum" } */
signed int short _Accum *k190; /* { dg-error "error" "signed int short _Accum" } */
signed int long _Accum *k192; /* { dg-error "error" "signed int long _Accum" } */
signed long int _Accum *k195; /* { dg-error "error" "signed long int _Accum" } */
signed long long _Accum *k198;
unsigned short int _Accum *k201; /* { dg-error "error" "unsigned short int _Accum" } */
unsigned int short _Accum *k204; /* { dg-error "error" "unsigned int short _Accum" } */
unsigned int long _Accum *k207; /* { dg-error "error" "unsigned int long _Accum" } */
unsigned long int _Accum *k210; /* { dg-error "error" "unsigned long int _Accum" } */
unsigned long long _Accum *k213;
int long long signed _Accum *k216; /* { dg-error "error" "int long long signed _Accum" } */
int long long unsigned _Accum *k219; /* { dg-error "error" "int long long unsigned _Accum" } */
int long signed long _Accum *k222; /* { dg-error "error" "int long signed long _Accum" } */
int long unsigned long _Accum *k226; /* { dg-error "error" "int long unsigned long _Accum" } */
int signed long long _Accum *k229; /* { dg-error "error" "int signed long long _Accum" } */
int unsigned long long _Accum *k232; /* { dg-error "error" "int unsigned long long _Accum" } */
long int long signed _Accum *k235; /* { dg-error "error" "long int long signed _Accum" } */
long int long unsigned _Accum *k238; /* { dg-error "error" "long int long unsigned _Accum" } */
long int signed long _Accum *k241; /* { dg-error "error" "long int signed long _Accum" } */
long int unsigned long _Accum *k244; /* { dg-error "error" "long int unsigned long _Accum" } */
long long int signed _Accum *k247; /* { dg-error "error" "long long int signed _Accum" } */
long long int unsigned _Accum *k250; /* { dg-error "error" "long long int unsigned _Accum" } */
long long signed int _Accum *k253; /* { dg-error "error" "long long signed int _Accum" } */
long long unsigned int _Accum *k256; /* { dg-error "error" "long long unsigned int _Accum" } */
long signed int long _Accum *k259; /* { dg-error "error" "long signed int long _Accum" } */
long signed long int _Accum *k262; /* { dg-error "error" "long signed long int _Accum" } */
long unsigned int long _Accum *k265; /* { dg-error "error" "long unsigned int long _Accum" } */
long unsigned long int _Accum *k268; /* { dg-error "error" "long unsigned long int _Accum" } */
signed long long int _Accum *k271; /* { dg-error "error" "signed long long int _Accum" } */
unsigned int long long _Accum *k274; /* { dg-error "error" "unsigned int long long _Accum" } */
unsigned long int long _Accum *k277; /* { dg-error "error" "unsigned long int long _Accum" } */
unsigned long long int _Accum *k280; /* { dg-error "error" "unsigned long long int _Accum" } */
_Complex _Accum *k283; /* { dg-error "error" "_Complex _Accum" } */
_Accum _Complex *k286; /* { dg-error "error" "_Accum _Complex" } */
unsigned _Accum *k289;
signed _Accum *k292;
