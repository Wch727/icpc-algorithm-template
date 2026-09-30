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
int head[N],to[N<<1],nxt[N<<1],ecnt;
int fa[N],dep[N],sz[N],son[N],top[N],dfn[N],rnk[N];
int stk[N],stk2[N];                     // 迭代 dfs 用的栈

void add_edge(int u,int v)
{
    to[++ecnt]=v,nxt[ecnt]=head[u],head[u]=ecnt;
    to[++ecnt]=u,nxt[ecnt]=head[v],head[v]=ecnt;
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
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(v==fa[u])continue;
            fa[v]=u,dep[v]=dep[u]+1;
            stk[++tp]=v;
        }
    }
    for(int i=ocnt;i>=1;i--)            // 逆序：先算完儿子再算父亲
    {
        int u=order[i];
        sz[u]=1,son[u]=0;
        for(int j=head[u];j;j=nxt[j])
        {
            int v=to[j];
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
        for(int i=head[u];i;i=nxt[i])   // 轻儿子各自开新链
        {
            int v=to[i];
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
        if(l==r){tr[p]=val[rnk[l]];return;}
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
        if(L<=l&&r<=R){tr[p]+=k*(r-l+1),lazy[p]+=k;return;}
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

// ---------- 以下为自测用的暴力 ----------
int bval[N];
int bsum_path(int u,int v)              // 暴力路径和
{
    int res=0;
    while(dep[u]>dep[v])res+=bval[u],u=fa[u];
    while(dep[v]>dep[u])res+=bval[v],v=fa[v];
    while(u!=v)res+=bval[u]+bval[v],u=fa[u],v=fa[v];
    return res+bval[u];
}
void bpath_add(int u,int v,int k)
{
    while(dep[u]>dep[v])bval[u]+=k,u=fa[u];
    while(dep[v]>dep[u])bval[v]+=k,v=fa[v];
    while(u!=v)bval[u]+=k,bval[v]+=k,u=fa[u],v=fa[v];
    bval[u]+=k;
}
void bsub_add(int u,int k)              // 暴力子树
{
    for(int i=1;i<=n;i++)
    {
        int x=i;
        while(x)
        {
            if(x==u){bval[i]+=k;break;}
            x=fa[x];
        }
    }
}
int bsub_sum(int u)
{
    int res=0;
    for(int i=1;i<=n;i++)
    {
        int x=i;
        while(x)
        {
            if(x==u){res+=bval[i];break;}
            x=fa[x];
        }
    }
    return res;
}

int main()
{
    srand(20240513);

    // 1. 手测：链 1-2-3-4-5，点权 1..5
    n=5,rt=1,ecnt=0,cnt=0;
    for(int i=1;i<=n;i++)head[i]=0,a[i]=i;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    dfs1_iter(rt),dfs2(rt,rt);
    for(int i=1;i<=n;i++)val[i]=a[i],bval[i]=a[i];
    seg.build(1,n,1);
    printf("path_sum(2,5)=%lld subtree_sum(3)=%lld point(1)=%lld\n",path_sum(2,5),subtree_sum(3),subtree_sum(1)-subtree_sum(2));
    path_add(2,5,1);
    printf("after path+1: path_sum(2,5)=%lld path_sum(2,2)=%lld subtree_sum(3)=%lld\n",path_sum(2,5),path_sum(2,2),subtree_sum(3));
    subtree_add(3,10);
    printf("after sub+10: subtree_sum(3)=%lld path_sum(1,5)=%lld\n",subtree_sum(3),path_sum(1,5));

    // 2. 随机对拍：真随机树（父亲从 [1,i-1] 里取）+ 随机操作
    bool ok=true;
    for(int T=1;T<=20&&ok;T++)
    {
        n=rand()%14+2,rt=rand()%n+1,ecnt=0,cnt=0;
        for(int i=1;i<=n;i++)head[i]=0;
        for(int i=2;i<=n;i++)
        {
            int f=rand()%(i-1)+1;
            add_edge(i,f);
        }
        dfs1_iter(rt),dfs2(rt,rt);
        for(int i=1;i<=n;i++)a[i]=rand()%21-10,bval[i]=a[i],val[i]=a[i];
        seg.build(1,n,1);
        m=80;
        for(int i=1;i<=m;i++)
        {
            op=rand()%4;
            int u=rand()%n+1,v=rand()%n+1,k=rand()%11-5;
            if(op==0)path_add(u,v,k),bpath_add(u,v,k);
            else if(op==1)
            {
                if(path_sum(u,v)!=bsum_path(u,v)){ok=false;break;}
            }
            else if(op==2)subtree_add(u,k),bsub_add(u,k);
            else
            {
                if(subtree_sum(u)!=bsub_sum(u)){ok=false;break;}
            }
        }
        for(int u=1;u<=n&&ok;u++)           // 收尾全量比对
        {
            if(subtree_sum(u)!=bsub_sum(u))ok=false;
            for(int v=1;v<=n&&ok;v++)
            {
                int uu=u,vv=v;
                if(path_sum(uu,vv)!=bsum_path(uu,vv))ok=false;
            }
        }
        printf("random round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 链状极限数据：100000 个点，路径和 / 子树和不能爆栈超时
    n=100000,rt=1,ecnt=0,cnt=0;
    for(int i=1;i<=n;i++)head[i]=0,a[i]=1;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    dfs1_iter(rt),dfs2(rt,rt);
    for(int i=1;i<=n;i++)val[i]=a[i];
    seg.build(1,n,1);
    printf("big chain: path_sum(1,100000)=%lld subtree_sum(50000)=%lld\n",path_sum(1,n),subtree_sum(50000));
    path_add(1,n,1);
    printf("after +1: path_sum(1,100000)=%lld subtree_sum(50000)=%lld\n",path_sum(1,n),subtree_sum(50000));
    return 0;
}
