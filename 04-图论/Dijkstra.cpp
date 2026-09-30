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
// 链式前向星存图，与上面 vector 邻接表等价，任选一种使用
int head[N],to[N<<1],w[N<<1],nxt[N<<1],num=0;
ll dis[N];
bool vis[N];

void add_edge(int u,int v,int c)
{
    to[++num]=v,w[num]=c,nxt[num]=head[u],head[u]=num;
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

void dijkstra_star(int s)// 与堆优化版同一套流程，仅换遍历方式
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
        if(vis[u])continue;
        vis[u]=1;
        for(int i=head[u];i;i=nxt[i])
            if(dis[u]+w[i]<dis[to[i]])
            {
                dis[to[i]]=dis[u]+w[i];
                Node tmp;
                tmp.u=to[i],tmp.d=dis[to[i]];
                pq.push(tmp);
            }
    }
}

// 自测用的链式前向星副本已移到文件顶部

int main()
{
    // 自测：随机非负权有向图，朴素 / 堆优化 / 链式前向星堆优化 三者两两比对
    n=12;
    for(int t=1;t<=300;t++)
    {
        for(int i=1;i<=n;i++)adj[i].clear();
        memset(head,0,sizeof(head)),num=0;
        m=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)// 无向图，每条边只随一次机、只取一个权值
                if(rand()%3==0)
                {
                    int cc=rand()%20+1;
                    m++;
                    Edge e;
                    e.to=j,e.w=cc;
                    adj[i].push_back(e);
                    e.to=i;
                    adj[j].push_back(e);
                    add_undirected(i,j,cc);// 链式前向星正反各加一次
                }
        for(int st=1;st<=n;st++)
        {
            ll r1[20],r2[20],r3[20];
            dijkstra_naive(st);
            for(int i=1;i<=n;i++)r1[i]=dis[i];
            dijkstra_heap(st);
            for(int i=1;i<=n;i++)r2[i]=dis[i];
            dijkstra_star(st);
            for(int i=1;i<=n;i++)r3[i]=dis[i];
            for(int i=1;i<=n;i++)
                if(r1[i]!=r2[i]||r1[i]!=r3[i])
                {
                    printf("WA t=%d st=%d i=%d %lld %lld %lld\n",t,st,i,r1[i],r2[i],r3[i]);
                    return 0;
                }
        }
    }
    printf("随机对拍 300 组全部通过：朴素=堆优化=链式前向星\n");
    // 手造小图：1->2(7) 1->3(9) 1->6(14) 2->3(10) 2->4(15) 3->4(11) 3->6(2) 4->5(6) 6->5(9)
    n=6,m=9;
    for(int i=1;i<=n;i++)adj[i].clear();
    int a[10]={0,1,1,1,2,2,3,3,4,6},b[10]={0,2,3,6,3,4,4,6,5,5},c[10]={0,7,9,14,10,15,11,2,6,9};
    for(int i=1;i<=m;i++)
    {
        Edge e;
        e.to=b[i],e.w=c[i];
        adj[a[i]].push_back(e);
    }
    dijkstra_heap(1);
    printf("手造图源点 1 的最短路：");
    for(int i=1;i<=n;i++)printf("%lld ",dis[i]);
    printf("\n期望 0 7 9 20 20 11\n");
    return 0;
}
/* 最短路模板，负权边不能用；INF 用 1e18，松弛时不再额外判 INF */
