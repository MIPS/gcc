// file tfindgimple-1.c
/* run in build dir to check the findgimple mode
 ln -sv $GCCMELT_SOURCE/gcc/testsuite/melt/tfindgimple-1.c
 ./cc1 -O @meltbuild-common.args -fmelt-init=@melt-default-modules.quicklybuilt \
    -fmelt-mode=findgimple -fmelt-arg="?_" \
    -fmelt-debug -I include tfindgimple-1.c \
    -o /dev/null
*/
#include <stdio.h>

int sum(int x, int y)
{
  return x+y;
}

int main (int argc, char** argv)
{
  int xx = (argc>1)?atoi(argv[1]):2;
  int yy = (argc>2)?atoi(argv[2]):3;
  printf ("xx=%d y=%d sum=%d\n", xx, yy, sum(xx,yy));
  printf ("2*xx=%d sum(2*xx,1)=%d\n", 2*xx, sum(2*xx,1));
  printf("sum(3,4)=%d\n", sum(3,4));
  return 0;
}

// eof tfindgimple-1.c
