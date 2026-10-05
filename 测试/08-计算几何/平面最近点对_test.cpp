// 平面最近点对 的测试与对拍代码
// 模板本体：08-计算几何/平面最近点对.cpp
#include "../../08-计算几何/平面最近点对.cpp"
double closest_pair(vector<Point> arg_a){a=arg_a;return closest_pair();}
double solve(vector<Point> &arg_a,vector<Point> &arg_tmp,int l,int r){a=arg_a;tmp=arg_tmp;double result=solve(l,r);arg_a=a;arg_tmp=tmp;return result;}

double brute(vector<Point> a)
{
    double ans=numeric_limits<double>::infinity();
    for(int i=0;i<(int)a.size();i++)for(int j=0;j<i;j++)ans=min(ans,dis2(a[i],a[j]));
    return sqrt(ans);
}
int main()
{
    srand(20260930);
    bool ok=fabs(closest_pair({{0,0},{3,4}})-5)<1e-9;
    ok&=isinf(closest_pair({}));
    ok&=closest_pair({{1,2},{1,2}})==0;
    for(int t=1;t<=300;t++)
    {
        vector<Point> a;
        for(int i=0,n=rand()%100+2;i<n;i++)a.push_back({(double)(t%4?rand()%101-50:0),(double)(rand()%101-50)});
        if(fabs(closest_pair(a)-brute(a))>1e-9)ok=false;
    }
    printf("平面最近点对: %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
