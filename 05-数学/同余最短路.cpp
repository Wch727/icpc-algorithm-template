// 正整数硬币无限使用：以最小硬币 m 为模，dist[r] 为能表示的最小且 ≡r 的数。
// x>=0 可表示 iff dist[x%m]<=x；用已有 Dijkstra 思路，边 r -> (r+a)%m，权 a。
// O(m*k log m)，空间 O(m)，适合 m 小；不能用最大的硬币为模随意增大状态。
// gcd>1 时不可表示数无限多；gcd=1 时最大不可表示非负数是 max(dist)-m，m=1 时为 -1。
#include<bits/stdc++.h>
using namespace std;
vector<long long> residue_shortest_path(vector<long long> coin)
{
    using ll=long long;const ll INF=LLONG_MAX/4;assert(!coin.empty());sort(coin.begin(),coin.end());assert(coin[0]>0&&coin[0]<=INT_MAX);
    coin.erase(unique(coin.begin(),coin.end()),coin.end());int m=coin[0];vector<ll>d(m,INF);d[0]=0;
    priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<pair<ll,int>>>q;q.push({0,0});
    while(!q.empty()){auto [x,u]=q.top();q.pop();if(x!=d[u])continue;for(ll a:coin)if(a<=INF-x){int v=(u+a%m)%m;if(x+a<d[v])d[v]=x+a,q.push({d[v],v});}}
    return d;
}
