#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=105;

const double EPS=1e-8;
int n;
double a[N][N];// a[i][1..n] 系数，a[i][n+1] 常数项

// 返回值：0 唯一解（解在 a[i][n+1]），1 无穷多解，-1 无解
// O(n^3)，列主元高斯-约当消元
int gauss(int n)
{
    int r=1;// 当前主元行
    for(int c=1;c<=n;c++)
    {
        int pivot=r;
        for(int i=r+1;i<=n;i++)
            if(fabs(a[i][c])>fabs(a[pivot][c]))pivot=i;
        if(fabs(a[pivot][c])<EPS)continue;// 这一列全 0，跳过
        for(int j=1;j<=n+1;j++)swap(a[r][j],a[pivot][j]);
        double div=a[r][c];
        for(int j=1;j<=n+1;j++)a[r][j]/=div;
        for(int i=1;i<=n;i++)
        {
            if(i==r)continue;
            double mul=a[i][c];
            if(fabs(mul)<EPS)continue;
            for(int j=1;j<=n+1;j++)a[i][j]-=mul*a[r][j];
        }
        r++;
    }
    if(r<=n)
    {
        for(int i=r;i<=n;i++)
            if(fabs(a[i][n+1])>EPS)return -1;// 0=非0，无解
        return 1;// 有效方程数小于未知数个数，无穷多解
    }
    return 0;
}
