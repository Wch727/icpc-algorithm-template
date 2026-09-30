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

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

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

// O(n^2)，两点距离，i=0 表示原点
double dist(int i,int j)
{
    double dx=px[i]-px[j],dy=py[i]-py[j];
    return sqrt(dx*dx+dy*dy);
}

// O(2^n*n^2)，吃奶酪：从原点出发走遍所有点，最小总距离
// f[s][i] 表示已经走过集合 s、当前停在 i 的最小距离
double tsp(int n)
{
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

// 暴力：逐行枚举所有 0..2^m-1 的状态
int brute_art(int row,int p1,int p2)
{
    if(row>n)return 0;
    int best=0;
    for(int x=0;x<(1<<m);x++)
    {
        if(x&(x<<1))continue;// 同行左右互攻
        if(x&(x<<2))continue;// 同行隔一个互攻
        if(x&mp[row])continue;// 不能放山地
        if(x&p1)continue;// 与上一行同列
        if(x&p2)continue;// 与上上行同列
        best=max(best,brute_art(row+1,x,p1)+popcnt(x));
    }
    return best;
}

// 暴力：枚举所有访问顺序
double brute_tsp(int n)
{
    int p[20];
    for(int i=1;i<=n;i++)p[i]=i;
    double ans=1e18;
    do
    {
        double cur=0;
        int last=0;
        for(int i=1;i<=n;i++)cur+=dist(last,p[i]),last=p[i];
        ans=min(ans,cur);
    }while(next_permutation(p+1,p+n+1));
    return ans;
}

int main()
{
    srand(20240606);
    printf("==== 固定样例 ====\n");
    n=2,m=3,mp[1]=0,mp[2]=0;// 两行全是平地，每行最多 1 门，共 2
    printf("炮兵阵地 2x3 全平地 : %d (期望 2)\n",artillery(n,m));
    n=5,m=4;
    const char *g[6]={"","PHPP","PPHH","PPPP","PHPP","PHHP"};
    for(int i=1;i<=n;i++)
    {
        mp[i]=0;
        for(int j=0;j<m;j++)
            if(g[i][j]=='H')mp[i]|=(1<<(m-1-j));// H 记成 1
    }
    printf("炮兵阵地 P2704 样例 5x4 : %d  暴力 %d (期望 6)\n",artillery(n,m),brute_art(1,0,0));
    n=4;
    px[1]=1,py[1]=1,px[2]=1,py[2]=-1,px[3]=-1,py[3]=1,px[4]=-1,py[4]=-1;
    printf("吃奶酪 4 个点 : %.2f (期望 7.41)\n",tsp(n));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(1,3),m=rndint(1,3);
        for(int i=1;i<=n;i++)
        {
            mp[i]=0;
            for(int j=0;j<m;j++)
                if(rndint(0,1))mp[i]|=(1<<j);// 随机山地
        }
        int ref=brute_art(1,0,0),cur=artillery(n,m);
        if(ref!=cur){bad++;printf("WA! 炮兵 轮%d n=%d m=%d ref=%d cur=%d\n",tt,n,m,ref,cur);break;}
    }
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(1,7);
        for(int i=1;i<=n;i++)px[i]=rndint(-5,5),py[i]=rndint(-5,5);
        double ref=brute_tsp(n),cur=tsp(n);
        if(fabs(ref-cur)>1e-9){bad++;printf("WA! 吃奶酪 轮%d n=%d ref=%.6f cur=%.6f\n",tt,n,ref,cur);break;}
    }
    if(!bad)printf("stress OK (炮兵 200 轮 + 吃奶酪 200 轮 全部通过)\n");
    return 0;
}
