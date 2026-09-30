#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
int n,m;// 左右各 n 个点（左右点数不同就取较大的上界，单独记 nl,nr）
vector<int> adj[N];// adj[u] 存左部点 u 能匹配的右部点
int match[N];// match[v]：右部点 v 匹配的左部点
int vis[N];// 本轮增广中右部点是否访问过

int dfs(int u)// 从左部点 u 出发找增广路，O(m)
{
    for(int i=0;i<(int)adj[u].size();i++)
    {
        int v=adj[u][i];
        if(vis[v])continue;// 本轮已经试过，跳过
        vis[v]=1;
        if(!match[v]||dfs(match[v]))// v 没匹配，或者能让它原来的搭档腾位置
        {
            match[v]=u;
            return 1;
        }
    }
    return 0;
}

int hungarian(int nl)// 匈牙利求最大匹配，O(n*m)
{
    for(int i=1;i<=n;i++)match[i]=0;
    int ans=0;
    for(int u=1;u<=nl;u++)
    {
        for(int i=1;i<=n;i++)vis[i]=0;// 每个左部点重新清空标记
        if(dfs(u))ans++;
    }
    return ans;
}
