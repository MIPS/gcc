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

#define TOTAL_KEYWORDS 94
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 20
#define MIN_HASH_VALUE 11
#define MAX_HASH_VALUE 203
/* maximum key range = 193, duplicates = 0 */

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
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204,  30, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204,   1, 204, 106,  76,  68,
       87,  29,   1,  85,  20,  13, 204,   2,  27,  23,
       58,  92,  53, 204,  19,   8,   1,  34,  34,  29,
        2,   1,   2, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
      204, 204, 204, 204, 204, 204
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
    {"", 0, 0}, {"", 0, 0},
    {"__typeof", TYPEOF, NORID},
    {"", 0, 0},
    {"__typeof__", TYPEOF, NORID},
    {"", 0, 0}, {"", 0, 0},
    {"if", IF, NORID},
    {"sizeof", SIZEOF, NORID},
    {"int", TYPESPEC, RID_INT},
    {"", 0, 0},
    {"__signed__", TYPESPEC, RID_SIGNED},
    {"", 0, 0}, {"", 0, 0},
    {"__imag__", IMAGPART, NORID},
    {"", 0, 0},
    {"__inline__", SCSPEC, RID_INLINE},
    {"", 0, 0},
    {"__iterator__", SCSPEC, RID_ITERATOR},
    {"", 0, 0},
    {"__real__", REALPART, NORID},
    {"", 0, 0},
    {"__restrict", TYPE_QUAL, RID_RESTRICT},
    {"", 0, 0},
    {"__restrict__", TYPE_QUAL, RID_RESTRICT},
    {"struct", STRUCT, NORID},
    {"", 0, 0},
    {"restrict", TYPE_QUAL, RID_RESTRICT},
    {"", 0, 0},
    {"__label__", LABEL, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"for", FOR, NORID},
    {"__iterator", SCSPEC, RID_ITERATOR},
    {"__extension__", EXTENSION, NORID},
    {"", 0, 0}, {"", 0, 0},
    {"switch", SWITCH, NORID},
    {"__volatile__", TYPE_QUAL, RID_VOLATILE},
    {"__unbounded__", TYPE_QUAL, RID_UNBOUNDED},
    {"", 0, 0},
    {"__inline", SCSPEC, RID_INLINE},
    {"", 0, 0},
    {"__real", REALPART, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"typeof", TYPEOF, NORID},
    {"typedef", SCSPEC, RID_TYPEDEF},
    {"", 0, 0}, {"", 0, 0},
    {"__ptrlow__", PTR_LOW_BOUND, NORID},
    {"__ptrhigh__", PTR_HIGH_BOUND, NORID},
    {"__ptrvalue__", PTR_VALUE, NORID},
    {"", 0, 0}, {"", 0, 0},
    {"else", ELSE, NORID},
    {"@class", CLASS, NORID},
    {"@defs", DEFS, NORID},
    {"in", TYPE_QUAL, RID_IN},
    {"__volatile", TYPE_QUAL, RID_VOLATILE},
    {"inline", SCSPEC, RID_INLINE},
    {"while", WHILE, NORID},
    {"__const", TYPE_QUAL, RID_CONST},
    {"", 0, 0},
    {"__const__", TYPE_QUAL, RID_CONST},
    {"__complex", TYPESPEC, RID_COMPLEX},
    {"__complex__", TYPESPEC, RID_COMPLEX},
    {"", 0, 0},
    {"__ptrhigh", PTR_HIGH_BOUND, NORID},
    {"return", RETURN, NORID},
    {"@protocol", PROTOCOL, NORID},
    {"@private", PRIVATE, NORID},
    {"@selector", SELECTOR, NORID},
    {"", 0, 0},
    {"__bounded__", TYPE_QUAL, RID_BOUNDED},
    {"enum", ENUM, NORID},
    {"__ptrlow", PTR_LOW_BOUND, NORID},
    {"", 0, 0},
    {"__ptrvalue", PTR_VALUE, NORID},
    {"extern", SCSPEC, RID_EXTERN},
    {"", 0, 0},
    {"default", DEFAULT, NORID},
    {"out", TYPE_QUAL, RID_OUT},
    {"volatile", TYPE_QUAL, RID_VOLATILE},
    {"float", TYPESPEC, RID_FLOAT},
    {"", 0, 0},
    {"byref", TYPE_QUAL, RID_BYREF},
    {"id", OBJECTNAME, RID_ID},
    {"", 0, 0},
    {"__signed", TYPESPEC, RID_SIGNED},
    {"__imag", IMAGPART, NORID},
    {"short", TYPESPEC, RID_SHORT},
    {"", 0, 0}, {"", 0, 0},
    {"case", CASE, NORID},
    {"union", UNION, NORID},
    {"inout", TYPE_QUAL, RID_INOUT},
    {"break", BREAK, NORID},
    {"", 0, 0}, {"", 0, 0},
    {"__asm__", ASM_KEYWORD, NORID},
    {"", 0, 0},
    {"__alignof", ALIGNOF, NORID},
    {"", 0, 0},
    {"__alignof__", ALIGNOF, NORID},
    {"", 0, 0},
    {"__attribute__", ATTRIBUTE, NORID},
    {"", 0, 0}, {"", 0, 0},
    {"@encode", ENCODE, NORID},
    {"", 0, 0},
    {"@implementation", IMPLEMENTATION, NORID},
    {"@interface", INTERFACE, NORID},
    {"oneway", TYPE_QUAL, RID_ONEWAY},
    {"", 0, 0}, {"", 0, 0},
    {"register", SCSPEC, RID_REGISTER},
    {"const", TYPE_QUAL, RID_CONST},
    {"__unbounded", TYPE_QUAL, RID_UNBOUNDED},
    {"", 0, 0},
    {"__asm", ASM_KEYWORD, NORID},
    {"", 0, 0},
    {"unsigned", TYPESPEC, RID_UNSIGNED},
    {"void", TYPESPEC, RID_VOID},
    {"@public", PUBLIC, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"@protected", PROTECTED, NORID},
    {"__attribute", ATTRIBUTE, NORID},
    {"", 0, 0}, {"", 0, 0},
    {"@compatibility_alias", ALIAS, NORID},
    {"bycopy", TYPE_QUAL, RID_BYCOPY},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"asm", ASM_KEYWORD, NORID},
    {"double", TYPESPEC, RID_DOUBLE},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"continue", CONTINUE, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"__bounded", TYPE_QUAL, RID_BOUNDED},
    {"long", TYPESPEC, RID_LONG},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"__builtin_va_arg", VA_ARG, NORID},
    {"@end", END, NORID},
    {"", 0, 0},
    {"do", DO, NORID},
    {"goto", GOTO, NORID},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"signed", TYPESPEC, RID_SIGNED},
    {"", 0, 0},
    {"static", SCSPEC, RID_STATIC},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"char", TYPESPEC, RID_CHAR},
    {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0}, {"", 0, 0},
    {"auto", SCSPEC, RID_AUTO}
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
