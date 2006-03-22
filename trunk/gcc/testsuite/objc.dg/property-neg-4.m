/* APPLE LOCAL file radar 4436866 */
/* Property cannot be accessed in class method. */
/* { dg-do compile { target *-*-darwin* } } */

@interface Person 
{
}
@property char *fullName;
+ (void) testClass;
@end	

@implementation  Person
@property char *fullName;
+ (void) testClass {
	fullName = "MyName"; /* { dg-error "property \\'fullName\\' accessed in class method" } */
}
@end

