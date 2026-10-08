// 无向非负权图全局最小割：O(n³) 时间，O(n²) 空间；允许重边合并。
// 输入对称邻接矩阵，0..n-1，自环忽略；n<2 返回 0。返回割值及割的一侧。
// 每轮最大邻接序末尾两个点合并；不是只求固定 s-t 的割。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
pair<ll,vector<int>> global_min_cut(vector<vector<ll>> w)
{
    int n= w.size();
    if(n < 2)
        return {0, {}};
    vector<int> v(n);iota(v.begin(),v.end(),0);
    vector<vector<int>> group(n);
    for(int i= 0; i < n; i++)
        group[i]= {i};
    ll ans=LLONG_MAX;vector<int> cut;
    while(v.size()>1)
    {
        vector<ll> d(n);vector<bool> used(n);int prev=-1;
        for(int k=0;k<(int)v.size();k++)
        {
            int t= -1;
            for(int u : v)
                if(!used[u] && (t < 0 || d[u] > d[t]))
                    t= u;
            if(k+1==(int)v.size())
            {
                if(d[t]<ans)ans=d[t],cut=group[t];
                for(int u : v)
                    if(u != prev && u != t)
                        w[prev][u]+= w[t][u], w[u][prev]= w[prev][u];
                group[prev].insert(group[prev].end(),group[t].begin(),group[t].end());
                v.erase(find(v.begin(),v.end(),t));break;
            }
            used[t]= true;
            prev= t;
            for(int u : v)
                if(!used[u])
                    d[u]+= w[t][u];
        }
    }
    return {ans,cut};
}
