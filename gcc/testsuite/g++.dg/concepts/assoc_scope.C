#include <concepts>

concept Concept<typename T>
{
  typename assoc;
};

concept RequiresConcept<typename T>
{
  requires Concept<T>;
};

assoc x; // { dg-error "does not name a type" }

template<typename T> requires RequiresConcept<T> void bar(const T&)
{
  typedef assoc honk; // { dg-error "does not name a type" }
}
