
#define __LIBSTD_CRT_BUILD

#include <libgen.h>

/*
       path           basename
       "/usr/lib"     "lib"
       "/usr/"        "usr"
       "usr"          "usr"
       "/"            "/"
       "."            "."
       ".."           ".."
*/

LIBSTD_LPROTO_IMPL(char *, basename, char *path)
{
    char *c;
    char *_s = path;

    for ( ; *_s != '\0'; ++_s)
        /* EMPTY */;

    while (1==1) {
        while ((--_s != path) && (*_s != '/') )
            /* EMPTY */;

        if (*_s == '/') {
            if (_s[1] != 0)
                return _s+1;
            if (_s == path)
                return _s;
            *_s = 0;
        } else
            return path;
    }
}
