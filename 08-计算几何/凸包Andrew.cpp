#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const double EPS=1e-9;
const double PI=acos(-1.0);

struct Point
{
    double x,y;
    Point(){}
    Point(double x,double y):x(x),y(y){}
    Point operator+(const Point &b)const{return Point(x+b.x,y+b.y);}
    Point operator-(const Point &b)const{return Point(x-b.x,y-b.y);}
    Point operator*(double k)const{return Point(x*k,y*k);}
    bool operator<(const Point &b)const{return x!=b.x?x<b.x:y<b.y;}
    bool operator==(const Point &b)const{return fabs(x-b.x)<EPS&&fabs(y-b.y)<EPS;}
};

int sgn(double x)
{
    if(x>EPS)return 1;
    if(x<-EPS)return -1;
    return 0;
}

double cross(Point a,Point b){return a.x*b.y-a.y*b.x;}
double dot(Point a,Point b){return a.x*b.x+a.y*b.y;}
double len(Point a){return sqrt(a.x*a.x+a.y*a.y);}
double len2(Point a){return a.x*a.x+a.y*a.y;}

// ---------- 极角排序 ----------
// atan2 精度差、常数大；用半平面 + 叉积排序，O(log) 常数极小
// 顺序：先按 y>=0 / y<0 分上下半平面，同半平面内按极角逆时针

// 排序比较不使用 EPS，避免破坏严格弱序；零向量无极角，实际应用应先去掉。
bool cmp_polar(Point a,Point b)
{
    int ha=a.y<0||(a.y==0&&a.x<0),hb=b.y<0||(b.y==0&&b.x<0);
    if(ha!=hb)return ha<hb;
    double v=cross(a,b);
    return v!=0?v>0:len2(a)<len2(b);
}

// 也可以用 atan2，写法最短但有精度损失
bool cmp_atan2(Point a,Point b){return atan2(a.y,a.x)<atan2(b.y,b.x);}

// ---------- Andrew 单调链求凸包 ----------
// 返回逆时针凸包顶点，无重复点；点集共线时返回两端点
// O(n log n)，瓶颈是排序
vector<Point> convex_hull(vector<Point> p)
{
    int n=p.size();
    sort(p.begin(),p.end());
    n=unique(p.begin(),p.end())-p.begin();//先去重，否则共线点会出错
    p.resize(n);
    if(n<3)return p;
    vector<Point> h(2*n);
    int k=0;
    for(int i=0;i<n;i++)//下凸壳，叉积 <=0 弹出（共线点只留两端）
    {
        while(k>=2&&sgn(cross(h[k-1]-h[k-2],p[i]-h[k-2]))<=0)k--;
        h[k++]=p[i];
    }
    for(int i=n-2,t=k+1;i>=0;i--)//上凸壳
    {
        while(k>=t&&sgn(cross(h[k-1]-h[k-2],p[i]-h[k-2]))<=0)k--;
        h[k++]=p[i];
    }
    h.resize(k-1);//最后一点是起点，去掉
    return h;
}

// 保留共线点的版本：把 <=0 改成 <0，凸包边上会留下中间点
vector<Point> convex_hull_keep_col(vector<Point> p)
{
    int n=p.size();
    sort(p.begin(),p.end());
    n=unique(p.begin(),p.end())-p.begin();
    p.resize(n);
    if(n<3)return p;
    bool all_line=true;
    for(Point x:p)if(sgn(cross(p.back()-p.front(),x-p.front()))!=0)all_line=false;
    if(all_line)return p;
    vector<Point> h(2*n);
    int k=0;
    for(int i=0;i<n;i++)
    {
        while(k>=2&&sgn(cross(h[k-1]-h[k-2],p[i]-h[k-2]))<0)k--;
        h[k++]=p[i];
    }
    for(int i=n-2,t=k+1;i>=0;i--)
    {
        while(k>=t&&sgn(cross(h[k-1]-h[k-2],p[i]-h[k-2]))<0)k--;
        h[k++]=p[i];
    }
    h.resize(k-1);
    return h;
}

double polygon_area(vector<Point> &p)//有向面积，逆时针为正
{
    double s=0;
    int n=p.size();
    for(int i=0;i<n;i++)s+=cross(p[i],p[(i+1)%n]);
    return s/2;
}

bool on_seg(Point a,Point b,Point p)
{
    if(sgn(cross(b-a,p-a))!=0)return false;
    return sgn(dot(p-a,p-b))<=0;
}
