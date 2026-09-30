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

int main()
{
    // 自测：4 个点 5 条边的有向图 + 同名无向图，两种存图结果必须完全一致
    n=4,m=5;
    int eu[6]={0,1,1,2,3,3},ev[6]={0,2,3,4,1,4},ew[6]={0,7,2,5,1,9};
    for(int i=1;i<=m;i++)add_edge(eu[i],ev[i],ew[i]),add_adj(eu[i],ev[i],ew[i]);
    printf("=== 有向图 链式前向星 ===\n");
    show_star();
    printf("=== 有向图 vector 邻接表 ===\n");
    show_adj();
    printf("=== 无向图 链式前向星(只印 u<v) ===\n");
    memset(head,0,sizeof(head)),num=0;
    for(int i=1;i<=n;i++)adj[i].clear();
    for(int i=1;i<=m;i++)add_undirected(eu[i],ev[i],ew[i]),add_adj(eu[i],ev[i],ew[i]),add_adj(ev[i],eu[i],ew[i]);
    show_ud_star();
    printf("=== 无向图 vector 邻接表(只印 u<v) ===\n");
    show_ud_adj();
    printf("边数 num=%d（无向边正反各加一次，所以是 2*m=%d）\n",num,2*m);
    return 0;
}
/* 样例输入 4 5 / 1 2 7 / 1 3 2 / 2 4 5 / 3 1 1 / 3 4 9
期望：两种存图打印出的边表逐行相同，无向图 num=10 */
