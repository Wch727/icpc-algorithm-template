#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
int n,m;
vector<int> adj[N];// 蕴含边：u -> v 表示 u 真则 v 真
int dfn[N],low[N],scc[N],stk[N],ins[N],tim,top,cnt;
int val[N];// val[i]：变量 i 的取值（0/1）

int id(int x,int v)// 变量 x 取值为 v 时对应的结点：1..n 为真，n+1..2n 为假
{
    return v?x:x+n;
}

void add_clause(int x,int vx,int y,int vy)// 加条件 (x==vx) 或 (y==vy)
{
    adj[id(x,!vx)].push_back(id(y,vy));// x 不取 vx，就只能 y 取 vy
    adj[id(y,!vy)].push_back(id(x,vx));
}

void tarjan(int u)// 缩点，O(n+m)
{
    dfn[u]=low[u]=++tim;
    stk[++top]=u,ins[u]=1;
    for(int i=0;i<(int)adj[u].size();i++)
    {
        int v=adj[u][i];
        if(!dfn[v])tarjan(v),low[u]=min(low[u],low[v]);
        else if(ins[v])low[u]=min(low[u],dfn[v]);
    }
    if(dfn[u]==low[u])
    {
        cnt++;
        int v;
        do
        {
            v=stk[top--];
            ins[v]=0;
            scc[v]=cnt;
        }while(v!=u);
    }
}

int solve()// 返回是否有解，有解时 val[] 是一组可行赋值
{
    for(int i=1;i<=2*n;i++)if(!dfn[i])tarjan(i);
    for(int i=1;i<=n;i++)
    {
        if(scc[i]==scc[i+n])return 0;// 真和假互相可达 -> 矛盾
        val[i]=scc[i]<scc[i+n];// Tarjan 出栈序是反拓扑序，编号小的在拓扑序后，取它为真
    }
    return 1;
}

void clear_all()// 多组数据清空
{
    for(int i=1;i<=2*n;i++)adj[i].clear(),dfn[i]=low[i]=scc[i]=ins[i]=0;
    tim=top=cnt=0;
}

int cx[105],cvx[105],cy[105],cvy[105];// 自测用：存下所有条件
