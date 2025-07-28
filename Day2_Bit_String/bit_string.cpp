#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm> // For std::max
#include <climits>   // For LLONG_MIN

using namespace std;

int N_len; // Length of bit strings
map<unsigned int, long long> pattern_weights; // Stores weights for each bit string (as integer)
map<unsigned int, long long> memo; // Memoization table for DP

// Converts a bit string (std::string) to its unsigned integer representation.
// Assumes the string is read from left to right, where s[0] is the MSB.
unsigned int bitstring_to_uint(const string& s) {
    unsigned int val = 0;
    for (int i = 0; i < N_len; ++i) {
        if (s[i] == '1') {
            val |= (1U << (N_len - 1 - i)); // Set the corresponding bit from MSB to LSB
        }
    }
    return val;
}

// Dynamic programming function to calculate the maximum quality degradation value
long long calculate_degradation(unsigned int current_int_bitstring) {
    // Check memoization table
    if (memo.count(current_int_bitstring)) {
        return memo[current_int_bitstring];
    }

    // Base case: If the bit string is all zeros, the degradation process ends.
    // The problem states the pattern of all 0s has a weight of 0.
    if (current_int_bitstring == 0) {
        return 0;
    }

    // Get the weight of the current pattern (bit string)
    long long current_pattern_weight = pattern_weights[current_int_bitstring];

    // Initialize max_next_degradation to LLONG_MIN to correctly find the maximum,
    // even if all subsequent paths lead to negative sums.
    long long max_next_degradation = LLONG_MIN;

    // Rule 2.a: Explore changing a single '1' to '0'
    for (int i = 0; i < N_len; ++i) {
        // Check if the i-th bit (from the right, LSB) is '1'
        if ((current_int_bitstring >> i) & 1) {
            unsigned int next_int_bitstring = current_int_bitstring ^ (1U << i); // Flip the i-th bit to '0'
            max_next_degradation = max(max_next_degradation, calculate_degradation(next_int_bitstring));
        }
    }

    // Rule 2.b: Explore changing "11" (adjacent '1's) to "00"
    // Iterate from right to left (LSB to MSB) for adjacent bits (i and i+1)
    for (int i = 0; i < N_len - 1; ++i) {
        // Check if both the i-th and (i+1)-th bits are '1'
        if (((current_int_bitstring >> i) & 1) && ((current_int_bitstring >> (i + 1)) & 1)) {
            // Flip both bits to '0'. (3U << i) represents 0b11 shifted by i positions.
            unsigned int next_int_bitstring = current_int_bitstring ^ (3U << i); 
            max_next_degradation = max(max_next_degradation, calculate_degradation(next_int_bitstring));
        }
    }

    // The maximum degradation for the current bit string is its own weight plus the maximum
    // degradation achievable from any of the next possible states.
    // Since the problem guarantees that degradation continues until all zeros, and any '1' can be flipped,
    // max_next_degradation will always be updated from LLONG_MIN to a valid sum.
    memo[current_int_bitstring] = current_pattern_weight + max_next_degradation;
    return memo[current_int_bitstring];
}

int main() {
    // Optimize C++ standard streams for faster input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int Q_queries;
    cin >> N_len >> Q_queries; // Read N (length) and Q (number of queries) 

    // Read all 2^N patterns and their weights 
    for (int i = 0; i < (1 << N_len); ++i) {
        string s;
        long long weight;
        cin >> s >> weight;
        pattern_weights[bitstring_to_uint(s)] = weight;
    }

    // Process Q queries 
    for (int i = 0; i < Q_queries; ++i) {
        string query_s;
        cin >> query_s;
        unsigned int query_uint = bitstring_to_uint(query_s);
        // Calculate and print the max degradation value for each query [cite: 25]
        cout << calculate_degradation(query_uint) << "\n";
    }

    return 0;
}