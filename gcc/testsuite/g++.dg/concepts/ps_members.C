
concept HasEmpty<typename T>
{
  bool T::empty() const; // { dg-error "unsatisfied requirement" }
};

struct XHasEmpty
{
  bool empty() const;
};

concept_map HasEmpty<XHasEmpty> { };

struct YNoEmpty
{
};
concept_map HasEmpty<YNoEmpty> { }; // { dg-error "HasEmpty" }

concept HasIntData<typename T>
{
  int* T::data(); // { dg-error "unsatisfied requirement" }
}

struct XHasIntData {
  int* data();
};

concept_map HasIntData<XHasIntData> { }

struct YHasFloatData {
  float* data();
};

concept_map HasIntData<YHasFloatData> { }; // { dg-error "cannot convert|HasIntData" }

