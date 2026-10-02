// 整体直径卡壳不直接给出每个顶点各自最远点；需要另证候选单调性或使用对应查询算法。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const double EPS=1e-9;

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

// 极角排序，半平面 + 叉积，O(1) 比较
// 排序比较不使用 EPS，避免破坏严格弱序；零向量无极角，实际应用应先去掉。
bool cmp_polar(Point a,Point b)
{
    int ha=a.y<0||(a.y==0&&a.x<0),hb=b.y<0||(b.y==0&&b.x<0);
    if(ha!=hb)return ha<hb;
    double v=cross(a,b);
    return v!=0?v>0:len2(a)<len2(b);
}

// Andrew 单调链，返回逆时针凸包，O(n log n)
vector<Point> convex_hull(vector<Point> p)
{
    int n=p.size();
    sort(p.begin(),p.end());
    n=unique(p.begin(),p.end())-p.begin();
    p.resize(n);
    if(n<3)return p;
    vector<Point> h(2*n);
    int k=0;
    for(int i=0;i<n;i++)
    {
        while(k>=2&&sgn(cross(h[k-1]-h[k-2],p[i]-h[k-2]))<=0)k--;
        h[k++]=p[i];
    }
    for(int i=n-2,t=k+1;i>=0;i--)
    {
        while(k>=t&&sgn(cross(h[k-1]-h[k-2],p[i]-h[k-2]))<=0)k--;
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

// 凸多边形周长
double polygon_perimeter(vector<Point> &p)
{
    double s=0;
    int n=p.size();
    for(int i=0;i<n;i++)s+=len(p[(i+1)%n]-p[i]);
    return s;
}

// ================= 旋转卡壳：凸包直径 =================
// 单调旋转，O(n)。i 是起点，j 是当前离直线 p[i]-p[i+1] 最远的点，
// 「面积」用叉积的绝对值表示，不必真的除以 2。
double rotating_diameter(vector<Point> &p)
{
    int n=p.size();
    if(n<=1)return 0;
    if(n==2)return len(p[1]-p[0]);
    double ans=0;
    for(int i=0,j=1;i<n;i++)
    {
        Point a=p[i],b=p[(i+1)%n];
        while(fabs(cross(b-a,p[(j+1)%n]-a))>fabs(cross(b-a,p[j]-a)))j=(j+1)%n;
        ans=max(ans,len2(p[j]-p[i]));
        ans=max(ans,len2(p[j]-p[(i+1)%n]));
    }
    return sqrt(ans);
}

// 凸包宽度：所有方向上的最小「平行支撑线间距」，也是旋转卡壳，O(n)
double min_width(vector<Point> &p)
{
    int n=p.size();
    if(n<=2)return 0;
    double ans=1e100;
    for(int i=0,j=1;i<n;i++)
    {
        Point a=p[i],b=p[(i+1)%n];
        while(fabs(cross(b-a,p[(j+1)%n]-a))>fabs(cross(b-a,p[j]-a)))j=(j+1)%n;
        ans=min(ans,fabs(cross(b-a,p[j]-a))/len(b-a));
    }
    return ans;
}

// ================= 点在凸包内判定 =================
// 顶点必须逆时针且无重复；O(log n)：先二分出极角所在的三角形，再用叉积判
// 返回 1 内部/边界，0 外部
int in_convex(vector<Point> &p,Point q)
{
    int n=p.size();
    if(!n)return 0;
    if(n==1)return q==p[0];
    if(n==2)
    {
        if(sgn(cross(p[1]-p[0],q-p[0]))!=0)return 0;
        return sgn(dot(q-p[0],q-p[1]))<=0;
    }
    if(sgn(cross(p[1]-p[0],q-p[0]))==0)return sgn(dot(q-p[0],q-p[1]))<=0;
    if(sgn(cross(p[n-1]-p[0],q-p[0]))==0)return sgn(dot(q-p[0],q-p[n-1]))<=0;
    if(sgn(cross(p[1]-p[0],q-p[0]))<0)return 0;//在 p0p1 外侧
    if(sgn(cross(p[n-1]-p[0],q-p[0]))>0)return 0;//在 p0p(n-1) 另一侧
    int lo=1,hi=n-1;
    while(hi-lo>1)//找夹住 q 的两个相邻顶点
    {
        int mid=(lo+hi)>>1;
        if(sgn(cross(p[mid]-p[0],q-p[0]))>=0)lo=mid;
        else hi=mid;
    }
    return sgn(cross(p[(lo+1)%n]-p[lo],q-p[lo]))>=0;
}
