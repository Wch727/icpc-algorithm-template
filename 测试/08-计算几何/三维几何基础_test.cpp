// 三维几何基础 的测试与对拍代码
// 模板本体：08-计算几何/三维几何基础.cpp
#include "../../08-计算几何/三维几何基础.cpp"

int main()
{
    srand(20260930);
    bool ok=true;
    Plane s;
    Point q;
    double d;
    ok&=make_plane({0,0,0},{1,0,0},{0,1,0},s);
    ok&=project({1,2,3},s,q,d)&&fabs(d-3)<EPS;
    ok&=line_plane({0,0,1},{0,0,-1},s,q)==1&&len(q)<EPS;
    ok&=line_plane({0,0,1},{1,0,0},s,q)==0;
    ok&=line_plane({0,0,0},{1,0,0},s,q)==2;
    ok&=!make_plane({0,0,0},{1,0,0},{2,0,0},s);
    ok&=!project({1,2,3},s,q,d);
    for(int t=1;t<=200;t++)
    {
        double a=rand()%9+1,b=rand()%9+1,c=rand()%9+1,h=rand()%11-5;
        // 独立标量平面方程 ax+by+cz=h，枚举网格点并代公式对拍
        s={{h/a,0,0},{a,b,c}};
        for(int x=-2;x<=2;x++)for(int y=-2;y<=2;y++)for(int z=-2;z<=2;z++)
        {
            Point p={(double)x,(double)y,(double)z};
            double ref=fabs(a*x+b*y+c*z-h)/sqrt(a*a+b*b+c*c);
            if(!project(p,s,q,d)||fabs(d-ref)>1e-8||fabs(a*q.x+b*q.y+c*q.z-h)>1e-8)ok=false;
            Point v={1,2,3};
            double k=(h-a*x-b*y-c*z)/(a+2*b+3*c);
            if(line_plane(p,v,s,q)!=1||len(q-(p+v*k))>1e-8)ok=false;
            double ref2=sqrt((y-2*x)*(y-2*x)+(z-3*x)*(z-3*x)+(2*z-3*y)*(2*z-3*y))/sqrt(14.0);
            if(fabs(point_line(p,{0,0,0},v)-ref2)>1e-8)ok=false;
            Point w={b,-a,0},u=cross(v,w);
            if(fabs(dot(u,v))>EPS||fabs(dot(u,w))>EPS)ok=false;
        }
    }
    printf("三维几何基础: %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
