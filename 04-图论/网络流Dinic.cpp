// 适用：容量限制、二分图匹配、最小割；最大流等于最小割容量。
// 编号：顶点 1..n；s,t 必须不同，容量 c>=0。
// 容量：两条残量边占两个槽，实际边数须保证 2*m+1<N*2。
// 关键：只走层数加一的边；当前弧按引用推进，失败边不反复扫描。
// 易错：调用会修改 cap；重新求原图最大流须重建，head 清零且 num=1。
// 复杂度：一般图 O(n^2*m)，空间 O(n+m)；递归深度最坏 O(n)。
// 最大权闭合图：选 u 必须选 v 就连 u->v(INF)；正权连 S->u(w)，负权连 u->T(-w)。
// 最优值=正权总和-最小割；最后残量图中 S 可达点即选集。INF 大于所有有限容量总和且不溢出。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=1e18;
int n,m,s,t;
int head[N],to[N<<1],nxt[N<<1],num=1;// 边从 2 开始编号，i^1 是反向边
ll cap[N<<1];// 剩余容量
int dep[N],cur[N];

// O(1)，加容量为 c 的 u->v 边；反向边初始为 0，后续用于撤销旧流。
void add_edge(int u,int v,ll c)
{
    to[++num]=v,cap[num]=c,nxt[num]=head[u],head[u]=num;
    to[++num]=u,cap[num]=0,nxt[num]=head[v],head[v]=num;// 反向边容量 0
}

// O(n+m)，按正残量边分层；每轮重置 dep，不能沿已满的边走。
int bfs()// 分层，O(m)；返回能否到达汇点
{
    for(int i=1;i<=n;i++)dep[i]=-1;
    queue<int> q;
    dep[s]=0;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int i=head[u];i;i=nxt[i])
            if(cap[i]>0&&dep[to[i]]<0)
            {
                dep[to[i]]=dep[u]+1;
                q.push(to[i]);
            }
    }
    return dep[t]>=0;
}

// 沿分层图从 u 推至 t，flow 为本次上限；单次最坏 O(n*m)，一轮阻塞流 O(n*m)。
ll dfs(int u,ll flow)// 沿分层图推流，当前弧优化保证每条边只扫一次
{
    if(u==t)return flow;
    for(int &i=cur[u];i;i=nxt[i])
    {
        int v=to[i];
        if(cap[i]>0&&dep[v]==dep[u]+1)
        {
            ll f=dfs(v,min(flow,cap[i]));
            if(f>0)
            {
                cap[i]-=f;
                cap[i^1]+=f;// 反向边加回去，支持退流
                return f;
            }
        }
    }
    return 0;
}

// O(n^2*m)，返回当前残量图还能增广的流量；每轮 BFS 后必须重置 cur。
ll dinic()// 最大流，O(n^2 m)，随机图/二分图很快
{
    ll ans=0;
    while(bfs())
    {
        for(int i=1;i<=n;i++)cur[i]=head[i];
        ll f;
        while((f=dfs(s,INF))>0)ans+=f;
    }
    return ans;
}
