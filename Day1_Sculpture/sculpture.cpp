#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set> // For unique support positions

const long long INF_LL = 4e18; // A sufficiently large number for infinity

// Segment Tree Node structure
struct Node {
    int max_len;
    long long min_sum_weight;

    // Default constructor for an empty range in segment tree
    Node(int l = 0, long long s = 0) : max_len(l), min_sum_weight(s) {}

    // Merge function for segment tree:
    // Prioritize higher max_len. If max_len is equal, prioritize lower min_sum_weight.
    static Node merge(const Node& left, const Node& right) {
        if (left.max_len > right.max_len) {
            return left;
        }
        if (right.max_len > left.max_len) {
            return right;
        }
        // If lengths are equal, choose the one with minimum sum weight
        return Node(left.max_len, std::min(left.min_sum_weight, right.min_sum_weight));
    }
};

std::vector<Node> seg_tree;
std::vector<long long> all_coords; // Stores unique sorted weights/capacities for coordinate compression
std::map<long long, int> coord_map; // Maps actual value to compressed index

// Builds the segment tree: initializes all nodes to default (0 length, 0 sum)
void build(int node_idx, int start, int end) {
    if (start == end) {
        seg_tree[node_idx] = Node(0, 0); 
    } else {
        int mid = (start + end) / 2;
        build(2 * node_idx, start, mid);
        build(2 * node_idx + 1, mid + 1, end);
        seg_tree[node_idx] = Node(0, 0); // Parent node also initialized
    }
}

// Updates the segment tree at a specific compressed index with a new Node value.
// The update rule follows the Node::merge logic.
void update(int node_idx, int start, int end, int idx_to_update, Node new_val) {
    if (start == end) {
        seg_tree[node_idx] = Node::merge(seg_tree[node_idx], new_val);
    } else {
        int mid = (start + end) / 2;
        if (start <= idx_to_update && idx_to_update <= mid) {
            update(2 * node_idx, start, mid, idx_to_update, new_val);
        } else {
            update(2 * node_idx + 1, mid + 1, end, idx_to_update, new_val);
        }
        seg_tree[node_idx] = Node::merge(seg_tree[2 * node_idx], seg_tree[2 * node_idx + 1]);
    }
}

// Queries the segment tree for the best Node (max_len, min_sum_weight) in a given range [l, r].
Node query(int node_idx, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return Node(0, 0); // Outside query range, return default empty node
    }
    if (l <= start && end <= r) {
        return seg_tree[node_idx]; // Node completely within query range
    }
    int mid = (start + end) / 2;
    Node p1 = query(2 * node_idx, start, mid, l, r);
    Node p2 = query(2 * node_idx + 1, mid + 1, end, l, r);
    return Node::merge(p1, p2);
}

// Structure to hold stone properties
struct Stone {
    int w, L;
};

