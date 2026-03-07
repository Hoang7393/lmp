# Lispy Meta Programming

C++ template meta programming in a Lisp style.

See `test.cc` for examples.


## Example: Sieve of Eratosthenes

```cpp
#include <lmp.h>

using namespace lmp;

meta_fn(infinite_integers, int n) {
    let_lazy(next, infinite_integers<n + 1>);
    meta_return (Cons<Int<n>, next>);
};

meta_fn(filter_mod, class lst, int n) {
    let_lazy(tail, filter_mod<cdr<lst>, n>);
    meta_return (
        cond<equal<mod<car<lst>, Int<n>>, Int<0>>,
            tail,
            Cons<car<lst>, tail>>);
};

meta_fn(prime_sieve, class lst) {
    static constexpr int n = car<lst>::value;
    let_lazy(tail, prime_sieve<filter_mod<cdr<lst>, n>>);
    meta_return (Cons<Int<n>, tail>);
};

struct primes {
    meta_return (prime_sieve<infinite_integers<2>>);
};

static_assert(nth<primes, 0>::value == 2);
static_assert(nth<primes, 1>::value == 3);
static_assert(nth<primes, 2>::value == 5);
static_assert(nth<primes, 3>::value == 7);
static_assert(nth<primes, 4>::value == 11);
static_assert(nth<primes, 5>::value == 13);
```