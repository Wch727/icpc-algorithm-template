#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 例：n 件物品恰选 k 件，最大化 sum(a)/sum(b)，要求每个 b[i]>0。
// 如 a 是价值、b 是重量，求所选物品“总价值 / 总重量”的最大值。
// 求的是总和之比，不是各物品 a[i]/b[i] 的平均数，也不能直接选比值最大的 k 件。
// 猜比值 x：sum(a)/sum(b)>=x <=> sum(a-x*b)>=0。
// 因而取最大的 k 个 a[i]-x*b[i]：和 >=0 表示有方案达到 x，否则 x 太大。
// 二分边界取 min/max(a[i]/b[i])；O(100*n log n)。
const int N=100005;
int n,k; // 物品数、必须选取的件数，1<=k<=n；数组下标从 1 开始。
double a[N],b[N],c[N]; // a：分子属性（收益）；b：分母属性（成本，>0）；c：判定时的新权值。

bool check(double x)
{
    for(int i=1;i<=n;i++)c[i]=a[i]-x*b[i];
    sort(c+1,c+n+1,greater<double>());
    double sum=0;
    for(int i=1;i<=k;i++)sum+=c[i];
    return sum>=0;
}

double fractional()
{
    double l=a[1]/b[1],r=l;
    for(int i=2;i<=n;i++)l=min(l,a[i]/b[i]),r=max(r,a[i]/b[i]);
    for(int i=1;i<=100;i++)
    {
        double mid=(l+r)/2;
        if(check(mid))l=mid;
        else r=mid;
    }
    return l;
}
// 其他约束只改 check 内的选取方式，不可直接套“取最大的 k 项”。
