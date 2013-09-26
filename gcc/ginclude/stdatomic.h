/* Copyright (C) 2013 Free Software Foundation, Inc.
 *
 * This file is part of GCC.
 *
 * GCC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GCC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Under Section 7 of GPL version 3, you are granted additional
 * permissions described in the GCC Runtime Library Exception, version
 * 3.1, as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License and
 * a copy of the GCC Runtime Library Exception along with this program;
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 * <http://www.gnu.org/licenses/>.  */

/*
 *  * ISO C11 Standard:  7.17  Atomics <stdatomic.h>
 *   */

#ifndef _STDATOMIC_H
#define _STDATOMIC_H


typedef enum memory_order
{
  memory_order_relaxed,
  memory_order_consume,
  memory_order_acquire,
  memory_order_release,
  memory_order_acq_rel,
  memory_order_seq_cst
} memory_order;


typedef _Atomic _Bool 	       atomic_bool;
typedef _Atomic char	       atomic_char
typedef _Atomic schar	       atomic_schar
typedef _Atomic uchar	       atomic_uchar
typedef _Atomic short	       atomic_short
typedef _Atomic ushort	       atomic_ushort
typedef _Atomic int            atomic_int
typedef _Atomic uint           atomic_uint
typedef _Atomic long           atomic_long
typedef _Atomic ulong          atomic_ulong
typedef _Atomic llong          atomic_llong
typedef _Atomic ullong         atomic_ullong
typedef _Atomic char16_t       atomic_char16_t
typedef _Atomic char32_t       atomic_char32_t
typedef _Atomic wchar_t        atomic_wchar_t
typedef _Atomic int_least8_t   atomic_int_least8_t
typedef _Atomic uint_least8_t  atomic_uint_least8_t
typedef _Atomic int_least16_t  atomic_int_least16_t
typedef _Atomic uint_least16_t atomic_uint_least16_t
typedef _Atomic int_least32_t  atomic_int_least32_t
typedef _Atomic uint_least32_t atomic_uint_least32_t
typedef _Atomic int_least64_t  atomic_int_least64_t
typedef _Atomic uint_least64_t atomic_uint_least64_t
typedef _Atomic int_fast8_t    atomic_int_fast8_t
typedef _Atomic uint_fast8_t   atomic_uint_fast8_t
typedef _Atomic int_fast16_t   atomic_int_fast16_t
typedef _Atomic uint_fast16_t  atomic_uint_fast16_t
typedef _Atomic int_fast32_t   atomic_int_fast32_t
typedef _Atomic uint_fast32_t  atomic_uint_fast32_t
typedef _Atomic int_fast64_t   atomic_int_fast64_t
typedef _Atomic uint_fast64_t  atomic_uint_fast64_t
typedef _Atomic intptr_t       atomic_intptr_t
typedef _Atomic uintptr_t      atomic_uintptr_t
typedef _Atomic size_t         atomic_size_t
typedef _Atomic ptrdiff_t      atomic_ptrdiff_t
typedef _Atomic intmax_t       atomic_intmax_t
typedef _Atomic uintmax_t      atomic_uintmax_t        


#define ATOMIC_VAR_INIT(VALUE)	(VALUE)
#define atomic_init(PTR, VAL)	{ *(PTR) = (VAL); }

/* TODO actually kill the dependancy.  */
#define kill_dependency(Y)	(Y)

#define atomic_thread_fence 	__atomic_thread_fence
#define atomic_signal_fence 	__atomic_signal_fence 
#define atomic_is_lock_free(OBJ) __atomic_is_lock_free (sizeof (*(OBJ)), NULL)

#define ATOMIC_BOOL_LOCK_FREE			\
			__atomic_is_lock_free (sizeof (atomic_bool), NULL)
#define ATOMIC_CHAR_LOCK_FREE			\
			__atomic_is_lock_free (sizeof (atomic_char), NULL)
#define ATOMIC_CHAR16_T_LOCK_FREE		\
			__atomic_is_lock_free (sizeof (atomic_char16_t), NULL)
#define ATOMIC_CHAR32_T_LOCK_FREE		\
			__atomic_is_lock_free (sizeof (atomic_char32_t), NULL)
#define ATOMIC_WCHAR_T_LOCK_FREE		\
			__atomic_is_lock_free (sizeof (atomic_wchar_t), NULL)
#define ATOMIC_SHORT_LOCK_FREE 			\
			__atomic_is_lock_free (sizeof (atomic_short), NULL)
#define ATOMIC_INT_LOCK_FREE 			\
			__atomic_is_lock_free (sizeof (atomic_int), NULL)
#define ATOMIC_LONG_LOCK_FREE			\
			__atomic_is_lock_free (sizeof (atomic_long), NULL)
