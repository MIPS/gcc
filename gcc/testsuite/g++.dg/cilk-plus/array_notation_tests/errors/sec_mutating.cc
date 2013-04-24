/* { dg-do compile } */
/* { dg-options "-fcilkplus" } */

int A[256];
int fn (int a, int b);

int main () {
      int a;
        a = __sec_reduce_mutating (20, A[:], fn); /* { dg-error "void value not ignored as it ought" } */
	return a;
}

