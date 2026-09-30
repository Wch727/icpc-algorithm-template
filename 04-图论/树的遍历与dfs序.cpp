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

int main()
{
    srand(20240515);

    // 自测 1：手造树 1-2 1-3 2-4 2-5 3-6，检查 dfs 序与子树区间
    n=6,ecnt=0,root=1;
    for(int i=1;i<=n;i++)head[i]=0,val[i]=i;
    add_edge(1,2),add_edge(1,3),add_edge(2,4),add_edge(2,5),add_edge(3,6);
    get_dfn(root);
    printf("树 1-2,1-3,2-4,2-5,3-6，dfn：");
    for(int u=1;u<=n;u++)printf("%d:%d ",u,in[u]);
    printf("\n子树区间：");
    for(int u=1;u<=n;u++)printf("[%d,%d] ",in[u],out[u]);
    printf("\n欧拉序长度 %d（期望 11 = 2n-1），lca(4,5)=%d（期望 2）lca(4,6)=%d（期望 1）\n",
           2*n-1,lca(4,5),lca(4,6));

    // 自测 2：子树转区间 + 树状数组
    for(int u=1;u<=n;u++)bit.add(in[u],val[u]);
    printf("子树和：");
    for(int u=1;u<=n;u++)printf("%d:%lld ",u,bit.range(in[u],out[u]));
    printf("（期望 21 14 7 4 5 6）\n");

    // 自测 3：随机树对拍，检查时间戳、子树区间、子树大小、LCA、子树和
    bool ok=true;
    int round=0;
    for(int T=1;T<=200;T++)
    {
        n=rand()%12+2,ecnt=0,root=1;
        for(int u=1;u<=n;u++)head[u]=0,val[u]=rand()%20+1,bit.tr[u]=0;
        for(int i=2;i<=n;i++)add_edge(rand()%(i-1)+1,i);
        get_dfn(root);
        int bpar[15]={0},q2[15],h2=0,t2=0;
        q2[t2++]=1;
        while(h2<t2)
        {
            int u=q2[h2++];
            for(int i=head[u];i;i=nxt[i])
                if(to[i]!=bpar[u])bpar[to[i]]=u,q2[t2++]=to[i];
        }
        // 时间戳合法 且 区间长度 == 子树大小
        for(int u=1;u<=n;u++)
        {
            if(in[u]<1||in[u]>n||out[u]<in[u]||out[u]>n){ok=false;break;}
            if(out[u]-in[u]+1!=sz[u]){ok=false;break;}
            if(rnk[in[u]]!=u){ok=false;break;}
        }
        // 区间里的点恰好是子树
        for(int u=1;u<=n&&ok;u++)
        {
            for(int x=1;x<=n;x++)
            {
                int p=x,insub=0;
                while(p)
                {
                    if(p==u){insub=1;break;}
                    p=bpar[p];
                }
                int inrange=in[u]<=in[x]&&in[x]<=out[u];
                if(insub!=inrange){ok=false;break;}
            }
        }
        // LCA 与暴力比
        for(int x=1;x<=n&&ok;x++)
            for(int y=1;y<=n;y++)
            {
                int depx=0,depy=0,p=x;
                while(p)depx++,p=bpar[p];
                p=y;
                while(p)depy++,p=bpar[p];
                int a=x,b=y;
                while(depx>depy)a=bpar[a],depx--;
                while(depy>depx)b=bpar[b],depy--;
                while(a!=b)a=bpar[a],b=bpar[b];
                if(lca(x,y)!=a){ok=false;break;}
            }
        if(!ok){printf("FAILED 结构 t=%d\n",T);break;}
        // 子树和与暴力比
        for(int u=1;u<=n;u++)bit.add(in[u],val[u]);// 按 dfs 序建树状数组
        for(int u=1;u<=n;u++)
        {
            ll s=0;
            for(int x=1;x<=n;x++)
            {
                int p=x;
                while(p)
                {
                    if(p==u){s+=val[x];break;}
                    p=bpar[p];
                }
            }
            if(bit.range(in[u],out[u])!=s){ok=false;break;}
        }
        if(!ok){printf("FAILED 子树和 t=%d\n",T);break;}
        if(++round%5==0)printf("随机对拍 round %d passed\n",round);
    }
    printf("dfs 序/欧拉序/子树区间对拍 %d 轮 %s\n",round,ok?"OK":"FAILED");

    // 自测 4：链 n=1e5，迭代 dfs 不爆栈；LCA 正确
    n=100000,ecnt=0,root=1;
    for(int i=1;i<=n;i++)head[i]=0,val[i]=1;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    get_dfn(root);
    printf("大链 n=100000：in[1]=%d out[1]=%d（期望 1 100000）sz[1]=%d lca(1,100000)=%d lca(50000,99999)=%d\n",
           in[1],out[1],sz[1],lca(1,n),lca(50000,99999));
    return 0;
}
/* 注意：in/out 是 [1,n] 的闭区间，子树求和直接 bit.range(in[u],out[u])；
   欧拉序用「每步都记」的版本（长度 2n-1），另一种 2n 版本是进/出各记一次。 */
