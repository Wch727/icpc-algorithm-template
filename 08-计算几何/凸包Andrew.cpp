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

// ---------- 极角排序 ----------
// atan2 精度差、常数大；用半平面 + 叉积排序，O(log) 常数极小
// 顺序：先按 y>=0 / y<0 分上下半平面，同半平面内按极角逆时针

bool cmp_polar(Point a,Point b)
{
    int ha=(a.y>EPS||(fabs(a.y)<=EPS&&a.x<-EPS))?1:0;//上半平面（含负 x 轴起点）
    int hb=(b.y>EPS||(fabs(b.y)<=EPS&&b.x<-EPS))?1:0;
    if(ha!=hb)return ha>hb;
    return sgn(cross(a,b))>0;
}

// 也可以用 atan2，写法最短但有精度损失
bool cmp_atan2(Point a,Point b){return atan2(a.y,a.x)<atan2(b.y,b.x);}

// ---------- Andrew 单调链求凸包 ----------
// 返回逆时针凸包顶点，无重复点；点集共线时返回两端点
// O(n log n)，瓶颈是排序
vector<Point> convex_hull(vector<Point> p)
{
    int n=p.size();
    if(n<3)return p;
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
    if(n<3)return p;
    sort(p.begin(),p.end());
    n=unique(p.begin(),p.end())-p.begin();
    p.resize(n);
    if(n<3)return p;
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

// ---------- 自测 ----------

double shoelace(vector<Point> p)//鞋带公式，与上面独立实现，用于对拍
{
    double s=0;
    int n=p.size();
    for(int i=0;i<n;i++)
    {
        int j=(i+1)%n;
        s+=p[i].x*p[j].y-p[j].x*p[i].y;
    }
    return s/2;
}

bool on_seg(Point a,Point b,Point p)
{
    if(sgn(cross(b-a,p-a))!=0)return false;
    return sgn(dot(p-a,p-b))<=0;
}

// 朴素 O(n^3) 凸包：枚举点对，若其余所有点都在同一侧，则该点对是凸包边
// 共线点也会被当成边收集进来（凸包边界上的点会全部留下），所以与保留共线点版本可比
vector<Point> naive_hull(vector<Point> p)
{
    sort(p.begin(),p.end());
    p.erase(unique(p.begin(),p.end()),p.end());
    int n=p.size();
    if(n<3)return p;
    vector<Point> h;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            if(i==j)continue;
            int pos=0,neg=0;
            for(int k=0;k<n;k++)
            {
                if(k==i||k==j)continue;
                int s=sgn(cross(p[j]-p[i],p[k]-p[i]));
                if(s>0)pos++;
                if(s<0)neg++;
            }
            if(pos&&neg)continue;//两侧都有点，不是凸包边
            h.push_back(p[i]);//否则 p[i] 是凸包顶点（边 i->j 朝向凸包外侧）
        }
    sort(h.begin(),h.end());
    h.erase(unique(h.begin(),h.end()),h.end());
    if((int)h.size()<3)return h;
    // 按绕质心的极角排成逆时针
    Point o(0,0);
    for(int i=0;i<(int)h.size();i++)o=o+h[i];
    o=o*(1.0/h.size());
    for(int i=0;i<(int)h.size();i++)h[i]=h[i]-o;
    sort(h.begin(),h.end(),cmp_polar);
    for(int i=0;i<(int)h.size();i++)h[i]=h[i]+o;
    return h;
}

void test_polar()
{
    vector<Point> v;
    v.push_back(Point(1,0));
    v.push_back(Point(0,1));
    v.push_back(Point(-1,0));
    v.push_back(Point(0,-1));
    v.push_back(Point(1,1));
    sort(v.begin(),v.end(),cmp_polar);
    printf("[polar] 极角排序(逆时针从 +x 起): ");
    for(int i=0;i<(int)v.size();i++)printf("(%.0f,%.0f) ",v[i].x,v[i].y);
    printf("\n");
}

void test_hull()
{
    // 正方形 + 内部点
    vector<Point> p;
    p.push_back(Point(0,0));p.push_back(Point(4,0));
    p.push_back(Point(4,4));p.push_back(Point(0,4));
    p.push_back(Point(1,1));p.push_back(Point(2,2));
    vector<Point> h=convex_hull(p);
    printf("[hull] 正方形+内部点 凸包顶点数=%d (期望 4) 面积=%.4f (期望 16)\n",(int)h.size(),fabs(polygon_area(h)));
    printf("[hull] 鞋带公式核对面积=%.4f\n",fabs(shoelace(h)));

    // 共线点集：凸包退化成线段，面积 0
    vector<Point> c;
    c.push_back(Point(0,0));c.push_back(Point(1,1));
    c.push_back(Point(2,2));c.push_back(Point(3,3));
    vector<Point> hc=convex_hull(c);
    printf("[hull] 共线点集 顶点数=%d 面积=%.4f (期望 2 与 0)\n",(int)hc.size(),fabs(polygon_area(hc)));
    vector<Point> hc2=convex_hull_keep_col(c);
    printf("[hull] 保留共线点版本 顶点数=%d (期望 4)\n",(int)hc2.size());

    // 随机点集与朴素 O(n^3) 凸包对拍：面积必须一致
    mt19937 rnd(2024);
    int ok=1,fail=0;
    for(int t=1;t<=300;t++)
    {
        int n=rnd()%8+3;
        vector<Point> q;
        for(int i=0;i<n;i++)q.push_back(Point(rnd()%21-10,rnd()%21-10));
        vector<Point> a=convex_hull(q);
        vector<Point> b=naive_hull(q);
        double sa=fabs(polygon_area(a)),sb=fabs(polygon_area(b));
        if(fabs(sa-sb)>1e-6)
        {
            ok=0,fail++;
            if(fail<=3)printf("  第 %d 组: Andrew=%.6f 朴素=%.6f (顶点 %d vs %d)\n",t,sa,sb,(int)a.size(),(int)b.size());
        }
    }
    printf("[hull] 300 组随机点集与朴素 O(n^3) 凸包面积对拍 %s\n",ok?"全部通过":"失败");
}

int main()
{
    test_polar();
    test_hull();
    return 0;
}
