#include <iostream>
#include <vector>
#include <utility> // For std::pair
#include <algorithm> // For std::sort if W is not guaranteed sorted by grader, but problem says it is.
#include "route.h" // Include your route function declaration

int main() {
    // Optimize C++ streams for faster input/output
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N, M;
    // Read N (number of locations) and M (number of cars)
    std::cin >> N >> M;

    std::vector<int> W(M);
    // Read M car costs (weights)
    for (int i = 0; i < M; ++i) {
        std::cin >> W[i];
    }
    // W is guaranteed to be sorted by the problem statement, so no need to sort here.

    // Call your route function
    std::vector<std::pair<int, int>> result_routes = route(N, W);

    // Output the results in the required format
    // Assumed format: M lines, each with u v
    for (int i = 0; i < M; ++i) {
        std::cout << result_routes[i].first << " " << result_routes[i].second << "\n";
    }

    return 0;
}