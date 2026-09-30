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

int main()
{
    // 自测 1：手造二分图 左 1,2,3 右 1,2,3；边 1-1 1-2 2-1 3-2，最大匹配 2
    n=3;
    adj[1].push_back(1),adj[1].push_back(2);
    adj[2].push_back(1);
    adj[3].push_back(2);
    printf("手造图 最大匹配=%d（期望 2）\n",hungarian(3));
    // 自测 2：完全二分图 K3,3，最大匹配 3
    for(int i=1;i<=n;i++)adj[i].clear();
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)adj[i].push_back(j);
    printf("K3,3 最大匹配=%d（期望 3）\n",hungarian(3));
    // 自测 3：随机二分图，与「枚举所有匹配方案」暴力对拍
    for(int t=1;t<=300;t++)
    {
        n=rand()%6+1;
        int mm=0,ea[40],eb[40];
        for(int i=1;i<=n;i++)adj[i].clear();
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(rand()%2)
                {
                    adj[i].push_back(j);
                    mm++;
                    ea[mm]=i,eb[mm]=j;
                }
        int got=hungarian(n);
        // 暴力：枚举每条边选/不选，检查是否构成匹配，取最大边数
        int want=0;
        for(int mask=0;mask<(1<<mm);mask++)
        {
            int lu[8]={0},rv[8]={0},c=0,ok=1;
            for(int i=1;i<=mm&&ok;i++)
                if(mask>>(i-1)&1)
                {
                    if(lu[ea[i]]||rv[eb[i]])ok=0;
                    else lu[ea[i]]=1,rv[eb[i]]=1,c++;
                }
            if(ok)want=max(want,c);
        }
        if(got!=want)
        {
            printf("WA t=%d n=%d got=%d want=%d\n",t,n,got,want);
            return 0;
        }
    }
    printf("匈牙利 与暴力枚举 300 组通过\n");
    return 0;
}
/* 复杂度 O(n*m)；左部点数用 nl 单独传，右部点编号 1..n
   二分图最小点覆盖 = 最大匹配；最大独立集 = n - 最大匹配（König 定理） */
