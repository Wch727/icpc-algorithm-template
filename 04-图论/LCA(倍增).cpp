#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m,root;
vector<int> adj[N];
int dep[N],fa[N][20];// fa[u][k]：u 往上跳 2^k 步的祖先
int diff[N];// 树上差分数组

void add_edge(int u,int v)
{
    adj[u].push_back(v);
}

void add_undirected(int u,int v)
{
    add_edge(u,v);
    add_edge(v,u);
}

void dfs(int u,int f)// 预处理深度和倍增表，O(n log n)
{
    dep[u]=dep[f]+1;
    fa[u][0]=f;
    for(int k=1;k<20;k++)fa[u][k]=fa[fa[u][k-1]][k-1];
    for(int v:adj[u])
        if(v!=f)dfs(v,u);
}

int lca(int x,int y)// 倍增求 LCA，O(log n)
{
    if(dep[x]<dep[y])swap(x,y);
    int d=dep[x]-dep[y];
    for(int k= 0; k < 20; k++)
        if(d >> k & 1)
            x= fa[x][k]; // 先把深的提到同一层
    if(x==y)return x;
    for(int k=19;k>=0;k--)
        if(fa[x][k]!=fa[y][k])x=fa[x][k],y=fa[y][k];
    return fa[x][0];
}

int dist(int x,int y)// 树上两点距离（边权为 1，带权就把 dep 换成前缀和）
{
    int f=lca(x,y);
    return dep[x]+dep[y]-2*dep[f];
}

void add_path(int x,int y)// 路径 (x,y) 差分打标记
{
    int f=lca(x,y);
    diff[x]++,diff[y]++,diff[f]-=2;// 边差分：点上减 2 次
}

void collect(int u,int f)// 自底向上合并差分，得到每条边被覆盖的次数
{
    for(int v:adj[u])
        if(v!=f)
        {
            collect(v,u);
            diff[u]+=diff[v];
        }
}
