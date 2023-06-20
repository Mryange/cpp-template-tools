#include <iostream>
#include <vector>
#include "src/Tuple.h"
#include "src/Any.h"
void test_Tuple();
void test_Any();
int main() {
    test_Tuple();
    test_Any();
}
void test_Tuple(){
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
void test_Any(){
    std::vector<Any> vec = { 114,514,1919,8.10,std::string("asoul"),std::string("4soul") };
    std::cout << vec << "\n";
    std::vector<Any>oth = { std::string("ygg"),233 };
    vec.push_back(oth);
    std::cout << vec << "\n";
    vec[1] = std::vector<Any>{ oth,oth };
    std::cout << vec << "\n";
}