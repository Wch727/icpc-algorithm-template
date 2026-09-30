// 旋转卡壳与多边形面积 的测试与对拍代码
// 模板本体：08-计算几何/旋转卡壳与多边形面积.cpp
#include "../../08-计算几何/旋转卡壳与多边形面积.cpp"

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
