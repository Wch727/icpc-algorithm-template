// 树链剖分（重链剖分）+ 线段树：路径加/路径和 + 子树加/子树和
// 建树 O(n)，路径操作 O(log^2 n)，子树操作 O(log n)
// 关键性质：子树在 dfs 序上是一段连续区间 [dfn[x], dfn[x]+sz[x]-1]
// 两遍 dfs 都写成迭代版（显式栈），链状数据也不会爆栈
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m,rt,cnt,op;
int a[N],val[N];
vector<int> adj[N];
int fa[N],dep[N],sz[N],son[N],top[N],dfn[N],rnk[N];
int stk[N],stk2[N];                     // 迭代 dfs 用的栈

void add_edge(int u,int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

// 第一遍（迭代）：求 fa / dep / sz / 重儿子
int order[N],ocnt;
void dfs1_iter(int root)
{
    int tp=0;
    fa[root]=0,dep[root]=1;
    stk[++tp]=root,ocnt=0;
    while(tp)
    {
        int u=stk[tp--];
        order[++ocnt]=u;
        for(int v:adj[u])
        {
            if(v==fa[u])continue;
            fa[v]=u,dep[v]=dep[u]+1;
            stk[++tp]=v;
        }
    }
    for(int i=ocnt;i>=1;i--)            // 逆序：先算完儿子再算父亲
    {
        int u=order[i];
        sz[u]=1,son[u]=0;
        for(int v:adj[u])
        {
            if(v==fa[u])continue;
            sz[u]+=sz[v];
            if(sz[v]>sz[son[u]])son[u]=v;
        }
    }
}

// 第二遍（迭代）：编 dfs 序，重儿子优先 → 同一条重链上 dfn 连续
void dfs2(int root,int tproot)
{
    cnt=0;
    int tp=0;
    stk[tp]=root,stk2[tp]=tproot,tp++;
    while(tp)
    {
        tp--;
        int u=stk[tp],tpv=stk2[tp];
        top[u]=tpv,dfn[u]=++cnt,rnk[cnt]=u;
        for(int v:adj[u])   // 轻儿子各自开新链
        {
            if(v==fa[u]||v==son[u])continue;
            stk[tp]=v,stk2[tp]=v,tp++;
        }
        if(son[u])stk[tp]=son[u],stk2[tp]=tpv,tp++;   // 重儿子接着这条链
    }
}

struct SegmentTree{
    ll tr[N<<2],lazy[N<<2];
    #define lp (p<<1)
    #define rp ((p<<1)|1)
    #define mid ((l+r)>>1)
    void build(int l,int r,int p)
    {
        lazy[p]=0;
        if(l == r)
        {
            tr[p]= val[rnk[l]];
            return;
        }
        build(l,mid,lp),build(mid+1,r,rp);
        tr[p]=tr[lp]+tr[rp];
    }
    void push_down(int l,int r,int p)
    {
        if(!lazy[p])return;
        tr[lp]+=lazy[p]*(mid-l+1),lazy[lp]+=lazy[p];
        tr[rp]+=lazy[p]*(r-mid),lazy[rp]+=lazy[p];
        lazy[p]=0;
    }
    void update(int L,int R,ll k,int l,int r,int p)
    {
        if(L <= l && r <= R)
        {
            tr[p]+= k * (r - l + 1), lazy[p]+= k;
            return;
        }
        push_down(l,r,p);
        if(L<=mid)update(L,R,k,l,mid,lp);
        if(R>mid)update(L,R,k,mid+1,r,rp);
        tr[p]=tr[lp]+tr[rp];
    }
    ll query(int L,int R,int l,int r,int p)
    {
        if(L<=l&&r<=R)return tr[p];
        push_down(l,r,p);
        ll res=0;
        if(L<=mid)res+=query(L,R,l,mid,lp);
        if(R>mid)res+=query(L,R,mid+1,r,rp);
        return res;
    }
}seg;

// 路径加：u,v 往上跳到同一条重链
void path_add(int u,int v,ll k)
{
    while(top[u]!=top[v])
    {
        if(dep[top[u]]<dep[top[v]])swap(u,v);
        seg.update(dfn[top[u]],dfn[u],k,1,n,1);
        u=fa[top[u]];
    }
    if(dep[u]>dep[v])swap(u,v);
    seg.update(dfn[u],dfn[v],k,1,n,1);
}

ll path_sum(int u,int v)
{
    ll res=0;
    while(top[u]!=top[v])
    {
        if(dep[top[u]]<dep[top[v]])swap(u,v);
        res+=seg.query(dfn[top[u]],dfn[u],1,n,1);
        u=fa[top[u]];
    }
    if(dep[u]>dep[v])swap(u,v);
    res+=seg.query(dfn[u],dfn[v],1,n,1);
    return res;
}

void subtree_add(int u,ll k){seg.update(dfn[u],dfn[u]+sz[u]-1,k,1,n,1);}
ll subtree_sum(int u){return seg.query(dfn[u],dfn[u]+sz[u]-1,1,n,1);}
