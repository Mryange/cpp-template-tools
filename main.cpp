#include <iostream>
#include <vector>
#include <array>
#include "src/Tuple.h"
#include "src/Any.h"
#include "src/Variant.h"
#include "src/Function.h"
#include "src/Function_ref.h"
void test_Tuple();
void test_Any();
void test_Variant();
void test_Function();
void test_Function_ref();
void test_Visit();
int main() {
    // using var_t = Variant<int, char, std::string>;
    // auto f = [](auto &&v) {
    //     // 获得v的类型
    //     using T = std::decay_t<decltype(v)>;
    //     // 根据不同的类型，执行不同的操作
    //     if constexpr (std::is_same_v<T, int>) {
    //         std::cout << "int : " << v << "\n";
    //     }
    //     if constexpr (std::is_same_v<T, char>) {
    //         std::cout << "char : " << v << "\n";
    //     }
    //     if constexpr (std::is_same_v<T, std::string>) {
    //         std::cout << "string : " << v << "\n";
    //     }
    // };
    // var_t v{114};
    // Visit(f, v);
    // v =std::string{ "514"};
    // Visit(f, v);
    // Visit(f, var_t{'a'});
    test_Function_ref();
}

void test_Visit() {
    auto f = [](auto &&v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, int>) {
            std::cout << "int\n";
        } else if constexpr (std::is_same_v<T, char>) {
            std::cout << "char\n";
        } else if constexpr (std::is_same_v<T, std::vector<int>>) {
            std::cout << "vec\n";
        } else {
            std::cout << "???\n";
        }
    };
    using var_t = Variant<int, char, std::vector<int>>;
    Visit(f, var_t{1});
    Visit(f, var_t{'a'});
    Visit(f, var_t{std::vector<int>{1, 2, 3, 4, 5}});
}

void test_Tuple() {
    Tuple<int, std::string> t{21, "ygg"};
    std::cout << Get<0>(t) << " " << Get<1>(t) << "\n";
    std::vector<Tuple<int, int, double>> vec;
    for (int i = 1; i <= 10; i++) {
        vec.push_back({i, i, 1.0 / i});
    }
    for (auto &x : vec) {
        std::cout << Get<0>(x) << " " << Get<1>(x) << " " << Get<2>(x) << "\n";
    }
}
void test_Any() {
    std::vector<Any> vec = {114, 514, 1919, 8.10, std::string("asoul"), std::string("4soul")};
    std::cout << vec << "\n";
    std::vector<Any> oth = {std::string("ygg"), 233};
    vec.push_back(oth);
    std::cout << vec << "\n";
    vec[1] = std::vector<Any>{oth, oth};
    std::cout << vec << "\n";
}

void test_Variant() {
    struct line {
        int x1, y1, x2, y2;
    };
    Variant<line, std::array<int, 4>> arr;
    for (int i = 0; i < 4; i++) arr.get<1>()[i] = (i + 1) * 100;
    std::cout << arr.get<0>().x1 << " " << arr.get<0>().y1 << " "
              << arr.get<0>().x2 << " " << arr.get<0>().y2 << std::endl;
    Variant<int, std::vector<std::string>> x;
    x = 14 + arr.get<0>().x1;
    std::cout << x.get<int>() << std::endl;
    x = std::move(std::vector<std::string>{"hello", "world"});
    std::string str = "ygg";
    x.get<1>().push_back(std::move(str));
    for (auto s : x.get<1>()) {
        std::cout << s << std::endl;
    }
}
void foo(int n, Function<void()> &call_back) {
    int sum = 0;
    for (int i = 1; i <= n; i++) sum += i;
    call_back = [=]() {
        std::cout << "sum  from 1"
                  << " to " << n << " is " << sum << "\n";
    };
}
void test_Function() {
    srand(time(nullptr));
    std::vector<Function<void()>> func(3);
    for (auto &f : func) {
        foo(rand(), f);
    }
    Function<int(int)> fib = [&](int n) {
        if (n == 0 || n == 1) return 1;
        return fib(n - 1) + fib(n - 2);
    };
    for (int i = 0; i < 10; i++) std::cout << fib(i) << " ";
    std::cout << "\n";
    for (auto &f : func) {
        f();
    }
}

void test_Function_ref() {
    auto foo = [](Function_ref<void()> ref) {
        ref();
    };
    int cnt{0};
    foo([&]() { cnt += 114514; });
    std::cout << cnt << "\n";
    Function_ref<void()> a = [](){std::cout<<"hello\n";};
    auto b =a;
    a();
    b();
}