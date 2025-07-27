using System;
using System.Collections.Generic;
using System.Linq;

class Program
{
    static void Main()
    {
        // Read input
        var firstLine = Console.ReadLine().Split().Select(int.Parse).ToArray();
        int N = firstLine[0], L = firstLine[1];

        var secondLine = Console.ReadLine().Split().Select(int.Parse).ToArray();
        int a = secondLine[0], b = secondLine[1];

        var weights = Console.ReadLine().Split().Select(long.Parse).ToArray();

        // Use SortedSet with custom comparer to mimic ordered multiset with unique id
        var orbs = new SortedSet<(long weight, int id)>(Comparer<(long, int)>.Create(
            (x, y) => x.weight != y.weight ? x.weight.CompareTo(y.weight) : x.id.CompareTo(y.id)
        ));

        int currentId = 0;
        foreach (var w in weights)
            orbs.Add((w, currentId++));

        for (int round = 0; round < L; ++round)
        {
            // Get a-th and b-th orb (1-based to 0-based)
            var list = orbs.ToList();
            var orbA = list[a - 1];
            var orbB = list[b - 1];

            long x = orbA.weight;
            long y = orbB.weight;

            orbs.Remove(orbA);
            orbs.Remove(orbB);

            long newWeight1 = y - x;
            long newWeight2 = (long)Math.Floor((x + y) / 2.0);

            orbs.Add((newWeight1, currentId++));
            orbs.Add((newWeight2, currentId++));
        }

        // Output sorted weights
        Console.WriteLine(string.Join(" ", orbs.Select(o => o.weight)));
    }
}