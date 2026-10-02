#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=505;
const ll INF=1e18;
int n;// 左右各 n 个点
ll w[N][N];// 边权；不成边的位置先设成很小的数（如 -1e12），要求存在完美匹配
ll lx[N],ly[N],slack[N];// 左右顶标、右部点的松弛量
int match[N];// match[v]：右部点 v 匹配的左部点
int visx[N],visy[N];// 本次增广中，左右部点是否在交错树上

int dfs(int u)// 从左部点 u 出发增广，只走 lx[u]+ly[v]==w[u][v] 的相等边
{
    visx[u]=1;
    for(int v=1;v<=n;v++)
    {
        if(visy[v])continue;
        ll d=lx[u]+ly[v]-w[u][v];
        if(d==0)
        {
            visy[v]=1;
            if(!match[v]||dfs(match[v]))// v 空着，或者能让它的搭档换一条路
            {
                match[v]=u;
                return 1;
            }
        }
        else if(d<slack[v])slack[v]=d;// 记下最小差值，供顶标调整用
    }
    return 0;
}

ll km()// 二分图最大权完美匹配，O(n^4) 最坏；本版反复 DFS，需 O(n^3) 时用保留交错树的增广写法
{
    for(int i=1;i<=n;i++)
    {
        lx[i]=-INF,ly[i]=0,match[i]=0;
        for(int j=1;j<=n;j++)lx[i]=max(lx[i],w[i][j]);// 左顶标取该行最大边权
    }
    for(int u=1;u<=n;u++)
    {
        for(int i=1;i<=n;i++)slack[i]=INF,visx[i]=0,visy[i]=0;
        while(!dfs(u))// 增广失败就调顶标，直到能找到相等边
        {
            ll d=INF;
            for(int i=1;i<=n;i++)if(!visy[i])d=min(d,slack[i]);
            for(int i=1;i<=n;i++)
            {
                if(visx[i])lx[i]-=d;
                if(visy[i])ly[i]+=d;
                else slack[i]-=d;
            }
            for(int i=1;i<=n;i++)visx[i]=0,visy[i]=0;
        }
    }
    ll ans=0;
    for(int v=1;v<=n;v++)ans+=w[match[v]][v];
    return ans;
}
