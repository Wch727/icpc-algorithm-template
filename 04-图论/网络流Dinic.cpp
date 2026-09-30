#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=1e18;
int n,m,s,t;
int head[N],to[N<<1],nxt[N<<1],num=1;// 边从 2 开始编号，i^1 是反向边
ll cap[N<<1];// 剩余容量
int dep[N],cur[N];

void add_edge(int u,int v,ll c)
{
    to[++num]=v,cap[num]=c,nxt[num]=head[u],head[u]=num;
    to[++num]=u,cap[num]=0,nxt[num]=head[v],head[v]=num;// 反向边容量 0
}

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
