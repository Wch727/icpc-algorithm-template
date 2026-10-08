#include<bits/stdc++.h>
using namespace std;
struct Point
{
    double x,y;
};
vector<Point> a,tmp;
double dis2(Point a,Point b){return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);}
bool cmp_y(Point a,Point b){return a.y<b.y;}
// 递归入口按 x 排序，出口按 y 排序；归并与带内扫描 O(n)。
double solve(int l,int r)
{
    if(r-l<=3)
    {
        double ans=numeric_limits<double>::infinity();
        for(int i= l; i < r; i++)
            for(int j= l; j < i; j++)
                ans= min(ans, dis2(a[i], a[j]));
        sort(a.begin()+l,a.begin()+r,cmp_y);
        return ans;
    }
    int mid=(l+r)>>1;
    double x=a[mid].x;
    double ans=min(solve(l,mid),solve(mid,r));
    merge(a.begin()+l,a.begin()+mid,a.begin()+mid,a.begin()+r,tmp.begin()+l,cmp_y);
    copy(tmp.begin()+l,tmp.begin()+r,a.begin()+l);
    vector<Point> p;
    for(int i=l;i<r;i++)
        if((a[i].x-x)*(a[i].x-x)<ans)
        {
            for(int j=(int)p.size()-1;j>=0&&(a[i].y-p[j].y)*(a[i].y-p[j].y)<ans;j--)ans=min(ans,dis2(a[i],p[j]));
            p.push_back(a[i]);
        }
    return ans;
}
// O(n log n)，最近点对距离；少于两点返回正无穷，重合点返回 0。
double closest_pair()
{
    sort(a.begin(), a.end(),
         [](Point x, Point y) { return x.x != y.x ? x.x < y.x : x.y < y.y; });
    tmp.resize(a.size());
    return sqrt(solve(0,a.size()));
}
