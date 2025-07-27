using System;
using System.Collections.Generic;

class RouteSolver
{
    // Main route function
    public static List<(int, int)> Route(int N, List<int> W)
    {
        int M = W.Count;
        var result_routes = new List<(int, int)>(new (int, int)[M]);
        var assigned_edges = new HashSet<(int, int)>();

        // Phase 1: Assign N-1 most expensive cars/edges to form a simple path (1-2, 2-3, ..., N-1-N).
        for (int i = 0; i < N - 1; ++i)
        {
            int u = i + 1;
            int v = i + 2;
            int idx = M - (N - 1) + i;
            result_routes[idx] = (u, v);
            assigned_edges.Add((Math.Min(u, v), Math.Max(u, v)));
        }

        // Phase 2: Assign the remaining M - (N-1) cheaper cars/edges to distinct, valid (u,v) pairs.
        int u_gen = 1;
        int v_gen = 1;

        for (int i = 0; i < M - (N - 1); ++i)
        {
            int car_index_for_other_edges = i;

            while (true)
            {
                v_gen++;
                if (v_gen > N)
                {
                    u_gen++;
                    v_gen = u_gen + 1;
                }
                if (u_gen > N || v_gen > N)
                {
                    break;
                }

                var current_pair = (u_gen, v_gen);

                if (!assigned_edges.Contains(current_pair))
                {
                    result_routes[car_index_for_other_edges] = current_pair;
                    assigned_edges.Add(current_pair);
                    break;
                }
            }
        }

        return result_routes;
    }
}