concept Moveable<typename T> {
  T::T(T&&);
  T& T::operator=(T&&);
}

struct move_only {
  move_only (move_only&&);
  move_only& operator=(move_only&&);

private:
  move_only(const move_only&);
  move_only& operator=(const move_only&);
};

concept_map Moveable<move_only> {};
