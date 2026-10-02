// 最小堆给字典序最小拓扑序，不能保证任意 DAG 的逆序数最少。
// 反例边 5->1、5->2：堆顺序 3,4,5,1,2 有 6 个逆序，5,1,2,3,4 只有 4 个。
// “按约束排列值”的构造可能需要 p[topo[i]]=i，输出排名数组而非 topo 本身。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
vector<int> adj[N];
int in[N],out[N];
int dp[N];// dp[u]：从所有入度为 0 的点走到 u 的方案数 / 最长路长度
int q[N];// 手写队列，避免 STL queue 在拓扑里反复 push

int topo(int f[],int mode)// mode=0 统计方案数(取模)，mode=1 最长路；返回拓扑点数
{
    int head=0,tail=0;
    for(int i=1;i<=n;i++)if(in[i]==0)q[tail++]=i;
    int cnt=0;
    while(head<tail)
    {
        int u=q[head++];
        cnt++;
        for(int i=0;i<(int)adj[u].size();i++)
        {
            int v=adj[u][i];
            if(mode==0)f[v]=(f[v]+f[u])%80112002;
            else f[v]=max(f[v],f[u]+1);
            if(--in[v]==0)q[tail++]=v;// 入度减到 0 立刻能入队
        }
    }
    if(cnt<n)return -1;// 有点没进队 -> 有环
    return cnt;
}
