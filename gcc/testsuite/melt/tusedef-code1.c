// file tusedef1.c
#include <string.h>

#define MAX_HASH 0x3fffffff

unsigned
hash_cstring (const char *s, int l)
{
  if (l < 0 && s)
    l = strlen (s);
  if (!s || l <= 0)
    return 11;
  int oldl = l;
  unsigned h1 = 17, h2 = 0;
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
  unsigned h = ((h1 * 1907) + (h2 * 1459)) & MAX_HASH;
  if (h == 0)
    return (oldl + 3) & MAX_HASH;
  return h;
}
