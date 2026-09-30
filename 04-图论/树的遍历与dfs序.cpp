// 树的遍历与 dfs 序：dfs 序 / 欧拉序 / 时间戳
// 三个数组：in[u],out[u] 是子树对应的时间戳区间；rnk[i] 是第 i 个时间戳上的点
// 子树 → 区间：子树 u 恰好是 [in[u], out[u]]（in 就是 dfn）
// euler 数组是长度 2n-1 的欧拉序（每步都记，配合 ST 表可 O(1) 求 LCA）
// dfs 全部写成迭代版（显式栈），n=1e5 的链也不会爆栈
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,root,ecnt;
int head[N],to[N<<1],nxt[N<<1],val[N];
int in[N],out[N],rnk[N],par[N],sz[N],timer_;
int euler[N<<1],first[N],edep[N<<1],elog[N<<1],st[20][N<<1];// 欧拉序 + ST 表
int stk[N],it[N];// 显式栈：stk 存点，it 存下一条要走的边（模拟递归）

void add_edge(int u,int v)
{
    to[++ecnt]=v,nxt[ecnt]=head[u],head[u]=ecnt;
    to[++ecnt]=u,nxt[ecnt]=head[v],head[v]=ecnt;
}

// 迭代 dfs 求 dfs 序 + 时间戳 + 子树大小 + 欧拉序，O(n)
void get_dfn(int rt)
{
    timer_=0;
    int tp=0,cnt=0;
    stk[++tp]=rt,it[tp]=head[rt],par[rt]=0,sz[rt]=1;
    in[rt]=++timer_,rnk[timer_]=rt;
    euler[++cnt]=rt,first[rt]=cnt,edep[cnt]=0;
    while(tp)
    {
        int u=stk[tp];
        if(it[tp])
        {
            int e=it[tp];
            it[tp]=nxt[e];// 回溯到这里时接着走下一条边
            int v=to[e];
            if(v==par[u])continue;
            par[v]=u,sz[v]=1;
            in[v]=++timer_,rnk[timer_]=v;
            euler[++cnt]=v,first[v]=cnt,edep[cnt]=edep[first[u]]+1;
            stk[++tp]=v,it[tp]=head[v];
        }
        else
        {
            out[u]=timer_;
            tp--;
            if(tp)
            {
                sz[stk[tp]]+=sz[u];// 回到父亲，把大小并上去
                euler[++cnt]=stk[tp],edep[cnt]=edep[first[stk[tp]]];// 欧拉序回退也记一次
            }
        }
    }
    for(int i=2;i<=cnt;i++)elog[i]=elog[i>>1]+1;// ST 表预处理的 log
    for(int i=1;i<=cnt;i++)st[0][i]=i;
    for(int k=1;k<20;k++)
        for(int i=1;i+(1<<k)-1<=cnt;i++)
        {
            int a=st[k-1][i],b=st[k-1][i+(1<<(k-1))];
            st[k][i]=edep[a]<edep[b]?a:b;
        }
}

// 欧拉序 + ST 表求 LCA，O(1)（建表 O(n log n)）
int lca(int x,int y)
{
    if(first[x]>first[y])swap(x,y);
    int l=first[x],r=first[y],k=elog[r-l+1];
    int a=st[k][l],b=st[k][r-(1<<k)+1];
    return edep[a]<edep[b]?euler[a]:euler[b];
}

// dfs 序上的树状数组：单点加 + 区间和，O(log n)，用来做「子树求和 / 子树加」
struct BIT{
    ll tr[N];
    void add(int x,ll k)
    {
        for(;x<=n;x+=x&(-x))tr[x]+=k;
    }
    ll query(int x)
    {
        ll s=0;
        for(;x>0;x-=x&(-x))s+=tr[x];
        return s;
    }
    ll range(int l,int r){return query(r)-query(l-1);}
}bit;
