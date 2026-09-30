#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;

// 一维/二维 前缀和与差分，预处理 O(nm)，单次查询 O(1)
int n,m,q;
ll a[N];// 原数组 1..n
ll pre[N];// 一维前缀和
ll dif[N];// 一维差分
ll s[N][N];// 二维前缀和：s[i][j] 表示 (1,1)-(i,j) 的和
ll d[N][N];// 二维差分：d[i][j] 的二维前缀和就是该点最终值

void build_pre()// 一维前缀和
{
    pre[0]=0;
    for(int i=1;i<=n;i++)pre[i]=pre[i-1]+a[i];
}

ll query_1d(int l,int r)// 区间和，注意 l<=r
{
    return pre[r]-pre[l-1];
}

void build_dif()// 一维差分数组
{
    dif[1]=a[1];
    for(int i=2;i<=n;i++)dif[i]=a[i]-a[i-1];
}

void add_1d(int l,int r,ll v)// 区间加 v
{
    dif[l]+=v;
    if(r+1<=n)dif[r+1]-=v;
}

void get_1d()// 差分还原原数组
{
    ll cur=0;
    for(int i=1;i<=n;i++)cur+=dif[i],a[i]=cur;
}

void build_pre2()// 二维前缀和
{
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            s[i][j]=s[i-1][j]+s[i][j-1]-s[i-1][j-1]+s[i][j];
}

ll query_2d(int x1,int y1,int x2,int y2)// 子矩形和
{
    return s[x2][y2]-s[x1-1][y2]-s[x2][y1-1]+s[x1-1][y1-1];
}

void add_2d(int x1,int y1,int x2,int y2,ll v)// 子矩形加 v
{
    d[x1][y1]+=v;
    d[x2+1][y1]-=v;
    d[x1][y2+1]-=v;
    d[x2+1][y2+1]+=v;
}

void get_2d()// 二维差分还原
{
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            d[i][j]+=d[i-1][j]+d[i][j-1]-d[i-1][j-1];
}

int main()
{
    srand(20240515);
    n=6,m=6;
    // 自测1：一维前缀和 / 差分 与暴力对拍
    for(int t=1;t<=200;t++)
    {
        n=rand()%20+1;
        for(int i=1;i<=n;i++)a[i]=rand()%21-10;
        build_pre();
        for(int l=1;l<=n;l++)
            for(int r=l;r<=n;r++)
            {
                ll sum=0;
                for(int i=l;i<=r;i++)sum+=a[i];
                if(query_1d(l,r)!=sum)
                {
                    printf("fail 1d pre\n");
                    return 0;
                }
            }
        ll cpy[N];
        for(int i=1;i<=n;i++)cpy[i]=a[i];
        build_dif();
        for(int k=1;k<=5;k++)// 随机若干次区间加
        {
            int l=rand()%n+1,r=rand()%n+1;
            if(l>r)swap(l,r);
            ll v=rand()%11-5;
            add_1d(l,r,v);
            for(int i=l;i<=r;i++)cpy[i]+=v;
        }
        get_1d();
        for(int i=1;i<=n;i++)
            if(a[i]!=cpy[i])
            {
                printf("fail 1d dif\n");
                return 0;
            }
    }
    printf("1d pre/dif self-check OK\n");

    // 自测2：二维前缀和样例 + 二维差分与暴力对拍
    n=m=5;
    memset(s,0,sizeof(s));
    int v[6][6]={{0,0,0,0,0,0},
                 {0,1,2,3,4,5},
                 {0,6,7,8,9,10},
                 {0,11,12,13,14,15},
                 {0,16,17,18,19,20},
                 {0,21,22,23,24,25}};
    ll brute[6][6];
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            s[i][j]=v[i][j],brute[i][j]=v[i][j];
    build_pre2();
    printf("rect(2,2,4,4)=%lld\n",query_2d(2,2,4,4));// 7+8+9+12+13+14+17+18+19=117
    for(int t=1;t<=200;t++)
    {
        memset(d,0,sizeof(d));
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                brute[i][j]=0;
        int k=rand()%6+1;
        for(int z=1;z<=k;z++)
        {
            int x1=rand()%n+1,y1=rand()%m+1,x2=rand()%n+1,y2=rand()%m+1;
            if(x1>x2)swap(x1,x2);
            if(y1>y2)swap(y1,y2);
            ll val=rand()%21-10;
            add_2d(x1,y1,x2,y2,val);
            for(int i=x1;i<=x2;i++)
                for(int j=y1;j<=y2;j++)
                    brute[i][j]+=val;
        }
        get_2d();
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                if(d[i][j]!=brute[i][j])
                {
                    printf("fail 2d dif at (%d,%d)\n",i,j);
                    return 0;
                }
    }
    printf("2d pre/dif self-check OK\n");
    return 0;
}
/* 样例（洛谷 P3397 地毯，用二维差分还原每格覆盖数）
3 2
1 1 2 2
2 2 3 3
输出：
1 1 0
1 2 1
0 1 1
*/
