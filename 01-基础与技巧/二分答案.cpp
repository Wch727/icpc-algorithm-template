#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 整数二分：O(log V) 次 check；浮点二分：固定迭代次数，精度 1e-7 左右
ll n,m;
ll a[N];

// 二分答案两个方向，都在 [l,r] 上做，返回 -1 表示区间内无解
// bs_first_true：check 满足 F F F T T T（随 x 变大由假变真），返回第一个真
//                常用于「最小化最大值」：check(x)=能否做到最大值 <= x
// bs_last_true ：check 满足 T T T F F F（随 x 变大由真变假），返回最后一个真
//                常用于「最大化最小值」：check(x)=能否做到最小值 >= x
// 取中点都用 l+((r-l)>>1)：区间含负数时 (l+r)>>1 不再等于数学中点，会漏解

template<typename F>
ll bs_first_true(ll l,ll r,F check)
{
    ll ans=-1;
    while(l<=r)
    {
        ll mid=l+((r-l)>>1);
        if(check(mid))ans=mid,r=mid-1;
        else l=mid+1;
    }
    return ans;
}

template<typename F>
ll bs_last_true(ll l,ll r,F check)
{
    ll ans=-1;
    while(l<=r)
    {
        ll mid=l+((r-l+1)>>1);// 右偏，保证 r 能收到 mid-1 且区间一定缩小
        if(check(mid))ans=mid,l=mid+1;
        else r=mid-1;
    }
    return ans;
}

// 有序数组中第一个 >= x 的位置，不存在返回 n+1（等价 lower_bound）
ll lower_id(ll* p,ll len,ll x)
{
    ll l=1,r=len+1;// 答案单调，右端点开区间 n+1 表示无解
    while(l<r)
    {
        ll mid=(l+r)>>1;
        if(p[mid]>=x)r=mid;
        else l=mid+1;
    }
    return l;
}

// 浮点二分：找满足 check 的最小值，固定 100 次迭代，精度够用且不会死循环
template<typename F>
double bs_double(double l,double r,F check)
{
    for(int i=1;i<=100;i++)
    {
        double mid=(l+r)/2;
        if(check(mid))r=mid;
        else l=mid;
    }
    return r;
}
