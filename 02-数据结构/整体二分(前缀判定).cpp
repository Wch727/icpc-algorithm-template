// 给定 m 条按时间加入的无向边，求每对点最早连通时间：0 初始连通，m+1 表示始终不连通。
// 每轮把查询按 mid 分桶，清空 DSU 后扫所有前缀；整体二分也适用于其他单调前缀判定。
// O((n+m+q) log(m+2) * alpha(n))，空间 O(n+m+q)；边/查询向量从 0 起，点 1..n。
#include<bits/stdc++.h>
using namespace std;
int n;
vector<pair<int,int>> e,query;
vector<int> first_connected()
{
    int m=e.size(),q=query.size();
    vector<int> l(q),r(q,m+1),fa(n+1),sz(n+1);
    auto find= [&](int x)
    {
        while(x != fa[x])
            x= fa[x]= fa[fa[x]];
        return x;
    };
    while(1)
    {
        vector<vector<int>> bucket(m+1);bool pending=false;
        for(int i= 0; i < q; ++i)
            if(l[i] < r[i])
                bucket[l[i] + (r[i] - l[i]) / 2].push_back(i), pending= true;
        if(!pending)return l;
        iota(fa.begin(),fa.end(),0),fill(sz.begin(),sz.end(),1);
        for(int t=0;t<=m;++t)
        {
            if(t)
            {
                auto [u,v]=e[t-1];u=find(u),v=find(v);
                if(u != v)
                {
                    if(sz[u] < sz[v])
                        swap(u, v);
                    fa[v]= u, sz[u]+= sz[v];
                }
            }
            for(int i:bucket[t])
                if(find(query[i].first)==find(query[i].second))r[i]=t;
                else l[i]=t+1;
        }
    }
}
// 换成 BIT/AC fail 树统计时，只替换重置、插入和 check；check 必须随前缀长度单调。
// 此处二分“时间/前缀”，不使用第 k 小模板里的“去右边减 k”；允许删除会破坏本例的单调性。
