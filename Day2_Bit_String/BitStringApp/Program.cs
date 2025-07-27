using System;
using System.Collections.Generic;

class Program
{
    static int N_len; // Length of bit strings
    static Dictionary<uint, long> pattern_weights = new Dictionary<uint, long>();
    static Dictionary<uint, long> memo = new Dictionary<uint, long>();

    // Converts a bit string to its unsigned integer representation.
    static uint BitstringToUInt(string s)
    {
        uint val = 0;
        for (int i = 0; i < N_len; ++i)
        {
            if (s[i] == '1')
                val |= (1U << (N_len - 1 - i));
        }
        return val;
    }

    // Dynamic programming function to calculate the maximum quality degradation value
    static long CalculateDegradation(uint currentIntBitstring)
    {
        if (memo.ContainsKey(currentIntBitstring))
            return memo[currentIntBitstring];

        // Base case: all zeros
        if (currentIntBitstring == 0)
            return 0;

        long currentPatternWeight = pattern_weights[currentIntBitstring];
        long maxNextDegradation = long.MinValue;

        // Rule 2.a: Change a single '1' to '0'
        for (int i = 0; i < N_len; ++i)
        {
            if (((currentIntBitstring >> i) & 1) != 0)
            {
                uint nextIntBitstring = currentIntBitstring ^ (1U << i);
                maxNextDegradation = Math.Max(maxNextDegradation, CalculateDegradation(nextIntBitstring));
            }
        }

        // Rule 2.b: Change "11" to "00"
        for (int i = 0; i < N_len - 1; ++i)
        {
            if (((currentIntBitstring >> i) & 1) != 0 && ((currentIntBitstring >> (i + 1)) & 1) != 0)
            {
                uint nextIntBitstring = currentIntBitstring ^ (3U << i);
                maxNextDegradation = Math.Max(maxNextDegradation, CalculateDegradation(nextIntBitstring));
            }
        }

        memo[currentIntBitstring] = currentPatternWeight + maxNextDegradation;
        return memo[currentIntBitstring];
    }

    static void Main()
    {
        var firstLine = Console.ReadLine().Split();
        N_len = int.Parse(firstLine[0]);
        int Q_queries = int.Parse(firstLine[1]);

        // Read all 2^N patterns and their weights
        for (int i = 0; i < (1 << N_len); ++i)
        {
            var parts = Console.ReadLine().Split();
            string s = parts[0];
            long weight = long.Parse(parts[1]);
            pattern_weights[BitstringToUInt(s)] = weight;
        }

        // Process Q queries
        for (int i = 0; i < Q_queries; ++i)
        {
            string query_s = Console.ReadLine();
            uint query_uint = BitstringToUInt(query_s);
            Console.WriteLine(CalculateDegradation(query_uint));
        }
    }
}