/* { dg-do assemble } */
/* FIXME lto.  We should use 'dg-do compile', but -S and -flto-single
   confuse cc1 because the flags specify two different output files.  */

enum _Ios_Fmtflags     {
 _S_boolalpha };

class ios_base   {
 static const  _Ios_Fmtflags boolalpha =   _S_boolalpha;
 _Ios_Fmtflags _M_flags;
};

ios_base& g() {
}
