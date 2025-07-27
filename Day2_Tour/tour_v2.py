import sys
import threading

def main():
    import sys
    sys.setrecursionlimit(1 << 25)

    N, M, L = map(int, sys.stdin.readline().split())
    total_zones = L
    n_shops_in_zone = [0] * (L + 1)
    zone_start_shop_id = [0] * (L + 2)
    zone_start_shop_id[1] = 1
    current_shop_acc = 1

    n_shops_input = list(map(int, sys.stdin.readline().split()))
    for i in range(1, L + 1):
        n_shops_in_zone[i] = n_shops_input[i - 1]
        if i > 1:
            zone_start_shop_id[i] = current_shop_acc
        current_shop_acc += n_shops_in_zone[i]
    zone_start_shop_id[L + 1] = current_shop_acc

    adj_forward = [[] for _ in range(N + 1)]
    adj_backward = [[] for _ in range(N + 1)]

    for _ in range(M):
        line = sys.stdin.readline()
        while line.strip() == "":
            line = sys.stdin.readline()
        u, v, s, w = line.split()
        u = int(u)
        v = int(v)
        s = int(s)
        w = int(w)
        adj_forward[u].append({'u': u, 'v': v, 'type': s, 'weight': w})
        adj_backward[v].append({'u': v, 'v': u, 'type': s, 'weight': w})

    def get_zone_id(shop_id):
        for i in range(1, len(zone_start_shop_id)):
            if shop_id < zone_start_shop_id[i]:
                return i - 1
        return len(zone_start_shop_id) - 2

    class PathInfo:
        def __init__(self, F_val, G_val, shops_sequence):
            self.F_val = F_val
            self.G_val = G_val
            self.shops_sequence = list(shops_sequence)

    all_forward_paths_info = []
    all_backward_paths_info = []

    def generate_forward_paths(current_shop, current_zone, current_F, current_G, path_sequence):
        path_sequence.append(current_shop)
        if current_zone == total_zones:
            all_forward_paths_info.append(PathInfo(current_F, current_G, path_sequence))
            path_sequence.pop()
            return
        for edge in adj_forward[current_shop]:
            if get_zone_id(edge['v']) == current_zone + 1:
                next_F = current_F + (edge['weight'] if edge['type'] == 1 else 0)
                next_G = current_G + (edge['weight'] if edge['type'] == 2 else 0)
                generate_forward_paths(edge['v'], current_zone + 1, next_F, next_G, path_sequence)
        path_sequence.pop()

    def generate_backward_paths(current_shop, current_zone, current_F_prime, current_G_prime, path_sequence):
        path_sequence.append(current_shop)
        if current_zone == 1:
            reversed_path = list(reversed(path_sequence))
            all_backward_paths_info.append(PathInfo(current_F_prime, current_G_prime, reversed_path))
            path_sequence.pop()
            return
        for edge in adj_backward[current_shop]:
            if get_zone_id(edge['v']) == current_zone - 1:
                next_F_prime = current_F_prime + (edge['weight'] if edge['type'] == 1 else 0)
                next_G_prime = current_G_prime + (edge['weight'] if edge['type'] == 2 else 0)
                generate_backward_paths(edge['v'], current_zone - 1, next_F_prime, next_G_prime, path_sequence)
        path_sequence.pop()

    generate_forward_paths(1, 1, 0, 0, [])
    generate_backward_paths(N, L, 0, 0, [])

    min_D = -1

    # Map for backward path lookup by sequence
    from collections import defaultdict
    backward_path_map = defaultdict(list)
    for bp_info in all_backward_paths_info:
        key = tuple(bp_info.shops_sequence)
        backward_path_map[key].append((bp_info.F_val, bp_info.G_val))

    for fp_info in all_forward_paths_info:
        F = fp_info.F_val
        G = fp_info.G_val
        path_f_seq = tuple(fp_info.shops_sequence)

        for bp_info in all_backward_paths_info:
            F_prime = bp_info.F_val
            G_prime = bp_info.G_val
            path_b_seq = tuple(bp_info.shops_sequence)

            paths_identical = (path_f_seq == path_b_seq)

            if not paths_identical:
                diff_F = F - F_prime
                diff_G = G - G_prime
                current_D = diff_F * diff_F + diff_G * diff_G
                if min_D == -1 or current_D < min_D:
                    min_D = current_D

    print(min_D)

if __name__ == "__main__":
    threading.Thread(target=main).start()