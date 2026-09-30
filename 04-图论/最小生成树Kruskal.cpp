#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
const ll INF=1e18;
int n,m;
struct Edge
{
    int u,v,w;
};
Edge e[N];
int fa[N],sz[N];

bool cmp(Edge x,Edge y)
{
    return x.w<y.w;
}

int findd(int x)
{
    if(fa[x]!=x)fa[x]=findd(fa[x]);
    return fa[x];
}

void unionn(int x,int y)
{
    x=findd(x),y=findd(y);
    if(x==y)return;
    if(sz[x]<sz[y])swap(x,y);
    fa[y]=x,sz[x]+=sz[y];
}

ll kruskal()// O(m log m)，返回最小生成树边权和；不连通返回 -1
{
    for(int i=1;i<=n;i++)fa[i]=i,sz[i]=1;
    sort(e+1,e+m+1,cmp);
    ll sum=0;
    int cnt=0;
    for(int i=1;i<=m;i++)
    {
        int u=findd(e[i].u),v=findd(e[i].v);
        if(u==v)continue;// 两端已连通，这条边舍掉
        unionn(u,v);
        sum+=e[i].w;
        cnt++;
        if(cnt==n-1)break;// 已经选了 n-1 条边，提前结束
    }
    if(cnt<n-1)return -1;
    return sum;
}

// ---------- 附：Prim 堆优化版 O((n+m) log n)，稠密图也可用朴素版 ----------
int head[N],to[N<<1],w[N<<1],nxt[N<<1],num=0;
void add_edge(int u,int v,int c)
{
    to[++num]=v,w[num]=c,nxt[num]=head[u],head[u]=num;
}
bool vis[N];
ll dis[N];

struct Node
{
    int u;
    ll d;
    bool operator>(const Node &b)const
    {
        return d>b.d;
    }
};

ll prim(int s)// 从 s 出发，返回 MST 权值和；不连通返回 -1
{
    for(int i=1;i<=n;i++)dis[i]=INF,vis[i]=0;
    dis[s]=0;
    priority_queue<Node,vector<Node>,greater<Node> > pq;
    Node st;
    st.u=s,st.d=0;
    pq.push(st);
    ll sum=0;
    int cnt=0;
    while(!pq.empty())
    {
        int u=pq.top().u;
        ll d=pq.top().d;
        pq.pop();
        if(vis[u])continue;
        vis[u]=1;
        sum+=d;
        cnt++;
        for(int i=head[u];i;i=nxt[i])
            if(w[i]<dis[to[i]])
            {
                dis[to[i]]=w[i];
                Node tmp;
                tmp.u=to[i],tmp.d=w[i];
                pq.push(tmp);
            }
    }
    if(cnt<n)return -1;
    return sum;
}
