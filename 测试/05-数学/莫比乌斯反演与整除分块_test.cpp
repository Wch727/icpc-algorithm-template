// 莫比乌斯反演与整除分块 的测试与对拍代码
// 模板本体：05-数学/莫比乌斯反演与整除分块.cpp
#include "../../05-数学/莫比乌斯反演与整除分块.cpp"

int main()
{
    int M=3000;
    get_mu(M);
    int bad=0;
    // 1) mu 与定义对拍（枚举平方因子）
    for(int i=1;i<=M;i++)
    {
        int x=i,c=0,sq=0;
        for(int j=2;j*j<=x;j++)
            if(x%j==0)
            {
                x/=j,c++;// 找到一个不同的质因子
                if(x%j==0)sq=1;// 还能被 j 整除 -> 有平方因子
                while(x%j==0)x/=j;// 把 j 全部除掉
            }
        if(x>1)c++;
        int want=sq?0:((c&1)?-1:1);
        if(mu[i]!=want)bad++;
    }
    // 2) ΣΣ[gcd==1] 与暴力对拍
    for(int a=1;a<=60;a++)
        for(int b=1;b<=60;b++)
            if(sum_coprime(a,b)!=sum_coprime_naive(a,b))bad++;
    // 3) Σ mu[i]*(n/i) 对拍
    for(int i=1;i<=M;i++)
        if(sum_mu_div(i)!=sum_mu_div_naive(i))bad++;
    // 4) 整除分块 Σ floor(n/i) 与暴力对拍
    for(int i=1;i<=M;i++)
    {
        ll s=0;
        for(int j=1;j<=i;j++)s+=i/j;
        if(sum_floor(i)!=s)bad++;
    }
    // 5) 大规模边界值：sum_mu 前缀和 + 整除分块一致性
    get_mu(1000000);
    if(sum_mu_div(1000000)!=sum_mu_div_naive(1000000))bad++;
    if(sum_mu[1000000]!=mertens_naive(1000000))bad++;
    // 6) sum_mu_mul_g 用 g(x)=x 检验：Σ mu[i]*(n/i) 与整除分块版一致
    static ll g[N];
    for(int i=1;i<=M;i++)g[i]=i;
    for(int i=1;i<=M;i++)
        if(sum_mu_mul_g(i,g,M)!=sum_mu_div_naive(i))bad++;
    printf("mu(1..10) = ");
    for(int i=1;i<=10;i++)printf("%d ",mu[i]);
    printf("\nsum_mu_div(1e6) = %lld\n",sum_mu_div(1000000));
    printf("M(1e6) = %lld\n",sum_mu[1000000]);
    printf("coprime pairs (1..100) = %lld\n",sum_coprime(100,100));
    printf("sum_floor(1e9) = %lld\n",sum_floor(1000000000));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P2158 输入 4 -> 9（n=4 时能看到 9 个点，即 2*Σφ+1）
// 样例：P2568 输入 4 -> 4（素数对 (2,2),(2,3),(3,2) 等）
// 边界：mu[1]=1；n=1 时整除分块只剩 l=r=1；sum_mu 用 ll 防溢出

/*
自测记录：
  1) 1..3000 的 mu 与「枚举平方因子」定义对拍；
  2) 60x60 以内 ΣΣ[gcd==1] 线性筛版本与 O(n^2) 暴力对拍；
  3) 1..3000 的 Σ mu[i]*(n/i) 整除分块与 O(n) 枚举对拍；
  4) 1..3000 的 Σ floor(n/i) 整除分块与二重暴力对拍；
  5) n=1e6 处前缀和、整除分块、通项三种算法结果一致。
*/
