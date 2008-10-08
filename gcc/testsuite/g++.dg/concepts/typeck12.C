auto concept Destructible<typename T>
{
  T::~T();
};

auto concept DefaultConstructible<typename T> : Destructible<T>
{
  T::T();
};

concept CopyConstructible<typename X> : Destructible<X>
{
  X::X(const X&);
};

concept RefinesDefaultConstructible<typename X>
: DefaultConstructible<X>, CopyConstructible<X>
{
};

concept Nothing<typename> { };

template<typename T> requires Nothing<T>
void def()
{
  T t; // { dg-error "no matching|candidates are" }
}

template<typename T> requires DefaultConstructible<T>
void def()
{
  T t;
}

template<typename U, typename T> requires RefinesDefaultConstructible<T>
void def()
{
  T t;
}
