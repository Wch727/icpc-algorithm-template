#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=1e18;
int n,m,s,t;
int head[N],to[N<<1],nxt[N<<1],num=1;// 边从 2 开始编号，i^1 是反向边
ll cap[N<<1],fee[N<<1];// 剩余容量、单位费用
ll dis[N],h[N];// dis：本次的约化费用最短路；h：势能，保证约化费用非负
int vis[N],pre[N];// pre[v]：最短路上到 v 的那条入边编号
ll ans_flow,ans_cost;

void add_edge(int u,int v,ll c,ll f)
{
    to[++num]=v,cap[num]=c,fee[num]=f,nxt[num]=head[u],head[u]=num;
    to[++num]=u,cap[num]=0,fee[num]=-f,nxt[num]=head[v],head[v]=num;// 反向边容量 0、费用取负
}

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
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(cap[i]>0&&h[u]+fee[i]<h[v])
            {
                h[v]=h[u]+fee[i];
                if(!vis[v])vis[v]=1,q.push(v);
            }
        }
    }
    for(int i=1;i<=n;i++)if(h[i]==INF)h[i]=0;// 不可达点势能置 0
}

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
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(cap[i]>0&&dis[u]+fee[i]+h[u]-h[v]<dis[v])
            {
                dis[v]=dis[u]+fee[i]+h[u]-h[v];
                pre[v]=i;
                q.push(make_pair(dis[v],v));
            }
        }
    }
    return dis[t]<INF;
}

void mcmf()// 最小费用最大流，结果放 ans_flow / ans_cost
{
    ans_flow=0,ans_cost=0;
    spfa_init();
    while(dijkstra())
    {
        for(int i=1;i<=n;i++)if(dis[i]<INF)h[i]+=dis[i];// 累加势能
        ll f=INF;
        for(int v=t;v!=s;v=to[pre[v]^1])f=min(f,cap[pre[v]]);// 沿路径找瓶颈
        for(int v=t;v!=s;v=to[pre[v]^1])
        {
            cap[pre[v]]-=f;
            cap[pre[v]^1]+=f;
            ans_cost+=f*fee[pre[v]];// 按真实费用累加
        }
        ans_flow+=f;
    }
}
