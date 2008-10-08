#include <concepts>

concept Bli< typename T >
{
  T bli();
};

concept_map Bli<int>
{
  int bli(): // { dg-error "only constructors" }
} // { dg-error "end of input|identifier|before|before" }
