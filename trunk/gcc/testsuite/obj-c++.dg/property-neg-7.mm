/* APPLE LOCAL file radar 4506893 */
/* Cannot write into a read-only property. */
/* { dg-do compile { target *-*-darwin* } } */

@interface NSArray 
@property(readonly) int count;
@end

@implementation NSArray
@end

void foo (NSArray *ans[], id pid, id apid[], int i) {
    NSArray *test;
    test.count = 1; /* { dg-error "readonly property can not be set" } */
    ((NSArray *)pid).count = 1;  /* { dg-error "readonly property can not be set" } */
    ((NSArray *)apid[i]).count = 1; /* { dg-error "readonly property can not be set" } */
    ans[i].count = 3; /* { dg-error "readonly property can not be set" } */
}
