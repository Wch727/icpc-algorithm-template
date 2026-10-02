// 向量基础 的测试与对拍代码
// 模板本体：08-计算几何/向量基础.cpp
#include "../../08-计算几何/向量基础.cpp"

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
    printf("[seg] 4 组手算样例 %s\n",(assert(ok),ok?"全部通过":"失败"));
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
    printf("[inpoly] 300 个随机点，射线法与凸包判定一致 %s\n",(assert(ok),ok?"全部通过":"失败"));
}

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        bool ok; Point p=line_intersect({0,0},{2,0},{1,-1},{1,2},ok);
        assert(ok&&fabs(p.x-1)<EPS&&fabs(p.y)<EPS);
    }

    test_basic();
    test_cross_seg();
    test_point_in_polygon();
    return 0;
}
