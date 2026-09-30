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

int main()
{
    // 自测 1：手造网络 1->2(3) 1->3(2) 2->3(1) 2->4(2) 3->4(3)，最大流 = 5
    n=4,s=1,t=4;
    add_edge(1,2,3),add_edge(1,3,2),add_edge(2,3,1),add_edge(2,4,2),add_edge(3,4,3);
    printf("手造网络 最大流=%lld（期望 5）\n",dinic());
    // 自测 2：随机小网络，最大流 = 最小割（枚举源点侧点集暴力求割）
    for(int T=1;T<=200;T++)
    {
        n=rand()%5+3;
        for(int i=1;i<=n;i++)head[i]=0;
        num=1;
        s=1,t=n;
        int eu[60],ev[60];
        ll ew[60];
        m=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(i!=j&&rand()%2)
                {
                    m++;
                    eu[m]=i,ev[m]=j,ew[m]=rand()%8+1;
                    add_edge(i,j,ew[m]);
                }
        ll got=dinic();
        // 暴力最小割：枚举所有包含 s 不含 t 的点集 S，割 = 从 S 出去的边权和
        ll best=INF;
        for(int mask=0;mask<(1<<n);mask++)
        {
            if(!(mask&1))continue;// 必须含 s
            if(mask>>(n-1)&1)continue;// 不能含 t
            ll sum=0;
            for(int i=1;i<=m;i++)
                if((mask>>(eu[i]-1)&1)&&!(mask>>(ev[i]-1)&1))sum+=ew[i];
            best=min(best,sum);
        }
        if(got!=best)
        {
            printf("WA T=%d got=%lld mincut=%lld\n",T,got,best);
            return 0;
        }
    }
    printf("最大流=最小割 对拍 200 组通过\n");
    // 自测 3：二分图匹配型网络，与匈牙利结果对拍
    for(int T=1;T<=200;T++)
    {
        int nl=rand()%4+1,nr=rand()%4+1;
        n=nl+nr+2;
        for(int i=1;i<=n;i++)head[i]=0;
        num=1;
        s=n-1,t=n;
        for(int i=1;i<=nl;i++)add_edge(s,i,1);
        for(int j=1;j<=nr;j++)add_edge(nl+j,t,1);
        int mm=0,ea[30],eb[30];
        for(int i=1;i<=nl;i++)
            for(int j=1;j<=nr;j++)
                if(rand()%2)
                {
                    add_edge(i,nl+j,1);
                    mm++;
                    ea[mm]=i,eb[mm]=j;
                }
        ll got=dinic();
        // 暴力枚举所有匹配，求最大匹配
        int want=0;
        for(int mask=0;mask<(1<<mm);mask++)
        {
            int lu[8]={0},rv[8]={0},c=0,ok=1;
            for(int i=1;i<=mm&&ok;i++)
                if(mask>>(i-1)&1)
                {
                    if(lu[ea[i]]||rv[eb[i]])ok=0;
                    else lu[ea[i]]=1,rv[eb[i]]=1,c++;
                }
            if(ok)want=max(want,c);
        }
        if(got!=want)
        {
            printf("WA 二分图 T=%d got=%lld want=%d\n",T,got,want);
            return 0;
        }
    }
    printf("Dinic 求二分图最大匹配 对拍 200 组通过\n");
    return 0;
}
/* 最小割含义：跑完最大流后，从源点只沿 cap>0 的边走能到达的点集就是 S 侧，
   S 到 T 的边全部满流，这些边的容量和 = 最大流 = 最小割
   必须多路增广（while dfs）而不是一次 bfs 只推一条路，否则会退化成 EK */
