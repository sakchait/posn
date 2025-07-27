using System;
using System.Collections.Generic;
using System.Linq;

class Program
{
    const long INF_LL = (long)4e18;

    // Segment Tree Node structure
    class Node
    {
        public int max_len;
        public long min_sum_weight;

        public Node(int l = 0, long s = 0)
        {
            max_len = l;
            min_sum_weight = s;
        }

        // Merge function for segment tree
        public static Node Merge(Node left, Node right)
        {
            if (left.max_len > right.max_len) return left;
            if (right.max_len > left.max_len) return right;
            return new Node(left.max_len, Math.Min(left.min_sum_weight, right.min_sum_weight));
        }
    }

    static List<Node> seg_tree;
    static List<long> all_coords = new List<long>();
    static Dictionary<long, int> coord_map = new Dictionary<long, int>();

    static void Build(int node_idx, int start, int end)
    {
        if (start == end)
        {
            seg_tree[node_idx] = new Node(0, 0);
        }
        else
        {
            int mid = (start + end) / 2;
            Build(2 * node_idx, start, mid);
            Build(2 * node_idx + 1, mid + 1, end);
            seg_tree[node_idx] = new Node(0, 0);
        }
    }

    static void Update(int node_idx, int start, int end, int idx_to_update, Node new_val)
    {
        if (start == end)
        {
            seg_tree[node_idx] = Node.Merge(seg_tree[node_idx], new_val);
        }
        else
        {
            int mid = (start + end) / 2;
            if (start <= idx_to_update && idx_to_update <= mid)
                Update(2 * node_idx, start, mid, idx_to_update, new_val);
            else
                Update(2 * node_idx + 1, mid + 1, end, idx_to_update, new_val);
            seg_tree[node_idx] = Node.Merge(seg_tree[2 * node_idx], seg_tree[2 * node_idx + 1]);
        }
    }

    static Node Query(int node_idx, int start, int end, int l, int r)
    {
        if (r < start || end < l)
            return new Node(0, 0);
        if (l <= start && end <= r)
            return seg_tree[node_idx];
        int mid = (start + end) / 2;
        Node p1 = Query(2 * node_idx, start, mid, l, r);
        Node p2 = Query(2 * node_idx + 1, mid + 1, end, l, r);
        return Node.Merge(p1, p2);
    }

    class Stone
    {
        public int w, L;
        public Stone(int w, int L) { this.w = w; this.L = L; }
    }

    static void Main()
    {
        var tokens = Console.ReadLine().Split().Select(int.Parse).ToArray();
        int N = tokens[0];

        var stones = new Stone[N + 1]; // 1-indexed
        all_coords.Clear();
        coord_map.Clear();

        for (int i = 1; i <= N; ++i)
        {
            var line = Console.ReadLine().Split().Select(int.Parse).ToArray();
            stones[i] = new Stone(line[0], line[1]);
            all_coords.Add(stones[i].w);
            all_coords.Add(stones[i].L);
        }

        all_coords = all_coords.Distinct().OrderBy(x => x).ToList();
        for (int i = 0; i < all_coords.Count; ++i)
            coord_map[all_coords[i]] = i;
        int K = all_coords.Count;

        // Left DP
        var Left = new (int, long)[N + 1];
        seg_tree = Enumerable.Repeat(new Node(), 4 * K).ToList();
        Build(1, 0, K - 1);

        for (int i = 1; i <= N; ++i)
        {
            Node res = Query(1, 0, K - 1, 0, coord_map[stones[i].L]);
            Left[i].Item1 = res.max_len + 1;
            Left[i].Item2 = res.min_sum_weight + stones[i].w;

            if (Left[i].Item2 <= all_coords[K - 1] && coord_map.ContainsKey(Left[i].Item2))
                Update(1, 0, K - 1, coord_map[Left[i].Item2], new Node(Left[i].Item1, Left[i].Item2));
        }

        // Right DP
        var Right = new (int, long)[N + 2];
        seg_tree = Enumerable.Repeat(new Node(), 4 * K).ToList();
        Build(1, 0, K - 1);

        for (int i = N; i >= 1; --i)
        {
            Node res = Query(1, 0, K - 1, 0, coord_map[stones[i].L]);
            Right[i].Item1 = res.max_len + 1;
            Right[i].Item2 = res.min_sum_weight + stones[i].w;

            if (Right[i].Item2 <= all_coords[K - 1] && coord_map.ContainsKey(Right[i].Item2))
                Update(1, 0, K - 1, coord_map[Right[i].Item2], new Node(Right[i].Item1, Right[i].Item2));
        }

        // Find Max M and Collect Support Positions
        int max_M = 0;
        var support_positions_set = new SortedSet<int>();

        // Step 1: Find max_M from single-segment stacks (no support device used)
        for (int i = 1; i <= N; ++i)
            max_M = Math.Max(max_M, Left[i].Item1);

        // Step 2: Find max_M from two-segment stacks (with support device at j)
        for (int j = 1; j < N; ++j)
        {
            if (Left[j].Item1 > 0 && Right[j + 1].Item1 > 0)
            {
                int current_M = Left[j].Item1 + Right[j + 1].Item1;
                max_M = Math.Max(max_M, current_M);
            }
        }

        // Step 3: Collect all support positions 'j' that achieve the overall max_M
        for (int j = 1; j < N; ++j)
        {
            if (Left[j].Item1 > 0 && Right[j + 1].Item1 > 0)
            {
                if (Left[j].Item1 + Right[j + 1].Item1 == max_M)
                    support_positions_set.Add(j);
            }
        }

        // Output results
        Console.WriteLine(max_M);
        if (support_positions_set.Count == 0)
        {
            Console.WriteLine("0");
        }
        else
        {
            Console.Write(support_positions_set.Count);
            foreach (var pos in support_positions_set)
                Console.Write(" " + pos);
            Console.WriteLine();
        }
    }
}