// 组合数与Lucas定理 的测试与对拍代码
// 模板本体：05-数学/组合数与Lucas定理.cpp
#include "../../05-数学/组合数与Lucas定理.cpp"

ll kfac(ll n,ll p)
{
    ll r=0;
    while(n)n/=p,r+=n;
    return r;
}

ll pf(ll n,ll p)
{
    ll r=1;
    while(n)
    {
        ll t=n/p;
        // t! 的 (n/p) 段的完整阶乘 mod p
        for(ll i=1;i<=n%p;i++)r=r*i%p;
        if(t&1)r=(p-r)%p;// (p-1)! === -1 (mod p)
        n=t;
    }
    return r;
}

ll C_naive(ll n,ll m,ll p)
{
    if(m<0||m>n)return 0;
    ll e=kfac(n,p)-kfac(m,p)-kfac(n-m,p);
    if(e>0)return 0;// p 整除组合数
    ll num=pf(n,p);
    ll den=pf(m,p)*pf(n-m,p)%p;
    return num*qpow(den,p-2,p)%p;
}

int main()
{
    int bad=0;
    const ll P=1000000007;
    C_init(100005,P);
    // 1) C 与杨辉三角对拍
    static ll c[505][505];
    for(int i=0;i<=500;i++)
    {
        c[i][0]=1;
        for(int j=1;j<=i;j++)
            c[i][j]=(c[i-1][j-1]+(j<=i-1?c[i-1][j]:0))%P;
    }
    for(int i=0;i<=500;i++)
        for(int j=0;j<=i;j++)
            if(C(i,j,P)!=c[i][j])bad++;
    // 2) Lucas 小素数下与 C_naive 对拍（n,m 到 2e9）
    ll ps[6]={2,3,5,7,11,13};
    mt19937_64 rnd(20250303);
    for(int t=0;t<6;t++)
    {
        ll p=ps[t];
        C_init(p-1,p);// 每个小素数按其模数重新预处理，边界是 p-1
        for(int i=1;i<=2000;i++)
        {
            ll n=rnd()%2000000000LL,m=rnd()%2000000000LL;
            if(m>n)swap(n,m);
            if(Lucas(n,m,p)!=C_naive(n,m,p))
            {
                bad++;
                if(bad<=5)printf("MISMATCH p=%lld n=%lld m=%lld\n",p,n,m);
            }
        }
        // 小范围再与杨辉三角（mod p）对拍
        static ll s[105][105];
        for(int i=0;i<=100;i++)
        {
            s[i][0]=1;
            for(int j=1;j<=i;j++)s[i][j]=(s[i-1][j-1]+(j<=i-1?s[i-1][j]:0))%p;
        }
        for(int i=0;i<=100;i++)
            for(int j=0;j<=i;j++)
                if(Lucas(i,j,p)!=s[i][j])bad++;
    }
    // 3) 边界
    if(C(5,6,P)!=0||C(5,-1,P)!=0||C(-1,3,P)!=0)bad++;
    if(Lucas(5,6,3)!=0||Lucas(5,-1,3)!=0)bad++;
    C_init(100005,P);
    printf("C(10,3) = %lld (expect 120)\n",C(10,3,P));
    printf("C(100,50) = %lld\n",C(100,50,P));
    printf("C(100000,50000) = %lld\n",C(100000,50000,P));
    C_init(6,7);
    printf("Lucas(10,3,7) = %lld (expect 1)\n",Lucas(10,3,7));
    C_init(2,3);
    printf("Lucas(1e18,1e9,3) = %lld\n",Lucas(1000000000000000000LL,1000000000LL,3));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P3807 输入 (1 2 5) -> 3, (2 1 5) -> 3（n,m 到 1e18，p<=1e5）
// 样例：P2822 求 C(i,j) 是 k 的倍数个数，用杨辉三角离线前缀和
// 边界：m<0 或 m>n 返回 0；Lucas 要求 p 为素数且 p 不大（C 预处理到 p-1）

/*
自测记录：
  1) C(n,m) 与杨辉三角 0..500 全表对拍；
  2) p=2,3,5,7,11,13 时 Lucas 与按 p 因子抽离的暴力 C_naive 各 2000 组对拍（n,m 到 2e9），
     另与杨辉三角 0..100 对拍；
  3) 越界参数返回 0。
*/
