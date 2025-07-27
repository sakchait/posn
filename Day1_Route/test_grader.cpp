#include <iostream>
#include <sstream>
#include "grader.cpp" // Directly include grader.cpp for testing

//g++ -std=c++17 -O2 test_grader.cpp route.cpp -o test_grader.exe
int main() {
    // Prepare test input: N, M, and M weights (example: N=4, M=6)
    std::istringstream test_input("4 6\n1 2 3 4 5 6\n");
    std::streambuf* orig_cin = std::cin.rdbuf(test_input.rdbuf());

    // Call main from grader.cpp
    main();

    // Restore original cin
    std::cin.rdbuf(orig_cin);

    return 0;
}