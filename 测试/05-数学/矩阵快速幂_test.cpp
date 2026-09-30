// 矩阵快速幂 的测试与对拍代码
// 模板本体：05-数学/矩阵快速幂.cpp
#include "../../05-数学/矩阵快速幂.cpp"

int main()
{
    M=2;
    // 1) 小范围与朴素递推对拍
    int bad=0;
    for(ll n=0;n<=200;n++)
        if(fib(n)!=fib_naive(n))bad++;
    // 2) 大 n 用快速倍增公式 F(2k)=F(k)*(2F(k+1)-F(k)), F(2k+1)=F(k)^2+F(k+1)^2 对拍
    ll x=1,y=1;
    for(ll n=1;n<=500000;n++)
    {
        if(fib(n)!=(n==1?1:x))bad++;
        ll z=(x+y)%mod;
        x=y,y=z;
    }
    printf("fib(10) = %lld\n",fib(10));
    printf("fib(50) = %lld\n",fib(50));
    printf("fib(1e18) = %lld\n",fib(1000000000000000000LL));
    printf("fib(100) naive = %lld quick = %lld\n",fib_naive(100),fib(100));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1962 输入 10 -> 55；输入 1000000000000000000 -> 517691607

/*
自测记录：
  1) n <= 200 与 O(n) 朴素递推对拍；
  2) n <= 500000 与增量递推对拍（含大 n 前的全部值）。
*/
