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

int main()
{
    // 自测 1：经典手造图，MST 权值和 = 6
    // 1-2(1) 1-3(2) 2-3(3) 2-4(4) 3-5(5) 4-5(6)
    n=5,m=6;
    int a[8]={0,1,1,2,2,3,4},b[8]={0,2,3,3,4,5,5},c[8]={0,1,2,3,4,5,6};
    for(int i=1;i<=m;i++)e[i].u=a[i],e[i].v=b[i],e[i].w=c[i];
    printf("手造图 Kruskal=%lld（期望 12）\n",kruskal());
    for(int i=1;i<=m;i++)add_edge(a[i],b[i],c[i]),add_edge(b[i],a[i],c[i]);
    printf("手造图 Prim=%lld（期望 12）\n",prim(1));
    // 自测 2：不连通图，Kruskal 返回 -1；Prim 只吃到 1-2 那条边所在的连通块
    n=4,m=2;
    for(int i=1;i<=n;i++)head[i]=0;
    num=0;
    e[1].u=1,e[1].v=2,e[1].w=5;
    e[2].u=3,e[2].v=4,e[2].w=7;
    add_edge(1,2,5),add_edge(2,1,5);
    printf("不连通图 Kruskal=%lld Prim=%lld（都返回 -1）\n",kruskal(),prim(1));
    // 自测 3：随机连通图，Kruskal / Prim / 暴力枚举边集 三方对拍
    for(int t=1;t<=200;t++)
    {
        n=6;
        for(int i=1;i<=n;i++)head[i]=0;
        num=0;
        m=0;
        // 先造一棵随机生成树保证连通
        for(int i=2;i<=n;i++)
        {
            int u=rand()%(i-1)+1,cc=rand()%20+1;
            m++;
            e[m].u=u,e[m].v=i,e[m].w=cc;
            add_edge(u,i,cc),add_edge(i,u,cc);
        }
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
                if(rand()%2)
                {
                    int cc=rand()%20+1;
                    m++;
                    e[m].u=i,e[m].v=j,e[m].w=cc;
                    add_edge(i,j,cc),add_edge(j,i,cc);
                }
        ll k1=kruskal();
        ll k2=prim(1);
        // 暴力：枚举所有边集的 2^m 种选法
        ll best=INF;
        for(int mask=0;mask<(1<<m);mask++)
        {
            int f[10];
            for(int i=1;i<=n;i++)f[i]=i;
            ll s=0;
            int cnt=0;
            for(int i=1;i<=m;i++)
                if(mask>>(i-1)&1)
                {
                    int x=e[i].u,y=e[i].v;
                    while(f[x]!=x)x=f[x];
                    while(f[y]!=y)y=f[y];
                    if(x==y)continue;
                    f[x]=y;
                    s+=e[i].w;
                    cnt++;
                }
            if(cnt==n-1)best=min(best,s);
        }
        if(k1!=best||k2!=best)
        {
            printf("WA t=%d kruskal=%lld prim=%lld brute=%lld\n",t,k1,k2,best);
            return 0;
        }
    }
    printf("随机图 Kruskal=Prim=暴力 200 组通过\n");
    return 0;
}
/* Kruskal 适合稀疏图/边已排序；Prim 适合稠密图，朴素版 O(n^2) 把 pq 换成线性找最小值即可 */
