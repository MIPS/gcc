concept C<typename T> {
  typename assoc;
}

concept D<typename T> : C<C<T>::assoc> { } // { dg-error "associated type" }
