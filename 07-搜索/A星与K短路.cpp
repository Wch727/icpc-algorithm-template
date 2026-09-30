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

// 返回第 k 短简单路的长度，不足返回 -1（同长度不同路径分别计数）
int brute_kth(int s,int t,int kk)
{
    bf_len.clear();
    memset(vis,0,sizeof(vis));
    vis[s]=1;
    bf_dfs(s,0);
    sort(bf_len.begin(),bf_len.end());
    if((int)bf_len.size()<kk)return -1;
    return bf_len[kk-1];
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
        if(a!=b)c++;
    }
    return c;
}

// A* 解八数码，h 用「不在目标位置的格子数」，O(状态数 log)
int astar_puzzle(int st[3][3])
{
    int goal[3][3]={{1,2,3},{4,5,6},{7,8,0}};
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

// ================= 四、自测 =================

void test_puzzle()
{
    int a[3][3]={{1,2,3},{4,5,6},{7,0,8}};
    printf("[A*八数码] 123456708 -> %d 步 (期望 1)\n",astar_puzzle(a));
    int b[3][3]={{1,2,3},{4,0,6},{7,5,8}};
    printf("[A*八数码] 123406758 -> %d 步 (期望 2)\n",astar_puzzle(b));
    int c[3][3]={{1,2,3},{4,5,6},{7,8,0}};
    printf("[A*八数码] 目标态 -> %d 步 (期望 0)\n",astar_puzzle(c));
}

void test_kth()
{
    mt19937 rnd(1234);
    int ok=1,fail=0,tested=0;
    for(int t=1;t<=400&&tested<30;t++)
    {
        n=rnd()%5+3;//3..7 个点
        g.init(n),rg.init(n);
        // 随机树：保证连通，且第 k 短路一定是简单路，可以和暴力完全对拍
        for(int i=2;i<=n;i++)
        {
            int u=rnd()%(i-1)+1,w=rnd()%5+1;
            g.add_edge(u,i,w),g.add_edge(i,u,w);
            rg.add_edge(i,u,w),rg.add_edge(u,i,w);
        }
        st=1,en=n;
        for(int kk=1;kk<=3;kk++)
        {
            int got=kth_shortest(st,en,kk);
            int bf=brute_kth(st,en,kk);
            if(bf==-1)continue;//树上不足 k 条简单路时跳过（A* 会给出重复走点的走法）
            tested++;
            if(got!=bf)
            {
                ok=0,fail++;
                if(fail<=3)printf("  第 %d 组 k=%d: A*=%d 暴力=%d\n",t,kk,got,bf);
            }
        }
    }
    printf("[k短路] %d 组随机树 与暴力枚举简单路 完全对拍 %s\n",tested,ok?"全部通过":"失败");

    // 含环的图上，A* 允许重复走点，答案不会长于「第 k 短简单路」
    ok=1,fail=0,tested=0;
    for(int t=1;t<=400&&tested<30;t++)
    {
        n=rnd()%5+3;
        g.init(n),rg.init(n);
        for(int i=2;i<=n;i++)
        {
            int u=rnd()%(i-1)+1,w=rnd()%5+1;
            g.add_edge(u,i,w),g.add_edge(i,u,w);
            rg.add_edge(i,u,w),rg.add_edge(u,i,w);
        }
        for(int i=1;i<=2;i++)
        {
            int u=rnd()%n+1,v=rnd()%n+1,w=rnd()%5+1;
            if(u==v)continue;
            g.add_edge(u,v,w),g.add_edge(v,u,w);
            rg.add_edge(v,u,w),rg.add_edge(u,v,w);
        }
        st=1,en=n;
        for(int kk=1;kk<=3;kk++)
        {
            int got=kth_shortest(st,en,kk);
            int bf=brute_kth(st,en,kk);
            if(bf==-1)continue;
            tested++;
            if(got>bf||got<0)
            {
                ok=0,fail++;
                if(fail<=3)printf("  第 %d 组 k=%d: A*=%d 简单路暴力=%d\n",t,kk,got,bf);
            }
        }
    }
    printf("[k短路] %d 组带环图 校验 A* 结果不长于第 k 短简单路 %s\n",tested,ok?"全部通过":"失败");
}

int main()
{
    // 小样例：4 个点，无向边 1-2(1) 2-4(1) 1-3(1) 3-4(1) 1-4(5)
    // 两条长度 2 的路径 + 一条长度 4 的绕路 + 直连 5
    n=4,m=5,k=3,st=1,en=4;
    g.init(n),rg.init(n);
    int ee[5][3]={{1,2,1},{2,4,1},{1,3,1},{3,4,1},{1,4,5}};
    for(int i=0;i<5;i++)//正向图和反向图都要加，反向图用来算 h
    {
        int u=ee[i][0],v=ee[i][1],w=ee[i][2];
        g.add_edge(u,v,w),g.add_edge(v,u,w);
        rg.add_edge(v,u,w),rg.add_edge(u,v,w);
    }
    for(int i=1;i<=5;i++)
        printf("[k短路] 第 %d 短路 = %d\n",i,kth_shortest(1,4,i));
    printf("[k短路] A* 允许重复走点，得到 2 2 4 4 4；暴力只数简单路，第 3 短 = %d\n",brute_kth(1,4,3));

    test_puzzle();
    test_kth();
    return 0;
}
