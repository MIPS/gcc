// file tusedef-code1.c
// see file tusedef-ana1.melt 
#include <string.h>
#include <stdio.h>

#define MAX_HASH 0x3fffffff

unsigned
hash_cstring (const char *s, int l)
{
  int oldl = l;
  unsigned h1 = 17, h2 = 0, h = 0;
  if (l < 0 && s)
    l = strlen (s);
  if (!s || l <= 0)
    return 11;
  while (l > 8)
    {
      h1 = (13 * h2 + l) ^ (s[0] * 17 + s[1] * 31);
      h2 = (19 * h2) ^ (s[2] * 1021 + s[3] * 3037);
      h1 = (421 * h1) ^ (s[4] * 439 - s[5] * 337);
      h2 = (19 * h2) ^ (s[6] * 617 + s[7] * 919);
      l -= 8;
      s += 8;
    }
  if (l <= 0)
    goto end;
  l--;
  h1 = 17 * h1 ^ (s[0] * 457 + 3);
  if (l <= 0)
    goto end;
  l--;
  h2 = 31 * h2 ^ (s[1] * 941 + 11);
  if (l <= 0)
    goto end;
  l--;
  h1 = 139 * h1 ^ (s[2] * 107 + 13);
  if (l <= 0)
    goto end;
  l--;
  h2 = 173 * h2 ^ (s[3] * 239 + 19);
  if (l <= 0)
    goto end;
  l--;
  h1 = 13 * h1 ^ (s[4] * 631 + 31);
  if (l <= 0)
    goto end;
  l--;
  h2 = 541 * h2 ^ (s[5] * 683 + 37);
  if (l <= 0)
    goto end;
  l--;
  h1 = 769 * h1 ^ (s[6] * 857 + 43);
  if (l <= 0)
    goto end;
  l--;
  h2 = 941 * h2 ^ (s[7] * 1021 + 53);
  if (l <= 0)
    goto end;
  l--;
end:
  h = ((h1 * 1907) + (h2 * 1459)) & MAX_HASH;
  if (h == 0)
    return (oldl + 3) & MAX_HASH;
  return h;
}


int main(int argc, char**argv)
{
  int i;
  if (argc>1) {
    for (i=1; i<argc; i++) 
      printf("argv[%d]=%s hash=%ux\n", i, argv[i],
	     hash_cstring(argv[i], -1));
  }
  else {
    static const char sample1[]="To be or not to be, that is the question.";
    static const char sample2[]="To not be or to be, that is not the question!";
    printf("argv0 %s: hash=%ux\n", argv[0],
	   hash_cstring(argv[0], -1));
    printf("sample1 %s: hash=%ux\n", sample1,
	   hash_cstring(sample1, sizeof(sample1)-1));
    printf("sample2 %s: hash=%ux\n", sample2,
	   hash_cstring(sample2, strlen(sample2)));
  }
  return 0;
}

