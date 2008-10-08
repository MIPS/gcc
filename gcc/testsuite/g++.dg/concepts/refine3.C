concept Incrementable<typename T> { }

concept InputIterator<typename Iter>
  : Incrementable<int> // { dg-error "parameter" }
{
}
