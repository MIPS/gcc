/* C code produced by gperf version 2.7.1 (19981006 egcs) */
/* Command-line: gperf -L C -F , 0, 0 -p -j1 -g -o -t -N is_reserved_word -k1,4,7,$ ../../../src/gcc/cp/gxx.gperf  */
/* Command-line: gperf -L C -F ', 0, 0' -p -j1 -g -o -t -N is_reserved_word -k1,4,$,7 gplus.gperf  */
struct resword { const char *name; short token; enum rid rid;};
#ifdef __GNUC__
__inline
#endif
static unsigned int hash PARAMS ((const char *, unsigned int));
#ifdef __GNUC__
__inline
#endif
struct resword *is_reserved_word PARAMS ((const char *, unsigned int));

#define TOTAL_KEYWORDS 115
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 56
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 242
/* maximum key range = 237, duplicates = 0 */

#ifdef __GNUC__
__inline
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static unsigned char asso_values[] =
    {
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243,   2, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243,   1, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243,   0, 243,  52,  14,  57,
       60,   8,  92,  15,   7,  65, 243,   2, 106, 115,
       14,   0,   6,  34,  32,  48,   0,  70,   6,  16,
       18,   3, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
      243, 243, 243, 243, 243, 243
    };
  register int hval = len;

  switch (hval)
    {
      default:
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      case 6:
      case 5:
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#endif
struct resword *
is_reserved_word (str, len)
     register const char *str;
     register unsigned int len;
{
  static struct resword wordlist[] =
    {
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"try", TRY, RID_UNUSED,},
      {"__const", CV_QUALIFIER, RID_CONST},
      {"", 0, 0},
      {"__const__", CV_QUALIFIER, RID_CONST},
      {"", 0, 0}, {"", 0, 0},
      {"__volatile__", CV_QUALIFIER, RID_VOLATILE},
      {"__typeof__", TYPEOF, RID_UNUSED},
      {"export", SCSPEC, RID_EXPORT,},
      {"", 0, 0},
      {"__real__", REALPART, RID_UNUSED},
      {"not", '!', RID_UNUSED,},
      {"__volatile", CV_QUALIFIER, RID_VOLATILE},
      {"goto", GOTO, RID_UNUSED,},
      {"true", CXX_TRUE, RID_UNUSED,},
      {"throw", THROW, RID_UNUSED,},
      {"template", TEMPLATE, RID_TEMPLATE,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"else", ELSE, RID_UNUSED,},
      {"__ptrhigh", PTR_HIGH_BOUND, RID_UNUSED},
      {"", 0, 0},
      {"__ptrhigh__", PTR_HIGH_BOUND, RID_UNUSED},
      {"", 0, 0},
      {"new", NEW, RID_UNUSED,},
      {"or", OROR, RID_UNUSED,},
      {"private", VISSPEC, RID_PRIVATE,},
      {"extern", SCSPEC, RID_EXTERN,},
      {"", 0, 0},
      {"__inline__", SCSPEC, RID_INLINE},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"__inline", SCSPEC, RID_INLINE},
      {"__extension__", EXTENSION, RID_UNUSED},
      {"", 0, 0},
      {"or_eq", ASSIGN, RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0},
      {"__restrict", CV_QUALIFIER, RID_RESTRICT},
      {"bitor", '|', RID_UNUSED,},
      {"__restrict__", CV_QUALIFIER, RID_RESTRICT},
      {"xor", '^', RID_UNUSED,},
      {"not_eq", EQCOMPARE, RID_UNUSED,},
      {"__asm__", ASM_KEYWORD, RID_UNUSED},
      {"auto", SCSPEC, RID_AUTO,},
      {"   that yields a duplicate-value error from gperf.  */",},
      {"xor_eq", ASSIGN, RID_UNUSED,},
      {"static_cast", STATIC_CAST, RID_UNUSED,},
      {"", 0, 0},
      {"switch", SWITCH, RID_UNUSED,},
      {"do", DO, RID_UNUSED,},
      {"__ptrlow__", PTR_LOW_BOUND, RID_UNUSED},
      {"__ptrvalue__", PTR_VALUE, RID_UNUSED},
      {"/* __bounded & __bounded__ can't be alphabetized because",},
      {"", 0, 0}, {"", 0, 0},
      {"int", TYPESPEC, RID_INT,},
      {"__ptrlow", PTR_LOW_BOUND, RID_UNUSED},
      {"__ptrvalue", PTR_VALUE, RID_UNUSED},
      {"__bounded__", CV_QUALIFIER, RID_BOUNDED},
      {"operator", OPERATOR, RID_UNUSED,},
      {"break", BREAK, RID_UNUSED,},
      {"typeid", TYPEID, RID_UNUSED,},
      {"protected", VISSPEC, RID_PROTECTED,},
      {"", 0, 0},
      {"case", CASE, RID_UNUSED,},
      {"__attribute__", ATTRIBUTE, RID_UNUSED},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"delete", DELETE, RID_UNUSED,},
      {"__signed__", TYPESPEC, RID_SIGNED},
      {"__attribute", ATTRIBUTE, RID_UNUSED},
      {"short", TYPESPEC, RID_SHORT,},
      {"", 0, 0}, {"", 0, 0},
      {"double", TYPESPEC, RID_DOUBLE,},
      {"union", AGGR, RID_UNION,},
      {"", 0, 0},
      {"namespace", NAMESPACE, RID_UNUSED,},
      {"and_eq", ASSIGN, RID_UNUSED,},
      {"", 0, 0},
      {"reinterpret_cast", REINTERPRET_CAST, RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0},
      {"__unbounded__", CV_QUALIFIER, RID_UNBOUNDED},
      {"__wchar_t", TYPESPEC, RID_WCHAR  /* Unique to ANSI C++ */,},
      {"", 0, 0},
      {"this", THIS, RID_UNUSED,},
      {"__builtin_va_arg", VA_ARG, RID_UNUSED},
      {"", 0, 0},
      {"__typeof", TYPEOF, RID_UNUSED},
      {"using", USING, RID_UNUSED,},
      {"", 0, 0},
      {"typeof", TYPEOF, RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"const", CV_QUALIFIER, RID_CONST,},
      {"static", SCSPEC, RID_STATIC,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"and", ANDAND, RID_UNUSED,},
      {"", 0, 0},
      {"__complex__", TYPESPEC, RID_COMPLEX},
      {"", 0, 0},
      {"default", DEFAULT, RID_UNUSED,},
      {"__real", REALPART, RID_UNUSED},
      {"", 0, 0},
      {"return", RETURN_KEYWORD, RID_UNUSED,},
      {"__imag__", IMAGPART, RID_UNUSED},
      {"struct", AGGR, RID_RECORD,},
      {"char", TYPESPEC, RID_CHAR,},
      {"catch", CATCH, RID_UNUSED,},
      {"for", FOR, RID_UNUSED,},
      {"signed", TYPESPEC, RID_SIGNED,},
      {"__bounded", CV_QUALIFIER, RID_BOUNDED},
      {"void", TYPESPEC, RID_VOID,},
      {"__alignof__", ALIGNOF, RID_UNUSED},
      {"bitand", '&', RID_UNUSED,},
      {"__complex", TYPESPEC, RID_COMPLEX},
      {"", 0, 0},
      {"while", WHILE, RID_UNUSED,},
      {"__imag", IMAGPART, RID_UNUSED},
      {"", 0, 0}, {"", 0, 0},
      {"typename", TYPENAME_KEYWORD, RID_UNUSED,},
      {"long", TYPESPEC, RID_LONG,},
      {"__signed", TYPESPEC, RID_SIGNED},
      {"", 0, 0},
      {"continue", CONTINUE, RID_UNUSED,},
      {"inline", SCSPEC, RID_INLINE,},
      {"register", SCSPEC, RID_REGISTER,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"float", TYPESPEC, RID_FLOAT,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"false", CXX_FALSE, RID_UNUSED,},
      {"sizeof", SIZEOF, RID_UNUSED,},
      {"__unbounded", CV_QUALIFIER, RID_UNBOUNDED},
      {"", 0, 0}, {"", 0, 0},
      {"class", AGGR, RID_CLASS,},
      {"if", IF, RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"friend", SCSPEC, RID_FRIEND,},
      {"__label__", LABEL, RID_UNUSED},
      {"__asm", ASM_KEYWORD, RID_UNUSED},
      {"", 0, 0},
      {"asm", ASM_KEYWORD, RID_UNUSED,},
      {"", 0, 0},
      {"const_cast", CONST_CAST, RID_UNUSED,},
      {"", 0, 0},
      {"compl", '~', RID_UNUSED,},
      {"public", VISSPEC, RID_PUBLIC,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"volatile", CV_QUALIFIER, RID_VOLATILE,},
      {"dynamic_cast", DYNAMIC_CAST, RID_UNUSED,},
      {"__null", CONSTANT, RID_NULL},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"explicit", SCSPEC, RID_EXPLICIT,},
      {"", 0, 0}, {"", 0, 0},
      {"mutable", SCSPEC, RID_MUTABLE,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"typedef", SCSPEC, RID_TYPEDEF,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"", 0, 0}, {"", 0, 0},
      {"unsigned", TYPESPEC, RID_UNSIGNED,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"__alignof", ALIGNOF, RID_UNUSED},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"virtual", SCSPEC, RID_VIRTUAL,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"bool", TYPESPEC, RID_BOOL,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"", 0, 0}, {"", 0, 0},
      {"enum", ENUM, RID_UNUSED,}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
