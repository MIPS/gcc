// PRMS Id: 4346
// Bug: g++ dies on redefinition of cc_Array::repInvariant.
// Build don't link:

class ccObjectInfo
{
public:
    virtual const ccObjectInfo& repInvariant (int =0) const;
};

template <class T>
class cc_Array : public ccObjectInfo
{
public:
  virtual const ccObjectInfo& repInvariant (int =0) const ;
};

template <class T>
const ccObjectInfo& cc_Array<T>::repInvariant(int) const // ERROR - redefined
{  return *this /* *this is required here */; } 

template <class T>
class ccArray :public ccObjectInfo
{
  ccArray (cc_Array<T>*);
};

template <class T>
class ccObjArray : public ccArray<T>
{
  ccObjArray();
}; 

template <class T>
const ccObjectInfo& cc_Array<T>::repInvariant(int) const // ERROR - causes compiler segfault
{  return 0; }			

typedef ccObjArray< double>	ccROIRuns;	 
