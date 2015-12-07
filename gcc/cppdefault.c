/* CPP Library.
   Copyright (C) 1986-2019 Free Software Foundation, Inc.
   Contributed by Per Bothner, 1994-95.
   Based on CCCP program by Paul Rubin, June 1986
   Adapted to ANSI C, Richard Stallman, Jan 1987

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "cppdefault.h"

#ifndef NATIVE_SYSTEM_HEADER_COMPONENT
#define NATIVE_SYSTEM_HEADER_COMPONENT 0
#endif

#if defined (CROSS_DIRECTORY_STRUCTURE) && !defined (TARGET_SYSTEM_ROOT)
# undef LOCAL_INCLUDE_DIR
# undef NATIVE_SYSTEM_HEADER_DIR
#else
# undef CROSS_INCLUDE_DIR
#endif

static char GPLUSPLUS_INCLUDE_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = GPLUSPLUS_INCLUDE_DIR;
char GCC_INCLUDE_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = GCC_INCLUDE_DIR;
static char GPLUSPLUS_TOOL_INCLUDE_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = GPLUSPLUS_TOOL_INCLUDE_DIR;
static char GPLUSPLUS_BACKWARD_INCLUDE_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = GPLUSPLUS_BACKWARD_INCLUDE_DIR;
static char STANDARD_STARTFILE_PREFIX_2VAR[4096] __attribute__ ((section (".gccrelocprefix"))) = STANDARD_STARTFILE_PREFIX_2 GCC_INCLUDE_SUBDIR_TARGET;
#ifdef LOCAL_INCLUDE_DIR
static char LOCAL_INCLUDE_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = LOCAL_INCLUDE_DIR;
#endif
#ifdef PREFIX_INCLUDE_DIR
static char PREFIX_INCLUDE_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = PREFIX_INCLUDE_DIR;
#endif
#ifdef FIXED_INCLUDE_DIR
static char FIXED_INCLUDE_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = FIXED_INCLUDE_DIR;
#endif
#ifdef CROSS_INCLUDE_DIR
static char CROSS_INCLUDE_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = CROSS_INCLUDE_DIR;
#endif
#ifdef TOOL_INCLUDE_DIR
static char TOOL_INCLUDE_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = TOOL_INCLUDE_DIR;
#endif
#ifdef NATIVE_SYSTEM_HEADER_DIR
static char NATIVE_SYSTEM_HEADER_DIRVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = NATIVE_SYSTEM_HEADER_DIR;
#endif

const struct default_include cpp_include_defaults[]
#ifdef INCLUDE_DEFAULTS
= INCLUDE_DEFAULTS;
#else
= {
#ifdef GPLUSPLUS_INCLUDE_DIR
    /* Pick up GNU C++ generic include files.  */
    { GPLUSPLUS_INCLUDE_DIRVAR, "G++", 1, 1,
      GPLUSPLUS_INCLUDE_DIR_ADD_SYSROOT, 0 },
#endif
#ifdef GPLUSPLUS_TOOL_INCLUDE_DIR
    /* Pick up GNU C++ target-dependent include files.  */
    { GPLUSPLUS_TOOL_INCLUDE_DIRVAR, "G++", 1, 1,
      GPLUSPLUS_INCLUDE_DIR_ADD_SYSROOT, 1 },
#endif
#ifdef GPLUSPLUS_BACKWARD_INCLUDE_DIR
    /* Pick up GNU C++ backward and deprecated include files.  */
    { GPLUSPLUS_BACKWARD_INCLUDE_DIRVAR, "G++", 1, 1,
      GPLUSPLUS_INCLUDE_DIR_ADD_SYSROOT, 0 },
#endif
#ifdef GCC_INCLUDE_DIR
    /* This is the dir for gcc's private headers.  */
    { GCC_INCLUDE_DIRVAR, "GCC", 0, 0, 0, 0 },
#endif
#ifdef GCC_INCLUDE_SUBDIR_TARGET
    /* This is the dir for gcc's private headers under the specified sysroot.  */
    { STANDARD_STARTFILE_PREFIX_2VAR, "GCC", 0, 0, 1, 0 },
#endif
#ifdef LOCAL_INCLUDE_DIR
    /* /usr/local/include comes before the fixincluded header files.  */
    { LOCAL_INCLUDE_DIRVAR, 0, 0, 1, 1, 2 },
    { LOCAL_INCLUDE_DIRVAR, 0, 0, 1, 1, 0 },
#endif
#ifdef PREFIX_INCLUDE_DIR
    { PREFIX_INCLUDE_DIRVAR, 0, 0, 1, 0, 0 },
#endif
#ifdef FIXED_INCLUDE_DIR
    /* This is the dir for fixincludes.  */
    { FIXED_INCLUDE_DIRVAR, "GCC", 0, 0, 0,
      /* A multilib suffix needs adding if different multilibs use
	 different headers.  */
#ifdef SYSROOT_HEADERS_SUFFIX_SPEC
      1
#else
      0
#endif
    },
#endif
#ifdef CROSS_INCLUDE_DIR
    /* One place the target system's headers might be.  */
    { CROSS_INCLUDE_DIRVAR, "GCC", 0, 0, 0, 0 },
#endif
#ifdef TOOL_INCLUDE_DIR
    /* Another place the target system's headers might be.  */
    { TOOL_INCLUDE_DIRVAR, "BINUTILS", 0, 1, 0, 0 },
#endif
#ifdef NATIVE_SYSTEM_HEADER_DIR
    /* /usr/include comes dead last.  */
    { NATIVE_SYSTEM_HEADER_DIRVAR, NATIVE_SYSTEM_HEADER_COMPONENT, 0, 0, 1, 2 },
    { NATIVE_SYSTEM_HEADER_DIRVAR, NATIVE_SYSTEM_HEADER_COMPONENT, 0, 0, 1, 0 },
#endif
    { 0, 0, 0, 0, 0, 0 }
  };
#endif /* no INCLUDE_DEFAULTS */

/* The configured prefix.  */
char PREFIXVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = PREFIX;
char EXEC_PREFIXVAR[4096] __attribute__ ((section (".gccrelocprefix"))) = STANDARD_EXEC_PREFIX;

/* This value is set by cpp_relocated at runtime */
const char *gcc_exec_prefix;

/* Return true if the toolchain is relocated.  */
bool
cpp_relocated (void)
{
  static int relocated = -1;

  /* A relocated toolchain ignores standard include directories.  */
  if (relocated == -1)
    {
      /* Check if the toolchain was relocated?  */
      gcc_exec_prefix = getenv ("GCC_EXEC_PREFIX");
      if (gcc_exec_prefix)
       relocated = 1;
      else
       relocated = 0;
    }

  return relocated;
}
