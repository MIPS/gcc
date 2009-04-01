// { dg-do link }
// { dg-options "{-flto -shared}" }
template < typename > struct Foo
{
 inline void rdstate() {
 }
};

extern template struct Foo<int>;

struct Bar:virtual public Foo<int>
{
};
