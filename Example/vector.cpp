// Simple example: Using std::vector in C++
#include <iostream>
#include <vector>

int main() {
    // Create a vector of integers
    std::vector<int> numbers;

    // Add elements to the vector
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    // Access elements by index
    std::cout << "First element: " << numbers[0] << std::endl;

    // Iterate and print all elements
    std::cout << "All elements: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Get the size of the vector
    std::cout << "Vector size: " << numbers.size() << std::endl;

    return 0;
}
// Output:
// First element: 10
// All elements: 10 20 30
// Vector size: 3