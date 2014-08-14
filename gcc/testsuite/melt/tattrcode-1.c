/* file tattrcode-1.c used by tattr-1.melt, in the public domain */
void emptyfun(void) {
}

int funwithattr(int x, int y) __attribute__((testmelt(1)));

int funwithattr(int x, int y)  {
  return x+y;
}
