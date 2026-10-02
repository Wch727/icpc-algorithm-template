#include "../../08-计算几何/圆的交点与公切线.cpp"
bool near(double a,double b){return fabs(a-b)<1e-7;}
void check_tangents()
{
    mt19937 g(132);
    for(int t=0;t<500;t++)
    {
        Circle a={{double(int(g()%201)-100)/10,double(int(g()%201)-100)/10},double(g()%41)/10};
        Circle b={{double(int(g()%201)-100)/10,double(int(g()%201)-100)/10},double(g()%41)/10};
        double d=norm(b.p-a.p);
        bool inf;
        auto lines=common_tangents(a,b,inf);
        if(d<=EPS){assert(inf==(fabs(a.r-b.r)<=EPS)&&lines.empty());continue;}
        int expected;
        if(a.r==0&&b.r==0)expected=1;
        else if(a.r==0||b.r==0)
        {
            double r=max(a.r,b.r);
            expected=d>r+EPS?2:d<r-EPS?0:1;
        }
        else if(d>a.r+b.r+EPS)expected=4;
        else if(fabs(d-a.r-b.r)<=EPS)expected=3;
        else if(d>fabs(a.r-b.r)+EPS)expected=2;
        else if(fabs(d-fabs(a.r-b.r))<=EPS)expected=1;
        else expected=0;
        assert(!inf&&(int)lines.size()==expected);
        for(auto [p,q]:lines)
        {
            assert(near(norm(p-a.p),a.r)&&near(norm(q-b.p),b.r));
            Point v=q-p;
            if(norm(v)<EPS)v=turn(b.p-a.p);
            v=v*(1/norm(v));
            assert(near(dot(p-a.p,v),0)&&near(dot(q-b.p,v),0));
        }
        assert(common_tangents(b,a,inf).size()==lines.size()&&!inf);
    }
}

int main(){check_tangents();bool inf;Circle a={{0,0},2};for(auto [b,num,tan]:vector<tuple<Circle,int,int>>{{{{6,0},2},0,4},{{{4,0},2},1,3},{{{2,0},2},2,2},{{{1,0},1},1,1},{{{0.5,0},1},0,0}}){auto pts=circle_circle(a,b,inf);assert(!inf&&int(pts.size())==num);for(auto p:pts)assert(near(norm(p-a.p),a.r)&&near(norm(p-b.p),b.r));auto lines=common_tangents(a,b,inf);assert(!inf&&int(lines.size())==tan);for(auto [p,q]:lines){assert(near(norm(p-a.p),a.r)&&near(norm(q-b.p),b.r));Point v=q-p;if(norm(v)<EPS)v=turn(b.p-a.p);assert(near(dot(p-a.p,v),0)&&near(dot(q-b.p,v),0));}}assert(circle_circle(a,a,inf).empty()&&inf);assert(common_tangents(a,a,inf).empty()&&inf);assert(circle_circle({{0,0},0},{{0,0},0},inf).size()==1&&!inf);assert(common_tangents({{0,0},0},{{2,0},0},inf).size()==1&&!inf);assert(circle_line(a,{-3,0},{3,0}).size()==2);assert(circle_line(a,{-3,2},{3,2}).size()==1);assert(circle_line(a,{-3,3},{3,3}).empty());mt19937 g(131);for(int z=0;z<1000;z++){Circle b={{double(int(g()%201)-100)/10,double(int(g()%201)-100)/10},double(1+g()%40)/10};Point p={double(int(g()%201)-100),double(int(g()%201)-100)},q=p+Point{2,3};for(auto x:circle_line(b,p,q)){assert(near(norm(x-b.p),b.r));assert(near(dot(x-p,turn(q-p)),0));}for(auto x:circle_circle(a,b,inf))assert(near(norm(x-a.p),a.r)&&near(norm(x-b.p),b.r));}cout<<"circle OK\n";}
