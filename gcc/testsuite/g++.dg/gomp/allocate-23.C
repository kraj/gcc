/* { dg-do compile { target c++14 } } */
#include "allocate-allocator-handle.h"

/* Valid uses of vars in an allocator clause.  */

void constexpr_var_declared_after()
{
  int a = 42;
  static constexpr omp_allocator_handle_t my_handle = omp_default_mem_alloc;
  #pragma omp allocate(a) allocator(my_handle)
}

template<typename...>
constexpr omp_allocator_handle_t get_alloc()
{
  return omp_default_mem_alloc;
}

void unevaluated_use_of_var()
{
  int a = 42;
  int b = 42;
  #pragma omp allocate(a, b) allocator(get_alloc<decltype(a), decltype(b)>())
}
