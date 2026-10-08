#include<bits/stdc++.h>
using namespace std;

// f 在区间内取值有限，eps>0 是绝对误差目标；返回积分估计，允许 l>r。
// 误差估计依赖函数足够光滑；不连续点、奇点应先拆段，振荡函数可能漏采样。
// 两半 Simpson 与整段比较，误差约为差值/15；分给子区间 eps/2，复用端点值。
// ok=false 表示深度耗尽或浮点中点无法细分，仍返回当前估计，不能当成精度达标。
// 每次细分只新增两个采样；时间 O(采样数)，栈空间 O(depth)。
template<class F>
double simpson(F f,double l,double r,double eps,bool &ok,int depth=25)
{
    assert(eps>0&&depth>=0&&isfinite(l)&&isfinite(r));
    ok=true;
    if(l==r)return 0;
    double sign=1;
    if(l>r)swap(l,r),sign=-1;
    auto area= [](double l, double r, double a, double b, double c)
    { return (r - l) * (a + 4 * b + c) / 6; };
    auto solve=[&](auto &&self,double l,double r,double a,double b,double c,double s,double e,int dep)->double
    {
        double m=l+(r-l)/2,x=l+(m-l)/2,y=m+(r-m)/2;
        if(x == l || x == m || y == m || y == r)
        {
            ok= false;
            return s;
        }
        double d=f(x),v=f(y),left=area(l,m,a,d,b),right=area(m,r,b,v,c);
        double delta=left+right-s;
        if(fabs(delta)<=15*e)return left+right+delta/15;
        if(!dep)
        {
            ok= false;
            return left + right + delta / 15;
        }
        return self(self,l,m,a,d,b,left,e/2,dep-1)+self(self,m,r,b,v,c,right,e/2,dep-1);
    };
    double m=l+(r-l)/2,a=f(l),b=f(m),c=f(r);
    return sign*solve(solve,l,r,a,b,c,area(l,r,a,b,c),eps,depth);
}
