#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
struct Point
{
    ll x,y;
    Point operator+(Point b)const{return {x+b.x,y+b.y};}
    Point operator-(Point b)const{return {x-b.x,y-b.y};}
    bool operator<(Point b)const{return x!=b.x?x<b.x:y<b.y;}
    bool operator==(Point b)const{return x==b.x&&y==b.y;}
};
ll cross(Point a,Point b)
{
    return a.x*b.y-a.y*b.x;
}
// O(n log n)，去重并删除共线中间点；坐标运算须不溢出 ll。
vector<Point> hull(vector<Point> a)
{
    sort(a.begin(),a.end());
    a.erase(unique(a.begin(),a.end()),a.end());
    if(a.size()<3)return a;
    vector<Point> p;
    for(Point x:a)
    {
        while(p.size()>1&&cross(p.back()-p[p.size()-2],x-p.back())<=0)p.pop_back();
        p.push_back(x);
    }
    int k=p.size();
    for(int i=(int)a.size()-2;i>=0;i--)
    {
        while((int)p.size()>k&&cross(p.back()-p[p.size()-2],a[i]-p.back())<=0)p.pop_back();
        p.push_back(a[i]);
    }
    p.pop_back();
    return p;
}
void rotate_low(vector<Point> &a)
{
    int k=0;
    for(int i=1;i<(int)a.size();i++)if(a[i].y<a[k].y||(a[i].y==a[k].y&&a[i].x<a[k].x))k=i;
    rotate(a.begin(),a.begin()+k,a.end());
}
// O(n+m)，输入逆时针严格凸包（无重复首点）；单点/线段也支持。
// 任意点集先调用 hull；空集的和仍为空集。
vector<Point> minkowski(vector<Point> a,vector<Point> b)
{
    if(a.empty()||b.empty())return {};
    rotate_low(a),rotate_low(b);
    int n=a.size(),m=b.size(),i=0,j=0;
    Point p=a[0]+b[0];
    vector<Point> c={p};
    while(i<n||j<m)
    {
        Point x=i<n?a[(i+1)%n]-a[i]:Point{0,0};
        Point y=j<m?b[(j+1)%m]-b[j]:Point{0,0};
        ll v=cross(x,y);
        if(j==m||(i<n&&v>0))p=p+x,i++;
        else if(i==n||v<0)p=p+y,j++;
        else p=p+x+y,i++,j++;
        c.push_back(p);
    }
    c.pop_back();
    // 线性删除退化零边与共线点，正常输出已按边极角有序。
    vector<Point> d;
    for(Point x:c)
    {
        if(!d.empty()&&x==d.back())continue;
        while(d.size()>1&&cross(d.back()-d[d.size()-2],x-d.back())==0)d.pop_back();
        d.push_back(x);
    }
    while(d.size()>2&&cross(d.back()-d[d.size()-2],d.front()-d.back())==0)d.pop_back();
    if(d.size()>2&&cross(d.front()-d.back(),d[1]-d.front())==0)d.erase(d.begin());
    return d;
}
// 独立暴力：枚举所有点对和再求凸包，O(nm log(nm))。
vector<Point> brute(vector<Point> a,vector<Point> b)
{
    vector<Point> c;
    for(Point x:a)for(Point y:b)c.push_back(x+y);
    return hull(c);
}
bool same(vector<Point> a,vector<Point> b)
{
    sort(a.begin(),a.end()),sort(b.begin(),b.end());
    return a==b;
}
