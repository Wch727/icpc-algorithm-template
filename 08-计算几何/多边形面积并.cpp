// 多个简单多边形的面积并，允许非凸，要求每个多边形顶点逆时针、无自交且无零长度边。
// 每条边按遮蔽区间的进入/离开事件扫描；只累加露在外面的边段，做 Green 积分。
// 重合且同向边只保留一份；反向边积分相消。O(N² log N)，浮点 EPS 按尺度调整。
// 用有向边边界积分求面积并时，共线重叠边只能计一次；固定多边形编号作为去重优先级。
#include<bits/stdc++.h>
using namespace std;
struct Point{double x,y;Point operator+(Point b)const{return {x+b.x,y+b.y};}Point operator-(Point b)const{return {x-b.x,y-b.y};}Point operator*(double t)const{return {x*t,y*t};}};
double cross(Point a,Point b){return a.x*b.y-a.y*b.x;}
double dot(Point a,Point b){return a.x*b.x+a.y*b.y;}
const double EPS=1e-9;
int inside(Point p,const vector<Point> &a)// 1 内部，0 边界，-1 外部
{
    bool in=false;for(int i=0;i<(int)a.size();i++){Point u=a[i],v=a[(i+1)%a.size()];if(fabs(cross(v-u,p-u))<=EPS&&dot(p-u,p-v)<=EPS)return 0;if((u.y>p.y)!=(v.y>p.y)){double x=u.x+(v.x-u.x)*(p.y-u.y)/(v.y-u.y);if(x>p.x)in=!in;}}
    return in?1:-1;
}
// 沿每条边收集其他多边形遮蔽区间的进入/离开事件，O(N² log N)。
// 算法参考 KACTL PolygonUnion.h（black_horse2014/chilli）；EPS 按坐标尺度调整。
double polygon_union_area(const vector<vector<Point>> &poly)
{
    auto sign=[](double x){return (x>EPS)-(x<-EPS);};double ans=0;
    for(int i=0;i<(int)poly.size();i++)for(int k=0;k<(int)poly[i].size();k++)
    {
        Point a=poly[i][k],b=poly[i][(k+1)%poly[i].size()],v=b-a;double len=dot(v,v);if(len==0)continue;
        vector<pair<double,int>> events={{0,0},{1,0}};
        for(int j=0;j<(int)poly.size();j++)if(j!=i)for(int h=0;h<(int)poly[j].size();h++)
        {
            Point c=poly[j][h],d=poly[j][(h+1)%poly[j].size()];int sc=sign(cross(v,c-a)),sd=sign(cross(v,d-a));
            if(sc!=sd)
            {
                double sa=cross(d-c,a-c),sb=cross(d-c,b-c);
                if(min(sc,sd)<0&&fabs(sa-sb)>EPS)events.push_back({sa/(sa-sb),sign(sc-sd)});
            }
            else if(sc==0&&j<i&&dot(v,d-c)>0)
            {events.push_back({dot(c-a,v)/len,1});events.push_back({dot(d-a,v)/len,-1});}
        }
        for(auto &e:events)e.first=clamp(e.first,0.0,1.0);sort(events.begin(),events.end());
        int count=0;double last=0,visible=0;
        for(auto [at,delta]:events){if(count==0)visible+=at-last;count+=delta;last=at;}
        ans+=cross(a,b)*visible/2;
    }
    return ans;
}
