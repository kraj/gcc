typedef enum omp_allocator_handle_t 
#if __cplusplus >= 201103L 
: __UINTPTR_TYPE__ 
#endif 
{ 
  omp_default_mem_alloc = 1, 
  omp_low_lat_mem_alloc = 5, 
  __omp_allocator_handle_t_max__ = __UINTPTR_MAX__ 
} omp_allocator_handle_t; 

omp_allocator_handle_t foo(int, int *);


void
f ()
{
  int v;  /* { dg-note "to be allocated variable declared here" } */
  static const int n = 5;
  int a = 1;
  /* { dg-note "declared here" "" { target *-*-* } .-1 } */
  int b[n];
  /* { dg-note "declared here" "" { target c++ } .-1 } */
  b[a] = 5;
  #pragma omp allocate (v) allocator (foo (a, &b[a]))
  /* { dg-error "variable 'a' used in the 'allocator' clause must be declared before 'v'" "" { target *-*-* } .-1 } */
  /* { dg-error "variable 'b' used in the 'allocator' clause must be declared before 'v'" "" { target c++ } .-2 } */
}

void
g ()
{
  int n = 5;
  int a = 1;
  int b[n];
  b[a] = 5;
  int v;  /* { dg-note "to be allocated variable declared here" "" { xfail c++ } } */
  a = 2;  /* { dg-note "modified here" "" { xfail c++ } } */
  #pragma omp allocate (v) allocator (foo (a, &b[a]))
  /* { dg-error "variable 'a' used in the 'allocator' clause must not be modified between declaration of 'v' and its 'allocate' directive" "" { xfail c++ } .-1 } */
}
