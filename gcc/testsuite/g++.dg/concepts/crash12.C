concept<typename T> foo {}; // { dg-error "parameter list" }

concept<typename T> bar<T> {}; // { dg-error "parameter list|expected" }
