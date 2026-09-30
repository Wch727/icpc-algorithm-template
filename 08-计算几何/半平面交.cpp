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
double cross(Point a,Point b)
{
    return a.x*b.y-a.y*b.x;
}
struct Line
{
    Point p,v;
    double ang;
    Line(Point a,Point b):p(a),v(b-a),ang(atan2(v.y,v.x)){}
};
bool inside(Line a,Point p)
{
    return cross(a.v,p-a.p)>=-EPS;
}
Point meet(Line a,Line b)
{
    return a.p+a.v*(cross(b.v,b.p-a.p)/cross(b.v,a.v));
}
// O(n log n)，保留有向直线左侧；方向非零，交集须有界（可显式加题目边界）。
// 空集及线段/点交集返回空多边形；不以任意大方框冒充无界交集。
vector<Point> half_plane(vector<Line> a)
{
    sort(a.begin(),a.end(),[](Line x,Line y){return x.ang<y.ang;});
    vector<Line> b;
    for(Line l:a)
    {
        if(!b.empty()&&fabs(cross(b.back().v,l.v))<EPS&&b.back().v.x*l.v.x+b.back().v.y*l.v.y>0)
        {
            if(cross(b.back().v,l.p-b.back().p)>0)b.back()=l;
        }
        else b.push_back(l);
    }
    deque<Line> q;
    for(Line l:b)
    {
        while(q.size()>1&&!inside(l,meet(q[q.size()-2],q.back())))q.pop_back();
        while(q.size()>1&&!inside(l,meet(q[0],q[1])))q.pop_front();
        if(!q.empty()&&fabs(cross(q.back().v,l.v))<EPS)return {};
        q.push_back(l);
    }
    while(q.size()>2&&!inside(q.front(),meet(q[q.size()-2],q.back())))q.pop_back();
    while(q.size()>2&&!inside(q.back(),meet(q[0],q[1])))q.pop_front();
    if(q.size()<3||fabs(cross(q.front().v,q.back().v))<EPS)return {};
    vector<Point> p;
    for(int i=0;i<(int)q.size();i++)p.push_back(meet(q[i],q[(i+1)%q.size()]));
    return p;
}
double area(vector<Point> p)
{
    double s=0;
    for(int i=0;i<(int)p.size();i++)s+=cross(p[i],p[(i+1)%p.size()]);
    return fabs(s)/2;
}
// 独立对拍：逐条直线裁剪已知包围盒，O(n^2)。
vector<Point> brute(vector<Line> a)
{
    vector<Point> p={{-10,-10},{10,-10},{10,10},{-10,10}};
    for(Line l:a)
    {
        vector<Point> b;
        for(int i=0;i<(int)p.size();i++)
        {
            Point x=p[i],y=p[(i+1)%p.size()];
            bool u=inside(l,x),v=inside(l,y);
            if(u)b.push_back(x);
            if(u!=v)b.push_back(meet(Line(x,y),l));
        }
        p=b;
    }
    return p;
}
vector<Line> box()
{
    return {Line({-10,-10},{10,-10}),Line({10,-10},{10,10}),Line({10,10},{-10,10}),Line({-10,10},{-10,-10})};
}
