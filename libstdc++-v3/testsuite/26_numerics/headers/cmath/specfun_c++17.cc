// { dg-do compile }

// Copyright (C) 2007-2025 Free Software Foundation, Inc.
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

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <cmath>

namespace gnu
{
  // C++17 additions from TR1.
  using std::assoc_laguerre;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::assoc_legendre;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::beta;		// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::comp_ellint_1;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::comp_ellint_2;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::comp_ellint_3;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::cyl_bessel_i;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::cyl_bessel_j;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::cyl_bessel_k;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::cyl_neumann;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::ellint_1;		// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::ellint_2;		// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::ellint_3;		// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::expint;		// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::hermite;		// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::laguerre;		// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::legendre;		// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::riemann_zeta;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::sph_bessel;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::sph_legendre;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  using std::sph_neumann;	// { dg-error "has not been declared" "" { target { ! c++17 } } }
  // These two were in TR1 but not added to C++17.
  using std::conf_hyperg;	// { dg-error "has not been declared" }
  using std::hyperg;		// { dg-error "has not been declared" }
}
