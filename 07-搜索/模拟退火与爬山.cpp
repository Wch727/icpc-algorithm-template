#include<bits/stdc++.h>
using namespace std;

struct Point
{
    double x,y;
};

// 示例目标：有界区域中最小化凸二次函数；通用启发式不保证全局最优
// 更换目标函数后需同步调整扰动尺度、温度和迭代次数
struct Optimizer
{
    double cx,cy;
    mt19937 rng;
    Optimizer(double x,double y,unsigned seed):cx(x),cy(y),rng(seed){}
    double value(Point p)
    {
        return (p.x-cx)*(p.x-cx)+2*(p.y-cy)*(p.y-cy);
    }

    double rnd()
    {
        return uniform_real_distribution<double>(0,1)(rng);
    }
    Point clamp(Point p)
    {
        p.x=max(-10.0,min(10.0,p.x)),p.y=max(-10.0,min(10.0,p.y));
        return p;
    }
    Point anneal(Point cur)
    {
        Point best=cur;
        for(double temp=10;temp>1e-5;temp*=0.98)
            for(int i=1;i<=20;i++)
            {
                Point p=clamp({cur.x+(rnd()*2-1)*temp,cur.y+(rnd()*2-1)*temp});
                double delta=value(p)-value(cur);
                if(delta<=0||rnd()<exp(-delta/temp))cur=p;
                if(value(cur)<value(best))best=cur;
            }
        return best;
    }
    Point climb(Point cur)
    {
        for(double step=10;step>1e-8;step*=0.5)
        {
            bool change=true;
            while(change)
            {
                change=false;
                for(int dx=-1;dx<=1;dx++)
                    for(int dy=-1;dy<=1;dy++)
                    {
                        Point p=clamp({cur.x+dx*step,cur.y+dy*step});
                        if(value(p)+1e-18<value(cur))cur=p,change=true;
                    }
            }
        }
        return cur;
    }
};
