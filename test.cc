#include <type_traits>
#include <cstdio>

struct nil { 
    using type = nil;
};

template<typename head, typename tail>
struct cons {
    using car = head;
    using cdr = tail;
    using type = cons<head, tail>;
};

template<typename lst>
using car = typename lst::car;

template<typename lst>
using cdr = typename lst::cdr;

template<typename lst>
using cadr = car<cdr<lst>>;

template<int N>
using cint = std::integral_constant<int, N>;

template<typename L, typename R>
using eq = std::is_same<L, R>;

template<typename T>
struct delay { using type = T; };

template<typename T>
using force = typename T::type;

template<bool B, typename tb, typename fb>
struct cond;

template<typename tb, typename fb>
struct cond<true, tb, fb> {
  using type = force<tb>;
};

template<typename tb, typename fb>
struct cond<false, tb, fb> {
  using type = force<fb>;
};

template<typename L>
struct length;

template<typename lhs, typename rhs>
struct add {
    using lvalue = force<lhs>;
    using rvalue = force<rhs>;
    using type = cint<lvalue::value + rvalue::value>;
};

#define let_lazy(__name__, ...) \
    struct __name__ { \
        using type = typename __VA_ARGS__::type; \
    }; \

template<typename Lst>
struct length {
    // length is a function, args must be evaluated first
    using lst = force<Lst>;

    let_lazy(cdr_length, length<cdr<lst>>);
    let_lazy(expr,
        cond<
            eq<lst, nil>::value,
            cint<0>,
            add<cint<1>, cdr_length>>);

    using type = force<expr>;
    static constexpr int value = type::value;
};

using my_list = cons<cint<1>, cons<cint<2>, nil>>;
int main() {
  std::printf("first: %d\n", car<my_list>::value);
  std::printf("second: %d\n", cadr<my_list>::value);
  std::printf("length: %d\n", length<my_list>::value);
}