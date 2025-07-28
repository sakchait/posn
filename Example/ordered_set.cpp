// Example: Using ordered_set_multiset_behavior to store duplicate values and access by order
#include <iostream>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

// Define ordered_set_multiset_behavior for int
using namespace __gnu_pbds;
template<typename T>
using ordered_set_multiset_behavior = tree<
    std::pair<T, int>,
    null_type,
    std::less<std::pair<T, int>>,
    rb_tree_tag,
    tree_order_statistics_node_update>;

int main() {
    ordered_set_multiset_behavior<int> os;
    int id = 0;

    // Insert duplicate values
    os.insert({5, id++});
    os.insert({3, id++});
    os.insert({5, id++});
    os.insert({7, id++});

    // Print all values in sorted order
    std::cout << "Ordered values: ";
    for (const auto& p : os) {
        std::cout << p.first << " ";
    }
    std::cout << std::endl;

    // Access by order (0-based)
    std::cout << "Element at order 1: " << os.find_by_order(1)->first << std::endl; // Should print 5

    // Count elements less than 6
    std::cout << "Elements less than 6: " << os.order_of_key(std::make_pair(6, 0)) << std::endl; // Should print 3

    return 0;
}
// Example output:
// Ordered values: 3 5 5 7
// Element at order 1: 5
// Elements less than 6: