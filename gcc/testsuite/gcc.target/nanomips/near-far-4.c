/* { dg-do compile } */
/* { dg-require-effective-target nonpic } */
/* { dg-options "-mcmodel=auto -mno-long-calls addressing=absolute" } */

/* This test fails only for the large model. Ideally, we would run this test
   for both the automatic and medium model, but we'll have to stick to forcing
   the automatic model for now. The former can be achieved by modifying
   nanomips.exp.  */

extern int long_call_func () __attribute__((long_call));
extern int far_func () __attribute__((far));
extern int near_func () __attribute__((near));
extern int normal_func ();

int test1 () { return long_call_func (); }
int test2 () { return far_func (); }
int test3 () { return near_func (); }
int test4 () { return normal_func (); }

/* { dg-final { scan-assembler-not "\t((j(|al|als))|b(|al)c)\tlong_call_func\n" } } */
/* { dg-final { scan-assembler-not "\t((j(|al|als))|b(|al)c)\tfar_func\n" } } */
/* { dg-final { scan-assembler     "\t((j(|al|als))|b(|al)c)\tnear_func\n" } } */
/* { dg-final { scan-assembler     "\t((j(|al|als))|b(|al)c)\tnormal_func\n" } } */
