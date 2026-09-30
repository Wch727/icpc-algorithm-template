#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 三分法：单峰（凸/凹）函数求极值，浮点 O(log(值域/eps))
// 凸函数（先减后增）求最小值；凹函数（先增后减）求最大值
template<typename F>
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
double f_val(int i,double x)
{
    return aa[i]*x*x+bb[i]*x+cc[i];
}
double F_val(double x)
{
    double res=f_val(0,x);
    for(int i=1;i<n;i++)res=max(res,f_val(i,x));
    return res;
}
