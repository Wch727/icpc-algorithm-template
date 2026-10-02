// 适用：有向图缩点后做 DAG DP、判互相可达；单点也属于 SCC。
// 参数：u 为 1..n 的顶点；图不连通时对每个未访问点调用 tarjan。
// 关键：low 是可追溯到的栈内最早时间；已出栈分量不能参与回退。
// 结论：belong[u] 是分量编号，sz[id] 是大小；跨分量边构成 DAG。
// 易错：多组清空 dfn/low/in_stk/belong/sz、head/dag，timer/top/scc_cnt/num 归零。
// 复杂度：全图 O(n+m)，空间 O(n+m)；递归长链可能爆栈，val/dp 的和注意 int 范围。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
int head[N],to[N<<1],nxt[N<<1],num=0;
int dfn[N],low[N],stk[N],in_stk[N],belong[N];
int timer=0,top=0,scc_cnt=0;
vector<int> dag[N];// 缩点后的 DAG
int sz[N];// 每个 SCC 的点数
int val[N];// 每个 SCC 的点权和（DP 用）
int dp[N];// 缩点 DAG 上 DP 的值

// O(1)，加有向 u->v 边，边下标从 1 开始。
void add_edge(int u,int v)
{
    to[++num]=v,nxt[num]=head[u],head[u]=num;
}

// O(1)，连加双向边；SCC 问题通常只需 add_edge，勿误用双向图。
void add_undirected(int u,int v)
{
    add_edge(u,v);
    add_edge(v,u);
}

// 全图 O(n+m)，u 是当前入口；low[u]==dfn[u] 时弹栈直到 u，恰好一个 SCC。
void tarjan(int u)// Tarjan 求 SCC，O(n+m)
{
    dfn[u]=low[u]=++timer;
    stk[++top]=u,in_stk[u]=1;
    for(int i=head[u];i;i=nxt[i])
    {
        int v=to[i];
        if(!dfn[v])
        {
            tarjan(v);
            low[u]=min(low[u],low[v]);
        }
        else if(in_stk[v])low[u]=min(low[u],dfn[v]);// 只回退栈内的点
    }
    if(dfn[u]==low[u])// u 是这个 SCC 的根
    {
        scc_cnt++;
        int x;
        do
        {
            x=stk[top--];
            in_stk[x]=0;
            belong[x]=scc_cnt;
            sz[scc_cnt]++;
        }while(x!=u);
    }
}
