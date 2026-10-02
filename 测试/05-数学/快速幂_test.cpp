// 快速幂 的测试与对拍代码
// 模板本体：05-数学/快速幂.cpp
#include "../../05-数学/快速幂.cpp"

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        ll p=LLONG_MAX-24;
        assert(qpow(2,0,1)==0&&qpow(-2,3,5)==2);
        assert(qmul(p-1,p-1,p)==1&&qmul128(-2,3,5)==4);
        assert(qpow_safe(p-1,2,p)==1&&qpow128(p-1,2,p)==1);
    }

    // 1) 小数据与暴力逐次相乘对拍
    int bad=0;
    for(int a=0;a<=30;a++)
        for(int e=0;e<=30;e++)
        {
            ll t=1;
            for(int i=1;i<=e;i++)t=t*a%1000000007;
            if(qpow(a,e,1000000007)!=t)bad++;
            if(qpow_safe(a,e,1000000007)!=t)bad++;
        }
    // 2) 大指数，与快速乘版互相对拍
    mt19937_64 rnd(20050101);
    for(int i=1;i<=2000;i++)
    {
        ll m=rnd()%1000000000+2;
        ll a=rnd()%m;
        ll e=rnd()%1000000000000000000ULL;
        if(qpow(a,e,m)!=qpow128(a,e,m))bad++;
        if(qpow_safe(a,e,m)!=qpow128(a,e,m))bad++;
    }
    // 3) 快速乘对大数取模（mod 接近 2^62，普通乘法必溢出）
    ll M=(1LL<<62)-1;
    for(int i=1;i<=2000;i++)
    {
        ll a=rnd()%M,b=rnd()%M;
        if(qmul(a,b,M)!=qmul128(a,b,M))bad++;
    }
    printf("qpow 2^10=1024 -> %lld\n",qpow(2,10,1000000007));
    printf("qpow 2^1000000005 mod 1e9+7 = %lld\n",qpow(2,1000000005,1000000007));
    printf("qmul 1234567890123456789*9876543210987654321 mod 2^62-1 = %lld\n",
           qmul(1234567890123456789LL,9876543210987654321ULL%M,M));
    printf("qmul(7,M-1,M) = %lld, via __int128 = %lld\n",qmul(7,M-1,M),qmul128(7,M-1,M));
    printf("qpow_safe(3,1e18,1e9+7) = %lld\n",qpow_safe(3,1000000000000000000LL,1000000007));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1226 > 2 10 9 -> 2^10 mod 9=7
// 边界：mod=1 时结果恒 0；指数 0 时结果为 1%mod

/*
自测记录：
  1) a,e <= 30 与朴素循环取模对拍；
  2) 随机 mod/a/e（e 到 1e18）与 __int128 版本对拍；
  3) 快速乘在 mod=2^62-1 上与 __int128 对拍。
*/
