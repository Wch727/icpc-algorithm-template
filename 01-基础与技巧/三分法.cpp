// 适用：连续单峰函数求极值；返回极值位置，函数值需再调用 f。
// 固定次数控制精度；非单峰或大段平台的函数不能直接套，函数求值代价要计入总复杂度。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 三分法：单峰（凸/凹）函数求极值，浮点 O(log(值域/eps))
// 凸函数（先减后增）求最小值；凹函数（先增后减）求最大值
template<typename F>
// O(iter * 单次求值代价)，在实数闭区间 [l,r] 找最小值位置；f 单峰，iter 为迭代次数。
double trisearch_min(double l,double r,F f,int iter=200)
{
    for(int i=1;i<=iter;i++)
    {
        double lm=l+(r-l)/3.0,rm=r-(r-l)/3.0;
        if(f(lm)<f(rm))r=rm;// 最小值在左边
        else l=lm;
    }
    return (l+r)/2;
}

template<typename F>
// O(iter * 单次求值代价)，在实数闭区间 [l,r] 找最大值位置；两内点比较排除无解一侧。
double trisearch_max(double l,double r,F f,int iter=200)
{
    for(int i=1;i<=iter;i++)
    {
        double lm=l+(r-l)/3.0,rm=r-(r-l)/3.0;
        if(f(lm)>f(rm))r=rm;// 最大值在左边
        else l=lm;
    }
    return (l+r)/2;
}

// 洛谷 P1883：F(x)=max(a_i x^2+b_i x+c_i)，凸函数，三分求最小值
// 这里用 double：本机 g++ 15.2 的 long double 比较大小时结果不可靠，double 精度够用
int T,n;
double aa[10005],bb[10005],cc[10005];
// O(1)，计算编号 i 的二次函数在 x 处的值；系数数组为 0-indexed，0<=i<n。
double f_val(int i,double x)
{
    return aa[i]*x*x+bb[i]*x+cc[i];
}
// O(n)，取 n 个函数在 x 处的最大值；要求 n>0、各函数凸才能保证最大包络凸。
double F_val(double x)
{
    double res=f_val(0,x);
    for(int i=1;i<n;i++)res=max(res,f_val(i,x));
    return res;
}
