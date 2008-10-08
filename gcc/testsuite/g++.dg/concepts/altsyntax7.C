concept Operator<typename Op, typename T> { }

concept Iterator<typename Iter> { typename value_type; }

template<Iterator Iter, Operator<Iter::value_type> Op> void foo();
