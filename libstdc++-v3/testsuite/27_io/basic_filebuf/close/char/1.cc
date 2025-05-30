// Copyright (C) 2001-2025 Free Software Foundation, Inc.
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

// C++98 27.8.1.3 filebuf member functions

// various tests for filebuf::open() and filebuf::close() including
// the non-portable functionality in the libstdc++-v3 IO library

// { dg-require-fileio "" }
// { dg-additional-files "filebuf_members-1.tst filebuf_members-1.txt" }
// { dg-final { file-io-diff "filebuf_members-1" } }

#include <fstream>
#include <testsuite_hooks.h>

const char name_01[] = "filebuf_members-1.tst";
const char name_02[] = "filebuf_members-1.txt";

// Test member functions.
void test_01() 
{
  const char* name_03 = "filebuf_members-3"; // empty file, need to create

  std::filebuf fb_01; // in 
  std::filebuf fb_02; // out
  std::filebuf fb_03; // in | out

  // bool is_open()
  VERIFY( !fb_01.is_open() );
  VERIFY( !fb_02.is_open() );
  VERIFY( !fb_03.is_open() );

  // filebuf_type* open(const char* __s, ios_base::openmode __mode)
  fb_01.open(name_01, std::ios_base::in | std::ios_base::ate);
  VERIFY( fb_01.is_open() );

  // Try to open two different files without closing the first:
  // Should keep the old file attached, and disregard attempt to overthrow.
  fb_02.open(name_02, std::ios_base::in | std::ios_base::out 
	     | std::ios_base::trunc);
  VERIFY( fb_02.is_open() );

  fb_03.open(name_03, std::ios_base::out | std::ios_base::trunc);
  VERIFY( fb_03.is_open() );

  // filebuf_type* close()
  std::filebuf* f;
  f = fb_01.close();
  VERIFY( f );
  VERIFY( !fb_01.is_open() );

  f = fb_02.close();
  VERIFY( f );
  VERIFY( !fb_02.is_open() );

  f = fb_03.close();
  VERIFY( f );
  VERIFY( !fb_03.is_open() );

  f = fb_03.close();
  VERIFY( !f );
  VERIFY( !fb_03.is_open() );
}

int
main()
{
  test_01();
  return 0;
}


