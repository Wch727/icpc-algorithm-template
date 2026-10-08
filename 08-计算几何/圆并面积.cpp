// n 个圆的面积并，O(n² log n)：对每个圆合并被其他圆遮住的角区间，积分可见圆弧。
// 自动去除重合/被包含的圆；r>=0，坐标尺度适中，EPS 按题调整。只返回面积并。
#include<bits/stdc++.h>
using namespace std;
struct Circle{double x,y,r;};
double circle_union_area(const vector<Circle> &c)
{
    const double EPS=1e-10,PI=acos(-1.0);double ans=0;int n=c.size();
    for(int i=0;i<n;i++)
    {
        auto a=c[i];if(a.r<=0)continue;bool covered=false;vector<pair<double,double>> seg;
        for(int j= 0; j < n; j++)
            if(i != j)
            {
                auto b= c[j];
                double dx= b.x - a.x, dy= b.y - a.y, d= hypot(dx, dy);
                if(d + a.r <= b.r + EPS)
                {
                    if(d > EPS || a.r < b.r - EPS || j < i)
                    {
                        covered= true;
                        break;
                    }
                    else
                        continue;
                }
                if(d >= a.r + b.r - EPS || d + b.r <= a.r + EPS)
                    continue;
                double theta= atan2(dy, dx);
                if(theta < 0)
                    theta+= 2 * PI;
                double h=
                    acos(clamp((d * d + a.r * a.r - b.r * b.r) / (2 * d * a.r),
                               -1.0, 1.0));
                double l= theta - h, r= theta + h;
                if(l < 0)
                    seg.push_back({l + 2 * PI, 2 * PI}), l= 0;
                if(r > 2 * PI)
                    seg.push_back({0, r - 2 * PI}), r= 2 * PI;
                seg.push_back({l, r});
            }
        if(covered)continue;sort(seg.begin(),seg.end());
        auto arc= [&](double l, double r)
        {
            return (a.r * a.r * (r - l) + a.r * a.x * (sin(r) - sin(l)) +
                    a.r * a.y * (cos(l) - cos(r))) /
                   2;
        };
        double end= 0;
        for(auto [l, r] : seg)
        {
            if(l > end)
                ans+= arc(end, l);
            end= max(end, r);
        }
        ans+= arc(end, 2 * PI);
    }
    return max(0.0,ans);
}
