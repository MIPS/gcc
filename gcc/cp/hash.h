/* C code produced by gperf version 2.7 */
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
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 237
/* maximum key range = 231, duplicates = 0 */

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
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238,   0, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238,   0, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238,   0, 238,  23, 122,  70,
        6,   5, 109,  54,   8,  60, 238,   1,  36,  54,
        4,  49,  11, 117,  16,  23,   0,  71,  73,  78,
       10,  71, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
      238, 238, 238, 238, 238, 238
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
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"not", '!', RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"__real__", REALPART, RID_UNUSED},
      {"true", CXX_TRUE, RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0},
      {"typeid", TYPEID, RID_UNUSED,},
      {"__inline__", SCSPEC, RID_INLINE},
      {"else", ELSE, RID_UNUSED,},
      {"extern", SCSPEC, RID_EXTERN,},
      {"__inline", SCSPEC, RID_INLINE},
      {"delete", DELETE, RID_UNUSED,},
      {"", 0, 0},
      {"template", TEMPLATE, RID_TEMPLATE,},
      {"", 0, 0},
      {"protected", VISSPEC, RID_PROTECTED,},
      {"__extension__", EXTENSION, RID_UNUSED},
      {"", 0, 0},
      {"xor", '^', RID_UNUSED,},
      {"__asm__", ASM_KEYWORD, RID_UNUSED},
      {"__restrict", CV_QUALIFIER, RID_RESTRICT},
      {"and", ANDAND, RID_UNUSED,},
      {"__restrict__", CV_QUALIFIER, RID_RESTRICT},
      {"static_cast", STATIC_CAST, RID_UNUSED,},
      {"__ptrvalue__", PTR_VALUE, RID_UNUSED},
      {"default", DEFAULT, RID_UNUSED,},
      {"switch", SWITCH, RID_UNUSED,},
      {"__ptrvalue", PTR_VALUE, RID_UNUSED},
      {"signed", TYPESPEC, RID_SIGNED,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"short", TYPESPEC, RID_SHORT,},
      {"", 0, 0},
      {"namespace", NAMESPACE, RID_UNUSED,},
      {"__real", REALPART, RID_UNUSED},
      {"", 0, 0}, {"", 0, 0},
      {"this", THIS, RID_UNUSED,},
      {"__alignof__", ALIGNOF, RID_UNUSED},
      {"reinterpret_cast", REINTERPRET_CAST, RID_UNUSED,},
      {"", 0, 0},
      {"   that yields a duplicate-value error from gperf.  */",},
      {"", 0, 0},
      {"__const", CV_QUALIFIER, RID_CONST},
      {"do", DO, RID_UNUSED,},
      {"__const__", CV_QUALIFIER, RID_CONST},
      {"__ptrlow__", PTR_LOW_BOUND, RID_UNUSED},
      {"export", SCSPEC, RID_EXPORT,},
      {"__volatile__", CV_QUALIFIER, RID_VOLATILE},
      {"__imag__", IMAGPART, RID_UNUSED},
      {"int", TYPESPEC, RID_INT,},
      {"__volatile", CV_QUALIFIER, RID_VOLATILE},
      {"", 0, 0},
      {"__bounded__", CV_QUALIFIER, RID_BOUNDED},
      {"or", OROR, RID_UNUSED,},
      {"__label__", LABEL, RID_UNUSED},
      {"", 0, 0},
      {"__bounded", CV_QUALIFIER, RID_BOUNDED},
      {"__ptrhigh__", PTR_HIGH_BOUND, RID_UNUSED},
      {"typename", TYPENAME_KEYWORD, RID_UNUSED,},
      {"__attribute__", ATTRIBUTE, RID_UNUSED},
      {"try", TRY, RID_UNUSED,},
      {"__signed__", TYPESPEC, RID_SIGNED},
      {"__attribute", ATTRIBUTE, RID_UNUSED},
      {"__ptrhigh", PTR_HIGH_BOUND, RID_UNUSED},
      {"", 0, 0},
      {"__signed", TYPESPEC, RID_SIGNED},
      {"asm", ASM_KEYWORD, RID_UNUSED,},
      {"", 0, 0},
      {"__asm", ASM_KEYWORD, RID_UNUSED},
      {"", 0, 0},
      {"case", CASE, RID_UNUSED,},
      {"new", NEW, RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0},
      {"__unbounded__", CV_QUALIFIER, RID_UNBOUNDED},
      {"void", TYPESPEC, RID_VOID,},
      {"", 0, 0}, {"", 0, 0},
      {"__unbounded", CV_QUALIFIER, RID_UNBOUNDED},
      {"", 0, 0},
      {"mutable", SCSPEC, RID_MUTABLE,},
      {"__wchar_t", TYPESPEC, RID_WCHAR  /* Unique to ANSI C++ */,},
      {"__complex__", TYPESPEC, RID_COMPLEX},
      {"return", RETURN_KEYWORD, RID_UNUSED,},
      {"const", CV_QUALIFIER, RID_CONST,},
      {"static", SCSPEC, RID_STATIC,},
      {"struct", AGGR, RID_RECORD,},
      {"private", VISSPEC, RID_PRIVATE,},
      {"", 0, 0}, {"", 0, 0},
      {"__complex", TYPESPEC, RID_COMPLEX},
      {"register", SCSPEC, RID_REGISTER,},
      {"char", TYPESPEC, RID_CHAR,},
      {"", 0, 0}, {"", 0, 0},
      {"explicit", SCSPEC, RID_EXPLICIT,},
      {"/* __bounded & __bounded__ can't be alphabetized because",},
      {"dynamic_cast", DYNAMIC_CAST, RID_UNUSED,},
      {"", 0, 0},
      {"__null", CONSTANT, RID_NULL},
      {"__imag", IMAGPART, RID_UNUSED},
      {"", 0, 0}, {"", 0, 0},
      {"enum", ENUM, RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0},
      {"typeof", TYPEOF, RID_UNUSED,},
      {"class", AGGR, RID_CLASS,},
      {"compl", '~', RID_UNUSED,},
      {"public", VISSPEC, RID_PUBLIC,},
      {"while", WHILE, RID_UNUSED,},
      {"auto", SCSPEC, RID_AUTO,},
      {"friend", SCSPEC, RID_FRIEND,},
      {"not_eq", EQCOMPARE, RID_UNUSED,},
      {"for", FOR, RID_UNUSED,},
      {"union", AGGR, RID_UNION,},
      {"__typeof__", TYPEOF, RID_UNUSED},
      {"inline", SCSPEC, RID_INLINE,},
      {"throw", THROW, RID_UNUSED,},
      {"xor_eq", ASSIGN, RID_UNUSED,},
      {"using", USING, RID_UNUSED,},
      {"__ptrlow", PTR_LOW_BOUND, RID_UNUSED},
      {"", 0, 0},
      {"float", TYPESPEC, RID_FLOAT,},
      {"operator", OPERATOR, RID_UNUSED,},
      {"double", TYPESPEC, RID_DOUBLE,},
      {"", 0, 0},
      {"__builtin_va_arg", VA_ARG, RID_UNUSED},
      {"false", CXX_FALSE, RID_UNUSED,},
      {"sizeof", SIZEOF, RID_UNUSED,},
      {"", 0, 0},
      {"volatile", CV_QUALIFIER, RID_VOLATILE,},
      {"and_eq", ASSIGN, RID_UNUSED,},
      {"", 0, 0},
      {"long", TYPESPEC, RID_LONG,},
      {"", 0, 0},
      {"unsigned", TYPESPEC, RID_UNSIGNED,},
      {"break", BREAK, RID_UNUSED,},
      {"virtual", SCSPEC, RID_VIRTUAL,},
      {"catch", CATCH, RID_UNUSED,},
      {"continue", CONTINUE, RID_UNUSED,},
      {"", 0, 0},
      {"goto", GOTO, RID_UNUSED,},
      {"bitand", '&', RID_UNUSED,},
      {"__alignof", ALIGNOF, RID_UNUSED},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"if", IF, RID_UNUSED,},
      {"", 0, 0},
      {"const_cast", CONST_CAST, RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0},
      {"or_eq", ASSIGN, RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"bitor", '|', RID_UNUSED,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"bool", TYPESPEC, RID_BOOL,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"typedef", SCSPEC, RID_TYPEDEF,},
      {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
      {"__typeof", TYPEOF, RID_UNUSED}
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
