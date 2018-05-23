/* { dg-do compile } */
/* { dg-options "-march=nms1 -mlra-equiv -fdump-rtl-reload" } */

extern void sink(void*);

void test(void) {
	char var[1024*128];
	sink(&var[0]);
}

