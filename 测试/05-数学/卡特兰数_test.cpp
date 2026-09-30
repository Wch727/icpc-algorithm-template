// 卡特兰数 的测试与对拍代码
// 模板本体：05-数学/卡特兰数.cpp
#include "../../05-数学/卡特兰数.cpp"

int main()
{
    int bad=0;
    int M=2000;
    C_init(2*M+5,1000000007);
    // 1) 组合公式 / 出栈 DP / 朴素递推 三者对拍
    for(int n=0;n<=500;n++)
    {
        ll x=cat_rec(n),y=cat_naive(n),z=cat_dp(n);
        if(x!=y||y!=z)
        {
            bad++;
            if(bad<=5)printf("MISMATCH n=%d rec=%lld naive=%lld dp=%lld\n",n,x,y,z);
        }
    }
    // 2) 小 n 与精确值比对（不取模，n<=18 时卡特兰数在 long long 内）
    ll exact[20],c[20];
    c[0]=1;
    for(int i=1;i<=18;i++)
    {
        c[i]=0;
        for(int j=0;j<i;j++)c[i]+=c[j]*c[i-1-j];
    }
    for(int n=0;n<=18;n++)
    {
        exact[n]=c[n];
        if(cat_rec(n)!=(c[n]%mod))bad++;
        if(cat_naive(n)!=(c[n]%mod))bad++;
        if(cat_dp(n)!=(c[n]%mod))bad++;
    }
    printf("Cat(0..10) = ");
    for(int n=0;n<=10;n++)printf("%lld ",cat_rec(n));
    printf("\nexact  Cat(10) = %lld, Cat(18) = %lld\n",exact[10],exact[18]);
    printf("Cat(1000) = %lld\n",cat_rec(1000));
    printf("stack out-sequence n=3 : %lld (expect 5)\n",cat_dp(3));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1044 输入 3 -> 5，输入 18 -> 477638700
// 样例：P1720 斐波那契数列，与卡特兰数无关但同属递推
// 边界：Cat[0]=1；组合公式要求 n+1 < mod 且 mod 为素数

/*
自测记录：
  1) n=0..500：组合公式与朴素 O(n^2) 递推、出栈序列 DP 三种方法对拍；
  2) n=0..18 与不取模的精确卡特兰数比对。
  关键点：出栈 DP 的一行内 j 必须逆序扫，否则同一行会重复转移。
*/
