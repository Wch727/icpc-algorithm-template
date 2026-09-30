// 适用：静态区间求和、离线批量区间加；下标均从 1 开始，端点均为闭区间。
// 二维需保留第 0 行/列和 x2+1、y2+1 哨兵，合法坐标及其后一格都须小于 N。
// 原值和累计增量须在 ll 内；二维还原会覆盖输入，再次执行会重复累加。
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

// O(n)，由 a[1..n] 建 pre；pre[0]=0 让 l=1 的查询无需特判。
void build_pre()// 一维前缀和
{
    pre[0]=0;
    for(int i=1;i<=n;i++)pre[i]=pre[i-1]+a[i];
}

// O(1)，查询 [l,r]，1<=l<=r<=n；须先 build_pre，修改原数组后重新建表。
ll query_1d(int l,int r)// 区间和，注意 l<=r
{
    return pre[r]-pre[l-1];
}

// O(n)，由 a[1..n] 建 dif；相邻差消去此前前缀，要求 n>=1。
void build_dif()// 一维差分数组
{
    dif[1]=a[1];
    for(int i=2;i<=n;i++)dif[i]=a[i]-a[i-1];
}

// O(1)，对闭区间 [l,r] 加 v；l 开始生效，r+1 撤销，查询前统一还原。
void add_1d(int l,int r,ll v)// 区间加 v
{
    dif[l]+=v;
    if(r+1<=n)dif[r+1]-=v;
}

// O(n)，对 dif 做前缀和写回 a；多个修改累计后调用，不会覆盖 dif。
void get_1d()// 差分还原原数组
{
    ll cur=0;
    for(int i=1;i<=n;i++)cur+=dif[i],a[i]=cur;
}

// O(nm)，s[1..n][1..m] 输入原值并原地改成前缀和；第 0 行/列须为零。
void build_pre2()// 二维前缀和
{
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            s[i][j]=s[i-1][j]+s[i][j-1]-s[i-1][j-1]+s[i][j];
}

// O(1)，查询左上 (x1,y1)、右下 (x2,y2) 的闭矩形；容斥补回重复减去的角。
ll query_2d(int x1,int y1,int x2,int y2)// 子矩形和
{
    return s[x2][y2]-s[x1-1][y2]-s[x2][y1-1]+s[x1-1][y1-1];
}

// O(1)，对闭矩形 [(x1,y1),(x2,y2)] 加 v；d 须先初始化为差分。
void add_2d(int x1,int y1,int x2,int y2,ll v)// 子矩形加 v
{
    d[x1][y1]+=v;
    d[x2+1][y1]-=v;
    d[x1][y2+1]-=v;
    d[x2+1][y2+1]+=v;
}

// O(nm)，原地积分 d 得各点最终值；上方与左方的重叠部分需减去一次。
void get_2d()// 二维差分还原
{
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            d[i][j]+=d[i-1][j]+d[i][j-1]-d[i-1][j-1];
}
