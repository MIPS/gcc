class foo {
  char z [10];
public:
  char *get_z () { return & this->z[0]; }
};

foo x;

x.get_z()[9] = 'a';
