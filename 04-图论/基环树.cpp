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
int ring[N],rn;                 // 环上的点，按环顺序（无向图会得到编号递增/递减的顺序，够用）
int dp0[N],dp1[N];              // dp0：不选 u，dp1：选 u（子树部分）
int f0[N],f1[N];                // 环上线性 DP

void add_edge(int u,int v)
{
    to[++ecnt]=v,nxt[ecnt]=head[u],head[u]=ecnt;
    to[++ecnt]=u,nxt[ecnt]=head[v],head[v]=ecnt;
}

// 迭代 dfs 找无向图（n 点 n 边）里的那个环，O(n)
void find_ring_undirected(int rt)
{
    for(int i=1;i<=n;i++)vis[i]=0,par[i]=0,pe[i]=0;
    rn=0;
    int tp=0,stk[N],it[N];
    vis[rt]=1,stk[++tp]=rt,it[tp]=head[rt];
    int cu=0,cv=0;
    while(tp)
    {
        int u=stk[tp];
        if(it[tp])
        {
            int e=it[tp];
            it[tp]=nxt[e];
            int v=to[e];
            if(e==(pe[u]^1))continue;// 无向边成对存，(编号^1) 是反向边
            if(vis[v]){cu=u,cv=v;break;}// 碰到走过的点 → 找到环
            vis[v]=1,par[v]=u,pe[v]=e;
            stk[++tp]=v,it[tp]=head[v];
        }
        else tp--;
    }
    if(!cu){rn=0;return;}
    // 从 cu 沿父亲爬到 cv 的最近公共祖先，两支接起来就是整个环
    int mark[N]={0};
    int p=cu;
    while(p)mark[p]=1,p=par[p];
    p=cv;
    while(p&&!mark[p])p=par[p];
    int l=p;
    if(!l){rn=0;return;}
    rn=0;
    p=cu;
    while(p!=l)ring[++rn]=p,p=par[p];
    ring[++rn]=l;
    int tmp[N],tc=0;
    p=cv;
    while(p!=l)tmp[++tc]=p,p=par[p];
    for(int i=tc;i>=1;i--)ring[++rn]=tmp[i];// 接上另一支，环顺序才连续
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
    reverse(ring+1,ring+rn+1);// 变成环上的正向顺序
}

