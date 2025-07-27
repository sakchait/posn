#include "route.h" // Include the header with the function declaration
#include <algorithm> // For std::min, std::max
#include <set>       // For std::set to track assigned edges

// The implementation of the route function as required by the problem
std::vector<std::pair<int, int>> route(int N, std::vector<int> W) {
    int M = W.size(); // M is the number of cars/edges, which is W.size()

    std::vector<std::pair<int, int>> result_routes(M);
    // Use a set to keep track of assigned edges to prevent duplicates.
    // Store edges as {min_node, max_node} to treat (u,v) and (v,u) as the same.
    std::set<std::pair<int, int>> assigned_edges;

    // Phase 1: Assign N-1 most expensive cars/edges to form a simple path (1-2, 2-3, ..., N-1-N).
    // These N-1 edges guarantee a spanning tree for the N nodes.
    // Since W is sorted by cost, the N-1 most expensive cars are at indices M-(N-1) through M-1.
    for (int i = 0; i < N - 1; ++i) {
        int u = i + 1;
        int v = i + 2;
        // Assign this path segment to the car corresponding to its cost's index.
        // W[M - (N - 1) + i] is the cost of the car being assigned this segment.
        result_routes[M - (N - 1) + i] = {u, v};
        assigned_edges.insert({std::min(u, v), std::max(u, v)});
    }

    // Phase 2: Assign the remaining M - (N-1) cheaper cars/edges to distinct, valid (u,v) pairs.
    // These cars are from index 0 to M-(N-1)-1 in the W vector.
    
    // Systematic generation of (u,v) pairs to ensure uniqueness and completeness.
    // We start looking for pairs from (1,2) onward, but skip those already used in the path.
    int u_gen = 1;
    int v_gen = 1; 

    for (int i = 0; i < M - (N - 1); ++i) {
        // Find the next available car index among the cheaper ones
        int car_index_for_other_edges = i; 

        // Loop until a unique, unassigned edge is found
        while (true) {
            v_gen++; // Increment v_gen to get the next potential partner for u_gen

            if (v_gen > N) { // If v_gen exceeds N, increment u_gen and reset v_gen
                u_gen++;
                v_gen = u_gen + 1; // Start v_gen from u_gen + 1 to avoid self-loops and duplicates (v>u)
            }
            
            // This condition is a safeguard; it implies we have run out of possible unique edges,
            // which should not happen based on the problem constraint M <= N*(N-1)/2.
            if (u_gen > N || v_gen > N) {
                // This indicates an unexpected scenario, typically all possible edges have been assigned.
                // In competitive programming, this means the constraints guarantee we won't reach here.
                break;
            }

            // Normalize the pair for set insertion (always u_gen < v_gen here)
            std::pair<int, int> current_pair = {u_gen, v_gen};

            if (assigned_edges.find(current_pair) == assigned_edges.end()) {
                // Found a unique, unassigned edge. Assign it.
                result_routes[car_index_for_other_edges] = {u_gen, v_gen};
                assigned_edges.insert(current_pair);
                break; // Exit while loop, move to assign the next car
            }
            // If the current pair is already assigned, the loop continues to find the next (u,v).
        }
    }

    return result_routes;
}