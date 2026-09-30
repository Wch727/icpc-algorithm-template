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
    bool operator<(const Point &b)const{return x<b.x-EPS||(x<b.x+EPS&&y<b.y-EPS);}
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
bool cmp_polar(Point a,Point b)
{
    int ha=(a.y>EPS||(fabs(a.y)<=EPS&&a.x<-EPS))?1:0;
    int hb=(b.y>EPS||(fabs(b.y)<=EPS&&b.x<-EPS))?1:0;
    if(ha!=hb)return ha>hb;
    return sgn(cross(a,b))>0;
}

// Andrew 单调链，返回逆时针凸包，O(n log n)
vector<Point> convex_hull(vector<Point> p)
{
    int n=p.size();
    if(n<3)return p;
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
    if(n==1)return 0;
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
    if(n==1)return q==p[0];
    if(n==2)
    {
        if(sgn(cross(p[1]-p[0],q-p[0]))!=0)return 0;
        return sgn(dot(q-p[0],q-p[1]))<=0;
    }
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

// 朴素 O(n) 判定，用于对拍
int in_convex_naive(vector<Point> &p,Point q)
{
    int n=p.size();
    for(int i=0;i<n;i++)
        if(sgn(cross(p[(i+1)%n]-p[i],q-p[i]))<0)return 0;
    return 1;
}

// ================= 自测 =================

void test_area()
{
    vector<Point> sq;
    sq.push_back(Point(0,0));sq.push_back(Point(4,0));
    sq.push_back(Point(4,4));sq.push_back(Point(0,4));
    printf("[area] 4x4 正方形 面积=%.4f (期望 16) 周长=%.4f (期望 16)\n",
        polygon_area(sq),polygon_perimeter(sq));
    vector<Point> tri;
    tri.push_back(Point(0,0));tri.push_back(Point(3,0));tri.push_back(Point(0,4));
    printf("[area] 直角边 3,4 三角形 面积=%.4f (期望 6) 周长=%.4f (期望 12)\n",
        polygon_area(tri),polygon_perimeter(tri));
}

void test_caliper()
{
    // 手算用例
    vector<Point> sq;
    sq.push_back(Point(0,0));sq.push_back(Point(4,0));
    sq.push_back(Point(4,4));sq.push_back(Point(0,4));
    printf("[caliper] 正方形直径=%.4f (期望 5.6569=4sqrt2) 最小宽度=%.4f (期望 4)\n",
        rotating_diameter(sq),min_width(sq));
    vector<Point> line;
    line.push_back(Point(0,0));line.push_back(Point(3,4));
    printf("[caliper] 两点直径=%.4f (期望 5)\n",rotating_diameter(line));

    vector<Point> tri2;
    tri2.push_back(Point(0,0));tri2.push_back(Point(4,0));tri2.push_back(Point(0,3));
    printf("[caliper] 直角三角形最小宽度=%.4f (期望 2.4=斜边上的高)\n",min_width(tri2));

    // 随机点集：旋转卡壳直径 与 O(n^2) 暴力 对拍
    mt19937 rnd(1919810);
    int ok=1,fail=0;
    for(int t=1;t<=300;t++)
    {
        int n=rnd()%12+1;
        vector<Point> q;
        for(int i=0;i<n;i++)q.push_back(Point((rnd()%2001-1000)/10.0,(rnd()%2001-1000)/10.0));
        vector<Point> h=convex_hull(q);
        double got=rotating_diameter(h);
        double bf=0;
        for(int i=0;i<(int)q.size();i++)
            for(int j=i+1;j<(int)q.size();j++)bf=max(bf,len(q[i]-q[j]));
        if(fabs(got-bf)>1e-6)
        {
            ok=0,fail++;
            if(fail<=3)printf("  第 %d 组: 旋转卡壳=%.6f 暴力=%.6f\n",t,got,bf);
        }
        // 顺带对拍凸包面积
        double sa=polygon_area(h),sb=0;
        int m=h.size();
        for(int i=0;i<m;i++)
        {
            int j=(i+1)%m;
            sb+=h[i].x*h[j].y-h[j].x*h[i].y;
        }
        sb/=2;
        if(fabs(sa-sb)>1e-6)ok=0;
    }
    printf("[caliper] 300 组随机点集 旋转卡壳直径 vs O(n^2) 暴力 %s\n",ok?"全部通过":"失败");
}

void test_in_convex()
{
    vector<Point> h;
    h.push_back(Point(0,0));h.push_back(Point(6,0));
    h.push_back(Point(6,6));h.push_back(Point(0,6));
    printf("[inconv] (3,3)=%d (期望 1)  (7,3)=%d (期望 0)  (0,3)=%d (期望 1 边界)\n",
        in_convex(h,Point(3,3)),in_convex(h,Point(7,3)),in_convex(h,Point(0,3)));
    mt19937 rnd(555);
    int ok=1;
    for(int t=1;t<=500;t++)
    {
        Point q((rnd()%1401-200)/100.0,(rnd()%1401-200)/100.0);
        if(in_convex(h,q)!=in_convex_naive(h,q))ok=0;
    }
    // 三角形（非正多边形）也要对
    vector<Point> tri;
    tri.push_back(Point(0,0));tri.push_back(Point(5,1));tri.push_back(Point(2,4));
    for(int t=1;t<=500;t++)
    {
        Point q((rnd()%601-50)/100.0,(rnd()%601-50)/100.0);
        if(in_convex(tri,q)!=in_convex_naive(tri,q))ok=0;
    }
    printf("[inconv] 1000 个随机点 二分判定 vs 朴素 O(n) 判定 %s\n",ok?"全部通过":"失败");
}

int main()
{
    test_area();
    test_caliper();
    test_in_convex();
    return 0;
}
