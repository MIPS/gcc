concept Foo<typename T> {};
concept Bar<typename T> {};

auto concept C<typename T>
 requires Foo<T> 
          Bar<T> // { dg-error "expected" }
{
};
