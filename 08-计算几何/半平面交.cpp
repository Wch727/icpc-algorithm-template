#include<bits/stdc++.h>
using namespace std;
const double EPS=1e-9;
struct Point
{
    double x,y;
    Point operator+(Point b)const{return {x+b.x,y+b.y};}
    Point operator-(Point b)const{return {x-b.x,y-b.y};}
    Point operator*(double k)const{return {x*k,y*k};}
};
double cross(Point a,Point b){return a.x*b.y-a.y*b.x;}
struct Line
{
    Point p,v;
    double ang;
    Line(Point a, Point b) : p(a), v(b - a), ang(atan2(v.y, v.x))
    {
        if(ang < 0)
            ang+= 2 * acos(-1.0);
    }
};
bool inside(Line a,Point p){return cross(a.v,p-a.p)>=-EPS;}
Point meet(Line a,Line b){return a.p+a.v*(cross(b.v,b.p-a.p)/cross(b.v,a.v));}
// O(n log n)，保留有向直线左侧；方向非零，交集须有界（可显式加题目边界）。
// 空集及线段/点交集返回空多边形；不以任意大方框冒充无界交集。
vector<Point> half_plane(vector<Line> a)
{
    sort(a.begin(), a.end(), [](Line x, Line y) { return x.ang < y.ang; });
    vector<Line> b;
    for(Line l:a)
    {
        if(!b.empty()&&fabs(cross(b.back().v,l.v))<EPS&&b.back().v.x*l.v.x+b.back().v.y*l.v.y>0)
        {
            if(cross(b.back().v,l.p-b.back().p)>0)b.back()=l;
        }
        else b.push_back(l);
    }
    if(b.size()>1&&fabs(cross(b.front().v,b.back().v))<EPS&&
       b.front().v.x*b.back().v.x+b.front().v.y*b.back().v.y>0)
    {
        if(cross(b.front().v,b.back().p-b.front().p)>0)b.front()=b.back();
        b.pop_back();
    }
    deque<Line> q;
    for(Line l:b)
    {
        while(q.size()>1&&!inside(l,meet(q[q.size()-2],q.back())))q.pop_back();
        while(q.size()>1&&!inside(l,meet(q[0],q[1])))q.pop_front();
        if(!q.empty() && fabs(cross(q.back().v, l.v)) < EPS)
            return {};
        q.push_back(l);
    }
    while(q.size()>2&&!inside(q.front(),meet(q[q.size()-2],q.back())))q.pop_back();
    while(q.size()>2&&!inside(q.back(),meet(q[0],q[1])))q.pop_front();
    if(q.size() < 3 || fabs(cross(q.front().v, q.back().v)) < EPS)
        return {};
    vector<Point> p;
    for(int i=0;i<(int)q.size();i++)p.push_back(meet(q[i],q[(i+1)%q.size()]));
    double twice=0;
    for(int i=0;i<(int)p.size();i++)twice+=cross(p[i],p[(i+1)%p.size()]);
    if(fabs(twice) < EPS)
        return {};
    return p;
}
double area(vector<Point> p)
{
    double s=0;
    for(int i=0;i<(int)p.size();i++)s+=cross(p[i],p[(i+1)%p.size()]);
    return fabs(s)/2;
}
