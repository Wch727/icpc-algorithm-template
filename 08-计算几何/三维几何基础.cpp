#include<bits/stdc++.h>
using namespace std;
const double EPS=1e-9;
struct Point
{
    double x,y,z;
    Point operator+(Point b)const{return {x+b.x,y+b.y,z+b.z};}
    Point operator-(Point b)const{return {x-b.x,y-b.y,z-b.z};}
    Point operator*(double k)const{return {x*k,y*k,z*k};}
};
double dot(Point a,Point b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
Point cross(Point a,Point b)
{
    return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};
}
double len(Point a)
{
    return sqrt(dot(a,a));
}
struct Plane
{
    Point p,n;// 平面上一点与非零法向量
};
// O(1)，三点建平面；共线或重复点返回 false。
bool make_plane(Point a,Point b,Point c,Plane &s)
{
    s={a,cross(b-a,c-a)};
    return len(s.n)>EPS;
}
// O(1)，点到平面距离及垂足；退化平面返回 false。
bool project(Point p,Plane s,Point &q,double &d)
{
    double v=dot(s.n,s.n);
    if(v<EPS*EPS)return false;
    double t=dot(p-s.p,s.n)/v;
    q=p-s.n*t,d=fabs(t)*sqrt(v);
    return true;
}
// O(1)，直线 a+t*v 与平面求交；0 无交，1 唯一交，2 直线在平面内，-1 退化。
int line_plane(Point a,Point v,Plane s,Point &q)
{
    if(len(v)<EPS||len(s.n)<EPS)return -1;
    double x=dot(s.n,v),y=dot(s.n,s.p-a);
    if(fabs(x)<=EPS*len(s.n)*len(v))return fabs(y)<=EPS*len(s.n)?2:0;
    q=a+v*(y/x);
    return 1;
}
// O(1)，点到直线距离；零方向退化成点。
double point_line(Point p,Point a,Point v)
{
    if(len(v)<EPS)return len(p-a);
    return len(cross(p-a,v))/len(v);
}

// 绕坐标轴正方向按右手法则转 times*90 度，axis=0/1/2 为 x/y/z；避免 sin/cos 误差。
Point rotate_axis(Point p,int axis,int times)
{
    assert(0<=axis&&axis<3);times=(times%4+4)%4;
    while(times--)
        if(axis==0)p={p.x,-p.z,p.y};
        else if(axis==1)p={p.z,p.y,-p.x};
        else p={-p.y,p.x,p.z};
    return p;
}
// 非原点轴先减轴上基点，再旋转再加回；维护旋转矩阵时，列向量先 A 后 B 为 B*A。
// 90 度矩阵：Rx=[1 0 0;0 0 -1;0 1 0]，Ry=[0 0 1;0 1 0;-1 0 0]，Rz=[0 -1 0;1 0 0;0 0 1]。
