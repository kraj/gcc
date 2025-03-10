// { dg-do run { target c++23 } }

#include <forward_list>
#include <span>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>
#include <testsuite_allocator.h>

template<typename Range, typename Alloc>
constexpr void
do_test()
{
  // The forward_list's value_type.
  using V = typename std::allocator_traits<Alloc>::value_type;

  // The range's value_type.
  using T = std::ranges::range_value_t<Range>;
  T a[]{1,2,3,4,5,6,7,8,9};

  auto eq = [](const std::forward_list<V, Alloc>& l, std::span<T> r) {
    if (std::distance(l.begin(), l.end()) != r.size())
      return false;
    unsigned i = 0;
    for (auto& e : l)
      if (e != r[i++])
	return false;
    return true;
  };

  // assign to empty forward_list
  std::forward_list<V, Alloc> v;
  v.assign_range(Range(a, a));
  VERIFY( v.empty() );
  v.assign_range(Range(a, a+4));
  VERIFY( eq(v, {a, 4}) );
  v.clear();
  v.assign_range(Range(a));
  VERIFY( eq(v, a) );

  // assign to non-empty forward_list
  v.assign_range(Range(a, a+4));
  VERIFY( eq(v, {a, 4}) );
  v.assign_range(Range(a)); // larger than size()
  VERIFY( eq(v, a) );
  v.assign_range(Range(a, a+4)); // smaller than size()
  VERIFY( eq(v, {a, 4}) );
  v.assign_range(Range(a+2, a+6)); // equal to size()
  VERIFY( eq(v, {a+2, 4}) );
  v.assign_range(Range(a, a));
  VERIFY( v.empty() );
}

template<typename Range>
void
do_test_a()
{
  do_test<Range, std::allocator<int>>();
  do_test<Range, __gnu_test::SimpleAllocator<int>>();
}

bool
test_ranges()
{
  using namespace __gnu_test;

  do_test_a<test_forward_range<int>>();
  do_test_a<test_forward_sized_range<int>>();
  do_test_a<test_sized_range_sized_sent<int, forward_iterator_wrapper>>();

  do_test_a<test_input_range<int>>();
  do_test_a<test_input_sized_range<int>>();
  do_test_a<test_sized_range_sized_sent<int, input_iterator_wrapper>>();

  do_test_a<test_range<int, input_iterator_wrapper_nocopy>>();
  do_test_a<test_sized_range<int, input_iterator_wrapper_nocopy>>();
  do_test_a<test_sized_range_sized_sent<int, input_iterator_wrapper_nocopy>>();

  do_test_a<test_forward_range<short>>();
  do_test_a<test_input_range<short>>();

  // Not lvalue-convertible to int
  struct C {
    C(int v) : val(v) { }
    operator int() && { return val; }
    bool operator==(int b) const { return b == val; }
    int val;
  };
  using rvalue_input_range = test_range<C, input_iterator_wrapper_rval>;
  do_test<rvalue_input_range, std::allocator<int>>();

  return true;
}

int main()
{
  test_ranges();
}
