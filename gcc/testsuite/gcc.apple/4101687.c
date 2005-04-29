/* APPLE LOCAL file 4101687 */
/* { dg-do run } */
/* { dg-options "-Os" } */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *
MD5End(void *ctx, char *buf)
{
    int i;
    unsigned char digest[16];
    static const char hex[]="0123456789abcdef";

    if (!buf)
        buf = malloc(2*16 + 1);
    if (!buf)
        return 0;
    memset(digest, 0x31, 16);
    for (i = 0; i < 16; i++) {
        buf[i+i] = hex[digest[i] >> 4];
        buf[i+i+1] = hex[digest[i] & 0x0f];
    }
    buf[i+i] = '\0';
    return buf;
}
int main(int argc, char *argv[])
{
 char buf[33];

 if (!MD5End(((void *)0), buf)) {
   fprintf((&__sF[2]), "error calculating md5\n");
   return 1;
 }
  if (buf[16] == '\0')
    abort ();
 return 0;
}
