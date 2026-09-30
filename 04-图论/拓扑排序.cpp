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

int main()
{
    // 自测 1：手造 DAG 1->2 1->3 2->3 3->4 2->4，方案数与最长路都对
    n=4,m=5;
    int a[8]={0,1,1,2,3,2},b[8]={0,2,3,3,4,4};
    for(int i=1;i<=m;i++)adj[a[i]].push_back(b[i]),in[b[i]]++,out[a[i]]++;
    for(int i=1;i<=n;i++)if(in[i]==0)dp[i]=1;
    int cnt=topo(dp,0);
    printf("拓扑点数=%d 方案数=",cnt);
    for(int i=1;i<=n;i++)printf("%d ",dp[i]);
    printf("\n期望 点数=4 方案数= 1 1 2 3\n");
    // 自测 2：同一张图求最长路（点权都当 1，答案=最长链边数）
    for(int i=1;i<=n;i++)in[i]=0,dp[i]=0;
    for(int i=1;i<=n;i++)
        for(int j=0;j<(int)adj[i].size();j++)in[adj[i][j]]++;
    topo(dp,1);
    int best=0;
    for(int i=1;i<=n;i++)best=max(best,dp[i]);
    printf("最长链边数=%d（期望 3）\n",best);
    // 自测 3：有环图必须返回 -1
    n=3;
    for(int i=1;i<=n;i++)adj[i].clear(),in[i]=0,dp[i]=0;
    adj[1].push_back(2),adj[2].push_back(3),adj[3].push_back(1);
    in[2]++,in[3]++,in[1]++;
    printf("有环图 拓扑点数=%d（期望 -1）\n",topo(dp,0));
    // 自测 4：随机 DAG，与指数级暴力枚举所有路径数对拍
    n=6;
    for(int t=1;t<=100;t++)
    {
        for(int i=1;i<=n;i++)adj[i].clear(),in[i]=0,dp[i]=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
                if(rand()%2)adj[i].push_back(j),in[j]++;
        for(int i=1;i<=n;i++)if(in[i]==0)dp[i]=1;
        topo(dp,0);
        // 暴力：另开一份入度数组，按拓扑序推路径数（topo 会把 in[] 清零，所以要先拷一份）
        int din[10];
        for(int i=1;i<=n;i++)din[i]=0;
        for(int i=1;i<=n;i++)
            for(int j=0;j<(int)adj[i].size();j++)din[adj[i][j]]++;
        int brute[10]={0};
        for(int st=1;st<=n;st++)
        {
            if(din[st])continue;
            int ways[10]={0};
            ways[st]=1;
            int ord[10],c2=0,deg[10];
            for(int i=1;i<=n;i++)deg[i]=din[i];
            int qq[10],h=0,tl=0;
            for(int i=1;i<=n;i++)if(!deg[i])qq[tl++]=i;
            while(h<tl)
            {
                int u=qq[h++];
                ord[c2++]=u;
                for(int j=0;j<(int)adj[u].size();j++)
                    if(--deg[adj[u][j]]==0)qq[tl++]=adj[u][j];
            }
            for(int i=0;i<c2;i++)
            {
                int u=ord[i];
                for(int j=0;j<(int)adj[u].size();j++)ways[adj[u][j]]=(ways[adj[u][j]]+ways[u])%80112002;
            }
            for(int i=1;i<=n;i++)brute[i]=(brute[i]+ways[i])%80112002;
        }
        for(int i=1;i<=n;i++)if(brute[i]!=dp[i])
        {
            printf("WA t=%d i=%d got=%d want=%d\n",t,i,dp[i],brute[i]);
            return 0;
        }
    }
    printf("随机 DAG 方案数对拍 100 组通过\n");
    return 0;
}
/* 用法：建图时统计 in[]，入度 0 的点先赋初值 dp[i]=1（方案数）或 dp[i]=0（最长路） */
