class C {
 public:
  virtual ~C();
  virtual void foo();
};
class D  {
  ~D();
  C lexer_;
};
D::~D() {
}
