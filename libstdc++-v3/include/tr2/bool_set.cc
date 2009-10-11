
#include <bool_set>

namespace std
{
namespace tr2
{

  //  Static operation arrays.

  ///
  bool_set::_Bool_set_val
  bool_set::_S_not[4] = 
  { _S_true_, _S_false, _S_indet, _S_empty };

  ///
  bool_set::_Bool_set_val
  bool_set::_S_xor[4][4] = 
  { { _S_false, _S_true_, _S_indet, _S_empty },
    { _S_true_, _S_false, _S_indet, _S_empty },
    { _S_indet, _S_indet, _S_indet, _S_empty },
    { _S_empty, _S_empty, _S_empty, _S_empty } };

  ///
  bool_set::_Bool_set_val
  bool_set::_S_or[4][4] = 
  { { _S_false, _S_true_, _S_indet, _S_empty },
    { _S_true_, _S_true_, _S_true_, _S_empty },
    { _S_indet, _S_true_, _S_indet, _S_empty },
    { _S_empty, _S_empty, _S_empty, _S_empty } };

  ///
  bool_set::_Bool_set_val
  bool_set::_S_and[4][4] = 
  { { _S_false, _S_false, _S_false, _S_empty },
    { _S_false, _S_true_, _S_indet, _S_empty },
    { _S_false, _S_indet, _S_indet, _S_empty },
    { _S_empty, _S_empty, _S_empty, _S_empty } };

  ///
  bool_set::_Bool_set_val
  bool_set::_S_eq[4][4] = 
  { { _S_true_, _S_false, _S_indet, _S_empty },
    { _S_false, _S_true_, _S_indet, _S_empty },
    { _S_indet, _S_indet, _S_indet, _S_empty },
    { _S_empty, _S_empty, _S_empty, _S_empty } };

}
}
