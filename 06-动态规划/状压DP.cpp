#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=110;
const int K=105;// 每行合法状态数上限(m<=10 时约 60 个)
const int NS=1<<15;// TSP 状态数上限
const int NEG=-0x3f3f3f3f;
int n,m,top;
int mp[N];// 地形压缩，1 表示山地不能放
int S[1<<10],cnt[1<<10];// 合法状态 和它里面 1 的个数
int dp[2][K][K];// 滚动两行，dp[i][j][k]：第 i 行状态 j、第 i-1 行状态 k
double px[20],py[20];// 吃奶酪的坐标，0 号是原点
double f[NS][16];// f[状态][当前点]，走完集合的最小距离

// O(log x)，二进制里 1 的个数
int popcnt(int x)
{
    int c=0;
    while(x)x&=x-1,c++;
    return c;
}

// O(n*top^3)，炮兵阵地：同行不能间隔 1 或 2 列，上下两行也不能同列
int artillery(int n,int m)
{
    top=0;
    for(int i=0;i<(1<<m);i++)// 预处理一行内的合法状态
        if((i&(i<<1))==0&&(i&(i<<2))==0)S[top]=i,cnt[top++]=popcnt(i);
    for(int j=0;j<top;j++)
        for(int k=0;k<top;k++)dp[0][j][k]=NEG;
    dp[0][0][0]=0;// 第 0 行只有空状态合法
    int ans=0;
    for(int i=1;i<=n;i++)
    {
        int cur=i&1,pre=cur^1;
        for(int j=0;j<top;j++)
            for(int k=0;k<top;k++)dp[cur][j][k]=NEG;
        for(int j=0;j<top;j++)// 本行状态
        {
            if(S[j]&mp[i])continue;// 山地不能放
            for(int k=0;k<top;k++)// 上一行状态
            {
                if(S[j]&S[k])continue;// 同列冲突
                int best=NEG;
                for(int l=0;l<top;l++)// 上上行状态
                    if(dp[pre][k][l]!=NEG&&(S[l]&S[j])==0)best=max(best,dp[pre][k][l]);
                if(best!=NEG)
                {
                    dp[cur][j][k]=best+cnt[j];
                    ans=max(ans,dp[cur][j][k]);
                }
            }
        }
    }
    return ans;
}

// O(1)，两点距离，i=0 表示原点
double dist(int i,int j)
{
    double dx=px[i]-px[j],dy=py[i]-py[j];
    return sqrt(dx*dx+dy*dy);
}

// O(2^n*n^2)，吃奶酪：从原点出发走遍所有点，最小总距离
// f[s][i] 表示已经走过集合 s、当前停在 i 的最小距离
double tsp(int n)
{
    if(!n)return 0;
    int full=1<<n;
    for(int s=0;s<full;s++)
        for(int i=0;i<n;i++)f[s][i]=1e18;
    for(int i=0;i<n;i++)f[1<<i][i]=dist(0,i+1);// 第一步从原点走到 i
    for(int s=1;s<full;s++)
        for(int i=0;i<n;i++)
            if(f[s][i]<1e18)
                for(int j=0;j<n;j++)
                    if(((s>>j)&1)==0)
                        f[s|(1<<j)][j]=min(f[s|(1<<j)][j],f[s][i]+dist(i+1,j+1));
    double ans=1e18;
    for(int i=0;i<n;i++)ans=min(ans,f[full-1][i]);
    return ans;
}
