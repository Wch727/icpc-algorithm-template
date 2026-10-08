// 原点 1..n；闭区间连边。出树父->子，入树子->父；两树在叶子共用原点。
// 建图 O(n)，一次点/区间或区间/区间连边 O(log n)；共 O(n+q) 点、O(n+q log n) 边。
// g 是 vector 邻接表，边为 {目标,权重}；Dijkstra 时所有权重非负，距离数组按 g.size() 分配。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
struct RangeGraph
{
    int n;
    vector<int> in,out;
    vector<vector<pair<int,ll>>> g;
    RangeGraph(int n) : n(n), in(4 * n + 4), out(4 * n + 4), g(n + 1)
    {
        assert(n > 0);
        build(1, 1, n);
    }
    int new_node()
    {
        g.emplace_back();
        return (int)g.size() - 1;
    }
    void add_edge(int u,int v,ll w){g[u].push_back({v,w});}
    void build(int p,int l,int r)
    {
        if(l == r)
        {
            in[p]= out[p]= l;
            return;
        }
        in[p]=new_node(),out[p]=new_node();int m=(l+r)/2;
        build(p*2,l,m),build(p*2+1,m+1,r);
        for(int c : {p * 2, p * 2 + 1})
            add_edge(out[p], out[c], 0), add_edge(in[c], in[p], 0);
    }
    // cover(u,L,R,w,type)：type=0 为 u->区间，1 为区间->u；u 可是辅助点。
    void cover(int p,int l,int r,int u,int L,int R,ll w,int type)
    {
        if(L <= l && r <= R)
        {
            if(type)
                add_edge(in[p], u, w);
            else
                add_edge(u, out[p], w);
            return;
        }
        int m=(l+r)/2;
        if(L<=m)cover(p*2,l,m,u,L,R,w,type);
        if(R>m)cover(p*2+1,m+1,r,u,L,R,w,type);
    }
    void point_to_range(int u,int l,int r,ll w)
    {
        if(l>r)return;
        assert(1<=l&&r<=n);cover(1,1,n,u,l,r,w,0);
    }
    void range_to_point(int l,int r,int u,ll w)
    {
        if(l>r)return;
        assert(1<=l&&r<=n);cover(1,1,n,u,l,r,w,1);
    }
    void range_to_range(int l,int r,int L,int R,ll w)
    {
        if(l>r||L>R)return;
        int u=new_node();range_to_point(l,r,u,0),point_to_range(u,L,R,w);
    }
};
// 区间->区间经过一个新中转点，整条路径只计一次 w，避免 O(log² n) 两两连边。
// 用于流：辅助树边须改为 INF 容量，不能把这里的 0 权当作 0 容量。
// 用于 2-SAT：辅助点只压缩可达性，仍要补齐原变量约束的逆否蕴含，不能给辅助点随意配否定。
// 子树同深度：tin/tout 用半开区间 [tin[u],tout[u])，按绝对深度把 tin 排进各桶。
// 在目标深度桶中 lower_bound(tin[u])、lower_bound(tout[u]) 即所求连续段，O(log n)。
// 桶内顺序作为区间坐标；若按目标深度分组建图，各桶总长度 O(n)，无需开 深度*n 个点。
// 禁止 u 与区间内 x 同选：同时连 u->非x、x->非u 两个方向，可分别压成点到区间和区间到点。
// 图上的辅助点不代表布尔变量，不可对它按 xor 1 取反；具体题目排除 u 时拆开对应位置。
