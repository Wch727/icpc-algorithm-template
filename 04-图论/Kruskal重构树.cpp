// 无向图按边权升序合并：原点为叶子，新结点权为合并边权；支持重边、负权、不连通。
// lca(u,v) 的权 = 两点路径中最大边权的最小值；异连通块返回 0，同点无边须另处理。
// component(u,w) 返回只保留边权<=w 时 u 所在连通块的树根，cnt[root] 为原点数。
// 构建 O(m log m+n log n)，查询 O(log n)，空间 O(m+n log n)；静态图，修改边后重建。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
struct KruskalTree
{
    struct Edge{int u,v;ll w;};
    int n,tot,lg;
    vector<vector<int>> ch,up;
    vector<int> dep,root,cnt;
    vector<ll> val;
    // 原点 1..n，n>=1；传入边副本，不修改调用者。
    KruskalTree(int n,vector<Edge> e):n(n),tot(n),lg(1),ch(2*n),dep(2*n),root(2*n),cnt(2*n,1),val(2*n,LLONG_MIN)
    {
        assert(n>=1);
        vector<int> fa(n+1),sz(n+1,1),node(n+1),par(2*n);
        iota(fa.begin(),fa.end(),0),iota(node.begin(),node.end(),0);
        auto find=[&](int x){while(x!=fa[x])x=fa[x]=fa[fa[x]];return x;};
        sort(e.begin(),e.end(),[](Edge a,Edge b){return a.w<b.w;});
        for(auto [u,v,w]:e)
        {
            int a=find(u),b=find(v);if(a==b)continue;
            int x=node[a],y=node[b];++tot;
            ch[tot]={x,y},par[x]=par[y]=tot,val[tot]=w,cnt[tot]=cnt[x]+cnt[y];
            if(sz[a]<sz[b])swap(a,b);
            fa[b]=a,sz[a]+=sz[b],node[a]=tot;
        }
        while((1LL<<lg)<=tot)++lg;
        up.assign(lg,vector<int>(tot+1));
        // 父编号总大于儿子，倒序即可处理整片森林，避免深链递归。
        for(int u=tot;u>=1;--u)
        {
            up[0][u]=par[u],dep[u]=dep[par[u]]+1;
            root[u]=par[u]?root[par[u]]:u;
            for(int j=1;j<lg;++j)up[j][u]=up[j-1][up[j-1][u]];
        }
    }
    int lca(int u,int v)const
    {
        if(root[u]!=root[v])return 0;
        if(dep[u]<dep[v])swap(u,v);
        for(int j=lg-1;j>=0;--j)if((dep[u]-dep[v])>>j&1)u=up[j][u];
        if(u==v)return u;
        for(int j=lg-1;j>=0;--j)if(up[j][u]!=up[j][v])u=up[j][u],v=up[j][v];
        return up[0][u];
    }
    int component(int u,ll w)const
    {
        for(int j=lg-1;j>=0;--j)if(up[j][u]&&val[up[j][u]]<=w)u=up[j][u];
        return u;
    }
};
// 例：KruskalTree t(3,{{1,2,5},{2,3,8}}); t.val[t.lca(1,3)]==8。
// 同权边会形成多个同权祖先；阈值查询要爬到最高合法祖先，不能停在第一次合并。
