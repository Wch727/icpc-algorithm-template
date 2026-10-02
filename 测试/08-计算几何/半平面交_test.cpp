// 半平面交 的测试与对拍代码
// 模板本体：08-计算几何/半平面交.cpp
#include "../../08-计算几何/半平面交.cpp"

vector<Point> brute(vector<Line> a)
{
    vector<Point> p={{-10,-10},{10,-10},{10,10},{-10,10}};
    for(Line l:a)
    {
        vector<Point> b;
        for(int i=0;i<(int)p.size();i++)
        {
            Point x=p[i],y=p[(i+1)%p.size()];
            bool u=inside(l,x),v=inside(l,y);
            if(u)b.push_back(x);
            if(u!=v)b.push_back(meet(Line(x,y),l));
        }
        p=b;
    }
    return p;
}

vector<Line> box()
{
    return {Line({-10,-10},{10,-10}),Line({10,-10},{10,10}),Line({10,10},{-10,10}),Line({-10,10},{-10,-10})};
}

int main()
{
    {
        auto a=box();a.emplace_back(Point{-10,0},Point{10,0});a.emplace_back(Point{10,0},Point{-10,0});
        assert(half_plane(a).empty());
    }
    srand(20260930);
    bool ok=fabs(area(half_plane(box()))-400)<EPS;
    vector<Line> a=box();
    a.push_back(Line({20,0},{20,-1}));
    ok&=area(half_plane(a))<EPS;
    a=box();
    a.push_back(Line({0,0},{0,1})),a.push_back(Line({0,0},{0,-1}));
    ok&=area(half_plane(a))<EPS;
    for(int t=1;t<=200;t++)
    {
        a=box();
        for(int i=0;i<12;i++)
        {
            Point p={(double)(rand()%31-15),(double)(rand()%31-15)};
            Point v={(double)(rand()%7-3),(double)(rand()%7-3)};
            if(v.x==0&&v.y==0)v.x=1;
            // 一半轮次保证原点可行，避免随机测试大多只检查空集
            if(t%2==0&&cross(v,Point{0,0}-p)<0)v=v*(-1);
            a.push_back(Line(p,p+v));
            if(i%4==0)a.push_back(a.back());
        }
        if(fabs(area(half_plane(a))-area(brute(a)))>1e-6)ok=false;
    }
    printf("半平面交: %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
