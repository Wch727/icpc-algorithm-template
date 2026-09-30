#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const double EPS=1e-9;

// 点的编号从 0 开始（几何用 0-indexed 更自然：多边形顶点 0..n-1）

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
    return a+(b-a)*(s1/(s1-s2));
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

// 点与多边形位置关系（任意简单多边形，射线法），0 外部 1 边界 -1 表示 -1 不能用
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
    for(int i=0;i<n;i++)
        if(sgn(cross(p[(i+1)%n]-p[i],q-p[i]))<0)return false;
    return true;
}

// ---------- 自测 ----------

void test_basic()
{
    Point a(0,0),b(3,4);
    printf("[vector] |b|=%f (期望 5.000000) 点积(a,b)=%f 叉积(a,b)=%f\n",len(b),dot(a,b),cross(a,b));
    printf("[vector] (1,0) 与 (0,1) 垂直=%d (期望 1)  与 (2,0) 平行=%d (期望 1)\n",
        (int)is_vertical(Point(1,0),Point(0,1)),(int)is_parallel(Point(1,0),Point(2,0)));
    printf("[vector] (1,1) 在 (0,0)-(2,2) 上=%d (期望 1)  (1,2) 在不在=%d (期望 0)\n",
        (int)on_segment(Point(0,0),Point(2,2),Point(1,1)),
        (int)on_segment(Point(0,0),Point(2,2),Point(1,2)));
}

void test_cross_seg()
{
    int ok=1;
    // 手算 4 组
    struct Case{double x1,y1,x2,y2,x3,y3,x4,y4;int e;};
    Case cs[4]={
        {0,0,2,2, 0,2,2,0, 1},//对角交叉
        {0,0,1,1, 2,2,3,3, 0},//共线但不相接
        {0,0,1,1, 0.5,0.5,2,0, 1},//端点落在另一条线段上
        {0,0,1,0, 2,0,3,0, 0}//同一条直线上分开
    };
    for(int i=0;i<4;i++)
    {
        int got=(int)segment_cross(Point(cs[i].x1,cs[i].y1),Point(cs[i].x2,cs[i].y2),
                                   Point(cs[i].x3,cs[i].y3),Point(cs[i].x4,cs[i].y4));
        printf("[seg] 第 %d 组相交=%d (期望 %d)\n",i+1,got,cs[i].e);
        if(got!=cs[i].e)ok=0;
    }
    printf("[seg] 4 组手算样例 %s\n",ok?"全部通过":"失败");
}

void test_point_in_polygon()
{
    vector<Point> tri;
    tri.push_back(Point(0,0));
    tri.push_back(Point(4,0));
    tri.push_back(Point(0,4));
    printf("[inpoly] (1,1) 三角形 -> %d (期望 2 内部)\n",point_in_polygon(tri,Point(1,1)));
    printf("[inpoly] (3,3) 三角形 -> %d (期望 0 外部)\n",point_in_polygon(tri,Point(3,3)));
    printf("[inpoly] (0,2) 三角形 -> %d (期望 1 边界)\n",point_in_polygon(tri,Point(0,2)));
    printf("[inpoly] (2,0) 三角形 -> %d (期望 1 边界)\n",point_in_polygon(tri,Point(2,0)));
    printf("[inpoly] 凸包判定 (1,1)=%d (期望 1) (3,3)=%d (期望 0)\n",
        (int)in_convex_polygon(tri,Point(1,1)),(int)in_convex_polygon(tri,Point(3,3)));

    // 与暴力「射线法计数 / 全边判定」互相印证：随机点，两个实现必须一致
    vector<Point> sq;
    sq.push_back(Point(0,0));
    sq.push_back(Point(4,0));
    sq.push_back(Point(4,4));
    sq.push_back(Point(0,4));
    mt19937 rnd(7);
    int ok=1;
    for(int t=1;t<=300;t++)
    {
        Point q((rnd()%1000)/1000.0*6-1,(rnd()%1000)/1000.0*6-1);
        int a=point_in_polygon(sq,q);
        int b=in_convex_polygon(sq,q)?2:0;
        if(a==1)continue;//边界上凸包判定不保证，跳过
        if(a!=b)ok=0;
    }
    printf("[inpoly] 300 个随机点，射线法与凸包判定一致 %s\n",ok?"全部通过":"失败");
}

int main()
{
    test_basic();
    test_cross_seg();
    test_point_in_polygon();
    return 0;
}
