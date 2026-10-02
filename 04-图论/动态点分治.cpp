// 点分树：带非负边权树，动态点亮/熄灭，查询距最近亮点距离；空集合返回 INF。
// 0-indexed；预存每点到重心祖先的距离，构建 O(n log n)，切换 O(log²n)，查询 O(log n)。
// 每个重心用 multiset 保存亮点距离；删除一个副本，重复距离不能全删。建树遍历迭代。
#include<bits/stdc++.h>
using namespace std;
struct DynamicCentroid
{
    using ll=long long;static constexpr ll INF=LLONG_MAX/4;
    vector<vector<pair<int,ll>>> g,path;vector<bool> removed,on;vector<int> parent,size;vector<multiset<ll>> bag;
    DynamicCentroid(vector<vector<pair<int,ll>>> g):g(g),path(g.size()),removed(g.size()),on(g.size()),parent(g.size()),size(g.size()),bag(g.size()){if(!g.empty())build(0);}
    void build(int entry)
    {
        vector<int> order={entry};parent[entry]=-1;
        for(int i=0;i<(int)order.size();i++){int u=order[i];for(auto [v,w]:g[u])if(!removed[v]&&v!=parent[u])parent[v]=u,order.push_back(v);}
        int c=entry,total=order.size(),best=total;
        for(auto it=order.rbegin();it!=order.rend();it++){int u=*it;size[u]=1;int largest=0;for(auto [v,w]:g[u])if(!removed[v]&&parent[v]==u)size[u]+=size[v],largest=max(largest,size[v]);largest=max(largest,total-size[u]);if(largest<best)best=largest,c=u;}
        vector<tuple<int,int,ll>> q={{c,-1,0}};
        for(int i=0;i<(int)q.size();i++){auto [u,p,d]=q[i];path[u].push_back({c,d});for(auto [v,w]:g[u])if(!removed[v]&&v!=p)q.push_back({v,u,d+w});}
        removed[c]=true;for(auto [v,w]:g[c])if(!removed[v])build(v);
    }
    void set_active(int u,bool active){if(on[u]==active)return;on[u]=active;for(auto [c,d]:path[u])if(active)bag[c].insert(d);else bag[c].erase(bag[c].find(d));}
    ll query(int u)const{ll ans=INF;for(auto [c,d]:path[u])if(!bag[c].empty())ans=min(ans,d+*bag[c].begin());return ans;}
};
