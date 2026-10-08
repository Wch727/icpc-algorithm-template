// 无向非负权图，连接 k 个指定终端的最小代价子图；k 小，顶点 0..n-1。
// dp[S][v]：连接终端集合 S、包含 v；子集合并后做多源 Dijkstra。
// O(3^k*n + 2^k*(n+m)log n)，空间 O(2^k*n)；不连通返回 INF，k=0 返回 0。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
vector<vector<pair<int,ll>>> g;
vector<int> terminal;
vector<vector<ll>> dp;
ll steiner_tree()
{
    const ll INF=LLONG_MAX/4;int n=g.size(),k=terminal.size();
    dp.clear();if(!k)return 0;assert(k<25);int lim=1<<k;
    dp.assign(lim,vector<ll>(n,INF));
    for(int i=0;i<k;i++)dp[1<<i][terminal[i]]=0;
    for(int s=1;s<lim;s++)
    {
        for(int a= (s - 1) & s; a; a= (a - 1) & s)
            if(a < (s ^ a))
                for(int v= 0; v < n; v++)
                    dp[s][v]= min(dp[s][v], dp[a][v] + dp[s ^ a][v]);
        priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<pair<ll,int>>> q;
        for(int v= 0; v < n; v++)
            if(dp[s][v] < INF)
                q.push({dp[s][v], v});
        while(!q.empty())
        {
            auto [d,u]=q.top();q.pop();if(d!=dp[s][u])continue;
            for(auto [v, w] : g[u])
                if(w <= INF - d && d + w < dp[s][v])
                    dp[s][v]= d + w, q.push({d + w, v});
        }
    }
    return dp.back()[terminal[0]];
}
