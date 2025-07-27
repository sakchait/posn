using System;
using System.Collections.Generic;
using System.Linq;

class Program
{
    const int INF = int.MaxValue;

    static void Main()
    {
        // Read input
        var firstLine = Console.ReadLine().Split().Select(long.Parse).ToArray();
        int N = (int)firstLine[0];
        long V = firstLine[1], W = firstLine[2];

        var x = Console.ReadLine().Split().Select(int.Parse).ToArray();

        // Precompute prefix sums for odd and even prices
        var pref_odd_price = new long[N + 1];
        var pref_even_price = new long[N + 1];

        for (int i = 0; i < N; ++i)
        {
            pref_odd_price[i + 1] = pref_odd_price[i];
            pref_even_price[i + 1] = pref_even_price[i];

            if (x[i] % 2 != 0)
                pref_odd_price[i + 1] += x[i];
            else
                pref_even_price[i + 1] += x[i];
        }

        // dp[i] = minimum number of segments to cover first i stores
        var dp = Enumerable.Repeat(INF, N + 1).ToArray();
        dp[0] = 0;

        for (int i = 2; i <= N; i += 2)
        {
            for (int prev_idx = 0; prev_idx <= i - 2; prev_idx += 2)
            {
                if (dp[prev_idx] == INF)
                    continue;

                int current_segment_len = i - prev_idx;

                long current_a_sum = pref_odd_price[prev_idx + current_segment_len / 2] - pref_odd_price[prev_idx];
                long current_b_sum = pref_even_price[i] - pref_even_price[prev_idx + current_segment_len / 2];

                if (current_a_sum <= V && current_b_sum <= W)
                    dp[i] = Math.Min(dp[i], dp[prev_idx] + 1);
            }
        }

        if (dp[N] == INF)
            Console.WriteLine(-1);
        else
            Console.WriteLine(dp[N]);
    }
}