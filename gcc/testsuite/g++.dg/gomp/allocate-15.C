/* { dg-do compile { target c++11 } } */

/* Diagnostics for rvalue reference vars used in an allocate directive.  */

void rref_var()
{
  int&& ref = 42; /* { dg-note "'ref' declared here" } */
  #pragma omp allocate(ref) /* { dg-error "variables with reference type may not appear in an 'allocate' directive" } */
}

void const_rref_var()
{
  int const&& ref = 42; /* { dg-note "'ref' declared here" } */
  #pragma omp allocate(ref) /* { dg-error "variables with reference type may not appear in an 'allocate' directive" } */
}

template<typename>
void rref_var_templ_not_instantiated()
{
  int&& ref = 42; /* { dg-note "'ref' declared here" } */
  #pragma omp allocate(ref) /* { dg-error "variables with reference type may not appear in an 'allocate' directive" } */
}

template<typename>
void const_rref_var_templ_not_instantiated()
{
  int const&& ref = 42; /* { dg-note "'ref' declared here" } */
  #pragma omp allocate(ref) /* { dg-error "variables with reference type may not appear in an 'allocate' directive" } */
}

template<typename T>
void dependent_rref_var_templ_not_instantiated()
{
  T&& t = 42; /* { dg-note "'t' declared here" } */
  #pragma omp allocate(t) /* { dg-error "variables with reference type may not appear in an 'allocate' directive" } */
}

template<typename T>
void dependent_var_templ()
{
  T t = 42; /* { dg-note "'t' declared here" } */
  #pragma omp allocate(t) /* { dg-error "variables with reference type may not appear in an 'allocate' directive" } */
}
void instantiate_var_templ()
{
  dependent_var_templ<int>(); /* { dg-bogus "required from here" } */
  dependent_var_templ<int&&>(); /* { dg-message "required from here" } */
  dependent_var_templ<int const&&>(); /* { dg-message "required from here" } */
}


/* Diagnostics for constexpr vars used in an allocate directive.  */

void cx_var()
{
  constexpr int cx = 42; /* { dg-note "'cx' declared here" } */
  #pragma omp allocate(cx) /* { dg-error "constexpr variable 'cx' may not appear as list item in an 'allocate' directive" } */
}

template<typename>
void cx_var_templ_not_instantiated()
{
  constexpr int cx = 42; /* { dg-note "'cx' declared here" } */
  #pragma omp allocate(cx) /* { dg-error "constexpr variable 'cx' may not appear as list item in an 'allocate' directive" } */
}

template<typename>
void cx_var_templ()
{
  constexpr int cx = 42; /* { dg-note "'cx' declared here" } */
  #pragma omp allocate(cx) /* { dg-error "constexpr variable 'cx' may not appear as list item in an 'allocate' directive" } */
}

template<typename T>
void dependent_cx_var_templ_not_instantiated()
{
  constexpr T cx = 42; /* { dg-note "'cx' declared here" } */
  #pragma omp allocate(cx) /* { dg-error "constexpr variable 'cx' may not appear as list item in an 'allocate' directive" } */
}

template<typename T>
void dependent_cx_var_templ()
{
  constexpr T cx = 42; /* { dg-note "'cx' declared here" } */
  #pragma omp allocate(cx) /* { dg-error "constexpr variable 'cx' may not appear as list item in an 'allocate' directive" } */
}

void instantiate_cx_templ()
{
  cx_var_templ<void>();
  dependent_cx_var_templ<int>();
}
