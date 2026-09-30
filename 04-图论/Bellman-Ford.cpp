#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=1e18;
int n,m,s;
struct Edge
{
    int u,v,w;
};
Edge e[N];// 边表，Bellman-Ford 只需要能枚举所有边
ll dis[N];

int bellman_ford(int s)// O(n*m)：n-1 轮松弛 + 第 n 轮判负环
{
    for(int i=1;i<=n;i++)dis[i]=INF;
    dis[s]=0;
    int flag=0;
    for(int i=1;i<=n;i++)
    {
        flag=0;
        for(int j=1;j<=m;j++)
            if(dis[e[j].u]<INF&&dis[e[j].u]+e[j].w<dis[e[j].v])
            {
                dis[e[j].v]=dis[e[j].u]+e[j].w;
                flag=1;
            }
        if(!flag)break;// 一整轮没松弛，提前结束
        if(i==n&&flag)return 1;// 第 n 轮还能松弛 -> 有可达负环
    }
    return 0;
}
