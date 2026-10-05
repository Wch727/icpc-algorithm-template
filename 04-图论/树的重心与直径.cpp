// 树的重心、直径、树上最远点
// 直径两种写法：两遍 bfs / 树形 DP；求法都是 O(n)
// 重心：删掉它之后最大连通块最小（重心最多两个，这里求编号最小的那个）
// 最远点：任意点 x 的某个最远点可取直径端点，O(1)（见 build_far/far_node/eccentricity）
// 递归写法给 n<=3e4 用；n=1e5 的链请用下面的迭代版（显式栈），否则可能爆栈
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n;
vector<int> adj[N];
int dis[N],que[N],order[N],fa[N],sz[N];// que 是 bfs 队列，order 是 bfs 序
int da,db,dia;// 直径两端点与长度

// 带权图把 adj 的元素换成 {to,w}，dis 累加边权即可
void add_edge(int u,int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

// 从 s 出发 bfs，返回最远点的编号；dis 顺手算好，O(n)，迭代不爆栈
int bfs(int s)
{
    for(int i=1;i<=n;i++)dis[i]=-1;
    int hd=0,tl=0,far=s;
    que[tl++]=s,dis[s]=0;
    while(hd<tl)
    {
        int u=que[hd++];
        if(dis[u]>dis[far])far=u;
        for(int v:adj[u])
        {
            if(dis[v]==-1)dis[v]=dis[u]+1,que[tl++]=v;
        }
    }
    return far;
}

// 两遍 bfs：第一次找端点，第二次定长度，O(n)
int get_diameter()
{
    da=bfs(1);
    db=bfs(da);
    dia=dis[db];
    return dia;
}

// 树形 DP 求直径：dp1/dp2 记向下最长、次长，O(n)，迭代不爆栈
int get_diameter_dp()
{
    int hd=0,tl=0;
    for(int i=1;i<=n;i++)fa[i]=0;
    que[tl++]=1,fa[1]=-1,dis[1]=0;
    while(hd<tl)
    {
        int u=que[hd++];
        order[hd]=u;// order[1..tl] 就是 bfs 序
        for(int v:adj[u])
        {
            if(v==fa[u])continue;
            fa[v]=u,dis[v]=dis[u]+1,que[tl++]=v;
        }
    }
    int dp1[N],dp2[N],ans=0;
    for(int i=tl;i>=1;i--)// 逆序保证儿子先算完
    {
        int u=order[i];
        dp1[u]=dp2[u]=0;
        for(int v:adj[u])
        {
            if(v==fa[u])continue;
            int d=dp1[v]+1;
            if(d>dp1[u])dp2[u]=dp1[u],dp1[u]=d;
            else if(d>dp2[u])dp2[u]=d;
        }
        ans=max(ans,dp1[u]+dp2[u]);
    }
    return ans;
}

// 求重心：最大子树最小的点，O(n)，迭代不爆栈
int get_centroid()
{
    int hd=0,tl=0;
    for(int i=1;i<=n;i++)fa[i]=0;
    que[tl++]=1,fa[1]=-1;
    while(hd<tl)
    {
        int u=que[hd++];
        order[hd]=u;
        for(int v:adj[u])
        {
            if(v==fa[u])continue;
            fa[v]=u,que[tl++]=v;
        }
    }
    for(int i=tl;i>=1;i--)
    {
        int u=order[i];
        sz[u]=1;
        for(int v:adj[u])
            if(v!=fa[u])sz[u]+=sz[v];
    }
    int best=1,mx=n+1;
    for(int u=1;u<=n;u++)
    {
        int m=n-sz[u];// 父亲那一边的块
        for(int v:adj[u])
            if(v!=fa[u])m=max(m,sz[v]);
        if(m<mx)mx=m,best=u;
    }
    return best;
}

// 树上最远点 / 偏心距：max(dist(x,da),dist(x,db))，并列时还可能有其他最远点。
// 前提：dis 是 bfs(da) 的结果（dis[da]=0, dis[db]=dia）
// 注意：不能写成 dia-dis[x]！那只在 x 落在直径路径上时才对，
//       一般点要分别算 d(x,da)=dis[x] 和 d(x,db)，其中 d(x,db) 需要 dist_db[]
int dist_db[N];// bfs(db) 的预处理结果，供 d_to_db 使用

void build_far()// O(n)，先调 get_diameter() 再用
{
    bfs(db);
    for(int i=1;i<=n;i++)dist_db[i]=dis[i];
    bfs(da);
}

int far_node(int x)
{
    return dist_db[x]>dis[x]?db:da;// 到 db 更远就选 db
}

int eccentricity(int x)
{
    return max(dis[x],dist_db[x]);// max(d(x,da), d(x,db))
}

// 朴素递归版求直径（n 小的时候用，链会爆栈）
int dfs_dia(int u,int f,int &res)
{
    int a=0;// 往下的最长链，次长的不单独存
    for(int v:adj[u])
    {
        if(v==f)continue;
        int d=dfs_dia(v,u,res)+1;
        res=max(res,a+d),a=max(a,d);
    }
    return a;
}
