#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
// 链式前向星：head[u] 为 u 的第一条边，nxt 串起同起点的边
int head[N],to[N<<1],w[N<<1],nxt[N<<1],num=0;
// vector 邻接表：adj[u] 存 u 的所有出边
struct Edge
{
    int to,w;
};
vector<Edge> adj[N];

void add_edge(int u,int v,int c)// 有向边 u->v 权 c
{
    to[++num]=v,w[num]=c,nxt[num]=head[u],head[u]=num;
}

void add_undirected(int u,int v,int c)// 无向边，正反各加一次
{
    add_edge(u,v,c);
    add_edge(v,u,c);
}

void add_adj(int u,int v,int c)// vector 邻接表加边
{
    Edge e;
    e.to=v,e.w=c;
    adj[u].push_back(e);
}

void show_star()// 遍历链式前向星，O(n+m)
{
    for(int u=1;u<=n;u++)
        for(int i=head[u];i;i=nxt[i])
            printf("%d->%d w=%d\n",u,to[i],w[i]);
}

void show_adj()// 遍历 vector 邻接表，O(n+m)
{
    for(int u=1;u<=n;u++)
        for(int i=0;i<(int)adj[u].size();i++)
            printf("%d->%d w=%d\n",u,adj[u][i].to,adj[u][i].w);
}

void show_ud_star()// 只按 u<v 打印无向边，方便对拍
{
    for(int u=1;u<=n;u++)
        for(int i=head[u];i;i=nxt[i])
            if(u<to[i])printf("%d-%d w=%d\n",u,to[i],w[i]);
}

void show_ud_adj()
{
    for(int u=1;u<=n;u++)
        for(int i=0;i<(int)adj[u].size();i++)
            if(u<adj[u][i].to)printf("%d-%d w=%d\n",u,adj[u][i].to,adj[u][i].w);
}
