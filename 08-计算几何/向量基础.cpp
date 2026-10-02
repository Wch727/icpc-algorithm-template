// 二维相似变换用复数：保持定向 z->a*z+b，反转定向 z->a*conj(z)+b。
// p0!=p1 映到 q0,q1：a=(q1-q0)/(p1-p0)，b=q0-a*p0；退化点对不能除。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const double EPS=1e-9;

// 点的编号从 0 开始（几何用 0-indexed 更自然：多边形顶点 0..n-1）

struct Point
{
    double x,y;
    Point():x(0),y(0){}
    Point(double x,double y):x(x),y(y){}
    Point operator+(const Point &b)const{return Point(x+b.x,y+b.y);}
    Point operator-(const Point &b)const{return Point(x-b.x,y-b.y);}
    Point operator*(double k)const{return Point(x*k,y*k);}
    bool operator<(const Point &b)const{return x!=b.x?x<b.x:y<b.y;}
    bool operator==(const Point &b)const{return fabs(x-b.x)<EPS&&fabs(y-b.y)<EPS;}
};

int sgn(double x)//三态符号函数，所有判断都经过它，避免浮点直接比大小
{
    if(x>EPS)return 1;
    if(x<-EPS)return -1;
    return 0;
}

double dot(Point a,Point b){return a.x*b.x+a.y*b.y;}//点积，>0 锐角
double cross(Point a,Point b){return a.x*b.y-a.y*b.x;}//叉积，>0 表示 b 在 a 逆时针侧
double len(Point a){return sqrt(a.x*a.x+a.y*a.y);}
double len2(Point a){return a.x*a.x+a.y*a.y;}

// ---------- 关系判定 ----------

bool is_parallel(Point a,Point b){return sgn(cross(a,b))==0;}//平行（含反向）
bool is_vertical(Point a,Point b){return sgn(dot(a,b))==0;}//垂直

// 点 p 是否在线段 ab 上（含端点），O(1)
bool on_segment(Point a,Point b,Point p)
{
    if(sgn(cross(b-a,p-a))!=0)return false;//不在直线上
    return sgn(dot(p-a,p-b))<=0;//投影落在线段内
}

// 直线 ab 与直线 cd 的交点；平行/重合时 ok=false
Point line_intersect(Point a,Point b,Point c,Point d,bool &ok)
{
    double s1=cross(b-a,c-a),s2=cross(b-a,d-a);
    if(sgn(s1-s2)==0&&sgn(s1)==0){ok=false;return Point();}//共线，无穷多交点
    if(sgn(s1-s2)==0){ok=false;return Point();}//平行
    ok=true;
    return c+(d-c)*(s1/(s1-s2));
}

// 线段 ab 与线段 cd 是否相交（含端点、含共线重叠），O(1)
bool segment_cross(Point a,Point b,Point c,Point d)
{
    int d1=sgn(cross(b-a,c-a)),d2=sgn(cross(b-a,d-a));
    int d3=sgn(cross(d-c,a-c)),d4=sgn(cross(d-c,b-c));
    if(d1*d2<0&&d3*d4<0)return true;//规范相交
    if(d1==0&&on_segment(a,b,c))return true;//端点落在另一条线段上
    if(d2==0&&on_segment(a,b,d))return true;
    if(d3==0&&on_segment(c,d,a))return true;
    if(d4==0&&on_segment(c,d,b))return true;
    return false;
}

// 点到线段的距离，O(1)
double point_seg_dis(Point a,Point b,Point p)
{
    if(sgn(dot(p-a,b-a))<=0)return len(p-a);//垂足在 a 外侧
    if(sgn(dot(p-b,a-b))<=0)return len(p-b);
    return fabs(cross(b-a,p-a))/len(b-a);
}

// 点与简单多边形位置关系，射线法。
// 返回：0 外部，1 边界上，2 内部
int point_in_polygon(vector<Point> &p,Point q)
{
    int n=p.size();
    for(int i=0;i<n;i++)
        if(on_segment(p[i],p[(i+1)%n],q))return 1;
    int cnt=0;
    for(int i=0;i<n;i++)
    {
        Point a=p[i],b=p[(i+1)%n];
        if((a.y>q.y)!=(b.y>q.y))//跨过 q 的水平线
        {
            double x=a.x+(q.y-a.y)*(b.x-a.x)/(b.y-a.y);
            if(x>q.x)cnt++;
        }
    }
    return cnt&1?2:0;
}

// 仅适用于凸多边形的内测：叉积同号，O(n)
bool in_convex_polygon(vector<Point> &p,Point q)//顶点逆时针
{
    int n=p.size();
    if(!n)return false;
    if(n==1)return q==p[0];
    if(n==2)return on_segment(p[0],p[1],q);
    for(int i=0;i<n;i++)
        if(sgn(cross(p[(i+1)%n]-p[i],q-p[i]))<0)return false;
    return true;
}
