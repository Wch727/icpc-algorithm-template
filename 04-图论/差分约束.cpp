#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=1e18;
int n,m;
int head[N],to[N<<1],nxt[N<<1],num;
ll w[N<<1];
ll dis[N];// dis[i] 就是变量 x_i 的一组可行解
int cnt[N],inq[N];
int eu[N],ev[N];// 自测用：记下所有约束，方便代回验证
ll ew[N];

void add_edge(int u,int v,ll c)// 有向边 u->v 权 c，表示 x_v <= x_u + c
{
    to[++num]=v,w[num]=c,nxt[num]=head[u],head[u]=num;
}

void add_leq(int u,int v,ll c)// 条件 x_v - x_u <= c，直接就是一条 u->v 边权 c
{
    add_edge(u,v,c);
}

void add_geq(int u,int v,ll c)// 条件 x_v - x_u >= c，转成 x_u - x_v <= -c，即 v->u 边权 -c
{
    add_edge(v,u,-c);
}

int spfa(int s)// 从超级源点 0 跑最短路，返回 1 表示有负环（约束矛盾，无解）
{
    for(int i=0;i<=n;i++)dis[i]=INF,cnt[i]=0,inq[i]=0;
    queue<int> q;
    dis[s]=0,inq[s]=1,q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        inq[u]=0;
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(dis[u]+w[i]<dis[v])
            {
                dis[v]=dis[u]+w[i];
                cnt[v]=cnt[u]+1;
                if(cnt[v]>n)return 1;// 最短路用到的边数超过 n，必有负环
                if(!inq[v])inq[v]=1,q.push(v);
            }
        }
    }
    return 0;
}

int solve()// 建超级源点 0 连向所有变量，返回是否有解
{
    for(int i=1;i<=n;i++)add_edge(0,i,0);// 每个变量都可以取 <= 0 的值，等价于固定一组参照
    return !spfa(0);
}

void clear_all()
{
    for(int i=0;i<=n;i++)head[i]=0;
    num=0;
}
