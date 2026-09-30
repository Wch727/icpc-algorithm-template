// FFT 的测试与对拍代码
// 模板本体：05-数学/FFT.cpp
#include "../../05-数学/FFT.cpp"

int main()
{
    int bad=multiply({1,2},{3,4})!=vector<ll>({3,10,8});
    mt19937 rnd(197);
    for(int t=1;t<=100;t++)
    {
        vector<ll> a(rnd()%65+1),b(rnd()%65+1),c(a.size()+b.size()-1);
        for(ll &x:a)x=(int)(rnd()%101)-50;
        for(ll &x:b)x=(int)(rnd()%101)-50;
        for(int i=0;i<(int)a.size();i++)for(int j=0;j<(int)b.size();j++)c[i+j]+=a[i]*b[j];
        bad+=multiply(a,b)!=c;
    }
    printf("FFT：%s\n",bad?"FAILED":"OK");
    if(bad)return 1;
    return 0;
}
