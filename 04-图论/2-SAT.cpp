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
    // 重新判定只清 SCC 工作数组，保留图与此前固定条件。
    for(int i=1;i<=2*n;i++)dfn[i]=low[i]=scc[i]=ins[i]=0;
    tim=top=cnt=0;
    for(int i= 1; i <= 2 * n; i++)
        if(!dfn[i])
            tarjan(i);
    for(int i=1;i<=n;i++)
    {
        if(scc[i]==scc[i+n])return 0;// 真和假互相可达 -> 矛盾
        val[i]=scc[i]<scc[i+n];// Tarjan 出栈序是反拓扑序，编号小的在拓扑序后，取它为真
    }
    return 1;
}

// 小规模字典序最小赋值：按变量 1..n 优先取 0；O(n*(n+m))，图中额外保留 n 条强制边。
// SCC 编号给出的任意解不保证字典序；单位子句 x=v 只需连 !(x=v)->(x=v)。
int solve_lex()
{
    if(!solve())return 0;
    for(int x=1;x<=n;x++)
    {
        adj[id(x,1)].push_back(id(x,0)); // 试 x=0
        if(!solve())
        {
            adj[id(x,1)].pop_back();
            adj[id(x,0)].push_back(id(x,1)); // 前面固定的条件保留，改成 x=1
        }
    }
    return solve();
}

void clear_all()// 多组数据清空
{
    for(int i=1;i<=2*n;i++)adj[i].clear(),dfn[i]=low[i]=scc[i]=ins[i]=0;
    tim=top=cnt=0;
}
