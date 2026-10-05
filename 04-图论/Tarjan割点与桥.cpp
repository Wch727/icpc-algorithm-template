// 适用：无向图删除点或边后的连通性，割点与桥必须分别判断。
// 参数：顶点 1..n；根调用 tarjan(u,0)，fa 是入边编号而非父顶点。
// 关键：桥要求 low[v]>dfn[u]，割点允许相等；根必须有至少两棵 DFS 子树。
// 从 2 开始成对加边，i^1 为反向边；重边仅跳过真正的父边。
// vector 邻接表保存 {目标,有向边编号}；重建时 num=1，清空 g 与各标记。
// DFS 树边用 low[v]，已访问邻点用 dfn[v]；否则会把跨割点的返祖信息错误传播。
// 复杂度：全图 O(n+m)，空间 O(n+m)，递归深度 O(n)；多组须清空标记与计数。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
vector<pair<int,int>> g[N];
int num=1;
int dfn[N],low[N];
int is_cut[N];// 是否为割点
int timer=0;
int ea[N],eb[N],ecnt=0;// 桥的列表
vector<int> cut_edge(2);// 按有向边编号标记桥

// O(1)，加一个方向的邻接边；编号由 num 决定，不能单独打乱配对。
void add_edge(int u,int v)
{
    g[u].push_back({v,++num});cut_edge.push_back(0);
}

// O(1)，按顺序加两个方向；注意现有边编号与异或反边约定的风险。
void add_undirected(int u,int v)
{
    add_edge(u,v);
    add_edge(v,u);
}

// 全图 O(n+m)，跳过入边的反边以保留重边回边；结果在 is_cut、ea/eb、cut_edge。
void tarjan(int u,int fa)// 无向图求割点与桥，O(n+m)；fa 是边的入边编号，避免走回父亲
{
    dfn[u]=low[u]=++timer;
    int child=0;
    for(auto [v,i]:g[u])
    {
        if(!dfn[v])
        {
            child++;
            tarjan(v,i);
            low[u]=min(low[u],low[v]);
            if(low[v]>dfn[u])// 子树回不到 u 及更早 -> 这条边是桥
            {
                ecnt++;
                ea[ecnt]=u,eb[ecnt]=v;
                cut_edge[i]=cut_edge[i^1]=1;
            }
            if(fa&&low[v]>=dfn[u])is_cut[u]=1;// 非根结点有子树回不到上面 -> 割点
        }
        else if(i!=(fa^1))low[u]=min(low[u],dfn[v]);// 反向边走一次即可
    }
    if(!fa&&child>=2)is_cut[u]=1;// 根结点有两棵以上子树才是割点
}
