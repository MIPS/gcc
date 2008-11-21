// { dg-do link }
// { dg-options "{-flto-single -shared}" }
template < typename > struct Foo
{
 inline void rdstate() {
 }
};

extern template struct Foo<int>;

struct Bar:virtual public Foo<int>
{
};
