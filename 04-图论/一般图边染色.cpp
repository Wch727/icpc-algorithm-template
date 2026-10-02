// 一般无向简单图边染色，O(nm)，最多 Δ+1 色，颜色从 0 开始；不允许重边、自环。
// 参考 KACTL EdgeColoring.h，Simon Lindholm，CC0；Misra–Gries 扇形旋转与双色链交换。
// 二分图可做到 Δ 色，本函数不保证；编号 0..n-1，返回与输入边一一对应的颜色。
// 二分图（含重边）只需 Δ 色；本一般简单图算法只保证 Δ+1，不能替代要求 Δ 色的构造。
// 二分图构造可沿两端缺色组成的双色交替链换色；判定/构造模型要分清点染色与边染色。
#include<bits/stdc++.h>
using namespace std;
vector<int> edgeColoring(int N, vector<pair<int,int>> eds) {
    vector<int> cc(N + 1), ret((int)eds.size()), fan(N), free(N), loc;
    for (pair<int,int> e : eds) ++cc[e.first], ++cc[e.second];
    int u, v, ncols = *max_element(cc.begin(),cc.end()) + 1;
    vector<vector<int>> adj(N, vector<int>(ncols, -1));
    for (pair<int,int> e : eds) {
        tie(u, v) = e;
        fan[0] = v;
        loc.assign(ncols, 0);
        int at = u, end = u, d, c = free[u], ind = 0, i = 0;
        while (d = free[v], !loc[d] && (v = adj[u][d]) != -1)
            loc[d] = ++ind, cc[ind] = d, fan[ind] = v;
        cc[loc[d]] = c;
        for (int cd = d; at != -1; cd ^= c ^ d, at = adj[at][cd])
            swap(adj[at][cd], adj[end = at][cd ^ c ^ d]);
        while (adj[fan[i]][d] != -1) {
            int left = fan[i], right = fan[++i], e = cc[i];
            adj[u][e] = left;
            adj[left][e] = u;
            adj[right][e] = -1;
            free[right] = e;
        }
        adj[u][d] = fan[i];
        adj[fan[i]][d] = u;
        for (int y : {fan[0], u, end})
            for (int& z = free[y] = 0; adj[y][z] != -1; z++);
    }
    for(int i=0;i<(int)eds.size();i++)
        for (tie(u, v) = eds[i]; adj[u][ret[i]] != v;) ++ret[i];
    return ret;
}
