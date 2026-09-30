#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
vector<int> adj[N];
int col[N];// 0 未染色，1 / 2 两种颜色（黑白）
int c1,c2;// 当前连通块两种颜色的点数

int dfs_color(int u,int c)// 深搜染色，返回 0 表示出现奇环（不是二分图）
{
    col[u]=c;
    if(c==1)c1++;
    else c2++;
    for(int i=0;i<(int)adj[u].size();i++)
    {
        int v=adj[u][i];
        if(!col[v])
        {
            if(!dfs_color(v,3-c))return 0;// 3-c 就是换成另一种颜色
        }
        else if(col[v]==c)return 0;// 相邻同色 -> 矛盾
    }
    return 1;
}

int bfs_color(int s)// 宽搜染色，写法与 dfs 等价
{
    queue<int> q;
    col[s]=1,c1=1,c2=0;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int i=0;i<(int)adj[u].size();i++)
        {
            int v=adj[u][i];
            if(!col[v])
            {
                col[v]=3-col[u];
                if(col[v]==1)c1++;
                else c2++;
                q.push(v);
            }
            else if(col[v]==col[u])return 0;
        }
    }
    return 1;
}

int check_bipartite(int mode)// 返回 1 是二分图；顺带统计每个连通块人数少的颜色总和
{
    for(int i=1;i<=n;i++)col[i]=0;
    int ans=0;
    for(int i=1;i<=n;i++)
        if(!col[i])
        {
            c1=c2=0;
            int ok=(mode==0?dfs_color(i,1):bfs_color(i));
            if(!ok)return -1;
            ans+=min(c1,c2);// P1330 型题：每个连通块取颜色少的一边
        }
    return ans;
}

int main()
{
    // 自测 1：偶环 1-2-3-4-1 是二分图（每块最少取 2 人）
    n=4,m=4;
    int a[10]={0,1,2,3,4},b[10]={0,2,3,4,1};
    for(int i=1;i<=m;i++)adj[a[i]].push_back(b[i]),adj[b[i]].push_back(a[i]);
    printf("偶环 dfs=%d bfs=%d（期望 2 2）\n",check_bipartite(0),check_bipartite(1));
    // 自测 2：奇环 1-2-3-1 不是二分图，返回 -1
    n=3,m=3;
    for(int i=1;i<=n;i++)adj[i].clear();
    int a2[5]={0,1,2,3},b2[5]={0,2,3,1};
    for(int i=1;i<=m;i++)adj[a2[i]].push_back(b2[i]),adj[b2[i]].push_back(a2[i]);
    printf("奇环 dfs=%d bfs=%d（期望 -1 -1）\n",check_bipartite(0),check_bipartite(1));
    // 自测 3：随机图，dfs 染色结果与「暴力枚举所有 2^n 种染色是否合法」对拍
    for(int t=1;t<=300;t++)
    {
        n=rand()%7+1;
        for(int i=1;i<=n;i++)adj[i].clear();
        int ea[25],eb[25],mm=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
                if(rand()%2)
                {
                    adj[i].push_back(j),adj[j].push_back(i);
                    mm++;
                    ea[mm]=i,eb[mm]=j;
                }
        int r1=check_bipartite(0);
        int r2=check_bipartite(1);
        int brute=0;// 0 不是二分图，1 是
        for(int mask=0;mask<(1<<n)&&!brute;mask++)
        {
            int good=1;
            for(int i=1;i<=mm;i++)
                if(((mask>>(ea[i]-1))&1)==((mask>>(eb[i]-1))&1))
                {
                    good=0;
                    break;
                }
            if(good)brute=1;
        }
        if((r1==-1)!=(brute==0)||(r2==-1)!=(brute==0))
        {
            printf("WA t=%d dfs=%d bfs=%d brute=%d\n",t,r1,r2,brute);
            return 0;
        }
        if(r1!=r2)
        {
            printf("WA dfs/bfs t=%d %d %d\n",t,r1,r2);
            return 0;
        }
    }
    printf("随机图 二分图判定与暴力对拍 300 组通过\n");
    return 0;
}
/* 无向图判二分图；孤立点算一个连通块，答案贡献 0 */
