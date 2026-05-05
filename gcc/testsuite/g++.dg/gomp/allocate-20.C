/* { dg-do compile { target c++11 } } */
#include "allocate-allocator-handle.h"

/* Diagnose static variables used in an OpenMP allocate directive in functions
   that are implicitly constexpr.
   Otherwise, check that static variables have a correct alignment applied to
   them.

   This test case is valid in c++11 but the bug we are testing for does not
   manifest until c++17, which makes lambdas implicit constexpr functions.
   
   For now, we simply do not support these cases.  */


/* Making a regex for demangled identifiers is actually way harder than making
   a regex for mangled ones, too many escapes are needed.  */

/* { dg-final { scan-assembler "\.align 256\\s*\.type\\s*_ZZZ6f0_256vENKUlvE_clEvE1a" { target c++14_down } } } */
int* f0_256()
{
  auto cl = [](){
    static int a = 42;
    #pragma omp allocate(a) align(256) allocator(omp_default_mem_alloc)
    /* { dg-message "static variables are not supported in an 'allocate' directive in an implicit constexpr function" "" { target c++17 } .-1 } */
    return &a;
  };
  return cl();
}
/* { dg-final { scan-assembler "\.align 512\\s*\.type\\s*_ZZZ6f0_512vENKUlvE_clEvE1a" { target c++14_down } } } */
int* f0_512()
{
  auto cl = [](){
    static int a = 42;
    #pragma omp allocate(a) align(512) allocator(omp_default_mem_alloc)
    /* { dg-message "static variables are not supported in an 'allocate' directive in an implicit constexpr function" "" { target c++17 } .-1 } */
    return &a;
  };
  return cl();
}
/* { dg-final { scan-assembler "\.align 1024\\s*\.type\\s*_ZZZ7f0_1024vENKUlvE_clEvE1a" { target c++14_down } } } */
int* f0_1024()
{
  auto cl = [](){
    static int a = 42;
    #pragma omp allocate(a) align(1024) allocator(omp_default_mem_alloc)
    /* { dg-message "static variables are not supported in an 'allocate' directive in an implicit constexpr function" "" { target c++17 } .-1 } */
    return &a;
  };
  return cl();
}

/* { dg-final { scan-assembler "\.align 256\\s*\.type\\s*_ZZZ6f1_256IvEPivENKUlvE_clEvE1a" { target c++14_down } } } */
template<typename>
int* f1_256()
{
  auto cl = [](){
    static int a = 42;
    #pragma omp allocate(a) align(256) allocator(omp_default_mem_alloc)
    /* { dg-message "static variables are not supported in an 'allocate' directive in an implicit constexpr function" "" { target c++17 } .-1 } */
    return &a;
  };
  return cl();
}
template int* f1_256<void>();

/* { dg-final { scan-assembler "\.align 512\\s*\.type\\s*_ZZZ6f1_512IvEPivENKUlvE_clEvE1a" { target c++14_down } } } */
template<typename>
int* f1_512()
{
  auto cl = [](){
    static int a = 42;
    #pragma omp allocate(a) align(512) allocator(omp_default_mem_alloc)
    /* { dg-message "static variables are not supported in an 'allocate' directive in an implicit constexpr function" "" { target c++17 } .-1 } */
    return &a;
  };
  return cl();
}
template int* f1_512<void>();

/* { dg-final { scan-assembler "\.align 1024\\s*\.type\\s*_ZZZ7f1_1024IvEPivENKUlvE_clEvE1a" { target c++14_down } } } */
template<typename>
int* f1_1024()
{
  auto cl = [](){
    static int a = 42;
    #pragma omp allocate(a) align(1024) allocator(omp_default_mem_alloc)
    /* { dg-message "static variables are not supported in an 'allocate' directive in an implicit constexpr function" "" { target c++17 } .-1 } */
    return &a;
  };
  return cl();
}
template int* f1_1024<void>();

/* Missing cases for generic lambda, and generic lambda in function template.
   They shouldn't behave differently, but for completeness they should be
   added, I'm just not going to spend any more time on this right now.  */
