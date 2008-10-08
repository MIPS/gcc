// { dg-options "-std=c++0x" }
concept Foo<typename T>
{
};

template<typename T> requires Foo<T> void foo();
