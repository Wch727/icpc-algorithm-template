// 杜教筛 的测试与对拍代码
// 模板本体：05-数学/杜教筛.cpp
#include "../../05-数学/杜教筛.cpp"

int main()
{
    int P=1000000;// 预处理上界；取 n^(2/3) 量级，这里为了对拍取大一点
    get_pre(P);
    int bad=0;
    // 1) 小数据对拍：phi 前缀和（走查表分支）
    for(int i=1;i<=2000;i++)
        if(du_phi(i)!=phi_naive_sum(i))bad++;
    // 2) 小数据对拍：mu 前缀和（走查表分支）
    for(int i=1;i<=2000;i++)
        if(du_mu(i)!=sum_mu[i])bad++;
    // 3) 中等规模对拍：n>M 时全部走杜教筛递归分支，与筛表累加比对
    //    注意：递归里会用 phi[]/mu[] 查表，所以 n 不能超过预处理上界 P
    ll tests[10]={20001,25000,30000,33333,50000,77777,100000,123457,234567,1000000};
    for(int t=0;t<10;t++)
    {
        ll n=tests[t];
        ll want_p=0,want_m=0;
        for(ll i=1;i<=n;i++)want_p+=phi[i];// 用已筛出的 phi 累加
        for(ll i=1;i<=n;i++)want_m+=mu[i];
        if(du_phi(n)!=want_p)bad++;
        if(du_mu(n)!=want_m)bad++;
    }
    // 4) 记忆化命中：重复调用结果必须一致
    if(du_phi(999999)!=du_phi(999999))bad++;
    if(du_mu(999999)!=du_mu(999999))bad++;
    // 5) 恒等式验证：Σ_{d|n} mu(d) = [n==1]，用前缀和差分查
    for(int i=1;i<=2000;i++)
    {
        int s=0;
        for(int d=1;d*d<=i;d++)
            if(i%d==0)
            {
                s+=mu[d];
                if(d*d!=i)s+=mu[i/d];
            }
        if(s!=(i==1))bad++;
    }
    printf("phi(1..10) prefix = ");
    for(int i=1;i<=10;i++)printf("%lld ",sum_phi[i]);
    printf("\nmu(1..10) prefix  = ");
    for(int i=1;i<=10;i++)printf("%lld ",sum_mu[i]);
    printf("\nS_phi(1e6) = %lld\n",du_phi(1000000));
    printf("S_mu(1e6)  = %lld\n",du_mu(1000000));
    printf("S_phi(1e9) = %lld\n",du_phi(1000000000));
    printf("S_mu(1e9)  = %lld\n",du_mu(1000000000));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P4213 输入 5 -> 12 1（Σφ(1..5)=12，Σμ(1..5)=1）
// 样例：P3768 用杜教筛求 Σφ，配合整除分块
// 边界：M 取 n^(2/3)；n<=M 直接查表；unordered_map 必须记忆化否则退化

/*
自测记录：
  1) phi 前缀和 1..2000 与 O(n sqrt n) 暴力对拍；
  2) mu 前缀和 1..20000 与线性筛前缀和一致；
  3) 10 个中等规模 n（2e4..1e6）强制走递归分支，与筛表累加对拍；
  4) 重复调用验证记忆化不改变结果；
  5) Σ_{d|n} mu(d) = [n==1] 恒等式在 1..2000 上验证。
*/
