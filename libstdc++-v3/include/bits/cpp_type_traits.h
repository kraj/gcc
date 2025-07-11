// The  -*- C++ -*- type traits classes for internal use in libstdc++

// Copyright (C) 2000-2025 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/cpp_type_traits.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ext/type_traits.h}
 */

// Written by Gabriel Dos Reis <dosreis@cmla.ens-cachan.fr>

#ifndef _CPP_TYPE_TRAITS_H
#define _CPP_TYPE_TRAITS_H 1

#ifdef _GLIBCXX_SYSHDR
#pragma GCC system_header
#endif

#include <bits/c++config.h>
#include <bits/version.h>
#if __glibcxx_type_trait_variable_templates
# include <type_traits> // is_same_v, is_integral_v
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"

//
// This file provides some compile-time information about various types.
// These representations were designed, on purpose, to be constant-expressions
// and not types as found in <bits/type_traits.h>.  In particular, they
// can be used in control structures and the optimizer hopefully will do
// the obvious thing.
//
// Why integral expressions, and not functions nor types?
// Firstly, these compile-time entities are used as template-arguments
// so function return values won't work:  We need compile-time entities.
// We're left with types and constant  integral expressions.
// Secondly, from the point of view of ease of use, type-based compile-time
// information is -not- *that* convenient.  One has to write lots of
// overloaded functions and to hope that the compiler will select the right
// one. As a net effect, the overall structure isn't very clear at first
// glance.
// Thirdly, partial ordering and overload resolution (of function templates)
// is highly costly in terms of compiler-resource.  It is a Good Thing to
// keep these resource consumption as least as possible.
//
// See valarray_array.h for a case use.
//
// -- Gaby (dosreis@cmla.ens-cachan.fr) 2000-03-06.
//
// Update 2005: types are also provided and <bits/type_traits.h> has been
// removed.
//

