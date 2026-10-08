#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
const ll INF=4000000000000000000LL;

// ================= 一、A* 求第 k 短路 =================
// h(x) = x 到终点的最短路（在反图上从终点 Dijkstra 一次预处理得到），可采纳
// 每次从堆里弹出「到终点第 i 次」的路径就是第 i 短路；O(k*E*log)
// 注意：与点 k 短路不同，这里按路径长度非递减计数，同长度不同路径要分别算
// 边权非负；s==t 时计入长度 0 的空走法（排除它就把 k 加 1）。
// 本模板允许路径重复经过同一个点（标准 k 短路定义），所以不能用 vis 砍点

struct Edge
{
    int to,w;
};

struct Graph
{
    vector<Edge> adj[N];
    void init(int n)
    {
        for(int i= 0; i <= n; i++)
            adj[i].clear();
    }
    void add_edge(int u,int v,int w){adj[u].push_back({v,w});}
};

Graph g,rg;
int n,m,k,st,en;
ll h[N];//h[i] = i 到 en 的最短路
int vis[N];

// 反图 Dijkstra 预处理 h，O(m log n)
void dijkstra_rev(int src)
{
    for(int i=1;i<=n;i++)h[i]=INF,vis[i]=0;
    priority_queue<pair<ll,int>,vector<pair<ll,int> >,greater<pair<ll,int> > > pq;
    h[src]=0;
    pq.push(make_pair(0,src));
    while(!pq.empty())
    {
        int u=pq.top().second;
        pq.pop();
        if(vis[u])continue;
        vis[u]=1;
        for(Edge e:rg.adj[u])
        {
            int v=e.to,w=e.w;
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
    int u; ll d;//d 是已经走过的实际距离
    bool operator>(const State &o)const{return d+h[u]>o.d+h[o.u];}//小根堆按 f=g+h
};

// 求第 k 短路的长度；不足 k 条返回 -1
ll kth_shortest(int s,int t,int kk)
{
    if(kk<1)return -1;
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
        for(Edge e:g.adj[u])
        {
            int v=e.to,w=e.w;
            if(h[v] < INF)
                pq.push({v, cur.d + w});
        }
    }
    return -1;
}

// ================= 三、A* 解八数码（洛谷 P1379）=================
// 状态压成 64 位整数（每格 4 bit），h 用「不在目标位置的格子数」

ll pz_encode(int b[3][3])
{
    ll s=0;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)s|=(ll)b[i][j]<<(4*(8-(i*3+j)));
    return s;
}

void pz_decode(ll s,int b[3][3])
{
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)b[i][j]=(s>>(4*(8-(i*3+j))))&15;
}

// 八数码里空格向上下左右移动
const int pdx[]={0,-1,1,0,0};
const int pdy[]={0,0,0,-1,1};

// 交换第 p 格与第 q 格（p,q 是 0..8 的格子编号），O(1)
ll pz_flip(ll s,int p,int q)
{
    int b[3][3];
    pz_decode(s,b);
    swap(b[p/3][p%3],b[q/3][q%3]);
    return pz_encode(b);
}

struct PzState
{
    ll s;
    int g,h,z;
    bool operator>(const PzState &o)const{return g+h>o.g+o.h;}
};

int pz_h(ll s)//不在目标位置的格子数，可采纳
{
    const ll t=0x123456780ll;//encode(123456780) 的结果，空格的 0 在最低 4 bit
    int c=0;
    for(int i=0;i<9;i++)
    {
        int a=(s>>(4*i))&15,b=(t>>(4*i))&15;
        if(a&&a!=b)c++;
    }
    return c;
}

// A* 解八数码，h 用「不在目标位置的格子数」，O(状态数 log)
int astar_puzzle(int st[3][3])
{
    int goal[3][3]= {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    const ll GOAL=pz_encode(goal);//目标态编码 = 0x123456780
    ll s0=pz_encode(st);
    if(s0==GOAL)return 0;
    int z0=0;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(!st[i][j])z0=i*3+j;
    unordered_map<ll,int> best;
    priority_queue<PzState,vector<PzState>,greater<PzState> > pq;
    best[s0]=0;
    PzState s1;
    s1.s=s0,s1.g=0,s1.h=pz_h(s0),s1.z=z0;
    pq.push(s1);
    while(!pq.empty())
    {
        PzState cur=pq.top();
        pq.pop();
        if(cur.s==GOAL)return cur.g;
        if(best[cur.s]<cur.g)continue;//过时状态
        int zx=cur.z/3,zy=cur.z%3;
        for(int i=1;i<=4;i++)
        {
            int xx=zx+pdx[i],yy=zy+pdy[i];
            if(xx<0||xx>2||yy<0||yy>2)continue;
            ll v=pz_flip(cur.s,zx*3+zy,xx*3+yy);
            int g2=cur.g+1;
            unordered_map<ll,int>::iterator it=best.find(v);
            if(it!=best.end()&&it->second<=g2)continue;
            best[v]=g2;
            PzState t2;
            t2.s=v,t2.g=g2,t2.h=pz_h(v),t2.z=xx*3+yy;
            pq.push(t2);
        }
    }
    return -1;
}
