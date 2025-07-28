// Simple example: Using std::map in C++
#include <iostream>
#include <map>

int main() {
    // Create a map from string to int
    std::map<std::string, int> age;

    // Add key-value pairs
    age["Alice"] = 25;
    age["Bob"] = 30;
    age["Charlie"] = 22;

    // Access value by key
    std::cout << "Alice's age: " << age["Alice"] << std::endl;

    // Iterate and print all key-value pairs
    std::cout << "All ages:\n";
    for (const auto& pair : age) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
// Output:
// Alice's age: 25
// All ages:
// Alice: 25
// Bob: 30
// Charlie: 22