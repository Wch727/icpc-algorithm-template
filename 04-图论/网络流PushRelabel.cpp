// 最高标号 Push–Relabel + gap，vector 存图，0-indexed；O(V² sqrt(E)) 标准界。
// addEdge(u,v,c,rcap=0)，有向边反向初始 0，无向容量可 rcap=c。容量及总流须放入 ll。
// calc(s,t) 仅调用一次，s!=t；边加完后再调用，运行期间 vector 不得扩容。
// leftOfMinCut(a) 表示最后残量图源侧；整理自 KACTL PushRelabel.h，Simon Lindholm，CC0。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
struct PushRelabel {
    struct Edge {
        int dest, back;
        ll f, c;
    };
    vector<vector<Edge>> g;
    vector<ll> ec;
    vector<Edge*> cur;
    vector<vector<int>> hs; vector<int> H;
    PushRelabel(int n) : g(n), ec(n), cur(n), hs(2*n), H(n) {}

    void addEdge(int s, int t, ll cap, ll rcap=0) {
        if (s == t) return;
        g[s].push_back({t, (int)g[t].size(), 0, cap});
        g[t].push_back({s, (int)g[s].size()-1, 0, rcap});
    }

    void addFlow(Edge& e, ll f) {
        Edge &back = g[e.dest][e.back];
        if (!ec[e.dest] && f) hs[H[e.dest]].push_back(e.dest);
        e.f += f; e.c -= f; ec[e.dest] += f;
        back.f -= f; back.c += f; ec[back.dest] -= f;
    }
    ll calc(int s, int t) {
        int v = (int)g.size(); H[s] = v; ec[t] = 1;
        vector<int> co(2*v); co[0] = v-1;
        for(int i=0;i<v;i++) cur[i] = g[i].data();
        for (Edge& e : g[s]) addFlow(e, e.c);

        for (int hi = 0;;) {
            while(hs[hi].empty())
                if(!hi--)
                    return -ec[s];
            int u = hs[hi].back(); hs[hi].pop_back();
            while (ec[u] > 0)  // discharge u
                if (cur[u] == g[u].data() + (int)g[u].size()) {
                    H[u] = 1e9;
                    for(Edge &e : g[u])
                        if(e.c && H[u] > H[e.dest] + 1)
                            H[u]= H[e.dest] + 1, cur[u]= &e;
                    if (++co[H[u]], !--co[hi] && hi < v)
                        for(int i= 0; i < v; i++)
                            if(hi < H[i] && H[i] < v)
                                --co[H[i]], H[i]= v + 1;
                    hi = H[u];
                } else if (cur[u]->c && H[u] == H[cur[u]->dest]+1)
                    addFlow(*cur[u], min(ec[u], cur[u]->c));
                else ++cur[u];
        }
    }
    bool leftOfMinCut(int a) { return H[a] >= (int)g.size(); }
};
