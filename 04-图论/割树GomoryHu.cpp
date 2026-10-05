// 无向非负容量图的割树：n-1 次最大流，编号 0..n-1；每对最小割是树路径最小边。
// 返回 {u,v,割值} 的 n-1 条边；不连通时允许 0 权边。每轮重建残量图。
// 内置 vector Dinic；流量总和须放入 ll，递归推流深度 O(n)。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
struct CutDinic
{
    struct E{int v,rev;ll c;};vector<vector<E>> g;vector<int> d,it;
    CutDinic(int n):g(n),d(n),it(n){}
    void add(int u,int v,ll c){if(u==v)return;int a=g[u].size(),b=g[v].size();g[u].push_back({v,b,c});g[v].push_back({u,a,c});}
    bool bfs(int s,int t){fill(d.begin(),d.end(),-1);queue<int> q;d[s]=0;q.push(s);while(!q.empty()){int u=q.front();q.pop();for(auto e:g[u])if(e.c&&d[e.v]<0)d[e.v]=d[u]+1,q.push(e.v);}return d[t]>=0;}
    ll dfs(int u,int t,ll f){if(u==t)return f;for(int &i=it[u];i<(int)g[u].size();i++){auto &e=g[u][i];if(e.c&&d[e.v]==d[u]+1){ll z=dfs(e.v,t,min(f,e.c));if(z){e.c-=z;g[e.v][e.rev].c+=z;return z;}}}return 0;}
    ll flow(int s,int t){ll ans=0;while(bfs(s,t)){fill(it.begin(),it.end(),0);while(ll z=dfs(s,t,LLONG_MAX/4))ans+=z;}return ans;}
};
int n;
vector<array<ll,3>> edges;
vector<array<ll,3>> gomory_hu()
{
    vector<int> p(n);vector<array<ll,3>> tree;
    for(int s=1;s<n;s++)
    {
        CutDinic f(n);for(auto [u,v,c]:edges)f.add(u,v,c);
        ll cut=f.flow(s,p[s]);tree.push_back({s,p[s],cut});
        for(int j=s+1;j<n;j++)if(p[j]==p[s]&&f.d[j]>=0)p[j]=s;
    }
    return tree;
}
