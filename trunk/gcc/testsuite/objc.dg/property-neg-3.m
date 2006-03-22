/* APPLE LOCAL file radar 4436866 */
/* Property name cannot match the ivar name. */
/* { dg-do compile { target *-*-darwin* } } */

@interface Person 
{
  char *firstName;
}
@property char *firstName; /* { dg-error "property name \\'firstName\\' matches an ivar name in this class" } */
@end	

@implementation  Person
@end /* { dg-warning "incomplete implementation of class \\'Person\\'" "" { target *-*-* } 13 } */
     /* { dg-warning "method definition for \\'-setFirstName:\\' not found" "" { target *-*-* } 13 } */
     /* { dg-warning "method definition for \\'-firstName\\' not found" "" { target *-*-* } 13 } */

