/* One cannot redeclare __mf_lc_mask in proper C from instrumented
   code, because of the way the instrumentation code emits its decls.  */
extern unsigned foo __asm__ ("__mf_lc_mask");
unsigned *bar = &foo;
*bar = 4;
