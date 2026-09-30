#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
const int INF=0x3f3f3f3f;

// ================= 一、A* 求第 k 短路 =================
// h(x) = x 到终点的最短路（在反图上从终点 Dijkstra 一次预处理得到），可采纳
// 每次从堆里弹出「到终点第 i 次」的路径就是第 i 短路；O(k*E*log)
// 注意：与点 k 短路不同，这里按路径长度严格递增计数，同长度不同路径要分别算
// 本模板允许路径重复经过同一个点（标准 k 短路定义），所以不能用 vis 砍点

struct Edge
{
    int to,w,nxt;
};

struct Graph
{
    int head[N],cnt;
    Edge e[N*20];
    void init(int n){cnt=0;for(int i=0;i<=n;i++)head[i]=-1;}
    void add_edge(int u,int v,int w)
    {
        e[cnt].to=v,e[cnt].w=w,e[cnt].nxt=head[u],head[u]=cnt++;
    }
};

Graph g,rg;
int n,m,k,st,en;
int h[N];//h[i] = i 到 en 的最短路
int vis[N];

// 反图 Dijkstra 预处理 h，O(m log n)
void dijkstra_rev(int src)
{
    for(int i=1;i<=n;i++)h[i]=INF,vis[i]=0;
    priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > > pq;
    h[src]=0;
    pq.push(make_pair(0,src));
    while(!pq.empty())
    {
        int u=pq.top().second;
        pq.pop();
        if(vis[u])continue;
        vis[u]=1;
        for(int i=rg.head[u];i!=-1;i=rg.e[i].nxt)
        {
            int v=rg.e[i].to,w=rg.e[i].w;
            if(h[v]>h[u]+w)
            {
                h[v]=h[u]+w;
                pq.push(make_pair(h[v],v));
            }
        }
    }
}

struct State
{
    int u,d;//d 是已经走过的实际距离
    bool operator>(const State &o)const{return d+h[u]>o.d+h[o.u];}//小根堆按 f=g+h
};

// 求第 k 短路的长度；不足 k 条返回 -1
int kth_shortest(int s,int t,int kk)
{
    dijkstra_rev(t);
    if(h[s]==INF)return -1;//根本不连通
    priority_queue<State,vector<State>,greater<State> > pq;
    int cnt[N]={0};
    pq.push({s,0});
    while(!pq.empty())
    {
        State cur=pq.top();
        pq.pop();
        int u=cur.u;
        cnt[u]++;
        if(u==t&&cnt[u]==kk)return cur.d;
        if(cnt[u]>kk)continue;//同一个点被弹出超过 k 次，后面的一定不会更优
        for(int i=g.head[u];i!=-1;i=g.e[i].nxt)
        {
            int v=g.e[i].to,w=g.e[i].w;
            pq.push({v,cur.d+w});
        }
    }
    return -1;
}

// ================= 二、暴力枚举所有简单路径，用于对拍 =================
// 只在 n<=8 时用，O(路径数)；把 st->en 的所有简单路长度收集起来排序

vector<int> bf_len;

void bf_dfs(int u,int d)
{
    if(u==en)
    {
        bf_len.push_back(d);
        return;
    }
    for(int i=g.head[u];i!=-1;i=g.e[i].nxt)
    {
        int v=g.e[i].to,w=g.e[i].w;
        if(vis[v])continue;
        vis[v]=1;
        bf_dfs(v,d+w);
        vis[v]=0;
    }
}
