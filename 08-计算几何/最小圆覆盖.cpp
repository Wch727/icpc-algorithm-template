#include<bits/stdc++.h>
using namespace std;
const double EPS=1e-8;
struct Point
{
    double x,y;
};
struct Circle
{
    Point p;
    double r;
};
double dis(Point a,Point b)
{
    return hypot(a.x-b.x,a.y-b.y);
}
bool inside(Circle c,Point p)
{
    return dis(c.p,p)<=c.r+EPS;
}
Circle two(Point a,Point b)
{
    Point p={(a.x+b.x)/2,(a.y+b.y)/2};
    return {p,dis(a,b)/2};
}
Circle three(Point a,Point b,Point c)
{
    double x=b.x-a.x,y=b.y-a.y,u=c.x-a.x,v=c.y-a.y;
    double d=2*(x*v-y*u);
    if(fabs(d)<1e-12)
    {
        Circle z=two(a,b);
        if(dis(a,c)>2*z.r)z=two(a,c);
        if(dis(b,c)>2*z.r)z=two(b,c);
        return z;// 共线时最远两点作直径
    }
    double s=x*x+y*y,t=u*u+v*v;
    Point p={a.x+(s*v-t*y)/d,a.y+(x*t-u*s)/d};
    return {p,dis(p,a)};
}
// 随机增量，期望 O(n)；空集半径 0，重复/共线点可用。
Circle min_circle(vector<Point> a)
{
    mt19937 rng(19260817);
    shuffle(a.begin(),a.end(),rng);
    Circle c={{0,0},0};
    for(int i=0;i<(int)a.size();i++)
        if(!inside(c,a[i]))
        {
            c={a[i],0};
            for(int j=0;j<i;j++)
                if(!inside(c,a[j]))
                {
                    c=two(a[i],a[j]);
                    for(int k=0;k<j;k++)
                        if(!inside(c,a[k]))c=three(a[i],a[j],a[k]);
                }
        }
    return c;
}
