/* APPLE LOCAL file Objective-C */
/* { dg-do compile } */
@interface List
{
@public
  int firstLink;
}
@end

@interface ListIndex_linked
{
@public
  List *collection;
  int link;
}
@end

@interface List_linked: List
@end

@implementation List
@end

@implementation ListIndex_linked
- next
{
   /* The gimplifier should be able to eliminate the cast in the rhs below.  */
   link = ((List_linked*)collection)->firstLink;
}
@end
