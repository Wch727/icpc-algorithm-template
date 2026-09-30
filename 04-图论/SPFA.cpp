#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=1e18;
int n,m,s;
struct Edge
{
    int to,w;
};
vector<Edge> adj[N];
ll dis[N];
int cnt[N];// cnt[u]：u 的入队次数，>=n 说明有负环
bool inq[N];

int spfa(int s)// 期望 O(k*m)，最坏 O(n*m)；返回 1 表示存在可达负环
{
    for(int i=1;i<=n;i++)dis[i]=INF,cnt[i]=0,inq[i]=0;
    queue<int> q;
    dis[s]=0,inq[s]=1,cnt[s]=1;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        inq[u]=0;
        for(int i=0;i<(int)adj[u].size();i++)
        {
            int v=adj[u][i].to,c=adj[u][i].w;
            if(dis[u]+c<dis[v])
            {
                dis[v]=dis[u]+c;
                if(!inq[v])
                {
                    q.push(v);
                    inq[v]=1;
                    cnt[v]++;
                    if(cnt[v]>=n)return 1;// 一个点入队 n 次必有负环
                }
            }
        }
    }
    return 0;
}
