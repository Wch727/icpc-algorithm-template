// 抛物面提升 Delaunay：三维下凸包投影，O(n²)，0-indexed 原点编号，重合点保留第一份。
// 共圆时任选合法对角线；全共线返回空。用 long double 与坐标归一化改善数值，不用于病态极近点。
// Voronoi 用半平面裁剪：返回给定凸 CCW 边界内每个点的区域，重合后出现的点区域为空。
// 裁剪版总 O(n³) 最坏，适合中小规模；无界 Voronoi 必须另表示射线，不能直接把截断边界当真实边。
#include<bits/stdc++.h>
#include "三维凸包.cpp" // 复用三维增量凸包；打印时查同章，避免重复实现。
using namespace std;
struct Point{long double x,y;Point operator+(Point b)const{return {x+b.x,y+b.y};}Point operator-(Point b)const{return {x-b.x,y-b.y};}Point operator*(long double t)const{return {x*t,y*t};}};
long double cross(Point a,Point b){return a.x*b.y-a.y*b.x;}
long double dot(Point a,Point b){return a.x*b.x+a.y*b.y;}
long double incircle(Point a,Point b,Point c,Point p)
{a=a-p;b=b-p;c=c-p;return dot(a,a)*cross(b,c)-dot(b,b)*cross(a,c)+dot(c,c)*cross(a,b);}
vector<array<int,3>> delaunay(const vector<Point> &input)
{
    vector<Point> p;vector<int> id;set<pair<long double,long double>> seen;
    for(int i=0;i<(int)input.size();i++)if(seen.insert({input[i].x,input[i].y}).second)p.push_back(input[i]),id.push_back(i);
    int n=p.size();if(n<3)return {};long double xmin=p[0].x,xmax=xmin,ymin=p[0].y,ymax=ymin;
    for(auto q:p)xmin=min(xmin,q.x),xmax=max(xmax,q.x),ymin=min(ymin,q.y),ymax=max(ymax,q.y);
    Point center={(xmin+xmax)/2,(ymin+ymax)/2};long double scale=max(xmax-xmin,ymax-ymin);if(scale==0)return {};
    vector<Point3> lifted;for(auto &q:p){q=(q-center)*(1/scale);lifted.push_back({q.x,q.y,dot(q,q)});}
    auto hull=convex_hull_3d(lifted,1e-18L);vector<array<int,3>> ans;
    for(auto [a,b,c]:hull.faces)
    {
        long double turn=cross(p[b]-p[a],p[c]-p[a]);
        if(fabsl(turn)<=1e-18L)continue;
        if(hull.dim==3&&turn>=0)continue;
        if(turn<0)swap(b,c);ans.push_back({id[a],id[b],id[c]});
    }
    return ans;
}
vector<vector<Point>> voronoi_cells(const vector<Point> &p,const vector<Point> &boundary)
{
    const long double EPS=1e-12L;int n=p.size();vector<vector<Point>> cell(n);
    for(int i=0;i<n;i++)
    {
        vector<Point>a=boundary;
        for(int j=0;j<n&&!a.empty();j++)if(i!=j)
        {
            Point normal=p[j]-p[i];long double norm=dot(normal,normal);if(norm==0){if(j<i)a.clear();continue;}
            auto side=[&](Point x){return dot(normal,x-p[i])-norm/2;};vector<Point>b;
            for(int k=0;k<(int)a.size();k++){Point u=a[k],v=a[(k+1)%a.size()];long double x=side(u),y=side(v);bool in=x<=EPS,next=y<=EPS;if(in)b.push_back(u);if(in!=next)b.push_back(u+(v-u)*(x/(x-y)));}a.swap(b);
        }
        cell[i]=a;
    }
    return cell;
}
