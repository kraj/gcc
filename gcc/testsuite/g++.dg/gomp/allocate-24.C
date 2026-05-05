/* { dg-do compile { target c++11 } } */

#include "allocate-allocator-handle.h"

/* TODO: Add scan-tree-dump checks once support for omp::decl is added.  */

int AAA [[omp::decl(allocate,allocator(omp_low_lat_mem_alloc),align(4096))]];
/* { dg-message "allocate directive not supported in 'omp::decl'" "" { target *-*-* } .-1 } */

static_assert (alignof(AAA) == alignof(int), "wrong alignment");

void f ()
{
  /* This applies to A1 and B1.
     (C++23 N4950 §9.1.3, sentence 5)
     The attribute-specifier-seq appertains to each of the entities declared by
     the declarators of the init-declarator-list.  */
  [[omp::decl(allocate, align(4096))]] int A1[5], B1;
  /* { dg-message "allocate directive not supported in 'omp::decl'" "" { target *-*-* } .-1 } */

  static_assert (alignof(A1) == alignof(int[5]), "wrong alignment");
  static_assert (alignof(B1) == alignof(int), "wrong alignment");

  static int BBB [[omp::decl(allocate,allocator(omp_low_lat_mem_alloc),align(4096))]];
  /* { dg-message "allocate directive not supported in 'omp::decl'" "" { target *-*-* } .-1 } */
}
