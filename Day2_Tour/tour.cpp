#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <tuple> // For storing paths in tuples if needed

using namespace std;

// Structure to hold edge information
struct Edge {
    int u, v;       // start_shop_id, end_shop_id
    int type;       // 1 for orange, 2 for rice
    long long weight; // number of dolls
};

// Store shop to zone mapping and shop start indices for zones
vector<int> zone_start_shop_id;
vector<int> n_shops_in_zone;
int total_zones;

// Function to get zone ID from shop ID
int get_zone_id(int shop_id) {
    auto it = upper_bound(zone_start_shop_id.begin(), zone_start_shop_id.end(), shop_id);
    return distance(zone_start_shop_id.begin(), it);
}

// Global vectors to store paths
// Each path will be a tuple: (F, G, path_sequence_hash/ID)
// Using vector<int> for path_sequence for now for simplicity, but a hash might be needed for large number of paths
vector<tuple<long long, long long, vector<int>>> all_forward_paths;
vector<tuple<long long, long long, vector<int>>> all_backward_paths;

// Adjacency list for forward graph (shop_id -> list of edges)
vector<vector<Edge>> adj_forward;
// Adjacency list for backward graph (shop_id -> list of reverse edges)
vector<vector<Edge>> adj_backward;

// DFS for generating forward paths
void generate_forward_paths(int current_shop, int current_zone, long long current_F, long long current_G, vector<int>& path_sequence) {
    path_sequence.push_back(current_shop);

    if (current_zone == total_zones) {
        all_forward_paths.emplace_back(current_F, current_G, path_sequence);
        path_sequence.pop_back(); // Backtrack
        return;
    }

    for (const auto& edge : adj_forward[current_shop]) {
        // Ensure the edge leads to the next zone
        if (get_zone_id(edge.v) == current_zone + 1) {
            long long next_F = current_F + (edge.type == 1 ? edge.weight : 0);
            long long next_G = current_G + (edge.type == 2 ? edge.weight : 0);
            generate_forward_paths(edge.v, current_zone + 1, next_F, next_G, path_sequence);
        }
    }
    path_sequence.pop_back(); // Backtrack
}

// DFS for generating backward paths
void generate_backward_paths(int current_shop, int current_zone, long long current_F_prime, long long current_G_prime, vector<int>& path_sequence) {
    path_sequence.push_back(current_shop);

    if (current_zone == 1) {
        // Paths are stored in reverse order from N to 1, so reverse for comparison
        vector<int> reversed_path = path_sequence;
        reverse(reversed_path.begin(), reversed_path.end());
        all_backward_paths.emplace_back(current_F_prime, current_G_prime, reversed_path);
        path_sequence.pop_back(); // Backtrack
        return;
    }

    for (const auto& edge : adj_backward[current_shop]) {
        // Ensure the edge leads to the previous zone
        if (get_zone_id(edge.v) == current_zone - 1) {
            long long next_F_prime = current_F_prime + (edge.type == 1 ? edge.weight : 0);
            long long next_G_prime = current_G_prime + (edge.type == 2 ? edge.weight : 0);
            generate_backward_paths(edge.v, current_zone - 1, next_F_prime, next_G_prime, path_sequence);
        }
    }
    path_sequence.pop_back(); // Backtrack
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, L;
    cin >> N >> M >> L;

    total_zones = L;
    n_shops_in_zone.resize(L + 1);
    zone_start_shop_id.resize(L + 1);
    zone_start_shop_id[0] = 0; // Dummy for easier indexing

    long long current_shop_id_prefix_sum = 0;
    for (int i = 1; i <= L; ++i) {
        cin >> n_shops_in_zone[i];
        current_shop_id_prefix_sum += n_shops_in_zone[i];
        zone_start_shop_id[i] = current_shop_id_prefix_sum;
    }

    // Adjust zone_start_shop_id to be 0-indexed for shops if shops are 1-indexed
    // For example, if shop 1 is in zone 1, and shop 10 in zone 2.
    // get_zone_id should return 1 for shop 1, 2 for shop 10.
    // It's better to store start shop ID for each zone:
    vector<int> shop_start_idx_per_zone(L + 1);
    shop_start_idx_per_zone[1] = 1;
    for(int i = 2; i <= L; ++i) {
        shop_start_idx_per_zone[i] = shop_start_idx_per_zone[i-1] + n_shops_in_zone[i-1];
    }
    // Update get_zone_id to use this:
    // int get_zone_id_from_shop_id(int shop_id) {
    //    for(int i = 1; i <= L; ++i) {
    //        if (shop_id >= shop_start_idx_per_zone[i] && shop_id < (shop_start_idx_per_zone[i] + n_shops_in_zone[i])) {
    //            return i;
    //        }
    //    }
    //    return -1; // Error
    // }
    // A binary search on shop_start_idx_per_zone will be faster for get_zone_id.
    // Let's stick with the current `get_zone_id` using `zone_start_shop_id` which essentially holds prefix sums of n_i.
    // `zone_start_shop_id[i]` represents the last shop ID in zone `i`.
    // So if `shop_id <= zone_start_shop_id[i]` and `shop_id > zone_start_shop_id[i-1]`, then it's in zone `i`.


    adj_forward.resize(N + 1);
    adj_backward.resize(N + 1);

    for (int j = 0; j < M; ++j) {
        int u, v, s;
        long long w;
        cin >> u >> v >> s >> w;
        adj_forward[u].push_back({u, v, s, w});
        adj_backward[v].push_back({v, u, s, w}); // Store reverse edges for backward shopper
    }

    // Generate all forward paths
    vector<int> current_path_sequence_f;
    generate_forward_paths(1, 1, 0, 0, current_path_sequence_f);

    // Generate all backward paths
    vector<int> current_path_sequence_b;
    generate_backward_paths(N, L, 0, 0, current_path_sequence_b);


    long long min_D = -1; // Use -1 to indicate not found, or a very large number

    // Compare all pairs of forward and backward paths
    for (const auto& fp : all_forward_paths) {
        long long F = get<0>(fp);
        long long G = get<1>(fp);
        const vector<int>& path_f_seq = get<2>(fp);

        for (const auto& bp : all_backward_paths) {
            long long F_prime = get<0>(bp);
            long long G_prime = get<1>(bp);
            const vector<int>& path_b_seq = get<2>(bp);

            // Check if paths are 100% identical
            bool paths_identical = true;
            if (path_f_seq.size() != path_b_seq.size()) { // Should always be L
                paths_identical = false; // This case should not happen if logic is correct
            } else {
                for (size_t i = 0; i < path_f_seq.size(); ++i) {
                    if (path_f_seq[i] != path_b_seq[i]) {
                        paths_identical = false;
                        break;
                    }
                }
            }

            if (!paths_identical) {
                long long diff_F = F - F_prime;
                long long diff_G = G - G_prime;
                long long current_D = diff_F * diff_F + diff_G * diff_G;

                if (min_D == -1 || current_D < min_D) {
                    min_D = current_D;
                }
            }
        }
    }

    cout << min_D << endl;

    return 0;
}