// { dg-do compile }

concept CopyConstructible<typename T>
{
  typename assoc_type;
};

concept_map CopyConstructible<float>
{
  typedef float assoc_type;
};

concept_map CopyConstructible<int> 
{
  typedef float assoc_type;
};

template<typename T> concept_map CopyConstructible<const T*> { typedef T assoc_type; };
