using System;
using System.Collections.Generic;
using System.Linq;

class Program
{
    class Edge
    {
        public int u, v, type;
        public long weight;
        public Edge(int u, int v, int type, long weight)
        {
            this.u = u; this.v = v; this.type = type; this.weight = weight;
        }
    }

    class PathInfo
    {
        public long F_val, G_val;
        public List<int> shops_sequence;
        public PathInfo(long F, long G, List<int> seq)
        {
            F_val = F; G_val = G; shops_sequence = seq;
        }
    }

    static List<List<Edge>> adj_forward;
    static List<List<Edge>> adj_backward;
    static List<int> n_shops_in_zone;
    static List<int> zone_start_shop_id;
    static int total_zones;

    static int GetZoneId(int shop_id)
    {
        for (int i = 1; i < zone_start_shop_id.Count; ++i)
        {
            if (shop_id < zone_start_shop_id[i])
                return i - 1;
        }
        return zone_start_shop_id.Count - 2;
    }

    static List<PathInfo> all_forward_paths_info = new();
    static List<PathInfo> all_backward_paths_info = new();

    static void GenerateForwardPaths(int current_shop, int current_zone, long current_F, long current_G, List<int> path_sequence)
    {
        path_sequence.Add(current_shop);

        if (current_zone == total_zones)
        {
            all_forward_paths_info.Add(new PathInfo(current_F, current_G, new List<int>(path_sequence)));
            path_sequence.RemoveAt(path_sequence.Count - 1);
            return;
        }

        foreach (var edge in adj_forward[current_shop])
        {
            if (GetZoneId(edge.v) == current_zone + 1)
            {
                long next_F = current_F + (edge.type == 1 ? edge.weight : 0);
                long next_G = current_G + (edge.type == 2 ? edge.weight : 0);
                GenerateForwardPaths(edge.v, current_zone + 1, next_F, next_G, path_sequence);
            }
        }
        path_sequence.RemoveAt(path_sequence.Count - 1);
    }

    static void GenerateBackwardPaths(int current_shop, int current_zone, long current_F_prime, long current_G_prime, List<int> path_sequence)
    {
        path_sequence.Add(current_shop);

        if (current_zone == 1)
        {
            var reversed_path = new List<int>(path_sequence);
            reversed_path.Reverse();
            all_backward_paths_info.Add(new PathInfo(current_F_prime, current_G_prime, reversed_path));
            path_sequence.RemoveAt(path_sequence.Count - 1);
            return;
        }

        foreach (var edge in adj_backward[current_shop])
        {
            if (GetZoneId(edge.v) == current_zone - 1)
            {
                long next_F_prime = current_F_prime + (edge.type == 1 ? edge.weight : 0);
                long next_G_prime = current_G_prime + (edge.type == 2 ? edge.weight : 0);
                GenerateBackwardPaths(edge.v, current_zone - 1, next_F_prime, next_G_prime, path_sequence);
            }
        }
        path_sequence.RemoveAt(path_sequence.Count - 1);
    }

    static void Main()
    {
        string firstLineRaw = Console.ReadLine();
        while (string.IsNullOrWhiteSpace(firstLineRaw))
            firstLineRaw = Console.ReadLine();
        var firstLine = firstLineRaw.Split().Select(int.Parse).ToArray();
        int N = firstLine[0], M = firstLine[1], L = firstLine[2];
        total_zones = L;
        n_shops_in_zone = new List<int>(new int[L + 1]);
        zone_start_shop_id = new List<int>(new int[L + 2]);
        zone_start_shop_id[1] = 1;
        long current_shop_acc = 1;
        string nShopsLineRaw = Console.ReadLine();
        while (string.IsNullOrWhiteSpace(nShopsLineRaw))
            nShopsLineRaw = Console.ReadLine();
        var n_shops_input = nShopsLineRaw.Split().Select(int.Parse).ToArray();
        for (int i = 1; i <= L; ++i)
        {
            n_shops_in_zone[i] = n_shops_input[i - 1];
            if (i > 1)
                zone_start_shop_id[i] = (int)current_shop_acc;
            current_shop_acc += n_shops_in_zone[i];
        }
        zone_start_shop_id[L + 1] = (int)current_shop_acc;

        adj_forward = new List<List<Edge>>(Enumerable.Range(0, N + 1).Select(_ => new List<Edge>()));
        adj_backward = new List<List<Edge>>(Enumerable.Range(0, N + 1).Select(_ => new List<Edge>()));

        for (int j = 0; j < M; ++j)
        {
            var edgeLine = Console.ReadLine().Split();
            int u = int.Parse(edgeLine[0]);
            int v = int.Parse(edgeLine[1]);
            int s = int.Parse(edgeLine[2]);
            long w = long.Parse(edgeLine[3]);
            adj_forward[u].Add(new Edge(u, v, s, w));
            adj_backward[v].Add(new Edge(v, u, s, w));
        }

        var current_path_sequence_f = new List<int>();
        GenerateForwardPaths(1, 1, 0, 0, current_path_sequence_f);

        var current_path_sequence_b = new List<int>();
        GenerateBackwardPaths(N, L, 0, 0, current_path_sequence_b);

        long min_D = -1;

        // Map for backward path lookup by sequence
        var backward_path_map = new Dictionary<string, List<(long, long)>>();
        foreach (var bp_info in all_backward_paths_info)
        {
            string key = string.Join(",", bp_info.shops_sequence);
            if (!backward_path_map.ContainsKey(key))
                backward_path_map[key] = new List<(long, long)>();
            backward_path_map[key].Add((bp_info.F_val, bp_info.G_val));
        }

        foreach (var fp_info in all_forward_paths_info)
        {
            long F = fp_info.F_val;
            long G = fp_info.G_val;
            var path_f_seq = fp_info.shops_sequence;
            string key_f = string.Join(",", path_f_seq);

            foreach (var bp_info in all_backward_paths_info)
            {
                long F_prime = bp_info.F_val;
                long G_prime = bp_info.G_val;
                var path_b_seq = bp_info.shops_sequence;
                string key_b = string.Join(",", path_b_seq);

                bool paths_identical = key_f == key_b;

                if (!paths_identical)
                {
                    long diff_F = F - F_prime;
                    long diff_G = G - G_prime;
                    long current_D = diff_F * diff_F + diff_G * diff_G;

                    if (min_D == -1 || current_D < min_D)
                        min_D = current_D;
                }
            }
        }

        Console.WriteLine(min_D);
    }
}