#define ATOMIC_LLONG_LOCK_FREE			\
			__atomic_is_lock_free (sizeof (atomic_llong), NULL)
#define ATOMIC_POINTER_LOCK_FREE		\
			__atomic_is_lock_free (sizeof (_Atomic void *), NULL)


/* TODO: Note this required the __typeof__ definition to drops the atomic
   qualifier, which means __typeof__ (atomic type) return the underlying 
   non-atomic type.
   I think this makes sense, as most meaningful uses of __typeof__ of an atomic
   object would want the non-atomic version to be useful, as it is above.

   If we dont want to change that meaning, we'll need to implement a __typeof__
   variant which does this. 
   
   Also note that the header file uses the generic form of __atomic builtins,
   which requires the address to be taken of the value parameter, and then
   we pass that value on.   This allows the macros to work for any type,
   and the compiler is smart enough to convert these to lock-free _N 
   variants if possible, and throw away the temps.  */

#define atomic_store_explicit(PTR, VAL, MO) ({		\
  __typeof__ (*(PTR)) __tmp  = (VAL); 			\
  __atomic_store ((PTR), &__tmp, (MO)); })

#define atomic_store(PTR, VAL)				\
  atomic_store_explicit (PTR, VAL, __ATOMIC_SEQ_CST)


#define atomic_load_explicit(PTR, MO) ({		\
  __typeof__ (*(PTR)) __tmp; 				\
  __atomic)load ((PTR), &__tmp, __ATOMIC_SEQ_CST);	\
  __tmp; })

#define atomic_load(PTR)  atomic_load_explicit (PTR, __ATOMIC_SEQ_CST)


#define atomic_exchange_explicit(PTR, VAL, MO) ({	\
  __typeof__ (*(PTR)) __tmp  = (VAL); 			\
  __atomic_exchange_n ((PTR), (VAL), (MO)); 		\
  __tmp; })

#define atomic_exchange(PTR, VAL) 			\
  atomic_exchange_explicit(PTR, VAL, __ATOMIC_SEQ_CST)


#define atomic_compare_exchange_strong_explicit(PTR, VAL, DES, SUC, FAIL) ({ \
  __typeof__ (*(PTR)) __tmp  = (DES); 			\
  __atomic_compare_exchange_n ((PTR), (VAL), &__tmp, 0, (SUC), (FAIL)); })

#define atomic_compare_exchange_strong(PTR, VAL, DES) 			   \
  atomic_compare_exchange_strong_explicit(PTR, VAL, DES, __ATOMIC_SEQ_CST, \
				          __ATOMIC_SEQ_CST)

#define atomic_compare_exchange_weak_explicit(PTR, VAL, DES, SUC, FAIL) ({  \
  __typeof__ (*(PTR)) __tmp  = (DES); 			\
  __atomic_compare_exchange_n ((PTR), (VAL), &__tmp, 1, (SUC), (FAIL)); })

#define atomic_compare_exchange_weak(PTR, VAL, DES) 			 \
  atomic_compare_exchange_weak_explicit(PTR, VAL, DES, __ATOMIC_SEQ_CST, \
					__ATOMIC_SEQ_CST)



#define atomic_fetch_add(PTR, VAL) __atomic_fetch_add ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define atomic_fetch_add_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_add ((PTR), (VAL), (MO))

#define atomic_fetch_sub(PTR, VAL) __atomic_fetch_sub ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define atomic_fetch_sub_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_sub ((PTR), (VAL), (MO))

#define atomic_fetch_or(PTR, VAL) __atomic_fetch_or ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define atomic_fetch_or_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_or ((PTR), (VAL), (MO))

#define atomic_fetch_xor(PTR, VAL) __atomic_fetch_xor ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define atomic_fetch_xor_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_xor ((PTR), (VAL), (MO))

#define atomic_fetch_and(PTR, VAL) __atomic_fetch_and ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define atomic_fetch_and_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_and ((PTR), (VAL), (MO))


#if __GCC_ATOMIC_TEST_AND_SET_TRUEVAL == 1
    typedef bool atomic_flag;
#else
    typedef unsigned char atomic_flag;
#endif

#define ATOMIC_FLAG_INIT	0


#define atomic_flag_test_and_set(PTR) 					\
			__atomic_flag_test_and_set ((PTR), __ATOMIC_SEQ_CST)
#define atomic_flag_test_and_set_explicit(PTR, MO)			\
			__atomic_flag_test_and_set ((PTR), (MO))

#define atomic_flag_clear(PTR)	__atomic_flag_clear ((PTR), __ATOMIC_SEQ_CST)
#define atomic_flag_clear_explicit(PTR, MO)   __atomic_flag_clear ((PTR), (MO))



#endif  /* _STDATOMIC_H */
