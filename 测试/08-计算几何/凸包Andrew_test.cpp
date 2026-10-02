// 凸包Andrew 的测试与对拍代码
// 模板本体：08-计算几何/凸包Andrew.cpp
#include "../../08-计算几何/凸包Andrew.cpp"

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
        for(int i=0;i<n;i++)q.push_back(Point((int)(rnd()%21)-10,(int)(rnd()%21)-10));
        vector<Point> a=convex_hull(q);
        vector<Point> b=naive_hull(q);
        double sa=fabs(polygon_area(a)),sb=fabs(polygon_area(b));
        if(fabs(sa-sb)>1e-6)
        {
            ok=0,fail++;
            if(fail<=3)printf("  第 %d 组: Andrew=%.6f 朴素=%.6f (顶点 %d vs %d)\n",t,sa,sb,(int)a.size(),(int)b.size());
        }
    }
    printf("[hull] 300 组随机点集与朴素 O(n^3) 凸包面积对拍 %s\n",(assert(ok),ok?"全部通过":"失败"));
}

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        vector<Point> a={{1,1},{1,1}};assert(convex_hull(a).size()==1);
        vector<Point> b={{0,0},{1,0},{2,0},{3,0}};
        assert(convex_hull_keep_col(b).size()==4);
    }

    test_polar();
    test_hull();
    return 0;
}
