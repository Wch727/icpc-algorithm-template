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
