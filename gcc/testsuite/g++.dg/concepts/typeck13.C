concept CopyConstructible<typename X>
{
  X::X(const X&);
};

concept InitWithInt<typename T> : CopyConstructible<T>
{
  T::T(int);
};

template<typename T>
requires InitWithInt<T>
void foo()
{
  T t = 0;
}
