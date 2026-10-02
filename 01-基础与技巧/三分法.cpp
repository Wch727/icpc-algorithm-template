#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 求最小值：最优点/最优区间左侧严格下降，右侧严格上升；求最大值则反过来。
// 多个局部极值、非最优处的平台不能直接套；单调函数也可用，最优点在端点。
// 均返回极值位置；函数值用 f/g 再算。若能直接求导或推出最优点，优先直接求。
double f(double x);

double ternary_min(double l,double r)
{
    for(int i=1;i<=100;i++)
    {
        double x=l+(r-l)/3,y=r-(r-l)/3;
        if(f(x)<f(y))r=y;
        else l=x;
    }
    return (l+r)/2;
}
// 100 轮后区间约为原来的 (2/3)^100，按初始范围及要求调整次数。
// 每轮调用两次 f；函数很贵时可用黄金分割复用一个点的函数值。

ll g(ll x); // 整数自变量的目标函数；值类型按题意改，r-l 须不溢出。
ll ternary_int(ll l,ll r)
{
    while(r-l>3)
    {
        ll x=l+(r-l)/3,y=r-(r-l)/3;
        if(g(x)<g(y))r=y;
        else l=x;
    }
    ll pos=l;
    for(ll i=l;i<r;i++)if(g(i+1)<g(pos))pos=i+1;
    return pos;
}
// 整数区间最后剩至多 4 个点，必须枚举；不能照搬固定轮数的实数版。
// 求最大值：实数版的 < 改成 >；整数版两处 < 都改成 >（循环条件不改）。
