// 拉格朗日插值 的测试与对拍代码
// 模板本体：05-数学/拉格朗日插值.cpp
#include "../../05-数学/拉格朗日插值.cpp"

int main()
{
    int bad=interpolate({0,1,4},5)!=25;
    mt19937 rnd(771);
    for(int t=1;t<=100;t++)
    {
        int n=rnd()%40;
        vector<int> a(n+1),y(n+1);
        for(int &v:a)v=rnd()%P;
        for(int i=0;i<=n;i++)y[i]=eval(a,i);
        for(int q=0;q<20;q++)
        {
            ll x=q<3?q:(ll)rnd()-2000000000LL;
            bad+=interpolate(y,x)!=eval(a,x);
        }
    }
    printf("拉格朗日插值：%s\n",bad?"FAILED":"OK");
    if(bad)return 1;
    return 0;
}
