// 李超树 的测试与对拍代码
// 模板本体：02-数据结构/李超树.cpp
#include "../../02-数据结构/李超树.cpp"

int main()
{
    mt19937 rng(20240513);
    LiChao tr,dis;
    bool ok=true;
    tr.init(-1000000000LL,1000000000LL);
    if(tr.query(0).c)ok=false;
    tr.insert(Line(0,0,1)),tr.insert(Line(1,0,1));
    tr.insert(Line(2,-3,1)),tr.insert(Line(-3,100,1));
    Line s=tr.query(100);
    if((lll)s.a*100+s.b!=197*s.c)ok=false;
    for(int T=1;T<=30;T++)
    {
        tr.init(-1000000000LL,1000000000LL);
        vector<ll> xs={-1000000000LL,0,1000000000LL};
        for(int i=1;i<=50;i++)xs.push_back((ll)(rng()%2000000001)-1000000000);
        dis.init(xs);
        vector<Line> a;
        for(int i=1;i<=40;i++)
        {
            s=Line((int)(rng()%201)-100,(int)(rng()%2001)-1000,rng()%7+1);
            a.push_back(s),tr.insert(s),dis.insert(s);
            for(ll x:xs)
            {
                Line best;
                for(Line z:a)if(better(z,best,x))best=z;
                Line u=tr.query(x),v=dis.query(x);
                if(better(u,best,x)||better(best,u,x)||better(v,best,x)||better(best,v,x))ok=false;
            }
        }
    }
    tr.init(5,5),tr.insert(Line(0,-7,3));
    if(tr.query(5).b!=-7)ok=false;
    printf("李超树 连续域/离散域/分数 对拍: %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