extern "C++" {

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  struct __true_type { };
  struct __false_type { };

  template<bool>
    struct __truth_type
    { typedef __false_type __type; };

  template<>
    struct __truth_type<true>
    { typedef __true_type __type; };

  // N.B. The conversions to bool are needed due to the issue
  // explained in c++/19404.
  template<class _Sp, class _Tp>
    struct __traitor
    {
      enum { __value = bool(_Sp::__value) || bool(_Tp::__value) };
      typedef typename __truth_type<__value>::__type __type;
    };

  // Compare for equality of types.
  template<typename, typename>
    struct __are_same
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<typename _Tp>
    struct __are_same<_Tp, _Tp>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  //
  // Integer types
  //
  template<typename _Tp>
    struct __is_integer
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  // Explicit specializations for the standard integer types.
  // Up to four target-specific __int<N> types are supported as well.
  template<>
    struct __is_integer<bool>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<signed char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

# ifdef __WCHAR_TYPE__
  template<>
    struct __is_integer<wchar_t>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
# endif

#ifdef _GLIBCXX_USE_CHAR8_T
  template<>
    struct __is_integer<char8_t>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

#if __cplusplus >= 201103L
  template<>
    struct __is_integer<char16_t>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<char32_t>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

  template<>
    struct __is_integer<short>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned short>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<int>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned int>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<long>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned long>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<long long>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned long long>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

#define __INT_N(TYPE) 			\
  __extension__				\
  template<>				\
    struct __is_integer<TYPE>		\
    {					\
      enum { __value = 1 };		\
      typedef __true_type __type;	\
    };					\
  __extension__				\
  template<>				\
    struct __is_integer<unsigned TYPE>	\
    {					\
      enum { __value = 1 };		\
      typedef __true_type __type;	\
    };

#ifdef __GLIBCXX_TYPE_INT_N_0
__INT_N(__GLIBCXX_TYPE_INT_N_0)
#endif
#ifdef __GLIBCXX_TYPE_INT_N_1
__INT_N(__GLIBCXX_TYPE_INT_N_1)
#endif
#ifdef __GLIBCXX_TYPE_INT_N_2
__INT_N(__GLIBCXX_TYPE_INT_N_2)
#endif
#ifdef __GLIBCXX_TYPE_INT_N_3
__INT_N(__GLIBCXX_TYPE_INT_N_3)
#endif

#if defined __STRICT_ANSI__ && defined __SIZEOF_INT128__
// In strict modes __GLIBCXX_TYPE_INT_N_0 is not defined for __int128,
// but we want to always treat signed/unsigned __int128 as integral types.
__INT_N(__int128)
#endif

#undef __INT_N

  //
  // Floating point types
  //
  template<typename _Tp>
    struct __is_floating
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  // three specializations (float, double and 'long double')
  template<>
    struct __is_floating<float>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_floating<double>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_floating<long double>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

#ifdef _GLIBCXX_USE_FLOAT128
  template<>
    struct __is_floating<__float128>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

#ifdef __STDCPP_FLOAT16_T__
  template<>
    struct __is_floating<_Float16>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

#ifdef __STDCPP_FLOAT32_T__
  template<>
    struct __is_floating<_Float32>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

#ifdef __STDCPP_FLOAT64_T__
  template<>
    struct __is_floating<_Float64>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

#ifdef __STDCPP_FLOAT128_T__
  template<>
    struct __is_floating<_Float128>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

#ifdef __STDCPP_BFLOAT16_T__
  template<>
    struct __is_floating<__gnu_cxx::__bfloat16_t>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

  //
  // An arithmetic type is an integer type or a floating point type
  //
  template<typename _Tp>
    struct __is_arithmetic
    : public __traitor<__is_integer<_Tp>, __is_floating<_Tp> >
    { };

  //
  // For use in std::copy and std::find overloads for streambuf iterators.
  //
  template<typename _Tp>
    struct __is_char
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<>
    struct __is_char<char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

#ifdef __WCHAR_TYPE__
  template<>
    struct __is_char<wchar_t>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

  template<typename _Tp>
    struct __is_byte
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<>
    struct __is_byte<char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_byte<signed char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_byte<unsigned char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

#ifdef __glibcxx_byte // C++ >= 17
  enum class byte : unsigned char;

  template<>
    struct __is_byte<byte>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif // C++17

#ifdef _GLIBCXX_USE_CHAR8_T
  template<>
    struct __is_byte<char8_t>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
#endif

  // A type that is safe for use with memcpy, memmove, memcmp etc.
  template<typename _Tp>
    struct __is_nonvolatile_trivially_copyable
    {
      enum { __value = __is_trivially_copyable(_Tp) };
    };

  // Cannot use memcpy/memmove/memcmp on volatile types even if they are
  // trivially copyable, so ensure __memcpyable<volatile int*, volatile int*>
  // and similar will be false.
  template<typename _Tp>
    struct __is_nonvolatile_trivially_copyable<volatile _Tp>
    {
      enum { __value = 0 };
    };

  // Whether two iterator types can be used with memcpy/memmove.
  template<typename _OutputIter, typename _InputIter>
    struct __memcpyable
    {
      enum { __value = 0 };
    };

  // Allow memcpy when source and destination are pointers to the same type.
  template<typename _Tp>
    struct __memcpyable<_Tp*, _Tp*>
    : __is_nonvolatile_trivially_copyable<_Tp>
    { };

  // Source pointer can be const.
  template<typename _Tp>
    struct __memcpyable<_Tp*, const _Tp*>
    : __is_nonvolatile_trivially_copyable<_Tp>
    { };

  template<typename _Tp> struct __memcpyable_integer;

  // For heterogeneous types, allow memcpy between equal-sized integers.
  // N.B. we cannot do the same for equal-sized enums, they're not assignable.
  // We cannot do it for pointers, because derived-to-base can adjust offset.
  template<typename _Tp, typename _Up>
    struct __memcpyable<_Tp*, _Up*>
    {
      enum {
	__value = __memcpyable_integer<_Tp>::__width != 0
		    && ((int)__memcpyable_integer<_Tp>::__width
			  == (int)__memcpyable_integer<_Up>::__width)
      };
    };

  // Specialization for const U* because __is_integer<const U> is never true.
  template<typename _Tp, typename _Up>
    struct __memcpyable<_Tp*, const _Up*>
    : __memcpyable<_Tp*, _Up*>
    { };

  template<typename _Tp>
    struct __memcpyable_integer
    {
      enum {
	__width = __is_integer<_Tp>::__value ? (sizeof(_Tp) * __CHAR_BIT__) : 0
      };
    };

  // Cannot memcpy volatile memory.
  template<typename _Tp>
    struct __memcpyable_integer<volatile _Tp>
    { enum { __width = 0 }; };

  // Specializations for __intNN types with padding bits.
#if defined __GLIBCXX_TYPE_INT_N_0 && __GLIBCXX_BITSIZE_INT_N_0 % __CHAR_BIT__
  __extension__
  template<>
    struct __memcpyable_integer<__GLIBCXX_TYPE_INT_N_0>
    { enum { __width = __GLIBCXX_BITSIZE_INT_N_0 }; };
  __extension__
  template<>
    struct __memcpyable_integer<unsigned __GLIBCXX_TYPE_INT_N_0>
    { enum { __width = __GLIBCXX_BITSIZE_INT_N_0 }; };
#endif
#if defined __GLIBCXX_TYPE_INT_N_1 && __GLIBCXX_BITSIZE_INT_N_1 % __CHAR_BIT__
  __extension__
  template<>
    struct __memcpyable_integer<__GLIBCXX_TYPE_INT_N_1>
    { enum { __width = __GLIBCXX_BITSIZE_INT_N_1 }; };
  __extension__
  template<>
    struct __memcpyable_integer<unsigned __GLIBCXX_TYPE_INT_N_1>
    { enum { __width = __GLIBCXX_BITSIZE_INT_N_1 }; };
#endif
#if defined __GLIBCXX_TYPE_INT_N_2 && __GLIBCXX_BITSIZE_INT_N_2 % __CHAR_BIT__
  __extension__
  template<>
    struct __memcpyable_integer<__GLIBCXX_TYPE_INT_N_2>
    { enum { __width = __GLIBCXX_BITSIZE_INT_N_2 }; };
  __extension__
  template<>
    struct __memcpyable_integer<unsigned __GLIBCXX_TYPE_INT_N_2>
    { enum { __width = __GLIBCXX_BITSIZE_INT_N_2 }; };
#endif
#if defined __GLIBCXX_TYPE_INT_N_3 && __GLIBCXX_BITSIZE_INT_N_3 % __CHAR_BIT__
  __extension__
  template<>
    struct __memcpyable_integer<__GLIBCXX_TYPE_INT_N_3>
    { enum { __width = __GLIBCXX_BITSIZE_INT_N_3 }; };
  __extension__
  template<>
    struct __memcpyable_integer<unsigned __GLIBCXX_TYPE_INT_N_3>
    { enum { __width = __GLIBCXX_BITSIZE_INT_N_3 }; };
#endif

#if _GLIBCXX_DOUBLE_IS_IEEE_BINARY64 && _GLIBCXX_LDOUBLE_IS_IEEE_BINARY64
  template<>
    struct __memcpyable<double*, long double*> { enum { __value = true }; };
  template<>
    struct __memcpyable<long double*, double*> { enum { __value = true }; };
#endif

#if defined(__STDCPP_FLOAT32_T__) && defined(_GLIBCXX_FLOAT_IS_IEEE_BINARY32)
  template<>
    struct __memcpyable<_Float32*, float*> { enum { __value = true }; };
  template<>
    struct __memcpyable<float*, _Float32*> { enum { __value = true }; };
#endif

#if defined(__STDCPP_FLOAT64_T__) && defined(_GLIBCXX_DOUBLE_IS_IEEE_BINARY64)
  template<>
    struct __memcpyable<_Float64*, double*> { enum { __value = true }; };
  template<>
    struct __memcpyable<double*, _Float64*> { enum { __value = true }; };
#endif

#if defined(__STDCPP_FLOAT128_T__) && defined(_GLIBCXX_LDOUBLE_IS_IEEE_BINARY128)
  template<>
    struct __memcpyable<_Float128*, long double*> { enum { __value = true }; };
  template<>
    struct __memcpyable<long double*, _Float128*> { enum { __value = true }; };
#endif

  // Whether two iterator types can be used with memcmp.
  // This trait only says it's well-formed to use memcmp, not that it
  // gives the right answer for a given algorithm. So for example, std::equal
  // needs to add additional checks that the types are integers or pointers,
  // because other trivially copyable types can overload operator==.
  template<typename _Iter1, typename _Iter2>
    struct __memcmpable
    {
      enum { __value = 0 };
    };

  // OK to use memcmp with pointers to trivially copyable types.
  template<typename _Tp>
    struct __memcmpable<_Tp*, _Tp*>
    : __is_nonvolatile_trivially_copyable<_Tp>
    { };

  template<typename _Tp>
    struct __memcmpable<const _Tp*, _Tp*>
    : __is_nonvolatile_trivially_copyable<_Tp>
    { };

  template<typename _Tp>
    struct __memcmpable<_Tp*, const _Tp*>
    : __is_nonvolatile_trivially_copyable<_Tp>
    { };

  // Whether memcmp can be used to determine ordering for a type
  // e.g. in std::lexicographical_compare or three-way comparisons.
  // True for unsigned integer-like types where comparing each byte in turn
  // as an unsigned char yields the right result. This is true for all
  // unsigned integers on big endian targets, but only unsigned narrow
  // character types (and std::byte) on little endian targets.
  template<typename _Tp, bool _TreatAsBytes =
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	__is_integer<_Tp>::__value
#else
	__is_byte<_Tp>::__value
#endif
    >
    struct __is_memcmp_ordered
    {
      static const bool __value = _Tp(-1) > _Tp(1); // is unsigned
    };

  template<typename _Tp>
    struct __is_memcmp_ordered<_Tp, false>
    {
      static const bool __value = false;
    };

  // Whether two types can be compared using memcmp.
  template<typename _Tp, typename _Up, bool = sizeof(_Tp) == sizeof(_Up)>
    struct __is_memcmp_ordered_with
    {
      static const bool __value = __is_memcmp_ordered<_Tp>::__value
	&& __is_memcmp_ordered<_Up>::__value;
    };

  template<typename _Tp, typename _Up>
    struct __is_memcmp_ordered_with<_Tp, _Up, false>
    {
      static const bool __value = false;
    };

#if __cplusplus >= 201703L
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  // std::byte is not an integer, but it can be compared using memcmp.
  template<>
    struct __is_memcmp_ordered<std::byte, false>
    { static constexpr bool __value = true; };
#endif

  // std::byte can only be compared to itself, not to other types.
  template<>
    struct __is_memcmp_ordered_with<std::byte, std::byte, true>
    { static constexpr bool __value = true; };

  template<typename _Tp, bool _SameSize>
    struct __is_memcmp_ordered_with<_Tp, std::byte, _SameSize>
    { static constexpr bool __value = false; };

  template<typename _Up, bool _SameSize>
    struct __is_memcmp_ordered_with<std::byte, _Up, _SameSize>
    { static constexpr bool __value = false; };
#endif

#if __glibcxx_type_trait_variable_templates
  template<typename _ValT, typename _Tp>
    constexpr bool __can_use_memchr_for_find
    // Can only use memchr to search for narrow characters and std::byte.
      = __is_byte<_ValT>::__value
	// And only if the value to find is an integer (or is also std::byte).
	  && (is_same_v<_Tp, _ValT> || is_integral_v<_Tp>);
#endif

  //
  // Move iterator type
  //
  template<typename _Tp>
    struct __is_move_iterator
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  // Fallback implementation of the function in bits/stl_iterator.h used to
  // remove the move_iterator wrapper.
  template<typename _Iterator>
    _GLIBCXX20_CONSTEXPR
    inline _Iterator
    __miter_base(_Iterator __it)
    { return __it; }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
} // extern "C++"

#pragma GCC diagnostic pop

#endif //_CPP_TYPE_TRAITS_H
