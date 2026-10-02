// 凸平面区域面积 S、周长 L，加半径 r 圆盘后：S'=S+L*r+pi*r²，L'=L+2*pi*r。
// 单独将该平面区域加半径 R 的三维球，体积 V=2*R*S+(pi/2)*R²*L+(4*pi/3)*R³。
// 两步同时出现时先更新 S、L，再代入球半径；二维圆盘和三维球的半径不能直接相加。
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
__int128 cross(Point a,Point b)
{
    return (__int128)a.x*b.y-(__int128)a.y*b.x;
}
// O(n log n)，去重并删除共线中间点；坐标加减须不溢出 ll，叉积用 __int128。
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
        __int128 v=cross(x,y);
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
