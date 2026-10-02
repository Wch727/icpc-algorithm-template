// 乘法换根不能默认模除可用：子项可能为 0，或模数非素；前后缀积能排除某个儿子而不求逆。
// 小边界连通 DP：三个不同边界点只有 5 种集合划分；边界重合先去重，再合并划分。
// 若统计连通块总数，维护(方案数,已封闭块数之和)，新封闭 t 块增加 t*两侧方案数乘积。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
const int NEG=-0x3f3f3f3f;
int n,m,root,dia;
int h[N],score[N],par[N],vis[N];// 欢乐值 学分 父亲 是否当过儿子
vector<int> son[N],adj[N];
int dp[N][2];// dp[x][0] x 不来 dp[x][1] x 来
int f[N][N];// f[x][j] 以 x 为根的子树里选 j 门课
int d1[N],d2[N];// 最长/次长向下链

// O(n)，没有上司的舞会
// dp[x][0] 表示 x 不参加时子树的最大欢乐值(儿子可选可不选)
// dp[x][1] 表示 x 参加时的最大值(儿子都不能参加)
void dfs_party(int x)
{
    dp[x][0]=0,dp[x][1]=h[x];
    for(int i=0;i<(int)son[x].size();i++)
    {
        int y=son[x][i];
        dfs_party(y);
        dp[x][0]+=max(dp[y][0],dp[y][1]);
        dp[x][1]+=dp[y][0];
    }
}

// O(n*m^2)，树上背包(选课)：f[x][j] 表示以 x 为根的子树里选 j 门课的最大收益
// 先把所有儿子合并上来，最后再把自己塞进去，体现"选子必须先选父"
// 恰好选 j 门；不可达为 NEG，支持负学分；虚拟根 0 不占名额。
void dfs_knap(int x)
{
    for(int t=0;t<=m;t++)f[x][t]=NEG;
    f[x][0]=0;
    for(int i=0;i<(int)son[x].size();i++)
    {
        int y=son[x][i];
        dfs_knap(y);
        for(int t=m;t>=0;t--)// 倒序，每个儿子只贡献一次
            for(int j=t;j>=0;j--)
                if(f[x][t-j]!=NEG&&f[y][j]!=NEG)f[x][t]=max(f[x][t],f[x][t-j]+f[y][j]);
    }
    if(x!=0)// 虚拟根 0 没有学分
        for(int t=m;t>0;t--)f[x][t]=f[x][t-1]==NEG?NEG:f[x][t-1]+score[x];
}

// O(n)，树形 dp 求直径（边数）：d1[x] 是最长向下链，d2[x] 是次长
void dfs_dia(int x)
{
    d1[x]=0,d2[x]=0;
    for(int i=0;i<(int)son[x].size();i++)
    {
        int y=son[x][i];
        dfs_dia(y);
        int t=d1[y]+1;
        if(t>d1[x])d2[x]=d1[x],d1[x]=t;
        else if(t>d2[x])d2[x]=t;
    }
    dia=max(dia,d1[x]+d2[x]);// 拐点在自己身上的最长路
}
