#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm> // For std::min
#include <climits>   // For INT_MAX

using namespace std;

const int INF = INT_MAX; // Represents an unreachable state or infinitely many segments

int main() {
    // Optimize C++ standard streams for faster input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N; // Number of stores 
    long long V, W; // Max allowed expensive period values for คุณคี่ and คุณคู่ 
    cin >> N >> V >> W;

    vector<int> x(N); // Store prices (0-indexed) 
    for (int i = 0; i < N; ++i) {
        cin >> x[i];
    }

    // Precompute prefix sums for odd and even prices.
    // pref_odd_price[k] stores the sum of odd prices in x[0...k-1].
    // pref_even_price[k] stores the sum of even prices in x[0...k-1].
    // Using N+1 size for easier 1-based conceptual indexing for sums.
    vector<long long> pref_odd_price(N + 1, 0);
    vector<long long> pref_even_price(N + 1, 0);

    for (int i = 0; i < N; ++i) {
        pref_odd_price[i + 1] = pref_odd_price[i];
        pref_even_price[i + 1] = pref_even_price[i];

        if (x[i] % 2 != 0) { // If price is odd, add to odd sum 
            pref_odd_price[i + 1] += x[i];
        } else { // If price is even, add to even sum 
            pref_even_price[i + 1] += x[i];
        }
    }

    // dp[i] = minimum number of segments (M) required to cover the first 'i' stores (x[0] to x[i-1]).
    vector<int> dp(N + 1, INF);
    dp[0] = 0; // 0 segments needed to cover 0 stores.

    // Iterate through all possible ending positions 'i' for the current segment.
    // 'i' represents the total number of stores covered so far, from 1 to N.
    // Since each segment length (l_j) must be even and N is even, 'i' must also be even.
    for (int i = 2; i <= N; i += 2) {
        // Iterate through all possible starting positions 'prev_idx' for the current segment.
        // 'prev_idx' represents the total number of stores covered by previous segments (x[0] to x[prev_idx-1]).
        // The current segment starts at x[prev_idx] and ends at x[i-1].
        // 'prev_idx' must also be even, as it's the sum of previous even segment lengths.
        for (int prev_idx = 0; prev_idx <= i - 2; prev_idx += 2) {
            // If the state `dp[prev_idx]` is unreachable, we cannot transition from it.
            if (dp[prev_idx] == INF) {
                continue;
            }

            int current_segment_len = i - prev_idx; // Length of the current segment

            // According to problem rules:
            // คุณคี่ shops from stores [prev_idx] to [prev_idx + current_segment_len / 2 - 1] 
            // คุณคู่ shops from stores [prev_idx + current_segment_len / 2] to [i-1] 

            // Calculate คุณคี่'s sum (odd prices in first half of current segment) 
            long long current_a_sum = pref_odd_price[prev_idx + current_segment_len / 2] - pref_odd_price[prev_idx];

            // Calculate คุณคู่'s sum (even prices in second half of current segment) 
            long long current_b_sum = pref_even_price[i] - pref_even_price[prev_idx + current_segment_len / 2];

            // Check if current segment satisfies the conditions (sums within V and W limits) 
            if (current_a_sum <= V && current_b_sum <= W) {
                // If conditions met, update dp[i] with the minimum segments needed
                dp[i] = min(dp[i], dp[prev_idx] + 1);
            }
        }
    }

    // Output the result. dp[N] contains the minimum M for covering all N stores. 
    if (dp[N] == INF) {
        cout << -1 << "\n"; // No valid plan found 
    } else {
        cout << dp[N] << "\n"; // Minimum M 
    }

    return 0;
}