// 树形 DP（环上的点当根，不跨过环边），迭代逆序，O(n)
// 返回值不关心，结果在 dp0/dp1 里；ban 是用来屏蔽的环边（这里用环点集合判断）
int on_ring[N];
void tree_dp(int rt)
{
    int order[N],fa2[N],q[N];
    int hd=0,tl=0,tot=0;
    q[tl++]=rt,fa2[rt]=0;
    while(hd<tl)
    {
        int u=q[hd++];
        order[++tot]=u;
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(v==fa2[u])continue;
            if(on_ring[u]&&on_ring[v])continue;// 环边不走
            fa2[v]=u,q[tl++]=v;
        }
    }
    for(int i=tot;i>=1;i--)
    {
        int u=order[i];
        dp0[u]=0,dp1[u]=val[u];
        for(int j=head[u];j;j=nxt[j])
        {
            int v=to[j];
            if(v==fa2[u]||(on_ring[u]&&on_ring[v]))continue;
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
    // 把环拆成链：case1 不选 ring[1]，case2 选 ring[1]（则 ring[rn] 必不选）
    int f0=0,f1=-1e9;// case1：1 号不选
    for(int i=1;i<=rn;i++)
    {
        int a=ring[i];
        int g0=max(f0,f1)+dp0[a],g1=f0+dp1[a];
        f0=g0,f1=g1;
    }
    ll ans1=max(f0,f1);
    f0=-1e9,f1=0;// case2：1 号必选
    for(int i=1;i<=rn;i++)
    {
        int a=ring[i];
        int g0=max(f0,f1)+dp0[a],g1=f0+dp1[a];
        f0=g0,f1=g1;
    }
    ll ans2=f0;// 最后要求 ring[rn] 不选，所以取 f0
    return max(ans1,ans2);
}

int main()
{
    srand(20240516);

    // 自测 1：三角形 1-2-3-1 各挂一个叶子，环长 3；最大独立集手工可算
    // 点权：1..6 = 3,2,5,1,4,6；挂法：4 挂 1，5 挂 2，6 挂 3
    n=6,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0;
    val[1]=3,val[2]=2,val[3]=5,val[4]=1,val[5]=4,val[6]=6;
    add_edge(1,2),add_edge(2,3),add_edge(3,1),add_edge(1,4),add_edge(2,5),add_edge(3,6);
    find_ring_undirected(1);
    printf("三角形挂叶子：环长 %d（期望 3）环上点",rn);
    for(int i=1;i<=rn;i++)printf(" %d",ring[i]);
    printf("\n最大独立集 %lld（暴力答案 15：选 3,4,5）\n",max_independent_set(1));

    // 自测 2：纯环 1-2-3-4-1，权 1,1,1,1，最大独立集 2
    n=4,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,val[i]=1;
    add_edge(1,2),add_edge(2,3),add_edge(3,4),add_edge(4,1);
    printf("四元环权全 1：环长 %d（期望 4）最大独立集 %lld（期望 2）\n",rn,max_independent_set(1));

    // 自测 3：有向基环树找环：1->2->3->4->2
    n=4,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0;
    add_edge(1,2),add_edge(2,3),add_edge(3,4),add_edge(4,2);
    find_ring_directed(1);
    printf("有向图 1->2->3->4->2：环长 %d（期望 3）环上点",rn);
    for(int i=1;i<=rn;i++)printf(" %d",ring[i]);
    printf("\n");

    // 自测 4：随机基环树对拍：环长 + 最大独立集（暴力枚举子集）
    bool ok=true;
    int round=0;
    for(int T=1;T<=200;T++)
    {
        n=rand()%10+3;
        int base=n;// 先造 n 个点的树，再加一条边 → n 点 n 边
        int eu[15],ev[15],ec=0;
        for(int i=2;i<=base;i++)eu[++ec]=rand()%(i-1)+1,ev[ec]=i;
        int a=rand()%base+1,b=rand()%base+1;
        while(b==a)b=rand()%base+1;
        int dup=0;
        for(int i=1;i<=ec;i++)if((eu[i]==a&&ev[i]==b)||(eu[i]==b&&ev[i]==a))dup=1;
        if(dup){T--;continue;}// 保证图里只有一个环
        eu[++ec]=a,ev[ec]=b;
        m=ec;
        ecnt=0;
        for(int i=1;i<=n;i++)head[i]=0,val[i]=rand()%10+1;
        for(int i=1;i<=m;i++)add_edge(eu[i],ev[i]);
        int rn2=0;
        find_ring_undirected(1),rn2=rn;
        ll got=max_independent_set(1);
        // 暴力：枚举所有子集
        ll want=0;
        for(int mask=0;mask<(1<<n);mask++)
        {
            int choose[15],cn=0,flag=1;
            ll s=0;
            for(int i=1;i<=n;i++)if(mask>>(i-1)&1)choose[++cn]=i,s+=val[i];
            for(int i=1;i<=m&&flag;i++)
            {
                int u=eu[i],v=ev[i];
                if((mask>>(u-1)&1)&&(mask>>(v-1)&1))flag=0;
            }
            if(flag)want=max(want,s);
        }
        if(got!=want)
        {
            printf("FAILED 独立集 t=%d n=%d got=%lld want=%lld\n",T,n,got,want);
            ok=false;
            break;
        }
        // 环长校验：环上的点两两相邻且去掉环边后是森林，这里只查点数与度数
        int edge_on_ring=0;
        for(int u=1;u<=n;u++)if(on_ring[u])
            for(int i=head[u];i;i=nxt[i])
                if(on_ring[to[i]])edge_on_ring++;
        edge_on_ring>>=1;
        if(edge_on_ring!=rn2)
        {
            printf("FAILED 环 t=%d rn=%d 环边数=%d\n",T,rn2,edge_on_ring);
            ok=false;
            break;
        }
        if(++round%5==0)printf("随机对拍 round %d passed\n",round);
    }
    printf("基环树环长/最大独立集对拍 %d 轮 %s\n",round,ok?"OK":"FAILED");

    // 自测 5：n=1e5 大环，迭代不爆栈（环长 1e5，最大独立集 50000）
    n=100000,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,val[i]=1;
    for(int i=1;i<=n;i++)add_edge(i,i%n+1);
    printf("大环 n=100000：环长 %d（期望 100000）最大独立集 %lld（期望 50000）\n",
           rn,max_independent_set(1));
    return 0;
}
/* 用法：先 find_ring_undirected(1) 拿到环，给环点打 on_ring 标记，
   再对每个环点 tree_dp()，最后两遍链上 DP 取 max。
   注意：本文件假设图连通且恰好一个环；多环（仙人掌）不适用。 */
