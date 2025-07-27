#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <utility> // For std::pair

// Function declaration for the route problem
// N: Number of important locations (nodes)
// W: Vector of car costs, sorted from least to greatest (M elements)
// Returns: A vector of M pairs (u, v), where (u, v) is the route assigned to car with cost W[i] at index i.
std::vector<std::pair<int, int>> route(int N, std::vector<int> W);

#endif // ROUTE_H