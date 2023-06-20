#include <iostream>
#include <vector>
#include <array>
#include "src/Tuple.h"
#include "src/Any.h"
#include "src/Variant.h"
void test_Tuple();
void test_Any();
void test_Variant();
int main() {
    // test_Tuple();
    // test_Any();
    test_Variant();
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
    Variant<line, std::array<int,4>> arr;
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
