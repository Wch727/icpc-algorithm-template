// 适用：运输、分配、带费用匹配；先最大化流量，再最小化总费用。
// 参数：顶点 1..n，s!=t；c 是非负容量，f 是单位费用，可为负。
// 前提：源点可达残量图不能有负费用环；初始化 SPFA 没有负环检测。
// 关键：fee+h[u]-h[v] 为约化费用；势能保持最短路选择与真实费用一致。
// 易错：e[id].cap 会被原地修改；重建时 清空 adj 与 e；反向边也占一个元素。
// 复杂度：初始化最坏 O(n*m)，A 次增广 O(A*(n+m)*log(n+m))；f*fee 和总费用须能放入 ll。
// 有限容量负费用边可先预置到上界，计入费用与点供需，再用其反向边撤销多余预流。
// 预置后必须平衡每个点的流量；这不是“直接灌满负边然后跑普通 s-t 流”，更不能用于无界负环。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=1e18;
int n,m,s,t;
struct Edge{int to;ll cap,fee;};
vector<Edge> e;
vector<int> adj[N];// 存 e 的下标，从 0 起；id^1 是反向边
ll dis[N],h[N];// dis：本次的约化费用最短路；h：势能，保证约化费用非负
int vis[N],pre[N];// pre[v]：最短路上到 v 的那条入边编号
ll ans_flow,ans_cost;

// O(1)，添加 u->v；c 为容量，f 为单价，反向边单价取负以抵消旧费用。
void add_edge(int u,int v,ll c,ll f)
{
    int id=e.size();
    e.push_back({v,c,f});
    e.push_back({u,0,-f});
    adj[u].push_back(id);
    adj[v].push_back(id^1);
}

// 最坏 O(n*m)，从全局 s 求初始势能；先处理负边才能使用 Dijkstra。
void spfa_init()// 先用 SPFA 求一遍初始势能，这样有负费用边也能跑 Dijkstra
{
    for(int i=1;i<=n;i++)h[i]=INF,vis[i]=0;
    queue<int> q;
    h[s]=0,vis[s]=1,q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        vis[u]=0;
        for(int i:adj[u])
        {
            int v=e[i].to;
            if(e[i].cap>0&&h[u]+e[i].fee<h[v])
            {
                h[v]=h[u]+e[i].fee;
                if(!vis[v])vis[v]=1,q.push(v);
            }
        }
    }
    for(int i= 1; i <= n; i++)
        if(h[i] == INF)
            h[i]= 0; // 不可达点势能置 0
}

// O((n+m)*log(n+m))，仅走正容量边；pre[v] 保存入边，返回 t 是否可达。
int dijkstra()// 沿约化费用最短路增广，O(m log n)；返回能否找到增广路
{
    for(int i=1;i<=n;i++)dis[i]=INF,vis[i]=0,pre[i]=0;
    priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<pair<ll,int>>> q;
    dis[s]=0,q.push(make_pair(0LL,s));
    while(!q.empty())
    {
        pair<ll,int> p=q.top();
        q.pop();
        int u=p.second;
        if(vis[u])continue;// 过期的旧距离，跳过
        vis[u]=1;
        for(int i:adj[u])
        {
            int v=e[i].to;
            if(e[i].cap>0&&dis[u]+e[i].fee+h[u]-h[v]<dis[v])
            {
                dis[v]=dis[u]+e[i].fee+h[u]-h[v];
                pre[v]=i;
                q.push(make_pair(dis[v],v));
            }
        }
    }
    return dis[t]<INF;
}

// 初始化加 A 次最短路增广；ans_flow/ans_cost 为输出，沿 pre 的反边终点回溯父点。
void mcmf()// 最小费用最大流，结果放 ans_flow / ans_cost
{
    ans_flow=0,ans_cost=0;
    // 最坏 O(n*m)，从全局 s 求初始势能；先处理负边才能使用 Dijkstra。
    spfa_init();
    while(dijkstra())
    {
        for(int i= 1; i <= n; i++)
            if(dis[i] < INF)
                h[i]+= dis[i]; // 累加势能
        ll f=INF;
        for(int v=t;v!=s;v=e[pre[v]^1].to)f=min(f,e[pre[v]].cap);// 沿路径找瓶颈
        for(int v=t;v!=s;v=e[pre[v]^1].to)
        {
            e[pre[v]].cap-=f;
            e[pre[v]^1].cap+=f;
            ans_cost+=f*e[pre[v]].fee;// 按真实费用累加
        }
        ans_flow+=f;
    }
}
