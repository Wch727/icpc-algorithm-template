// 多项式求逆与ln_exp 的测试与对拍代码
// 模板本体：05-数学/多项式求逆与ln_exp.cpp
#include "../../05-数学/多项式求逆与ln_exp.cpp"

// O(n^2) 的系数递推，独立验证三种运算

int main()
{
    int bad=inverse({1,1},3)!=vector<int>({1,P-1,1});
    mt19937 rnd(7231);
    for(int t=1;t<=80;t++)
    {
        int n=rnd()%100+1;
        vector<int> a(n),b(n),ln(n),ex(n),iv(n,1);
        for(int &x:a)x=rnd()%P;
        if(!a[0])a[0]=1;
        b[0]=qpow(a[0],P-2);
        for(int i=1;i<n;i++)
        {
            ll v=0;
            for(int j=1;j<=i;j++)v=(v+(ll)a[j]*b[i-j])%P;
            b[i]=(P-v)*b[0]%P;
        }
        bad+=inverse(a,n)!=b;
        a[0]=1;
        for(int i=2;i<n;i++)iv[i]=qpow(i,P-2);
        for(int i=1;i<n;i++)
        {
            ll v=(ll)i*a[i]%P;
            for(int j=1;j<i;j++)v=(v-(ll)j*ln[j]%P*a[i-j]%P+P)%P;
            ln[i]=v*iv[i]%P;
        }
        bad+=logarithm(a,n)!=ln;
        a[0]=0,ex[0]=1;
        for(int i=1;i<n;i++)
        {
            ll v=0;
            for(int j=1;j<=i;j++)v=(v+(ll)j*a[j]%P*ex[i-j])%P;
            ex[i]=v*iv[i]%P;
        }
        bad+=exponential(a,n)!=ex;
    }
    printf("多项式求逆与 ln/exp：%s\n",bad?"FAILED":"OK");
    if(bad)return 1;
    return 0;
}
