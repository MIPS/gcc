/* APPLE LOCAL file radar 3803157 */
/* Test implementaion of attributes on methods. */
/* { dg-do compile } */

@interface INTF
- (int) foo1: (int)arg1 __attribute__((deprecated));	/* { dg-warning "and previous declaration of" } */

- (int) foo: (int)arg1;	/* { dg-warning "and previous declaration of" } */

- (int) foo2: (int)arg1 __attribute__((deprecated)) __attribute__((unavailable));/* { dg-warning "and previous declaration of" } */
@end

@implementation INTF
- (int) foo: (int)arg1  __attribute__((deprecated)){	/* { dg-warning "conflicting attributes" } */
	return 10;
}
- (int) foo1: (int)arg1 {	/* { dg-warning "conflicting attributes" } */
	return 10;
}
- (int) foo2: (int)arg1 __attribute__((deprecated)) {/* { dg-warning "conflicting attributes" } */
	return 10;
}
@end

int main()
{
	INTF *p;
	[p foo1:2];	/* { dg-warning "\\'foo1:\\' is deprecated" } */
	[p foo2:2];	/* { dg-warning "\\'foo2:\\' is deprecated" } */
			/* { dg-warning "\\'foo2:\\' is unavailable" "" { target *-*-* } 29 } */
	return [p foo:1];
}

