"""Small independent state enumeration for paired-PDF derivations."""
from collections import deque, Counter
from itertools import product
from math import comb, prod
from fractions import Fraction


def compositions(total, n):
    if n == 1:
        yield (total,)
    else:
        for x in range(total + 1):
            for tail in compositions(total - x, n - 1):
                yield (x,) + tail


def stone_reachable(a):
    n, total = len(a), sum(a)
    seen, q = {a}, deque([a])
    while q:
        b = q.popleft()
        bag = total - sum(b)
        for i in range(n):
            for delta in (-1, n):
                if (delta == -1 and b[i] == 0) or (delta == n and bag < n):
                    continue
                c = b[:i] + (b[i] + delta,) + b[i + 1:]
                if c not in seen:
                    seen.add(c)
                    q.append(c)
    return seen


def stone_predicate(a, b):
    n, total = len(a), sum(a)
    r = tuple(x % n for x in b)
    return sum(b) <= total and (all(x <= y for x, y in zip(r, a))
                               or total - sum(r) >= n - max(r))


def stone_layer_count(a):
    n, total = len(a), sum(a)
    ans = 0
    for r in product(range(n), repeat=n):
        rs, p = sum(r), max(r)
        if rs > total:
            continue
        m = (total - rs) // n
        if m >= 1:
            ans += comb(m + n, n)
        elif rs <= total - n + p:
            ans += 1
        elif all(x <= y for x, y in zip(r, a)):
            ans += 1
    return ans


def partitions(n, upper=None):
    if n == 0:
        yield ()
    else:
        for first in range(min(n, upper or n), 0, -1):
            for rest in partitions(n - first, first):
                yield (first,) + rest


def cells(p):
    return {(r, c) for r, width in enumerate(p) for c in range(width)}


def diagonal(p):
    out = [0] * (sum(p) + 1)
    for r, c in cells(p):
        out[r + c] += 1
    return tuple(out)


def contour_count(p):
    t = (0,) + diagonal(p) + (0,)
    d = [t[i] - t[i + 1] for i in range(len(t) - 1)]
    assert all(x >= -1 for x in d)
    return prod(comb(d[i - 1] + d[i] + 1, d[i])
                for i in range(1, len(d)) if d[i] >= 0)


def young_reachable(p):
    q, seen = deque([p]), {p}
    while q:
        cur = q.popleft()
        old = cells(cur)
        for r, c in old:
            transformed = {(r + y - c, c + x - r) if x >= r and y >= c
                           else (x, y) for x, y in old}
            height = max(x for x, y in transformed) + 1
            widths = tuple(sum(x == row for x, y in transformed)
                           for row in range(height))
            if any(x == 0 for x in widths) or any(x < y for x, y in zip(widths, widths[1:])):
                continue
            if cells(widths) != transformed:
                continue
            if widths not in seen:
                seen.add(widths)
                q.append(widths)
    return seen


def xor_sum_count(target, upper, k):
    bound = k * k // 4
    dp = {(k, 0): 1}
    for b in reversed(range(max(target.bit_length(), upper.bit_length(), 1))):
        new = Counter()
        for (u, carry), ways in dp.items():
            for ones in range(k + 1):
                lower_carry = ((target >> b) & 1) + 2 * carry - ones * (k - ones)
                if not 0 <= lower_carry <= bound:
                    continue
                if not ((upper >> b) & 1):
                    if ones <= k - u:
                        new[u, lower_carry] += ways * comb(k - u, ones)
                else:
                    for j in range(u + 1):
                        if 0 <= ones - j <= k - u:
                            new[j, lower_carry] += ways * comb(u, j) * comb(k - u, ones - j)
        dp = new
    return sum(ways for (u, carry), ways in dp.items() if carry == 0)


def lower_hull(points):
    best = {}
    for x, y in points:
        best[x] = min(best.get(x, y), y)
    out = []
    for p in sorted(best.items()):
        while len(out) >= 2:
            a, b = out[-2:]
            cross = (b[0] - a[0]) * (p[1] - b[1]) - (b[1] - a[1]) * (p[0] - b[0])
            if cross > 0:
                break
            out.pop()
        out.append(p)
    return out


def budget_hull(points, a, b, budget):
    hull = lower_hull(points)
    if a == 0:
        return Fraction(0)
    for p, q in zip(hull, hull[1:]):
        c, d = a * p[0] + b * p[1], a * q[0] + b * q[1]
        if c <= budget < d:
            return a * (p[0] + Fraction(budget - c, d - c) * (q[0] - p[0]))
    return Fraction(a * hull[-1][0])


def budget_pairs(points, a, b, budget):
    # Independent LP vertices: slack is represented by the empty operation.
    vals = [(a * x + b * y, a * x) for x, y in points]
    ans = max(Fraction(v) for c, v in vals if c <= budget)
    for c, v in vals:
        for d, w in vals:
            if c <= budget < d:
                ans = max(ans, v + Fraction(budget - c, d - c) * (w - v))
    return ans


def main():
    cases = 0
    for n in range(1, 5):
        for total in range(7):
            for a in compositions(total, n):
                reachable = stone_reachable(a)
                predicted = {b for s in range(total + 1)
                             for b in compositions(s, n) if stone_predicate(a, b)}
                assert reachable == predicted, (a, reachable ^ predicted)
                assert stone_layer_count(a) == len(reachable), a
                cases += 1
    assert len(stone_reachable((1, 3))) == 15
    assert len(stone_reachable((2, 1, 3))) == 83
    print(f'Stonebag: {cases} initial states, predicate + disjoint layers PASS')
    count = 0
    for size in range(1, 13):
        ps = list(partitions(size))
        frequencies = Counter(diagonal(p) for p in ps)
        for p in ps:
            assert contour_count(p) == frequencies[diagonal(p)], p
            if size <= 10:
                assert young_reachable(p) == {v for v in ps if diagonal(v) == diagonal(p)}, p
            count += 1
    print(f'Young diagram: {count} partitions, contour count PASS; all operations through area 10 PASS')
    groups = 0
    for upper in range(5):
        for k in range(1, 5):
            direct = Counter(sum(x[i] ^ x[j] for i in range(k) for j in range(i))
                             for x in product(range(upper + 1), repeat=k))
            for target in range(max(direct) + 3):
                assert xor_sum_count(target, upper, k) == direct[target], (target, upper, k)
            groups += 1
    assert xor_sum_count(6, 2, 3) == 12
    assert xor_sum_count(30, 6, 5) == 1520
    print(f'XOR Sum: {groups} full value distributions and both samples PASS')
    graphs = [[], [(0, 0)], [(0, 0), (1, 0), (2, 0)],
              [(0, 0), (0, 1), (1, 1), (1, 2), (2, 2)],
              [(u, v) for u in range(3) for v in range(3)]]
    for edges in graphs:
        points = [(mask.bit_count(), len({v for u, v in edges if mask >> u & 1}))
                  for mask in range(8)]
        for a, b, budget in product(range(3), range(3), range(13)):
            assert budget_hull(points, a, b, budget) == budget_pairs(points, a, b, budget)
    print('Bipartite weighting: 5 edge/boundary graphs, all small budgets PASS')


if __name__ == '__main__':
    main()