int main() {
    // Optimize C++ streams for faster input/output
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N;
    std::cin >> N;

    std::vector<Stone> stones(N + 1); // 1-indexed for convenience

    // Collect all weights (w_i) and capacities (L_i) for coordinate compression
    for (int i = 1; i <= N; ++i) {
        std::cin >> stones[i].w >> stones[i].L;
        all_coords.push_back(stones[i].w);
        all_coords.push_back(stones[i].L);
    }

    // Sort and remove duplicates from collected coordinates
    std::sort(all_coords.begin(), all_coords.end());
    all_coords.erase(std::unique(all_coords.begin(), all_coords.end()), all_coords.end());

    // Create a map from actual value to its compressed index
    for (int i = 0; i < all_coords.size(); ++i) {
        coord_map[all_coords[i]] = i;
    }

    int K = all_coords.size(); // Number of unique coordinates (size of compressed range)

    // Left DP: Left[i] stores (max_length, min_sum_weight) of a stack
    // where stone 'i' is the bottommost stone, considering stones 1 to i.
    std::vector<std::pair<int, long long>> Left(N + 1);
    
    seg_tree.assign(4 * K, Node()); // Initialize segment tree
    build(1, 0, K - 1); // Explicitly build to set default Node(0,0)

    for (int i = 1; i <= N; ++i) {
        // Query segment tree for the best stack (max length, min sum) that can be placed *on top of* stone 'i'.
        // This means total weight of that stack must be <= stones[i].L.
        // Query range from 0 to coord_map[stones[i].L] (inclusive).
        Node res = query(1, 0, K - 1, 0, coord_map[stones[i].L]);
        
        Left[i].first = res.max_len + 1; // Add stone 'i' to the stack
        Left[i].second = res.min_sum_weight + stones[i].w; // Update total weight

        // Update segment tree with the stack ending at 'i'.
        // The segment tree is indexed by the total weight of the stack it represents.
        // Only update if the total weight of the stack is within the range of possible L_i values.
        // If it's too heavy, no subsequent stone can be placed below it anyway.
        if (Left[i].second <= all_coords.back() && coord_map.count(Left[i].second)) {
             update(1, 0, K - 1, coord_map[Left[i].second], Node(Left[i].first, Left[i].second));
        } else if (Left[i].second > all_coords.back()) {
            // If Left[i].second is greater than max coordinate, map it to the largest available index.
            // This is a common way to handle values outside the compressed range.
            // However, since such heavy stacks cannot be placed, it's safer not to update for them at all.
            // The condition `Left[i].second <= all_coords.back()` handles this correctly.
        } else {
             // If Left[i].second is not in coord_map (e.g. 0), it might be an issue. But min_sum_weight will be 0 for length 0.
             // If Left[i].second becomes 0, and coord_map doesn't contain 0, it might error.
             // Assuming w_i >= 1, Left[i].second will always be >= 1. So coord_map will have it.
        }
    }

    // Right DP: Right[i] stores (max_length, min_sum_weight) of a stack
    // where stone 'i' is the topmost stone, considering stones i to N.
    // Right DP: Right[i] stores (max_length, min_sum_weight) of a stack
    // where stone 'i' is the topmost stone, considering stones i to N.
    std::vector<std::pair<int, long long>> Right(N + 2); // 1-indexed, N+2 to avoid out-of-bounds

    seg_tree.assign(4 * K, Node()); // Re-initialize segment tree for Right DP
    build(1, 0, K - 1); // Explicitly build

    for (int i = N; i >= 1; --i) {
        // Query segment tree for the best stack that can be placed *below* stone 'i'.
        // This means total weight of that stack must be <= stones[i].L.
        Node res = query(1, 0, K - 1, 0, coord_map[stones[i].L]);
        
        Right[i].first = res.max_len + 1; // Add stone 'i' to the top of this stack
        Right[i].second = res.min_sum_weight + stones[i].w; // Update total weight

        // Update segment tree with the stack starting at 'i'.
        if (Right[i].second <= all_coords.back() && coord_map.count(Right[i].second)) {
            update(1, 0, K - 1, coord_map[Right[i].second], Node(Right[i].first, Right[i].second));
        }
    }

    // Find Max M and Collect Support Positions
    int max_M = 0;
    std::set<int> support_positions_set; // Use a set for unique and sorted positions

    // Step 1: Find max_M from single-segment stacks (no support device used)
    for (int i = 1; i <= N; ++i) {
        max_M = std::max(max_M, Left[i].first);
    }

    // Step 2: Find max_M from two-segment stacks (with support device at j)
    for (int j = 1; j < N; ++j) { // Support device cannot be on the bottommost stone
        // Both parts of the stack must be valid (length > 0)
        if (Left[j].first > 0 && Right[j+1].first > 0) {
            int current_M = Left[j].first + Right[j+1].first;
            max_M = std::max(max_M, current_M);
        }
    }
    
    // Step 3: Collect all support positions 'j' that achieve the overall max_M
    for (int j = 1; j < N; ++j) {
        if (Left[j].first > 0 && Right[j+1].first > 0) {
            if (Left[j].first + Right[j+1].first == max_M) {
                support_positions_set.insert(j);
            }
        }
    }

    // Output results
    std::cout << max_M << "\n";
    if (support_positions_set.empty()) {
        std::cout << "0\n"; // If no support position achieves max_M
    } else {
        std::cout << support_positions_set.size();
        for (int pos : support_positions_set) {
            std::cout << " " << pos;
        }
        std::cout << "\n";
    }

    return 0;
}