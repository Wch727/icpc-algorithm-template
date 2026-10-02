#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=505;// 断环成链后要开 2n
const int INF=0x3f3f3f3f;
const int NEG=-0x3f3f3f3f;
int n;
int a[N],sum[N];// a 存石子/项链，sum 是前缀和
int gmin[N][N],gmax[N][N];// 环形石子合并
int emax[N][N];// 能量项链
int pmax[N][N],pmin[N][N];// 多边形游戏：有负数乘法，必须同时存 max 和 min
int val[N];// 多边形顶点
char op[N];// 多边形边上的运算符，op[i] 连 val[i] 和 val[i+1]

// O(n^3)，环形石子合并最小代价
// f[l][r] 表示把 [l,r] 合并成一堆的最小代价，枚举断点 k
// 断环成链 a[i+n]=a[i]，答案是 min f[i][i+n-1]
int stone_merge_min(int n,int a[])
{
    for(int i=1;i<=n;i++)a[i+n]=a[i];
    for(int i=1;i<=2*n;i++)sum[i]=sum[i-1]+a[i];
    for(int i=1;i<=2*n;i++)
        for(int j=1;j<=2*n;j++)gmin[i][j]=(i==j)?0:INF;// 边界：一堆不用合并
    for(int len=2;len<=n;len++)// 必须按区间长度从小到大
        for(int l=1;l+len-1<=2*n;l++)
        {
            int r=l+len-1;
            for(int k=l;k<r;k++)
                gmin[l][r]=min(gmin[l][r],gmin[l][k]+gmin[k+1][r]+sum[r]-sum[l-1]);
        }
    int ans=INF;
    for(int i=1;i<=n;i++)ans=min(ans,gmin[i][i+n-1]);
    return ans;
}

// O(n^3)，环形石子合并最大代价
int stone_merge_max(int n,int a[])
{
    for(int i=1;i<=n;i++)a[i+n]=a[i];
    for(int i=1;i<=2*n;i++)sum[i]=sum[i-1]+a[i];
    for(int i=1;i<=2*n;i++)
        for(int j=1;j<=2*n;j++)gmax[i][j]=0;
    for(int len=2;len<=n;len++)
        for(int l=1;l+len-1<=2*n;l++)
        {
            int r=l+len-1;
            for(int k=l;k<r;k++)
                gmax[l][r]=max(gmax[l][r],gmax[l][k]+gmax[k+1][r]+sum[r]-sum[l-1]);
        }
    int ans=0;
    for(int i=1;i<=n;i++)ans=max(ans,gmax[i][i+n-1]);
    return ans;
}

// O(n^3)，能量项链：emax[l][r] 表示 [l,r] 合成一颗珠子的最大能量
// 合并 (l..k) 和 (k..r) 时释放 a[l]*a[k]*a[r]，注意端点共用
int energy_necklace(int n,int a[])
{
    for(int i=1;i<=n+1;i++)a[i+n]=a[i];// 链要开到 2n+1
    for(int i=1;i<=2*n+1;i++)
        for(int j=1;j<=2*n+1;j++)emax[i][j]=0;
    for(int len=2;len<=n;len++)
        for(int l=1;l+len<=2*n+1;l++)
        {
            int r=l+len;
            for(int k=l+1;k<r;k++)
                emax[l][r]=max(emax[l][r],emax[l][k]+emax[k][r]+a[l]*a[k]*a[r]);
        }
    int ans=0;
    for(int i=1;i<=n;i++)ans=max(ans,emax[i][i+n]);
    return ans;
}

// O(n^4)，多边形游戏：枚举删哪条边(旋转)，再区间 dp 求最大值
// 乘法遇到负数会让"最小"翻成"最大"，所以 max 由 max*max / max*min / min*max / min*min 取
int polygon_game(int n,int val[],char op[])
{
    int ans=NEG;
    for(int cut=1;cut<=n;cut++)// 删掉第 cut 条边，链从 val[cut+1] 开始
    {
        int v[N];char o[N];
        for(int i=1;i<=n;i++)v[i]=val[(cut+i-1)%n+1];
        for(int i=1;i<=n-1;i++)o[i]=op[(cut+i-1)%n+1];
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)pmax[i][j]=NEG,pmin[i][j]=INF;
        for(int i=1;i<=n;i++)pmax[i][i]=pmin[i][i]=v[i];
        for(int len=2;len<=n;len++)
            for(int l=1;l+len-1<=n;l++)
            {
                int r=l+len-1;
                for(int k=l;k<r;k++)
                {
                    int cand[4],cnt=0;
                    if(o[k]=='+')
                    {
                        cand[cnt++]=pmax[l][k]+pmax[k+1][r];
                        cand[cnt++]=pmin[l][k]+pmin[k+1][r];
                    }
                    else
                    {
                        cand[cnt++]=pmax[l][k]*pmax[k+1][r];
                        cand[cnt++]=pmax[l][k]*pmin[k+1][r];
                        cand[cnt++]=pmin[l][k]*pmax[k+1][r];
                        cand[cnt++]=pmin[l][k]*pmin[k+1][r];
                    }
                    for(int t=0;t<cnt;t++)
                        pmax[l][r]=max(pmax[l][r],cand[t]),pmin[l][r]=min(pmin[l][r],cand[t]);
                }
            }
        ans=max(ans,pmax[1][n]);
    }
    return ans;
}
