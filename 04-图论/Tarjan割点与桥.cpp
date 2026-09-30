#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
int head[N],to[N<<1],nxt[N<<1],num=0;
int dfn[N],low[N];
int is_cut[N];// 是否为割点
int timer=0;
int ea[N],eb[N],cut_edge[N],ecnt=0;// 桥的列表

void add_edge(int u,int v)
{
    to[++num]=v,nxt[num]=head[u],head[u]=num;
}

void add_undirected(int u,int v)
{
    add_edge(u,v);
    add_edge(v,u);
}

void tarjan(int u,int fa)// 无向图求割点与桥，O(n+m)；fa 是边的入边编号，避免走回父亲
{
    dfn[u]=low[u]=++timer;
    int child=0;
    for(int i=head[u];i;i=nxt[i])
    {
        int v=to[i];
        if(!dfn[v])
        {
            child++;
            tarjan(v,i);
            low[u]=min(low[u],low[v]);
            if(low[v]>dfn[u])// 子树回不到 u 及更早 -> 这条边是桥
            {
                ecnt++;
                ea[ecnt]=u,eb[ecnt]=v;
                cut_edge[i]=cut_edge[i^1]=1;
            }
            if(fa&&low[v]>=dfn[u])is_cut[u]=1;// 非根结点有子树回不到上面 -> 割点
        }
        else if(i!=(fa^1))low[u]=min(low[u],dfn[v]);// 反向边走一次即可
    }
    if(!fa&&child>=2)is_cut[u]=1;// 根结点有两棵以上子树才是割点
}
