#include <iostream>
#include <vector>
#include "route.h"

//g++ -std=c++17 -O2 test_route.cpp route.cpp -o test_route.exe
int main() {
    int N = 4;
    std::vector<int> W = {1, 2, 3, 4, 5, 6}; // Example weights

    auto routes = route(N, W);

    std::cout << "Assigned routes:" << std::endl;
    for (const auto& p : routes) {
        std::cout << p.first << " " << p.second << std::endl;
    }
    return 0;
}