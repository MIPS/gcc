typedef void (*pfv)();
void f ();
extern "C"
{
  typedef void (*pcfv)(void);
  void cf (void);
}

pfv p = f;
pfv p2 = cf;			// ERROR - mismatch XFAIL *-*-*
pcfv p3 = f;			// ERROR - mismatch XFAIL *-*-*
pcfv p4 = cf;
