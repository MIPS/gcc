#include "core_concepts.h"

concept InputIterator<typename Iter>
{
  typename value_type;
  typename reference;
};

concept ForwardIterator<typename Iter> : InputIterator<Iter>
{
  requires std::SameType<reference, value_type&>;
};
