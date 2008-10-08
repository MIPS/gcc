concept H<typename H, typename Input> { // { dg-error "using template type" }
  typename return_type; 
  return_type operator()(const T&, Input&);
};
