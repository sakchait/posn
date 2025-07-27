#include <iostream>
#include <vector>
#include <algorithm> // For std::sort if needed, but ordered_set keeps sorted.
#include <cmath>     // For std::floor, std::abs

// For policy-based data structures
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

// Define an ordered_set that behaves like a multiset by storing pairs (value, unique_id)
// This is necessary because std::less_equal can be tricky with tree.
// std::less is generally safer and unique_id ensures distinct nodes for find_by_order
using namespace __gnu_pbds;
template<typename T>
using ordered_set_multiset_behavior = tree<
    std::pair<T, int>, // Store pair<value, unique_id>
    null_type,
    std::less<std::pair<T, int>>, // Use std::less for pairs
    rb_tree_tag,
    tree_order_statistics_node_update>;

int main() {
    // Optimize C++ streams for faster input/output
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N; // Number of orbs
    int L; // Number of rounds
    std::cin >> N >> L;

    int a, b; // Parameters for orb selection
    std::cin >> a >> b;

    ordered_set_multiset_behavior<long long> orbs;
    int current_id = 0; // Unique ID counter for new orbs

    // Read initial orb weights
    for (int i = 0; i < N; ++i) {
        long long w;
        std::cin >> w;
        orbs.insert({w, current_id++});
    }

    // Simulate L rounds
    for (int round = 0; round < L; ++round) {
        // Step 1: Select the a-th and b-th orbs
        // find_by_order is 0-indexed, so we use (a-1) and (b-1)
        auto it_a = orbs.find_by_order(a - 1);
        auto it_b = orbs.find_by_order(b - 1);

        long long x = it_a->first; // Weight of a-th orb
        long long y = it_b->first; // Weight of b-th orb

        // Step 2: Remove the selected orbs
        orbs.erase(it_a);
        orbs.erase(it_b);

        // Step 3: Create and insert new orbs
        long long new_weight1 = y - x;
        long long new_weight2 = static_cast<long long>(std::floor(static_cast<double>(x + y) / 2.0));
        // Note: The problem statement uses floor( (x+y)/2 ). For integers, (x+y)/2 in integer division
        // naturally floors for positive numbers. But using std::floor and double conversion is safer
        // to match the exact mathematical definition, especially if x+y could be negative (though weights are positive here).
        // For positive integers, (x+y)/2 (integer division) is equivalent to floor((x+y)/2.0).

        orbs.insert({new_weight1, current_id++});
        orbs.insert({new_weight2, current_id++});
    }

    // Output the remaining orb weights in sorted order
    bool first = true;
    for (const auto& orb_pair : orbs) {
        if (!first) {
            std::cout << " ";
        }
        std::cout << orb_pair.first;
        first = false;
    }
    std::cout << std::endl;

    return 0;
}