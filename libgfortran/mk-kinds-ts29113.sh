#!/bin/sh
LC_ALL=C
export LC_ALL

compile="$1"

# Obtain the available kinds:

echo "use iso_fortran_env; print *, integer_kinds; print *, real_kinds ; end" > tmq$$.f90
$compile -S -fdump-tree-original tmq$$.f90
INTEGER_KINDS=`grep ' integer_kinds' tmq$$.f90.*original* | sed -e 's/.*{//' -e 's/,//g' -e 's/}.*//'`
REAL_KINDS=`grep ' real_kinds' tmq$$.f90.*original* | sed -e 's/.*{//' -e 's/,//g' -e 's/}.*//'`
rm -f tmq$$.*

# Obtain the ISO-C-Binding kinds
echo ""
echo "/* TS 29113 types.  */"
echo ""

# Integers
cat <<EOF  > tmq$$.f90
subroutine kinds()
use iso_c_binding
integer :: i_c_signed_char, i_c_short, i_c_int, i_c_long, i_c_long_long
integer :: i_c_size_t, i_c_int8_t, i_c_int16_t, i_c_int32_t, i_c_int64_t
integer :: i_c_int_least8_t, i_c_int_least16_t, i_c_int_least32_t
integer :: i_c_int_least64_t, i_c_int_fast8_t, i_c_int_fast16_t
integer :: i_c_int_fast32_t, i_c_int_fast64_t, i_c_intmax_t
integer :: i_c_intptr_t, i_c_ptrdiff_t

i_c_signed_char = c_signed_char
i_c_short = c_short
i_c_int = c_int
i_c_long = c_long
i_c_long_long = c_long_long
i_c_size_t = c_size_t
i_c_int8_t = c_int8_t
i_c_int16_t = c_int16_t
i_c_int32_t = c_int32_t
i_c_int64_t = c_int64_t
i_c_int_least8_t = c_int_least8_t
i_c_int_least16_t = c_int_least16_t
i_c_int_least32_t = c_int_least32_t
i_c_int_least64_t = c_int_least64_t
i_c_int_fast8_t = c_int_fast8_t
i_c_int_fast16_t = c_int_fast16_t
i_c_int_fast32_t = c_int_fast32_t
i_c_int_fast64_t = c_int_fast64_t
i_c_intmax_t = c_intmax_t
i_c_intptr_t = c_intptr_t
i_c_ptrdiff_t = c_ptrdiff_t
EOF

if echo $INTEGER_KINDS |grep -q 16; then
cat <<EOF  >> tmq$$.f90
block
integer :: i_c_int128_t, i_c_int_least128_t, i_c_int_fast128_t
i_c_int128_t = c_int128_t
i_c_int_least128_t = c_int_least128_t
i_c_int_fast128_t = c_int_fast128_t
end block
EOF
fi
echo "end" >> tmq$$.f90

$compile -S -fdump-tree-original tmq$$.f90
grep ' = ' tmq$$.f90.*original* \
   | sed -e 's/ *i_c_/#define CFI_type_/' -e 's/ = / (CFI_type_Integer + (/' \
         -e 's/;/ << CFI_type_kind_shift))/'
rm -f tmq$$.*

# Logical/Bool
cat <<EOF  > tmq$$.f90
subroutine kinds()
use iso_c_binding
integer :: i_c_bool
i_c_bool = c_bool
end
EOF

$compile -S -fdump-tree-original tmq$$.f90
grep ' = ' tmq$$.f90.*original* \
   | sed -e 's/bool/Bool/' -e 's/ *i_c_/#define CFI_type_/' \
         -e 's/ = / (CFI_type_Logical + (/' \
         -e 's/;/ << CFI_type_kind_shift))/'
rm -f tmq$$.*

# Real
cat <<EOF  > tmq$$.f90
subroutine kinds()
use iso_c_binding
integer :: i_c_float, i_c_double, i_c_long_double

i_c_float = c_float
i_c_double = c_double
i_c_long_double = c_long_double
EOF

if echo $REAL_KINDS |grep -q 16; then
cat <<EOF  >> tmq$$.f90
block
integer :: i_C_FLOAT128
i_C_FLOAT128 = C_FLOAT128
end block
EOF
fi

echo "end" >> tmq$$.f90
$compile -S -fdump-tree-original tmq$$.f90
grep ' = ' tmq$$.f90.*original* \
   | sed -e 's/ *i_c_/#define CFI_type_/' -e 's/ = / (CFI_type_Real + (/' \
         -e 's/;/ << CFI_type_kind_shift))/'
rm -f tmq$$.*

# Complex
cat <<EOF  > tmq$$.f90
subroutine kinds()
use iso_c_binding
integer :: i_c_float_complex, i_c_double_complex, i_c_long_double_complex

i_c_float_complex = c_float_complex
i_c_double_complex = c_double_complex
i_c_long_double_complex = c_long_double_complex
EOF

if echo $REAL_KINDS |grep -q 16; then
cat <<EOF  >> tmq$$.f90
block
integer :: i_C_FLOAT128_COMPLEX
i_C_FLOAT128_COMPLEX = C_FLOAT128_COMPLEX
end block
EOF
fi

echo "end" >> tmq$$.f90
$compile -S -fdump-tree-original tmq$$.f90
grep ' = ' tmq$$.f90.*original* \
   | sed -e 's/complex/Complex/' \
         -e 's/ *i_c_/#define CFI_type_/' -e 's/ = / (CFI_type_Complex + (/' \
         -e 's/;/ << CFI_type_kind_shift))/'
rm -f tmq$$.*


echo ""
echo "/* gfortran intrinsic non-character types.  */"
echo ""
for I in $INTEGER_KINDS; do
  echo "#define CFI_type_Integer$I (CFI_type_Integer + ($I << CFI_type_kind_shift))"
done

for I in $INTEGER_KINDS; do
  echo "#define CFI_type_Logical$I (CFI_type_Logical + ($I << CFI_type_kind_shift))"
done

for I in $REAL_KINDS; do
  echo "#define CFI_type_Real$I (CFI_type_Real + ($I << CFI_type_kind_shift))"
done

for I in $REAL_KINDS; do
  echo "#define CFI_type_Complex$I (CFI_type_Complex + ($I << CFI_type_kind_shift))"
done

cat <<EOF

#ifdef __cplusplus
}
#endif

#endif  /* ISO_FORTRAN_BINDING_H */
EOF
