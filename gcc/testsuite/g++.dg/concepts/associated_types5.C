namespace std {
  auto concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept InputIterator<typename T>
{
  typename value_type;
};

concept ForwardIterator<typename T> : InputIterator<T>
{
};

auto concept BinaryPredicate<typename F, typename T1, typename T2>
{
  bool operator()(F&, T1, T2);
};

template<typename Iter1, typename Iter2, typename F>
  requires InputIterator<Iter1>, ForwardIterator<Iter2>,
          BinaryPredicate<F, 
                          InputIterator<Iter1>::value_type,
                          ForwardIterator<Iter2>::value_type>
void find_first_of(Iter1, Iter2, F);

struct class1 {};
struct class2 {};

struct iter1 {};
concept_map InputIterator<iter1> { typedef class1 value_type; };

struct iter2 {};
concept_map ForwardIterator<iter2> { typedef class2 value_type; };

bool comp(class1, class2);

void f()
{
  find_first_of(iter1(), iter2(), &comp);
}
