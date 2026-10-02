#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=1e18;// 距离上界，加法前判 INF 防溢出
int n,m,s;
struct Edge
{
    int to,w;
};
vector<Edge> adj[N];
ll dis[N];
bool vis[N];

void add_edge(int u,int v,int c)
{
    adj[u].push_back({v,c});
}

void add_undirected(int u,int v,int c)
{
    add_edge(u,v,c);
    add_edge(v,u,c);
}

struct Node// 堆里的小根堆结点
{
    int u;
    ll d;
    bool operator>(const Node &b)const
    {
        return d>b.d;
    }
};

void dijkstra_naive(int s)// 朴素 O(n^2+m)，稠密图更稳
{
    for(int i=1;i<=n;i++)dis[i]=INF,vis[i]=0;
    dis[s]=0;
    for(int i=1;i<=n;i++)
    {
        int u=0;
        for(int j=1;j<=n;j++)
            if(!vis[j]&&(u==0||dis[j]<dis[u]))u=j;
        if(u==0||dis[u]==INF)break;// 剩下的点都不可达
        vis[u]=1;
        for(int k=0;k<(int)adj[u].size();k++)
        {
            int v=adj[u][k].to,c=adj[u][k].w;
            if(dis[u]+c<dis[v])dis[v]=dis[u]+c;
        }
    }
}

void dijkstra_heap(int s)// 堆优化 O((n+m) log n)，只适合非负权
{
    for(int i=1;i<=n;i++)dis[i]=INF,vis[i]=0;
    dis[s]=0;
    priority_queue<Node,vector<Node>,greater<Node> > pq;
    Node st;
    st.u=s,st.d=0;
    pq.push(st);
    while(!pq.empty())
    {
        int u=pq.top().u;
        pq.pop();
        if(vis[u])continue;// vis 判重，每个点只出堆一次
        vis[u]=1;
        for(int k=0;k<(int)adj[u].size();k++)
        {
            int v=adj[u][k].to,c=adj[u][k].w;
            if(dis[u]+c<dis[v])
            {
                dis[v]=dis[u]+c;
                Node tmp;
                tmp.u=v,tmp.d=dis[v];
                pq.push(tmp);
            }
        }
    }
}
