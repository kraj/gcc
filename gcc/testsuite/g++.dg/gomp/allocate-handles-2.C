/* { dg-do compile { target c++11 } } */
#include "allocate-allocator-handle.h"

/* I had wanted to simply include /include/gomp-constants.h to ensure
   synchronization, but including files from that directory does not seem
   to be supported.  */
#define GOMP_OMP_PREDEF_ALLOC_MAX	8
#define GOMP_OMPX_PREDEF_ALLOC_MIN	200
#define GOMP_OMPX_PREDEF_ALLOC_MAX	201

/* Test that all predefined allocators are correctly treated as predefined.  */

template<omp_allocator_handle_t Alloc>
void test_predefined_alloc()
{
  static int a = 42;
  #pragma omp allocate(a) allocator(Alloc)
}

template<omp_allocator_handle_t Alloc>
void test_predefined_alloc_disallowed()
{
  static int a = 42;
  #pragma omp allocate(a) allocator(Alloc)  // { dg-error "'allocator' clause must not be 'omp_cgroup_mem_alloc', 'omp_pteam_mem_alloc', or 'omp_thread_mem_alloc'" }
}

/* Because this is written to work as far back as c++11 it is a little bit
   crusty.  It is metaprogrammed to automatically test the full ranges
   specified above.  */

void do_tests()
{
  test_predefined_alloc<omp_default_mem_alloc> ();
  test_predefined_alloc<omp_large_cap_mem_alloc> ();
  test_predefined_alloc<omp_const_mem_alloc> ();
  test_predefined_alloc<omp_high_bw_mem_alloc> ();
  test_predefined_alloc<omp_low_lat_mem_alloc> ();

  /* These are expected to give an error because they don't make sense
     on static variables.  See PR122892.  */
  test_predefined_alloc_disallowed<omp_cgroup_mem_alloc> ();  // { dg-message "required from here" }
  test_predefined_alloc_disallowed<omp_pteam_mem_alloc> ();  // { dg-message "required from here" }
  test_predefined_alloc_disallowed<omp_thread_mem_alloc> ();  // { dg-message "required from here" }
}
