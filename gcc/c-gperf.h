/* C code produced by gperf version 2.7 */
/* Command-line: gperf -L C -F , 0, 0 -p -j1 -i 1 -g -o -t -G -N is_reserved_word -k1,3,$ ../../src/gcc/c-parse.gperf  */
/* Command-line: gperf -L C -F ', 0, 0' -p -j1 -i 1 -g -o -t -N is_reserved_word -k1,3,$ c-parse.gperf  */ 
struct resword { const char *name; short token; enum rid rid; };
#ifdef __GNUC__
__inline
#endif
static unsigned int hash PARAMS ((const char *, unsigned int));
#ifdef __GNUC__
__inline
#endif
struct resword *is_reserved_word PARAMS ((const char *, unsigned int));

#define TOTAL_KEYWORDS 92
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 20
#define MIN_HASH_VALUE 13
#define MAX_HASH_VALUE 239
/* maximum key range = 227, duplicates = 0 */

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
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240,  90, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240,   1, 240,  84,  44,  98,
       33,   9,  24,  41,   7,  20, 240,   6,  84,  18,
       55,  55,  30, 240,   7,   6,   1,  44,  31,   1,
        5,   2,   1, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
      240, 240, 240, 240, 240, 240
    };
  register int hval = len;

  switch (hval)
    {
      default:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

static struct resword wordlist[] =
  {
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"__typeof__", TYPEOF, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"__real__", REALPART, NORID},
    {"__signed__", TYPESPEC, RID_SIGNED},
    {"__restrict", TYPE_QUAL, RID_RESTRICT},
    {"struct", STRUCT, NORID},
    {"__restrict__", TYPE_QUAL, RID_RESTRICT},
    {"restrict", TYPE_QUAL, RID_RESTRICT},
    {"", 0, 0},
    {"__extension__", EXTENSION, NORID},
    {"int", TYPESPEC, RID_INT},
    {"", 0, 0}, {"", 0, 0},
    {"else", ELSE, NORID},
    {"", 0, 0},
    {"__imag__", IMAGPART, NORID},
    {"", 0, 0},
    {"__inline__", SCSPEC, RID_INLINE},
    {"", 0, 0},
    {"__typeof", TYPEOF, NORID},
    {"while", WHILE, NORID},
    {"", 0, 0},
    {"sizeof", SIZEOF, NORID},
    {"__inline", SCSPEC, RID_INLINE},
    {"switch", SWITCH, NORID},
    {"__ptrlow", PTR_LOW_BOUND, NORID},
    {"for", FOR, NORID},
    {"__ptrlow__", PTR_LOW_BOUND, NORID},
    {"__ptrhigh__", PTR_HIGH_BOUND, NORID},
    {"__ptrvalue__", PTR_VALUE, NORID},
    {"__volatile__", TYPE_QUAL, RID_VOLATILE},
    {"if", IF, NORID},
    {"__ptrhigh", PTR_HIGH_BOUND, NORID},
    {"__signed", TYPESPEC, RID_SIGNED},
    {"", 0, 0},
    {"__ptrvalue", PTR_VALUE, NORID},
    {"__volatile", TYPE_QUAL, RID_VOLATILE},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"id", OBJECTNAME, RID_ID},
    {"", 0, 0},
    {"__bounded__", TYPE_QUAL, RID_BOUNDED},
    {"", 0, 0},
    {"__unbounded__", TYPE_QUAL, RID_UNBOUNDED},
    {"out", TYPE_QUAL, RID_OUT},
    {"typeof", TYPEOF, NORID},
    {"typedef", SCSPEC, RID_TYPEDEF},
    {"register", SCSPEC, RID_REGISTER},
    {"break", BREAK, NORID},
    {"default", DEFAULT, NORID},
    {"", 0, 0},
    {"short", TYPESPEC, RID_SHORT},
    {"__imag", IMAGPART, NORID},
    {"return", RETURN, NORID},
    {"", 0, 0},
    {"extern", SCSPEC, RID_EXTERN},
    {"oneway", TYPE_QUAL, RID_ONEWAY},
    {"", 0, 0}, {"", 0, 0},
    {"enum", ENUM, NORID},
    {"", 0, 0},
    {"in", TYPE_QUAL, RID_IN},
    {"", 0, 0}, {"", 0, 0},
    {"byref", TYPE_QUAL, RID_BYREF},
    {"inout", TYPE_QUAL, RID_INOUT},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"float", TYPESPEC, RID_FLOAT},
    {"signed", TYPESPEC, RID_SIGNED},
    {"__bounded", TYPE_QUAL, RID_BOUNDED},
    {"void", TYPESPEC, RID_VOID},
    {"__unbounded", TYPE_QUAL, RID_UNBOUNDED},
    {"do", DO, NORID},
    {"unsigned", TYPESPEC, RID_UNSIGNED},
    {"double", TYPESPEC, RID_DOUBLE},
    {"__asm__", ASM_KEYWORD, NORID},
    {"", 0, 0},
    {"__label__", LABEL, NORID},
    {"", 0, 0},
    {"__alignof__", ALIGNOF, NORID},
    {"__real", REALPART, NORID},
    {"__attribute__", ATTRIBUTE, NORID},
    {"", 0, 0},
    {"goto", GOTO, NORID},
    {"__builtin_va_arg", VA_ARG, NORID},
    {"", 0, 0}, {"", 0, 0},
    {"__attribute", ATTRIBUTE, NORID},
    {"", 0, 0},
    {"__const", TYPE_QUAL, RID_CONST},
    {"__asm", ASM_KEYWORD, NORID},
    {"__const__", TYPE_QUAL, RID_CONST},
    {"@defs", DEFS, NORID},
    {"__complex__", TYPESPEC, RID_COMPLEX},
    {"", 0, 0},
    {"__complex", TYPESPEC, RID_COMPLEX},
    {"@private", PRIVATE, NORID},
    {"@selector", SELECTOR, NORID},
    {"", 0, 0},
    {"case", CASE, NORID},
    {"__alignof", ALIGNOF, NORID},
    {"inline", SCSPEC, RID_INLINE},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"asm", ASM_KEYWORD, NORID},
    {"union", UNION, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"volatile", TYPE_QUAL, RID_VOLATILE},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"@protected", PROTECTED, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"auto", SCSPEC, RID_AUTO},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"bycopy", TYPE_QUAL, RID_BYCOPY},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"const", TYPE_QUAL, RID_CONST},
    {"", 0, 0},
    {"@encode", ENCODE, NORID},
    {"", 0, 0}, {"", 0, 0},
    {"@interface", INTERFACE, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"continue", CONTINUE, NORID},
    {"@compatibility_alias", ALIAS, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"@implementation", IMPLEMENTATION, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"@end", END, NORID},
    {"", 0, 0},
    {"long", TYPESPEC, RID_LONG},
    {"", 0, 0},
    {"@class", CLASS, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"@protocol", PROTOCOL, NORID},
    {"", 0, 0}, {"", 0, 0},
    {"char", TYPESPEC, RID_CHAR},
    {"static", SCSPEC, RID_STATIC},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"@public", PUBLIC, NORID}
  };

#ifdef __GNUC__
__inline
#endif
struct resword *
is_reserved_word (str, len)
     register const char *str;
     register unsigned int len;
{
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
