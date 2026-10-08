#include<bits/stdc++.h>
using namespace std;
const double EPS=1e-10;
struct Point
{
    double x,y;
    Point operator+(Point b)const{return {x+b.x,y+b.y};}
    Point operator-(Point b)const{return {x-b.x,y-b.y};}
    Point operator*(double k)const{return {x*k,y*k};}
};
struct Circle{Point p;double r;};// 所有接口要求 r>=0，EPS 按坐标尺度调整。
double dot(Point a,Point b){return a.x*b.x+a.y*b.y;}
Point turn(Point a){return {-a.y,a.x};}
double norm(Point a){return hypot(a.x,a.y);}
bool same(Point a,Point b){return norm(a-b)<=EPS;}

// 圆与无限直线 ab 的交点，0/1/2 个；a==b 时按一个点处理。
// r>=0，坐标尺度适中；EPS 是容差，极端尺度需要按题调整。
vector<Point> circle_line(Circle c,Point a,Point b)
{
    Point v=b-a;
    double len=norm(v);
    if(len <= EPS)
        return fabs(norm(a - c.p) - c.r) <= EPS ? vector<Point>{a}
                                                : vector<Point>{};
    v=v*(1/len);
    Point foot=a+v*dot(c.p-a,v);
    double dist=norm(foot-c.p);
    if(dist > c.r + EPS)
        return {};
    if(fabs(dist - c.r) <= EPS)
        return {foot};
    double h=sqrt(max(0.0,(c.r-dist)*(c.r+dist)));
    return {foot+v*h,foot-v*h};
}

// 圆圆交点：相离/内含 0 个，相切 1 个，相交 2 个；重合时 infinite=true。
// 两个重合的零半径圆只有一个交点；其余重合圆有无穷交点。
vector<Point> circle_circle(Circle a,Circle b,bool &infinite)
{
    infinite=false;
    Point v=b.p-a.p;
    double d=norm(v);
    if(d<=EPS)
    {
        if(fabs(a.r - b.r) > EPS)
            return {};
        if(a.r <= EPS)
            return {a.p};
        infinite=true;return {};
    }
    if(d > a.r + b.r + EPS || d < fabs(a.r - b.r) - EPS)
        return {};
    v=v*(1/d);
    double x=(d+(a.r-b.r)*(a.r+b.r)/d)/2;
    Point p=a.p+v*x;
    double h2=(a.r-x)*(a.r+x);
    if(fabs(d - a.r - b.r) <= EPS || fabs(d - fabs(a.r - b.r)) <= EPS)
        return {p};
    double h=sqrt(max(0.0,h2));
    return {p+turn(v)*h,p-turn(v)*h};
}

// 所有公切线，返回各线在两圆上的切点对；外切线与内切线统一计算。
// 切点重合时，切线过该点，方向垂直圆心连线；不能直接用两切点作方向。
// 相离两圆 4 条，外切 3 条，相交 2 条，内切 1 条，严格内含 0 条。
// 重合圆 infinite=true；不同圆心的两点圆返回连接它们的直线。
vector<pair<Point,Point>> common_tangents(Circle a,Circle b,bool &infinite)
{
    Point d=b.p-a.p;
    double len=norm(d);
    infinite=false;
    if(len <= EPS)
    {
        infinite= fabs(a.r - b.r) <= EPS;
        return {};
    }
    Point u=d*(1/len);
    vector<pair<Point,Point>> ans;
    for(int s : {1, -1})
    {
        double r=s*b.r,h=(a.r-r)/len;
        if(fabs(h)>1+EPS)continue;
        h=clamp(h,-1.0,1.0);
        double z=sqrt(max(0.0,1-h*h));
        for(int sign : {1, -1})
        {
            Point v=u*h+turn(u)*(z*sign);
            pair<Point,Point> line={a.p+v*a.r,b.p+v*r};
            bool duplicate=false;
            for(auto e : ans)
                if(same(e.first, line.first) && same(e.second, line.second))
                    duplicate= true;
            if(!duplicate)ans.push_back(line);
        }
    }
    return ans;
}

// 两圆交面积，O(1)；相离/相切、包含、重合、零半径均可。角度用弧度。
double circle_intersection_area(Circle a,Circle b)
{
    double d=norm(a.p-b.p),pi=acos(-1.0);
    if(d>=a.r+b.r)return 0;
    if(d<=fabs(a.r-b.r))return pi*min(a.r,b.r)*min(a.r,b.r);
    double x=acos(clamp((d*d+a.r*a.r-b.r*b.r)/(2*d*a.r),-1.0,1.0));
    double y=acos(clamp((d*d+b.r*b.r-a.r*a.r)/(2*d*b.r),-1.0,1.0));
    return a.r*a.r*(x-sin(2*x)/2)+b.r*b.r*(y-sin(2*y)/2);
}
// 圆与简单多边形交面积，O(n)：切分每条边，内部用三角形、外部用扇形。
// 顶点按边界顺序，不要求凸；逆时针返回正有向面积，顺时针为负，通常取 fabs。
double circle_polygon_area(Circle c,const vector<Point> &p)
{
    auto cross= [](Point a, Point b) { return a.x * b.y - a.y * b.x; };
    double ans= 0;
    if(c.r<=0)return 0;
    for(int i=0;i<(int)p.size();i++)
    {
        Point a=p[i]-c.p,b=p[(i+1)%p.size()]-c.p,d=b-a;
        double A=dot(d,d);if(A==0)continue;
        vector<double> ts={0,1};double B=dot(a,d),C=dot(a,a)-c.r*c.r,D=B*B-A*C;
        if(D > 0)
            for(double t : {(-B - sqrt(D)) / A, (-B + sqrt(D)) / A})
                if(t > 0 && t < 1)
                    ts.push_back(t);
        sort(ts.begin(),ts.end());
        for(int j=1;j<(int)ts.size();j++)
        {
            Point u=a+d*ts[j-1],v=a+d*ts[j],mid=a+d*((ts[j-1]+ts[j])/2);
            if(dot(mid,mid)<=c.r*c.r)ans+=cross(u,v)/2;
            else ans+=c.r*c.r*atan2(cross(u,v),dot(u,v))/2;
        }
    }
    return ans;
}
