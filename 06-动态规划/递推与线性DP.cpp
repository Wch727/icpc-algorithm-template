#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
const int INF=0x3f3f3f3f;
const int NEG=-0x3f3f3f3f;
int n,m;
int a[N];
char s[N],t[N];
int f[N],g[N],dp[N][N],best[N];// f/g 是滚动的线性 dp 数组
int tri[15][15];// 数字三角形
ll dp2[15][15];
int grid[15][15];// 网格路径（1 表示障碍）

// O(n^2)，最长上升子序列（严格递增），f[i] 表示以 i 结尾的 LIS 长度
// 转移：f[i]=max(f[j])+1，其中 j<i 且 a[j]<a[i]
int lis_n2()
{
    int ans=0;
    for(int i=1;i<=n;i++)
    {
        f[i]=1;
        for(int j=1;j<i;j++)
            if(a[j]<a[i])f[i]=max(f[i],f[j]+1);
        ans=max(ans,f[i]);
    }
    return ans;
}

// O(n log n)，LIS 贪心+二分：g[len] 存长度为 len 的上升子序列的最小结尾

int lis_nlogn()
{
    int len=0;
    for(int i=1;i<=n;i++)
    {
        int p=lower_bound(g+1,g+len+1,a[i])-g;
        g[p]=a[i];
        len=max(len,p);
    }
    return len;
}

// O(n)，最大子段和：f[i] 表示以 i 结尾的最大子段和，f[i]=max(f[i-1],0)+a[i]
int max_subarray()
{
    f[0]=0;
    int ans=INT_MIN;
    for(int i=1;i<=n;i++)
    {
        f[i]=max(f[i-1],0)+a[i];
        ans=max(ans,f[i]);
    }
    return ans;
}

// O(n^2)，数字三角形：从顶走到底的最大路径和，顺推
// 转移：dp[i][j]=max(dp[i-1][j-1],dp[i-1][j])+tri[i][j]
// 边界必须判掉：j=1 没有左上方，j=i 没有正上方（否则会把没算过的 0 当答案）
int triangle_max()
{
    dp[0][1]=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=i;j++)
        {
            if(j==1)dp[i][j]=dp[i-1][j]+tri[i][j];
            else if(j==i)dp[i][j]=dp[i-1][j-1]+tri[i][j];
            else dp[i][j]=max(dp[i-1][j-1],dp[i-1][j])+tri[i][j];
        }
    int ans=INT_MIN;
    for(int j=1;j<=n;j++)ans=max(ans,dp[n][j]);
    return ans;
}

// O(n^2)，网格路径数：只能往右/往下走，grid=1 是障碍
ll grid_paths()
{
    for(int i=0;i<=n;i++)
        for(int j=0;j<=m;j++)dp2[i][j]=0;
    dp2[1][1]=(grid[1][1]==1)?0:1;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
        {
            if(i==1&&j==1)continue;
            if(grid[i][j] == 1)
            {
                dp2[i][j]= 0;
                continue;
            }
            dp2[i][j]=dp2[i-1][j]+dp2[i][j-1];// 从上面或左面推过来
        }
    return dp2[n][m];
}

// O(n^2)，最长公共子序列长度
int lcs()
{
    for(int i=0;i<=n;i++)
        for(int j=0;j<=m;j++)
        {
            if(i == 0 || j == 0)
            {
                dp[i][j]= 0;
                continue;
            }
            if(s[i]==t[j])dp[i][j]=dp[i-1][j-1]+1;
            else dp[i][j]=max(dp[i-1][j],dp[i][j-1]);
        }
    return dp[n][m];
}

// O(n)，爬楼梯方案数（每次 1 或 2 级），f[i]=f[i-1]+f[i-2]
ll climb(int n)
{
    ll x=1,y=1;// f[0]=1,f[1]=1
    for(int i=2;i<=n;i++)
    {
        ll z=x+y;
        x=y,y=z;
    }
    return y;
}

// O(n)，数字解码方案数：'1'~'9' 单独成一位，'10'~'26' 两位成一位
ll decode_ways()
{
    ll f0=1,f1=(n>=1&&s[1]!='0')?1:0;// f[i-2],f[i-1]
    if(n==0)return 0;
    if(n==1)return f1;
    for(int i=2;i<=n;i++)
    {
        ll cur=0;
        if(s[i]!='0')cur+=f1;
        int two=(s[i-1]-'0')*10+(s[i]-'0');
        if(two>=10&&two<=26)cur+=f0;
        f0=f1,f1=cur;
    }
    return f1;
}
