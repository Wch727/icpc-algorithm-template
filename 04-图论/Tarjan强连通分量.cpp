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

void add_edge(int u,int v)
{
    to[++num]=v,nxt[num]=head[u],head[u]=num;
}

void add_undirected(int u,int v)
{
    add_edge(u,v);
    add_edge(v,u);
}

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
