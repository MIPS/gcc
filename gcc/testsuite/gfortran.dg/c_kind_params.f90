! { dg-do run }
! { dg-additional-sources c_kinds.c }
! { dg-options "-w -std=c99" }
! the -w option is needed to make f951 not report a warning for 
! the -std=c99 option that the C file needs.
module c_kind_params
  use, intrinsic :: iso_c_binding

contains
  subroutine param_test(my_short, my_int, my_long, my_long_long, &
       my_int8_t, my_int_least8_t, my_int_fast8_t, my_int16_t, &
       my_int_least16_t, my_int_fast16_t, my_int32_t, my_int_least32_t, &
       my_int_fast32_t, my_int64_t, my_int_least64_t, my_int_fast64_t, &
       my_intmax_t, my_intptr_t, my_float, my_double, my_long_double, &
       my_char, my_bool) bind(c)
    integer(c_short), value :: my_short
    integer(c_int), value :: my_int
    integer(c_long), value :: my_long
    integer(c_long_long), value :: my_long_long
    integer(c_int8_t), value :: my_int8_t
    integer(c_int_least8_t), value :: my_int_least8_t
    integer(c_int_fast8_t), value :: my_int_fast8_t
    integer(c_int16_t), value :: my_int16_t
    integer(c_int_least16_t), value :: my_int_least16_t
    integer(c_int_fast16_t), value :: my_int_fast16_t
    integer(c_int32_t), value :: my_int32_t
    integer(c_int_least32_t), value :: my_int_least32_t
    integer(c_int_fast32_t), value :: my_int_fast32_t
    integer(c_int64_t), value :: my_int64_t
    integer(c_int_least64_t), value :: my_int_least64_t
    integer(c_int_fast64_t), value :: my_int_fast64_t
    integer(c_intmax_t), value :: my_intmax_t
    integer(c_intptr_t), value :: my_intptr_t
    real(c_float), value :: my_float
    real(c_double), value :: my_double
    real(c_long_double), value :: my_long_double
    character(c_char), value :: my_char
    logical(c_bool), value :: my_bool

    print *, 'my_short is:              ', my_short
    print *, 'my_int is:                ', my_int
    print *, 'my_long is:               ', my_long
    print *, 'my_long_long is:          ', my_long_long
    print *
    print *, 'my_int8_t is:             ', my_int8_t
    print *, 'my_int_least8_t is:       ', my_int_least8_t
    print *, 'my_int_fast8_t is:        ', my_int_fast8_t
    print *
    print *, 'my_int16_t is:            ', my_int16_t
    print *, 'my_int_least16_t is:      ', my_int_least16_t
    print *, 'my_int_fast16_t is:       ', my_int_fast16_t
    print *
    print *, 'my_int32_t is:            ', my_int32_t
    print *, 'my_int_least32_t is:      ', my_int_least32_t
    print *, 'my_int_fast32_t is:       ', my_int_fast32_t
    print *
    print *, 'my_int64_t is:            ', my_int64_t
    print *, 'my_int_least64_t is:      ', my_int_least64_t
    print *, 'my_int_fast64_t is:       ', my_int_fast64_t
    print *
    print *, 'my_intmax_t is:           ', my_intmax_t
    print *, 'my_intptr_t is:           ', my_intptr_t
    print *
    print *, 'my_float is:              ', my_float
    print *, 'my_double is:             ', my_double
    print *, 'my_long_double is:        ', my_long_double
    print *
    print *, 'my_char is:               ', my_char
    print *, 'my_bool is:               ', my_bool

  end subroutine param_test
    
end module c_kind_params
