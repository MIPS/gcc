
#define __LIBSTD_CRT_BUILD

#include <libgen.h>

/*
       path           dirname
       "/usr/lib"     "/usr"
       "/usr/"        "/"
       "usr"          "."
       "/"            "/"
       "."            "."
       ".."           "."
*/

LIBSTD_LPROTO_IMPL(char *, dirname, char *path)
{
    char *_s = path;

    for ( ; *_s != '\0'; ++_s)
        /* EMPTY */;

    while (1==1) {
        while ((--_s != path) && (*_s != '/') )
            /* EMPTY */;

        if (_s[0] != '/')
            return ".";
        
        if (_s == path) {
            _s[1] = '\0';
            return path;
        }
        else if (_s[1] != 0) {
            _s[0] = '\0';
            return path;
        }

        _s[0] = '\0';
    }
}
