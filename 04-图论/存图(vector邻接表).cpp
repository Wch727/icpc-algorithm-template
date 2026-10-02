#include<bits/stdc++.h>
using namespace std;
const int N=100005;
int n,m;
struct Edge{int to,w;};
vector<Edge> adj[N];
void add_edge(int u,int v,int w){adj[u].push_back({v,w});}
void add_undirected(int u,int v,int w){add_edge(u,v,w);add_edge(v,u,w);}
// 遍历：for(auto [v,w]:adj[u])；重建：for(int u=1;u<=n;u++)adj[u].clear();
// 无向边存两次；要区分重边/反向边时在 Edge 里增加 id。
