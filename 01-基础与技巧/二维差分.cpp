#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N=1005;
int n,m;
ll a[N][N],d[N][N]; // 1-indexed，预留第 0 行/列和 n+1、m+1

// 原数组非零时先建差分；原数组全零时直接清 d，再做修改。
void build()
{
    memset(d,0,sizeof d);
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            d[i][j]=a[i][j]-a[i-1][j]-a[i][j-1]+a[i-1][j-1];
}

// O(1)，闭矩形 [x1,x2] × [y1,y2] 加 v。
void add(int x1,int y1,int x2,int y2,ll v)
{
    d[x1][y1]+=v;
    d[x2+1][y1]-=v;
    d[x1][y2+1]-=v;
    d[x2+1][y2+1]+=v;
}

// O(nm)，所有修改结束后还原一次，结果写入 a。
void restore()
{
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
        {
            d[i][j]+=d[i-1][j]+d[i][j-1]-d[i-1][j-1];
            a[i][j]=d[i][j];
        }
}
// 流程：build -> 多次 add -> restore；不要重复 restore。
