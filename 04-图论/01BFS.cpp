#include<bits/stdc++.h>
using namespace std;
const int INF=INT_MAX/4;
vector<vector<pair<int,int>>> adj;
vector<int> d;
deque<pair<int,int>> q;

// adj[u] 存 {v,w}，只允许 w=0/1；点编号取 adj 的合法下标，支持 0 起。
// 松弛成功时：0 边入队首，1 边入队尾；不能用普通 BFS 首次访问就锁定。
// O(n+m)，不可达为 INF；队列带入队距离，跳过过期项。
void bfs01(int s)
{
    int n=adj.size();
    assert(0<=s&&s<n);
    d.assign(n,INF);q.clear();
    d[s]=0,q.push_front({0,s});
    while(!q.empty())
    {
        auto [dist,u]=q.front();q.pop_front();
        if(dist!=d[u])continue;
        for(auto [v,w]:adj[u])
        {
            assert(0<=v&&v<n&&(w==0||w==1));
            if(d[v]<=dist+w)continue;
            d[v]=dist+w;
            if(w==0)q.push_front({d[v],v});
            else q.push_back({d[v],v});
        }
    }
}
