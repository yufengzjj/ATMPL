#include <type_traits>
#include <functional>
#include "bind.hpp"
void test(int, char*){}
auto test_lambda = [](int, char*)->void{};
class test_class
{
public:
    test_class(){}
    ~test_class(){}
    void test(int, char*){}
};
using namespace std::placeholders;
void bind_holder_test()
{
    auto f1 = functional_helper::bind(test, _1,_2);
    auto f2 = functional_helper::bind(f1, _1, _2);
    auto f3 = functional_helper::bind(f1, _1, _2);
    f1(1, nullptr);
    f2(1, nullptr);
    f3(1, nullptr);
    static_assert(std::is_same<decltype(f1), decltype(f2)::bind_type>::value , "");
    static_assert(std::is_same<decltype(f1)::underlying_type::arg<1>::type, char*>::value, "");
    static_assert(std::is_same<functional_helper::bind_holder_stripper<decltype(f3)>::arg<1>::type,char*>::value,"");
    static_assert(std::is_same<functional_helper::bind_holder_stripper<decltype(f3)>::arg<1>::type, decltype(f3)::underlying_type::arg<1>::type>::value, "");
}

void function_traits_test()
{
    auto test_functor = std::function<void(int, char*)>(test);
    static_assert(std::is_same<functional_helper::function_traits<decltype(test)>::arg<1>::type,char* >::value, "");
    static_assert(std::is_same<functional_helper::function_traits<decltype(test_functor)>::arg<1>::type, char*>::value, "");
    static_assert(std::is_same<functional_helper::function_traits<decltype(test_lambda)>::arg<1>::type, char*>::value, "");
}

int main(int argc, char* argv[])
{
    bind_holder_test();
}