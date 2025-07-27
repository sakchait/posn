using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        // Read input
        var firstLine = Console.ReadLine().Split();
        int N = int.Parse(firstLine[0]);
        int M = int.Parse(firstLine[1]);

        var W = new List<int>();
        var wLine = Console.ReadLine().Split();
        for (int i = 0; i < M; ++i)
            W.Add(int.Parse(wLine[i]));

        // Call Route function
        var resultRoutes = RouteSolver.Route(N, W);

        // Output results
        foreach (var pair in resultRoutes)
            Console.WriteLine($"{pair.Item1} {pair.Item2}");
    }
}