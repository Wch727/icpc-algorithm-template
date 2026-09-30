// 最小圆覆盖 的测试与对拍代码
// 模板本体：08-计算几何/最小圆覆盖.cpp
#include "../../08-计算几何/最小圆覆盖.cpp"

// O(n^4)，枚举 1/2/3 个边界点的候选圆并检查所有点。

double brute(vector<Point> a)
{
    if(a.empty())return 0;
    double ans=1e100;
    vector<Circle> c;
    for(int i=0;i<(int)a.size();i++)
    {
        c.push_back({a[i],0});
        for(int j=0;j<i;j++)
        {
            c.push_back(two(a[i],a[j]));
            for(int k=0;k<j;k++)
            {
                double x=a[j].x-a[i].x,y=a[j].y-a[i].y;
                double u=a[k].x-a[i].x,v=a[k].y-a[i].y;
                if(fabs(x*v-y*u)<1e-12)continue;
                // 解两条垂直平分线，独立计算圆心
                double s=(a[j].x*a[j].x+a[j].y*a[j].y-a[i].x*a[i].x-a[i].y*a[i].y)/2;
                double t=(a[k].x*a[k].x+a[k].y*a[k].y-a[i].x*a[i].x-a[i].y*a[i].y)/2;
                Point p={(s*v-y*t)/(x*v-y*u),(x*t-s*u)/(x*v-y*u)};
                c.push_back({p,dis(p,a[i])});
            }
        }
    }
    for(Circle z:c)
    {
        bool ok=true;
        for(Point p:a)if(!inside(z,p))ok=false;
        if(ok)ans=min(ans,z.r);
    }
    return ans;
}
int main()
{
    srand(20260930);
    bool ok=fabs(min_circle({{0,0},{2,0},{1,0}}).r-1)<EPS;
    ok&=min_circle({}).r==0;
    for(int t=1;t<=200;t++)
    {
        vector<Point> a;
        for(int i=0,n=rand()%9+1;i<n;i++)a.push_back({(double)(rand()%11-5),(double)(t%5?rand()%11-5:0)});
        Circle c=min_circle(a);
        if(fabs(c.r-brute(a))>1e-6)ok=false;
        for(Point p:a)if(!inside(c,p))ok=false;
    }
    printf("最小圆覆盖: %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
