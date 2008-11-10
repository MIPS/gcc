/* { dg-do compile } */
/* { dg-options "{-w -flto-single}" }  */
void Foo(void) { char bar[1]; free(bar); }
