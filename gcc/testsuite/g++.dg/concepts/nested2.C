concept TargetConcept<typename T>
{
};

concept NestedSup<typename T>
{
  requires TargetConcept<T>;
};

concept RefinedSup<typename T> : NestedSup<T>
{
};

concept SourceConcept<typename T>
{
  requires RefinedSup<T>;
};

template<typename X> requires TargetConcept<X> void foo(const X&) {}

template<typename X> requires SourceConcept<X> void bar(const X& x)
{
  foo(x);
}
