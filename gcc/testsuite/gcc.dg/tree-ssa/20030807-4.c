/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-dom2" } */

typedef struct cpp_reader cpp_reader;
typedef unsigned int cppchar_t;
struct cpp_reader
{
  int char_precision;
};
cppchar_t
cpp_parse_escape (pfile, pstr, limit, wide)
     cpp_reader *pfile;
     const unsigned char **pstr;
     const unsigned char *limit;
     int wide;
{
  unsigned int width = ((pfile)->char_precision);
  cppchar_t mask = ((cppchar_t) 1 << width) - 1;
  {
    cppchar_t i = 0, overflow = 0;
    while (*pstr < limit)
      {
	overflow |= i ^ (i << 4 >> 4);
	i = arf ();
      }
    if (overflow | (i != (i & mask)))
      foo ();
  }
}

/* There should be two IF conditionals.  */
/* { dg-final { scan-tree-dump-times "if " 2 "dom2"} } */
