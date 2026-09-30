// 点分治：统计树上距离恰为 k 的有序点对数
// 过程：每层找重心 → 统计经过重心的路径 → 删掉重心递归处理各子树，共 O(n log n) 层
// 单层统计：先加「重心到各点距离」的桶，再对每棵子树先减去子树内部多算的配对（容斥）
// 找重心与统计都是迭代的；solve 递归深度是 O(log n)，n=1e5 也不会爆栈
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m,k;
int head[N],to[N<<1],nxt[N<<1],ecnt;
int dep_[N],siz[N],fa_[N],vis[N];
int bucket[N],que[N],qtmp[N];// bucket：桶；que：子树的 bfs 序；qtmp：本层加过桶的点
int K;

void add_edge(int u,int v)
{
    to[++ecnt]=v,nxt[ecnt]=head[u],head[u]=ecnt;
    to[++ecnt]=u,nxt[ecnt]=head[v],head[v]=ecnt;
}

// 迭代求重心：返回重心；同时把以 rt 为根的子树大小算好（不含已删除的点）
int get_centroid(int rt)
{
    int hd=0,tl=0,order[N];
    fa_[rt]=0,dep_[rt]=0;
    que[tl++]=rt;
    while(hd<tl)
    {
        int u=que[hd++];
        order[++tl]=u;// order[1..tl] 是 bfs 序
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(v==fa_[u]||vis[v])continue;
            fa_[v]=u,dep_[v]=dep_[u]+1,que[tl++]=v;
        }
    }
    for(int i=tl;i>=1;i--)
    {
        int u=order[i];
        siz[u]=1;
        for(int j=head[u];j;j=nxt[j])
            if(to[j]!=fa_[u]&&!vis[to[j]])siz[u]+=siz[to[j]];
    }
    int tot=siz[rt],c=rt;
    for(int i=1;i<=tl;i++)
    {
        int u=order[i];
        int mx=tot-siz[u];
        for(int j=head[u];j;j=nxt[j])
        {
            int v=to[j];
            if(v==fa_[u]||vis[v])continue;
            mx=max(mx,siz[v]);
        }
        if(mx<(tot+1)/2){c=u;break;}// 最大块 < tot/2 的点就是重心
    }
    return c;
}

// 从 c 出发走 cnt 步以内，边走边统计答案（当前 dep 与桶里的距离配对）
ll walk(int c,int cnt,int sgn)
{
    int hd=0,tl=0;
    fa_[c]=0,dep_[c]=0;
    que[tl++]=c;
    ll add=0;
    int added=0;
    while(hd<tl)
    {
        int u=que[hd++];
        if(dep_[u]>K)continue;
        if(bucket[K-dep_[u]])add+=1ll*sgn*bucket[K-dep_[u]];
        if(dep_[u]<=cnt)qtmp[added++]=dep_[u];// 只把 cnt 范围内的记下来
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(v==fa_[u]||vis[v])continue;
            fa_[v]=u,dep_[v]=dep_[u]+1,que[tl++]=v;
        }
    }
    for(int i=0;i<added;i++)bucket[qtmp[i]]+=sgn;// 更新桶
    return add;
}

// 分治主体：统计经过重心 c 的点对，再递归子树
ll solve(int rt)
{
    int c=get_centroid(rt);
    vis[c]=1;
    ll res=0;
    bucket[0]=1;// 重心自己
    int used[2]={0,0};// used[0] 记录加过的深度个数，用于清桶
    int list[N];
    for(int i=head[c];i;i=nxt[i])
    {
        int v=to[i];
        if(vis[v])continue;
        res-=walk(v,K-1,-1);// 容斥：先在「已有桶」里减去同子树内部的配对
        res+=walk(v,K,1);
    }
    for(int i=1;i<=K;i++)if(bucket[i])bucket[i]=0;// 本层桶清空
    bucket[0]=0;
    for(int i=head[c];i;i=nxt[i])
        if(!vis[to[i]])res+=solve(to[i]);
    return res;
}

int main()
{
    srand(20240518);

    // 自测 1：手造链 1-2-3-4-5，k=2
    n=5,k=2,K=2,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,vis[i]=0;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    printf("链 n=5 k=2：距离恰为 2 的无序点对 %lld（期望 3：(1,3)(2,4)(3,5)）\n",solve(1)/2);

    // 自测 2：手造星形 1 连 2..6，k=2
    n=6,k=2,K=2,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,vis[i]=0;
    for(int i=2;i<=n;i++)add_edge(1,i);
    printf("星形 n=6 k=2：%lld（期望 10 对，C(5,2)=10）\n",solve(1)/2);

    // 自测 3：随机树对拍（暴力：每个点 bfs 一遍）
    bool ok=true;
    int round=0;
    for(int T=1;T<=300;T++)
    {
        n=rand()%11+2,k=rand()%n,K=k,ecnt=0;
        for(int i=1;i<=n;i++)head[i]=0,vis[i]=0;
        for(int i=2;i<=n;i++)add_edge(rand()%(i-1)+1,i);
        ll got=solve(1);
        // 暴力
        ll want=0;
        for(int s=1;s<=n;s++)
        {
            int d[15];
            for(int i=1;i<=n;i++)d[i]=-1;
            int q2[15],h2=0,t2=0;
            q2[t2++]=s,d[s]=0;
            while(h2<t2)
            {
                int u=q2[h2++];
                if(d[u]>=k)continue;
                for(int i=head[u];i;i=nxt[i])
                    if(d[to[i]]<0)d[to[i]]=d[u]+1,q2[t2++]=to[i];
            }
            for(int t=1;t<=n;t++)if(d[t]==k)want++;
        }
        if(got!=want)
        {
            printf("FAILED t=%d n=%d k=%d got=%lld want=%lld\n",T,n,k,got,want);
            ok=false;
            break;
        }
        if(++round%6==0)printf("随机对拍 round %d passed\n",round);
    }
    printf("点分治路径计数对拍 %d 轮 %s\n",round,ok?"OK":"FAILED");

    // 自测 4：n=1e5 的链，k=1，答案 n-1；验证 O(n log n) 不爆栈
    n=100000,k=1,K=1,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,vis[i]=0;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    printf("大链 n=100000 k=1：%lld（期望 199998，即无序 99999 对）\n",solve(1)/2);
    return 0;
}
/* 注意：
   1) solve 返回的是有序点对数（(u,v) 与 (v,u) 各算一次），需要无序答案就 /2。
   2) k 要写进全局 K，walk 用它剪枝（dep>k 直接不再往下走）。
   3) 桶的清理只清本层用过的深度，别整段 memset，否则复杂度退化。 */
