// Prototypes for GLIBC thread locale __-prefixed functions -*- C++ -*-

// Copyright (C) 2002, 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

// Written by Jakub Jelinek <jakub@redhat.com>

#include <bits/c++config.h>
#include <clocale>

#ifdef __UCLIBC_MJN3_ONLY__
#warning clean this up
#endif

#ifdef __UCLIBC_HAS_XLOCALE__

extern "C" __typeof(nl_langinfo_l) __nl_langinfo_l;
extern "C" __typeof(strcoll_l) __strcoll_l;
extern "C" __typeof(strftime_l) __strftime_l;
extern "C" __typeof(strtod_l) __strtod_l;
extern "C" __typeof(strtof_l) __strtof_l;
extern "C" __typeof(strtold_l) __strtold_l;
extern "C" __typeof(strxfrm_l) __strxfrm_l;
extern "C" __typeof(newlocale) __newlocale;
extern "C" __typeof(freelocale) __freelocale;
extern "C" __typeof(duplocale) __duplocale;
extern "C" __typeof(uselocale) __uselocale;

#ifdef _GLIBCXX_USE_WCHAR_T
extern "C" __typeof(iswctype_l) __iswctype_l;
extern "C" __typeof(towlower_l) __towlower_l;
extern "C" __typeof(towupper_l) __towupper_l;
extern "C" __typeof(wcscoll_l) __wcscoll_l;
extern "C" __typeof(wcsftime_l) __wcsftime_l;
extern "C" __typeof(wcsxfrm_l) __wcsxfrm_l;
extern "C" __typeof(wctype_l) __wctype_l;
#endif

# define __nl_langinfo_l nl_langinfo_l
# define __strcoll_l strcoll_l
# define __strftime_l strftime_l
# define __strtod_l strtod_l
# define __strtof_l strtof_l
# define __strtold_l strtold_l
# define __strxfrm_l strxfrm_l
# define __newlocale newlocale
# define __freelocale freelocale
# define __duplocale duplocale
# define __uselocale uselocale

# ifdef _GLIBCXX_USE_WCHAR_T
#  define __iswctype_l iswctype_l
#  define __towlower_l towlower_l
#  define __towupper_l towupper_l
#  define __wcscoll_l wcscoll_l
#  define __wcsftime_l wcsftime_l
#  define __wcsxfrm_l wcsxfrm_l
#  define __wctype_l wctype_l
# endif

#else
# define __nl_langinfo_l(N, L)       nl_langinfo((N))
# define __strcoll_l(S1, S2, L)      strcoll((S1), (S2))
# define __strtod_l(S, E, L)         strtod((S), (E))
# define __strtof_l(S, E, L)         strtof((S), (E))
# define __strtold_l(S, E, L)        strtold((S), (E))
# define __strxfrm_l(S1, S2, N, L)   strxfrm((S1), (S2), (N))
# warning should dummy __newlocale check for C|POSIX ?
# define __newlocale(a, b, c)        NULL
# define __freelocale(a)             ((void)0)
# define __duplocale(a)              __c_locale()
//# define __uselocale ?
//
# ifdef _GLIBCXX_USE_WCHAR_T
#  define __iswctype_l(C, M, L)       iswctype((C), (M))
#  define __towlower_l(C, L)          towlower((C))
#  define __towupper_l(C, L)          towupper((C))
#  define __wcscoll_l(S1, S2, L)      wcscoll((S1), (S2))
//#  define __wcsftime_l(S, M, F, T, L)  wcsftime((S), (M), (F), (T))
#  define __wcsxfrm_l(S1, S2, N, L)   wcsxfrm((S1), (S2), (N))
#  define __wctype_l(S, L)            wctype((S))
# endif

#endif // GLIBC 2.3 and later
