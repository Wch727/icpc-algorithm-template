// 基环树：n 个点 n 条边的连通图，有且只有一个环
// 内容：找环（无向/有向两种） + 环上 DP（例题：基环树最大独立集，即「没有上司的舞会」带环版）
// 最大独立集做法：先对每个环点挂的树做树形 DP，再把环拆成链做两遍线性 DP
// 找环与 DP 全部迭代实现，n=1e5 的链+环不会爆栈
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m,ecnt;
int head[N],to[N<<1],nxt[N<<1],w[N],val[N];
int par[N],pe[N],vis[N];        // par：父亲，pe：从父亲过来的边编号
int ring[N],rn;                 // 环上的点，按环顺序（按环边相邻，编号不必单调）
int fstk[N],fit[N];             // 找环用的显式栈（放全局，避免递归/大数组爆栈）
int mark[N],tmp[N];             // mark：祖先标记；tmp：暂存另一支路径
ll dp0[N],dp1[N];              // dp0：不选 u，dp1：选 u（子树部分）
int f0[N],f1[N];                // 环上线性 DP

void add_edge(int u,int v)
{
    to[++ecnt]=v,nxt[ecnt]=head[u],head[u]=ecnt;
    to[++ecnt]=u,nxt[ecnt]=head[v],head[v]=ecnt;
}

// 迭代 dfs 找无向图（n 点 n 边）里的那个环，O(n)
// 做法：dfs 时碰到已访问的点 (u,v)，就说明树边链 u→根 和 v→根 在某个点汇合，环 = 两支接起来
void find_ring_undirected(int rt)
{
    for(int i=1;i<=n;i++)vis[i]=0,par[i]=0,pe[i]=0,mark[i]=0;
    rn=0;
    int tp=0,cu=0,cv=0;
    vis[rt]=1,fstk[++tp]=rt,fit[tp]=head[rt];
    while(tp)
    {
        int u=fstk[tp];
        if(fit[tp])
        {
            int e=fit[tp];
            fit[tp]=nxt[e];
            int v=to[e];
            if(pe[u]&&e==(((pe[u]-1)^1)+1))continue;// 只跳父边反向，保留重边
            if(vis[v]){cu=u,cv=v;break;}// 碰到走过的点 → 找到环
            vis[v]=1,par[v]=u,pe[v]=e;
            fstk[++tp]=v,fit[tp]=head[v];
        }
        else tp--;
    }
    if(!cu){rn=0;return;}
    // 先打出 cu 到根的链，再从 cv 往上走到第一个落在链上的点 l（用时间戳判，不用清数组）
    // 环 = cu→l 这一支 + cv→l 这一支接起来，正好是「树边 + 那条回边」构成的唯一环
    // 注意 l 可能就是 cv 自己（cv 是 cu 的祖先），所以要从 cv 自身开始判
    for(int i=1;i<=n;i++)mark[i]=0;
    for(int p=cu;p;p=par[p])mark[p]=1;
    int l=0;
    for(int p=cv;p;p=par[p])if(mark[p]){l=p;break;}
    if(!l){rn=0;return;}
    rn=0;
    for(int p=cu;p!=l;p=par[p])ring[++rn]=p;// cu 这一支
    ring[++rn]=l;
    int tc=0;
    for(int p=cv;p!=l;p=par[p])tmp[++tc]=p;// cv 这一支，反着接上去
    for(int i=tc;i>=1;i--)ring[++rn]=tmp[i];
}

// 有向图找环（每个点出度为 1 / 内向基环树都适用）：沿出边走，走过就说明有环
void find_ring_directed(int rt)
{
    for(int i=1;i<=n;i++)vis[i]=0;
    rn=0;
    int p=rt;
    while(!vis[p])vis[p]=1,p=to[head[p]];
    int q=to[head[p]];
    while(q!=p)ring[++rn]=q,q=to[head[q]];
    ring[++rn]=p;
}

// 树形 DP：以环点 rt 为根，只走 rt 挂的那棵树（环上的其他点都不进去），迭代逆序，O(n)
// 结果在 dp0/dp1 里：dp0[u] 不选 u，dp1[u] 选 u（只算这块子树）
int on_ring[N];
int torder[N],tfa[N],tq[N];// tree_dp 用的全局数组（放栈上会爆栈）
void tree_dp(int rt)
{
    int hd=0,tl=0,tot=0;
    tq[tl++]=rt,tfa[rt]=0,dp0[rt]=dp1[rt]=0;
    while(hd<tl)
    {
        int u=tq[hd++];
        torder[++tot]=u;
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(v==tfa[u]||on_ring[v])continue;// 环上别的点不走，只走 rt 挂的树
            tfa[v]=u,tq[tl++]=v;
        }
    }
    for(int i=tot;i>=1;i--)
    {
        int u=torder[i];
        dp0[u]=0,dp1[u]=val[u];
        for(int j=head[u];j;j=nxt[j])
        {
            int v=to[j];
            if(v==tfa[u]||on_ring[v])continue;
            dp0[u]+=max(dp0[v],dp1[v]);
            dp1[u]+=dp0[v];
        }
    }
}

// 基环树最大独立集：先各环点挂树 DP，再在两段链上 DP，O(n)
ll max_independent_set(int rt)
{
    find_ring_undirected(rt);
    for(int i=1;i<=n;i++)on_ring[i]=0;
    for(int i=1;i<=rn;i++)on_ring[ring[i]]=1;
    for(int i=1;i<=rn;i++)tree_dp(ring[i]);
    // 首点不选、首点必选；末点不能与首点同时选
    const ll neg=-4e18;
    ll ans=0;
    for(int take=0;take<=(rn>1);take++)
    {
        ll f0=take?neg:dp0[ring[1]],f1=take?dp1[ring[1]]:neg;
        for(int i=2;i<=rn;i++)
        {
            int u=ring[i];
            ll g0=max(f0,f1)+dp0[u],g1=f0+dp1[u];
            f0=g0,f1=g1;
        }
        ans=max(ans,take?f0:max(f0,f1));
    }
    return ans;
}
