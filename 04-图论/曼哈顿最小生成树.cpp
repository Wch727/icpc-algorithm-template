// 二维点的曼哈顿距离完全图 MST：O(n log n) 生成候选边，再 Kruskal。
// 顶点 0..n-1；返回 {距离,u,v}，允许重合点。坐标绝对值及距离须远离 ll 上界。
// 方向扫描参考 KACTL ManhattanMST.h（CC0）；点按值传入，不改变原坐标。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
vector<array<ll,3>> manhattan_edges(vector<pair<ll,ll>> p)
{
    int n=p.size();vector<int> id(n);iota(id.begin(),id.end(),0);
    vector<array<ll,3>> e;
    for(int k=0;k<4;k++)
    {
        sort(id.begin(), id.end(),
             [&](int a, int b)
             {
                 return (__int128)p[a].first + p[a].second <
                        (__int128)p[b].first + p[b].second;
             });
        map<ll,int> sweep;
        for(int i:id)
        {
            for(auto it=sweep.lower_bound(-p[i].second);it!=sweep.end();)
            {
                int j=it->second;ll dx=p[i].first-p[j].first,dy=p[i].second-p[j].second;
                if(dy>dx)break;
                e.push_back({dx+dy,i,j});it=sweep.erase(it);
            }
            sweep[-p[i].second]=i;
        }
        for(auto &[x, y] : p)
            if(k & 1)
                x= -x;
            else
                swap(x, y);
    }
    return e;
}
